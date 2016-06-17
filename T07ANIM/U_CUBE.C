#include <time.h>
#include "render.h"
#include "units.h"

/* Ball unit representation type */
typedef struct
{
  vk3UNIT;         /* Base unit fields */
  VEC Pos;         /* Cube position */
  DBL TimerShift;  /* Timer shift phase value*/
  vk3PRIM Pr;
  DBL TimerSpeed;  /* Timer speed value*/
} vk3UNIT_CUBE;


/* Cube points */
VEC CubeP[] =
{
  {-1, -1,  1},
  { 1, -1,  1},
  { 1, -1, -1},
  {-1, -1, -1},
  {-1,  1,  1},
  { 1,  1,  1},
  { 1,  1, -1},
  {-1,  1, -1}
};

/* Cube edges */
INT CubeE[][2] =
{
  {0, 1}, {1, 2}, {2, 3}, {3, 0},
  {4, 5}, {5, 6}, {6, 7}, {7, 4},
  {0, 4}, {1, 5}, {2, 6}, {3, 7}
};

/* Cube primitive */
vk3PRIM Cube =
{
  CubeP, sizeof(CubeP) / sizeof(CubeP[0]),
  CubeE, sizeof(CubeE) / sizeof(CubeE[0])
};

/* Unit cube initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       vk3UNIT_CUBE *Uni;
 *   - animation context:
 *       vk3ANIM *Ani;
 * RETURNS: None.
 */
static VOID VK3_UnitInit( vk3UNIT_CUBE *Uni, vk3Anim *Ani )
{
  Uni->Pos = VecSet(Rnd0() * 5, Rnd0() * 5, Rnd0() * 5);
  Uni->TimerShift = Rnd1() * 59;
  Uni->TimerSpeed = Rnd1() * 8;
} /* End of ‘VK3_UnitInit' function */

/* Unit cube deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       vk3UNIT_CUBE *Uni;
 *   - animation context:
 *       vk3ANIM *Ani;
 * RETURNS: None.
 */
static VOID VK3_UnitClose( vk3UNIT_CUBE *Uni, vk3Anim *Ani )
{
   VK3_RndPrimFree(&Uni->Pr);
} /* End of ‘VK3_UnitClose' function */

/* Unit cube inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       vk3UNIT_CUBE *Uni;
 *   - animation context:
 *       vk3ANIM *Ani;
 * RETURNS: None.
 */
static VOID VK3_UnitResponse( vk3UNIT_CUBE *Uni, vk3Anim *Ani )
{
  if (Ani->KeysClick[VK_SPACE])
    VK3_AnimAddUnit(VK3_UnitCreateBall());
  if (Ani->KeysClick[VK_F1])
    VK3_AnimAddUnit(VK3_UnitCreateCube());
  if (Ani->KeysClick[VK_F2])
    VK3_AnimFlipFullScreen();
  if (Ani->KeysClick[VK_ESCAPE])
    VK3_AnimDoExit();
} /* End of ‘VK3_UnitResponse' function */

/* Unit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       VK3UNIT_CUBE *Uni;
 *   - animation context:
 *       VK3ANIM *Ani;
 * RETURNS: None.
 */
static VOID VK3_UnitRender( vk3UNIT_CUBE *Uni, vk3Anim *Ani )
{
  VK3_RndMatrWorld = MatrMulMatr(MatrScale(VecSet(0.1F, 0.1F, 0.1F)),
                     MatrMulMatr(MatrRotateY((Uni->TimerSpeed * Ani->Time) * 30 + Uni->TimerShift),
                                 MatrTranslate(VecAddVec(Uni->Pos,
                                                         VecMulNum(VecSet(Ani->JX, Ani->JY, Ani->JZ), 3)))));
  VK3_RndPrimDraw(&Uni->Pr);
} /* End of ‘VK3_UnitRender' function */

/* Unit cube creation function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (VK3UNIT *) pointer to created unit.
 */
vk3UNIT * VK3_UnitCreateCube( VOID )
{
  vk3UNIT_CUBE *Uni;

  if ((Uni = (vk3UNIT_CUBE *)VK3_AnimUnitCreate(sizeof(vk3UNIT_CUBE))) == NULL)
    return NULL;
  /* Setup unit methods */
  Uni->Init = (VOID *)VK3_UnitInit;
  Uni->Close = (VOID *)VK3_UnitClose;
  Uni->Response = (VOID *)VK3_UnitResponse;
  Uni->Render = (VOID *)VK3_UnitRender;
  return (vk3UNIT *)Uni;
} /* End of ‘VK3_UnitCreateCube' function */

/* END OF 'U_CUBE.C' FILE */
 




