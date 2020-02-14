#include "framework.h"
#include "general.h"
#include "CSpy++.h"
#include "MainThread.h"
#include <duilib/UIlib.h>
#include <base/win32/path_util.h>
#include "basic_form.h"
#include <ui_components/ui_components.h>
#include <ui_components/toast/toast.h>
#include "SystemVersion.h"
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

void MainThread::Init()
{
	nbase::ThreadManager::RegisterThread(0);
	std::wstring theme_dir = nbase::win32::GetCurrentModuleDirectory();
	ui::GlobalManager::Startup(theme_dir + L"resources\\", ui::CreateControlCallback(), false);
	
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