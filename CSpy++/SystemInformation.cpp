#include "SystemInformation.h"

#ifdef WIN32_LEAN_AND_MEAN
#undef WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>
#include <intrin.h>
#include <array>
using namespace std;

typedef NTSTATUS (*_RtlGetVersion)(PRTL_OSVERSIONINFOW);


SystemVersion SystemInformation::GetSystemVersion()
{
	SystemVersion version = { 0,0,0,0,{0} };
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
	version.Major = info.dwMajorVersion;
	version.Minor = info.dwMinorVersion;
	version.Build = info.dwBuildNumber;
	version.PlatformId = info.dwPlatformId;
	wcscpy_s(version.CSDVersion, info.szCSDVersion);
	return version;
}

CPUInformation SystemInformation::GetCPUInformation()
{
	CPUInformationA info = GetCPUInformationA();
	CPUInformation res;
	res.Family = info.Family;
	res.ProcessorID = info.ProcessorID;
	MultiByteToWideChar(CP_ACP, 0, info.Vendor, -1, res.Vendor, sizeof(res.Vendor) / sizeof(wchar_t));
	MultiByteToWideChar(CP_ACP, 0, info.Brand, -1, res.Brand, sizeof(res.Brand) / sizeof(wchar_t));
	return res;
}

CPUInformationA SystemInformation::GetCPUInformationA()
{
	CPUInformationA info;
	int32_t buffer[4];

	__cpuid(buffer, 0);
	*(int32_t*)&info.Vendor[0] = buffer[1];
	*(int32_t*)&info.Vendor[4] = buffer[3];
	*(int32_t*)&info.Vendor[8] = buffer[2];
	info.Vendor[12] = '\0';

	__cpuid(buffer, 0x1);
	info.Family = (buffer[0] >> 8) & 0xf;

	__cpuid(buffer, 0x80000000);
	if (buffer[0] < 0x80000004)
	{
		return info;
	}

	__cpuid((int32_t*)&info.Brand[0], 0x80000002);
	__cpuid((int32_t*)&info.Brand[16], 0x80000003);
	__cpuid((int32_t*)&info.Brand[32], 0x80000004);
	info.Brand[48] = '\0';

	__cpuidex(buffer, 1, 1);
	info.ProcessorID = ((uint64_t) * ((uint32_t*)(&buffer[3])) << 32) | (*((uint32_t*)(&buffer[0])));

	return info;
}
