#include "Control.h"
#include <iostream>
#include <memory>
#include <map>
#include <list>

unsigned Control::id_counter = 0;

std::map<unsigned, Control*> all_controls;
std::list<Control*> tmp_controls;

Control::Control() : id(++id_counter), parent(nullptr), hwnd(nullptr), window_style(0){
	x = y = width = height = CW_USEDEFAULT;
}

Control::Control(unsigned id) : id(id), parent(nullptr), hwnd(nullptr), window_style(0) {
	x = y = width = height = CW_USEDEFAULT;
}

void Control::create() {
	if (hwnd) return;
	if (id) {
		if (all_controls.find(id) != all_controls.end()) return;
		all_controls[id] = this;
	}
	hwnd = CreateWindow(
		class_name.c_str(),
		title.c_str(),
		window_style,
		x, y, width, height, //Position, Size
		parent ? parent->hwnd : nullptr,
		(HMENU)id, // Menu
		GetModuleHandle(nullptr),
		nullptr  // Additional data
	);
	for (auto* c : controls) {
		c->create();
	}
}

void Control::add_control(Control& control) {
	controls.push_back(&control);
	control.parent = this;
	if (hwnd) {
		control.create();
	}
}

void Control::add_control(Control&& control) {
	Control* tmp = control.deep_move(std::move(control));
	tmp_controls.push_back(tmp);
	add_control(*tmp);
}

void Control::remove_control(Control& control) {
	controls.remove(&control);
	control.parent = nullptr;
}

Control::~Control() {
	if (parent) parent->remove_control(*this);
	all_controls.erase(id);
	tmp_controls.remove(this);
}

const std::list<Control*>& Control::get_controls() const {
	return controls;
}

const std::wstring& Control::get_title() const {
	return title;
}

const unsigned& Control::get_id() const { 
	return id;
}

Control &Control::set_title(const std::wstring& text) {
	if (hwnd) SetWindowText(hwnd, text.c_str());
	title = text;
	return *this;
}

RECT Control::get_rectangle() const {
	if (hwnd) {
		RECT retval;
		GetWindowRect(hwnd, &retval);
		if (parent) {
			POINT topLeft{ retval.left, retval.top };
			POINT bottomRight{ retval.right, retval.bottom };
			ScreenToClient(parent->hwnd, &topLeft);
			ScreenToClient(parent->hwnd, &bottomRight);
			retval.left = topLeft.x; retval.top = topLeft.y;
			retval.right = bottomRight.x; retval.bottom = bottomRight.y;
		}
		return retval;
	}
	else {
		RECT retval{x,y,x+width, y+height};
		return retval;
	}
}

Control& Control::set_rectangle(const RECT& rect) {
	x = rect.left; y = rect.top;
	width = rect.right - rect.left;
	height = rect.bottom - rect.top;
	if (hwnd) {
		SetWindowPos(hwnd, HWND_TOP, x, y, width, height, SWP_NOZORDER | SWP_SHOWWINDOW);
	}
	return *this;
}

SIZE Control::get_size() const {
	if (hwnd) {
		RECT rect;
		GetWindowRect(hwnd, &rect);
		SIZE retval{ rect.right - rect.left, rect.bottom - rect.top };
		return retval;
	}
	else {
		SIZE retval{width, height};
		return retval;
	}
}

Control& Control::set_size(const SIZE& size) {
	width = size.cx;
	height = size.cy;
	if (hwnd) {
		SetWindowPos(hwnd, HWND_TOP, x, y, width, height, SWP_NOMOVE| SWP_NOZORDER | SWP_SHOWWINDOW);
	}
	return *this;
}

POINT Control::get_location() const {
	if (hwnd) {
		RECT rect;
		GetWindowRect(hwnd, &rect);
		POINT topLeft{ rect.left, rect.top };
		if (parent) {
			ScreenToClient(parent->hwnd, &topLeft);
		}
		return topLeft;

	}
	else {
		POINT retval{x,y};
		return retval;
	}
}

Control& Control::set_location(const POINT& location) {
	if (hwnd) {
		SetWindowPos(hwnd, HWND_TOP, location.x, location.y, width, height, SWP_NOSIZE | SWP_NOZORDER | SWP_SHOWWINDOW);
	}
	else {
		x = location.x;
		y = location.y;
	}
	return *this;
}

const HWND& Control::get_hwnd() const {
	return hwnd;
}

Control* Control::get_control_by_id(unsigned id) {
	if (all_controls.find(id) == all_controls.end()) return nullptr;
	return all_controls[id];
}
