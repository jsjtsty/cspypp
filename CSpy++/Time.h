#pragma once
#include <Windows.h>
#include <cstdint>
#include <string>
#include <stdexcept>

class Time
{
public:
	Time() {}
	Time(const SYSTEMTIME& _val, bool local = false);
	Time(const FILETIME& _val, bool local = false);
	Time(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second, uint16_t milliseconds = 0);
	
	SYSTEMTIME toSystemTime() const;
	SYSTEMTIME toLocalSystemTime() const;
	FILETIME toFileTime() const;
	FILETIME toLocalFileTime() const;

	std::wstring toString(bool local = true) const;
	std::string toANSIString(bool local = true) const;
	std::wstring format(const std::wstring& _format) const;
	std::string format(const std::string& _format) const;

	Time& parse(const std::wstring& str);
	Time& parse(const std::string& str);

	uint16_t getYear() const noexcept;
	uint16_t getMonth() const noexcept;
	uint16_t getDay() const noexcept;
	uint16_t getHour() const noexcept;
	uint16_t getMinute() const noexcept;
	uint16_t getSecond() const noexcept;
	uint16_t getMilliseconds() const noexcept;

	void setYear(uint16_t _val);
	void setMonth(uint8_t _val);
	void setDay(uint8_t _val);
	void setHour(uint8_t _val);
	void setMinute(uint8_t _val);
	void setSecond(uint8_t _val);
	void setMilliseconds(uint16_t _val);
	void setTime(FILETIME _val, bool local = false);
	void setTime(SYSTEMTIME _val, bool local = false);

	operator FILETIME() const;
	operator SYSTEMTIME() const;

	bool operator==(const Time& other) const;
	bool operator!=(const Time& other) const;
	bool operator<(const Time& other) const;
	bool operator>(const Time& other) const;
	bool operator<=(const Time& other) const;
	bool operator>=(const Time& other) const;

	static Time getLocalTime();
	static Time getSystemTime();
	static bool isLeapYear(uint16_t year);

protected:
	uint16_t year = 0, milliseconds = 0;
	uint8_t month = 0, day = 0, hour = 0, minute = 0, second = 0, dayOfWeek = 0;
};

class TimeError : public std::logic_error
{
public:
	explicit TimeError(const std::string& _Message) : std::logic_error(_Message) {}
	explicit TimeError(const char* _Message) : std::logic_error(_Message) {}
};