#pragma once
#include "stdafx.h"

class Window
{
public:

    Window(int x, int y, int width, int height, HMENU instance_id, TCHAR* window_text) :
        _handle(), _window_text(window_text), _instance_id(instance_id),
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
        Window(x, y, width, height, (HMENU)instance_id, windows_text)
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
        Window(x, y, width, height, (HMENU)(instance_id), windows_text)
    {
    }

    virtual void Create(HWND hParent /* = NULL */)
    {
        Window::CreateBase(_T("static"), WS_VISIBLE | WS_CHILD, hParent);
    }
};


class EditBox : public Window
{
public:
    EditBox(int x, int y, int width, int height, TCHAR* windows_text, int instance_id) :
        Window(x, y, width, height, (HMENU)(instance_id), windows_text){}

    virtual void Create(HWND hParent /* = NULL */)
    {
        Window::CreateBase(_T("edit"), WS_VISIBLE | WS_CHILD | WS_BORDER, hParent);
    }

    std::string GetText()
    {
        int len = GetWindowTextLengthW(GetHandle());
        std::vector<TCHAR> edit_text(len);
        GetWindowText(GetHandle(), &edit_text[0], len);
        return std::string(edit_text.begin(), edit_text.end());
    }
};


class PasswordBox : public Window
{
public:
    PasswordBox(int x, int y, int width, int height, TCHAR* windows_text, int instance_id) :
        Window(x, y, width, height, (HMENU)(instance_id), windows_text){}

    virtual void Create(HWND hParent /* = NULL */)
    {
        Window::CreateBase(_T("edit"), WS_VISIBLE | WS_CHILD | WS_BORDER | ES_PASSWORD, hParent);
    }

    std::string GetText()
    {
        int len = GetWindowTextLengthW(GetHandle());
        std::vector<TCHAR> edit_text(len);
        GetWindowText(GetHandle(), &edit_text[0], len);
        return std::string(edit_text.begin(), edit_text.end());
    }
};
