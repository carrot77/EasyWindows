#include "EditBox.h"
#include <windowsx.h>
#include <CommCtrl.h>
#include <vector>

EditBox::EditBox(bool password, bool multiline, bool readonly, int max_text_len) : 
	text(L""), max_text_len(max_text_len) {
	class_name = WC_EDIT;
	window_style = WS_VISIBLE | WS_CHILD | WS_TABSTOP | ES_LEFT | ES_AUTOHSCROLL;
	if (multiline) {
		window_style |= ES_MULTILINE | ES_WANTRETURN | ES_AUTOVSCROLL;
	}
	if (password) {
		window_style |= ES_PASSWORD;
	}
	if (readonly) {
		window_style |= ES_READONLY;
	}
}

EditBox& EditBox::set_text(const std::wstring& text){
	if (hwnd) {
		Edit_SetText(hwnd, text.c_str());
	}
	else {
		this->text = text;
	}
	return *this;
}

std::wstring EditBox::get_text() const{
	if (hwnd) {
		int len = Edit_GetTextLength(hwnd);
		std::vector<wchar_t> buffer(len+2);
		Edit_GetText(hwnd, buffer.data(), buffer.size());
		return buffer.data();
	}else{
		return text;
	}
}

void EditBox::create() {
	Control::create();
	if (!text.empty()) {
		set_text(text);
		text.clear();
	}
	if (max_text_len >= 0) {
		Edit_LimitText(hwnd, max_text_len);
	}
}