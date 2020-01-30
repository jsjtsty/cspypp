#include "Volume.h"
#include <Windows.h>
#include <string>
using namespace std;

Volume::Volume(wchar_t driveLetter)
{
	this->driveLetter = driveLetter;
	updateInformation();
}

inline wchar_t Volume::getDriveLetter() const noexcept
{
	return driveLetter;
}

inline wstring Volume::getDiskLabel() const noexcept
{
	return diskLabel;
}

inline wstring Volume::getFileSystem() const noexcept
{
	return fileSystem;
}

inline uint64_t Volume::getTotalSpace() const noexcept
{
	return totalSpace;
}

inline uint64_t Volume::getFreeSpace() const noexcept
{
	return freeSpace;
}

inline uint64_t Volume::getFreeSpaceToCaller() const noexcept
{
	return freeSpaceToCaller;
}

inline uint32_t Volume::getFileSystemFlags() const noexcept
{
	return fileSystemFlags;
}

inline uint32_t Volume::getVolumeSerialNumber() const noexcept
{
	return volumeSerialNumber;
}

inline uint32_t Volume::getMaximumComponentLength() const noexcept
{
	return maximumComponentLength;
}

inline uint32_t Volume::getDiskType() const noexcept
{
	return diskType;
}

bool Volume::updateInformation()
{
	wchar_t path[3] = { driveLetter,L':',L'\0' };
	diskType = GetDriveTypeW(path);
	if (diskType == DRIVE_UNKNOWN || diskType == DRIVE_NO_ROOT_DIR) {
		return false;
	}

	wchar_t volumeName[260], fileSystemName[260];
	GetVolumeInformationW(path, volumeName, 260, (LPDWORD)&volumeSerialNumber, (LPDWORD)&maximumComponentLength, (LPDWORD)&fileSystemFlags, fileSystemName, 260);
	GetDiskFreeSpaceExW(path, (PULARGE_INTEGER)&freeSpaceToCaller, (PULARGE_INTEGER)&totalSpace, (PULARGE_INTEGER)&freeSpace);
	this->diskLabel = volumeName;
	this->fileSystem = fileSystemName;

	return true;
}
