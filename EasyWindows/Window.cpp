#include "Window.h"
#include <set>
#include <map>
#include <iostream>
#include "EasyWindows.h"

std::set<std::wstring> registered_classes;

std::map<HWND, Window*> windows;

bool Contains(RECT rectangle, POINT point) {
    if (point.x < rectangle.left) return false;
    if (point.x > rectangle.right) return false;
    if (point.y < rectangle.top) return false;
    if (point.y > rectangle.bottom) return false;
    return true;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    Window* win = windows.find(hWnd) == windows.end() ? nullptr : windows[hWnd];
    if (!win) return DefWindowProc(hWnd, message, wParam, lParam);
    bool handled = false;
    switch (message)
    {
    case WM_COMMAND: {
        int wmId = LOWORD(wParam);
        int wmEvent = HIWORD(wParam);
        Control* aktControl = Control::get_control_by_id(wmId);
        Button* btn = dynamic_cast<Button*>(aktControl);
        ListBox* lst = dynamic_cast<ListBox*>(aktControl);
        EditBox* edt = dynamic_cast<EditBox*>(aktControl);
        ComboBox* cbx = dynamic_cast<ComboBox*>(aktControl);
        if (btn) {
            switch (wmEvent)
            {
            case BN_CLICKED:
                btn->clicked.Invoke(*btn);
                handled = true;
                break;
            }
        }
        else if (lst) {
            switch (wmEvent)
            {
            case LBN_DBLCLK:
                handled = true;
                lst->item_dbl_clicked.Invoke(*lst, lst->get_caret_item());
                break;
            case LBN_SELCHANGE:
            case LBN_SELCANCEL:
                handled = true;
                lst->selection_changed.Invoke(*lst);
                break;
            }
        }
        else if (edt) {
            switch (wmEvent)
            {
            case EN_CHANGE:
                handled = true;
                edt->text_changed.Invoke(*edt, edt->get_text());
                break;
            default:
                break;
            }
        }
        else if (cbx) {
            switch (wmEvent)
            {
            case CBN_EDITCHANGE:
                handled = true;
                cbx->text_changed.Invoke(*cbx, cbx->get_text());
                break;
            case CBN_SELCHANGE:
                handled = true;
                cbx->text_changed.Invoke(*cbx, cbx->get_list_selected_text());
                break;
            default:
                break;
            }
        }
    }
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        win->paint.Invoke(win, hdc, ps);
        EndPaint(hWnd, &ps);
        handled = true;
    }
    break;
    case WM_SIZE:
        handled = true;
        win->handle_size_changed({LOWORD(lParam), HIWORD(lParam)});
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        handled = true;
        break;
    }
    if (!handled) {
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

Window::Window(const std::wstring& class_name) : Control(0) {
    this->class_name = class_name;
    this->window_style = WS_OVERLAPPEDWINDOW;
    dummy.set_location({ 0,0 });
    Control::add_control(dummy);
}

Window::Window(Window&& window) noexcept : 
    Control(std::move(window)), 
    dummy(std::move(window.dummy)) 
{
}

void Window::show(){
	if (registered_classes.find(class_name) == registered_classes.end()) {
        WNDCLASSEX wc;
        ZeroMemory(&wc, sizeof(WNDCLASSEX));

        wc.cbSize = sizeof(WNDCLASSEXA);
        wc.style = CS_HREDRAW | CS_VREDRAW;
        wc.lpfnWndProc = WndProc;
        wc.hInstance = GetModuleHandle(nullptr);
        wc.hCursor = LoadCursor(NULL, IDC_ARROW);
        wc.lpszClassName = class_name.c_str();
        RegisterClassEx(&wc);
        registered_classes.emplace(class_name);
    }

    create();
    windows[hwnd] = this;
    ShowWindow(hwnd, SW_NORMAL);
    UpdateWindow(hwnd);
}

void Window::run() {
    MSG msg;
    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

void Window::handle_size_changed(SIZE new_size) {
    dummy.set_size(new_size);
    Control::handle_size_changed(new_size);
}

Control& Window::add_control(Control& control) {
    dummy.add_control(control);
    return *this;
}

Control& Window::add_control(Control&& control) {
    dummy.add_control(std::move(control));
    return *this;
}

void Window::remove_control(Control& control) {
    dummy.remove_control(control);
}
