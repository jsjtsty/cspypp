#include "ProgramUpdate.h"
#include <string>
#include <memory>
#include <fstream>
#include <vector>
#include <regex>
#include <algorithm>
#include <filesystem>
#include <curl/curl.h>
#include <curl/easy.h>
#include <json/json.h>
#include <unzipper.h>
#include "StringExt.h"
#include "JsonExt.h"
#include "LogSupport.h"
#include "Version.h"
#include "Hasher.h"
using namespace std;
using namespace zipper;

namespace {
	ProgramUpdate* pUpdate = nullptr;

	size_t WriteFileCallback(char* ptr, size_t size, size_t nmemb, void* userdata) {
		return fwrite(ptr, size, nmemb, (FILE*)userdata);
	}

	DWORD WINAPI autoUpdate(LPVOID) {
		PostThreadMessage(CS_MAIN_THREADID, WM_UPDATE_START, GetCurrentThreadId(), NULL);
		UpdateInformation info = ProgramUpdate::getInstance()->checkForUpdate();
		LOG_INFO(L"Check done.");
		if (info.code != UpdateCode::CS_UPDATE_OK) {
			LOG_ERROR(wstring(L"Update failed: ") + ProgramUpdate::getErrorString(info.code) + L" Return code: " + to_wstring((unsigned int)info.code));
			PostThreadMessage(CS_MAIN_THREADID, WM_UPDATE_CHECKEND, reinterpret_cast<WPARAM>(new UpdateInformation(info)), 0);
			return 0;
		}
		if (!info.available) {
			LOG_INFO(L"Update failed: Update not available.");
			PostThreadMessage(CS_MAIN_THREADID, WM_UPDATE_CHECKEND, reinterpret_cast<WPARAM>(new UpdateInformation(info)), 0);
			return 0;
		}
		if (info.currentVersion < Version::currentVersion) {
			LOG_INFO(L"Update failed: Update not available.");
			PostThreadMessage(CS_MAIN_THREADID, WM_UPDATE_CHECKEND, reinterpret_cast<WPARAM>(new UpdateInformation(info)), 0);
			return 0;
		}

		UpdateResult dlResult = { CS_UPDATE_OK,0 };		if (dlResult.code != CS_UPDATE_OK) {
			LOG_INFO(L"Update failed: Update download failed.");
			PostThreadMessage(CS_MAIN_THREADID, WM_UPDATE_CHECKEND, reinterpret_cast<WPARAM>(new UpdateInformation(info)), 0);
			return 0;
		}

		PostThreadMessage(CS_MAIN_THREADID, WM_UPDATE_CHECKEND, reinterpret_cast<WPARAM>(new UpdateInformation(info)), 1);
		return 0;
	}
}

ProgramUpdate* ProgramUpdate::getInstance()
{
	if (!pUpdate) {
		pUpdate = new ProgramUpdate();
	}
	return pUpdate;
}

ProgramUpdate::ProgramUpdate()
{
	
}

void ProgramUpdate::autoUpdate() const
{
	DWORD ThreadID;
	CreateThread(NULL, 0, ::autoUpdate, nullptr, 0, &ThreadID);
}

