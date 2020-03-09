#include "CriticalSection.h"
#include <utility>

CriticalSection::CriticalSection(CriticalSection&& source) noexcept
{
	section = std::move(source.section);
	source.status = false;
}

CriticalSection::~CriticalSection()
{
	if (status) {
		DeleteCriticalSection(&section);
	}
}

inline bool CriticalSection::isInitialized() const
{
	return status;
}

inline void CriticalSection::init()
{
	if (!status) {
		InitializeCriticalSection(&section);
		status = true;
	}
}

void CriticalSection::destroy()
{
	if (status) {
		DeleteCriticalSection(&section);
		status = false;
	}
}

bool CriticalSection::enter()
{
	if (status) {
		EnterCriticalSection(&section);
		return true;
	}
	else {
		return false;
	}
}

void CriticalSection::exit()
{
	if (status) {
		LeaveCriticalSection(&section);
	}
}
