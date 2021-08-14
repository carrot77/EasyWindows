#pragma once

#include <Windows.h>
#include <string>
#include <functional>
#include <list>

#define DEEP_COPY_DECL(T) \
	T(T&&) noexcept = default; \
	inline Control *deep_move(Control&& c) noexcept override { \
		return new T(std::move(dynamic_cast<T&>(c)));\
	}

template <class ...Params>
class EventHandler {
	std::list<std::function<void(Params...)>> registrations;
public:
	EventHandler() = default;
	EventHandler(const EventHandler&) = delete;
	EventHandler(EventHandler&&) noexcept = default;
	EventHandler& operator =(const EventHandler&) = delete;
	EventHandler& operator =(EventHandler&&) = default;
	EventHandler& operator +=(std::function<void(Params...)> func) {
		registrations.push_back(func);
		return *this;
	}
	void clear() { registrations.clear(); }
	EventHandler& operator -=(std::function<void(Params...)> func) {
		registrations.remove(func);
		return *this;
	}
	void Invoke(Params... args) {
		for (const auto& func : registrations) {
			func(args...);
		}
	}
};

class Control
{
	static unsigned id_counter;
	unsigned id;
protected:
	Control* parent;
	Control();
	Control(Control&&) noexcept;
	Control(unsigned id);
	virtual ~Control();
	std::wstring class_name;
	uint32_t window_style;
	EventHandler<Control*, HDC, PAINTSTRUCT&> paint;
	virtual void create();
	std::list<Control*> controls;
	HWND hwnd;
	std::wstring title;
	int32_t x, y, width, height;
	static Control* get_control_by_id(unsigned id);
	virtual Control* deep_move(Control &&control) noexcept;
public:
	const unsigned& get_id() const;
	const std::list<Control*> &get_controls() const;
	const std::wstring &get_title() const;
	Control &set_title(const std::wstring&);
	RECT get_rectangle() const;
	Control &set_rectangle(const RECT&);
	SIZE get_size() const;
	Control& set_size(const SIZE&);
	POINT get_location() const;
	Control& set_location(const POINT&);

	const HWND& get_hwnd() const;


	Control& add_control(Control& control);
	Control& add_control(Control&& control);
	void remove_control(Control& control);
};

