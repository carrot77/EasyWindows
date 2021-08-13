#include "CheckBox_3State.h"
#include "windowsx.h"

CheckBox_3State::CheckBox_3State() : checked(CheckState::Unchecked) {
	window_style = WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_AUTO3STATE;
	Button::clicked += [this](Button& btn) {
		clicked.Invoke(*this);
	};
}

CheckBox_3State::CheckState CheckBox_3State::checked_state() const {
	if (hwnd) {
		switch (Button_GetCheck(hwnd))
		{
		case BST_CHECKED:
			return CheckState::Checked;
		case BST_INDETERMINATE:
			return CheckState::Indeterminate;
		case BST_UNCHECKED:
		default:
			return CheckState::Unchecked;
		}
	}
	else {
		return checked;
	}
}

CheckBox_3State& CheckBox_3State::set_checked_state(CheckState state) {
	if (hwnd) {
		switch (state)
		{
		case CheckBox_3State::CheckState::Checked:
			Button_SetCheck(hwnd, BST_CHECKED);
			break;
		case CheckBox_3State::CheckState::Indeterminate:
			Button_SetCheck(hwnd, BST_INDETERMINATE);
			break;
		case CheckBox_3State::CheckState::Unchecked:
			Button_SetCheck(hwnd, BST_UNCHECKED);
			break;
		}
	}
	else { 
		checked = state;
	}
	return *this;
}

void CheckBox_3State::create() {
	Button::create();
	if (checked != CheckState::Unchecked) {
		set_checked_state(checked);
	}
}