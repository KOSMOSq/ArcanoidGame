#include "framework.h"
#include "Arcanoid.h"
#include "Game.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

Object Platform(5, 25, 0, 0);
Object Ball(6, 15, 0.6f, 0.6f);

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

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
    LoadStringW(hInstance, IDC_ARCANOID, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_ARCANOID));

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

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ARCANOID));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_ARCANOID);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      0, 0, 700, 900, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
    {
        StartGame(1,hWnd, Ball);
        break;
    }
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            switch (wmId)
            {
            case 1: {
                StartGame(1, hWnd, Ball);
                break;
            }
            case 2: {
                StartGame(2, hWnd, Ball);
                break;
            }
            case 3: {
                StartGame(3, hWnd, Ball);
                break;
            }
               
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
            HBITMAP Heart = (HBITMAP)LoadImage(NULL, L"heart.BMP", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
            HDC HeartDC = CreateCompatibleDC(hdc);
            SelectObject(HeartDC, Heart);
            HBRUSH red = CreateSolidBrush(RGB(255, 100, 100));
            HBRUSH green = CreateSolidBrush(RGB(100, 255, 100));
            HBRUSH blue = CreateSolidBrush(RGB(100, 100, 255));
            for (int i = 0; i < life; i++)
            {
                StretchBlt(hdc, 14 * i, 0, 14, 14, HeartDC, 0, 0, 14, 14, SRCCOPY);
            }
            for (int i = 0; i < 100; i++)
            {
                if (arr[i].life > 0) {
                    switch (arr[i].life)
                    {
                    case 1: {
                        SelectObject(hdc, red);
                        break;
                    }
                    case 2: {
                        SelectObject(hdc, green);
                        break;
                    }
                    case 3: {
                        SelectObject(hdc, blue);
                        break;
                    }
                    }
                    Rectangle(hdc, arr[i].x * Width + LeftShift, arr[i].y * Height + TopShift,
                        arr[i].x * Width + Width + LeftShift, arr[i].y * Height + Height + TopShift);
                }
               
            }
            SelectObject(hdc, red);
            Rectangle(hdc, Platform.x * Width, Platform.y * Height,
                Platform.x * Width + PlatformWidth, Platform.y * Height + 25);
            Ellipse(hdc, Ball.x * Width, Ball.y * Height,
                Ball.x * Width + Radius, Ball.y * Height + Radius);
            DeleteObject(red);
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_TIMER: {
        BallMove(hWnd,Ball,Platform);
        if (life == 0)
        {
            KillTimer(hWnd, timer);
            MessageBox(hWnd, L"Вы проиграли!", L"Конец игры", MB_OK);
        }
        InvalidateRect(hWnd, NULL, true);
        break;
    }
    case WM_KEYDOWN: {
        switch (wParam)
        {
        case VK_RIGHT: {
            RECT rt;
            GetClientRect(hWnd, &rt);
            if (Platform.x * Width + PlatformWidth < rt.right)
            {
                Platform.x += 1;
            }
            break;
        }
        case VK_LEFT: {
            if (Platform.x * Width > 0)
            {
                Platform.x += -1;
            }

            break;
        }
        case VK_UP: {
            if (UserSpeed < 0.5)
            {
                UserSpeed += 0.05f;
            }
            
            break;
        }
        case VK_DOWN: {
            if (UserSpeed > -0.3)
            {
                UserSpeed -= 0.05f;
            }
            
            break;
        }
        }
        break;
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

