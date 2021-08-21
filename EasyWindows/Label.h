#pragma once

#include "Control.h"

struct Label : public Control
{
	Label();
	using Control::set_title;
	using Control::get_title;
protected:
	DEEP_COPY_DECL(Label)
};

