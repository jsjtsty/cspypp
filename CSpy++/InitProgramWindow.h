#pragma once
#include <duilib/UIlib.h>

class InitProgramWindow : public ui::WindowImplBase
{
public:
	InitProgramWindow() = default;
	~InitProgramWindow() = default;

	virtual std::wstring GetSkinFolder() override;
	virtual std::wstring GetSkinFile() override;
	virtual std::wstring GetWindowClassName() const override;
	virtual void InitWindow() override;
};

