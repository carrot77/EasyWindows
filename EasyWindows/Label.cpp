#include "Label.h"
#include <CommCtrl.h>

Label::Label() {
	class_name = WC_STATIC;
	window_style = WS_VISIBLE | WS_CHILD | SS_SIMPLE;
}