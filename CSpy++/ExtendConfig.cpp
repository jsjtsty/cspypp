#include "general.h"
#include "ExtendConfig.h"
#include <array>
#include <fstream>
#include "JsonExt.h"
using namespace std;

#define JSON_CHECK_MEMBER(val, type, member) do{ if(!val.isMember(member)) return false; if(!val[member].is##type()) return false; } while(0)

ExtendConfig::ExtendConfig() : MonitorConfig()
{
	this->path = L"extcfg.json";
}

ExtendConfig::ExtendConfig(const std::wstring_view path) : MonitorConfig()
{
	this->path = path.data();
}

bool ExtendConfig::readConfig()
{
	Json::Value root;
	ifstream ifs(this->path);
	ifs >> root;

	if (!MonitorConfig::readBasicConfig(root)) {
		return false;
	}



	return true;
}

bool ExtendConfig::writeConfig() const
{
	return false;
}
