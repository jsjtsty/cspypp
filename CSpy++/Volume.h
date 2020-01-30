#pragma once
#include <string>
#include <cstdint>

class Volume
{
public:
	Volume() {}
	Volume(wchar_t driveLetter);

	inline wchar_t getDriveLetter() const noexcept;
	inline std::wstring getDiskLabel() const noexcept;
	inline std::wstring getFileSystem() const noexcept;
	inline uint64_t getTotalSpace() const noexcept;
	inline uint64_t getFreeSpace() const noexcept;
	inline uint64_t getFreeSpaceToCaller() const noexcept;
	inline uint32_t getFileSystemFlags() const noexcept;
	inline uint32_t getVolumeSerialNumber() const noexcept;
	inline uint32_t getMaximumComponentLength() const noexcept;
	inline uint32_t getDiskType() const noexcept;

	virtual bool updateInformation();

protected:
	wchar_t driveLetter = 0;
	std::wstring diskLabel, fileSystem;
	uint64_t totalSpace = 0, freeSpace = 0, freeSpaceToCaller = 0;
	uint32_t fileSystemFlags = 0, volumeSerialNumber = 0, maximumComponentLength = 0;
	uint32_t diskType = 0;
};
