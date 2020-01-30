#pragma once

constexpr const wchar_t* CS_APPNAME = L"CSpy++";

constexpr unsigned int CS_VERSION_MAJOR = 0;
constexpr unsigned int CS_VERSION_MINOR = 0;
constexpr unsigned int CS_VERSION_FIX = 1;
constexpr unsigned int CS_VERSION_BUILD = 2;
constexpr unsigned int CS_VERSION_RELEASEDATE_YEAR = 2020;
constexpr unsigned int CS_VERSION_RELEASEDATE_MONTH = 1;
constexpr unsigned int CS_VERSION_RELEASEDATE_DAY = 30;
constexpr bool CS_VERSION_ISPREVIEW = true;
constexpr const wchar_t* CS_VERSION_PREVIEW = L"Technical Preview";
constexpr const wchar_t* CS_VERSION = L"0.0.1";
constexpr const wchar_t* CS_VERSION_FULL = L"0.0.1 Technical Preview Build 2";

constexpr const wchar_t* CS_COPYRIGHT = L"Copyright (C) ISLStudio 2014-2020.";
constexpr const wchar_t* CS_COPYRIGHT_FULL = L"Copyright (C) ISLStudio 2014-2020. All rights reserved.";

#if defined(_WIN64)
constexpr bool CS_ARCH_X86 = false;
constexpr bool CS_ARCH_X64 = true;
#elif defined(_WIN32)
constexpr bool CS_ARCH_X86 = true;
constexpr bool CS_ARCH_X64 = false;
#else
constexpr bool CS_ARCH_X86 = false;
constexpr bool CS_ARCH_X64 = false;
#endif

#define WM_UPDATE_CHECK (WM_USER + 701)
#define WM_UPDATE_CHECKEND (WM_USER + 702)
#define WM_UPDATE_START (WM_USER + 710)
#define WM_UPDATE_END (WM_USER + 711)

extern unsigned long CS_MAIN_THREADID;