#include "general.h"
#include "Version.h"
#include <sstream>
using namespace std;

constexpr const uint8_t CV_DESCRIPTION = 0B00000001U;
constexpr const uint8_t CV_PACKAGE = 0B00000010U;
constexpr const uint8_t CV_PACKAGE64 = 0B00000100U;


Version Version::currentVersion = Version(CS_VERSION_MINOR, CS_VERSION_MINOR, CS_VERSION_FIX, CS_VERSION_BUILD, Version::PreviewType::TECHNICAL_PREVIEW);

Version::Version()
{
	major = 0; minor = 0; fix = 0; build = 0;
	previewType = PreviewType::UNKNOWN;
}

Version::Version(unsigned int major, unsigned int minor, unsigned int fix, unsigned int build) {
	this->major = major; this->minor = minor; this->fix = fix; this->build = build;
	this->previewType = PreviewType::UNKNOWN;
}

Version::Version(unsigned int major, unsigned int minor, unsigned int fix, unsigned int build, PreviewType type)
{
	this->major = major; this->minor = minor; this->fix = fix; this->build = build;
	this->previewType = type;
}

void Version::setPreview(PreviewType type)
{
	this->previewType = type;
}

bool Version::isPreview() const
{
	return this->previewType != PreviewType::RELEASE && this->previewType != PreviewType::UNKNOWN;
}

Version::PreviewType Version::getPreviewType() const
{
	return this->previewType;
}

wstring Version::getPreviewName() const
{
	wstring res;
	switch (this->previewType) {
	case PreviewType::UNKNOWN:
	case PreviewType::RELEASE:
		break;
	case PreviewType::TECHNICAL_PREVIEW:
		res = L"Technical Preview";
		break;
	case PreviewType::INSIDER_PREVIEW:
		res = L"Insider Preview";
		break;
	case PreviewType::ALPHA:
		res = L"Alpha";
		break;
	case PreviewType::BETA:
		res = L"Beta";
		break;
	}
	return res;
}

void Version::setMajor(unsigned int _val)
{
	major = _val;
}

void Version::setMinor(unsigned int _val)
{
	minor = _val;
}

void Version::setFix(unsigned int _val)
{
	fix = _val;
}

void Version::setBuild(unsigned int _val)
{
	build = _val;
}

unsigned int Version::getMajor() const
{
	return major;
}

unsigned int Version::getMinor() const
{
	return minor;
}

unsigned int Version::getFix() const
{
	return fix;
}

unsigned int Version::getBuild() const
{
	return build;
}

void Version::setPackage(Package pkg)
{
	package = pkg;
}

void Version::setPackage(const std::wstring & href, Hash sha256)
{
	package.available = true;
	package.href = href;
	package.sha256 = sha256;
}

void Version::setPackage64(Package pkg64)
{
	package64 = pkg64;
}

void Version::setPackage64(const std::wstring & href, Hash sha256)
{
	package64.available = true;
	package64.href = href;
	package64.sha256 = sha256;
}

Version::Package Version::getPackage() const
{
	return package;
}

Version::Package Version::getPackage64() const
{
	return package64;
}

wstring Version::getVersionName() const
{
	wstring res;
	wstringstream ss;
	ss << major << L"." << minor << L"." << fix;
	ss >> res;
	return res;
}

wstring Version::getFullVersionName() const
{
	wstring res;
	wstringstream ss;
	ss << major << L"." << minor << L"." << fix << L" ";
	if (isPreview()) {
		ss << getPreviewName() << L" ";
	}
	ss << L"Build " << build;
	ss >> res;
	return res;
}

void Version::setDescription(const wstring _val)
{
	description = _val;
}

wstring Version::getDescription() const
{
	return description;
}

size_t Version::getBinarySize() const
{
	size_t size = sizeof(uint8_t) + 4 * sizeof(uint32_t) + sizeof(PreviewType);
	if (description.size() != 0) {
		size += sizeof(uint32_t) + description.size() * sizeof(wchar_t);
	}
	if (package.available) {
		size += sizeof(uint32_t) + package.href.size() * sizeof(wchar_t) + 32;
	}
	if (package64.available) {
		size += sizeof(uint32_t) + package64.href.size() * sizeof(wchar_t) + 32;
	}
	return size;
}

