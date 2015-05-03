// gui_prototype.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "gui_prototype.h"
#include "controls.h"

#define MAX_LOADSTRING 100

class Application
{

public:

    static void Init(_In_ HINSTANCE hInstance,
        _In_opt_ HINSTANCE hPrevInstance,
        _In_ LPTSTR    lpCmdLine,
        _In_ int       nCmdShow) 
    {

        // Initialize global strings
        LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
        LoadString(hInstance, IDC_GUI_PROTOTYPE, szWindowClass, MAX_LOADSTRING);
        MyRegisterClass(hInstance);

        // Perform application initialization:
        if (!InitInstance(hInstance, nCmdShow))
        {
            throw std::runtime_error("Unable to create main application window");
        }

        hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_GUI_PROTOTYPE));
    }

    static void AddChild(Window* child) 
    {
        _child_windows.emplace_back(std::unique_ptr<Window>(child));
    }


    static int Run()
    {
        while (GetMessage(&msg, NULL, 0, 0))
        {
            if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }

        return (int)msg.wParam;
    }

    static Window* GetChildWindow(HWND handle)
    {
        auto window = find_if(_child_windows.begin(), _child_windows.end(),
            [&](std::unique_ptr<Window>& window)
        {
            return handle == window->GetHandle();
        });
        return (*window).get();
    }

    static Button* GetChildButton(HWND handle)
    {
        Window* wnd = GetChildWindow(handle);
        Button* btn = dynamic_cast<Button*>(wnd);
        if (NULL == btn)
        {
            std::stringstream ss;
            ss << "Trying to convert non-button control with ID = " << handle;
            throw std::runtime_error(ss.str());
        }
    }

    static void OnConnectClick()
    {
        // add handler
        __asm nop
    }

    static void OnDisconnectClick()
    {
        // add handler
        __asm nop
    }

protected:

    static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
    {
        int wmId = 0, wmEvent = 0;
        PAINTSTRUCT ps;
        HDC hdc;

        switch (message)
        {
        case WM_CREATE:

            for_each(_child_windows.begin(), _child_windows.end(), 
                [&](std::unique_ptr<Window>& window)
            {
                window->Create(hWnd);
            });

            break;
        case WM_KEYDOWN:
            if (wParam == VK_ESCAPE)
            {
                SendMessage(hWnd, WM_CLOSE, 0, 0);
            }
            break;
        case WM_COMMAND:

            wmId = LOWORD(wParam);
            wmEvent = HIWORD(wParam);

            // Parse the menu selections:
            if (0 == lParam)
            {
                return ProcessMenuEntries(hWnd, message, wParam, lParam);
            }
            // Parse the accelerator
            else if (1 == lParam)
            {
            }
            else
            {
                ProcessControls(hWnd, message, wParam, lParam);
            }

            break;
        case WM_PAINT:
            hdc = BeginPaint(hWnd, &ps);
            EndPaint(hWnd, &ps);
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
        return 0;
    }

    // Message handler for about box.
    static INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
    {
        UNREFERENCED_PARAMETER(lParam);
        
        switch (message)
        {
        case WM_INITDIALOG:
            return (INT_PTR)TRUE;

        case WM_COMMAND:
            if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
            {
                EndDialog(hDlg, LOWORD(wParam));
                return (INT_PTR)TRUE;
            }
            break;
        }
        return (INT_PTR)FALSE;
    }

    static void ProcessControls(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
    {
        int wmId, wmEvent;
        wmId = LOWORD(wParam);
        wmEvent = HIWORD(wParam);

        if (BN_CLICKED == wmEvent)
        {
            auto button = find_if(_child_windows.begin(), _child_windows.end(), 
                [&](std::unique_ptr<Window>& window)
            { 
                return (HWND)lParam == window->GetHandle();
            });
            Button* the_button = dynamic_cast<Button*>((*button).get());
            the_button->Clicked();
        }
    }


    static LRESULT ProcessMenuEntries(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam){
        int wmId = LOWORD(wParam);
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, &Application::About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
        return 0;
    }


    // Registers the window class.
    static ATOM MyRegisterClass(HINSTANCE hInstance)
    {
        WNDCLASSEX wcex;

        wcex.cbSize = sizeof(WNDCLASSEX);

        wcex.style = CS_HREDRAW | CS_VREDRAW;
        wcex.lpfnWndProc = &Application::WindowProc;
        wcex.cbClsExtra = 0;
        wcex.cbWndExtra = 0;
        wcex.hInstance = hInstance;
        wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GUI_PROTOTYPE));
        wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
        wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW);
        wcex.lpszMenuName = MAKEINTRESOURCE(IDC_GUI_PROTOTYPE);
        wcex.lpszClassName = szWindowClass;
        wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

        return RegisterClassEx(&wcex);
    }

    // Saves instance handle and creates main window
    // In this function, we save the instance handle in a global variable and
    // create and display the main program window.
    static BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
    {
        HWND hWnd;

        hInst = hInstance; // Store instance handle in our global variable

        hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

        if (!hWnd)
        {
            return FALSE;
        }

        ShowWindow(hWnd, nCmdShow);
        UpdateWindow(hWnd);

        return TRUE;
    }

private:

    // current instance
    static HINSTANCE hInst;

    // The title bar text
    static TCHAR szTitle[MAX_LOADSTRING];

    // the main window class name
    static TCHAR szWindowClass[MAX_LOADSTRING];

    static MSG msg;

    static HACCEL hAccelTable;

    static std::vector< std::unique_ptr<Window>> _child_windows;
};

HINSTANCE Application::hInst = NULL;
TCHAR Application::szTitle[MAX_LOADSTRING] = {};
TCHAR Application::szWindowClass[MAX_LOADSTRING] = {};
MSG Application::msg = {};
HACCEL Application::hAccelTable = {};
std::vector< std::unique_ptr<Window>> Application::_child_windows;


int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// Initialize global controls
    Application::AddChild(new Button(20, 50, 80, 25, _T("Connect"), ID_CONNECT_BUTTON, &Application::OnConnectClick));
    Application::AddChild(new Button(120, 50, 80, 25, _T("Disconnect"), ID_DISCONNECT_BUTTON, &Application::OnDisconnectClick));

    Application::AddChild(new StaticBox(20, 90, 80, 25, _T("IP Address: "), ID_IPADDRESS_STATIC));
    Application::AddChild(new StaticBox(20, 130, 80, 25, _T("Protocol: "), ID_PROTOCOL_STATIC));
    Application::AddChild(new StaticBox(20, 170, 80, 25, _T("User: "), ID_USER_STATIC));
    Application::AddChild(new StaticBox(20, 210, 80, 25, _T("Password: "), ID_PASSWORD_STATIC));

    Application::AddChild(new EditBox(120, 90, 80, 25, _T(""), ID_IPADDRESS_EDIT));
    Application::AddChild(new EditBox(120, 130, 80, 25, _T(""), ID_PROTOCOL_EDIT));
    Application::AddChild(new EditBox(120, 170, 80, 25, _T("user1"), ID_USER_EDIT));
    Application::AddChild(new PasswordBox(120, 210, 80, 25, _T(""), ID_PASSWORD_EDIT));

    Application::Init(hInstance, hPrevInstance, lpCmdLine, nCmdShow);

	// Main message loop:
    return Application::Run();
}

