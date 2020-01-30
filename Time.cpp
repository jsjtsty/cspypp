#include "Time.h"
using namespace std;

namespace {
	uint32_t calcDays(uint16_t year, uint16_t month, uint16_t day) {
		uint32_t days = 0;
		uint32_t dayOfMonth[12] = { 31,28,31,30,31,30,31,31,30,31,30,31 };

		if (year < 1582) {
			int leapYears = (year - 1) / 4;
			int commonYears = year - leapYears - 1;
			days += leapYears * 366 + commonYears * 365;
			if (Time::isLeapYear(year)) {
				dayOfMonth[1] = 29;
			}
			for (int i = 0; i < month - 1; ++i) {
				days += dayOfMonth[i];
			}
			days += day;
		}
		else if (year == 1582) {
			days = 577460U;
			dayOfMonth[9] = 21;
			for (int i = 0; i < month - 1; ++i) {
				days += dayOfMonth[i];
			}
			if (month != 10) {
				days += day;
			}
			else {
				if (day <= 4) {
					days += day;
				}
				else {
					days += day - 10;
				}
			}
		}
		else {
			days = 577815U;
			int leapYears = (year - 1 - 1580) / 4;
			int centuries = (year - 1 - 1582) / 100;
			leapYears -= centuries - centuries / 4;
			int commonYears = year - 1 - 1582 - leapYears;
			days += leapYears * 366 + commonYears * 365;
			if (Time::isLeapYear(year)) {
				dayOfMonth[1] = 29;
			}
			for (int i = 0; i < month - 1; ++i) {
				days += dayOfMonth[i];
			}
			days += day;
		}
		return days;
	}
}

Time::Time(const SYSTEMTIME & _val, bool local)
{
	SYSTEMTIME st = _val;
	if (local) {
		TIME_ZONE_INFORMATION info;
		GetTimeZoneInformation(&info);
		TzSpecificLocalTimeToSystemTime(&info, &_val, &st);
	}
	year = st.wYear; month = st.wMonth; day = st.wDay;
	dayOfWeek = st.wDayOfWeek; hour = st.wHour; minute = st.wMinute;
	second = st.wSecond; milliseconds = st.wMilliseconds;
}

Time::Time(const FILETIME & _val, bool local)
{
	SYSTEMTIME st;
	if (local) {
		SYSTEMTIME st2;
		TIME_ZONE_INFORMATION info;
		GetTimeZoneInformation(&info);
		LocalFileTimeToLocalSystemTime(&info, &_val, &st2);
		TzSpecificLocalTimeToSystemTime(&info, &st2, &st);
	}
	else {
		FileTimeToSystemTime(&_val, &st);
	}
	year = st.wYear; month = st.wMonth; day = st.wDay;
	dayOfWeek = st.wDayOfWeek; hour = st.wHour; minute = st.wMinute;
	second = st.wSecond; milliseconds = st.wMilliseconds;
}

Time::Time(uint16_t year, uint16_t month, uint16_t day, uint16_t hour, uint16_t minute, uint16_t second, uint16_t milliseconds)
{
	// 1. Check if values are valid.
	bool valid = true;
	uint16_t dayOfMonth[12] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
	if (isLeapYear(year)) {
		dayOfMonth[1] = 29;
	}

	if (year == 0 || month == 0 || month >= 13 || day == 0 || day > dayOfMonth[month - 1]) {
		valid = false;
	}
	if (hour >= 24 || minute >= 60 || second >= 60 || milliseconds >= 1000) {
		valid = false;
	}
	if (year == 1582 && month == 10 && day >= 5 && day <= 14) {
		valid = false;
	}
	if (!valid) {
		throw TimeError("Invalid time.");
	}

	// 2. Set values of the class.
	this->year = year; this->month = month; this->day = day;
	this->hour = hour; this->minute = minute; this->second = second;
	this->milliseconds = milliseconds;

	// 3. Calculate day of week (Using Zeller formula).
	int y = year, m = month, d = day, c = year / 100;
	bool newFormula = true;
	if (year < 1582 || (year == 1582 && month <= 9) || (year == 1582 && month == 10 && day <= 4)) {
		newFormula = false;
	}

	if (newFormula) {
		dayOfWeek = (c / 4 - 2 * c + y + y / 4 + (m + 1) * 13 / 5 + d - 1) % 7;
	}
	else {
		dayOfWeek = (y + y / 4 + c / 4 - 2 * c + 13 * (m + 1) / 5 + d + 3) % 7;
	}
}

