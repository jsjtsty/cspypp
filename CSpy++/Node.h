#pragma once
#include <string>
#include <cstdint>
#include <memory>
#include <map>
#include "Time.h"
#include <functional>
#include "GuidExt.h"

class Directory;

typedef std::function<size_t(const void*, size_t, size_t)> StreamWriterFunction;
typedef std::function<size_t(void*, size_t, size_t)> StreamReaderFunction;
typedef std::map<GUID, Directory*> DirectoryList;

class Node
{
public:
	Node();
	Node(WIN32_FIND_DATA _val, Directory* parent = nullptr);
	Node(const Time& creationTime, const Time& lastAccessTime, const Time& lastWriteTime, uint32_t fileAttributes,
		uint64_t fileSize, const std::wstring_view fileName, const GUID& guid, Directory* parent = nullptr);

	virtual bool isDirectory() const noexcept;
	virtual bool isFile() const noexcept;

	void setData(WIN32_FIND_DATA _val, Directory* parent = nullptr);

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
	void setParent(Directory* parent) noexcept;
	Directory* getParent() const noexcept;

	size_t getBinarySize() const;
	size_t getBinaryData(void* buffer) const;
	bool readBinaryData(const void* data);
	bool readBinaryData(const void* data, Directory* parent);
	bool readBinaryData(const void* data, DirectoryList& dirList);
	bool readBinaryData(StreamReaderFunction StreamReader);
	bool readBinaryData(StreamReaderFunction StreamReader, Directory* parent);
	bool readBinaryData(StreamReaderFunction StreamReader, DirectoryList& dirList);
	size_t writeBinaryData(StreamWriterFunction WriteStream);

protected:
	uint32_t fileAttributes = 0;
	uint64_t fileSize = 0;
	Time creationTime, lastAccessTime, lastWriteTime;
	GUID guid;
	Directory* parent;
	std::wstring fileName;
};

typedef std::shared_ptr<Node> NodePtr;