#include "Panel.h"
#include <CommCtrl.h>

Panel::Panel() {
	class_name = WC_STATIC;
	window_style = WS_VISIBLE | WS_CHILD | SS_LEFT;
}