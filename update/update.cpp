#include "framework.h"
#include "update.h"
#include <unzipper.h>

HINSTANCE hInstance;
HINSTANCE GetCurrentInstance();

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE, LPWSTR lpCmdLine, int nCmdShow)
{
	::hInstance = hInstance;

	HANDLE hMutex = CreateMutexW(NULL, false, L"cspypp-update");
	DWORD lastError = GetLastError();
	if (lastError == ERROR_ALREADY_EXISTS || lastError == ERROR_ACCESS_DENIED) {
		return 0;
	}

	zipper::Unzipper unz("update.zip");
	unz.extract();
	unz.close();

	return 0;
}

HINSTANCE GetCurrentInstance()
{
	return hInstance;
}