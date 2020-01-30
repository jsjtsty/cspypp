#pragma once
#include "Node.h"
#include "File.h"
#include <set>

class Directory : public Node
{
public:
	Directory() : Node() {}
	Directory(WIN32_FIND_DATA _val, const std::wstring& path) : Node(_val, path) {}
	~Directory();

	virtual bool isDirectory() const noexcept override;
	virtual bool isFile() const noexcept override;

	typedef std::set<Directory*> DirectorySet;
	typedef std::set<File*> FileSet;

	File* addFile(File* file);
	File* addFile(WIN32_FIND_DATA _val, const std::wstring& path);
	Directory* addDirectory(Directory* file);
	Directory* addDirectory(WIN32_FIND_DATA _val, const std::wstring& path);

	inline const DirectorySet& getDirectoryList() const;
	inline const FileSet& getFileList() const;

protected:
	DirectorySet directoryList;
	FileSet fileList;
};

