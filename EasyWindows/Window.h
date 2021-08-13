#pragma once

#include <Windows.h>
#include <string>
#include "Control.h"

class Window : public Control
{
	friend LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
protected:
	DEEP_COPY_DECL(Window)
public:
	Window(const std::wstring &class_name = L"MyGuiApp");
	using Control::paint;
	void show();
	void run();
};

