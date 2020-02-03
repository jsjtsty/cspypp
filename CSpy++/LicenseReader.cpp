#include "LicenseReader.h"
#include <Windows.h>

HINSTANCE GetCurrentInstance();

LicenseReader::LicenseReader(LicenseType licenseType)
{
	if (licenseType != LicenseType::UNKONWN) {
		HRSRC src = FindResourceW(GetCurrentInstance(), MAKEINTRESOURCE(licenseType), L"LICENSE");
		if (src) {
			hResource = LoadResource(GetCurrentInstance(), src);
		}
		else {
			hResource = NULL;
		}
	}
}

LicenseReader::~LicenseReader()
{
	FreeResource(hResource);
}

void* LicenseReader::lockResource()
{
	if (!hResource) {
		return nullptr;
	}

	return LockResource(hResource);
}
