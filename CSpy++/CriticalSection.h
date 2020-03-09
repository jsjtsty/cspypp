#pragma once
#include <Windows.h>

class CriticalSection
{
public:
	CriticalSection() noexcept {}
	CriticalSection(CriticalSection&& source) noexcept;
	CriticalSection(const CriticalSection&) = delete;
	~CriticalSection();
	
	bool isInitialized() const;
	void init();
	void destroy();
	bool enter();
	void exit();

protected:
	CRITICAL_SECTION section;
	bool status = false;
};

