#include "ArgParser.h"
using namespace std;

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

void Argument::addArgString(const std::wstring_view arg)
{
	this->args.insert(arg.data());
}

void Argument::addArgString(std::wstring&& arg)
{
	this->args.insert(std::move(arg));
}

void Argument::deleteArgString(const std::wstring_view arg)
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

size_t Argument::getArgCount() const
{
	return this->args.size();
}

void Argument::clear()
{
	this->args.clear();
}

bool Argument::operator==(const Argument& arg) const
{
	return this->args == arg.args;
}

bool Argument::operator!=(const Argument& arg) const
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

void ArgParser::setDefault(const Argument& arg, const std::wstring_view val)
{
	for (const std::wstring& str : arg) {
		std::map<int,int>::begin
	}
}
