#pragma once
#include "CheckBox.h"

class RadioButton :
    public CheckBox
{
protected:
    DEEP_COPY_DECL(RadioButton)
public:
    RadioButton();
};

