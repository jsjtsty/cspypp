#include "USBDirectory.h"

USBDirectory::USBDirectory() : VolumeDirectory(), USBDevice() {}

USBDirectory::USBDirectory(wchar_t driveLetter) : VolumeDirectory(driveLetter), USBDevice(driveLetter) {}

USBDirectory::USBDirectory(const USBDevice & device) : USBDevice(device), VolumeDirectory() {}
