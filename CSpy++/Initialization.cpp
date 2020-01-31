#include "framework.h"
#include "general.h"
#include <string>
#include "LogSupport.h"
#include "FileLister.h"
using namespace std;
using namespace log4cplus;

void InitLogSystem();
void InitConstants();

int InitProgram()
{
    locale::global(locale("chs"));
    InitConstants();
    InitLogSystem();
    return 0;
}

void dfs(Directory* dir, FILE* file) {
	const auto& dirSet = dir->getDirectoryList();
	const auto& fileSet = dir->getFileList();
	for (Directory* idir : dirSet) {
		dfs(idir, file);
	}
	for (File* ifile : fileSet) {
		fwprintf_s(file, L"%s\n", ifile->getPath().c_str());
	}
}

int HandleCreateMessage(WPARAM wParam, LPARAM lParam)
{
	VolumeDirectoryPtr ptr = FileLister::list_volume(L'E');
	FILE* file;
	_wfopen_s(&file, L"flist.csf", L"wt");
	fwprintf_s(file, L"Start Time: %s\n\n", Time::getLocalTime().toString().c_str());
	dfs(ptr.get(), file);
	fwprintf_s(file, L"\nEnd Time: %s", Time::getLocalTime().toString().c_str());
	fclose(file);
	ptr.reset();
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