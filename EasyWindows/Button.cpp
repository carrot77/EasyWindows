#include "Button.h"
#include <windowsx.h>

Button::Button() : enabled(true) {
	class_name = L"button";
	window_style = WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON;
}

Button &Button::set_enabled(bool enabled) {
	if (hwnd) {
		Button_Enable(get_hwnd(), enabled);
	}
	else {
		this->enabled = enabled;
	}
	return *this;
}

void Button::create() {
	Control::create();
	if (!enabled) {
		set_enabled(false);
	}
}