#include <windows.h>
#include <math.h>
#include <stdio.h>
#include "sphere.h"
#include <time.h>
#define n 40
#define m 80
#define pi 3.141592653589

typedef DOUBLE DBL;

typedef struct
{
  DBL x, y, z;
}VEC;

VEC Rot( VEC V, DBL Angle )
{
  DBL A = Angle * pi / 60;
  VEC r;
  r.x = V.x;
  r.y = V.y * cos(A) - V.z * sin(A);
  r.z = V.y * sin(A) + V.z * cos(A);
  return r;
}

VOID DrawSphere( HDC hDC, INT xc, INT yc )
{
  INT i, j, r = 500;
  DBL theta, phi, x, y, phase = clock() / 5000.0;
  static VEC G[n][m];
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
  SelectObject(hDC, GetStockObject(DC_PEN));
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
  SetDCPenColor(hDC, RGB(0, 0, 0));
  SetDCBrushColor(hDC, RGB(0, 0, 0));
}