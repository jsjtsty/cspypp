#include "Node.h"
using namespace std;

Node::Node()
{
	CoCreateGuid(&guid);
}

Node::Node(WIN32_FIND_DATA _val, const wstring& path)
{
	fileAttributes = _val.dwFileAttributes;
	creationTime.setTime(_val.ftCreationTime, false);
	lastAccessTime.setTime(_val.ftLastAccessTime, false);
	lastWriteTime.setTime(_val.ftLastWriteTime, false);
	fileSize = ((uint64_t)(_val.nFileSizeHigh) << 32) | _val.nFileSizeLow;
	wcscpy_s(fileName, 260, _val.cFileName);
	filePath = path;
	CoCreateGuid(&guid);
}

Node::Node(const Time & creationTime, const Time & lastAccessTime, const Time & lastWriteTime, uint32_t fileAttributes, 
	uint64_t fileSize, const std::wstring_view fileName, const std::wstring_view filePath, const GUID & guid)
	: creationTime(creationTime), lastAccessTime(lastAccessTime), lastWriteTime(lastWriteTime),
	fileAttributes(fileAttributes), fileSize(fileSize), filePath(filePath.data()), guid(guid)
{
	wcscpy_s(this->fileName, fileName.data());
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

void Node::setPath(std::wstring_view _val)
{
	filePath = _val.data();
}

Time Node::getCreationTime() const noexcept
{
	return creationTime;
}

Time Node::getLastAccessTime() const noexcept
{
	return lastAccessTime;
}

Time Node::getLastWriteTime() const noexcept
{
	return lastWriteTime;
}

wstring Node::getPath() const noexcept
{
	return filePath;
}

wstring Node::getFileName() const noexcept
{
	return fileName;
}

uint64_t Node::getFileSize() const noexcept
{
	return fileSize;
}

uint32_t Node::getFileAttributes() const noexcept {
	return fileAttributes;
}

GUID Node::getGUID() const noexcept
{
	return guid;
}

void Node::setGUID(const GUID& rg) noexcept
{
	guid = rg;
}

void Node::setFileAttributes(uint32_t _val) noexcept
{
	fileAttributes = _val;
}


