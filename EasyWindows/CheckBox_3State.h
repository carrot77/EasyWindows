#pragma once
#include "Button.h"
struct CheckBox_3State :
    public Button
{
    enum class CheckState{Checked, Indeterminate, Unchecked};
    CheckBox_3State();
    EventHandler<CheckBox_3State&> clicked;
    CheckState checked_state() const;
    CheckBox_3State& set_checked_state(CheckState);
protected:
    DEEP_COPY_DECL(CheckBox_3State)
    void create() override;
private:
    CheckState checked;
};

