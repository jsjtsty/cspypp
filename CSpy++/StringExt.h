#pragma once

#include <string>

namespace std {
	typedef string u8string;

	wstring to_wstring(const string& _val);

	wstring utf8_to_wstring(const char* _val, int len);
	wstring utf8_to_wstring(const char* _val, int len);
	u8string wstring_to_utf8string(const wstring& _val);

	string to_string(const wstring& _val);

	string trim_left(const string& _val);
	wstring trim_left(const wstring& _val);
	string trim_right(const string& _val);
	wstring trim_right(const wstring& _val);
	string trim(const string& _val);
	wstring trim(const wstring& _val);
}