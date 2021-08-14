#include "Window.h"
#include <set>
#include <map>
#include <iostream>
#include "Button.h"
#include "ListBox.h"

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
