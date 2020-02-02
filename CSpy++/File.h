#pragma once
#include "Node.h"
#include "Hash.h"

class File : public Node
{
public:
	File() : Node() {}
	File(WIN32_FIND_DATA _val, std::wstring path) : Node(_val, path) {}
	File(const Time& creationTime, const Time& lastAccessTime, const Time& lastWriteTime, uint32_t fileAttributes,
		uint64_t fileSize, const std::wstring_view fileName, const std::wstring_view filePath, const GUID& guid);

	virtual bool isDirectory() const noexcept override;
	virtual bool isFile() const noexcept override;
};

typedef std::shared_ptr<File*> FilePtr;
