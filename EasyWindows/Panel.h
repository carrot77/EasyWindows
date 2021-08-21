#pragma once
#include "Control.h"

struct Panel :
    public Control
{
	Panel();
protected:
	friend class Window;
	DEEP_COPY_DECL(Panel)
};

