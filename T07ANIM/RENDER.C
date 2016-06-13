#include "units.h"
#include "def.h"
#include "anim.h"
#include <windows.h>

VOID VK3_RndSetProj( VOID )
{
  DBL ratio.x = 1, ratio.y = 1;
}

typedef struct
{
  VEC *P;
  INT NumOfP;
  INT(*Edges)[2];
  INT NumOfE;
}vk3PRIM;

VOID VK3_PrimDraw(vk3PRIM *Pr)
{
  INT i;
  MATR M;
  POINT *pts;
  M = MatrMulMatr(VK3_RndMatrWorld, MatrMulMatr(VK3_RndMatrView, VK3_rndMatrProj));
  pts = malloc(sizeof(POINT) *Pr->NumOfP);
  if (pts == NULL)
    return;
  for (i = 0; i < Pr->NumOfP; i++)
  {
    VEC p = PointTransformProj(Pr->P[i], M);
    pts[i].x = (pt.x + 1) * VK3_Anim.W / 2;
    pts[i].y = (pt.y + 1) * VK3_Anim.H / 2;
  }
  for (i = 0; i < Pr->NumOfE; i++)
  {
    INT n0 = Pr->E[i], n1 = Pr->E[i][1];
    MoveToEx(VK3_Anim.hDC, pts[n1].x, pts[n1].y, NULL);
    LineTo(VK3_Amin.hDC, pts[n1].x, pts[n1].y);
  }
  free(pts);
}
