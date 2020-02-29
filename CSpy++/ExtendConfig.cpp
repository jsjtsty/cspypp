#include "general.h"
#include "ExtendConfig.h"
#include <array>
#include <fstream>
#include "JsonExt.h"
using namespace std;

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



	return true;
}

bool ExtendConfig::writeConfig() const
{
	return false;
}
