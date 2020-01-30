#include "general.h"
#include <string>
#include <json/json.h>
#include "StringExt.h"
using namespace std;

namespace JsonExt
{
	wstring read_wstring(const Json::Value& _val) {
		const char *begin, *end;
		_val.getString(&begin, &end);
		return utf8_to_wstring(begin, static_cast<int>(end - begin));
	}
}