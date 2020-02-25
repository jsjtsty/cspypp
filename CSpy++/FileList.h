#pragma once
#include "filemanager.h"
#include "Version.h"

constexpr const uint32_t CS_FILELIST_CURVER = 5U;

class FileList
{
public:
	FileList(const std::wstring_view path);
	FileList(const std::wstring_view path, Directory* dir);
	FileList(const std::wstring_view path, Directory* dir, void* additionalHeader, size_t size);
	FileList(const std::wstring_view path, VolumeDirectory* dir);
	FileList(const std::wstring_view path, USBDirectory* dir);
	~FileList();

	enum class DirectoryType : uint32_t {
		UNKNOWN,
		DIRECTORY,
		VOLUME_DIRECTORY,
		USB_DIRECTORY,
		OTHER
	};

	DirectoryType getDirectoryType() const noexcept;
	Directory* getDirectory() const noexcept;
	size_t getAdditionalHeaderSize() const noexcept;
	void* getAdditionalHeader() const noexcept;
	uint64_t getVersion() const noexcept;

	bool readFileList();
	bool writeFile() const;

	void releaseDirectory();
	void releaseAdditionalHeader();
	void release();

protected:
	DirectoryType type;
	Directory* directory = nullptr;
	Version clientVersion;
	std::wstring path;
	void* additionalHeader = nullptr;
	uint64_t headerSize;
	uint32_t version = CS_FILELIST_CURVER;
};