#include "Window.h"
#include <set>
#include <map>
#include <iostream>
#include "Button.h"

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
    switch (message)
    {
    case WM_COMMAND: {
        int wmId = LOWORD(wParam);
        int wmEvent = HIWORD(wParam);
        Control* aktControl = Control::get_control_by_id(wmId);
        if (aktControl){
            // Parse the menu selections:
            switch (wmEvent)
            {
            case BN_CLICKED:
            {
                Button* btn = dynamic_cast<Button*>(aktControl);
                btn->clicked.Invoke(*btn);
            }
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }else{
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        win->paint.Invoke(win, hdc, ps);
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
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