SYSTEMTIME Time::toSystemTime() const
{
	SYSTEMTIME res;
	res.wYear = year; res.wMonth = month; res.wDay = day;
	res.wDayOfWeek = dayOfWeek; res.wHour = hour; res.wMinute = minute;
	res.wSecond = second; res.wMilliseconds = milliseconds;
	return res;
}

SYSTEMTIME Time::toLocalSystemTime() const
{
	SYSTEMTIME res, utc = this->toSystemTime();
	TIME_ZONE_INFORMATION info;
	GetTimeZoneInformation(&info);
	SystemTimeToTzSpecificLocalTime(&info, &utc, &res);
	return res;
}

FILETIME Time::toFileTime() const
{
	SYSTEMTIME st = this->toSystemTime();
	FILETIME res;
	SystemTimeToFileTime(&st, &res);
	return res;
}

FILETIME Time::toLocalFileTime() const
{
	FILETIME res, utc = this->toFileTime();
	FileTimeToLocalFileTime(&utc, &res);
	return res;
}

wstring Time::toString() const
{
	wchar_t buffer[64];
	_snwprintf_s(buffer, 64, 63, L"%u/%u/%u %02u:%02u:%02u.%03u", year, month, day, hour, minute, second, milliseconds);
	return buffer;
}

string Time::toANSIString() const
{
	char buffer[64];
	_snprintf_s(buffer, 64, 63, "%u/%u/%u %02u:%02u:%02u.%03u", year, month, day, hour, minute, second, milliseconds);
	return buffer;
}

