#include "CPUID.h"
#include "StringExt.h"
#include <intrin.h>
using namespace std;

CpuId::CpuId()
{
	vendor[0] = 0;
	brand[0] = 0;
	vendorLength = 0;
	brandLength = 0;
	family = 0;
	processorID = 0ULL;
}

CpuId::CpuId(string_view vendor, string_view brand, int family, uint64_t processorID) : family(family), processorID(processorID)
{
	if (vendor.length() <= 12) {
		strcpy_s(this->vendor, vendor.data());
	}
	else {
		strncpy_s(this->vendor, vendor.data(), 12);
	}
	this->vendorLength = (uint8_t)strlen(this->vendor);

	if (brand.length() <= 48) {
		strcpy_s(this->brand, brand.data());
	}
	else {
		strncpy_s(this->brand, brand.data(), 48);
	}
	this->brandLength = (uint8_t)strlen(this->brand);
}

CpuId::CpuId(wstring_view vendor, wstring_view brand, int family, uint64_t processorID) : family(family), processorID(processorID)
{
	string sVendor = to_string(vendor.data()), sBrand = to_string(brand.data());
	if (sVendor.length() <= 12) {
		strcpy_s(this->vendor, sVendor.c_str());
		this->vendorLength = (uint8_t)sVendor.length();
	}
	else {
		strncpy_s(this->vendor, sVendor.c_str(), 12);
		this->vendorLength = 12;
	}

	if (sBrand.length() <= 48) {
		strcpy_s(this->brand, sBrand.c_str());
		this->brandLength = (uint8_t)sBrand.length();
	}
	else {
		strncpy_s(this->brand, sBrand.c_str(), 48);
		this->brandLength = 48;
	}
}

CpuId::CpuId(const void* buffer)
{
	const unsigned char* ptr = reinterpret_cast<const unsigned char*>(buffer);

	vendorLength = *reinterpret_cast<const decltype(vendorLength)*>(ptr);
	ptr += sizeof(vendorLength);
	memcpy_s(vendor, sizeof(vendor), ptr, vendorLength);
	vendor[vendorLength] = 0;
	ptr += vendorLength;

	brandLength = *reinterpret_cast<const decltype(brandLength)*>(ptr);
	ptr += sizeof(brandLength);
	memcpy_s(brand, sizeof(brand), ptr, brandLength);
	brand[brandLength] = 0;
	ptr += brandLength;

	family = *reinterpret_cast<const decltype(family)*>(ptr);
	ptr += sizeof(family);
	processorID = *reinterpret_cast<const decltype(processorID)*>(ptr);
}

CpuId CpuId::getCurrentCpuId()
{
	CpuId cpuid;
	int32_t buffer[4];

	__cpuid(buffer, 0);
	*(int32_t*)&cpuid.vendor[0] = buffer[1];
	*(int32_t*)&cpuid.vendor[4] = buffer[3];
	*(int32_t*)&cpuid.vendor[8] = buffer[2];
	cpuid.vendor[12] = '\0';
	cpuid.vendorLength = (uint8_t)strlen(cpuid.vendor);

	__cpuid(buffer, 0x1);
	cpuid.family = (buffer[0] >> 8) & 0xf;

	__cpuid(buffer, 0x80000000);
	if (buffer[0] < 0x80000004)
	{
		return cpuid;
	}

	__cpuid((int32_t*)&cpuid.brand[0], 0x80000002);
	__cpuid((int32_t*)&cpuid.brand[16], 0x80000003);
	__cpuid((int32_t*)&cpuid.brand[32], 0x80000004);
	cpuid.brand[48] = '\0';
	cpuid.brandLength = (uint8_t)strlen(cpuid.brand);

	__cpuidex(buffer, 1, 1);
	cpuid.processorID = ((uint64_t) * ((uint32_t*)(&buffer[3])) << 32) | (*((uint32_t*)(&buffer[0])));

	return cpuid;
}

inline const char* CpuId::getVendor() const
{
	return vendor;
}

inline const char* CpuId::getBrand() const
{
	return brand;
}

inline uint8_t CpuId::getVendorLength() const
{
	return vendorLength;
}

inline uint8_t CpuId::getBrandLength() const
{
	return brandLength;
}

inline int32_t CpuId::getFamily() const
{
	return family;
}

inline uint64_t CpuId::getProcessorID() const
{
	return processorID;
}

inline size_t CpuId::getBinarySize() const
{
	return sizeof(vendorLength) + sizeof(brandLength) + sizeof(family) + sizeof(processorID) + vendorLength + brandLength;
}

size_t CpuId::getBinaryData(void* buffer) const
{
	unsigned char* ptr = reinterpret_cast<unsigned char*>(buffer);

	*reinterpret_cast<decltype(vendorLength)*>(ptr) = vendorLength;
	ptr += sizeof(vendorLength);
	memcpy(ptr, vendor, vendorLength);
	ptr += vendorLength;

	*reinterpret_cast<decltype(brandLength)*>(ptr) = brandLength;
	ptr += sizeof(brandLength);
	memcpy(ptr, brand, brandLength);
	ptr += brandLength;

	*reinterpret_cast<decltype(family)*>(ptr) = family;
	ptr += sizeof(family);
	*reinterpret_cast<decltype(processorID)*>(ptr) = processorID;

	return getBinarySize();
}

bool CpuId::operator==(const CpuId& other) const
{
	if (processorID != other.processorID) {
		return false;
	}
	if (vendorLength != other.vendorLength) {
		return false;
	}
	if (brandLength != other.brandLength) {
		return false;
	}
	if (strcmp(vendor, other.vendor) != 0) {
		return false;
	}
	if (strcmp(brand, other.brand) != 0) {
		return false;
	}
	if (family != other.family) {
		return false;
	}
	return true;
}

inline bool CpuId::operator!=(const CpuId& other) const
{
	return !this->operator==(other);
}
