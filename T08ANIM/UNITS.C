#include "anim.h"
#include <windows.h>

static VOID VK3_UnitInit( vk3UNIT *Uni, vk3ANIM *Ani )
{
} /* End of 'VK3_UnitInit' function */

/* Unit deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       vk3UNIT *Uni;
 *   - animation context:
 *       vk3ANIM *Ani;
 * RETURNS: None.
 */

static VOID VK3_UnitClose( vk3UNIT *Uni, vk3ANIM *Ani )
{
} /* End of 'VK3_UnitClose' function */


/* Unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       vk3UNIT *Uni;
 *   - animation context:
 *       vk3ANIM *Ani;
 * RETURNS: None.
 */

static VOID VK3_UnitResponse( vk3UNIT *Uni, vk3ANIM *Ani )
{
} /* End of 'vk3_UnitResponse' function */ 

/* Unit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       vk3UNIT *Uni;
 *   - animation context:
 *       vk3ANIM *Ani;
 * RETURNS: None.
 */

static VOID VK3_UnitRender( vk3UNIT *Uni, vk3ANIM *Ani )
{
} /* End of 'VK3_UnitRender' function */


/* Unit creation function.
 * ARGUMENTS:
 *   - unit structure size in bytes:
 *       INT Size;
 * RETURNS:
 *   (vk3UNIT *) pointer to created unit.
 */
vk3UNIT * VK3_AnimUnitCreate( INT Size )
{
  vk3UNIT *Uni;

  /* Memory allocation */
  if (Size < sizeof(vk3UNIT) ||
      (Uni = malloc(Size)) == NULL)
    return NULL;
  memset(Uni, 0, Size);
  /* Setup unit methods */
  Uni->Init = VK3_UnitInit;
  Uni->Close = VK3_UnitClose;
  Uni->Response = VK3_UnitResponse;
  Uni->Render = VK3_UnitRender;
  return Uni;
} /* End of 'VK3_AnimUnitCreate' function */   
                                               


