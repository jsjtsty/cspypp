#pragma once
#include <cstdint>
#include <string>

namespace Json {
	class Value;
}

class MonitorConfig {
public:
	MonitorConfig() : currentProtrolVersion(1U), minimumProtrolVersion(0U), protrolVersion(0U) {}

	virtual bool readConfig() = 0;
	virtual bool writeConfig() const = 0;

	uint32_t getCurrentProtrolVersion() const noexcept;
	uint32_t getProtrolVersion() const noexcept;
	void setProtrolVersion(uint32_t _val) noexcept;
	uint32_t getMinimumProtrolVersion() const noexcept;
	void setMinimumProtrolVersion(uint32_t _val) noexcept;

protected:
	bool readBasicConfig(const Json::Value& val);

	uint32_t currentProtrolVersion, minimumProtrolVersion, protrolVersion;
	std::wstring path;
};