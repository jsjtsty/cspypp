#include "general.h"
#include "Plugin.h"
#include <memory>
#include <Windows.h>
using namespace std;

#define CS_PLUGIN_OK 0

extern "C" {
	typedef int32_t(*_GlobalInitialization)(uint32_t threadId);
	typedef uint32_t(*_GetProtrolVersion)();
	typedef int32_t(*_CheckProtrolVersion)(uint32_t currentProtrolVersion);
	typedef uint32_t(*_GetPluginVersion)();
	typedef SYSTEMTIME(*_GetReleaseTime)();
}

Plugin::Plugin(const std::wstring_view path)
{
	this->path = path.data();
}

bool Plugin::init()
{
	auto libraryDeleter = [](HMODULE ptr) -> void {
		FreeLibrary(ptr);
	};
	HMODULE temp = LoadLibrary(path.c_str());
	if (!temp) {
		return false;
	}
	unique_ptr<decltype(*temp), decltype(libraryDeleter)> hLibrary(temp, libraryDeleter);
	temp = nullptr;

	_GlobalInitialization GlobalInitialization = (_GlobalInitialization)GetProcAddress(hLibrary.get(), "CSPYPP_GlobalInitialization");
	_GetProtrolVersion GetProtrolVersion = (_GetProtrolVersion)GetProcAddress(hLibrary.get(), "CSPYPP_GetProtrolVersion");
	_CheckProtrolVersion CheckProtrolVersion = (_CheckProtrolVersion)GetProcAddress(hLibrary.get(), "CSPYPP_CheckProtrolVersion");
	_GetPluginVersion GetPluginVersion = (_GetPluginVersion)GetProcAddress(hLibrary.get(), "CSPYPP_GetPluginVersion");
	_GetReleaseTime GetReleaseTime = (_GetReleaseTime)GetProcAddress(hLibrary.get(), "CSPYPP_GetReleaseTime");

	if (!(GlobalInitialization && GetProtrolVersion && CheckProtrolVersion && GetPluginVersion && GetReleaseTime)) {
		return false;
	}

	if (GlobalInitialization(GetCurrentThreadId()) != CS_PLUGIN_OK) {
		return false;
	}
	
	if (CheckProtrolVersion(CS_PLUGIN_PROTROL_VERSION) != CS_PLUGIN_OK) {
		return false;
	}

	this->protrolVersion = GetPluginVersion();
	this->version = GetPluginVersion();
	this->releaseTime = GetReleaseTime();
	
}
