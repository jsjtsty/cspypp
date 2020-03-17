#include "InitProgramWindow.h"

std::wstring InitProgramWindow::GetSkinFolder()
{
	return L"initprogram";
}

std::wstring InitProgramWindow::GetSkinFile()
{
	return L"initprogram.xml";
}

std::wstring InitProgramWindow::GetWindowClassName() const
{
	return L"cspyplusplus_initprogram";
}

void InitProgramWindow::InitWindow()
{
}
