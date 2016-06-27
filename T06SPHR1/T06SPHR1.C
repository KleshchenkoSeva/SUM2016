#include <windows.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#pragma warning(disable: 4244)

#define WND_CLASS_NAME "My window class"

#define PI 3.14159265358979323846
#define D2R(X) ((X) * PI / 180)

#define SIZE_Y 10
#define SIZE_X 15

typedef DOUBLE DBL;

typedef struct 
{
  DBL X, Y, Z;  
} VEC;

typedef struct
{
  DBL A[4][4];
} MATR;

MATR MatrRotateX( DBL A )
{
  DBL angle = D2R(A), si = sin(angle), co = cos(angle);
  MATR r =
  {
    {
      {co, si, 0, 0},
      {-si, co, 0, 0},
      {0, 0, 1, 0},
      {0, 0, 0, 1},
    }
  };

  return r;
}

MATR MatrRotateY( DBL A )
{
  DBL angle = D2R(A), si = sin(angle), co = cos(angle);
  MATR r =
  {
    {
      {1, 0, 0, 0},
      {0, co, si, 0},
      {0, -si, co, 0},
      {0, 0, 0, 1},
    }
  };

  return r;
}

MATR MatrRotateZ( DBL A )
{
  DBL angle = D2R(A), si = sin(angle), co = cos(angle);
  MATR r =
  {
    {
      {co, 0, -si, 0},
      {0, 1, 0, 0},
      {si, 0, co, 0},
      {0, 0, 0, 1}
    }
  };

  return r;
}

VEC VecMulMatr( VEC V, MATR M )
{
  VEC r =
  {
    V.X * M.A[0][0] + V.Y * M.A[1][0] + V.Z * M.A[2][0] + M.A[3][0],
    V.X * M.A[0][1] + V.Y * M.A[1][1] + V.Z * M.A[2][1] + M.A[3][1],
    V.X * M.A[0][2] + V.Y * M.A[1][2] + V.Z * M.A[2][2] + M.A[3][2]
  };

  return r;
}

MATR MatrMulMatr( MATR M1, MATR M2 )
{
  MATR r =
  {
    {
      {
        M1.A[0][0] * M2.A[0][0] + M1.A[0][1] * M2.A[1][0] + M1.A[0][2] * M2.A[2][0] + M1.A[0][3] * M2.A[3][0],
        M1.A[0][0] * M2.A[0][1] + M1.A[0][1] * M2.A[1][1] + M1.A[0][2] * M2.A[2][1] + M1.A[0][3] * M2.A[3][1],
        M1.A[0][0] * M2.A[0][2] + M1.A[0][1] * M2.A[1][2] + M1.A[0][2] * M2.A[2][2] + M1.A[0][3] * M2.A[3][2],
        M1.A[0][0] * M2.A[0][3] + M1.A[0][1] * M2.A[1][3] + M1.A[0][2] * M2.A[2][3] + M1.A[0][3] * M2.A[3][3]
      },
      {
        M1.A[1][0] * M2.A[0][0] + M1.A[1][1] * M2.A[1][0] + M1.A[1][2] * M2.A[2][0] + M1.A[1][3] * M2.A[3][0],
        M1.A[1][0] * M2.A[0][1] + M1.A[1][1] * M2.A[1][1] + M1.A[1][2] * M2.A[2][1] + M1.A[1][3] * M2.A[3][1],
        M1.A[1][0] * M2.A[0][2] + M1.A[1][1] * M2.A[1][2] + M1.A[1][2] * M2.A[2][2] + M1.A[1][3] * M2.A[3][2],
        M1.A[1][0] * M2.A[0][3] + M1.A[1][1] * M2.A[1][3] + M1.A[1][2] * M2.A[2][3] + M1.A[1][3] * M2.A[3][3]
      },
      {
        M1.A[2][0] * M2.A[0][0] + M1.A[2][1] * M2.A[1][0] + M1.A[2][2] * M2.A[2][0] + M1.A[2][3] * M2.A[3][0],
        M1.A[2][0] * M2.A[0][1] + M1.A[2][1] * M2.A[1][1] + M1.A[2][2] * M2.A[2][1] + M1.A[2][3] * M2.A[3][1],
        M1.A[2][0] * M2.A[0][2] + M1.A[2][1] * M2.A[1][2] + M1.A[2][2] * M2.A[2][2] + M1.A[2][3] * M2.A[3][2],
        M1.A[2][0] * M2.A[0][3] + M1.A[2][1] * M2.A[1][3] + M1.A[2][2] * M2.A[2][3] + M1.A[2][3] * M2.A[3][3]
      },
      {
        M1.A[3][0] * M2.A[0][0] + M1.A[3][1] * M2.A[1][0] + M1.A[3][2] * M2.A[2][0] + M1.A[3][3] * M2.A[3][0],
        M1.A[3][0] * M2.A[0][1] + M1.A[3][1] * M2.A[1][1] + M1.A[3][2] * M2.A[2][1] + M1.A[3][3] * M2.A[3][1],
        M1.A[3][0] * M2.A[0][2] + M1.A[3][1] * M2.A[1][2] + M1.A[3][2] * M2.A[2][2] + M1.A[3][3] * M2.A[3][2],
        M1.A[3][0] * M2.A[0][3] + M1.A[3][1] * M2.A[1][3] + M1.A[3][2] * M2.A[2][3] + M1.A[3][3] * M2.A[3][3]
      }
    }
  };

  return r;
}


LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam );

INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInctance, CHAR *CmdLine, INT ShowCmd )
{
  WNDCLASS wc;
  HWND hWnd;
  MSG msg;

  wc.style = CS_VREDRAW | CS_HREDRAW;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
  wc.hCursor = LoadCursor(NULL, IDC_ARROW);
  wc.hIcon = LoadIcon(NULL, IDI_ERROR);
  wc.lpszMenuName = NULL;
  wc.hInstance = hInstance;
  wc.lpfnWndProc = MyWindowFunc;
  wc.lpszClassName = WND_CLASS_NAME;

  if (!RegisterClass(&wc))
  {
    MessageBox(NULL, "Error register window class", "ERROR", MB_OK);
    return 0;
  }

  hWnd =
    CreateWindow(WND_CLASS_NAME,
    "30!",
    WS_OVERLAPPEDWINDOW,
    CW_USEDEFAULT, CW_USEDEFAULT,
    CW_USEDEFAULT, CW_USEDEFAULT,
    NULL,
    NULL,
    hInstance,
    NULL);
  
  ShowWindow(hWnd, SW_SHOWNORMAL);
  UpdateWindow(hWnd);

  while (GetMessage(&msg, NULL, 0, 0))
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
  return msg.wParam;
}

VOID DrawRect( HDC hDC, POINT P0, POINT P1, POINT P2, POINT P3 )
{
  if ((P0.x - P1.x) * (P0.y + P1.y) +
      (P1.x - P2.x) * (P1.y + P2.y) +
      (P2.x - P3.x) * (P2.y + P3.y) +
      (P3.x - P0.x) * (P3.y + P0.y) < 0)
  {
    POINT p[4];

    p[0] = P0;
    p[1] = P1;
    p[2] = P2;
    p[3] = P3;

    Polygon(hDC, p, 4);
  }
}