size_t Version::getBinaryData(void* buffer) const
{
	unsigned char* ptr = reinterpret_cast<unsigned char*>(buffer);
	uint8_t flag = 0;

	if (description.size() != 0) {
		flag |= CV_DESCRIPTION;
	}
	if (package.available) {
		flag |= CV_PACKAGE;
	}
	if (package64.available) {
		flag |= CV_PACKAGE64;
	}

	*reinterpret_cast<uint8_t*>(ptr) = flag;
	ptr += sizeof(uint8_t);
	*reinterpret_cast<uint32_t*>(ptr) = major;
	ptr += sizeof(uint32_t);
	*reinterpret_cast<uint32_t*>(ptr) = minor;
	ptr += sizeof(uint32_t);
	*reinterpret_cast<uint32_t*>(ptr) = fix;
	ptr += sizeof(uint32_t);
	*reinterpret_cast<uint32_t*>(ptr) = build;
	ptr += sizeof(uint32_t);
	*reinterpret_cast<PreviewType*>(ptr) = previewType;
	ptr += sizeof(PreviewType);

	if (flag & CV_DESCRIPTION) {
		*reinterpret_cast<uint32_t*>(ptr) = (uint32_t)description.length();
		ptr += sizeof(uint32_t);
		memcpy(ptr, description.c_str(), description.length() * sizeof(wchar_t));
		ptr += sizeof(wchar_t) * description.length();
	}

	if (flag & CV_PACKAGE) {
		*reinterpret_cast<uint32_t*>(ptr) = (uint32_t)package.href.length();
		ptr += sizeof(uint32_t);
		memcpy(ptr, package.href.c_str(), package.href.length() * sizeof(wchar_t));
		ptr += sizeof(wchar_t) * package.href.length();
		memcpy(ptr, package.sha256, 32);
		ptr += 32;
	}

	if (flag & CV_PACKAGE64) {
		*reinterpret_cast<uint32_t*>(ptr) = (uint32_t)package64.href.length();
		ptr += sizeof(uint32_t);
		memcpy(ptr, package64.href.c_str(), package64.href.length() * sizeof(wchar_t));
		ptr += sizeof(wchar_t) * package64.href.length();
		memcpy(ptr, package64.sha256, 32);
		ptr += 32;
	}

	return getBinarySize();
}

bool Version::readBinaryData(const void* buffer)
{
	const unsigned char* ptr = reinterpret_cast<const unsigned char*>(buffer);

	uint8_t flag = *reinterpret_cast<const uint8_t*>(ptr);
	ptr += sizeof(uint8_t);
	major = *reinterpret_cast<const uint32_t*>(ptr);
	ptr += sizeof(uint32_t);
	minor = *reinterpret_cast<const uint32_t*>(ptr);
	ptr += sizeof(uint32_t);
	fix = *reinterpret_cast<const uint32_t*>(ptr);
	ptr += sizeof(uint32_t);
	build = *reinterpret_cast<const uint32_t*>(ptr);
	ptr += sizeof(uint32_t);
	previewType = *reinterpret_cast<const PreviewType*>(ptr);
	ptr += sizeof(PreviewType);
	
	uint32_t size;
	if (flag & CV_DESCRIPTION) {
		size = *reinterpret_cast<const uint32_t*>(ptr);
		ptr += sizeof(uint32_t);

		wchar_t* buffer = new wchar_t[(size_t)size + 1];
		memcpy(buffer, ptr, (size_t)size * 2);
		buffer[size] = 0;
		description = buffer;
		delete[] buffer;
		ptr += sizeof(wchar_t) * size;
	}

	if (flag & CV_PACKAGE) {
		package.available = true;

		size = *reinterpret_cast<const uint32_t*>(ptr);
		ptr += sizeof(uint32_t);

		wchar_t* buffer = new wchar_t[(size_t)size + 1];
		memcpy(buffer, ptr, (size_t)size * 2);
		buffer[size] = 0;
		package.href = buffer;
		delete[] buffer;
		ptr += sizeof(wchar_t) * size;

		memcpy(package.sha256, ptr, 32);
		ptr += 32;
	}

	if (flag & CV_PACKAGE64) {
		package64.available = true;

		size = *reinterpret_cast<const uint32_t*>(ptr);
		ptr += sizeof(uint32_t);

		wchar_t* buffer = new wchar_t[(size_t)size + 1];
		memcpy(buffer, ptr, (size_t)size * 2);
		buffer[size] = 0;
		package64.href = buffer;
		delete[] buffer;
		ptr += sizeof(wchar_t) * size;

		memcpy(package64.sha256, ptr, 32);
		ptr += 32;
	}

	return true;
}

bool Version::operator==(const Version& ver) const
{
	return major == ver.major && minor == ver.minor && fix == ver.fix && build == ver.build;
}

bool Version::operator<(const Version& ver) const
{
	bool res;
	if (major > ver.major) {
		res = false;
	}
	else if (major < ver.major) {
		res = true;
	}
	else {
		if (minor > ver.minor) {
			res = false;
		}
		else if (minor < ver.minor) {
			res = true;
		}
		else {
			if (fix > ver.fix) {
				res = false;
			}
			else if (fix < ver.fix) {
				res = true;
			}
			else {
				if (build < ver.build) {
					res = true;
				}
				else {
					res = false;
				}
			}
		}
	}
	return res;
}

bool Version::operator<=(const Version& ver) const
{
	bool res;
	if (major > ver.major) {
		res = false;
	}
	else if (major < ver.major) {
		res = true;
	}
	else {
		if (minor > ver.minor) {
			res = false;
		}
		else if (minor < ver.minor) {
			res = true;
		}
		else {
			if (fix > ver.fix) {
				res = false;
			}
			else if (fix < ver.fix) {
				res = true;
			}
			else {
				if (build <= ver.build) {
					res = true;
				}
				else {
					res = false;
				}
			}
		}
	}
	return res;
}

bool Version::operator>(const Version& ver) const
{
	return ver < *this;
}

bool Version::operator>=(const Version& ver) const
{
	return ver <= *this;
}
