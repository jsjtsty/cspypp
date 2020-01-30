#include "Notify.h"
using namespace std;

namespace {
	Notify* instance = nullptr;
}

void Notify::notify()
{
	Shell_NotifyIcon(NIM_ADD, &data);
}

void Notify::update()
{
	Shell_NotifyIcon(NIM_MODIFY, &data);
}

void Notify::remove()
{
	Shell_NotifyIcon(NIM_DELETE, &data);
}

Notify::Notify()
{
	data.cbSize = sizeof(NOTIFYICONDATA);
	data.uID = 1;
	isShowing = false;
}
