#include <stdlib.h>
#include <math.h>
#include <windows.h>

/* Window class name */
#define WND_CLASS_NAME "My Window Class"

LRESULT CALLBACK MyWinFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam );

/* The main program function */
INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    CHAR *CmdLine, INT CmdShow )
{
  WNDCLASS wc;
  HWND hWnd;
  MSG msg;

  /* Register window class */
  wc.style = 0;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hCursor = LoadCursor(NULL, IDC_HAND);
  wc.hIcon = LoadIcon(NULL, IDI_EXCLAMATION);
  wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
  wc.hInstance = hInstance;
  wc.lpszClassName = WND_CLASS_NAME;
  wc.lpszMenuName = NULL;
  wc.lpfnWndProc = MyWinFunc;

  if (!RegisterClass(&wc))
  {
    MessageBox(NULL, "Error register window class", "ERROR", MB_OK | MB_ICONERROR);
    return 0;
  }

  /* Create window */
  hWnd = CreateWindow(WND_CLASS_NAME,
    "30!",
    WS_OVERLAPPEDWINDOW,
    CW_USEDEFAULT, CW_USEDEFAULT,
    CW_USEDEFAULT, CW_USEDEFAULT,
    NULL, NULL, hInstance, NULL);
  if (hWnd == NULL)
  {
    MessageBox(NULL, "Create window erroe", "ERROR", MB_OK | MB_ICONERROR);
    return 0;
  }
  ShowWindow(hWnd, CmdShow);
  UpdateWindow(hWnd);                                                                        
  /* Run message loop */

  while (GetMessage(&msg, NULL, 0, 0))
    DispatchMessage(&msg);
  return 30;
}

LRESULT CALLBACK MyWinFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
  CHAR s[150];
  DOUBLE a;
  DOUBLE r;
  DOUBLE p = 3.141592653589;
  SYSTEMTIME t;
  HDC hDC;
  PAINTSTRUCT ps;
  static INT w, h;
  static BITMAP bm;
  static HBITMAP hBm, hBmLogo;
  static HDC hMemDC, hMemDCLogo;
  GetLocalTime(&t);
    switch (Msg)
  {
  case WM_CREATE:
    SetTimer(hWnd, 30, 10, NULL);
    hBmLogo = LoadImage(NULL, "SE.BMP", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    GetObject(hBmLogo, sizeof(bm), &bm);
    hDC = GetDC(hWnd);
    hMemDC = CreateCompatibleDC(hDC);
    hMemDCLogo = CreateCompatibleDC(hDC);
    SelectObject(hMemDCLogo, hBmLogo);
    ReleaseDC(hWnd, hDC);
    return 0;
  case WM_SIZE:
    w = LOWORD(lParam);
    h = HIWORD(lParam);
    if (hBm != NULL)
      DeleteObject(hBm);
    hDC = GetDC(hWnd);
    hBm = CreateCompatibleBitmap(hDC, w, h);
    ReleaseDC(hWnd, hDC);
    SelectObject(hMemDC, hBm);
    SendMessage(hWnd, WM_TIMER, 0, 0);
    return 0;
  case WM_ERASEBKGND:
    return 0;
  case WM_TIMER:
    Rectangle(hMemDC, 0, 0, w + 1, h + 1);
    BitBlt(hMemDC, (w - bm.bmWidth) / 2, (h - bm.bmHeight) / 2, bm.bmWidth, bm.bmHeight, hMemDCLogo, 0, 0, SRCCOPY);
    srand(59);
    SetBkMode(hMemDC, TRANSPARENT);
    SetTextColor(hMemDC, RGB(255, 0, 0));
    a = (t.wSecond + t.wMilliseconds / 1000.0) * 2 * p / 60;
    r = bm.bmHeight / 2.3;
    MoveToEx(hMemDC, w / 2, h / 2, NULL);
    LineTo(hMemDC, w / 2 + sin(a) * r, h / 2 - cos(a) * r);
    a = (t.wMinute + t.wSecond / 60.0)  * 2 * p / 60;
    r = bm.bmHeight / 3.3;
    MoveToEx(hMemDC, w / 2, h / 2, NULL);
    LineTo(hMemDC, w / 2 + sin(a) * r, h / 2 - cos(a) * r);
    a = (t.wHour + t.wMinute / 60.0) / 12 * 2 * p / 60;
    r = bm.bmHeight / 4.2;
    MoveToEx(hMemDC, w / 2, h / 2, NULL);
    LineTo(hMemDC, w / 2 + sin(a) * r , h / 2 - cos(a) * r);
    TextOut(hMemDCLogo, 30, 30, s, sprintf(s, "%02i.%02i.%i", t.wDay, t.wMonth, t.wYear));
    InvalidateRect(hWnd, NULL, FALSE);
    return 0;
  case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);
    BitBlt(hDC, 0, 0, w, h, hMemDC, 0, 0, SRCCOPY);
    EndPaint(hWnd, &ps);
    return 0;
  case WM_DESTROY:
    KillTimer(hWnd, 30);
    DeleteDC(hMemDC);
    DeleteObject(hBm);
    PostQuitMessage(0);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
}