
#include "anim.h"


VOID VK3_RndSetProj( VOID )
{
  DBL ratio_x = 1, ratio_y = 1;

  if (VK3_Anim.w >= VK3_Anim.h)
    ratio_x = (DBL)VK3_Anim.w / VK3_Anim.h;
  else
    ratio_y = (DBL)VK3_Anim.h / VK3_Anim.w;

  VK3_RndMatrProj = MatrFrustum(-ratio_x * VK3_RndProjSize / 2,
                                 ratio_x * VK3_RndProjSize / 2,
                                -ratio_y * VK3_RndProjSize / 2,
                                 ratio_y * VK3_RndProjSize / 2,
                                 VK3_RndProjDist, VK3_RndFarClip);
} /* End of 'VK3_RndSetProj' function */

typedef struct
{
  VEC *P;
  INT NumOfP;
  INT(*Edges)[2];
  INT NumOfE;
}vk3PRIM;


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
    VEC p = PointTransformProj(Pr->P[i], M);

    pts[i].x = (p.X + 1) * VK3_Anim.w / 2;
    pts[i].y = (-p.Y + 1) * VK3_Anim.h / 2;
  }

  /* Draw all lines */
  for (i = 0; i < Pr->NumOfE; i++)
  {
    INT n0 = Pr->Edges[i][0], n1 = Pr->Edges[i][1];

    MoveToEx(VK3_Anim.hDC, pts[n0].x, pts[n0].y, NULL);
    LineTo(VK3_Anim.hDC, pts[n1].x, pts[n1].y);
  }
  free(pts);
} /* End of 'VK3_RndPrimDraw' function */


VEC CubeP[] = 
{
  { -1,  -1,   1},
  {  1,  -1,   1},
  {  1,  -1,  -1},
  { -1,  -1,  -1},
  { -1,   1,   1},
  {  1,   1,   1},
  {  1,   1,  -1},
  { -1,   1,  -1}
};

INT Cube[][2] = 
{
  {0, 1}, {1, 2}, {2, 3}, {3, 0},
  {4, 5}, {5, 6}, {6, 7}, {7, 4},
  {0, 4}, {1, 5}, {2, 6}, {3, 7}
};

MATR VK3_MatrView( VEC Loc, VEC At, VEC Up1 )
{
  VEC
    Dir = VecNormalize(VecSubVec(At, Loc));
    Right = VecNormalize(VecCrossVec(Dir, Up1);
    Up = VecNormalize(VecCrossVec(Right, Dir));
  MATR M = 
  {
    {
      {Right.X, Up.X, -Dir.X, 0},
      {Right.Y, Up.Y, -Dir.Y, 0},
      {Right.Z, UP.Z, -Dir.Z, 0},
      {VecDotvec(Loc, Right), -VecDotVec(Loc, Up), VecDotVec(Loc, Dir), 1}
    }
  };
  return M;
}

typedef struct
{
  VEC  P,  /* Vertex position */
  VEC2 T,  /* Vertex texture coordinates */
  VEC  N,  /* Normal at vertex */
  VEC4 C,  /* Vertex color */
} vk3VERTEX;

VOID VK3_RndPrimFree( vk3PRIM *Pr )
{
  if (Pr->V != NULL)
    free(Pr->V);
  if (Pr->I != NULL)
    free(Pr->I);
  memset(Pr, 0, sizeof(vk3PRIM));
} /* End of 'VK3_RndPrimFree' function */

BOOL VK3_RndPrimLoad( vk3PRIM *Pr, CHAR *FileName )
{
  FILE *F;
  DWORD Sign;
  INT NumOfPrimitives;
  CHAR MtlFile[300];
  INT NumOfP;
  INT NumOfI;
  CHAR Mtl[300];
  INT p;

  memset(Pr, 0, sizeof(vk3PRIM));

  F = fopen(FileName, "rb");
  if (F == NULL)
    return FALSE;

  fread(&Sign, 4, 1, F);
  if (Sign != *(DWORD *)"G3D")
  {
    fclose(F);
    return FALSE;
  }
  fread(&NumOfPrimitives, 4, 1, F);
  fread(MtlFile, 1, 300, F);
  for (p = 0; p < NumOfPrimitives; p++)
  {                                                       
    /* Read primitive info */
    fread(&NumOfP, 4, 1, F);
    fread(&NumOfI, 4, 1, F);
    fread(Mtl, 1, 300, F);

    /* Allocate memory for primitive */
    if ((Pr->V = malloc(sizeof(vk3VERTEX) * NumOfP)) == NULL)
    {
      fclose(F);
      return FALSE;
    }
    if ((Pr->I = malloc(sizeof(INT) * NumOfI)) == NULL)
    {
      free(Pr->V);
      Pr->V = NULL;
      fclose(F);
      return FALSE;
    }
    Pr->NumOfV = NumOfP;
    Pr->NumOfI = NumOfI;
    fread(Pr->V, sizeof(vk3VERTEX), NumOfP, F);
    fread(Pr->I, sizeof(INT), NumOfI, F);
    if (Pr->NumOfV > 0)
    {
      INT i;

      for (i = 0; i < Pr->NumOfV; i++)
        Pr->V[i].C = Vec4Set(Pr->V[i].N.X / 2 + 0.5,
                             Pr->V[i].N.Y / 2 + 0.5,
                             Pr->V[i].N.Z / 2 + 0.5, 1); /* Vec4Set(Rnd0(), Rnd0(), Rnd0(), 1); */
    }
    break;
  }
  fclose(F);
  return TRUE;
} /* End of 'VK3_RndPrimLoad' function */
