#pragma once
#include "Control.h"

struct ListBox :
    public Control
{
    enum class SelectionMode {NoSelect, SingleSelect, MultipleSelect};
    ListBox(SelectionMode selectionMode = SelectionMode::SingleSelect, bool sort = true);
    EventHandler<ListBox&> selection_changed;
    EventHandler<ListBox&, std::wstring> item_dbl_clicked;
    ListBox& add_string(const std::wstring&);
    ListBox& remove_string(const std::wstring&);
    ListBox& clear();
    ListBox& set_selected(const std::wstring&, bool);
    std::list<std::wstring> get_selection() const;
    std::list<std::wstring> get_items() const;
    std::wstring get_caret_item() const;
protected:
    virtual void create() override;
    DEEP_COPY_DECL(ListBox)
private:
    int max_item_length;
    std::list<std::wstring> init_strings;
    std::list<std::wstring> selected_strings;
};

