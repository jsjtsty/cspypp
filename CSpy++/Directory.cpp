#include "Directory.h"

Directory::Directory(const Time & creationTime, const Time & lastAccessTime, const Time & lastWriteTime, uint32_t fileAttributes,
	uint64_t fileSize, const std::wstring_view fileName, const GUID & guid, Node* parent)
	: Node(creationTime, lastAccessTime, lastWriteTime, fileAttributes, fileSize, fileName, guid, parent)
{
}

Directory::~Directory()
{
	for (const File* ptr : fileList) {
		delete ptr;
	}

	for (const Directory* ptr : directoryList) {
		delete ptr;
	}
}

bool Directory::isDirectory() const noexcept
{
	return true;
}

bool Directory::isFile() const noexcept
{
	return false;
}

File* Directory::addFile(File* file)
{
	fileList.push_back(file);
	return file;
}

File* Directory::addFile(WIN32_FIND_DATA _val)
{
	File* file = new File(_val, this);
	fileList.push_back(file);
	return file;
}

Directory* Directory::addDirectory(Directory* file)
{
	directoryList.push_back(file);
	return file;
}

Directory* Directory::addDirectory(WIN32_FIND_DATA _val)
{
	Directory* file = new Directory(_val, this);
	directoryList.push_back(file);
	return file;
}

const Directory::DirectoryVector& Directory::getDirectoryList() const
{
	return directoryList;
}

const Directory::FileVector& Directory::getFileList() const
{
	return fileList;
}
