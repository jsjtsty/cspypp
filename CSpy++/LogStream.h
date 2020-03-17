#pragma once
#include <map>
#include <string>
#include <sstream>
#include "Time.h"
#include "CriticalSection.h"

class LogStream;

class LogStreamBuffer final {
public:
	LogStreamBuffer(LogStream& logStream);
	LogStreamBuffer(LogStream& logStream, std::wstring_view val);
	LogStreamBuffer(LogStream& logStream, std::wstringstream&& stream);
	LogStreamBuffer(LogStreamBuffer&& buffer) noexcept;
	~LogStreamBuffer();

	LogStreamBuffer& operator<<(const std::string_view val);
	LogStreamBuffer& operator<<(const std::wstring_view val);
	LogStreamBuffer& operator<<(int val);
	LogStreamBuffer& operator<<(unsigned int val);
	LogStreamBuffer& operator<<(short val);
	LogStreamBuffer& operator<<(unsigned short val);
	LogStreamBuffer& operator<<(long val);
	LogStreamBuffer& operator<<(unsigned long val);
	LogStreamBuffer& operator<<(signed char val);
	LogStreamBuffer& operator<<(unsigned char val);
	LogStreamBuffer& operator<<(char val);
	LogStreamBuffer& operator<<(wchar_t val);
	LogStreamBuffer& operator<<(long long val);
	LogStreamBuffer& operator<<(unsigned long long val);
	LogStreamBuffer& operator<<(double val);
	LogStreamBuffer& operator<<(float val);
	LogStreamBuffer& operator<<(long double val);

private:
	std::wstringstream stream;
	LogStream& logStream;
	bool valid = true;
};

class LogStream final
{
public:
	LogStream(const std::wstring_view type);
	LogStream(const std::wstring_view type, const std::wstring_view path);
	~LogStream();

	LogStreamBuffer operator<<(const std::string_view val);
	LogStreamBuffer operator<<(const std::wstring_view val);
	LogStreamBuffer operator<<(int val);
	LogStreamBuffer operator<<(unsigned int val);
	LogStreamBuffer operator<<(short val);
	LogStreamBuffer operator<<(unsigned short val);
	LogStreamBuffer operator<<(long val);
	LogStreamBuffer operator<<(unsigned long val);
	LogStreamBuffer operator<<(signed char val);
	LogStreamBuffer operator<<(unsigned char val);
	LogStreamBuffer operator<<(char val);
	LogStreamBuffer operator<<(wchar_t val);
	LogStreamBuffer operator<<(long long val);
	LogStreamBuffer operator<<(unsigned long long val);
	LogStreamBuffer operator<<(double val);
	LogStreamBuffer operator<<(float val);
	LogStreamBuffer operator<<(long double val);

private:
	std::wstring type, path;
	std::pair<CriticalSection, uint16_t>* pdata;
	static std::map<std::wstring, std::pair<CriticalSection, uint16_t>> pool;
	static CriticalSection poolSection;

	friend class LogStreamBuffer;
};

extern LogStream logi, loge, logd, logw, logv;