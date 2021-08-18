#include "Control.h"
#include <iostream>
#include <memory>
#include <map>
#include <list>

unsigned Control::id_counter = 0;

std::map<unsigned, Control*> all_controls;
std::list<Control*> tmp_controls;

Control::Control() : 
	id(++id_counter), parent(nullptr), hwnd(nullptr), window_style(0),
	last_known_size({0,0}),
	anchor(static_cast<Anchor>(Anchor::Anchor_Top | Anchor::Anchor_Left)){
	x = y = width = height = CW_USEDEFAULT;
}

Control::Control(unsigned id) : 
	id(id), parent(nullptr), hwnd(nullptr), window_style(0),
	last_known_size({ 0,0 }),
	anchor(static_cast<Anchor>(Anchor::Anchor_Top | Anchor::Anchor_Left)) {
	x = y = width = height = CW_USEDEFAULT;
}

Control::Control(Control&& control) noexcept :
	id(control.id), 
	parent(control.parent),
	class_name(control.class_name),
	window_style(control.window_style),
	controls(std::move(control.controls)),
	paint(std::move(control.paint)),
	hwnd(control.hwnd),
	title(control.title),
	x(control.x),
	y(control.y),
	width(control.width),
	height(control.height),
	last_known_size(control.last_known_size),
	anchor(control.anchor)
{
	if (parent) parent->add_control(*this);
	if (std::find(tmp_controls.begin(), tmp_controls.end(), &control) != tmp_controls.end()) {
		tmp_controls.push_back(this);
	}
	for (Control* c : controls) {
		c->parent = this;
	}
	control.hwnd = nullptr;
	control.id = -1;
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
	last_known_size = get_size();
	for (auto* c : controls) {
		c->create();
	}
}

Control &Control::add_control(Control& control) {
	controls.push_back(&control);
	control.parent = this;
	if (hwnd) {
		control.create();
	}
	return *this;
}

Control &Control::add_control(Control&& control) {
	Control* tmp = control.deep_move(std::move(control));
	tmp_controls.push_back(tmp);
	return add_control(*tmp);
}

void Control::remove_control(Control& control) {
	controls.remove(&control);
	control.parent = nullptr;
}

Control::~Control() {
	if (parent) parent->remove_control(*this);
	all_controls.erase(id);
	tmp_controls.remove(this);
	if (hwnd) DestroyWindow(hwnd);
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
		handle_size_changed(size);
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

Control* Control::deep_move(Control&& control) noexcept {
	return new Control(std::move(control));
}

void Control::handle_size_changed(SIZE new_size) {
	if (!hwnd) return;
	const bool width_changed = new_size.cx != last_known_size.cx;
	const bool height_changed = new_size.cy |= last_known_size.cy;
	if (!width_changed && !height_changed) return;
	int dx = new_size.cx - last_known_size.cx;
	int dy = new_size.cy - last_known_size.cy;
	Anchor size_changer = static_cast<Anchor>(Anchor_Right | Anchor_Bottom);
	for (Control* c : controls) {
		if (c->has_anchor(size_changer)) {
			POINT new_location = c->get_location();
			SIZE new_size = c->get_size();
			bool location_changed = false, size_changed = false;
			if (c->has_anchor(Anchor_Right) && width_changed) {
				if (c->has_anchor(Anchor_Left)) {
					size_changed = true;
					new_size.cx += dx;
				}
				else {
					location_changed = true;
					new_location.x += dx;
				}
			}
			if (c->has_anchor(Anchor_Bottom) && height_changed) {
				if (c->has_anchor(Anchor_Top)) {
					size_changed = true;
					new_size.cy += dy;
				}
				else {
					location_changed = true;
					new_location.y += dy;
				}
			}
			if (location_changed) {
				c->set_location(new_location);
			}
			if (size_changed) {
				c->set_size(new_size);
			}
		}
	}
	last_known_size = new_size;
	size_changed.Invoke(this, last_known_size);
}

Control& Control::set_anchor(Anchor anchor) {
	this->anchor = anchor;
	return *this;
}
Control::Anchor Control::get_anchor() const {
	return anchor;
}

bool Control::has_anchor(Anchor anchor) const {
	return (this->anchor & anchor) > 0;
}

Control& Control::add_anchor(Anchor anchor) {
	this->anchor = static_cast<Anchor>(this->anchor | anchor);
	return *this;
}

Control& Control::remove_anchor(Anchor) {
	this->anchor = static_cast<Anchor>(this->anchor & (anchor ^ Anchor_All));
	return *this;
}
