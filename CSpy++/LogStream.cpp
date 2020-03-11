#include "LogStream.h"
#include "StringExt.h"
#include <fstream>
#include <iostream>

constexpr const wchar_t* LogPath = L"cspypp.log";

std::map<std::wstring, std::pair<CriticalSection, uint16_t>> LogStream::pool = std::map<std::wstring, std::pair<CriticalSection, uint16_t>>();
CriticalSection LogStream::poolSection = CriticalSection();

LogStream logi(L"Info"), loge(L"Error"), logd(L"Debug"), logw(L"Warning"), logv(L"Verbose");

LogStream::LogStream(const std::wstring_view type) : LogStream(type, LogPath) {}

LogStream::LogStream(const std::wstring_view type, const std::wstring_view path)
{
	poolSection.init();

	this->type = type;
	this->path = path;

	if (pool.find(LogPath) == pool.end()) {
		CriticalSection section;
		section.init();
		poolSection.enter();
		pool.insert(std::map<std::wstring, std::pair<CriticalSection, uint16_t>>::value_type(path,
			std::pair<CriticalSection, uint16_t>{ std::move(section), 1 }));
		pdata = &pool[this->path];
		poolSection.exit();
	}
	else {
		poolSection.enter();
		pdata = &pool[this->path];
		++pdata->second;
		poolSection.exit();
	}
}

LogStream::~LogStream()
{
	if (pdata->second == 1) {
		pool.erase(path);
	}
}

LogStreamBuffer::LogStreamBuffer(LogStream& logStream) : logStream(logStream) {}

LogStreamBuffer::LogStreamBuffer(LogStream& logStream, std::wstring_view val) : logStream(logStream)
{
	stream << val;
}

LogStreamBuffer::LogStreamBuffer(LogStream& logStream, std::wstringstream&& stream)
	: logStream(logStream), stream(std::forward<std::wstringstream>(stream)) {}

LogStreamBuffer::LogStreamBuffer(LogStreamBuffer&& buffer) noexcept : logStream(buffer.logStream)
{
	stream = std::move(buffer.stream);
	buffer.valid = false;
}

LogStreamBuffer::~LogStreamBuffer()
{
	if (valid) {
		CriticalSection& section = logStream.pdata->first;
		section.enter();
		std::wofstream fs(logStream.path, std::ios::app);
		fs << L'[' << logStream.type << L"](" << Time::getLocalTime().toString() << L"): " << stream.str() << std::endl;
		fs.close();
		section.exit();
	}
}

LogStreamBuffer& LogStreamBuffer::operator<<(const std::string_view val)
{
	stream << std::to_wstring(val);
	return *this;
}

LogStreamBuffer& LogStreamBuffer::operator<<(const std::wstring_view val)
{
	stream << val;
	return *this;
}

LogStreamBuffer& LogStreamBuffer::operator<<(int val)
{
	stream << val;
	return *this;
}

LogStreamBuffer& LogStreamBuffer::operator<<(unsigned int val)
{
	stream << val;
	return *this;
}

LogStreamBuffer& LogStreamBuffer::operator<<(short val)
{
	stream << val;
	return *this;
}

LogStreamBuffer& LogStreamBuffer::operator<<(unsigned short val)
{
	stream << val;
	return *this;
}

LogStreamBuffer& LogStreamBuffer::operator<<(long val)
{
	stream << val;
	return *this;
}

LogStreamBuffer& LogStreamBuffer::operator<<(unsigned long val)
{
	stream << val;
	return *this;
}

LogStreamBuffer& LogStreamBuffer::operator<<(long long val)
{
	stream << val;
	return *this;
}

LogStreamBuffer& LogStreamBuffer::operator<<(unsigned long long val)
{
	stream << val;
	return *this;
}

LogStreamBuffer& LogStreamBuffer::operator<<(double val)
{
	stream << val;
	return *this;
}

LogStreamBuffer& LogStreamBuffer::operator<<(float val)
{
	stream << val;
	return *this;
}

LogStreamBuffer& LogStreamBuffer::operator<<(long double val)
{
	stream << val;
	return *this;
}

