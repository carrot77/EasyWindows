#pragma once
#include "Control.h"

struct EditBox :
    public Control
{
    EditBox(bool password = false, bool multiline = false, bool readonly = false, int max_text_len = -1);
    EventHandler<EditBox&, std::wstring> text_changed;
    EditBox& set_text(const std::wstring &);
    std::wstring get_text() const;
protected:
    virtual void create() override;
    DEEP_COPY_DECL(EditBox)
private:
    std::wstring text;
    int max_text_len;
};

