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
	window.set_size({800,600});
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
	button2.clicked += [&button2](Button&) {
		button2.set_title(button2.is_checked() ? L"Checked" : L"Unchecked");
	};
	Button move_test;
	move_test.clicked += [](Button& btn) {btn.set_title(L"Miau"); };
	GroupBox gb;
	gb.set_title(L"Group1").set_location({ 10,140 }).set_size({ 210, 120 });
	gb.add_control(std::move(RadioButton().set_title(L"RB1").set_rectangle({ 10,20,100,40 }))).
		add_control(std::move(RadioButton().set_title(L"RB2").set_rectangle({ 110,20,200,40 }))).
		add_control(std::move(RadioButton().set_title(L"RB3").set_rectangle({ 10,50,100,70 }))).
		add_control(std::move(RadioButton().set_title(L"RB4").set_rectangle({ 110,50,200,70 })));
	ListBox lb(ListBox::SelectionMode::MultipleSelect, false);
	lb.set_location({ 230,140 }).set_size({ 210, 120 });
	lb.add_string(L"elso").add_string(L"masodik").add_string(L"harmadik");
	lb.item_dbl_clicked += [&window](ListBox&, std::wstring str) {
		window.set_title(str);
	};
	EditBox eb(false, true, false);
	eb
		.set_text(L"test\r\ntext")
		.add_anchor(Control::Anchor_Right)
		.set_location({460, 10})
		.set_size({120, 100});
	eb.text_changed += [&window](EditBox&, std::wstring str) {
		window.set_title(str);
	};
	ComboBox cb(ComboBox::ComboBoxStyles::DropDownList);
	cb.add_list_item(L"L1").
		add_list_item(L"L2").
		add_list_item(L"L3").
		set_text(L"Combobox").
		set_rectangle({ 10, 300, 360, 360 });
	cb.text_changed += [&window](ComboBox&, std::wstring str) {
		window.set_title(str);
	};
	TableLayoutPanel<3, 4> tp(
		{
			ColumnDefinition{SizeDefinition::Type::Percent, 50},
			ColumnDefinition{SizeDefinition::Type::Absolute, 100},
			ColumnDefinition{SizeDefinition::Type::Percent, 50},
		}, {
			RowDefinition{SizeDefinition::Type::Percent, 50},
			RowDefinition{SizeDefinition::Type::Absolute, 100},
			RowDefinition{SizeDefinition::Type::Percent, 50},
			RowDefinition{SizeDefinition::Type::Percent, 50},
		}
	);
	tp
		.add_control(std::move(Label()
			.set_title(L"Label1")
			.set_rectangle({0,0,100,100})), 0, 0)
		.add_control(std::move(Label()
			.set_title(L"Label2")
			.set_rectangle({ 0,0,100,100 })), 0, 1)
		.add_control(std::move(Label()
			.set_title(L"Label3")
			.set_rectangle({ 0,0,100,100 })), 0, 2)
		.add_control(std::move(Label()
			.set_title(L"Label4")
			.set_rectangle({ 0,0,100,100 })), 0, 3)
		.add_control(std::move(Label()
			.set_title(L"Label5")
			.set_rectangle({ 0,0,100,100 })), 1, 0)
		.add_control(std::move(Label()
			.set_title(L"Label6")
			.set_rectangle({ 0,0,100,100 })), 1, 1)
		.add_control(std::move(Label()
			.set_title(L"Label7")
			.set_rectangle({ 0,0,100,100 })), 1, 2)
		.add_control(std::move(Label()
			.set_title(L"Label8")
			.set_rectangle({ 0,0,100,100 })), 1, 3)
		.add_control(std::move(Label()
			.set_title(L"Label9")
			.set_rectangle({ 0,0,100,100 })), 2, 0)
		.add_control(std::move(Label()
			.set_title(L"LabelA")
			.set_rectangle({ 0,0,100,100 })), 2, 1)
		.add_control(std::move(Label()
			.set_title(L"LabelB")
			.set_rectangle({ 0,0,100,100 })), 2, 2)
		.add_control(std::move(Label()
			.set_title(L"LabelC")
			.set_rectangle({ 0,0,100,100 })), 2, 3)
		.set_rectangle({ 210,400,490,490 });
	window
		.add_control(tp)
		.add_control(std::move(Label()
			.set_title(L"Label")
			.set_rectangle({ 10,10,110, 60 })))
		.add_control(button)
		.add_control(button2)
		.add_control(std::move(move_test
			.set_title(L"Kiscica")
			.set_location({ 10, 70 })
			.set_size({ 100, 60 })))
		.add_control(cb)
		.add_control(std::move(gb))
		.add_control(lb)
		.add_control(eb)
		.add_control(std::move(RadioButton()
			.set_title(L"RB5")
			.set_rectangle({ 10, 400, 100, 440 })))
		.add_control(std::move(RadioButton()
			.set_title(L"RB6")
			.set_rectangle({ 110,400,190,440 })))
		.add_control(std::move(RadioButton()
			.set_title(L"RB7")
			.set_rectangle({ 10,450,100,490 })))
		.add_control(std::move(RadioButton()
			.set_title(L"RB8")
			.set_rectangle({ 110,450,190,490 })))
		;
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
