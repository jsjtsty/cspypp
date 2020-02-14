#pragma once
#include <cstdint>
#include <string>

class SystemVersion final
{
public:
	SystemVersion() {}
	SystemVersion(uint32_t major, uint32_t minor, uint32_t build, uint32_t platformId, std::wstring_view CSDVersion)
		: major(major), minor(minor), build(build), platformId(platformId), CSDVersion(CSDVersion),
		  _haveCSDVersion(true), _havePlatformId(true) {}
	SystemVersion(uint32_t major, uint32_t minor, uint32_t build) : major(major), minor(minor), build(build),
		  _haveCSDVersion(false), _havePlatformId(false) {}
	SystemVersion(const void* buffer);

	static SystemVersion getCurrentSystemVersion();

	inline uint32_t getMajor() const;
	inline uint32_t getMinor() const;
	inline uint32_t getBuild() const;
	inline uint32_t getPlatformId() const;
	inline std::wstring getCSDVersion() const;

	size_t getBinarySize(bool platformId = true, bool CSDVersion = true) const;
	size_t getBinaryData(void* buffer, bool platformId = true, bool CSDVersion = true) const;

	inline bool havePlatformId() const;
	inline bool haveCSDVersion() const;
private:
	uint32_t major = 0, minor = 0, build = 0, platformId = 0;
	std::wstring CSDVersion;
	bool _havePlatformId = false, _haveCSDVersion = false;
};
