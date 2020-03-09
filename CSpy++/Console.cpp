#include "general.h"
#include "ArgParser.h"
#include <cstdio>
#include <iostream>
#include "StringExt.h"
#include "JsonExt.h"
#include <openssl/crypto.h>
#include <curl/curl.h>
#include <zlib.h>
using std::wcout;
using std::endl;

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4996)
#endif

int __cdecl CommandLineMain()
{
	if (__argc <= 1) {
		return 0;
	}

	//freopen("conout$", "wt+", stdout);
	//freopen("conerr$", "wt+", stderr);
	//freopen("conin$", "rt+", stdin);

	ArgParser parser;
	if (!parser.parse(__argc, __wargv)) {
		wcout << L"Invalid command." << endl;
		return -1;
	}

	Argument arg{ L"v",L"-version" };
	if (parser.hasArg(arg)) {
		std::wstringstream ss;
		printf("Fuck!\n");
		wcout << L"CSpy++ " << CS_VERSION << L" jsoncpp/1.9.2 " << std::to_wstring(curl_version()) << endl;
		wcout << L"Release Date: " << CS_VERSION_RELEASEDATE_YEAR << L"-" << CS_VERSION_RELEASEDATE_MONTH << L"-"
			<< CS_VERSION_RELEASEDATE_DAY << endl;
		MessageBox(NULL, ss.str().c_str(), L"Info", MB_ICONINFORMATION);
		//WriteConsole(GetStdHandle(STD_OUTPUT_HANDLE), ss.str().c_str(), ss.str().size(), NULL, NULL);
		return 1;
	}

	return 0;
}

#ifdef _MSC_VER
#pragma warning(pop)
#endif
