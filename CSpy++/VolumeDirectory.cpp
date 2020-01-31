#include "VolumeDirectory.h"

VolumeDirectory::VolumeDirectory() : Volume(), Directory() {}

VolumeDirectory::VolumeDirectory(wchar_t driveLetter) : Volume(driveLetter) , Directory()
{
	wchar_t path[3] = { driveLetter,L':',L'\0' };
	this->setPath(path);
}

VolumeDirectory::VolumeDirectory(const Volume& volume) : Volume(volume), Directory()
{
	wchar_t path[3] = { driveLetter,L':',L'\0' };
	this->setPath(path);
}

VolumeDirectory::VolumeDirectory(const Volume & volume, const Directory & directory) : Volume(volume), Directory(directory) {}
