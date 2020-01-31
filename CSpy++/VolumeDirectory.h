#pragma once
#include "Directory.h"
#include "Volume.h"

class VolumeDirectory : public Directory, public Volume
{
public:
	VolumeDirectory();
	VolumeDirectory(wchar_t driveLetter);
	VolumeDirectory(const Volume& volume);
	VolumeDirectory(const Volume& volume, const Directory& directory);
};

typedef std::shared_ptr<VolumeDirectory> VolumeDirectoryPtr;