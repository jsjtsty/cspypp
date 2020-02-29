#include "MonitorConfig.h"
#include "JsonExt.h"
using namespace std;

#define JSON_CHECK_MEMBER(val, type, member) do{ if(!val.isMember(member)) return false; if(!val[member].is##type()) return false; } while(0)

uint32_t MonitorConfig::getCurrentProtrolVersion() const noexcept
{
	return this->currentProtrolVersion;
}

uint32_t MonitorConfig::getProtrolVersion() const noexcept
{
	return this->protrolVersion;
}

void MonitorConfig::setProtrolVersion(uint32_t _val) noexcept
{
	this->protrolVersion = _val;
}

uint32_t MonitorConfig::getMinimumProtrolVersion() const noexcept
{
	return this->minimumProtrolVersion;
}

void MonitorConfig::setMinimumProtrolVersion(uint32_t _val) noexcept
{
	this->minimumProtrolVersion = _val;
}

bool MonitorConfig::readBasicConfig(const Json::Value& val)
{
	JSON_CHECK_MEMBER(val, String, "encoding");
	if (JsonExt::read_wstring(val["encoding"]) != L"utf-8") {
		return false;
	}

	JSON_CHECK_MEMBER(val, String, "appname");
	if (JsonExt::read_wstring(val["appname"]) != L"cspypp") {
		return false;
	}

	JSON_CHECK_MEMBER(val, UInt, "protrolVersion");
	this->protrolVersion = val["protrolVersion"].asUInt();

	JSON_CHECK_MEMBER(val, UInt, "minimumVersion");
	this->minimumProtrolVersion = val["minimumVersion"].asUInt();

	if (this->minimumProtrolVersion > this->currentProtrolVersion) {
		return false;	// Unsupported format.
	}

	return true;
}
