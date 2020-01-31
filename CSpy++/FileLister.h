#pragma once
#include <string>
#include "Directory.h"
#include "VolumeDirectory.h"
#include "USBDirectory.h"

namespace FileLister {
	DirectoryPtr list_directory(std::wstring_view path);
	VolumeDirectoryPtr list_volume(wchar_t driveLetter);
	USBDirectoryPtr list_usb(wchar_t driveLetter);
	USBDirectoryPtr list_usb(const USBDevice& usb);
}