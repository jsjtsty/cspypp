#pragma once
#include <base/base.h>

class MainThread : public nbase::FrameworkThread
{
public:
	MainThread() : nbase::FrameworkThread("MainThread") {}
	virtual ~MainThread() {}

private:
	virtual void Init() override;
	virtual void Cleanup() override;
};