UpdateResult ProgramUpdate::downloadUpdate(const Version::Package& package) const
{
	auto file_deleter = [](FILE* ptr) {
		fclose(ptr);
	};
	auto curl_deleter = [](CURL* ptr) {
		curl_easy_cleanup(ptr);
	};

	unique_ptr<CURL, decltype(curl_deleter)> curl(curl_easy_init(), curl_deleter);
	curl_easy_setopt(curl.get(), CURLOPT_TIMEOUT, 2400);
	curl_easy_setopt(curl.get(), CURLOPT_URL, to_string(package.href).c_str());
	if (filesystem::exists(L"cacert.pem")) {
		curl_easy_setopt(curl.get(), CURLOPT_SSL_VERIFYPEER, 0);
		curl_easy_setopt(curl.get(), CURLOPT_SSL_VERIFYHOST, 0);
	}
	else {
		curl_easy_setopt(curl.get(), CURLOPT_SSL_VERIFYPEER, 0);
		curl_easy_setopt(curl.get(), CURLOPT_SSL_VERIFYHOST, 2);
		curl_easy_setopt(curl.get(), CURLOPT_CAINFO, "cacert.pem");
	}

	UpdateResult result = { CS_UPDATE_OK,0 };
	return result;

	for (int tryCount = 1; tryCount <= 3; ++tryCount) {
		FILE* temp;
		errno_t fileErrorCode = _wfopen_s(&temp, L"update.zip", L"wb");
		if (fileErrorCode != 0) {
			fclose(temp);
			if (tryCount <= 3) {
				continue;
			}
			else {
				result.code = CS_UPDATE_FILE_ERROR;
				result.extraMessage = fileErrorCode;
				break;
			}
		}
		unique_ptr<FILE, decltype(file_deleter)> stream(temp, file_deleter);
		temp = nullptr;

		curl_easy_setopt(curl.get(), CURLOPT_WRITEDATA, stream.get());
		curl_easy_setopt(curl.get(), CURLOPT_WRITEFUNCTION, WriteFileCallback);
		CURLcode res = curl_easy_perform(curl.get());

		if (res != CURLE_OK) {
			wstring errmsg = to_wstring(curl_easy_strerror(res));
			if (tryCount < 3) {
				errmsg = L"Network error: Download update failed. Retry count: " + to_wstring(tryCount) + L". Error message: " + errmsg;
				LOG_ERROR(errmsg);
				continue;
			}
			else {
				errmsg = L"Network error: Update failed." + to_wstring(tryCount) + L". Error message: " + errmsg;
				LOG_ERROR(errmsg);
				result.code = CS_UPDATE_CONNECT_FAILED;
				result.extraMessage = res;
				break;
			}
		}

		stream.reset();
		Hash hash = hash_file(L"update.zip");
		if (hash != package.sha256) {
			if (tryCount < 3) {
				continue;
			}
			else {
				LOG_ERROR(L"Update failed: Update file check failed.");
				result.code = CS_UPDATE_INVALID_FILE;
				break;
			}
		}
		break;
	}
	
	return result;
}

UpdateResult ProgramUpdate::update() const
{
	UpdateResult result = { CS_UPDATE_OK,0 };
	FILE* stream;
	errno_t err = _wfopen_s(&stream, L"update.zip", L"rb");
	if (err != 0) {
		if (stream) {
			fclose(stream);
		}
		result.code = CS_UPDATE_FILE_ERROR;
		result.extraMessage = err;
		return result;
	}
	fclose(stream);

	Unzipper unz("update.zip");
	unz.extract();
	unz.close();

	if (filesystem::exists(L"update.exe")) {
		STARTUPINFO si = { sizeof(STARTUPINFO) };
		CreateProcessW(L"update.exe", NULL, NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, NULL);
		PostQuitMessage(0);
		return { CS_UPDATE_OK,0 };
	}
	else {
		LOG_ERROR(L"fatal: update failed: cannot find update.exe. abort.");
		return { CS_UPDATE_PROGRAM_LOST,0 };
	}
}