wstring Time::format(const wstring& _format) const
{
	// 1. Analyze buffer size.
	constexpr const wchar_t* const week[7] = { L"Sunday",L"Monday",L"Tuesday",L"Wednesday",L"Thursday",L"Friday",L"Saturday" };
	constexpr const size_t weekLength[7] = { 6,6,7,9,8,6,7 };
	bool escape = false, escape2 = false;
	size_t length = 0;
	for (wstring::const_iterator i = _format.cbegin(); i != _format.cend(); ++i) {
		if (escape) {
			escape = false;
			switch (*i) {
			case L'%':
				++length;
				break;
			case L'$':
				escape = false;
				escape2 = true;
				break;
			case L'y': case L'm': case L'd':
			case L'H': case L'M': case L'S':
				length += 2;
				break;
			case L'q':
				length += 3;
				break;
			case L'Y':
				length += static_cast<size_t>(log10(year)) + 1;
				break;
			case L'w':
				++length;
				break;
			case L'W':
				length += weekLength[dayOfWeek];
			}
		}
		else if (escape2) {
			escape2 = false;
			switch (*i) {
			case L'$':
				++length;
				break;
			case L'%':
				escape = true;
				escape2 = false;
				break;
			case L'y':
				length += 2;
				break;
			case L'Y':
				length += static_cast<size_t>(log10(year)) + 1;
				break;
			case L'm':
				length += static_cast<size_t>(log10(month)) + 1;
				break;
			case L'd':
				length += static_cast<size_t>(log10(day)) + 1;
				break;
			case L'H':
				length += static_cast<size_t>(log10(hour)) + 1;
				break;
			case L'M':
				length += static_cast<size_t>(log10(minute)) + 1;
				break;
			case L'S':
				length += static_cast<size_t>(log10(second)) + 1;
				break;
			case L'q':
				length += static_cast<size_t>(log10(milliseconds)) + 1;
				break;
			case L'w':
				++length;
				break;
			case L'W':
				length += weekLength[dayOfWeek];
				break;
			}
		}
		else {
			if (*i == L'%') {
				escape = true;
				escape2 = false;
			}
			else if (*i == L'$') {
				escape = false;
				escape2 = true;
			}
			else {
				++length;
			}
		}
	}

	// 2. Generate formatted string.
	wchar_t* buffer = new wchar_t[length + 1];
	int p = 0;
	escape = false; escape2 = false;
	for (wstring::const_iterator i = _format.cbegin(); i != _format.cend(); ++i) {
		if (escape) {
			escape = false;
			switch (*i) {
			case L'%':
				buffer[p++] = L'%';
				break;
			case L'$':
				escape = false;
				escape2 = true;
				break;
			case L'y':
			{
				uint16_t y = year % 100;
				buffer[p++] = L'0' + y / 10;
				buffer[p++] = L'0' + y % 10;
				break;
			}
			case L'Y':
			{
				int length = static_cast<int>(log10(year)) + 1;
				uint16_t y = year;
				for (int i = length; i > 0; --i) {
					uint16_t m = static_cast<uint16_t>(pow(10, i - 1));
					buffer[p++] = L'0' + y / m;
					y %= m;
				}
				break;
			}
			case L'm':
				buffer[p++] = L'0' + month / 10;
				buffer[p++] = L'0' + month % 10;
				break;
			case L'd':
				buffer[p++] = L'0' + day / 10;
				buffer[p++] = L'0' + day % 10;
				break;
			case L'H':
				buffer[p++] = L'0' + hour / 10;
				buffer[p++] = L'0' + hour % 10;
				break;
			case L'M':
				buffer[p++] = L'0' + minute / 10;
				buffer[p++] = L'0' + minute % 10;
				break;
			case L'S':
				buffer[p++] = L'0' + second / 10;
				buffer[p++] = L'0' + second % 10;
				break;
			case L'q':
				buffer[p++] = L'0' + milliseconds / 100;
				buffer[p++] = L'0' + milliseconds / 10 % 10;
				buffer[p++] = L'0' + milliseconds % 10;
				break;
			case L'w':
				buffer[p++] = L'0' + dayOfWeek;
				break;
			case L'W':
				for (size_t i = 0; i < weekLength[dayOfWeek]; ++i) {
					buffer[p++] = week[dayOfWeek][i];
				}
				break;
			}
		}
		else if (escape2) {
			escape2 = false;
			switch (*i) {
			case L'$':
				buffer[p++] = L'$';
				break;
			case L'%':
				escape = true;
				escape2 = false;
				break;
			case L'y':
				buffer[p++] = L'0' + year / 10;
				buffer[p++] = L'0' + year % 10;
				break;
			case L'Y': case L'm': case L'd':
			case L'H': case L'M': case L'S':
			case L'q':
			{
				const uint16_t* ptr = nullptr;
				switch (*i) {
				case L'Y':
					ptr = &year;
					break;
				case L'm':
					ptr = &month;
					break;
				case L'd':
					ptr = &day;
					break;
				case L'H':
					ptr = &hour;
					break;
				case L'M':
					ptr = &minute;
					break;
				case L'S':
					ptr = &second;
					break;
				case L'q':
					ptr = &milliseconds;
					break;
				}
				int length = static_cast<int>(log10(*ptr)) + 1;
				uint16_t pn = *ptr;
				for (int i = length; i > 0; --i) {
					uint16_t m = static_cast<uint16_t>(pow(10, i - 1));
					buffer[p++] = L'0' + pn / m;
					pn %= m;
				}
				break;
			}
			case L'w':
				buffer[p++] = L'0' + dayOfWeek;
				break;
			case L'W':
				for (size_t i = 0; i < weekLength[dayOfWeek]; ++i) {
					buffer[p++] = week[dayOfWeek][i];
				}
				break;
			}
		}
		else {
			if (*i == L'%') {
				escape = true;
				escape2 = false;
			}
			else if (*i == L'$') {
				escape = false;
				escape2 = true;
			}
			else {
				buffer[p++] = *i;
			}
		}
	}
	buffer[p] = L'\0';
	wstring res = buffer;
	delete[] buffer;
	return res;
}

