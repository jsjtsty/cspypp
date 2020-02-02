#include "Volume.h"
#include <Windows.h>
#include <string>
using namespace std;

Volume::Volume(wchar_t driveLetter)
{
	this->driveLetter = driveLetter;
	updateInformation();
}

wchar_t Volume::getDriveLetter() const noexcept
{
	return driveLetter;
}

wstring Volume::getDiskLabel() const noexcept
{
	return diskLabel;
}

wstring Volume::getFileSystem() const noexcept
{
	return fileSystem;
}

uint64_t Volume::getTotalSpace() const noexcept
{
	return totalSpace;
}

uint64_t Volume::getFreeSpace() const noexcept
{
	return freeSpace;
}

uint64_t Volume::getFreeSpaceToCaller() const noexcept
{
	return freeSpaceToCaller;
}

uint32_t Volume::getFileSystemFlags() const noexcept
{
	return fileSystemFlags;
}

uint32_t Volume::getVolumeSerialNumber() const noexcept
{
	return volumeSerialNumber;
}

uint32_t Volume::getMaximumComponentLength() const noexcept
{
	return maximumComponentLength;
}

uint32_t Volume::getDiskType() const noexcept
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
	wcscpy_s(this->diskLabel, volumeName);
	wcscpy_s(this->fileSystem, fileSystemName);

	return true;
}
