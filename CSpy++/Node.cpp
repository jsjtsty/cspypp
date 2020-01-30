#include "Node.h"
using namespace std;

Node::Node(WIN32_FIND_DATA _val, const wstring& path)
{
	fileAttributes = _val.dwFileAttributes;
	creationTime.setTime(_val.ftCreationTime, true);
	lastAccessTime.setTime(_val.ftLastAccessTime, true);
	lastWriteTime.setTime(_val.ftLastWriteTime, true);
	fileSize = ((uint64_t)(_val.nFileSizeHigh) << 32) | _val.nFileSizeLow;
	wcscpy_s(fileName, 260, _val.cFileName);
	filePath = path;
}

bool Node::isDirectory() const noexcept
{
	return fileAttributes & FILE_ATTRIBUTE_DIRECTORY;
}

bool Node::isFile() const noexcept
{
	return !(fileAttributes & FILE_ATTRIBUTE_DIRECTORY);
}

void Node::setData(WIN32_FIND_DATA _val, const std::wstring & path)
{
	fileAttributes = _val.dwFileAttributes;
	creationTime.setTime(_val.ftCreationTime, true);
	lastAccessTime.setTime(_val.ftLastAccessTime, true);
	lastWriteTime.setTime(_val.ftLastWriteTime, true);
	fileSize = ((uint64_t)(_val.nFileSizeHigh) << 32) | _val.nFileSizeLow;
	wcscpy_s(fileName, 260, _val.cFileName);
	filePath = path;
}

inline Time Node::getCreationTime() const noexcept
{
	return creationTime;
}

inline Time Node::getLastAccessTime() const noexcept
{
	return lastAccessTime;
}

inline Time Node::getLastWriteTime() const noexcept
{
	return lastWriteTime;
}

inline wstring Node::getPath() const noexcept
{
	return filePath;
}

inline wstring Node::getFileName() const noexcept
{
	return fileName;
}


