#pragma once
#include <Windows.h>
#include <string>

class Notify
{
public:
	static Notify* initInstance(HWND hWnd);
	static Notify* getInstance();
	void notify();
	void update();
	void remove();
protected:
	Notify();
	NOTIFYICONDATA data;
	bool isShowing;
};