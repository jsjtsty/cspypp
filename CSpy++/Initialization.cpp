#include "framework.h"
#include "general.h"
#include <string>
#include "LogSupport.h"
#include "FileLister.h"
#include "FileList.h"
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

	//auto usb = FileLister::list_usb(L'H');
	FileList list(L"flist.csf");
	//list.writeFile();
	list.readFileList();
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