#include "GroupBox.h"

GroupBox::GroupBox() : Control(0) {
	class_name = L"button";
	window_style = WS_VISIBLE | WS_CHILD | BS_GROUPBOX;
}
