#include "framework.h"
#include "general.h"
#include "CSpy++.h"
#include <Windows.h>
#include <shellapi.h>
#include "Directory.h"

HINSTANCE hInst;

int __cdecl CommandLineMain();

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	//MainThread thread;
	//thread.RunOnCurrentThreadWithLoop(nbase::MessageLoop::kUIMessageLoop);

	int res;
	if (res = CommandLineMain()) {
		TerminateProcess(GetCurrentProcess(), res);
		return res;
	}

	TerminateProcess(GetCurrentProcess(), 0);
	return 0;
}

HINSTANCE GetCurrentInstance()
{
	return hInst;
}


void dfs(Directory* dir, FILE* file, std::function<size_t(const void*, size_t, size_t)> fwrite2) {
	for (File* fl : dir->getFileList()) {
		fl->writeBinaryData(fwrite2);
	}
	for (Directory* d : dir->getDirectoryList()) {
		d->writeBinaryData(fwrite2);
		dfs(d, file, fwrite2);
	}
}
/*
void MainThread::Init()
{
	nbase::ThreadManager::RegisterThread(0);
	std::wstring theme_dir = nbase::win32::GetCurrentModuleDirectory();
	ui::GlobalManager::Startup(theme_dir + L"resources\\", ui::CreateControlCallback(), false);

	VolumeDirectoryPtr dptr = FileLister::list_volume(L'R');
	FileList list = FileList(L"F:/R.csf", dptr.get());
	list.writeFile();
	FileList list2(L"F:/R.csf");
	list2.readFileList();
	Directory* dir = list2.getDirectory();
	Volume* vol = (Volume*)list2.getAdditionalHeader();
}

void MainThread::Cleanup()
{
	ui::GlobalManager::Shutdown();
	SetThreadWasQuitProperly(true);
	nbase::ThreadManager::UnregisterThread();
}
*/