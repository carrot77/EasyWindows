#pragma once
#include "Control.h"

class Button : public Control
{
	bool enabled;
protected:
	DEEP_COPY_DECL(Button)
	void create() override;
public:
	Button();
	EventHandler<Button&> clicked;
	Button& set_enabled(bool enabled = true);
};

