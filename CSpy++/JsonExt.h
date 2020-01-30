#pragma once

#include <json/json.h>
#include <string>

namespace JsonExt {
	std::wstring read_wstring(const Json::Value& _val);
}