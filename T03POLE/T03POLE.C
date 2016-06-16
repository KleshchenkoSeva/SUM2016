#include <stdlib.h> 
#include <math.h> 

#include <windows.h> 

/* Window class name */ 
#define WND_CLASS_NAME "My Window Class" 

/* Forward references */ 
LRESULT CALLBACK MyWinFunc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam); 

VOID Draw(HDC hDC, HWND hWnd, INT X, INT Y) 
{ 
INT i; 
DOUBLE si, co; 
POINT pt; 
static POINT pts[] = 
{ 
{ 0, -20 },{ 40, 0 },{ 0, 20 } 
}; 
static POINT pts1[] = 
{ 
{ 0, 20 },{ -40, 0 },{ 0, -20 } 
}; 
POINT pt1[sizeof(pts) / sizeof(pts[0])]; 
POINT pt2[sizeof(pts1) / sizeof(pts1[0])]; 

GetCursorPos(&pt); 
ScreenToClient(hWnd, &pt); 

si = (Y - pt.y) / sqrt((pt.x - X) * (pt.x - X) + (Y - pt.y) * (Y - pt.y)); 
co = (pt.x - X) / sqrt((pt.x - X) * (pt.x - X) + (Y - pt.y) * (Y - pt.y)); 

for (i = 0; i < sizeof(pts) / sizeof(pts[0]); i++) 
{ 
pt1[i].x = X + pts[i].x * co - pts[i].y * si; 
pt1[i].y = Y - (pts[i].x * si + pts[i].y * co); 
pt2[i].x = X + pts1[i].x * co - pts1[i].y * si; 
pt2[i].y = Y - (pts1[i].x * si + pts1[i].y * co); 
} 
SelectObject(hDC, GetStockObject(DC_PEN)); 
SelectObject(hDC, GetStockObject(DC_BRUSH)); 

srand(clock() / 1000); 

SetDCPenColor(hDC, RGB(rand() % 256, rand() % 256, rand() % 256)); 
SetDCBrushColor(hDC, RGB(rand() % 256, rand() % 256, rand() % 256)); 
Polygon(hDC, pt1, sizeof(pts) / sizeof(pts[0])); 

SetDCPenColor(hDC, RGB(rand() % 256, rand() % 256, rand() % 256)); 
SetDCBrushColor(hDC, RGB(rand() % 256, rand() % 256, rand() % 256)); 
Polygon(hDC, pt2, sizeof(pts1) / sizeof(pts1[0])); 

SetDCPenColor(hDC, RGB(0, 0, 255)); 
SetDCBrushColor(hDC, RGB(0, 0, 255)); 
} /* End of 'Draw' function */ 

VOID FlipFullScreen(HWND hWnd) 
{ 
static BOOL IsFullScreen = FALSE; 
static RECT SaveRect; 

if (IsFullScreen) 
{ 
/* restore window size */ 
SetWindowPos(hWnd, HWND_TOP, 
SaveRect.left, SaveRect.top, 
SaveRect.right - SaveRect.left, SaveRect.bottom - SaveRect.top, 
SWP_NOOWNERZORDER); 
} 
else 
{ 
/* Set full screen size to window */ 
HMONITOR hmon; 
MONITORINFOEX moninfo; 
RECT rc; 

GetWindowRect(hWnd, &SaveRect); 

hmon = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST); 

moninfo.cbSize = sizeof(moninfo); 
GetMonitorInfo(hmon, (MONITORINFO *)&moninfo); 
 
rc = moninfo.rcMonitor; 
AdjustWindowRect(&rc, GetWindowLong(hWnd, GWL_STYLE), FALSE); 

SetWindowPos(hWnd, HWND_TOPMOST, 
rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, 
SWP_NOOWNERZORDER); 
} 
IsFullScreen = !IsFullScreen; 
} 
/* The main program function */ 
INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, 
CHAR *CmdLine, INT CmdShow) 
{ 
WNDCLASS wc; 
HWND hWnd; 
MSG msg; 

/* Register window class */ 
wc.style = CS_HREDRAW | CS_VREDRAW; 
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

/* Show window */ 
ShowWindow(hWnd, CmdShow); 
UpdateWindow(hWnd); 

/* Run message loop */ 
while (GetMessage(&msg, NULL, 0, 0)) 
DispatchMessage(&msg); 

return 30; 
} /* End of 'WinMain' function */ 

/* Window message handle function */ 
LRESULT CALLBACK MyWinFunc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) 
{ 
SYSTEMTIME st; 
INT i, j; 
HDC hDC; 
PAINTSTRUCT ps; 
static INT w, h; 
static BITMAP bm; 
static HBITMAP hBm, hBmLogo; 
static HDC hMemDC, hMemDCLogo; 

switch (Msg) 
{ 
case WM_CREATE: 
SetTimer(hWnd, 30, 10, NULL); 
hBmLogo = LoadImage(NULL, "GLOBE.BMP", IMAGE_BITMAP, 0, 0, 
LR_LOADFROMFILE); 
GetObject(hBmLogo, sizeof(bm), &bm); 
hDC = GetDC(hWnd); 
hMemDC = CreateCompatibleDC(hDC); 
hMemDCLogo = CreateCompatibleDC(hDC); 
SelectObject(hMemDCLogo, hBmLogo); 
ReleaseDC(hWnd, hDC); 
return 0; 
case WM_KEYDOWN: 
if (LOWORD(wParam) == 'F') 
FlipFullScreen(hWnd); 
if (LOWORD(wParam) == VK_ESCAPE) 
SendMessage(hWnd, WM_DESTROY, 0, 0); 
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
BitBlt(hMemDC, 0, 0, bm.bmWidth, bm.bmHeight, 
hMemDCLogo, 0, 0, SRCCOPY); 
srand(59); 
Draw(hMemDC, hWnd, 300, 500);
SetBkMode(hMemDC, TRANSPARENT); 
SetTextColor(hMemDC, RGB(255, 0, 0)); 
TextOut(hMemDC, 30, 30, "Pole", 4); 
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
} /* End of 'MyWinFunc' function */ 


/* END OF 'T03POLE.C' FILE */
