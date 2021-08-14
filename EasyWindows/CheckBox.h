#pragma once
#include "Button.h"

struct CheckBox :
    public Button
{
    CheckBox();
    bool is_checked() const;
    CheckBox& set_checked(bool checked);
protected:
    DEEP_COPY_DECL(CheckBox)
    void create() override;
private:
    bool checked;
};