LogStreamBuffer& LogStreamBuffer::operator<<(char val)
{
	stream << val;
	return *this;
}

LogStreamBuffer& LogStreamBuffer::operator<<(wchar_t val)
{
	stream << val;
	return *this;
}

LogStreamBuffer& LogStreamBuffer::operator<<(signed char val)
{
	stream << (int)val;
	return *this;
}

LogStreamBuffer& LogStreamBuffer::operator<<(unsigned char val)
{
	stream << (unsigned int)val;
	return *this;
}

LogStreamBuffer LogStream::operator<<(const std::string_view val)
{
	std::wstringstream stream;
	stream << std::to_wstring(val);
	LogStreamBuffer buffer(*this, std::move(stream));
	return std::move(buffer);
}

LogStreamBuffer LogStream::operator<<(const std::wstring_view val)
{
	std::wstringstream stream;
	stream << val;
	LogStreamBuffer buffer(*this, std::move(stream));
	return std::move(buffer);
}

LogStreamBuffer LogStream::operator<<(int val)
{
	std::wstringstream stream;
	stream << val;
	LogStreamBuffer buffer(*this, std::move(stream));
	return std::move(buffer);
}

LogStreamBuffer LogStream::operator<<(unsigned int val)
{
	std::wstringstream stream;
	stream << val;
	LogStreamBuffer buffer(*this, std::move(stream));
	return std::move(buffer);
}

LogStreamBuffer LogStream::operator<<(short val)
{
	std::wstringstream stream;
	stream << val;
	LogStreamBuffer buffer(*this, std::move(stream));
	return std::move(buffer);
}

LogStreamBuffer LogStream::operator<<(unsigned short val)
{
	std::wstringstream stream;
	stream << val;
	LogStreamBuffer buffer(*this, std::move(stream));
	return std::move(buffer);
}

LogStreamBuffer LogStream::operator<<(long val)
{
	std::wstringstream stream;
	stream << val;
	LogStreamBuffer buffer(*this, std::move(stream));
	return std::move(buffer);
}

LogStreamBuffer LogStream::operator<<(unsigned long val)
{
	std::wstringstream stream;
	stream << val;
	LogStreamBuffer buffer(*this, std::move(stream));
	return std::move(buffer);
}

LogStreamBuffer LogStream::operator<<(long long val)
{
	std::wstringstream stream;
	stream << val;
	LogStreamBuffer buffer(*this, std::move(stream));
	return std::move(buffer);
}

LogStreamBuffer LogStream::operator<<(unsigned long long val)
{
	std::wstringstream stream;
	stream << val;
	LogStreamBuffer buffer(*this, std::move(stream));
	return std::move(buffer);
}

LogStreamBuffer LogStream::operator<<(double val)
{
	std::wstringstream stream;
	stream << val;
	LogStreamBuffer buffer(*this, std::move(stream));
	return std::move(buffer);
}

LogStreamBuffer LogStream::operator<<(float val)
{
	std::wstringstream stream;
	stream << val;
	LogStreamBuffer buffer(*this, std::move(stream));
	return std::move(buffer);
}

LogStreamBuffer LogStream::operator<<(long double val)
{
	std::wstringstream stream;
	stream << val;
	LogStreamBuffer buffer(*this, std::move(stream));
	return std::move(buffer);
}

LogStreamBuffer LogStream::operator<<(char val)
{
	std::wstringstream stream;
	stream << val;
	LogStreamBuffer buffer(*this, std::move(stream));
	return std::move(buffer);
}

LogStreamBuffer LogStream::operator<<(wchar_t val)
{
	std::wstringstream stream;
	stream << val;
	LogStreamBuffer buffer(*this, std::move(stream));
	return std::move(buffer);
}

LogStreamBuffer LogStream::operator<<(signed char val)
{
	std::wstringstream stream;
	stream << (int)val;
	LogStreamBuffer buffer(*this, std::move(stream));
	return std::move(buffer);
}

LogStreamBuffer LogStream::operator<<(unsigned char val)
{
	std::wstringstream stream;
	stream << (unsigned int)val;
	LogStreamBuffer buffer(*this, std::move(stream));
	return std::move(buffer);
}