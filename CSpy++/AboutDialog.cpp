#include "general.h"
#include "AboutDialog.h"

std::wstring AboutDialog::GetSkinFolder()
{
	return L"about";
}

std::wstring AboutDialog::GetSkinFile()
{
	return L"about.xml";
}

std::wstring AboutDialog::GetWindowClassName() const
{
	return L"cspypp_about";
}

void AboutDialog::InitWindow()
{
	ui::Label* version = static_cast<ui::Label*>(this->FindControl(L"version"));
	version->SetText(std::wstring(L"Version ") + CS_VERSION_FULL);
}
