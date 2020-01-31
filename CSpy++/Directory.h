#pragma once
#include "Node.h"
#include "File.h"
#include <vector>

class Directory;

typedef std::shared_ptr<Directory> DirectoryPtr;

class Directory : public Node
{
public:
	Directory() : Node() {}
	Directory(WIN32_FIND_DATA _val, const std::wstring& path) : Node(_val, path) {}
	virtual ~Directory();

	virtual bool isDirectory() const noexcept override;
	virtual bool isFile() const noexcept override;

	typedef std::vector<Directory*> DirectoryVector;
	typedef std::vector<File*> FileVector;

	File* addFile(File* file);
	File* addFile(WIN32_FIND_DATA _val, const std::wstring& path);
	Directory* addDirectory(Directory* file);
	Directory* addDirectory(WIN32_FIND_DATA _val, const std::wstring& path);

	const DirectoryVector& getDirectoryList() const;
	const FileVector& getFileList() const;

protected:
	DirectoryVector directoryList;
	FileVector fileList;
};