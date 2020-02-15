#include "framework.h"
#include "general.h"
#include "CSpy++.h"
#include "MainThread.h"
#include <duilib/UIlib.h>
#include <base/win32/path_util.h>
#include "basic_form.h"
#include <ui_components/ui_components.h>
#include <ui_components/toast/toast.h>
#include "FileLister.h"
#include "FileList.h"
using namespace nim_comp;

HINSTANCE hInst;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	MainThread thread;
	thread.RunOnCurrentThreadWithLoop(nbase::MessageLoop::kUIMessageLoop);

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

void MainThread::Init()
{
	nbase::ThreadManager::RegisterThread(0);
	std::wstring theme_dir = nbase::win32::GetCurrentModuleDirectory();
	ui::GlobalManager::Startup(theme_dir + L"resources\\", ui::CreateControlCallback(), false);

	Time time1 = Time::getLocalTime();
	VolumeDirectoryPtr dptr = FileLister::list_volume(L'C');
	Time time2 = Time::getLocalTime();
	FileList list = FileList(L"c.csf", dptr.get());
	list.writeFile();
	dptr.reset();

	MessageBox(NULL, (time1.toString() + L"\n" + time2.toString()).c_str(), L"Info", MB_ICONINFORMATION);

	/*
	BasicForm* form = new BasicForm();
	form->Create(NULL, L"MainFrame", 0, 0);
	form->ShowWindow();
	*/
}

void MainThread::Cleanup()
{
	ui::GlobalManager::Shutdown();
	SetThreadWasQuitProperly(true);
	nbase::ThreadManager::UnregisterThread();
}