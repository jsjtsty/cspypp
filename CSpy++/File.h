#pragma once
#include "Node.h"
#include "Hash.h"

class File : public Node
{
public:
	File() : Node() {}
	File(WIN32_FIND_DATA _val, std::wstring path) : Node(_val, path) {}

	virtual bool isDirectory() const noexcept override;
	virtual bool isFile() const noexcept override;

	inline uint64_t getFileSize() const noexcept;
};

typedef std::shared_ptr<File*> FilePtr;