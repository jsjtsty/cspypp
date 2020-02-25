#include "FileList.h"
#include <queue>
#include <zlib.h>
#include <map>
#include "Hasher.h"
#include "Version.h"
using namespace std;

FileList::FileList(const std::wstring_view path)
{
	this->path = path.data();
	this->directory = nullptr;
	this->type = DirectoryType::UNKNOWN;
	this->headerSize = 0;
	this->additionalHeader = nullptr;
	this->clientVersion = Version::currentVersion;
}

FileList::FileList(const std::wstring_view path, Directory* dir)
{
	this->path = path.data();
	this->directory = dir;
	this->type = DirectoryType::DIRECTORY;
	this->headerSize = 0;
	this->additionalHeader = nullptr;
	this->clientVersion = Version::currentVersion;
}

FileList::FileList(const std::wstring_view path, Directory* dir, void* additionalHeader, size_t size)
{
	this->path = path.data();
	this->directory = dir;
	this->type = DirectoryType::OTHER;
	this->headerSize = size;
	this->additionalHeader = malloc(this->headerSize);
	memcpy_s(this->additionalHeader, size, additionalHeader, size);
	this->clientVersion = Version::currentVersion;
}

FileList::FileList(const std::wstring_view path, VolumeDirectory* dir)
{
	this->path = path.data();
	this->directory = dir;
	this->type = DirectoryType::VOLUME_DIRECTORY;
	this->headerSize = sizeof(Volume);
	this->additionalHeader = malloc(sizeof(Volume));
	Volume temp = *dir;
	this->clientVersion = Version::currentVersion;
	memcpy_s(this->additionalHeader, sizeof(Volume), &temp, sizeof(Volume));
}

FileList::FileList(const std::wstring_view path, USBDirectory* dir)
{
	this->path = path.data();
	this->directory = dir;
	this->type = DirectoryType::USB_DIRECTORY;
	this->additionalHeader = malloc(sizeof(USBDevice));
	this->headerSize = sizeof(USBDevice);
	USBDevice temp = *dir;
	this->clientVersion = Version::currentVersion;
	memcpy_s(this->additionalHeader, sizeof(USBDevice), &temp, sizeof(USBDevice));
}

FileList::~FileList()
{
	free(this->additionalHeader);
}

void* FileList::getAdditionalHeader() const noexcept
{
	return additionalHeader;
}

uint64_t FileList::getVersion() const noexcept
{
	return version;
}

bool FileList::readFileList()
{
	if (directory) {
		delete directory;
		directory = nullptr;
	}

	gzFile file = gzopen_w(path.c_str(), "r");
	uint32_t bfsize;
	void* buffer;
	if (!file) {
		return false;
	}

	char fileType[29] = { 0 };
	gzfread(fileType, 29 * sizeof(char), 1, file);
	if (strcmp(fileType, "CSpy++ FileList (*.csf) File") != 0) {
		gzclose_w(file);
		return false;
	}

	gzfread(&version, sizeof(version), 1, file);
	gzfread(&bfsize, sizeof(uint32_t), 1, file);
	buffer = malloc(bfsize);
	if (!clientVersion.readBinaryData(buffer)) {
		return false;
	}
	free(buffer);

	gzfread(&type, sizeof(type), 1, file);
	gzfread(&headerSize, sizeof(headerSize), 1, file);

	if (additionalHeader) {
		free(additionalHeader);
	}

	if (headerSize) {
		additionalHeader = malloc(headerSize);
		gzfread(additionalHeader, headerSize, 1, file);
	}
	else {
		additionalHeader = nullptr;
	}

	DirectoryList nodeList;
	auto gzfread2 = std::bind(gzfread, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, file);
	Node temp;

	while (true) {
		if (!temp.readBinaryData(gzfread2, nodeList)) {
			break;
		}
		
		if (temp.isDirectory()) {
			Directory* dir = new Directory(temp);
			nodeList[dir->getGuid()] = dir;
			if (dir->getParent()) {
				dir->getParent()->addDirectory(dir);
			}
			else {
				directory = dir;
			}
		}
		else {
			File* file = new File(temp);
			file->getParent()->addFile(file);
		}
	}

	gzclose_w(file);
	return true;
}

bool FileList::writeFile() const
{
	if (!directory) {
		return false;
	}

	uint32_t bfsize;
	void* buffer;
	gzFile file = gzopen_w(path.c_str(), "w");
	if (!file) {
		return false;
	}

	// 1. Write file type.
	const char* fileType = "CSpy++ FileList (*.csf) File";
	gzfwrite(fileType, 29 * sizeof(char), 1, file);

	// 2. Write version information.
	gzfwrite(&version, sizeof(version), 1, file);
	bfsize = (uint32_t)Version::currentVersion.getBinarySize();
	buffer = malloc(bfsize);
	Version::currentVersion.getBinaryData(buffer);
	gzfwrite(&bfsize, sizeof(uint32_t), 1, file);
	gzfwrite(buffer, bfsize, 1, file);
	free(buffer);
	
	// 3. Write addtional header type.
	uint32_t headerType = (uint32_t)this->type;
	gzfwrite(&headerType, sizeof(headerType), 1, file);

	// 4. Write addtional header size.
	gzfwrite(&headerSize, sizeof(headerSize), 1, file);

	// 5. Write addtional header.
	if (additionalHeader) {
		gzfwrite(additionalHeader, headerSize, 1, file);
	}

	// 6. Write file list.
	queue<Directory*> dir;
	dir.push(directory);
	Hash hash;
	auto gzfwrite2 = std::bind(gzfwrite, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, file);
	directory->writeBinaryData(gzfwrite2);
	while(!dir.empty()) {
		for (File* fl : dir.front()->getFileList()) {
			fl->writeBinaryData(gzfwrite2);
		}
		for (Directory* dl : dir.front()->getDirectoryList()) {
			dl->writeBinaryData(gzfwrite2);
			dir.push(dl);
		}
		dir.pop();
	}
	
	// 7. Close file.
	gzclose_w(file);
	return true;
}

void FileList::releaseDirectory()
{
	delete directory;
	directory = nullptr;
}

void FileList::releaseAdditionalHeader()
{
	free(additionalHeader);
	additionalHeader = nullptr;
}

void FileList::release()
{
	delete directory;
	directory = nullptr;
	free(additionalHeader);
	additionalHeader = nullptr;
}

FileList::DirectoryType FileList::getDirectoryType() const noexcept
{
	return type;
}

Directory* FileList::getDirectory() const noexcept
{
	return directory;
}

size_t FileList::getAdditionalHeaderSize() const noexcept
{
	return headerSize;
}