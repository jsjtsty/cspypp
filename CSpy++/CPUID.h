#pragma once
#include <cstdint>
#include <string>

class CPUID final
{
public:
	CPUID();
	CPUID(std::string_view vendor, std::string_view brand, int family, uint64_t processorID);
	CPUID(std::wstring_view vendor, std::wstring_view brand, int family, uint64_t processorID);
	CPUID(const void* buffer);

	static CPUID getCurrentCpuId();

	inline const char* getVendor() const;
	inline const char* getBrand() const;
	inline uint8_t getVendorLength() const;
	inline uint8_t getBrandLength() const;
	inline int32_t getFamily() const;
	inline uint64_t getProcessorID() const;

	inline size_t getBinarySize() const;
	size_t getBinaryData(void* buffer) const;

	bool operator==(const CPUID& other) const;
	inline bool operator!=(const CPUID& other) const;

private:
	char vendor[13], brand[49];
	uint8_t vendorLength, brandLength;
	int32_t family;
	uint64_t processorID;
};