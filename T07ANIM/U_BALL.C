#include <time.h>

#include "anim.h"

typedef struct
{
  vk3UNIT;
  VEC Pos;
  VEC Shift;
  DBL TimerShift;
  DBL TimerSpeed;
  COLORREF Color;
} vk3UNIT_BALL;

static VOID VK3_UnitInit( vk3UNIT_BALL *Uni, vk3ANIM *Ani )
{
  Uni->Pos = VecSet(Rnd0() * 1024, Rnd0() * 768, 0);
  Uni->TimerShift = Rnd1() * 59;
  Uni->TimerSpeed = Rnd1() * 8;
  Uni->Color = RGB(Rnd0() * 255, Rnd0() *255, Rnd0() *255);
}

static VOID VK3_UnitResponse( vk3UNIT_BALL *Uni, vk3ANIM *Ani )
{
  DBL t = Uni->TimerSpeed * clock() / 1000.0 + Uni->TimerShift;
  Uni->Shift = VecSet(30 * sin(t), 30 * cos(t), 0);
}

static VOID VK3_UnitRender( vk3UNIT_BALL *Uni, vk3ANIM *Ani )
{
  VEC p = VecAddVec(Uni->Pos, Uni->Shift);
  SetDCBrushColor(Ani->hDC, RGB(255, 0, 0));
  Ellipse(Ani->hDC, p.X - 8, p.Y - 8, p.X + 8, p.Y + 8);
}

vk3UNIT * VK3_UnitCreateBall( VOID )
{
  vk3UNIT_BALL *Uni;

  if ((Uni = (vk3UNIT_BALL *)VK3_AnimUnitCreate(sizeof(vk3UNIT_BALL))) == NULL)
    return NULL;
  Uni->Init = (VOID *)VK3_UnitInit;
  Uni->Response = (VOID *)VK3_UnitResponse;
  Uni->Render = (VOID *)VK3_UnitRender;
  return (vk3UNIT *)Uni;
}