string Time::format(const string & _format) const
{
	// 1. Analyze buffer size.
	constexpr const char* const week[7] = { "Sunday","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday" };
	constexpr const size_t weekLength[7] = { 6,6,7,9,8,6,7 };
	bool escape = false, escape2 = false;
	size_t length = 0;
	for (string::const_iterator i = _format.cbegin(); i != _format.cend(); ++i) {
		if (escape) {
			escape = false;
			switch (*i) {
			case '%':
				++length;
				break;
			case '$':
				escape = false;
				escape2 = true;
				break;
			case 'y': case 'm': case 'd':
			case 'H': case 'M': case 'S':
				length += 2;
				break;
			case 'q':
				length += 3;
				break;
			case 'Y':
				length += static_cast<size_t>(log10(year)) + 1;
				break;
			case 'w':
				++length;
				break;
			case 'W':
				length += weekLength[dayOfWeek];
			}
		}
		else if (escape2) {
			escape2 = false;
			switch (*i) {
			case '$':
				++length;
				break;
			case '%':
				escape = true;
				escape2 = false;
				break;
			case 'y':
				length += 2;
				break;
			case 'Y':
				length += static_cast<size_t>(log10(year)) + 1;
				break;
			case 'm':
				length += static_cast<size_t>(log10(month)) + 1;
				break;
			case 'd':
				length += static_cast<size_t>(log10(day)) + 1;
				break;
			case 'H':
				length += static_cast<size_t>(log10(hour)) + 1;
				break;
			case 'M':
				length += static_cast<size_t>(log10(minute)) + 1;
				break;
			case 'S':
				length += static_cast<size_t>(log10(second)) + 1;
				break;
			case 'q':
				length += static_cast<size_t>(log10(milliseconds)) + 1;
				break;
			case 'w':
				++length;
				break;
			case 'W':
				length += weekLength[dayOfWeek];
				break;
			}
		}
		else {
			if (*i == '%') {
				escape = true;
				escape2 = false;
			}
			else if (*i == '$') {
				escape = false;
				escape2 = true;
			}
			else {
				++length;
			}
		}
	}

	// 2. Generate formatted string.
	char* buffer = new char[length + 1];
	int p = 0;
	escape = false; escape2 = false;
	for (string::const_iterator i = _format.cbegin(); i != _format.cend(); ++i) {
		if (escape) {
			escape = false;
			switch (*i) {
			case '%':
				buffer[p++] = '%';
				break;
			case '$':
				escape = false;
				escape2 = true;
				break;
			case 'y':
			{
				uint16_t y = year % 100;
				buffer[p++] = '0' + y / 10;
				buffer[p++] = '0' + y % 10;
				break;
			}
			case 'Y':
			{
				int length = static_cast<int>(log10(year)) + 1;
				uint16_t y = year;
				for (int i = length; i > 0; --i) {
					uint16_t m = static_cast<uint16_t>(pow(10, i - 1));
					buffer[p++] = '0' + y / m;
					y %= m;
				}
				break;
			}
			case 'm':
				buffer[p++] = '0' + month / 10;
				buffer[p++] = '0' + month % 10;
				break;
			case 'd':
				buffer[p++] = '0' + day / 10;
				buffer[p++] = '0' + day % 10;
				break;
			case 'H':
				buffer[p++] = '0' + hour / 10;
				buffer[p++] = '0' + hour % 10;
				break;
			case 'M':
				buffer[p++] = '0' + minute / 10;
				buffer[p++] = '0' + minute % 10;
				break;
			case 'S':
				buffer[p++] = '0' + second / 10;
				buffer[p++] = '0' + second % 10;
				break;
			case 'q':
				buffer[p++] = '0' + milliseconds / 100;
				buffer[p++] = '0' + milliseconds / 10 % 10;
				buffer[p++] = '0' + milliseconds % 10;
				break;
			case 'w':
				buffer[p++] = '0' + dayOfWeek;
				break;
			case 'W':
				for (size_t i = 0; i < weekLength[dayOfWeek]; ++i) {
					buffer[p++] = week[dayOfWeek][i];
				}
				break;
			}
		}
		else if (escape2) {
			escape2 = false;
			switch (*i) {
			case '$':
				buffer[p++] = '$';
				break;
			case '%':
				escape = true;
				escape2 = false;
				break;
			case 'y':
				buffer[p++] = '0' + year / 10;
				buffer[p++] = '0' + year % 10;
				break;
			case 'Y': case 'm': case 'd':
			case 'H': case 'M': case 'S':
			case 'q':
			{
				const uint16_t* ptr = nullptr;
				switch (*i) {
				case 'Y':
					ptr = &year;
					break;
				case 'm':
					ptr = &month;
					break;
				case 'd':
					ptr = &day;
					break;
				case 'H':
					ptr = &hour;
					break;
				case 'M':
					ptr = &minute;
					break;
				case 'S':
					ptr = &second;
					break;
				case 'q':
					ptr = &milliseconds;
					break;
				}
				int length = static_cast<int>(log10(*ptr)) + 1;
				uint16_t pn = *ptr;
				for (int i = length; i > 0; --i) {
					uint16_t m = static_cast<uint16_t>(pow(10, i - 1));
					buffer[p++] = '0' + pn / m;
					pn %= m;
				}
				break;
			}
			case 'w':
				buffer[p++] = '0' + dayOfWeek;
				break;
			case 'W':
				for (size_t i = 0; i < weekLength[dayOfWeek]; ++i) {
					buffer[p++] = week[dayOfWeek][i];
				}
				break;
			}
		}
		else {
			if (*i == '%') {
				escape = true;
				escape2 = false;
			}
			else if (*i == '$') {
				escape = false;
				escape2 = true;
			}
			else {
				buffer[p++] = *i;
			}
		}
	}
	buffer[p] = '\0';
	string res = buffer;
	delete[] buffer;
	return res;
}

