#include "ArgParser.h"

Argument::Argument(const std::wstring_view arg)
{
	this->args.insert(arg.data());
}

Argument::Argument(std::wstring&& arg)
{
	this->args.insert(std::move(arg));
}

Argument::Argument(std::initializer_list<const std::wstring_view> args)
{
	for (const std::wstring_view& arg : args) {
		this->args.insert(arg.data());
	}
}

inline void Argument::addArgString(const std::wstring_view arg)
{
	this->args.insert(arg.data());
}

inline void Argument::addArgString(std::wstring&& arg)
{
	this->args.insert(std::move(arg));
}

inline void Argument::deleteArgString(const std::wstring_view arg)
{
	this->args.erase(arg.data());
}

bool Argument::match(const std::wstring_view arg) const
{
	return this->args.find(arg.data()) != this->args.cend();
}

bool Argument::match(const Argument& arg) const
{
	for (const std::wstring& str : arg.args) {
		if (this->match(str))
			return true;
	}
	return false;
}

inline size_t Argument::getArgCount() const
{
	return this->args.size();
}

inline void Argument::clear()
{
	this->args.clear();
}

inline bool Argument::operator==(const Argument& arg) const
{
	return this->args == arg.args;
}

inline bool Argument::operator!=(const Argument& arg) const
{
	return this->args != arg.args;
}

Argument& Argument::operator=(std::initializer_list<const std::wstring_view> args)
{
	this->args.clear();
	for (const std::wstring_view& arg : args) {
		this->args.insert(arg.data());
	}
	return *this;
}

inline Argument::const_iterator Argument::begin() const noexcept
{
	return this->args.begin();
}

inline Argument::const_iterator Argument::end() const noexcept
{
	return this->args.end();
}

inline Argument::const_iterator Argument::cbegin() const noexcept
{
	return this->args.cbegin();
}

inline Argument::const_iterator Argument::cend() const noexcept
{
	return this->args.cend();
}

inline Argument::const_reverse_iterator Argument::rbegin() const noexcept
{
	return this->args.rbegin();
}

inline Argument::const_reverse_iterator Argument::rend() const noexcept
{
	return this->args.rend();
}

inline Argument::const_reverse_iterator Argument::crbegin() const noexcept
{
	return this->args.crbegin();
}

inline Argument::const_reverse_iterator Argument::crend() const noexcept
{
	return this->args.crend();
}

bool ArgParser::parse(int argc, const wchar_t** const argv)
{
	bool raw = false;
	const wchar_t* str = *argv;
	for (int i = 0; i < argc; ++i, str = argv[i]) {

	}
}

void ArgParser::setDefault(const Argument& arg, const std::wstring_view val)
{
	for (const std::wstring& str : arg) {
		data[str] = val;
	}
}

void ArgParser::setDefault(const std::wstring_view val)
{
	data[L""] = val;
}

bool ArgParser::hasArg(const Argument& arg) const
{
	for (const std::pair<std::wstring, std::wstring>& p : this->data) {
		if (arg.match(p.first)) {
			return true;
		}
	}
	return false;
}

std::wstring ArgParser::getArg(const Argument& arg) const
{
	for (const std::pair<std::wstring, std::wstring>& p : this->data) {
		if (arg.match(p.first)) {
			return p.second;
		}
	}
	return std::wstring();
}

std::wstring ArgParser::getArg(const std::wstring_view arg) const
{
	for (const std::pair<std::wstring, std::wstring>& p : this->data) {
		if (arg == p.first) {
			return p.second;
		}
	}
	return std::wstring();
}

std::wstring ArgParser::getArg() const
{
	if (this->data.find(L"") != this->data.cend()) {
		return data.at(L"");
	}
	return std::wstring();
}