VOID DrawSphere( HWND hWnd, HDC hDC, INT Xc, INT Yc, INT R )
{
  INT i, j;
  DOUBLE x, y;
  MATR mt;
  static VEC G[SIZE_Y][SIZE_X];
  static POINT pts[SIZE_Y][SIZE_X];

  /* Evaluate sphere coordinates */
  mt = MatrMulMatr(MatrRotateY(30 * clock() / CLOCKS_PER_SEC),
                   MatrRotateZ(30 * clock() / CLOCKS_PER_SEC));
  for (i = 0; i < SIZE_Y; i++)
  {
    DBL theta = PI * i / (SIZE_Y - 1);

    for (j = 0; j < SIZE_X; j++)
    {
      DBL phi = 2 * PI * j / (SIZE_X - 1); 
      
      G[i][j].X = R * sin(theta) * cos(phi);
      G[i][j].Y = R * sin(theta) * sin(phi);
      G[i][j].Z = R * cos(theta);

      G[i][j] = VecMulMatr(G[i][j], mt);
    }
  }

  /* Obtain screen positions */
  for (i = 0; i < SIZE_Y; i++)
    for (j = 0; j < SIZE_X; j++)
    {
      pts[i][j].x = Xc + G[i][j].X;
      pts[i][j].y = Yc - G[i][j].Z;
    }  

#if 0
  /* Draw by points */
  for (i = 0; i < SIZE_Y; i++)
    for (j = 0; j < SIZE_X; j++)
      Ellipse(hDC, pts[i][j].x + 2, pts[i][j].y + 2, pts[i][j].x - 2, pts[i][j].y - 2);

  /* Draw by parallels */
  for (i = 0; i < SIZE_Y; i++)
    for (j = 0; j < SIZE_X - 1; j++)
    {
      MoveToEx(hDC, pts[i][j].x, pts[i][j].y, NULL);
      LineTo(hDC, pts[i][j + 1].x, pts[i][j + 1].y);
    }  
  /* Draw by meridians */
  for (j = 0; j < SIZE_X; j++)
    for (i = 0; i < SIZE_Y - 1; i++)
    {
      MoveToEx(hDC, pts[i][j].x, pts[i][j].y, NULL);
      LineTo(hDC, pts[i + 1][j].x, pts[i + 1][j].y);
    }  
#endif /* 0 */

  /* Draw by quads */
  for (j = 0; j < SIZE_X - 1; j++)
    for (i = 0; i < SIZE_Y - 1; i++)
    {
      if (i % 2  == 0 && j % 2 == 0 || i % 2 == 1 && j % 2 == 1)
      {
        SetDCBrushColor(hDC, RGB(255, 0, 0));
        DrawRect(hDC, pts[i][j], pts[i][j + 1], pts[i + 1][j + 1], pts[i + 1][j]);
      }
      else
      {
        SetDCBrushColor(hDC, RGB(0, 0, 255));
        DrawRect(hDC, pts[i][j], pts[i][j + 1], pts[i + 1][j + 1], pts[i + 1][j]);
      }
    }


  SetDCBrushColor(hDC, RGB(255, 255, 255));
}

LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
  HDC hDC;
  PAINTSTRUCT ps;
  static INT x, y, w, h;
  static HDC hMemDC;
  static HBITMAP hBm;
  
  switch (Msg)
  {                                                                                                                                        
  case WM_CREATE:
    hDC = GetDC(hWnd);
    hMemDC = CreateCompatibleDC(hDC);
    ReleaseDC(hWnd, hDC);
    SetTimer(hWnd, 30, 2, NULL);
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
    SelectObject(hMemDC, GetStockObject(DC_BRUSH));
    Rectangle(hMemDC, 0, 0, w + 1, h + 1);
    DrawSphere(hWnd, hMemDC, w / 2, h / 2, 300);
    InvalidateRect(hWnd, NULL, FALSE);
    return 0;
  case WM_MOUSEMOVE:
    x = (SHORT)LOWORD(lParam);
    y = (SHORT)HIWORD(lParam);
    return 0;
  case WM_PAINT:   
    hDC = BeginPaint(hWnd, &ps);
    BitBlt(hDC, 0, 0, w, h, hMemDC, 0, 0, SRCCOPY);
    EndPaint(hWnd, &ps);
    return 0;
  case WM_KEYDOWN:
    if (LOWORD(wParam) == VK_ESCAPE)
      SendMessage(hWnd, WM_CLOSE, 0, 0);
    return 0;
  case WM_ERASEBKGND:
    return 0;
  case WM_DESTROY:
    DeleteObject(hBm);
    DeleteDC(hMemDC);
    KillTimer(hWnd, 30);
    PostQuitMessage(0);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
}