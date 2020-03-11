#include "LogStream.h"
#include "StringExt.h"
#include <fstream>

constexpr const wchar_t* LogPath = L"cspypp.log";

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
		poolSection.exit();
	}
}

LogStream::~LogStream()
{
	if (pdata->second == 1) {
		pool.erase(path);
	}
}

LogStreamBuffer::LogStreamBuffer(CriticalSection& section) : section(section) {}

LogStreamBuffer::LogStreamBuffer(CriticalSection& section, std::wstring_view val) : section(section)
{
	stream << val;
}

LogStreamBuffer::LogStreamBuffer(CriticalSection& section, std::wstringstream&& stream)
	: section(section), stream(std::forward<std::wstringstream>(stream)) {}
