#include <windows.h>
#include "vec.h"
#include <math.h>
#include <stdlib.h>

INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstanse, CHAR *CmdLine, INT CmdShow )
{
  MATR m;
  m = VecSet(X, Y, Z );
  m = VecAddVec( A, B );
  m = VecSubVec( A, B );
  m = VecMulNum( A, N );
  m = VecDivNum( A, N );
  m = VecDivNum( A, N );
  m = VecNeg( A );
  m = VecDotVec( A, B );
  m = VecCrossVec( A, B );
  m = VecLen2( V );
  m = VecLen( V );
  m = VecNormalize( V );
  m = PointTransform( V, M );
  m = VectorTransform( V, M );
  m = PointTransform4( V, M );
  m = MatrTranslate( D );
  m = MatrScale( S );
  m = MatrRotateX( AngleDegree );
  m = MatrRotateY( AngleDegree );
  m = MatrRotateZ( AngleDegree );
  m = MatrRotate( AngleDegree, R );
  m = MatrIdentity( VOID );
  m = MatrMulMatr( M1, M2 );
  m = MatrTranspose( MATR M );
  m = MatrDeterm3x3( DBL A11, DBL A12, DBL A13,
                    A21, A22, DBL A23,
                    DBL A31, DBL A32, DBL A33 );
  m = MatrDeterm( M );
  m = MatrInverse( M );
}

