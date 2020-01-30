#include "Directory.h"

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
	fileList.insert(file);
	return file;
}

File* Directory::addFile(WIN32_FIND_DATA _val, const std::wstring & path)
{
	File* file = new File(_val, path);
	fileList.insert(file);
	return file;
}

Directory* Directory::addDirectory(Directory* file)
{
	directoryList.insert(file);
	return file;
}

Directory* Directory::addDirectory(WIN32_FIND_DATA _val, const std::wstring& path)
{
	Directory* file = new Directory(_val, path);
	directoryList.insert(file);
	return file;
}

inline const Directory::DirectorySet& Directory::getDirectoryList() const
{
	return directoryList;
}

inline const Directory::FileSet& Directory::getFileList() const
{
	return fileList;
}
