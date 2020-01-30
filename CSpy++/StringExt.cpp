#include "general.h"
#include "StringExt.h"
#include <Windows.h>

namespace std {
	wstring utf8_to_wstring(const string& _val)
	{
		wchar_t* stemp = new wchar_t[_val.length() + 1];
		int res = MultiByteToWideChar(CP_UTF8, 0, _val.c_str(), -1, stemp, (int)_val.length() + 1);
		wstring str = stemp;
		delete[] stemp;
		return str;
	}

	wstring utf8_to_wstring(const char* _val, int len)
	{
		wchar_t* stemp = new wchar_t[len + 1];
		int res = MultiByteToWideChar(CP_UTF8, 0, _val, -1, stemp, len + 1);
		wstring str = stemp;
		delete[] stemp;
		return str;
	}

	u8string wstring_to_utf8string(const wstring& _val)
	{
		char* stemp = new char[(_val.length() + 1) * 4];
		BOOL transformStatus;
		int res = WideCharToMultiByte(CP_UTF8, 0, _val.c_str(), -1, stemp, (int)((_val.length() + 1) * 4), NULL, &transformStatus);
		u8string str = stemp;
		delete[] stemp;
		return str;
	}

	wstring to_wstring(const string& _val)
	{
		wchar_t* stemp = new wchar_t[_val.length() + 1];
		int res = MultiByteToWideChar(CP_ACP, 0, _val.c_str(), -1, stemp, (int)_val.length() + 1);
		wstring str = stemp;
		delete[] stemp;
		return str;
	}

	string to_string(const wstring& _val)
	{
		char* stemp = new char[(_val.length() + 1) * 4];
		BOOL transformStatus;
		int res = WideCharToMultiByte(CP_ACP, 0, _val.c_str(), -1, stemp, (int)((_val.length() + 1) * 4), NULL, &transformStatus);
		string str = stemp;
		delete[] stemp;
		return str;
	}

	string trim_left(const string& _val)
	{
		size_t pos = 0;
		for (auto it = _val.cbegin(); it != _val.cend(); ++it, ++pos) {
			if (*it != ' ')
				break;
		}
		return _val.substr(pos, _val.length() - pos);
	}

	string trim_right(const string& _val)
	{
		size_t pos = _val.length();
		for (auto it = _val.crbegin(); it != _val.crend(); ++it, --pos) {
			if (*it != ' ')
				break;
		}
		return _val.substr(0, pos);
	}

	wstring trim_left(const wstring& _val)
	{
		size_t pos = 0;
		for (auto it = _val.cbegin(); it != _val.cend(); ++it, ++pos) {
			if (*it != L' ')
				break;
		}
		return _val.substr(pos, _val.length() - pos);
	}

	wstring trim_right(const wstring& _val)
	{
		size_t pos = _val.length();
		for (auto it = _val.crbegin(); it != _val.crend(); ++it, --pos) {
			if (*it != L' ')
				break;
		}
		return _val.substr(0, pos);
	}

	string trim(const string& _val)
	{
		size_t left = 0, right = _val.length();
		for (auto it = _val.cbegin(); it != _val.cend(); ++it, ++left) {
			if (*it != ' ')
				break;
		}
		for (auto it = _val.crbegin(); it != _val.crend(); ++it, --right) {
			if (*it != ' ')
				break;
		}
		return _val.substr(left, right - left);
	}

	wstring trim(const wstring& _val)
	{
		size_t left = 0, right = _val.length();
		for (auto it = _val.cbegin(); it != _val.cend(); ++it, ++left) {
			if (*it != L' ')
				break;
		}
		for (auto it = _val.crbegin(); it != _val.crend(); ++it, --right) {
			if (*it != L' ')
				break;
		}
		return _val.substr(left, right - left);
	}
}