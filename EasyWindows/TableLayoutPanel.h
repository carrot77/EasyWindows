#pragma once
#include "Panel.h"
#include "Control.h"
#include <array>

struct SizeDefinition {
	enum class Type {Absolute, Percent};
	Type type;
	unsigned size;
};

typedef SizeDefinition ColumnDefinition;
typedef SizeDefinition RowDefinition;

template <size_t C, size_t R>
class TableLayoutPanel : public Panel {
	ObservableArray<ColumnDefinition,C> columns;
	ObservableArray<RowDefinition, R> rows;
	std::array<Control*, C * R> sub_items;
	Control* &get_item(int col, int row) {
		return sub_items[col * R + row];
	}
	template <class T, size_t N>
	std::array<int, N>distribute(ObservableArray<T,N>& items, int size) {
		int percent_sum = 0;
		int size_sum = 0;
		for (const SizeDefinition& def : items) {
			switch (def.type)
			{
			case SizeDefinition::Type::Absolute:
				size_sum += def.size;
				break;
			case SizeDefinition::Type::Percent:
				percent_sum += def.size;
				break;
			default:
				break;
			}
		}
		int size_left = size - size_sum;
		std::array<int, N> ret;
		for (int i = 0; i < N; ++i) {
			switch (items[i].type) {
			case SizeDefinition::Type::Absolute:
				ret[i] = items[i].size;
				break;
			case SizeDefinition::Type::Percent:
				ret[i] = (items[i].size * size_left) / percent_sum;
				break;
			default:
				break;
			}
		}
		return ret;
	}
	void resize() {
		auto size = get_size();
		auto drows = distribute(rows, size.cy);
		auto dcols = distribute(columns, size.cx);
		int cSum = 0;
		for (int i = 0; i < C; ++i) {
			int rSum = 0;
			for (int j = 0; j < R; ++j) {
				Control* item;
				if (item = get_item(i, j)) {
					item->set_size({dcols[i], drows[j]});
					item->set_location({cSum, rSum});
				}
				rSum += drows[j];
			}
			cSum += dcols[i];
		}
	}
protected:
	void create() override {
		Panel::create();
		resize();
	}
	void handle_size_changed(SIZE) override {
		resize();
	}
	TableLayoutPanel(TableLayoutPanel<C,R>&&) noexcept = default;
	inline Control* deep_move(Control && c) noexcept override {
		return new TableLayoutPanel<C,R>(std::move(dynamic_cast<TableLayoutPanel<C,R>&>(c)));
	}
	void control_deleted(Control* _old) override {
		for (int i=0; i<C; ++i) 
			for (int j=0; j<R; ++j)
				if (get_item(i, j) == _old) {
					get_item(i, j) = nullptr;
				}
	}
	void control_moved(Control* _old, Control* _new) {
		for (int i = 0; i < C; ++i)
			for (int j = 0; j < R; ++j)
				if (get_item(i, j) == _old) {
					get_item(i, j) = _new;
				}
	}
public:
	TableLayoutPanel(
		const std::array<ColumnDefinition, C>& columns, 
		const std::array<RowDefinition, R>& rows) :
			columns(columns), rows(rows), sub_items{nullptr}
	{
	}
	TableLayoutPanel& add_control(Control& c, int pos_x, int pos_y) {
		if (get_item(pos_x, pos_y)) {
			delete get_item(pos_x, pos_y);
		}
		get_item(pos_x, pos_y) = &c;
		Control::add_control(c);
		return *this;
	}
	TableLayoutPanel& add_control(Control&& c, int pos_x, int pos_y) {
		if (get_item(pos_x, pos_y)) {
			delete get_item(pos_x, pos_y);
		}
		get_item(pos_x, pos_y) = &c;
		Control::add_control(std::move(c));
		return *this;
	}
	void clear_control(int pos_x, int pos_y, bool _delete = true) {
		if (get_item(pos_x, pos_y)) {
			if (_delete) {
				delete get_item(pos_x, pos_y);
			}
			get_item(pos_x, pos_y) = nullptr;
		}
	}
};
