#include "Node.h"
#include <Windows.h>
#include <objbase.h>
using namespace std;

Node::Node()
{
	CoCreateGuid(&guid);
}

Node::Node(WIN32_FIND_DATA _val, Node* parent)
{
	fileAttributes = _val.dwFileAttributes;
	creationTime.setTime(_val.ftCreationTime, false);
	lastAccessTime.setTime(_val.ftLastAccessTime, false);
	lastWriteTime.setTime(_val.ftLastWriteTime, false);
	fileSize = ((uint64_t)(_val.nFileSizeHigh) << 32) | _val.nFileSizeLow;
	fileName = _val.cFileName;
	this->parent = parent;
	CoCreateGuid(&guid);
}

Node::Node(const Time& creationTime, const Time& lastAccessTime, const Time& lastWriteTime, uint32_t fileAttributes,
	uint64_t fileSize, const std::wstring_view fileName, const GUID& guid, Node* parent)
	: creationTime(creationTime), lastAccessTime(lastAccessTime), lastWriteTime(lastWriteTime),
	fileAttributes(fileAttributes), fileSize(fileSize), guid(guid), fileName(fileName.data()),
	parent(parent)
{

}

bool Node::isDirectory() const noexcept
{
	return fileAttributes & FILE_ATTRIBUTE_DIRECTORY;
}

bool Node::isFile() const noexcept
{
	return !(fileAttributes & FILE_ATTRIBUTE_DIRECTORY);
}

void Node::setData(WIN32_FIND_DATA _val, Node* parent)
{
	fileAttributes = _val.dwFileAttributes;
	creationTime.setTime(_val.ftCreationTime, true);
	lastAccessTime.setTime(_val.ftLastAccessTime, true);
	lastWriteTime.setTime(_val.ftLastWriteTime, true);
	fileSize = ((uint64_t)(_val.nFileSizeHigh) << 32) | _val.nFileSizeLow;
	fileName = _val.cFileName;
	this->parent = parent;
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

wstring Node::getPath() const
{
	wstring path = fileName;
	const Node* node = this;
	while (node = node->getParent()) {
		path = node->getFileName() + L'/' + path;
	}
	return path;
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

void Node::setParent(Node* parent) noexcept
{
	this->parent = parent;
}

Node* Node::getParent() const noexcept
{
	return this->parent;
}

size_t Node::getBinarySize() const
{
	return sizeof(uint32_t) + sizeof(uint64_t) + 3 * sizeof(Time) + sizeof(GUID) + sizeof(uint16_t) + 2 * fileName.size();
}

size_t Node::getBinaryData(void* buffer) const
{
	unsigned char* ptr = reinterpret_cast<unsigned char*>(buffer);
	*reinterpret_cast<uint32_t*>(ptr) = fileAttributes;
	ptr += sizeof(uint32_t);
	*reinterpret_cast<uint64_t*>(ptr) = fileSize;
	ptr += sizeof(uint64_t);
	*reinterpret_cast<Time*>(ptr) = creationTime;
	ptr += sizeof(Time);
	*reinterpret_cast<Time*>(ptr) = lastAccessTime;
	ptr += sizeof(Time);
	*reinterpret_cast<Time*>(ptr) = lastWriteTime;
	ptr += sizeof(Time);
	*reinterpret_cast<GUID*>(ptr) = guid;
	ptr += sizeof(GUID);
	*reinterpret_cast<uint16_t*>(ptr) = (uint16_t)fileName.size();
	ptr += sizeof(uint16_t);
	memcpy(ptr, fileName.c_str(), fileName.size() * 2);
	return getBinarySize();
}

size_t Node::writeBinaryData(StreamWriterFunction WriteStream)
{
	uint16_t size = (uint16_t)fileName.size();

	WriteStream(&fileAttributes, sizeof(uint32_t), 1);
	WriteStream(&fileSize, sizeof(uint64_t), 1);
	WriteStream(&creationTime, sizeof(Time), 1);
	WriteStream(&lastAccessTime, sizeof(Time), 1);
	WriteStream(&lastWriteTime, sizeof(Time), 1);
	WriteStream(&guid, sizeof(GUID), 1);
	WriteStream(&size, sizeof(uint16_t), 1);
	WriteStream(fileName.c_str(), (size_t)size * 2, 1);

	return getBinarySize();
}


