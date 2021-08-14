#include "GroupBox.h"
#include <CommCtrl.h>

GroupBox::GroupBox() : Control(0) {
	class_name = WC_BUTTON;
	window_style = WS_VISIBLE | WS_CHILD | BS_GROUPBOX;
}
