#include "LicenseReader.h"
#include <Windows.h>

HINSTANCE GetCurrentInstance();

LicenseReader::LicenseReader(LicenseType licenseType)
{
	if (licenseType != LicenseType::UNKONWN) {
		HRSRC src = FindResourceW(GetCurrentInstance(), MAKEINTRESOURCE(licenseType), L"LICENSE");
		hResource = LoadResource(GetCurrentInstance(), src);
	}
}

void* LicenseReader::lockResource()
{
	if (resourceMemory) {
		return resourceMemory;
	}

	if (!hResource) {
		return nullptr;
	}

	resourceMemory = LockResource(hResource);
	return resourceMemory;
}

void * LicenseReader::getResource()
{
	return resourceMemory;
}

void LicenseReader::releaseResoure()
{
	
}
