#pragma once
#include <vector>
#include <map>
#include <set>
#include <string>

class Argument {
public:
	Argument() {}
	Argument(const std::wstring_view arg);
	Argument(std::wstring&& arg);
	Argument(std::initializer_list<const std::wstring_view> args);
	
	void addArgString(const std::wstring_view arg);
	void addArgString(std::wstring&& arg);
	void deleteArgString(const std::wstring_view arg);

	bool match(const std::wstring_view arg) const;
	bool match(const Argument& arg) const;

	size_t getArgCount() const;
	void clear();

	bool operator==(const Argument& arg) const;
	bool operator!=(const Argument& arg) const;
	Argument& operator=(std::initializer_list<const std::wstring_view> args);

	using iterator = std::set<std::wstring>::iterator;
	using const_iterator = std::set<std::wstring>::const_iterator;
	using reverse_iterator = std::set<std::wstring>::reverse_iterator;
	using const_reverse_iterator = std::set<std::wstring>::const_reverse_iterator;

	const_iterator begin() const noexcept;
	const_iterator end() const noexcept;
	const_iterator cbegin() const noexcept;
	const_iterator cend() const noexcept;
	const_reverse_iterator rbegin() const noexcept;
	const_reverse_iterator rend() const noexcept;
	const_reverse_iterator crbegin() const noexcept;
	const_reverse_iterator crend() const noexcept;

protected:
	std::set<std::wstring> args;
};

class ArgParser
{
public:
	ArgParser() = default;
	
	bool parse(int argc, const wchar_t* argv[]);

	void setDefault(const Argument& arg, const std::wstring_view val);
	void setDefault(const std::wstring_view val);
	void setDefault(const std::vector<std::wstring>&& val);
	void setDefault(const std::vector<std::wstring>& val);

	bool hasArg(const Argument& arg) const;

	std::wstring getArg(const Argument& arg) const;
	std::wstring getArg(const std::wstring_view arg) const;
	const std::vector<std::wstring>& getArg() const noexcept;

protected:
	std::map<std::wstring, std::wstring> data;
	std::vector<std::wstring> command;
};