UpdateInformation ProgramUpdate::checkForUpdate() const
{
	bool curVerAvail = false;

	UpdateInformation info;
	info.available = false;
	info.code = CS_UPDATE_OK;

	string updateCfgUrl = ""; // TODO: URL
	
	auto file_deleter = [](FILE* ptr) {
		fclose(ptr);
	};
	auto curl_deleter = [](CURL* ptr) {
		curl_easy_cleanup(ptr);
	};

	unique_ptr<CURL, decltype(curl_deleter)> curl(curl_easy_init(), curl_deleter);
	curl_easy_setopt(curl.get(), CURLOPT_TIMEOUT, 60);
	if (filesystem::exists(L"cacert.pem")) {
		curl_easy_setopt(curl.get(), CURLOPT_SSL_VERIFYPEER, 0);
		curl_easy_setopt(curl.get(), CURLOPT_SSL_VERIFYHOST, 0);
	}
	else {
		curl_easy_setopt(curl.get(), CURLOPT_SSL_VERIFYPEER, 0);
		curl_easy_setopt(curl.get(), CURLOPT_SSL_VERIFYHOST, 2);
		curl_easy_setopt(curl.get(), CURLOPT_CAINFO, "cacert.pem");
	}

	for (int tryCount = 1; tryCount <= 3; ++tryCount) {
		FILE* temp;
		errno_t fileErrorCode = _wfopen_s(&temp, L"update-cfg.json", L"w");
		if (fileErrorCode != 0) {
			fclose(temp);
			if (tryCount <= 3) {
				continue;
			}
			else {
				info.code = CS_UPDATE_FILE_ERROR;
				return info;
			}
		}
		unique_ptr<FILE, decltype(file_deleter)> stream(temp, file_deleter);
		temp = nullptr;
		curl_easy_setopt(curl.get(), CURLOPT_URL, updateCfgUrl.c_str());
		curl_easy_setopt(curl.get(), CURLOPT_WRITEDATA, stream.get());
		curl_easy_setopt(curl.get(), CURLOPT_WRITEFUNCTION, WriteFileCallback);

		CURLcode code = curl_easy_perform(curl.get());
		if (code != CURLE_OK) {
			wstring errmsg = to_wstring(curl_easy_strerror(code));
			if (tryCount < 3) {
				errmsg = L"Network error: Update failed. Retry count: " + to_wstring(tryCount) + L". Error message: " + errmsg;
				LOG_ERROR(errmsg);
				continue;
			}
			else {
				errmsg = L"Network error: Update failed." + to_wstring(tryCount) + L". Error message: " + errmsg;
				LOG_ERROR(errmsg);
				info.code = CS_UPDATE_CONNECT_FAILED;
				return info;
			}
		}
		stream.reset();
		
		Json::Value value;
		ifstream ifs(L"update-cfg.json");
		try {
			ifs >> value;
		}
		catch(Json::RuntimeError e) {
			string s = e.what();
			throw;
		}
		ifs.close();

		if (!value.isMember("minimumVersion")) {
			info.code = CS_UPDATE_INVALID_CFGFILE;
			return info;
		}
		if (!value["minimumVersion"].isUInt()) {
			info.code = CS_UPDATE_INVALID_CFGFILE;
			return info;
		}
		unsigned int minimumVersion = value["minimumVersion"].asUInt();
		if (minimumVersion > CS_VERSION_BUILD) {
			if (!value.isMember("compatHref")) {
				info.code = CS_UPDATE_OLD_VERSION;
				return info;
			}
			if (!value["compatHref"].isObject()) {
				info.code = CS_UPDATE_INVALID_CFGFILE;
				return info;
			}
			auto& compatHref = value["compatHref"];

			Json::Value::Members members = compatHref.getMemberNames();
			vector<unsigned int> compatVersionName;
			for (const Json::String& member : members) {
				int ver = stoi(member);
				if (ver != 0) {
					compatVersionName.push_back(ver);
				}
			}

			sort(compatVersionName.begin(), compatVersionName.end());

			if (compatVersionName.size() == 0) {
				info.code = CS_UPDATE_OLD_VERSION;
				return info;
			}
			if (compatVersionName[0] > CS_VERSION_BUILD) {
				info.code = CS_UPDATE_OLD_VERSION;
				return info;
			}

			int result = compatVersionName[0];
			for (auto it = compatVersionName.cbegin() + 1; it != compatVersionName.cend(); ++it) {
				if (*it <= CS_VERSION_BUILD) {
					result = *it;
				}
				else {
					break;
				}
			}

			auto& href = compatHref[to_string(result)];
			if (!href.isString()) {
				info.code = CS_UPDATE_INVALID_CFGFILE;
				return info;
			}
			updateCfgUrl = href.asString();
			LOG_INFO(L"Application Update: Redirect to compatHref[" + to_wstring(result) + L"] - " + to_wstring(updateCfgUrl));
			--tryCount;
			continue;
		}

		if (value.isMember("href")) {
			auto& href = value["href"];
			if (href.isObject()) {
				if (href.isMember("enabled") && href.isMember("destination")) {
					auto &enabled = href["enabled"], &destination = href["destination"];
					if (enabled.isBool() && destination.isString()) {
						if (enabled.asBool()) {
							updateCfgUrl = destination.asString();
							LOG_INFO(L"Application Update: Redirect to " + to_wstring(updateCfgUrl));
							--tryCount;
							continue;
						}
					}
				}
			}
		}

		if (value.isMember("currentVersion")) {
			auto& currentVersion = value["currentVersion"];
			curVerAvail = checkVersion(currentVersion, info, info.currentVersion);
			if (info.code != CS_UPDATE_OK) {
				return info;
			}
		}

		if (value.isMember("stableVersion")) {
			auto& currentVersion = value["stableVersion"];
			curVerAvail = checkVersion(currentVersion, info, info.stableVersion);
			if (info.code != CS_UPDATE_OK) {
				return info;
			}
		}

		break;
	}
	info.available = true;
	return info;
}

const wchar_t * ProgramUpdate::getErrorString(UpdateCode code)
{
	const wchar_t* res = nullptr;
	switch (code) {
	case CS_UPDATE_OK:
		res = L"No error.";
		break;
	case CS_UPDATE_CONNECT_FAILED:
		res = L"Connect failed.";
		break;
	case CS_UPDATE_INVALID_CFGFILE:
		res = L"Invalid configuration file.";
		break;
	case CS_UPDATE_OLD_VERSION:
		res = L"This version is too old to update.";
		break;
	default:
		res = L"Unknown error.";
		break;
	}
	return res;
}

