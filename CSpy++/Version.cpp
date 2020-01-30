#include "general.h"
#include "Version.h"
#include <sstream>
using namespace std;

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
