#include "File.h"
#include "Hasher.h"
using namespace std;

bool File::isDirectory() const noexcept
{
	return false;
}

bool File::isFile() const noexcept
{
	return true;
}

uint64_t File::getFileSize() const noexcept
{
	return fileSize;
}
