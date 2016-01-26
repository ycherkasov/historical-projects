#pragma once
#include "stdafx.h"

class Window
{
public:

    Window(int x, int y, int width, int height, TCHAR* window_text, int instance_id) :
        _handle(), _window_text(window_text), _instance_id((HMENU)instance_id),
        _x(x), _y(y), _width(width), _height(height)
    {
    }

    virtual ~Window()
    {
    }

    void CreateBase(TCHAR* windows_class, DWORD flags = WS_VISIBLE | WS_CHILD, HWND hParent = NULL)
    {
        _handle = CreateWindow(windows_class,
            _window_text.c_str(),
            flags,
            _x, _y, _width, _height,
            hParent, _instance_id, NULL, NULL);
    }

    virtual void Create(HWND hParent = NULL) = 0;

    virtual void SetHandle(HWND h)
    {
        _handle = h;
    }

    virtual HWND GetHandle() const
    {
        return _handle;
    }

    virtual HMENU GetId() const
    {
        return _instance_id;
    }

private:
    HWND _handle = NULL;
    tstring _window_text;
    HMENU _instance_id;
    int _x = 0;
    int _y = 0;
    int _width = 0;
    int _height = 0;
};


class Button : public Window
{
public:

    typedef void(*click_handle_t)();

    Button(int x, int y, int width, int height, TCHAR* windows_text, int instance_id, click_handle_t click_handle) :
        Window(x, y, width, height, windows_text, instance_id)
        , _click_handle(click_handle)
    {}

    virtual void Create(HWND hParent /* = NULL */)
    {
        Window::CreateBase(_T("button"), WS_VISIBLE | WS_CHILD | WS_BORDER, hParent);
    }

    virtual void Clicked()
    {
        _click_handle();
    }
        
private:
    click_handle_t _click_handle;
};


class StaticBox : public Window
{
public:
    StaticBox(int x, int y, int width, int height, TCHAR* windows_text, int instance_id) :
        Window(x, y, width, height, windows_text, instance_id)
    {
    }

    virtual void Create(HWND hParent /* = NULL */)
    {
        Window::CreateBase(_T("static"), WS_VISIBLE | WS_CHILD, hParent);
    }

    void SetText(const TCHAR* text)
    {
        SetWindowText(GetHandle(), text);
    }
};


class EditBox : public Window
{
public:
    EditBox(int x, int y, int width, int height, TCHAR* windows_text, int instance_id) :
        Window(x, y, width, height, windows_text, instance_id){}

    virtual void Create(HWND hParent /* = NULL */)
    {
        Window::CreateBase(_T("edit"), WS_VISIBLE | WS_CHILD | WS_BORDER, hParent);
    }

    std::string GetText()
    {
        TCHAR buffer[1024] = {};
        int len = GetWindowTextLengthW(GetHandle())+1;
        GetWindowText(GetHandle(), buffer, len);
        return std::string(buffer);
    }
};


class PasswordBox : public EditBox
{
public:
    PasswordBox(int x, int y, int width, int height, TCHAR* windows_text, int instance_id) :
        EditBox(x, y, width, height, windows_text, instance_id){}

    virtual void Create(HWND hParent /* = NULL */)
    {
        Window::CreateBase(_T("edit"), WS_VISIBLE | WS_CHILD | WS_BORDER | ES_PASSWORD, hParent);
    }
};

static const wchar_t* test_str = L"Test string";

class ListBox : public Window
{
public:
    ListBox(int x, int y, int width, int height, TCHAR* windows_text, int instance_id) :
        Window(x, y, width, height, windows_text, instance_id){}

    virtual void Create(HWND hParent /* = NULL */)
    {
        Window::CreateBase(_T("listbox"), WS_VISIBLE | WS_CHILD | WS_BORDER | WS_VSCROLL, hParent);
    }

    void AddString(const TCHAR* string_to_add)
    {
        SendMessage(GetHandle(), LB_ADDSTRING, 0, (LPARAM)string_to_add);
        SendMessage(GetHandle(), LB_SETCURSEL, _items, (LPARAM)string_to_add);
        ++_items;
    }

    void RemoveString(size_t index_to_delete)
    {
        if (_items == 0)
        {
            return;
        }
        SendMessageW(GetHandle(), LB_DELETESTRING, (WPARAM)index_to_delete, 0);
        --_items;
    }

    size_t GetCount() const
    {
        return _items;
    }
private:
    size_t _items = 0;
};
