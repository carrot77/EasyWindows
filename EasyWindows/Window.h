#pragma once

#include <Windows.h>
#include <string>
#include "Control.h"
#include "Panel.h"

class Window : public Control
{
	friend LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	Panel dummy;
	Window(const Window&) = delete;
protected:
	void handle_size_changed(SIZE new_size) override;
	Window(Window&&) noexcept;
	inline Control* deep_move(Control && c) noexcept override {
		return new Window(std::move(dynamic_cast<Window&>(c)));
	}
public:
	Window(const std::wstring &class_name = L"MyGuiApp");
	using Control::paint;
	void show();
	void run();
	using Control::set_title;
	using Control::get_title;
};

