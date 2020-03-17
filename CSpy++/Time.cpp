#include "Time.h"

Time::Time(const SYSTEMTIME & _val, bool local)
{
	SYSTEMTIME st = _val;
	if (local) {
		TIME_ZONE_INFORMATION info;
		GetTimeZoneInformation(&info);
		TzSpecificLocalTimeToSystemTime(&info, &_val, &st);
	}
	year = st.wYear; month = (uint8_t)st.wMonth; day = (uint8_t)st.wDay;
	dayOfWeek = (uint8_t)st.wDayOfWeek; hour = (uint8_t)st.wHour; minute = (uint8_t)st.wMinute;
	second = (uint8_t)st.wSecond; milliseconds = st.wMilliseconds;
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
	year = st.wYear; month = (uint8_t)st.wMonth; day = (uint8_t)st.wDay;
	dayOfWeek = (uint8_t)st.wDayOfWeek; hour = (uint8_t)st.wHour; minute = (uint8_t)st.wMinute;
	second = (uint8_t)st.wSecond; milliseconds = st.wMilliseconds;
}

Time::Time(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second, uint16_t milliseconds)
{
	uint16_t dayOfMonth[12] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
	if (isLeapYear(year)) {
		dayOfMonth[1] = 29;
	}

	this->year = year; this->month = month; this->day = day;
	this->hour = hour; this->minute = minute; this->second = second;
	this->milliseconds = milliseconds;

	int y = year, m = month, d = day, c = year / 100;

	dayOfWeek = (c / 4 - 2 * c + y + y / 4 + (m + 1) * 13 / 5 + d - 1) % 7;
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

std::wstring Time::toString(bool local) const
{
	wchar_t buffer[64];
	if (local) {
		SYSTEMTIME time = toLocalSystemTime();
		_snwprintf_s(buffer, 64, 63, L"%02hu-%02hu-%02hu %02hu:%02hu:%02hu.%03u", time.wYear, time.wMonth, time.wDay, time.wHour, 
			time.wMinute, time.wSecond, time.wMilliseconds);
	}
	else {
		_snwprintf_s(buffer, 64, 63, L"%02hu-%02hu-%02hu %02hu:%02hu:%02hu.%03u", year, month, day, hour, minute, second, milliseconds);
	}
	return buffer;
}

std::string Time::toANSIString(bool local) const
{
	char buffer[64];
	if (local) {
		SYSTEMTIME time = toLocalSystemTime();
		_snprintf_s(buffer, 64, 63, "%u/%u/%u %02u:%02u:%02u.%03u", time.wYear, time.wMonth, time.wDay, time.wHour,
			time.wMinute, time.wSecond, time.wMilliseconds);
	}
	else {
		_snprintf_s(buffer, 64, 63, "%u/%u/%u %02u:%02u:%02u.%03u", year, month, day, hour, minute, second, milliseconds);
	}
	return buffer;
}

std::wstring Time::format(const std::wstring& _format) const
{
	constexpr const wchar_t* const week[7] = { L"Sunday",L"Monday",L"Tuesday",L"Wednesday",L"Thursday",L"Friday",L"Saturday" };
	constexpr const size_t weekLength[7] = { 6,6,7,9,8,6,7 };
	bool escape = false, escape2 = false;
	size_t length = 0;
	for (std::wstring::const_iterator i = _format.cbegin(); i != _format.cend(); ++i) {
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

		wchar_t* buffer = new wchar_t[length + 1];
	int p = 0;
	escape = false; escape2 = false;
	for (std::wstring::const_iterator i = _format.cbegin(); i != _format.cend(); ++i) {
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
				bool _8bits = true;
				const uint8_t* ptr = nullptr;
				const uint16_t* ptr2 = nullptr;
				switch (*i) {
				case L'Y':
					ptr2 = &year;
					_8bits = false;
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
					ptr2 = &milliseconds;
					_8bits = false;
					break;
				}
				if (_8bits) {
					int length = static_cast<int>(log10(*ptr)) + 1;
					uint8_t pn = *ptr;
					for (int i = length; i > 0; --i) {
						uint8_t m = static_cast<uint8_t>(pow(10, i - 1));
						buffer[p++] = L'0' + pn / m;
						pn %= m;
					}
				}
				else {
					int length = static_cast<int>(log10(*ptr2)) + 1;
					uint16_t pn = *ptr;
					for (int i = length; i > 0; --i) {
						uint16_t m = static_cast<uint16_t>(pow(10, i - 1));
						buffer[p++] = L'0' + pn / m;
						pn %= m;
					}
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
	std::wstring res = buffer;
	delete[] buffer;
	return res;
}

std::string Time::format(const std::string & _format) const
{
		constexpr const char* const week[7] = { "Sunday","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday" };
	constexpr const size_t weekLength[7] = { 6,6,7,9,8,6,7 };
	bool escape = false, escape2 = false;
	size_t length = 0;
	for (std::string::const_iterator i = _format.cbegin(); i != _format.cend(); ++i) {
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

	char* buffer = new char[length + 1];
	int p = 0;
	escape = false; escape2 = false;
	for (std::string::const_iterator i = _format.cbegin(); i != _format.cend(); ++i) {
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
				bool _8bits = true;
				const uint8_t* ptr = nullptr;
				const uint16_t* ptr2 = nullptr;
				switch (*i) {
				case 'Y':
					ptr2 = &year;
					_8bits = false;
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
					ptr2 = &milliseconds;
					_8bits = false;
					break;
				}
				if (_8bits) {
					int length = static_cast<int>(log10(*ptr)) + 1;
					uint8_t pn = *ptr;
					for (int i = length; i > 0; --i) {
						uint8_t m = static_cast<uint8_t>(pow(10, i - 1));
						buffer[p++] = L'0' + pn / m;
						pn %= m;
					}
				}
				else {
					int length = static_cast<int>(log10(*ptr2)) + 1;
					uint16_t pn = *ptr;
					for (int i = length; i > 0; --i) {
						uint16_t m = static_cast<uint16_t>(pow(10, i - 1));
						buffer[p++] = L'0' + pn / m;
						pn %= m;
					}
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
	std::string res = buffer;
	delete[] buffer;
	return res;
}

Time& Time::parse(const std::wstring & str)
{
	uint16_t y, ms;
	uint8_t mo, d, h, min, s;
	int res = _snwscanf_s(str.c_str(), str.length(), L"%hu-%02hhu-%02hhu %02hhu:%02hhu:%02hhu.%03hu", &y, &mo, &d, &h, &min, &s, &ms);
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

Time& Time::parse(const std::string & str)
{
	uint16_t y, ms;
	uint8_t mo, d, h, min, s;
	int res = _snscanf_s(str.c_str(), str.length(), "%hu-%02hhu-%02hhu %02hhu:%02hhu:%02hhu.%03hu", &y, &mo, &d, &h, &min, &s, &ms);
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
	return Time(st, true);
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
		return true;
	}
}

uint16_t Time::getYear() const noexcept
{
	return year;
}

uint16_t Time::getMonth() const noexcept
{
	return month;
}

uint16_t Time::getDay() const noexcept
{
	return day;
}

uint16_t Time::getHour() const noexcept
{
	return year;
}

uint16_t Time::getMinute() const noexcept
{
	return minute;
}

uint16_t Time::getSecond() const noexcept
{
	return second;
}

uint16_t Time::getMilliseconds() const noexcept
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

void Time::setMonth(uint8_t _val)
{
	if (_val == 0 || _val > 12) {
		throw TimeError("Invalid month. (month >= 12)");
	}
	else {
		month = _val;
	}
}

void Time::setDay(uint8_t _val)
{
	uint8_t dayOfMonth[12] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
	if (isLeapYear(year))
		dayOfMonth[1] = 29;

	if (_val > dayOfMonth[month - 1]) {
		throw TimeError("Invalid date. (Day does not exist)");
	}
	else {
		day = _val;
	}
}

void Time::setHour(uint8_t _val)
{
	if (_val >= 24) {
		throw TimeError("Invalid time. (hour >= 24)");
	}
	else {
		hour = _val;
	}
}

void Time::setMinute(uint8_t _val)
{
	if (_val >= 60) {
		throw TimeError("Invalid time. (minute >= 60)");
	}
	else {
		minute = _val;
	}
}

void Time::setSecond(uint8_t _val)
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

void Time::setTime(FILETIME _val, bool local)
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
	year = st.wYear; month = (uint8_t)st.wMonth; day = (uint8_t)st.wDay;
	dayOfWeek = (uint8_t)st.wDayOfWeek; hour = (uint8_t)st.wHour; minute = (uint8_t)st.wMinute;
	second = (uint8_t)st.wSecond; milliseconds = st.wMilliseconds;
}

void Time::setTime(SYSTEMTIME _val, bool local)
{
	SYSTEMTIME st = _val;
	if (local) {
		TIME_ZONE_INFORMATION info;
		GetTimeZoneInformation(&info);
		TzSpecificLocalTimeToSystemTime(&info, &_val, &st);
	}
	year = st.wYear; month = (uint8_t)st.wMonth; day = (uint8_t)st.wDay;
	dayOfWeek = (uint8_t)st.wDayOfWeek; hour = (uint8_t)st.wHour; minute = (uint8_t)st.wMinute;
	second = (uint8_t)st.wSecond; milliseconds = st.wMilliseconds;
}

Time::operator FILETIME() const
{
	return this->toFileTime();
}

Time::operator SYSTEMTIME() const
{
	return this->toSystemTime();
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

inline std::ostream& operator<<(std::ostream& out, const Time& val)
{
	out << val.toString().c_str();
	return out;
}
