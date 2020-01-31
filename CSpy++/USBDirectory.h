#pragma once
#include "VolumeDirectory.h"
#include "USBDevice.h"

class USBDirectory : public VolumeDirectory, public USBDevice
{
public:
	USBDirectory();
	USBDirectory(wchar_t driveLetter);
	USBDirectory(const USBDevice& device);
};

typedef std::shared_ptr<USBDirectory> USBDirectoryPtr;