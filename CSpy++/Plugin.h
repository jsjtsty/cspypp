#pragma once
#include <string>
#include "Time.h"

constexpr const uint32_t CS_PLUGIN_PROTROL_VERSION = 1U;

class Plugin
{
public:
	Plugin(const std::wstring_view path);

	virtual bool init();
	virtual bool free();

protected:
	uint32_t protrolVersion, version;
	std::wstring path, pluginName, versionName, companyName;
	Time releaseTime;
};