Time& Time::parse(const wstring & str)
{
	uint16_t y, mo, d, h, min, s, ms;
	int res = _snwscanf_s(str.c_str(), str.length(), L"%hu/%hu/%hu %02hu:%02hu:%02hu.%03hu", &y, &mo, &d, &h, &min, &s, &ms);
	if (res == EOF) {
		throw TimeError("Unsupported format.");
		return *this;
	}
	else if (res != 7) {
		throw TimeError("Invalid time string.");
		return *this;
	}
	year = y; month = mo; day = d;
	hour = h; minute = min; second = s;
	milliseconds = ms;
	return *this;
}

Time& Time::parse(const string & str)
{
	uint16_t y, mo, d, h, min, s, ms;
	int res = _snscanf_s(str.c_str(), str.length(), "%hu/%hu/%hu %02hu:%02hu:%02hu.%03hu", &y, &mo, &d, &h, &min, &s, &ms);
	if (res == EOF) {
		throw TimeError("Unsupported format.");
		return *this;
	}
	else if (res != 7) {
		throw TimeError("Invalid time string.");
		return *this;
	}
	year = y; month = mo; day = d;
	hour = h; minute = min; second = s;
	milliseconds = ms;
	return *this;
}

Time Time::getLocalTime()
{
	SYSTEMTIME st;
	GetLocalTime(&st);
	return st;
}

Time Time::getSystemTime()
{
	SYSTEMTIME st;
	GetSystemTime(&st);
	return st;
}

bool Time::isLeapYear(uint16_t year)
{
	if (year % 4 != 0) {
		return false;
	}
	else {
		if (year > 1582 && year % 100 == 0 && year % 400 != 0) {
			return false;
		}
		else {
			return true;
		}
	}
}

inline uint16_t Time::getYear() const noexcept
{
	return year;
}

inline uint16_t Time::getMonth() const noexcept
{
	return month;
}

inline uint16_t Time::getDay() const noexcept
{
	return day;
}

inline uint16_t Time::getHour() const noexcept
{
	return year;
}

inline uint16_t Time::getMinute() const noexcept
{
	return minute;
}

inline uint16_t Time::getSecond() const noexcept
{
	return second;
}

inline uint16_t Time::getMilliseconds() const noexcept
{
	return milliseconds;
}

void Time::setYear(uint16_t _val)
{
	if (_val == 0) {
		throw TimeError("Invalid year. (year cannot be 0)");
	}
	else {
		year = _val;
	}
}

void Time::setMonth(uint16_t _val)
{
	if (_val == 0 || _val > 12) {
		throw TimeError("Invalid month. (month >= 12)");
	}
	else {
		month = _val;
	}
}

void Time::setDay(uint16_t _val)
{
	uint16_t dayOfMonth[12] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
	if (isLeapYear(year))
		dayOfMonth[1] = 29;

	if (year == 1528 && month == 10 && _val >= 5 && _val <= 14) {
		throw TimeError("Invalid date. (1582.10.5~14 is not valid)");
	}
	else if (_val > dayOfMonth[month - 1]) {
		throw TimeError("Invalid date. (Day does not exist)");
	}
	else {
		day = _val;
	}
}

void Time::setHour(uint16_t _val)
{
	if (_val >= 24) {
		throw TimeError("Invalid time. (hour >= 24)");
	}
	else {
		hour = _val;
	}
}

void Time::setMinute(uint16_t _val)
{
	if (_val >= 60) {
		throw TimeError("Invalid time. (minute >= 60)");
	}
	else {
		minute = _val;
	}
}

void Time::setSecond(uint16_t _val)
{
	if (_val >= 60) {
		throw TimeError("Invalid time. (second >= 60)");
	}
	else {
		second = _val;
	}
}

void Time::setMilliseconds(uint16_t _val)
{
	if (_val >= 1000) {
		throw TimeError("Invalid time. (milliseconds >= 1000)");
	}
	else {
		milliseconds = _val;
	}
}

Time::operator FILETIME() const
{
	return this->toFileTime();
}

Time::operator SYSTEMTIME() const
{
	return this->toSystemTime();
}

TimePeriod Time::operator-(const Time & other) const
{
	TimePeriod tp;
	uint16_t y, m, d;
	analyseDate(calcDays(488, 12, 31), &y, &m, &d);
	return TimePeriod();
}

bool Time::operator==(const Time & other) const
{
	return year == other.year && month == other.month && day == other.day && hour == other.hour
		&& minute == other.minute && second == other.second && milliseconds == other.milliseconds;
}

bool Time::operator!=(const Time & other) const
{
	return !(*this == other);
}

