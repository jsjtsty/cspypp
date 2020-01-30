#include "general.h"
#include "Hash.h"
#include <sstream>
#include <iomanip>
using namespace std;

Hash::Hash()
{
	memset(_data, 0, sizeof(_data));
}

Hash::Hash(const unsigned char * _val)
{
	memcpy_s(_data, sizeof(_data), _val, 32);
}

Hash::operator std::wstring() const
{
	wstringstream ss;
	for (int i = 0; i < 32; ++i) {
		ss.width(2);
		ss.fill(L'0');
		ss << hex << _data[i];
	}
	return ss.str();
}

Hash::operator std::string() const
{
	stringstream ss;
	for (int i = 0; i < 32; ++i) {
		ss.width(2);
		ss.fill('0');
		ss << hex << _data[i];
	}
	return ss.str();
}

Hash::operator const wchar_t*() const
{
	return this->operator std::wstring().c_str();
}

Hash::operator const char*() const
{
	return this->operator std::string().c_str();
}

Hash::operator unsigned char*()
{
	return _data;
}

Hash::operator const unsigned char*() const
{
	return _data;
}

bool Hash::operator==(const Hash & _val) const
{
	return memcmp(_data, _val._data, 32) == 0;
}

bool Hash::operator!=(const Hash & _val) const
{
	return memcmp(_data, _val._data, 32) != 0;
}

wstring Hash::toString() const
{
	return this->operator std::wstring();
}

bool Hash::parse(const std::wstring & _val)
{
	return parse(_val.c_str());
}

bool Hash::parse(const wchar_t* _val)
{
	size_t len = wcslen(_val);
	wchar_t* str = new wchar_t[len + 1];
	size_t i = 0, j = 0;
	for (; i < len; ++i) {
		if (_val[i] != L' ') {
			str[j] = _val[i];
			++j;
		}
		if (!((_val[i] >= L'0' && _val[i] <= L'9') || (_val[i] >= L'a' && _val[i] <= L'f') || (_val[i] >= L'A' && _val[i] <= L'F'))) {
			delete[] str;
			return false;
		}
	}
	str[j] = L'\0';
	if (j != 64) {
		delete[] str;
		return false;
	}

	for (size_t p = 0; p < 32; ++p) {
		_data[p] = 0;

		if (str[p * 2] >= L'0' && str[p * 2] <= L'9') {
			_data[p] += static_cast<unsigned char>(str[p * 2] - L'0') << 4;
		}
		else if (str[p * 2] >= L'A' && str[p * 2] <= L'F') {
			_data[p] += static_cast<unsigned char>(str[p * 2] - L'A' + 10) << 4;
		}
		else {
			_data[p] += static_cast<unsigned char>(str[p * 2] - L'a' + 10) << 4;
		}

		if (str[p * 2 + 1] >= L'0' && str[p * 2 + 1] <= L'9') {
			_data[p] += str[p * 2 + 1] - L'0';
		}
		else if (str[p * 2 + 1] >= L'A' && str[p * 2 + 1] <= L'F') {
			_data[p] += str[p * 2 + 1] - L'A' + 10;
		}
		else {
			_data[p] += str[p * 2 + 1] - L'a' + 10;
		}
	}
	delete[] str;

	return true;
}
