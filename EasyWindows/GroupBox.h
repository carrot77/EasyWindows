#pragma once
#include "Control.h"

class GroupBox :
    public Control
{
protected:
    DEEP_COPY_DECL(GroupBox)
public:
    GroupBox();
    using Control::set_title;
    using Control::get_title;
};

