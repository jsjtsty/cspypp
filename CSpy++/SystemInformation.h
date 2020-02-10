#pragma once
#include <cstdint>

struct SystemVersion {
	uint32_t Major;
	uint32_t Minor;
	uint32_t Build;
	uint32_t PlatformId;
	wchar_t CSDVersion[128];
};

struct CPUInformationA {
	char Vendor[13];
	char Brand[49];
	int Family;
	uint64_t ProcessorID;
};

struct CPUInformation {
	wchar_t Vendor[13];
	wchar_t Brand[49];
	int Family;
	uint64_t ProcessorID;
};

class SystemInformation
{
public:
	static SystemVersion GetSystemVersion();
	static CPUInformation GetCPUInformation();
	static CPUInformationA GetCPUInformationA();
private:
	SystemInformation() = delete;
	~SystemInformation() = delete;
	SystemInformation(const SystemInformation& info) = delete;
};

