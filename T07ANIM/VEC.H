#include <math.h>
#include <windows.h>
#define PI 3.14159265358979323846
#define D2R(A) ((A) * PI / 180.0)
#define Degree2Radian(a) D2R(a)

/* базовый вещественный тип */
typedef double DBL;

/* тип для вектора в простанстве */
typedef struct tagVEC
{
  DBL X, Y, Z;
} VEC;

/* тип для матрицы - массив в структуре */
typedef struct tagMATR
{
  DBL A[4][4];
} MATR;

__inline VEC VecSet( DBL X, DBL Y, DBL Z )
{
  VEC r = {X, Y, Z};

  return r;
}

__inline VEC VecAddVec( VEC A, VEC B )
{
  return VecSet(A.X + B.X, A.Y + B.Y, A.Z + B.Z);
}
__inline VEC VecSubVec( VEC A, VEC B )
{
  return VecSet(A.X - B.X, A.Y - B.Y, A.Z - B.Z);
}

__inline VEC VecMulNum( VEC A, DBL N )
{
  return VecSet(A.X * N, A.Y * N, A.Z * N); 
}


__inline VEC VecDivNum( VEC A, DBL N )
{
  return VecSet(A.X / N, A.Y / N, A.Z / N);
}
__inline VEC VecNeg( VEC A )
{
  return VecSet(-A.X, -A.Y, -A.Z);
}
 
__inline VEC VecCrossVec( VEC A, VEC B )
{
  return VecSet(A.Y * B.Z - A.Z * B.Y, A.Z * B.X - A.X * B.Z, A.X * B.Y - A.Y * B.X);
}
__inline DBL VecLen2( VEC V )
{
  return (V.Z * V.Z + V.Y * V.Y + V.X * V.X);
}
__inline DBL VecLen( VEC V )
{
  return sqrt(V.Z * V.Z + V.Y * V.Y + V.X * V.X);
}

__inline VEC VecDotVec( VEC A, VEC B )
{
  return VecSet(A.X * B.X, A.Y * B.Y, A.Z * B.Z);
}

__inline DBL VecDotVec1( VEC A, VEC B )
{
  return (A.X * B.X + A.Y * B.Y + A.Z * B.Z);
}

__inline VEC VecNormalize( VEC V )
{
  DBL len = VecDotVec1(V, V);

  if (len != 1 && len != 0)
    len = sqrt(len), V.X /= len, V.Y /= len, V.Z /= len;
  return V;
} 

__inline VEC PointTransform( VEC V, MATR M )
{
  return VecSet(
    V.X * M.A[0][0] + V.Y * M.A[1][0] + V.Z * M.A[2][0] + M.A[3][0],
    V.X * M.A[0][1] + V.Y * M.A[1][1] + V.Z * M.A[2][1] + M.A[3][1],
    V.X * M.A[0][2] + V.Y * M.A[1][2] + V.Z * M.A[2][2] + M.A[3][2]);
}
__inline VEC VectorTransform( VEC V, MATR M )
{
  return VecSet(
    V.X * M.A[0][0] + V.Y * M.A[1][0] + V.Z * M.A[2][0],
    V.X * M.A[0][1] + V.Y * M.A[1][1] + V.Z * M.A[2][1],
    V.X * M.A[0][2] + V.Y * M.A[1][2] + V.Z * M.A[2][2]);
}

__inline VEC PointTransform4( VEC V, MATR M )
{
  DBL w = V.X * M.A[0][3] + V.Y * M.A[1][3] + V.Z * M.A[2][3] + M.A[3][3];

  return VecSet(
    (V.X * M.A[0][0] + V.Y * M.A[1][0] + V.Z * M.A[2][0] + M.A[3][0]) / w,
    (V.X * M.A[0][1] + V.Y * M.A[1][1] + V.Z * M.A[2][1] + M.A[3][1]) / w,
    (V.X * M.A[0][2] + V.Y * M.A[1][2] + V.Z * M.A[2][2] + M.A[3][2]) / w);
}

__inline MATR MatrTranslate( VEC D )
{
  MATR M =
  {
    {
      {  1,   0,   0,  0},
      {  0,   1,   0,  0},
      {  0,   0,   1,  0},
      {D.X, D.Y, D.Z,  1}
    }
  };

  return M;
}


