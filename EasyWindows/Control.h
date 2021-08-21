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

template <class T>
class Observable {
	T value;
public:
	EventHandler<const T& oldValue, const T& newValue> changed;
	const T& operator T() const {
		return value;
	}
	Observable<T>& operator =(const T& newValue) {
		if (value != newValue) {
			T tmp = value;
			value = newValue;
			changed.Invoke(tmp, value);
		}
		return *this;
	}
};

template <class T, size_t K>
struct ObservableArray : std::array<T,K> {
	EventHandler<const ObservableArray<T, K>&> changed;
	const T& operator [](size_t ind) const {
		return std::array<T, K>::operator [](ind);
	}
	ObservableArray& set(size_t ind, const T& val) {
		if (val != std::array<T, K>::operator [](ind)) {
			std::array<T, K>::operator [](ind) = val;
			changed.Invoke(*this);
		}
	}
};

class Control
{
	static unsigned id_counter;
	unsigned id;
	SIZE last_known_size;
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
	virtual void handle_size_changed(SIZE new_size);
	const std::wstring& get_title() const;
	virtual Control& set_title(const std::wstring&);
public:
	enum Anchor {
		Anchor_Top = 1,
		Anchor_Left = Anchor_Top << 1,
		Anchor_Bottom = Anchor_Left << 1,
		Anchor_Right = Anchor_Bottom << 1,
		Anchor_All = Anchor_Top | Anchor_Left | Anchor_Bottom | Anchor_Right
	};
	const unsigned& get_id() const;
	const std::list<Control*> &get_controls() const;
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

	Control& set_anchor(Anchor);
	Anchor get_anchor() const;

	bool has_anchor(Anchor) const;
	Control& add_anchor(Anchor);
	Control& remove_anchor(Anchor);

	EventHandler<Control*, SIZE &> size_changed;
private:
	Anchor anchor;
};

