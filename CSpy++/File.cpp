#include "File.h"
#include "Hasher.h"
using namespace std;

File::File(const Time & creationTime, const Time & lastAccessTime, const Time & lastWriteTime, uint32_t fileAttributes,
	uint64_t fileSize, const std::wstring_view fileName, const GUID & guid, Directory* parent)
	: Node(creationTime, lastAccessTime, lastWriteTime, fileAttributes, fileSize, fileName, guid, parent)
{
}

bool File::isDirectory() const noexcept
{
	return false;
}

bool File::isFile() const noexcept
{
	return true;
}