bool ProgramUpdate::checkVersion(Json::Value& version, UpdateInformation& info, Version& pver) const
{
	if (version.isMember("available")) {
		auto& available = version["available"];
		if (!available.isBool()) {
			info.code = CS_UPDATE_INVALID_CFGFILE;
			return false;
		}
		if (!available.asBool()) {
			return false;
		}
	}

	if (!version.isMember("version")) {
		return false;
	}
	if (!version["version"].isString()) {
		info.code = CS_UPDATE_INVALID_CFGFILE;
		return false;
	}
	wstring versionStr = JsonExt::read_wstring(version["version"]);
	wregex reg(LR"RegexString([0-9]+\.[0-9]+\.[0-9]+)RegexString");
	if (!regex_match(versionStr, reg)) {
		info.code = CS_UPDATE_INVALID_CFGFILE;
		return false;
	}
	unsigned int major, minor, fix;
	swscanf_s(versionStr.c_str(), L"%u.%u.%u", &major, &minor, &fix);
	pver.setMajor(major);
	pver.setMinor(minor);
	pver.setFix(fix);

	if (!version.isMember("build")) {
		info.code = CS_UPDATE_INVALID_CFGFILE;
		return false;
	}
	if (!version["build"].isUInt()) {
		info.code = CS_UPDATE_INVALID_CFGFILE;
		return false;
	}
	unsigned int build = version["build"].asUInt();
	pver.setBuild(build);

	bool isPreview = false;
	if (version.isMember("preview")) {
		auto& preview = version["preview"];
		if (!preview.isBool()) {
			info.code = CS_UPDATE_INVALID_CFGFILE;
			return false;
		}
		isPreview = preview.asBool();
	}

	if (isPreview) {
		if (version.isMember("previewType")) {
			auto& previewType = version["previewType"];
			if (!previewType.isString()) {
				info.code = CS_UPDATE_INVALID_CFGFILE;
				return false;
			}
			wstring spType = JsonExt::read_wstring(previewType);
			Version::PreviewType type = Version::PreviewType::UNKNOWN;
			if (spType == L"Technical Preview") {
				type = Version::PreviewType::TECHNICAL_PREVIEW;
			}
			else if (spType == L"Insider Preview") {
				type = Version::PreviewType::INSIDER_PREVIEW;
			}
			else if (spType == L"Alpha") {
				type = Version::PreviewType::ALPHA;
			}
			else if (spType == L"Beta") {
				type = Version::PreviewType::BETA;
			}
			pver.setPreview(type);
		}
		else {
			pver.setPreview(Version::PreviewType::UNKNOWN);
		}
	}
	else {
		pver.setPreview(Version::PreviewType::RELEASE);
	}

	if (version.isMember("description")) {
		auto& description = version["description"];
		if (!description.isString()) {
			info.code = CS_UPDATE_INVALID_CFGFILE;
			return false;
		}
		pver.setDescription(JsonExt::read_wstring(description));
	}

	if (version.isMember("package")) {
		auto& package = version["package"];
		if (!package.isObject()) {
			info.code = CS_UPDATE_INVALID_CFGFILE;
			return false;
		}

		if (package.isMember("href") && package.isMember("sha256")) {
			auto &href = package["href"], &sha256 = package["sha256"];
			if (!(href.isString() && sha256.isString())) {
				info.code = CS_UPDATE_INVALID_CFGFILE;
				return false;
			}
			Hash hash;
			wstring s = JsonExt::read_wstring(sha256);
			if (!hash.parse(JsonExt::read_wstring(sha256))) {
				info.code = CS_UPDATE_INVALID_CFGFILE;
				return false;
			}
			pver.setPackage(JsonExt::read_wstring(href), hash);
		}
	}

	if (version.isMember("package64")) {
		auto& package64 = version["package64"];
		if (!package64.isObject()) {
			info.code = CS_UPDATE_INVALID_CFGFILE;
			return false;
		}

		if (package64.isMember("href") && package64.isMember("sha256")) {
			auto &href = package64["href"], &sha256 = package64["sha256"];
			if (!(href.isString() && sha256.isString())) {
				info.code = CS_UPDATE_INVALID_CFGFILE;
				return false;
			}
			Hash hash;
			if (!hash.parse(JsonExt::read_wstring(sha256))) {
				info.code = CS_UPDATE_INVALID_CFGFILE;
				return false;
			}
			pver.setPackage64(JsonExt::read_wstring(href), hash);
		}
	}

	return true;
}
