#pragma once
#include <array>
#include <map>
#include <string>
#include <sstream>
#include "Time.h"
#include "CriticalSection.h"

class LogStreamBuffer final {
public:
	LogStreamBuffer(CriticalSection& section);
	LogStreamBuffer(CriticalSection& section, std::wstring_view val);
	LogStreamBuffer(CriticalSection& section, std::wstringstream&& stream);
	~LogStreamBuffer();

	operator bool() const;

	LogStreamBuffer&& operator<<(const std::string_view val);
	LogStreamBuffer&& operator<<(const std::wstring_view val);
	LogStreamBuffer&& operator<<(int val);
	LogStreamBuffer&& operator<<(unsigned int val);
	LogStreamBuffer&& operator<<(short val);
	LogStreamBuffer&& operator<<(unsigned short val);
	LogStreamBuffer&& operator<<(long val);
	LogStreamBuffer&& operator<<(unsigned long val);
	LogStreamBuffer&& operator<<(signed char val);
	LogStreamBuffer&& operator<<(unsigned char val);
	LogStreamBuffer&& operator<<(char val);
	LogStreamBuffer&& operator<<(wchar_t val);
	LogStreamBuffer&& operator<<(long long val);
	LogStreamBuffer&& operator<<(unsigned long long val);
	LogStreamBuffer&& operator<<(double val);
	LogStreamBuffer&& operator<<(float val);
	LogStreamBuffer&& operator<<(long double val);

private:
	std::wstringstream stream;
	CriticalSection& section;
};

class LogStream final
{
public:
	LogStream(const std::wstring_view type);
	LogStream(const std::wstring_view type, const std::wstring_view path);
	~LogStream();

	operator LogStreamBuffer && ();

	void flush();

private:
	std::wstring type, path;
	std::pair<CriticalSection, uint16_t>* pdata;
	static std::map<std::wstring, std::pair<CriticalSection, uint16_t>> pool;
	static CriticalSection poolSection;

	template<typename T> friend inline LogStream& logout(LogStream& stream, T msg);
};

extern LogStream logi, loge, logd, logw, logv;