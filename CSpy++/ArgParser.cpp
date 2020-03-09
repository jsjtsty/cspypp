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

bool ArgParser::parse(int argc, const wchar_t* argv[])
{
	bool raw = false;
	const wchar_t* arg;
	std::wstring* val = nullptr;
	size_t length;
	for (int i = 0; i < argc; ++i) {
		arg = argv[i];
		if (raw) {
			length = wcslen(arg);
			const wchar_t* end = arg + length;
			std::wstring temp;
			for (const wchar_t* ptr = arg; ptr != end; ++ptr) {
				if (*ptr == L'\"') {
					if (ptr != arg && *(ptr - 1) == L'\\') {
						temp.erase(temp.end() - 1);
						temp += L'\"';
						continue;
					}
					if (ptr != end - 1) {
						this->data.clear();
						return false;
					}
					else {
						raw = false;
					}
				}
			}
			val->append(temp);
			if (raw) {
				*val += L' ';
			}
		}
		else {
			if (*arg == L'-' || *arg == L'/') {
				val = &this->data[arg + 1];
			}
			else {
				if (*arg == L'\"') {
					raw = true;
					const wchar_t* end = arg + wcslen(arg);
					for (const wchar_t* ptr = arg + 1; ptr != end; ++ptr) {
						if (*ptr == L'\"') {
							if (ptr == end - 1) {
								raw = false;
							}
							else if (*(ptr - 1) == L'\\') {
								val->erase(val->end() - 1);
								*val += L'\"';
							}
							else {
								this->data.clear();
								return false;
							}
						}
						else {
							*val += *ptr;
						}
					}
				}
				else {
					if (val != nullptr) {

					}
					else {

					}
				}
			}
		}
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
	this->command.clear();
	this->command.push_back(val.data());
}

void ArgParser::setDefault(const std::vector<std::wstring>&& val)
{
	this->command = std::move(val);
}

void ArgParser::setDefault(const std::vector<std::wstring>& val)
{
	this->command = val;
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

inline const std::vector<std::wstring>& ArgParser::getArg() const noexcept
{
	return this->command;
}
