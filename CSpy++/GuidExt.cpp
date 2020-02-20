#include "general.h"
#include "GuidExt.h"
#include <cstdint>

#ifdef WIN32_LEAN_AND_MEAN
#undef WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>

constexpr const GUID emptyGuid = { 0,0,0,{0,0,0,0,0,0,0,0} };

bool IsEmptyGuid(const GUID& guid)
{
	return guid == emptyGuid;
}

bool operator<(const GUID& guid1, const GUID& guid2)
{
	uint64_t high1 = *((uint64_t*)&guid1), low1 = *((uint64_t*)&guid1 + 1),
		high2 = *((uint64_t*)&guid2), low2 = *((uint64_t*)&guid2 + 1);
	if (high1 < high2) {
		return true;
	}
	else if (high1 > high2) {
		return false;
	}
	else {
		return low1 < low2;
	}
}
