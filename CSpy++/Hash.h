#pragma once
#include <string>

class Hash
{
public:
	Hash();
	Hash(const unsigned char* _val);

	explicit operator std::wstring() const;
	explicit operator std::string() const;
	explicit operator const wchar_t*() const;
	explicit operator const char*() const;
	operator unsigned char*();
	operator const unsigned char*() const;

	bool operator==(const Hash& _val) const;
	bool operator!=(const Hash& _val) const;

	std::wstring toString() const;

	bool parse(const std::wstring& _val);
	bool parse(const wchar_t* _val);
protected:
	unsigned char _data[32];
};