__inline MATR MatrScale( VEC S )
{
  MATR M =
  {
    {
      {  S.X,   0,   0,    0},
      {  0,   S.Y,   0,    0},
      {  0,     0,   S.Z,  0},
      {  0,     0,     0,  1}
    }
  };

  return M;  
}
__inline MATR MatrRotateX( DBL AngleDegree )
{
  MATR X = 
  {
    {
      {  1,   0,                                  0},
      {  0,   cos(AngleDegree),   -sin(AngleDegree)},
      {  0,   sin(AngleDegree),   sin(AngleDegree)}
    }
  };

  return X;

}

__inline MATR MatrRotateY( DBL AngleDegree )
{
   MATR X = 
  {
    {
      {  cos(AngleDegree),   0,    sin(AngleDegree)},
      {  0,                  1,                   0},
      {  -sin(AngleDegree),  0,    cos(AngleDegree)}
    }
  };

  return X;
}
__inline MATR MatrRotateZ( DBL AngleDegree )
{
  MATR X = 
  {
    {
      {  cos(AngleDegree),   -sin(AngleDegree),   0},
      {  sin(AngleDegree),    cos(AngleDegree),   0},
      {  0,                                  0,   1}
    }
  };
  return X;
}
__inline MATR MatrRotate( DBL AngleDegree, VEC R )
{
  DBL A = D2R(AngleDegree), si = sin(A), co = cos(A);
  VEC V = VecNormalize(R);
  MATR M =
  {
    {
      {co + V.X * V.X * (1 - co),        V.X * V.Y * (1 - co) - V.Z * si,  V.X * V.Z * (1 - co) + V.Y * si, 0},
      {V.Y * V.X * (1 - co) + V.Z * si,  co + V.Y * V.Y * (1 - co),        V.Y * V.Z * (1 - co) - V.X * si, 0},
      {V.Z * V.X * (1 - co) - V.Y * si,  V.Z * V.Y * (1 - co) + V.X * si,  co + V.Z * V.Z * (1 - co),       0},
      {0,                                0,                                0,                               1}
    }
  };

  return M;
}
__inline MATR MatrIdentity( VOID )
{
  MATR M =
  {
    {
      {1, 0, 0, 0},
      {0, 1, 0, 0},
      {0, 0, 1, 0},
      {0, 0, 0, 1}
    }
  };

  return M;
}

__inline MATR MatrMulMatr( MATR M1, MATR M2 )
{
  INT i, j, k;
  MATR r;

  for (i = 0; i < 4; i++)
    for (j = 0; j < 4; j++)
      for (k = 0, r.A[i][j] = 0; k < 4; k++)
        r.A[i][j] += M1.A[i][k] * M2.A[k][j];
  return r;
}

__inline MATR MatrTranspose( MATR M )
{
  INT i, j;
  MATR X;
  for (i = 0; i < 4; i++)
    for (j = 0; j < 4; j++)
      X.A[i][j] = M.A[j][i];
  return X;
}

__inline DBL MatrDeterm3x3( DBL A11, DBL A12, DBL A13,
                   DBL A21, DBL A22, DBL A23,
                   DBL A31, DBL A32, DBL A33 )
{
  return A11 * A22 * A33 + A12 * A23 * A31 + A13 * A21 * A32 -
         A11 * A23 * A32 - A12 * A21 * A33 - A13 * A22 * A31;
}

__inline DBL MatrDeterm( MATR M )
{
  return
    M.A[0][0] * MatrDeterm3x3(M.A[1][1], M.A[1][2], M.A[1][3],
                              M.A[2][1], M.A[2][2], M.A[2][3],
                              M.A[3][1], M.A[3][2], M.A[3][3]) -
    M.A[0][1] * MatrDeterm3x3(M.A[1][0], M.A[1][2], M.A[1][3],
                              M.A[2][0], M.A[2][2], M.A[2][3],
                              M.A[3][0], M.A[3][2], M.A[3][3]) +
    M.A[0][2] * MatrDeterm3x3(M.A[1][0], M.A[1][1], M.A[1][3],
                              M.A[2][0], M.A[2][1], M.A[2][3],
                              M.A[3][0], M.A[3][1], M.A[3][3]) -
    M.A[0][3] * MatrDeterm3x3(M.A[1][0], M.A[1][1], M.A[1][2],
                              M.A[2][0], M.A[2][1], M.A[2][2],
                              M.A[3][0], M.A[3][1], M.A[3][2]);
}

