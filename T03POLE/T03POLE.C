#include <stdlib.h>
#include <math.h>

#include <windows.h>

/* Window class name */
#define WND_CLASS_NAME "My Window Class"

LRESULT CALLBACK MyWinFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam );

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

  while (GetMessage(&msg, NULL, 0, 0))
    DispatchMessage(&msg);

  return 30;
}


LRESULT CALLBACK MyWinFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
  POINT pt;
  POINT pts[], pts1[];
  pts[] = 
  {
    {0; -50}, {-110, 0}, {0, 50}
  },
  pts1[] = 
  {
    {0; -50}, {30, 0}, {0, 50}
  };
  POINT  pts2 = [sizeof(pts) / sizeof(pts[0])], pts3 = [sizeof(pts1) / sizeof(pts1[0]);
  DOUBLE si, co;
  HDC hDC;
  INT i;
  PAINTSTRUCT ps;
  static INT w, h;
  static BITMAP bm;
  static HBITMAP hBm, hBmLogo;
  static HDC hMemDC, hMemDCLogo;

  si = (y - pt.y) / sqrt((y - pt.y) * (y - pt.y) + (x - pt.x) * (x - pt.x));
  co = (x - pt.x) / sqrt((x - pt.x) * (x - pt.x) + (y - pt.y) * (y - pt.y));
  for (i = 0; i < sizeof(pts) / sizeof(pts[0]); i++)
  {
    pts2[i].x = x + pts[i].x * co - pts[i].y * si;
    pts2[i].y = y - (pts[i].x * si - pts[i].y * co);
    pts3[i].x = x + pts1[i].x * co - pts1[i].y * si;
    pts3[i].y = y - (pts1[i].x * si - pts1[i].y * si);
  }
    switch (Msg)
  {
  case WM_CREATE:                                                                                                                            
    SetTimer(hWnd, 30, 10, NULL);
    GetObject(hBmLogo, sizeof(bm), &bm);
    hDC = GetDC(hWnd);
    hMemDC = CreateCompatibleDC(hDC);
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
  case WM_TIMER:
    Rectangle(hMemDC, 0, 0, w + 1, h + 1);
    srand(59);
    SetBkMode(hMemDC, TRANSPARENT);
    SetTextColor(hMemDC, RGB(255, 0, 0));
    TextOut(hMemDC, 30, 30, "30!", 3);
    InvalidateRect(hWnd, NULL, FALSE);
    return 0;
  case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);
    DrawEye(hWnd ,100, 100, 30 , 30);
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
