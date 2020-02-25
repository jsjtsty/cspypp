#pragma once
#include <cstdint>

class MonitorConfig {
public:
	MonitorConfig();

	virtual bool readConfig() = 0;
	virtual bool writeConfig() = 0;


};