#include <stdlib.h>
#include <math.h>
#include <windows.h>
#include <time.h>

/* Window class name */
#define WND_CLASS_NAME "My Window Class"

LRESULT CALLBACK MyWinFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam );

/* The main program function */

VOID Draw(HDC hDC, HWND hWnd, INT X, INT Y, INT s, INT l, DOUBLE a, INT r, INT g, INT b, INT n, INT c, INT d) 
{ 
  INT i; 
  DOUBLE si, co;
  SYSTEMTIME t;
  DOUBLE pi = 3.14159265358979323846;
  POINT p[] = 
  { 
    { 0, -s },{ -s, 0 },{ 0, l } 
  }; 
  POINT p1[] = 
  { 
    { 0, -s },{ s, 0 },{ 0, l } 
  }; 
  POINT pts1[sizeof(p) / sizeof(p[0])]; 
  POINT pts2[sizeof(p1) / sizeof(p1[0])]; 

  si = sin(a);
  co = cos(a); 

  for (i = 0; i < sizeof(p) / sizeof(p[0]); i++) 
  { 
    pts1[i].x = X + p[i].x * co + p[i].y * si; 
    pts1[i].y = Y - (-p[i].x * si + p[i].y * co); 
    pts2[i].x = X + p1[i].x * co + p1[i].y * si; 
    pts2[i].y = Y - (-p1[i].x * si + p1[i].y * co); 
  } 
  SelectObject(hDC, GetStockObject(DC_PEN)); 
  SelectObject(hDC, GetStockObject(DC_BRUSH)); 

  srand(clock() / 1000); 

  SetDCPenColor(hDC, RGB(r, g, b)); 
  SetDCBrushColor(hDC, RGB(r, g, b)); 
  Polygon(hDC, pts1, sizeof(p) / sizeof(p[0])); 

  SetDCPenColor(hDC, RGB(n, c, d)); 
  SetDCBrushColor(hDC, RGB(n, c, d)); 
  Polygon(hDC, pts2, sizeof(p1) / sizeof(p1[0])); 

  SetDCPenColor(hDC, RGB(255, 255, 255)); 
  SetDCBrushColor(hDC, RGB(255, 255, 255)); 
} /* End of 'Draw' function */ 

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
    hBmLogo = LoadImage(NULL, "s.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
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
    Draw(hMemDC, hWnd, w / 2, h / 2, 4, 250, a, 255, 0, 0, 255, 0, 0);
    
    a = (t.wMinute + t.wMilliseconds / 60000.0)  * 2 * p / 60;
    Draw(hMemDC, hWnd, w / 2, h / 2, 6, 200, a, 0, 0, 0, 90, 90, 90);
    
    a = (t.wHour + t.wMilliseconds / 3600000.0) / 12 * 2 * p / 60;
    Draw(hMemDC, hWnd, w / 2, h / 2, 8, 150, a, 90, 90, 90, 0, 0, 0);
    
    
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