bool Time::operator<(const Time & other) const
{
	if (year < other.year)
		return true;
	else if (year > other.year)
		return false;
	else {
		if (month < other.month)
			return true;
		else if (month > other.month)
			return false;
		else {
			if (day < other.day)
				return true;
			else if (day > other.day)
				return false;
			else {
				uint32_t m1 = uint32_t(hour) * 60 * 60 * 1000 + uint32_t(minute) * 60 * 1000 + uint32_t(second) * 1000 + milliseconds,
					m2 = uint32_t(other.hour) * 60 * 60 * 1000 + uint32_t(other.minute) * 60 * 1000 + uint32_t(other.second) * 1000 + other.milliseconds;
				return m1 < m2;
			}
		}
	}
}

bool Time::operator>(const Time & other) const
{
	if (year < other.year)
		return false;
	else if (year > other.year)
		return true;
	else {
		if (month < other.month)
			return false;
		else if (month > other.month)
			return true;
		else {
			if (day < other.day)
				return false;
			else if (day > other.day)
				return true;
			else {
				uint32_t m1 = uint32_t(hour) * 60 * 60 * 1000 + uint32_t(minute) * 60 * 1000 + uint32_t(second) * 1000 + milliseconds,
					m2 = uint32_t(other.hour) * 60 * 60 * 1000 + uint32_t(other.minute) * 60 * 1000 + uint32_t(other.second) * 1000 + other.milliseconds;
				return m1 > m2;
			}
		}
	}
}

bool Time::operator<=(const Time & other) const
{
	if (year < other.year)
		return true;
	else if (year > other.year)
		return false;
	else {
		if (month < other.month)
			return true;
		else if (month > other.month)
			return false;
		else {
			if (day < other.day)
				return true;
			else if (day > other.day)
				return false;
			else {
				uint32_t m1 = uint32_t(hour) * 60 * 60 * 1000 + uint32_t(minute) * 60 * 1000 + uint32_t(second) * 1000 + milliseconds,
					m2 = uint32_t(other.hour) * 60 * 60 * 1000 + uint32_t(other.minute) * 60 * 1000 + uint32_t(other.second) * 1000 + other.milliseconds;
				return m1 <= m2;
			}
		}
	}
}

bool Time::operator>=(const Time & other) const
{
	if (year < other.year)
		return false;
	else if (year > other.year)
		return true;
	else {
		if (month < other.month)
			return false;
		else if (month > other.month)
			return true;
		else {
			if (day < other.day)
				return false;
			else if (day > other.day)
				return true;
			else {
				uint32_t m1 = uint32_t(hour) * 60 * 60 * 1000 + uint32_t(minute) * 60 * 1000 + uint32_t(second) * 1000 + milliseconds,
					m2 = uint32_t(other.hour) * 60 * 60 * 1000 + uint32_t(other.minute) * 60 * 1000 + uint32_t(other.second) * 1000 + other.milliseconds;
				return m1 >= m2;
			}
		}
	}
}

TimePeriod::TimePeriod(uint32_t days, uint16_t hour, uint16_t minute, uint16_t second, uint16_t milliseconds, Symbol symbol)
{
	if (hour >= 24 || minute >= 60 || second >= 60 || milliseconds >= 1000) {
		throw TimeError("Invalid time period.");
	}

	this->days = days; this->hour = hour; this->minute = minute;
	this->second = second; this->milliseconds = milliseconds;
	if (!(days || hour || second || milliseconds)) {
		symbol = ZERO;
	}
	this->symbol = symbol;
}

wstring TimePeriod::toString() const
{
	wchar_t buffer[64];
	if (symbol != NEGATIVE) {
		_snwprintf_s(buffer, 64, 63, L"%ud%uh%umin%u.%03us", days, hour, minute, second, milliseconds);
	}
	else {
		_snwprintf_s(buffer, 64, 63, L"-%ud%uh%umin%u.%03us", days, hour, minute, second, milliseconds);
	}
	return buffer;
}

string TimePeriod::toANSIString() const
{
	char buffer[64];
	if (symbol != NEGATIVE) {
		_snprintf_s(buffer, 64, 63, "%ud%uh%umin%u.%03us", days, hour, minute, second, milliseconds);
	}
	else {
		_snprintf_s(buffer, 64, 63, "-%ud%uh%umin%u.%03us", days, hour, minute, second, milliseconds);
	}
	return buffer;
}

inline uint32_t TimePeriod::getDays() const noexcept
{
	return days;
}

