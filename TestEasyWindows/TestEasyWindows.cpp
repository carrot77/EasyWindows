// TestEasyWindows.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "../EasyWindows/EasyWindows.h"

const wchar_t click_text[] = L"Clicked: ";

template <class T, unsigned N>
constexpr unsigned size_of(const T (&)[N]) {
	return N;
}

int main()
{
	Window window;
	window.set_title(L"Hello cicus");
	Button button;
	button.set_title(L"Button").set_rectangle({ 120, 10, 220, 60 });
	CheckBox button2;
	button.clicked += [&window, &button2](Button& btn) {
		Label other_label;
		other_label.set_title(L"New label").set_rectangle({ 120, 70, 220, 120 });
		window.add_control(std::move(other_label));
		btn.set_enabled(false);
		button2.set_enabled(true);
	};
	button2.set_enabled(false).set_title(L"CheckBox").set_rectangle({ 230, 10, 330, 60 });
	button2.clicked += [](CheckBox& btn) {
		btn.set_title(btn.is_checked() ? L"Checked" : L"Unchecked");
	};
	window.add_control(std::move(Label().set_title(L"Label").set_rectangle({ 10,10,110, 60 })));
	window.add_control(button);
	window.add_control(button2);
	Button move_test;
	move_test.clicked += [](Button& btn) {btn.set_title(L"Miau"); };
	window.add_control(move_test.
		set_title(L"Kiscica").
		set_location({10, 70}).
		set_size({100, 60}));
	window.show();
	window.run();
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
