#pragma once

#include "Control.h"

struct Label : public Control
{
	Label();
protected:
	friend class Window;
	DEEP_COPY_DECL(Label)
};

