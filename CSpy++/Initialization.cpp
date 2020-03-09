#include "framework.h"
#include "general.h"
#include <string>
#include <locale>
#include "LicenseReader.h"
#include "RSAKey.h"

void InitLogSystem();
void InitConstants();

int InitProgram()
{
	std::locale::global(std::locale("chs"));
	InitConstants();
	InitLogSystem();
	return 0;
}

int HandleCreateMessage(WPARAM wParam, LPARAM lParam)
{
	
	return 0;
}

void InitLogSystem()
{

}