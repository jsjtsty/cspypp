#include "general.h"
#include "Plugin.h"
#include <Windows.h>
using namespace std;

#define CS_PLUGIN_OK 0

extern "C" {
	typedef int32_t (*_GlobalInitialization)(uint32_t threadId);
	typedef uint32_t (*_GetProtrolVersion)();
	typedef int32_t (*_CheckProtrolVersion)(uint32_t currentProtrolVersion);
	typedef uint32_t (*_GetPluginVersion)();
	typedef SYSTEMTIME (*_GetReleaseTime)();
	typedef void (*_GetCompanyName)(wchar_t* buffer, uint32_t bufferSize);
	typedef void (*_GetPluginName)(wchar_t* buffer, uint32_t bufferSize);
	typedef void (*_GetPluginVersionName)(wchar_t* buffer, uint32_t bufferSize);
}

Plugin::Plugin(const std::wstring_view path)
{
	this->path = path.data();
}

bool Plugin::init()
{
	if (hLibrary) {
		return true;
	}

	hLibrary = LoadLibraryW(path.c_str());
	if (!hLibrary) {
		return false;
	}

	if (!this->isInfoLoaded) {
		_GlobalInitialization GlobalInitialization = (_GlobalInitialization)GetProcAddress((HMODULE)hLibrary, "CSPYPP_GlobalInitialization");
		_GetProtrolVersion GetProtrolVersion = (_GetProtrolVersion)GetProcAddress((HMODULE)hLibrary, "CSPYPP_GetProtrolVersion");
		_CheckProtrolVersion CheckProtrolVersion = (_CheckProtrolVersion)GetProcAddress((HMODULE)hLibrary, "CSPYPP_CheckProtrolVersion");
		_GetPluginVersion GetPluginVersion = (_GetPluginVersion)GetProcAddress((HMODULE)hLibrary, "CSPYPP_GetPluginVersion");
		_GetReleaseTime GetReleaseTime = (_GetReleaseTime)GetProcAddress((HMODULE)hLibrary, "CSPYPP_GetReleaseTime");
		_GetCompanyName GetCompanyName = (_GetCompanyName)GetProcAddress((HMODULE)hLibrary, "CSPYPP_GetCompanyName");
		_GetPluginName GetPluginName = (_GetPluginName)GetProcAddress((HMODULE)hLibrary, "CSPYPP_GetPluginName");
		_GetPluginVersionName GetPluginVersionName = (_GetPluginVersionName)GetProcAddress((HMODULE)hLibrary, "CSPYPP_GetPluginVersionName");

		if (!(GlobalInitialization && GetProtrolVersion && CheckProtrolVersion && GetPluginVersion && GetReleaseTime
			&& GetCompanyName && GetPluginName && GetPluginVersionName)) {
			FreeLibrary((HMODULE)hLibrary);
			return false;
		}

		if (GlobalInitialization(GetCurrentThreadId()) != CS_PLUGIN_OK) {
			FreeLibrary((HMODULE)hLibrary);
			return false;
		}

		if (CheckProtrolVersion(CS_PLUGIN_PROTROL_VERSION) != CS_PLUGIN_OK) {
			FreeLibrary((HMODULE)hLibrary);
			return false;
		}

		this->protrolVersion = GetPluginVersion();
		this->version = GetPluginVersion();
		this->releaseTime = GetReleaseTime();

		wchar_t* buffer = new wchar_t[256];
		GetCompanyName(buffer, 256);
		this->companyName = buffer;
		GetPluginName(buffer, 256);
		this->pluginName = buffer;
		GetPluginVersionName(buffer, 256);
		this->versionName = buffer;
		delete[] buffer;

		this->isInfoLoaded = true;
	}
	return true;
}

bool Plugin::free()
{
	if (hLibrary) {
		FreeLibrary((HMODULE)hLibrary);
		hLibrary = nullptr;
	}

	return true;
}

uint32_t Plugin::getProtrolVersion()
{
	if (!this->isInfoLoaded) {
		if (!this->init()) {
			return 0;
		}
	}

	return this->protrolVersion;
}

uint32_t Plugin::getVersion()
{
	if (!this->isInfoLoaded) {
		if (!this->init()) {
			return 0;
		}
	}

	return this->version;
}

Time Plugin::getReleaseTime()
{
	if (!this->isInfoLoaded) {
		if (!this->init()) {
			return Time();
		}
	}

	return this->releaseTime;
}

std::wstring Plugin::getPath()
{
	return this->path;
}

std::wstring Plugin::getName()
{
	if (!this->isInfoLoaded) {
		if (!this->init()) {
			return std::wstring();
		}
	}

	return pluginName;
}

std::wstring Plugin::getVersionName()
{
	if (!this->isInfoLoaded) {
		if (!this->init()) {
			return std::wstring();
		}
	}

	return this->versionName;
}

std::wstring Plugin::getCompanyName()
{
	if (!this->isInfoLoaded) {
		if (!this->init()) {
			return std::wstring();
		}
	}

	return this->companyName;
}
