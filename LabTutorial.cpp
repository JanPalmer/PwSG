// LabTutorial.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "LabTutorial.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
ATOM MyRegisterClass1(HINSTANCE hInstance);
LRESULT CALLBACK eee(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_LABTUTORIAL, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass1(hInstance);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LABTUTORIAL));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
    wcex.hCursor        = LoadCursor(nullptr, MAKEINTRESOURCE(IDC_ARROW));
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName = NULL; //MAKEINTRESOURCEW(IDC_LABTUTORIAL);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ICON1));

    return RegisterClassExW(&wcex);
}
ATOM MyRegisterClass1(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = eee;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
    wcex.hCursor = LoadCursor(nullptr, MAKEINTRESOURCE(IDC_ARROW));
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 4);
    wcex.lpszMenuName = NULL; //MAKEINTRESOURCEW(IDC_LABTUTORIAL);
    wcex.lpszClassName = L"negus";
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ICON1));

    return RegisterClassExW(&wcex);
}
//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable
   int screemWidth = GetSystemMetrics(SM_CXSCREEN);
   int screenHeight = GetSystemMetrics(SM_CYSCREEN);

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      screemWidth/2 - 200, screenHeight/2 - 150, 400, 300, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   SetWindowText(hWnd, L"FruitNinja");
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK eee(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    return 0;
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static HCURSOR cursor = NULL;
    switch (message)
    {
    case WM_CREATE:
    {
        cursor = LoadCursor(hInst, MAKEINTRESOURCE(IDC_CURSOR1));
        for (int i = 0; i <= 300; i += 100) {
            CreateWindowW(L"negus", (LPCWSTR)0, WS_CHILD | WS_VISIBLE, i, 100, 100, 1000, hWnd, NULL, hInst, NULL);
            CreateWindowW(L"negus", (LPCWSTR)0, WS_CHILD | WS_VISIBLE, i, 200, 100, 100, hWnd, NULL, hInst, NULL);
            CreateWindowW(L"negus", (LPCWSTR)0, WS_CHILD | WS_VISIBLE, i, 0, 100, 100, hWnd, NULL, hInst, NULL);
        }
    }
    break;
    case WM_SETCURSOR:
    {
        SetCursor(cursor);
    }
    break;
    case WM_WINDOWPOSCHANGING:
    {
        int screemWidth = GetSystemMetrics(SM_CXSCREEN);
        int screenHeight = GetSystemMetrics(SM_CYSCREEN);
        WINDOWPOS* winpos = (WINDOWPOS*)lParam;
        winpos->x = screemWidth/2 - 200;
        winpos->y = screenHeight/2 - 150;
        winpos->cx = 400;
        winpos->cy = 300;
    }
    //case WM_MOVING:
    //{
    //    RECT *rs = (RECT*)lParam;
    //    rs->left = 500;
    //    rs->right = rs->left + 400;
    //    rs->top = 400;
    //    rs->bottom = rs->top + 300;
    //}
    //break;
    //case WM_GETMINMAXINFO:
    //{
    //    MINMAXINFO* minmaxinfo = (MINMAXINFO*)lParam;
    //    minmaxinfo->ptMaxSize.x = minmaxinfo->ptMaxTrackSize.x = 800;
    //    minmaxinfo->ptMaxSize.y = minmaxinfo->ptMaxTrackSize.y = 400;
    //    minmaxinfo->
    //}
    case WM_SYSCOMMAND:
    {
        switch (wParam)
        {
        case SC_MINIMIZE:
            return 0;
        default:
            //lResult = DefWindowProc(hwnd, message, wParam, lParam);
            break;
        }
    }
        break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
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

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
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
