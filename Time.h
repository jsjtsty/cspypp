#pragma once
#include <Windows.h>
#include <cstdint>
#include <string>
#include <stdexcept>
#include <ostream>

class TimePeriod;

class Time
{
public:
	Time() {}
	Time(const SYSTEMTIME& _val, bool local = false);
	Time(const FILETIME& _val, bool local = false);
	Time(uint16_t year, uint16_t month, uint16_t day, uint16_t hour, uint16_t minute, uint16_t second, uint16_t milliseconds = 0);
	
	SYSTEMTIME toSystemTime() const;
	SYSTEMTIME toLocalSystemTime() const;
	FILETIME toFileTime() const;
	FILETIME toLocalFileTime() const;

	std::wstring toString() const;
	std::string toANSIString() const;
	std::wstring format(const std::wstring& _format) const;
	std::string format(const std::string& _format) const;

	Time& parse(const std::wstring& str);
	Time& parse(const std::string& str);

	inline uint16_t getYear() const noexcept;
	inline uint16_t getMonth() const noexcept;
	inline uint16_t getDay() const noexcept;
	inline uint16_t getHour() const noexcept;
	inline uint16_t getMinute() const noexcept;
	inline uint16_t getSecond() const noexcept;
	inline uint16_t getMilliseconds() const noexcept;

	void setYear(uint16_t _val);
	void setMonth(uint16_t _val);
	void setDay(uint16_t _val);
	void setHour(uint16_t _val);
	void setMinute(uint16_t _val);
	void setSecond(uint16_t _val);
	void setMilliseconds(uint16_t _val);

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
	uint16_t year = 0, month = 0, day = 0, hour = 0, minute = 0, second = 0, milliseconds = 0;
	uint16_t dayOfWeek = 0;
};

class TimePeriod
{
public:
	enum Symbol : int32_t {
		NEGATIVE = -1,
		ZERO = 0,
		POSITIVE = 1
	};

	TimePeriod() {}
	TimePeriod(uint32_t days, uint16_t hour, uint16_t minute, uint16_t second, uint16_t milliseconds = 0, Symbol symbol = POSITIVE);

	std::wstring toString() const;
	std::string toANSIString() const;

	inline uint32_t getDays() const noexcept;
	inline uint16_t getHour() const noexcept;
	inline uint16_t getMinute() const noexcept;
	inline uint16_t getSecond() const noexcept;
	inline uint16_t getMilliseconds() const noexcept;
	inline Symbol getSymbol() const noexcept;

	void setDays(uint32_t _val) noexcept;
	void setHour(uint16_t _val);
	void setMinute(uint16_t _val);
	void setSecond(uint16_t _val);
	void setMilliseconds(uint16_t _val);
	void setSymbol(Symbol _val);

	TimePeriod operator+(const TimePeriod& other) const;
	TimePeriod operator-(const TimePeriod& other) const;
	Time operator+(const Time& other) const;
	TimePeriod& operator+=(const TimePeriod& other);
	TimePeriod& operator-=(const TimePeriod& other);

	bool operator==(const TimePeriod& other) const;
	bool operator!=(const TimePeriod& other) const;
	bool operator<(const TimePeriod& other) const;
	bool operator>(const TimePeriod& other) const;
	bool operator<=(const TimePeriod& other) const;
	bool operator>=(const TimePeriod& other) const;

protected:
	uint32_t days = 0;
	uint16_t hour = 0, minute = 0, second = 0, milliseconds = 0;
	Symbol symbol = ZERO;
};

class TimeError : public std::logic_error
{
public:
	explicit TimeError(const std::string& _Message) : std::logic_error(_Message) {}
	explicit TimeError(const char* _Message) : std::logic_error(_Message) {}
};