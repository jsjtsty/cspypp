#include "USBDirectory.h"

USBDirectory::USBDirectory() : VolumeDirectory(), USBDevice() {}

USBDirectory::USBDirectory(wchar_t driveLetter, WIN32_FIND_DATA _val) : VolumeDirectory(driveLetter, _val), USBDevice(driveLetter) {}
