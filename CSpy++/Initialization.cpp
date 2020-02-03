#include "framework.h"
#include "general.h"
#include <string>
#include "LogSupport.h"
#include "LicenseReader.h"
using namespace std;
using namespace log4cplus;

void InitLogSystem();
void InitConstants();

int InitProgram()
{
	locale::global(locale("chs"));
	InitConstants();
	InitLogSystem();
	_CrtSetDebugFillThreshold(0);

	LicenseReader reader(LicenseReader::LicenseType::LOG4CPLUS);
	char* data = (char*)reader.lockResource();
	return 0;
}

int HandleCreateMessage(WPARAM wParam, LPARAM lParam)
{
	
	return 0;
}

void InitLogSystem()
{
	SharedAppenderPtr append(new RollingFileAppender(L"cspypp.log"));
	append->setName(L"defaultAppender");
	append->setLayout(make_unique<PatternLayout>(L"[%p](%D{%Y-%m-%d %H:%M:%S.%q})[pid=%t, %l]: %m%n"));
	logger = Logger::getInstance(L"defaultLogger");
	logger.addAppender(append);
}