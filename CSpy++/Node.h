#pragma once
#include <string>
#include <cstdint>
#include <memory>
#include "Time.h"
#include <functional>

#ifndef GUID_DEFINED
#define GUID_DEFINED
#if defined(__midl)
typedef struct {
	unsigned long  Data1;
	unsigned short Data2;
	unsigned short Data3;
	byte           Data4[8];
} GUID;
#else
typedef struct _GUID {
	unsigned long  Data1;
	unsigned short Data2;
	unsigned short Data3;
	unsigned char  Data4[8];
} GUID;
#endif
#endif

typedef std::function<size_t(const void*, size_t, size_t)> StreamWriterFunction;

class Node
{
public:
	Node();
	Node(WIN32_FIND_DATA _val, Node* parent = nullptr);
	Node(const Time& creationTime, const Time& lastAccessTime, const Time& lastWriteTime, uint32_t fileAttributes,
		uint64_t fileSize, const std::wstring_view fileName, const GUID& guid, Node* parent = nullptr);
	Node(const void* data, Node* parent = nullptr);

	virtual bool isDirectory() const noexcept;
	virtual bool isFile() const noexcept;

	void setData(WIN32_FIND_DATA _val, Node* parent = nullptr);

	Time getCreationTime() const noexcept;
	Time getLastAccessTime() const noexcept;
	Time getLastWriteTime() const noexcept;
	std::wstring getPath() const;
	std::wstring getFileName() const noexcept;
	uint64_t getFileSize() const noexcept;
	uint32_t getFileAttributes() const noexcept;
	GUID getGUID() const noexcept;
	void setGUID(const GUID& rg) noexcept;
	void setFileAttributes(uint32_t _val) noexcept;
	void setParent(Node* parent) noexcept;
	Node* getParent() const noexcept;

	size_t getBinarySize() const;
	size_t getBinaryData(void* buffer) const;
	size_t writeBinaryData(StreamWriterFunction WriteStream);

protected:
	uint32_t fileAttributes = 0;
	uint64_t fileSize = 0;
	Time creationTime, lastAccessTime, lastWriteTime;
	GUID guid;
	Node* parent;
	std::wstring fileName;
};

typedef std::shared_ptr<Node> NodePtr;