#include "ComboBox.h"
#include <CommCtrl.h>
#include <windowsx.h>
#include <vector>

ComboBox::ComboBox(ComboBox::ComboBoxStyles style, bool sort)
{
    class_name = WC_COMBOBOX;
    window_style = WS_VISIBLE | WS_CHILD | CBS_HASSTRINGS | CBS_AUTOHSCROLL | CBS_DISABLENOSCROLL | WS_VSCROLL;
    if (sort) window_style |= CBS_SORT;
    switch (style) {
    case ComboBoxStyles::Simple:
        window_style |= CBS_SIMPLE;
        break;
    case ComboBoxStyles::DropDown:
        window_style |= CBS_DROPDOWN;
        break;
    case ComboBoxStyles::DropDownList:
        window_style |= CBS_DROPDOWNLIST;
        break;
    }
}

std::list<std::wstring> ComboBox::get_list_items() const
{
    if (hwnd) {
        int item_count = ComboBox_GetCount(hwnd);
        std::vector<wchar_t> tmp(256);
        std::list<std::wstring> retval;
        for (int i = 0; i < item_count; ++i) {
            tmp.reserve(ComboBox_GetLBTextLen(hwnd, i) + 2);
            ComboBox_GetLBText(hwnd, i, tmp.data());
            retval.push_back(tmp.data());
        }
        return retval;
    }
    else {
        return items;
    }
}

ComboBox& ComboBox::add_list_item(const std::wstring& str)
{
    if (hwnd) {
        ComboBox_AddString(hwnd, str.c_str());
    }
    else {
        items.push_back(str);
    }
    return *this;
}

ComboBox& ComboBox::remove_list_item(const std::wstring& text)
{
    if (hwnd) {
        int pos = ComboBox_FindStringExact(hwnd, -1, text.c_str());
        if (pos >= 0) {
            ComboBox_DeleteString(hwnd, pos);
        }
    }
    else {
        items.remove(text);
    }
    return *this;
}

std::wstring ComboBox::get_text() const
{
    if (hwnd) {
        std::vector<wchar_t> buffer(ComboBox_GetTextLength(hwnd) + 2);
        ComboBox_GetText(hwnd, buffer.data(), buffer.size());
        return buffer.data();
    }
    else {
        return text;
    }
}

ComboBox& ComboBox::set_text(const std::wstring& text)
{
    if (hwnd) {
        ComboBox_SetText(hwnd, text.c_str());
    }
    else {
        this->text = text;
    }
    return *this;
}

std::wstring ComboBox::get_list_selected_text() const
{
    if (hwnd) {
        int index = ComboBox_GetCurSel(hwnd);
        if (index >= 0) {
            std::vector<wchar_t> tmp(256);
            tmp.reserve(ComboBox_GetLBTextLen(hwnd, index) + 2);
            ComboBox_GetLBText(hwnd, index, tmp.data());
            return tmp.data();
        }
    }
    return {};
}

void ComboBox::create()
{
    Control::create();
    if (!text.empty()) {
        set_text(text);
    }
    if (!items.empty()) {
        for (const auto& item : items) {
            add_list_item(item);
        }
        items.clear();
    }
}
