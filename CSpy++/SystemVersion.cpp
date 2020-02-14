#include "SystemVersion.h"

#ifdef WIN32_LEAN_AND_MEAN
#undef WIN32_LEAN_AND_MEAN
#endif

#include <Windows.h>
using namespace std;

constexpr uint8_t SV_PLATFORMID = 0B01;
constexpr uint8_t SV_CSDVERSION = 0B10;

typedef NTSTATUS (*_RtlGetVersion)(PRTL_OSVERSIONINFOW);

SystemVersion::SystemVersion(const void* buffer)
{
	const unsigned char* ptr = reinterpret_cast<const unsigned char*>(buffer);
	uint8_t type = *reinterpret_cast<const uint8_t*>(ptr);
	ptr += sizeof(uint8_t);
	_havePlatformId = type & SV_PLATFORMID;
	_haveCSDVersion = type & SV_CSDVERSION;

	major = *reinterpret_cast<const uint32_t*>(ptr);
	ptr += sizeof(uint32_t);
	minor = *reinterpret_cast<const uint32_t*>(ptr);
	ptr += sizeof(uint32_t);
	build = *reinterpret_cast<const uint32_t*>(ptr);
	ptr += sizeof(uint32_t);

	if (_havePlatformId) {
		platformId = *reinterpret_cast<const uint32_t*>(ptr);
		ptr += sizeof(uint32_t);
	}
	if (_haveCSDVersion) {
		uint8_t length = *reinterpret_cast<const uint32_t*>(ptr);
		ptr += sizeof(uint8_t);
		size_t size = size_t(length) * sizeof(wchar_t) + sizeof(wchar_t);
		wchar_t* buffer = new wchar_t[size];
		memcpy_s(buffer, size, ptr, size_t(length) * sizeof(wchar_t));
		buffer[length] = L'\0';
		CSDVersion = buffer;
		delete[] buffer;
	}
}

SystemVersion SystemVersion::getCurrentSystemVersion()
{
	SystemVersion version;
	RTL_OSVERSIONINFOW info;
	info.dwOSVersionInfoSize = sizeof(RTL_OSVERSIONINFOW);
	_RtlGetVersion RtlGetVersion;
	HMODULE hNtdll = LoadLibraryW(L"ntdll.dll");
	if (!hNtdll) {
		return version;
	}
	RtlGetVersion = (_RtlGetVersion)GetProcAddress(hNtdll, "RtlGetVersion");
	if (!RtlGetVersion) {
		FreeLibrary(hNtdll);
		return version;
	}
	NTSTATUS ret = RtlGetVersion(&info);
	FreeLibrary(hNtdll);
	version.major = info.dwMajorVersion;
	version.minor = info.dwMinorVersion;
	version.build = info.dwBuildNumber;
	version.platformId = info.dwPlatformId;
	version.CSDVersion = info.szCSDVersion;
	version._haveCSDVersion = true;
	version._havePlatformId = true;
	return version;
}

inline uint32_t SystemVersion::getMajor() const
{
	return major;
}

inline uint32_t SystemVersion::getMinor() const
{
	return minor;
}

inline uint32_t SystemVersion::getBuild() const
{
	return build;
}

inline uint32_t SystemVersion::getPlatformId() const
{
	return platformId;
}

inline std::wstring SystemVersion::getCSDVersion() const
{
	return CSDVersion;
}

size_t SystemVersion::getBinarySize(bool platformId, bool CSDVersion) const
{
	size_t size = 3 * sizeof(uint32_t);
	if (platformId && _havePlatformId) {
		size += sizeof(uint32_t);
	}
	if (CSDVersion && _haveCSDVersion) {
		size += this->CSDVersion.length() * sizeof(wchar_t);
	}
	return size;
}

size_t SystemVersion::getBinaryData(void* buffer, bool platformId, bool CSDVersion) const
{
	unsigned char* ptr = reinterpret_cast<unsigned char*>(buffer);
	bool pid = platformId && _havePlatformId, cid = CSDVersion && _haveCSDVersion;
	uint8_t type = 0;
	if (pid) {
		type |= SV_PLATFORMID;
	}
	if (cid) {
		type |= SV_CSDVERSION;
	}
	*reinterpret_cast<uint8_t*>(ptr) = type;
	ptr += sizeof(uint8_t);
	*reinterpret_cast<uint32_t*>(ptr) = major;
	ptr += sizeof(uint32_t);
	*reinterpret_cast<uint32_t*>(ptr) = minor;
	ptr += sizeof(uint32_t);
	*reinterpret_cast<uint32_t*>(ptr) = build;
	ptr += sizeof(uint32_t);

	if (pid) {
		*reinterpret_cast<uint32_t*>(ptr) = this->platformId;
		ptr += sizeof(uint32_t);
	}
	if (cid) {
		*reinterpret_cast<uint8_t*>(ptr) = (uint8_t)this->CSDVersion.length();
		ptr += sizeof(uint8_t);
		memcpy(ptr, this->CSDVersion.c_str(), this->CSDVersion.length() * 2);
	}
	return getBinarySize();
}

inline bool SystemVersion::havePlatformId() const
{
	return _havePlatformId;
}

inline bool SystemVersion::haveCSDVersion() const
{
	return _haveCSDVersion;
}
