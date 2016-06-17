#include <windows.h>
#include <math.h>
#include <stdio.h>
#include "sphere.h"
#include <time.h>
#define n 40
#define m 80
#define pi 3.141592653589
#pragma warning(disable: 4244)

typedef DOUBLE DBL;

typedef struct
{
  DBL x, y, z;
}VEC;
typedef struct
{
  HBITMAP hBmGlobe;
  DWORD *Bits;
  INT W, H;
} IMG;
VEC Rot( VEC V, DBL Angle )
{
  DBL A = Angle * pi / 60;
  VEC r;
  r.x = V.x;
  r.y = V.y * cos(A) - V.z * sin(A);
  r.z = V.y * sin(A) + V.z * cos(A);
  return r;
}

IMG Globe;

/* Load sphere texture function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID LoadSphere( VOID )
{
  HDC hDC = GetDC(NULL), hMemDC, hMemDC1;
  HBITMAP hBm;
  BITMAP bm;
  BITMAPINFOHEADER bmih;

  /* Load image from file */
  hMemDC = CreateCompatibleDC(hDC);
  hBm = LoadImage(NULL, "E.BMP", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
  GetObject(hBm, sizeof(bm), &bm);
  Globe.W = bm.bmWidth;
  Globe.H = bm.bmHeight;
  SelectObject(hMemDC, hBm);

  /* Fill DIB info */
  memset(&bmih, 0, sizeof(bmih));
  bmih.biSize = sizeof(BITMAPINFOHEADER);
  bmih.biBitCount = 32;                   /* BGRx - DWORD */
  bmih.biPlanes = 1;                      /* always */
  bmih.biCompression = BI_RGB;            /* 0 - no compression */
  bmih.biWidth = bm.bmWidth;              /* width */
  bmih.biHeight = -bm.bmHeight;           /* height + first coded row is 0 */
  bmih.biSizeImage = bm.bmWidth * bm.bmHeight * 4;  /* image size in bytes */

  Globe.hBmGlobe = CreateDIBSection(NULL, (BITMAPINFO *)&bmih, DIB_RGB_COLORS,
                 (VOID **)&Globe.Bits, NULL, 0);
  hMemDC1 = CreateCompatibleDC(hDC);
  SelectObject(hMemDC1, Globe.hBmGlobe);

  BitBlt(hMemDC1, 0, 0, Globe.W, Globe.H, hMemDC, 0, 0, SRCCOPY);

  DeleteDC(hMemDC);
  DeleteDC(hMemDC1);
  DeleteObject(hBm);

  ReleaseDC(NULL, hDC);
} /* End of 'LoadSphere' function */
VOID DrawQuad( HDC hDC, POINT P0, POINT P1, POINT P2, POINT P3, DWORD Color )
{
  INT s =
    (P0.x - P1.x) * (P0.y + P1.y) +
    (P1.x - P2.x) * (P1.y + P2.y) +
    (P2.x - P3.x) * (P2.y + P3.y) +
    (P3.x - P0.x) * (P3.y + P0.y);

  if (s > 0)
  {
    POINT pts[4];
    
    pts[0] = P0;
    pts[1] = P1;
    pts[2] = P2;
    pts[3] = P3;

    Polygon(hDC, pts, 4);
  }
} /* End of 'DrawQuad' function */

VOID DrawSphere( HDC hDC, INT xc, INT yc )
{
  INT i, j, r = 500;
  DBL theta, phi, x, y, phase = clock() / 5000.0;
  static VEC G[m][n];
  POINT p0, p1, p2, p3;
  static POINT pts[M][N];
  for (i = 0; i < n; i++)
  {
    theta = i * pi / (n - 1);
    
    for (j = 0; j < m; j++)
    {
      phi = j * 2 * pi / (m - 1) + phase;
      G[i][j].x = r * sin(theta) * cos(phi);
      G[i][j].y = r * sin(theta) * sin(phi);
      G[i][j].z = r * cos(theta);
      G[i][j] = Rot(G[i][j], 30 * sin(phase));
    }
  }
  /* SelectObject(hDC, GetStockObject(DC_PEN));
  SelectObject(hDC, GetStockObject(DC_BRUSH));
  SetDCPenColor(hDC, RGB(0, 255, 0));
  SetDCBrushColor(hDC, RGB(0, 255, 0));
  
  for (i = 0; i < n; i++)
  {
    for (j = 0; j < m; j++)
    {
      x = xc + G[i][j].x;
      y = yc - G[i][j].z;
      Ellipse(hDC, x - 2, y - 2, x + 2, y + 2);
    }
  }
  
  for (i = 0; i < n; i++)
  {
    x = xc + G[i][0].x;
    y = yc - G[i][0].z;
    MoveToEx(hDC, x, y, NULL);
    for (j = 1; j < m; j++)
    {
      x = xc + G[i][j].x;
      y = yc - G[i][j].z;
      LineTo(hDC, x, y);
    }
  }
  
  for (j = 0; j < m; j++)
  {
    x = xc + G[0][j].x;
    y = yc - G[0][j].z;
    MoveToEx(hDC, x, y, NULL);
    for (i = 1; i < n; i++)
    {
      x = xc + G[i][j].x;
      y = yc - G[i][j].z;
      LineTo(hDC, x, y);
    }
  }
  */
  for (i = 0; i < M; i++)
    for (j = 0; j < N; j++)
    {
      pts[i][j].x = Xc + G[i][j].X;
      pts[i][j].y = Yc - G[i][j].Z;
    }

  LoadSphere();

  for (i = 0; i < M - 1; i++)
  {
    for (j = 0; j < N - 1; j++)
    {
      INT img_x, img_y;
      COLORREF c;
      BYTE r, g, b;

      img_x = j * (Globe.W - 1) / (N - 1);
      img_y = i * (Globe.H - 1) / (M - 1);

      c = Globe.Bits[img_x + img_y * Globe.W]; 
      r = GetRValue(c);
      g = GetGValue(c);
      b = GetBValue(c);
      c = RGB(b, g, r);
    
      SelectObject(hDC, GetStockObject(DC_PEN));
      SelectObject(hDC, GetStockObject(DC_BRUSH));

      SetDCPenColor(hDC, c);                  
      SetDCBrushColor(hDC, c);                                                                
      p0 = pts[i][j];                                                                             
      p3 = pts[i + 1][j];
      p1 = pts[i][j + 1];
      p2 = pts[i + 1][j + 1];
      if (G[i][j].Y < 0)
        DrawQuad(hDC, p0, p1, p2, p3, 0); 
    }
  }

  SetDCPenColor(hDC, RGB(255, 255, 255));
  SetDCBrushColor(hDC, RGB(255, 255, 255));
}

