#ifndef __IMAGE_H_
#define __IMAGE_H_

#include "def.h"

/* Image represetation type */
typedef struct
{
  HBITMAP hImage; /* Image bitmap handle */
  INT W, H;       /* Image size */
  DWORD *Bits;    /* Image pixel color data */
} vk3IMG;

/* Load image function.
 * ARGUMENTS:
 *   - image store data pointer:
 *       vk3IMG *Im;
 *   - image file name (*.BMP):
 *       CHAR *FileName;
 * RETURNS:
 *   (BOOL) TRUE if success, FALSE otherwise.
 */
BOOL VK3_ImageLoad( vk3IMG *Im, CHAR *FileName );

/* Free image function.
 * ARGUMENTS:
 *   - image store data pointer:
 *       vk3IMG *Im;
 * RETURNS: None.
 */
VOID VK3_ImageFree( vk3IMG *Im );

/* Get image pixel color function.
 * ARGUMENTS:
 *   - image store data pointer:
 *       vk3IMG *Im;
 *   - pixel coordinates:
 *       INT X, Y;
 * RETURNS:
 *   (DWORD) pixel color.
 */
DWORD VK3_ImageGetP( vk3IMG *Im, INT X, INT Y );


#endif /* __IMAGE_H_ */

/* END OF 'IMAGE.H' FILE */
