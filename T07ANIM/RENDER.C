#include "anim.h"

/* Global transformation matrices */
MATR
  VK3_RndMatrWorld, /* World (object) transformation matrix */
  VK3_RndMatrView,  /* Camera view transform */
  VK3_RndMatrProj;  /* Projection transform */

/* Projection parameters */
DBL
  VK3_RndProjDist, /* Near clip plane */
  VK3_RndFarClip,  /* Far clip plane */
  VK3_RndProjSize; /* Project plane size */

/* Setup projection function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID VK3_RndSetProj( VOID )
{
  DBL ratio_x = 1, ratio_y = 1;

  if (VK3_Anim.W >= VK3_Anim.H)
    ratio_x = (DBL)VK3_Anim.W / VK3_Anim.H;
  else
    ratio_y = (DBL)VK3_Anim.H / VK3_Anim.W;

  VK3_RndMatrProj = MatrFrustum(-ratio_x * VK3_RndProjSize / 2,
                                 ratio_x * VK3_RndProjSize / 2,
                                -ratio_y * VK3_RndProjSize / 2,
                                 ratio_y * VK3_RndProjSize / 2,
                                 VK3_RndProjDist, VK3_RndFarClip);
} /* End of 'RA3_RndSetProj' function */

VOID VK3_RndPrimDraw( vk3PRIM *Pr )
{
  INT i;
  MATR M;
  POINT *pts;

  /* Build transform matrix */
  M = MatrMulMatr(VK3_RndMatrWorld,
    MatrMulMatr(VK3_RndMatrView, VK3_RndMatrProj));

  /* Transform all points */
  pts = malloc(sizeof(POINT) * Pr->NumOfP);
  if (pts == NULL)
    return;
  for (i = 0; i < Pr->NumOfP; i++)
  {
    /* NDC */
    VEC p = VecMulMatr(Pr->P[i], M);
    pts[i].x = (p.X + 1) * VK3_Anim.W / 2;
    pts[i].y = (-p.Y + 1) * VK3_Anim.H / 2;
  }

  /* Draw all lines */
  for (i = 0; i < Pr->NumOfE; i++)
  {
    INT n0 = Pr->Edges[i][0], n1 = Pr->Edges[i][1];

    MoveToEx(VK3_Anim.hDC, pts[n0].x, pts[n0].y, NULL);
    LineTo(VK3_Anim.hDC, pts[n1].x, pts[n1].y);
  }
  free(pts);
}
/* END OF 'RENDER.C' FILE */
