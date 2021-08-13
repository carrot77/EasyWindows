#include "CheckBox.h"
#include "windowsx.h"

CheckBox::CheckBox() : checked(false) {
	window_style = WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX;
	Button::clicked += [this](Button& btn) {
		clicked.Invoke(*this);
	};
}

bool CheckBox::is_checked() const {
	if (hwnd) {
		return Button_GetCheck(hwnd) != BST_UNCHECKED;
	}
	else {
		return checked;
	}
}

CheckBox& CheckBox::set_checked(bool checked) {
	if (hwnd) {
		Button_SetCheck(hwnd, checked ? BST_CHECKED : BST_UNCHECKED);
	}else{
		this->checked = checked;
	}
	return *this;
}

void CheckBox::create() {
	Button::create();
	if (checked) {
		set_checked(checked);
	}
}