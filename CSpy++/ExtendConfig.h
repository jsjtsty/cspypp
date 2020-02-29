#pragma once
#include "MonitorConfig.h"

constexpr const uint32_t CS_EXTCFG_PROTROL_VERSION = 1U;

class ExtendConfig : public MonitorConfig
{
public:
	ExtendConfig();
	ExtendConfig(const std::wstring_view path);

	virtual bool readConfig() override;
	virtual bool writeConfig() const override;

protected:

};

