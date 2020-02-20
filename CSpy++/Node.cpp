#include "Node.h"
#include "Directory.h"
#include <Windows.h>
#include <objbase.h>
using namespace std;

Node::Node() : parent(nullptr)
{
	CoCreateGuid(&guid);
}

Node::Node(WIN32_FIND_DATA _val, Directory* parent)
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
	uint64_t fileSize, const std::wstring_view fileName, const GUID& guid, Directory* parent)
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

void Node::setData(WIN32_FIND_DATA _val, Directory* parent)
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

void Node::setParent(Directory* parent) noexcept
{
	this->parent = parent;
}

Directory* Node::getParent() const noexcept
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
	GUID parentGuid = { 0,0,0,{0} };
	if (parent) {
		parentGuid = parent->guid;
	}

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
	*reinterpret_cast<GUID*>(ptr) = parentGuid;
	ptr += sizeof(GUID);
	*reinterpret_cast<uint16_t*>(ptr) = (uint16_t)fileName.size();
	ptr += sizeof(uint16_t);
	memcpy(ptr, fileName.c_str(), fileName.size() * 2);
	return getBinarySize();
}

bool Node::readBinaryData(const void* data)
{
	const unsigned char* ptr = reinterpret_cast<const unsigned char*>(data);
	uint16_t size;

	fileAttributes = *reinterpret_cast<const uint32_t*>(ptr);
	ptr += sizeof(uint32_t);
	fileSize = *reinterpret_cast<const uint64_t*>(ptr);
	ptr += sizeof(uint64_t);
	creationTime = *reinterpret_cast<const Time*>(ptr);
	ptr += sizeof(Time);
	lastAccessTime = *reinterpret_cast<const Time*>(ptr);
	ptr += sizeof(Time);
	lastWriteTime = *reinterpret_cast<const Time*>(ptr);
	ptr += sizeof(Time);
	guid = *reinterpret_cast<const GUID*>(ptr);
	ptr += sizeof(GUID);
	ptr += sizeof(GUID);
	size = *reinterpret_cast<const uint16_t*>(ptr);
	ptr += sizeof(uint16_t);
	fileName = wstring(reinterpret_cast<const wchar_t*>(ptr), size);
	return true;
}

bool Node::readBinaryData(const void* data, Directory* parent)
{
	const unsigned char* ptr = reinterpret_cast<const unsigned char*>(data);
	uint16_t size;

	fileAttributes = *reinterpret_cast<const uint32_t*>(ptr);
	ptr += sizeof(uint32_t);
	fileSize = *reinterpret_cast<const uint64_t*>(ptr);
	ptr += sizeof(uint64_t);
	creationTime = *reinterpret_cast<const Time*>(ptr);
	ptr += sizeof(Time);
	lastAccessTime = *reinterpret_cast<const Time*>(ptr);
	ptr += sizeof(Time);
	lastWriteTime = *reinterpret_cast<const Time*>(ptr);
	ptr += sizeof(Time);
	guid = *reinterpret_cast<const GUID*>(ptr);
	ptr += sizeof(GUID);
	this->parent = parent;
	ptr += sizeof(GUID);
	size = *reinterpret_cast<const uint16_t*>(ptr);
	ptr += sizeof(uint16_t);
	fileName = wstring(reinterpret_cast<const wchar_t*>(ptr), size);
	return true;
}

bool Node::readBinaryData(const void* data, DirectoryList& dirList)
{
	const unsigned char* ptr = reinterpret_cast<const unsigned char*>(data);
	uint16_t size;
	GUID parentGuid = { 0,0,0,{0} };

	fileAttributes = *reinterpret_cast<const uint32_t*>(ptr);
	ptr += sizeof(uint32_t);
	fileSize = *reinterpret_cast<const uint64_t*>(ptr);
	ptr += sizeof(uint64_t);
	creationTime = *reinterpret_cast<const Time*>(ptr);
	ptr += sizeof(Time);
	lastAccessTime = *reinterpret_cast<const Time*>(ptr);
	ptr += sizeof(Time);
	lastWriteTime = *reinterpret_cast<const Time*>(ptr);
	ptr += sizeof(Time);
	guid = *reinterpret_cast<const GUID*>(ptr);
	ptr += sizeof(GUID);

	parentGuid = *reinterpret_cast<const GUID*>(ptr);
	if (IsEmptyGuid(parentGuid)) {
		this->parent = nullptr;
	}
	else {
		this->parent = dirList.at(parentGuid);
	}
	ptr += sizeof(GUID);

	size = *reinterpret_cast<const uint16_t*>(ptr);
	ptr += sizeof(uint16_t);
	fileName = wstring(reinterpret_cast<const wchar_t*>(ptr), size);
	return true;
}

