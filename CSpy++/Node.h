#pragma once
#include <string>
#include <cstdint>
#include <Windows.h>
#include <memory>
#include "Time.h"

class Node
{
public:
	Node() {}
	Node(WIN32_FIND_DATA _val, const std::wstring& path);

	virtual bool isDirectory() const noexcept;
	virtual bool isFile() const noexcept;

	void setData(WIN32_FIND_DATA _val, const std::wstring& path);

	inline Time getCreationTime() const noexcept;
	inline Time getLastAccessTime() const noexcept;
	inline Time getLastWriteTime() const noexcept;
	inline std::wstring getPath() const noexcept;
	inline std::wstring getFileName() const noexcept;

protected:
	uint32_t fileAttributes = 0;
	Time creationTime, lastAccessTime, lastWriteTime;
	uint64_t fileSize = 0;
	std::wstring filePath;
	wchar_t fileName[260] = { 0 };
};

typedef std::shared_ptr<Node> NodePtr;