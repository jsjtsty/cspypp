#include "framework.h"
#include "general.h"
#include "CSpy++.h"
#include "MainThread.h"
#include <Windows.h>
#include <shellapi.h>
#include "Directory.h"
#include "LogStream.h"
#include <duilib/UIlib.h>
#include <base/win32/path_util.h>
#include <ui_components/ui_components.h>
#include <ui_components/toast/toast.h>
#include "AboutDialog.h"
#include "InitProgramWindow.h"
using namespace nim_comp;

HINSTANCE hInst;

int __cdecl CommandLineMain();

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	int res;
	if (res = CommandLineMain()) {
		return res;
	}

	MainThread thread;
	thread.RunOnCurrentThreadWithLoop(nbase::MessageLoop::kUIMessageLoop);

	return 0;
}

HINSTANCE GetCurrentInstance()
{
	return hInst;
}

void MainThread::Init()
{
	nbase::ThreadManager::RegisterThread(0);
	//std::wstring theme_dir = nbase::win32::GetCurrentModuleDirectory();
	std::wstring theme_dir = L"E:\\VSWorkspace\\CSpy++\\CSpy++\\";
	ui::GlobalManager::Startup(theme_dir + L"resources\\", ui::CreateControlCallback(), false);

	AboutDialog* dl = new AboutDialog();
	dl->Create(NULL, L"Test", WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX, 0);
	dl->CenterWindow();
	dl->ShowWindow();
}

void MainThread::Cleanup()
{
	ui::GlobalManager::Shutdown();
	SetThreadWasQuitProperly(true);
	nbase::ThreadManager::UnregisterThread();
}
