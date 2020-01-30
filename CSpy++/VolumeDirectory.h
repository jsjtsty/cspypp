#pragma once
#include "Directory.h"
#include "Volume.h"

class VolumeDirectory : public Directory, public Volume
{
public:
	VolumeDirectory();
	VolumeDirectory(wchar_t driveLetter, WIN32_FIND_DATA _val);
	VolumeDirectory(const Volume& volume, WIN32_FIND_DATA _val);
	VolumeDirectory(const Volume& volume, const Directory& directory);
};

typedef std::shared_ptr<VolumeDirectory> VolumeDirectoryPtr;