#include "general.h"
#include <Windows.h>
#include <string>
using namespace std;

unsigned long CS_MAIN_THREADID = 0;

void InitConstants()
{
	CS_MAIN_THREADID = GetCurrentThreadId();
}