bool Node::readBinaryData(StreamReaderFunction ReadStream)
{
	GUID parentGuid;
	uint16_t size;
	size_t res;

	res = ReadStream(&fileAttributes, sizeof(uint32_t), 1);;

	if (!res) {
		return false;
	}

	ReadStream(&fileSize, sizeof(uint64_t), 1);
	ReadStream(&creationTime, sizeof(Time), 1);
	ReadStream(&lastAccessTime, sizeof(Time), 1);
	ReadStream(&lastWriteTime, sizeof(Time), 1);
	ReadStream(&guid, sizeof(GUID), 1);
	ReadStream(&parentGuid, sizeof(GUID), 1);
	ReadStream(&size, sizeof(uint16_t), 1);

	wchar_t* buffer = new wchar_t[(size_t)size * 2 + 1];
	ReadStream(buffer, size * sizeof(wchar_t), 1);
	buffer[size] = L'\0';
	fileName = buffer;
	delete[] buffer;
	return true;
}

bool Node::readBinaryData(StreamReaderFunction ReadStream, Directory* parent)
{
	GUID parentGuid;
	uint16_t size;
	size_t res;

	this->parent = parent;

	res = ReadStream(&fileAttributes, sizeof(uint32_t), 1);

	if (!res) {
		return false;
	}

	ReadStream(&fileSize, sizeof(uint64_t), 1);
	ReadStream(&creationTime, sizeof(Time), 1);
	ReadStream(&lastAccessTime, sizeof(Time), 1);
	ReadStream(&lastWriteTime, sizeof(Time), 1);
	ReadStream(&guid, sizeof(GUID), 1);
	ReadStream(&parentGuid, sizeof(GUID), 1);
	ReadStream(&size, sizeof(uint16_t), 1);

	wchar_t* buffer = new wchar_t[(size_t)size * 2 + 1];
	ReadStream(buffer, size * sizeof(wchar_t), 1);
	buffer[size] = L'\0';
	fileName = buffer;
	delete[] buffer;
	return true;
}

bool Node::readBinaryData(StreamReaderFunction ReadStream, DirectoryList& dirList)
{
	GUID parentGuid;
	uint16_t size;
	size_t res;

	res = ReadStream(&fileAttributes, sizeof(uint32_t), 1);
	if (!res) {
		return false;
	}

	ReadStream(&fileSize, sizeof(uint64_t), 1);
	ReadStream(&creationTime, sizeof(Time), 1);
	ReadStream(&lastAccessTime, sizeof(Time), 1);
	ReadStream(&lastWriteTime, sizeof(Time), 1);
	ReadStream(&guid, sizeof(GUID), 1);

	ReadStream(&parentGuid, sizeof(GUID), 1);

	if (IsEmptyGuid(parentGuid)) {
		this->parent = nullptr;
	}
	else {
		this->parent = dirList.at(parentGuid);
	}

	ReadStream(&size, sizeof(uint16_t), 1);
	wchar_t* buffer = new wchar_t[(size_t)size + 1];
	ReadStream(buffer, size * sizeof(wchar_t), 1);
	buffer[size] = L'\0';
	fileName = buffer;
	delete[] buffer;
	return true;
}

size_t Node::writeBinaryData(StreamWriterFunction WriteStream)
{
	uint16_t size = (uint16_t)fileName.size();
	GUID parentGuid = { 0,0,0,{0} };
	if (parent) {
		parentGuid = parent->guid;
	}

	WriteStream(&fileAttributes, sizeof(uint32_t), 1);
	WriteStream(&fileSize, sizeof(uint64_t), 1);
	WriteStream(&creationTime, sizeof(Time), 1);
	WriteStream(&lastAccessTime, sizeof(Time), 1);
	WriteStream(&lastWriteTime, sizeof(Time), 1);
	WriteStream(&guid, sizeof(GUID), 1);
	WriteStream(&parentGuid, sizeof(GUID), 1);
	WriteStream(&size, sizeof(uint16_t), 1);
	WriteStream(fileName.c_str(), (size_t)size * 2, 1);

	return getBinarySize();
}


