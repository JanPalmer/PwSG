// TypeMaster.cpp : Definiuje punkt wejścia dla aplikacji.
//

#include "framework.h"
#include "TypeMaster.h"
#include <vector>
#include <ctime>
#include <string>

#define MAX_LOADSTRING 100
#define ID_TIMER_1 7
#define ID_TIMER_2 8

// Zmienne globalne:
HINSTANCE hInst;                                // bieżące wystąpienie
WCHAR szTitle[MAX_LOADSTRING];                  // Tekst paska tytułu
WCHAR szWindowClass[MAX_LOADSTRING];            // nazwa klasy okna głównego

std::vector<HWND> blockHwnds;
std::vector<int> blockSpeeds;
std::vector<LPCWSTR> letters;
const int minFactoryDelay = 300;
const int maxFactoryDelay = 1000;
const int minSpeed = 1;
const int maxSpeed = 5;
const int moveSpacing = 10;
const int blockLimit = 100;
const int blockSize = 25;
int xScreen = GetSystemMetrics(SM_CXSCREEN);
int yScreen = GetSystemMetrics(SM_CYSCREEN);
BOOL pause = FALSE;
int score = 0;

#define RAND_FACTORY_DELAY (rand()%(maxFactoryDelay-minFactoryDelay)+minFactoryDelay)
#define RAND_SPEED (rand()%(maxSpeed-minSpeed)+minSpeed)

// Przekaż dalej deklaracje funkcji dołączone w tym module kodu:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

// BOLCKS
ATOM                BlockRegisterClass(HINSTANCE hInstance);
BOOL                InitBlock(HWND);
LRESULT CALLBACK    BlockWndProc(HWND, UINT, WPARAM, LPARAM);
void MoveBlocks(HWND);
void RemoveBlock(int);
void addPoints(HWND, int);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: W tym miejscu umieść kod.
    srand(time(NULL));    

    // Inicjuj ciągi globalne
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_TYPEMASTER, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);
    BlockRegisterClass(hInstance);


    // Wykonaj inicjowanie aplikacji:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TYPEMASTER));

    MSG msg;

    // Główna pętla komunikatów:
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
//  FUNKCJA: MyRegisterClass()
//
//  PRZEZNACZENIE: Rejestruje klasę okna.
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TYPEMASTER));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_TYPEMASTER);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNKCJA: InitInstance(HINSTANCE, int)
//
//   PRZEZNACZENIE: Zapisuje dojście wystąpienia i tworzy okno główne
//
//   KOMENTARZE:
//
//        W tej funkcji dojście wystąpienia jest zapisywane w zmiennej globalnej i
//        jest tworzone i wyświetlane okno główne programu.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Przechowuj dojście wystąpienia w naszej zmiennej globalnej


   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
      xScreen/4, yScreen/4, xScreen/2, yScreen/2, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNKCJA: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PRZEZNACZENIE: Przetwarza komunikaty dla okna głównego.
