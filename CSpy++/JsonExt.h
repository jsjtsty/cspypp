#pragma once

#include <json/json.h>
#include <string>

namespace JsonExt {
	std::wstring readString(const Json::Value& _val);
}