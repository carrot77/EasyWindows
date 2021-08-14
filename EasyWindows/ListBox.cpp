#include "ListBox.h"
#include <windowsx.h>
#include <CommCtrl.h>
#include <vector>

ListBox::ListBox(SelectionMode mode, bool sort) : max_item_length(10) {
	class_name = WC_LISTBOX;
	window_style = WS_VISIBLE | WS_CHILD | WS_TABSTOP | LBS_NOTIFY | LBS_STANDARD;
	switch (mode)
	{
	case ListBox::SelectionMode::NoSelect:
		window_style |= LBS_NOSEL;
		break;
	case ListBox::SelectionMode::MultipleSelect:
		window_style |= LBS_MULTIPLESEL;
		break;
	case ListBox::SelectionMode::SingleSelect:
	default:
		break;
	}
	if (!sort) {
		window_style &= (~LBS_SORT);
	}
}

void ListBox::create() {
	Control::create();
	for (const auto& str : init_strings) {
		add_string(str);
	}
	init_strings.clear();
	for (const auto& str : selected_strings) {
		set_selected(str, true);
	}
	selected_strings.clear();
}

ListBox& ListBox::add_string(const std::wstring &str) {
	if (str.size() > max_item_length) max_item_length = str.size();
	if (hwnd) {
		ListBox_AddString(hwnd, str.c_str());
	}
	else {
		init_strings.push_back(str);
	}
	return *this;
}

ListBox& ListBox::remove_string(const std::wstring& str) {
	if (hwnd) {
		auto ind = ListBox_FindStringExact(hwnd, -1, str.c_str());
		if (ind >= 0) {
			ListBox_DeleteString(hwnd, ind);
		}
	}
	else {
		init_strings.remove(str);
	}
	return *this;
}

ListBox& ListBox::clear() {
	if (hwnd) {
		ListBox_ResetContent(hwnd);
	}
	else {
		init_strings.clear();
	}
	return *this;
}

ListBox& ListBox::set_selected(const std::wstring& str, bool selected) {
	if (hwnd) {
		auto ind = ListBox_FindStringExact(hwnd, -1, str.c_str());
		if (ind >= 0) {
			ListBox_SetSel(hwnd, selected, ind);
		}
	}
	else {
		selected_strings.push_back(str);
	}
	return *this;
}

std::list<std::wstring> ListBox::get_selection() const {
	if (hwnd) {
		std::list<std::wstring> retval;
		int count = ListBox_GetSelCount(hwnd);
		if (count > 0) {
			std::vector<int> ibuffer(count);
			ListBox_GetSelItems(hwnd, count, ibuffer.data());
			std::vector<wchar_t> wbuffer(max_item_length + 2);
			std::list<std::wstring> retval;
			for (int i = 0; i < count; ++i) {
				ListBox_GetText(hwnd, ibuffer[i], wbuffer.data());
				retval.push_back(wbuffer.data());
			}
		}
		return retval;
	}
	else {
		return selected_strings;
	}
}

std::list<std::wstring> ListBox::get_items() const {
	if (hwnd) {
		int count = ListBox_GetCount(hwnd);
		std::vector<wchar_t> wbuffer(max_item_length + 2);
		std::list<std::wstring> retval;
		for (int i = 0; i < count; ++i) {
			ListBox_GetText(hwnd, i, wbuffer.data());
			retval.push_back(wbuffer.data());
		}
		return retval;
	}
	else {
		return init_strings;
	}
}

std::wstring ListBox::get_caret_item() const {
	std::wstring retval(L"");
	if (hwnd) {
		int ind = ListBox_GetCaretIndex(hwnd);
		if (ind >= 0) {
			std::vector<wchar_t> wbuffer(max_item_length + 2);
			ListBox_GetText(hwnd, ind, wbuffer.data());
			retval = wbuffer.data();
		}
	}
	return retval;
}