inline uint16_t TimePeriod::getHour() const noexcept
{
	return hour;
}

inline uint16_t TimePeriod::getMinute() const noexcept
{
	return minute;
}

inline uint16_t TimePeriod::getSecond() const noexcept
{
	return second;
}

inline uint16_t TimePeriod::getMilliseconds() const noexcept
{
	return milliseconds;
}

inline TimePeriod::Symbol TimePeriod::getSymbol() const noexcept
{
	return symbol;
}

void TimePeriod::setDays(uint32_t _val) noexcept
{
	days = _val;
}

void TimePeriod::setHour(uint16_t _val)
{
	if (_val >= 24) {
		throw TimeError("Invalid time. (hour >= 24)");
	}
	else {
		hour = _val;
	}
}

void TimePeriod::setMinute(uint16_t _val)
{
	if (_val >= 60) {
		throw TimeError("Invalid time. (minute >= 60)");
	}
	else {
		minute = _val;
	}
}

void TimePeriod::setSecond(uint16_t _val)
{
	if (_val >= 60) {
		throw TimeError("Invalid time. (second >= 60)");
	}
	else {
		second = _val;
	}
}

void TimePeriod::setMilliseconds(uint16_t _val)
{
	if (_val >= 1000) {
		throw TimeError("Invalid time. (millisecond >= 1000)");
	}
	else {
		milliseconds = _val;
	}
}

void TimePeriod::setSymbol(Symbol _val)
{
	if (_val != POSITIVE && _val != NEGATIVE && _val != ZERO) {
		throw TimeError("Invalid symbol.");
	}
	else if (!(days || hour || minute || second || milliseconds)) {
		symbol = ZERO;
	}
	else {
		symbol = _val;
	}
}

TimePeriod TimePeriod::operator+(const TimePeriod & other) const
{
	TimePeriod tp;
	int64_t ires = (days * 86400000LL + hour * 3600000LL + minute * 60000LL + second * 1000LL + milliseconds) * symbol
		+ (other.days * 86400000LL + other.hour * 3600000LL + other.minute * 60000LL + other.second * 1000LL + other.milliseconds) * other.symbol;
	if (ires == 0LL) {
		tp.symbol = ZERO;
	}
	else {
		if (ires > 0LL) {
			tp.symbol = POSITIVE;
		}
		else {
			tp.symbol = NEGATIVE;
		}
		tp.days = static_cast<uint32_t>(ires / 86400000LL);
		ires -= tp.days * 86400000LL;
		tp.hour = static_cast<uint16_t>(ires / 3600000LL);
		ires -= tp.hour * 3600000LL;
		tp.minute = static_cast<uint16_t>(ires / 60000LL);
		ires -= tp.minute * 60000LL;
		tp.second = static_cast<uint16_t>(ires / 1000LL);
		ires -= tp.second * 1000LL;
		tp.milliseconds = static_cast<uint16_t>(ires);
	}
	return tp;
}

TimePeriod TimePeriod::operator-(const TimePeriod & other) const
{
	TimePeriod tp;
	int64_t ires = (days * 86400000LL + hour * 3600000LL + minute * 60000LL + second * 1000LL + milliseconds) * symbol
		- (other.days * 86400000LL + other.hour * 3600000LL + other.minute * 60000LL + other.second * 1000LL + other.milliseconds) * other.symbol;
	if (ires == 0LL) {
		tp.symbol = ZERO;
	}
	else {
		if (ires > 0LL) {
			tp.symbol = POSITIVE;
		}
		else {
			tp.symbol = NEGATIVE;
		}
		tp.days = static_cast<uint32_t>(ires / 86400000LL);
		ires -= tp.days * 86400000LL;
		tp.hour = static_cast<uint16_t>(ires / 3600000LL);
		ires -= tp.hour * 3600000LL;
		tp.minute = static_cast<uint16_t>(ires / 60000LL);
		ires -= tp.minute * 60000LL;
		tp.second = static_cast<uint16_t>(ires / 1000LL);
		ires -= tp.second * 1000LL;
		tp.milliseconds = static_cast<uint16_t>(ires);
	}
	return tp;
}

