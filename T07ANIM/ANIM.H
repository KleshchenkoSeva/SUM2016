#ifndef __ANIM_H_
#define __ANIM_H_

#include "def.h"

#define VK3_MAX_UNITS 6532

typedef struct tagvk3UNIT vk3UNIT;
typedef struct
{
  HWND hWnd;
  HDC hDC;
  INT w, h;
  HBITMAP hFrame;
  vk3UNIT *Units[VK3_MAX_UNITS];
  INT NumOfUnits;
  /* Timer Data */
  DBL
    GlobalTime, GlobalDeltaTime, Time, DeltaTime, FPS;
  BOOL IsPause;
  INT
    mx, my, mz, mdx, mdy, mdz;
  BYTE
    Keys[256], OldKeys[256], KeysClick[256];
  BYTE
    JBut[32];
  INT JPov;
  DBL
    jx, jy, jz, jr;
} vk3ANIM;

struct tagvk3UNIT
{
  VOID (*Init)( vk3UNIT *Uni, vk3ANIM *Ani );

  /* Unit deinitialization function.
   * ARGUMENTS:
   *   - self-pointer to unit object:
   *       vk3UNIT *Uni;
   *   - animation context:
   *       vk3ANIM *Ani;
   * RETURNS: None.
   */
  VOID (*Close)( vk3UNIT *Uni, vk3ANIM *Ani );

  /* Unit inter frame events handle function.
   * ARGUMENTS:
   *   - self-pointer to unit object:
   *       vk3UNIT *Uni;
   *   - animation context:
   *       vk3ANIM *Ani;
   * RETURNS: None.
   */
  VOID (*Response)( vk3UNIT *Uni, vk3ANIM *Ani );

  /* Unit render function.
   * ARGUMENTS:
   *   - self-pointer to unit object:
   *       vg4UNIT *Uni;
   *   - animation context:
   *       vg4ANIM *Ani;
   * RETURNS: None.
   */
  VOID (*Render)( vk3UNIT *Uni, vk3ANIM *Ani );
};

/* Unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       vk3UNIT *Uni;
 *   - animation context:
 *       vk3ANIM *Ani;
 * RETURNS: None.
 */
extern INT VK3_MouseWheel;
extern vk3ANIM VK3_Anim;

VOID VK3_AnimInit(HWND hWnd);
VOID VK3_AnimClose( VOID );
VOID VK3_AnimResize( INT w, INT h);
VOID VK3_AnimCopyFrame( HDC hDC );
VOID VK3_AnimRender( VOID );
VOID VK3_AnimAddUnit( vk3UNIT *Uni );
VOID VK3_AnimDoExit( VOID );
VOID VK3_AnimFlipFullScreen( VOID );

/* Unit creation function.
 * ARGUMENTS:
 *   - unit structure size in bytes:
 *       INT Size;
 * RETURNS:
 *   (vk34UNIT *) pointer to created unit.
 */
vk3UNIT * VK3_AnimUnitCreate( INT Size );

#endif /*__ANIM_H_*/
