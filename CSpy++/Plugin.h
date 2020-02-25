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

	uint32_t getProtrolVersion();
	uint32_t getVersion();
	Time getReleaseTime();
	std::wstring getPath();
	std::wstring getName();
	std::wstring getVersionName();
	std::wstring getCompanyName();

protected:
	bool isInfoLoaded = false;
	uint32_t protrolVersion, version;
	std::wstring path, pluginName, versionName, companyName;
	Time releaseTime;
	void* hLibrary;
};