__inline MATR MatrInverse( MATR M )
{
  DBL det = MatrDeterm(M);
  MATR r;

  if (det == 0)
    return M;
  r.A[0][0] = MatrDeterm3x3(M.A[1][1], M.A[1][2], M.A[1][3],
                            M.A[2][1], M.A[2][2], M.A[2][3],
                            M.A[3][1], M.A[3][2], M.A[3][3]) / det;
  r.A[1][0] = -MatrDeterm3x3(M.A[1][0], M.A[1][2], M.A[1][3],
                             M.A[2][0], M.A[2][2], M.A[2][3],
                             M.A[3][0], M.A[3][2], M.A[3][3]) / det;
  r.A[2][0] = MatrDeterm3x3(M.A[1][0], M.A[1][1], M.A[1][3],
                            M.A[2][0], M.A[2][1], M.A[2][3],
                            M.A[3][0], M.A[3][1], M.A[3][3]) / det;
  r.A[3][0] = -MatrDeterm3x3(M.A[1][0], M.A[1][1], M.A[1][2],
                             M.A[2][0], M.A[2][1], M.A[2][2],
                             M.A[3][0], M.A[3][1], M.A[3][2]) / det;

  r.A[0][1] = -MatrDeterm3x3(M.A[0][1], M.A[0][2], M.A[0][3],
                             M.A[2][1], M.A[2][2], M.A[2][3],
                             M.A[3][1], M.A[3][2], M.A[3][3]) / det;
  r.A[1][1] = MatrDeterm3x3(M.A[0][0], M.A[0][2], M.A[0][3],
                            M.A[2][0], M.A[2][2], M.A[2][3],
                            M.A[3][0], M.A[3][2], M.A[3][3]) / det;
  r.A[2][1] = -MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][3],
                             M.A[2][0], M.A[2][1], M.A[2][3],
                             M.A[3][0], M.A[3][1], M.A[3][3]) / det;
  r.A[3][1] = MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][2],
                            M.A[2][0], M.A[2][1], M.A[2][2],
                            M.A[3][0], M.A[3][1], M.A[3][2]) / det;

  r.A[0][2] = MatrDeterm3x3(M.A[0][1], M.A[0][2], M.A[0][3],
                            M.A[1][1], M.A[1][2], M.A[1][3],
                            M.A[3][1], M.A[3][2], M.A[3][3]) / det;
  r.A[1][2] = -MatrDeterm3x3(M.A[0][0], M.A[0][2], M.A[0][3],
                             M.A[1][0], M.A[1][2], M.A[1][3],
                             M.A[3][0], M.A[3][2], M.A[3][3]) / det;
  r.A[2][2] = MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][3],
                            M.A[1][0], M.A[1][1], M.A[1][3],
                            M.A[3][0], M.A[3][1], M.A[3][3]) / det;
  r.A[3][2] = -MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][2],
                             M.A[1][0], M.A[1][1], M.A[1][2],
                             M.A[3][0], M.A[3][1], M.A[3][2]) / det;

  r.A[0][2] = MatrDeterm3x3(M.A[0][1], M.A[0][2], M.A[0][3],
                            M.A[1][1], M.A[1][2], M.A[1][3],
                            M.A[2][1], M.A[2][2], M.A[2][3]) / det;
  r.A[1][2] = -MatrDeterm3x3(M.A[0][0], M.A[0][2], M.A[0][3],
                             M.A[1][0], M.A[1][2], M.A[1][3],
                             M.A[2][0], M.A[2][2], M.A[2][3]) / det;
  r.A[2][2] = MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][3],
                            M.A[1][0], M.A[1][1], M.A[1][3],
                            M.A[2][0], M.A[2][1], M.A[2][3]) / det;
  r.A[3][2] = -MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][2],
                             M.A[1][0], M.A[1][1], M.A[1][2],
                             M.A[2][0], M.A[2][1], M.A[2][2]) / det;

}

_inline DBL Rnd0( VOID )
{
  return (DBL)rand() / RAND_MAX;
} /* End of 'Rnd0' function */

/* Random number obtain in range [-1.0 .. 1.0] function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (DBL) result random number.
 */
__inline DBL Rnd1( VOID )
{
  return 2.0 * rand() / RAND_MAX - 1;
} /* End of 'Rnd1' function */

MATR MatrFrustum(DBL l, DBL r, DBL b, DBL t, DBL n, DBL f)
{
  MATR F =
  {
    {
      {2 * n / (r - l),     0,               0,                   0},
      {0,                   2 * n / (t - b), 0,                   0},
      {(r + l) / (r - l), (t + b) / (t - b), (-t - n) / (t - n), -1},
      {0,                  0,               -2 * n * f / (f - n), 0}
    }
  };

  return F;
}