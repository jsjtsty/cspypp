#pragma once
#include "VolumeDirectory.h"
#include "USBDevice.h"

class USBDirectory : public VolumeDirectory, public USBDevice
{
public:
	USBDirectory();
	USBDirectory(wchar_t driveLetter, WIN32_FIND_DATA _val);
};

typedef std::shared_ptr<USBDirectory> USBDirectoryPtr;