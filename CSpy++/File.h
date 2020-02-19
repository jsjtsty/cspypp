#pragma once
#include "Node.h"
#include "Hash.h"

class File : public Node
{
public:
	File() : Node() {}
	File(WIN32_FIND_DATA _val, Node* parent = nullptr) : Node(_val, parent) {}
	File(const Time& creationTime, const Time& lastAccessTime, const Time& lastWriteTime, uint32_t fileAttributes,
		uint64_t fileSize, const std::wstring_view fileName, const GUID& guid, Node* parent);
	File(StreamReaderFunction ReadStream, Node* parent) : Node(ReadStream, parent) {}
	File(const void* data, Node* parent) : Node(data, parent) {}

	virtual bool isDirectory() const noexcept override;
	virtual bool isFile() const noexcept override;
};

typedef std::shared_ptr<File*> FilePtr;
