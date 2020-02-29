#pragma once
#include <map>
#include <string>

class ArgParser final
{
public:
	ArgParser();
	
	bool parse(int argc, wchar_t** argv);

	bool setDefault(const std::wstring_view arg, const std::wstring_view val);
	bool hasArg(std::initializer_list<const std::wstring_view> list);

private:
	std::map<std::wstring, std::wstring> data;
};

