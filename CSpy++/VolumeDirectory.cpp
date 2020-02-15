#include "VolumeDirectory.h"

VolumeDirectory::VolumeDirectory() : Volume(), Directory() {
	this->parent = nullptr;
	this->fileAttributes |= FILE_ATTRIBUTE_DIRECTORY;
}

VolumeDirectory::VolumeDirectory(wchar_t driveLetter) : Volume(driveLetter) , Directory()
{
	wchar_t path[3] = { driveLetter,L':',L'\0' };
	this->parent = nullptr;
	this->fileName = path;
	this->fileAttributes |= FILE_ATTRIBUTE_DIRECTORY;
}

VolumeDirectory::VolumeDirectory(const Volume& volume) : Volume(volume), Directory()
{
	wchar_t path[3] = { driveLetter,L':',L'\0' };
	this->parent = nullptr;
	this->fileName = path;
	this->fileAttributes |= FILE_ATTRIBUTE_DIRECTORY;
}

VolumeDirectory::VolumeDirectory(const Volume & volume, const Directory & directory) : Volume(volume), Directory(directory) {
	this->parent = nullptr;
	this->fileAttributes |= FILE_ATTRIBUTE_DIRECTORY;
}
