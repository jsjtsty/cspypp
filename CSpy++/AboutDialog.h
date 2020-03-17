#pragma once
#include <duilib/UIlib.h>

class AboutDialog : public ui::WindowImplBase
{
public:
	AboutDialog() = default;
	~AboutDialog() = default;

	virtual std::wstring GetSkinFolder() override;
	virtual std::wstring GetSkinFile() override;
	virtual std::wstring GetWindowClassName() const override;
	virtual void InitWindow() override;
	
};

