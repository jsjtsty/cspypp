#include "File.h"
#include "Hasher.h"
using namespace std;

File::File(const Time & creationTime, const Time & lastAccessTime, const Time & lastWriteTime, uint32_t fileAttributes,
	uint64_t fileSize, const std::wstring_view fileName, const std::wstring_view filePath, const GUID & guid)
	: Node(creationTime, lastAccessTime, lastWriteTime, fileAttributes, fileSize, fileName, filePath, guid)
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