//
//  WM_COMMAND  - przetwarzaj menu aplikacji
//  WM_PAINT    - Maluj okno główne
//  WM_DESTROY  - opublikuj komunikat o wyjściu i wróć
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
        {
            SetTimer(hWnd, ID_TIMER_1, moveSpacing, NULL);
            SetTimer(hWnd, ID_TIMER_2, RAND_FACTORY_DELAY, NULL);
        }
        break;
    case WM_KEYDOWN:
        if (blockHwnds.size() > 0)
        {
            int lowestBlock = 0;
            RECT rc;
            GetWindowRect(blockHwnds[0], &rc);
            int lowestHeight = rc.bottom;
            for (int i = 1; i < blockHwnds.size(); i++)
            {                
                GetWindowRect(blockHwnds[i], &rc);
                if (rc.bottom > lowestHeight)
                {
                    lowestBlock = i;
                    lowestHeight = rc.bottom;
                }
            }
            RemoveBlock(lowestBlock);
            addPoints(hWnd, 1);
        }
        break;
    case WM_TIMER:
        {
        if(!pause)
            switch (wParam)
            {
            case ID_TIMER_1:
                MoveBlocks(hWnd);
                break;
            case ID_TIMER_2:
                if (blockHwnds.size() < blockLimit)
                {
                    InitBlock(hWnd);
                    SetTimer(hWnd, ID_TIMER_2, RAND_FACTORY_DELAY, NULL);                    
                }
                break;
            }
        }
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Analizuj zaznaczenia menu:
            switch (wmId)
            {
            case ID_GAME_PAUSE:
                pause = TRUE;
                MessageBox(hWnd, L"Pause", L"Pause", MB_ICONASTERISK | MB_OK);
                pause = FALSE;
                break;
            case ID_GAME_NEW_GAME:
                pause = TRUE;
                int buttonClicked;
                buttonClicked = MessageBox(hWnd, L"New Game", L"New Game", MB_ICONASTERISK | MB_YESNO);
                if (buttonClicked == IDYES)
                {
                    score = 0;
                    for (int i = blockHwnds.size() - 1; i >= 0; i--)
                        RemoveBlock(i);
                }
                pause = FALSE;
                break;
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

// Procedura obsługi komunikatów dla okna informacji o programie.
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

// BLOCKS

ATOM BlockRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = BlockWndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TYPEMASTER));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(CreateSolidBrush(RGB(0, 0, 0)));
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_TYPEMASTER);
    wcex.lpszClassName = L"ChildWClass";
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

LRESULT CALLBACK BlockWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {    
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // Analizuj zaznaczenia menu:
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
    case WM_ERASEBKGND:
        //HDC hdc = (HDC)wParam;
        //hdc.
        break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        RECT rc;
        GetWindowRect(hWnd, &rc);
        // TODO: Tutaj dodaj kod rysujący używający elementu hdc...
        //SetTextColor(hdc, 0x00FFFFFF);
        
        TextOut(hdc, 5, 5, TEXT("a"), strlen("a"));
        EndPaint(hWnd, &ps);
        ReleaseDC(hWnd, hdc);
    }
    break;
    case WM_DESTROY:
        //PostQuitMessage(0);  zamyka wszystko
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

int RandPosition(RECT& rc)
{
    return rand() % (rc.right - rc.left - blockSize) + rc.left;
}

BOOL InitBlock(HWND parentHwnd)
{    
    RECT rc;
    GetWindowRect(parentHwnd, &rc);


    HWND hWnd = CreateWindowEx(0,
        L"ChildWClass",
        (LPCTSTR)NULL,
        WS_CHILD | WS_BORDER,
        rand()%(rc.right-rc.left-2*blockSize), 0, blockSize, blockSize,
        parentHwnd,
        (HMENU) 100, hInst, NULL);

    blockHwnds.push_back(hWnd);
    blockSpeeds.push_back(RAND_SPEED);
    letters.push_back(L"a");

    ShowWindow(hWnd, SW_SHOW);
    UpdateWindow(hWnd);
    return TRUE;
}

void MoveBlocks(HWND parentHwnd)
{
    RECT rcM;
    GetWindowRect(parentHwnd, &rcM);
    for (int i = 0; i < blockHwnds.size(); i++)
    {
        RECT rc;
        GetWindowRect(blockHwnds[i], &rc);        
        if (rcM.bottom <= rc.bottom)
        {
            RemoveBlock(i);
            addPoints(parentHwnd, -2);
        }
        else
        {
            OffsetRect(&rc, 0, blockSpeeds[i]);
            POINT point = { rc.left, rc.top };
            int width = rc.right - rc.left, height = rc.bottom - rc.top;
            ScreenToClient(parentHwnd,&point);
            MoveWindow(blockHwnds[i], point.x, point.y, width, height, TRUE);
        }
    }
    //InvalidateRect(parentHwnd, &rcM,TRUE);
}

void RemoveBlock(int i)
{
    DestroyWindow(blockHwnds[i]);
    blockHwnds.erase(blockHwnds.begin() + i);
    letters.erase(letters.begin() + i);
    blockSpeeds.erase(blockSpeeds.begin() + i);
}

void addPoints(HWND hWnd, int points)
{
    score += points;
    char str[100];
    snprintf(str, 100, "TypeMaster score: %d", score);
    SetWindowTextA(hWnd, str);
}