TimePeriod& TimePeriod::operator+=(const TimePeriod & other)
{
	int64_t ires = (days * 86400000LL + hour * 3600000LL + minute * 60000LL + second * 1000LL + milliseconds) * symbol
		+ (other.days * 86400000LL + other.hour * 3600000LL + other.minute * 60000LL + other.second * 1000LL + other.milliseconds) * other.symbol;
	if (ires == 0LL) {
		symbol = ZERO;
	}
	else {
		if (ires > 0LL) {
			symbol = POSITIVE;
		}
		else {
			symbol = NEGATIVE;
		}
		days = static_cast<uint32_t>(ires / 86400000LL);
		ires -= days * 86400000LL;
		hour = static_cast<uint16_t>(ires / 3600000LL);
		ires -= hour * 3600000LL;
		minute = static_cast<uint16_t>(ires / 60000LL);
		ires -= minute * 60000LL;
		second = static_cast<uint16_t>(ires / 1000LL);
		ires -= second * 1000LL;
		milliseconds = static_cast<uint16_t>(ires);
	}
	return *this;
}

TimePeriod & TimePeriod::operator-=(const TimePeriod & other)
{
	int64_t ires = (days * 86400000LL + hour * 3600000LL + minute * 60000LL + second * 1000LL + milliseconds) * symbol
		- (other.days * 86400000LL + other.hour * 3600000LL + other.minute * 60000LL + other.second * 1000LL + other.milliseconds) * other.symbol;
	if (ires == 0LL) {
		symbol = ZERO;
	}
	else {
		if (ires > 0LL) {
			symbol = POSITIVE;
		}
		else {
			symbol = NEGATIVE;
		}
		days = static_cast<uint32_t>(ires / 86400000LL);
		ires -= days * 86400000LL;
		hour = static_cast<uint16_t>(ires / 3600000LL);
		ires -= hour * 3600000LL;
		minute = static_cast<uint16_t>(ires / 60000LL);
		ires -= minute * 60000LL;
		second = static_cast<uint16_t>(ires / 1000LL);
		ires -= second * 1000LL;
		milliseconds = static_cast<uint16_t>(ires);
	}
	return *this;
}

bool TimePeriod::operator==(const TimePeriod & other) const
{
	return days == other.days && hour == other.hour && minute == other.minute
		&& second == other.second && milliseconds == other.milliseconds;
}

bool TimePeriod::operator!=(const TimePeriod & other) const
{
	return !(*this == other);
}

bool TimePeriod::operator<(const TimePeriod & other) const
{
	uint64_t ms1 = uint64_t(days) * 24 * 60 * 60 * 1000 + uint64_t(hour) * 60 * 60 * 1000 + uint64_t(minute) * 60 * 1000
		+ uint64_t(second) * 1000 + milliseconds,
		ms2 = uint64_t(other.days) * 24 * 60 * 60 * 1000 + uint64_t(other.hour) * 60 * 60 * 1000 + uint64_t(other.minute) * 60 * 1000
		+ uint64_t(other.second) * 1000 + other.milliseconds;
	return ms1 < ms2;
}

bool TimePeriod::operator>(const TimePeriod & other) const
{
	uint64_t ms1 = uint64_t(days) * 24 * 60 * 60 * 1000 + uint64_t(hour) * 60 * 60 * 1000 + uint64_t(minute) * 60 * 1000
		+ uint64_t(second) * 1000 + milliseconds,
		ms2 = uint64_t(other.days) * 24 * 60 * 60 * 1000 + uint64_t(other.hour) * 60 * 60 * 1000 + uint64_t(other.minute) * 60 * 1000
		+ uint64_t(other.second) * 1000 + other.milliseconds;
	return ms1 > ms2;
}

bool TimePeriod::operator<=(const TimePeriod & other) const
{
	uint64_t ms1 = uint64_t(days) * 24 * 60 * 60 * 1000 + uint64_t(hour) * 60 * 60 * 1000 + uint64_t(minute) * 60 * 1000
		+ uint64_t(second) * 1000 + milliseconds,
		ms2 = uint64_t(other.days) * 24 * 60 * 60 * 1000 + uint64_t(other.hour) * 60 * 60 * 1000 + uint64_t(other.minute) * 60 * 1000
		+ uint64_t(other.second) * 1000 + other.milliseconds;
	return ms1 <= ms2;
}

bool TimePeriod::operator>=(const TimePeriod & other) const
{
	uint64_t ms1 = uint64_t(days) * 24 * 60 * 60 * 1000 + uint64_t(hour) * 60 * 60 * 1000 + uint64_t(minute) * 60 * 1000
		+ uint64_t(second) * 1000 + milliseconds,
		ms2 = uint64_t(other.days) * 24 * 60 * 60 * 1000 + uint64_t(other.hour) * 60 * 60 * 1000 + uint64_t(other.minute) * 60 * 1000
		+ uint64_t(other.second) * 1000 + other.milliseconds;
	return ms1 >= ms2;
}
