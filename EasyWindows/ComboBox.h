#pragma once
#include "Control.h"

struct ComboBox : public Control
{
	enum class ComboBoxStyles {Simple, DropDown, DropDownList};
	ComboBox(ComboBoxStyles, bool sort = true);
	std::list<std::wstring> get_list_items() const;
	ComboBox& add_list_item(const std::wstring&);
	ComboBox& remove_list_item(const std::wstring&);
	std::wstring get_text() const;
	ComboBox& set_text(const std::wstring&);
	EventHandler<ComboBox&, std::wstring> text_changed;
	std::wstring get_list_selected_text() const;
protected:
	void create() override;
	DEEP_COPY_DECL(ComboBox)
private:
	std::list<std::wstring> items;
	std::wstring text;
};

