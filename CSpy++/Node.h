#pragma once
#include <string>
#include <cstdint>
#include <Windows.h>
#include <memory>
#include "Time.h"

class Node
{
public:
	Node();
	Node(WIN32_FIND_DATA _val, const std::wstring& path);
	Node(const Time& creationTime, const Time& lastAccessTime, const Time& lastWriteTime, uint32_t fileAttributes,
		uint64_t fileSize, const std::wstring_view fileName, const std::wstring_view filePath, const GUID& guid);

	virtual bool isDirectory() const noexcept;
	virtual bool isFile() const noexcept;

	void setData(WIN32_FIND_DATA _val, const std::wstring& path);
	void setPath(std::wstring_view _val);

	Time getCreationTime() const noexcept;
	Time getLastAccessTime() const noexcept;
	Time getLastWriteTime() const noexcept;
	std::wstring getPath() const noexcept;
	std::wstring getFileName() const noexcept;
	uint64_t getFileSize() const noexcept;
	uint32_t getFileAttributes() const noexcept;

	GUID getGUID() const noexcept;
	void setGUID(const GUID& rg) noexcept;
	void setFileAttributes(uint32_t _val) noexcept;

protected:
	uint32_t fileAttributes = 0;
	Time creationTime, lastAccessTime, lastWriteTime;
	uint64_t fileSize = 0;
	std::wstring filePath;
	wchar_t fileName[260] = { 0 };
	GUID guid;
};

typedef std::shared_ptr<Node> NodePtr;