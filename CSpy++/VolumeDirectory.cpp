#include "VolumeDirectory.h"

VolumeDirectory::VolumeDirectory() : Volume(), Directory() {}

VolumeDirectory::VolumeDirectory(wchar_t driveLetter, WIN32_FIND_DATA _val) : Volume(driveLetter)
{
	wchar_t path[3] = { driveLetter,L':',L'\0' };
	Directory::Directory(_val, path);
}

VolumeDirectory::VolumeDirectory(const Volume& volume, WIN32_FIND_DATA _val) : Volume(volume)
{
	wchar_t path[3] = { driveLetter,L':',L'\0' };
	Directory::Directory(_val, path);
}

VolumeDirectory::VolumeDirectory(const Volume & volume, const Directory & directory) : Volume(volume), Directory(directory) {}
