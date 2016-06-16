#include <stdio.h>
#include <string.h>

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

/* Shader support */
UINT VK3_RndPrg;

/* Materials array */
vk3MTL VK3_RndMaterials[VK3_MAX_MATERIALS];
INT VK3_RndNumOfMaterials;

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
    ratio_y = (DBL)VG4_Anim.H / VG4_Anim.W;

  VG4_RndMatrProj = MatrFrustum(-ratio_x * VK3_RndProjSize / 2,
                                 ratio_x * VK3_RndProjSize / 2,
                                -ratio_y * VK3_RndProjSize / 2,
                                 ratio_y * VK3_RndProjSize / 2,
                                 VK3_RndProjDist, VK3_RndFarClip);
} /* End of 'VK3_RndSetProj' function */

/* Object draw function.
 * ARGUMENTS:
 *   - object structure pointer:
 *       vk3OBJ *Obj;
 * RETURNS: None.
 */
VOID VK3_RndObjDraw( vk3OBJ *Obj )
{
  INT i;
  INT loc, mtl;
  MATR M, MSave;

  for (i = 0; i < Obj->NumOfPrims; i++)
  {
    /* Build transform matrix */
    MSave = VK3_RndMatrWorld;
    VK3_RndMatrWorld = MatrMulMatr(VK3_RndMatrWorld, Obj->Prims[i].M);
    M = MatrMulMatr(VK3_RndMatrWorld,
      MatrMulMatr(VK3_RndMatrView, VK3_RndMatrProj));
    glLoadMatrixf(M.A[0]);
    glUseProgram(VK3_RndPrg);

    mtl = Obj->Prims[i].MtlNo;
    if (mtl != -1)
    {
      if (VK3_RndMaterials[mtl].TexNo != 0)
      {
        glBindTexture(GL_TEXTURE_2D, VK3_RndMaterials[mtl].TexNo);
        if ((loc = glGetUniformLocation(VK3_RndPrg, "IsTexture")) != -1)
          glUniform1i(loc, 1);
      }
      else
      {
        if ((loc = glGetUniformLocation(VK3_RndPrg, "IsTexture")) != -1)
          glUniform1i(loc, 0);
      }
      if ((loc = glGetUniformLocation(VK3_RndPrg, "Ka")) != -1)
        glUniform3fv(loc, 1, &VK3_RndMaterials[mtl].Ka.X);
      if ((loc = glGetUniformLocation(VK3_RndPrg, "Kd")) != -1)
        glUniform3fv(loc, 1, &VK3_RndMaterials[mtl].Kd.X);
      if ((loc = glGetUniformLocation(VK3_RndPrg, "Ks")) != -1)
        glUniform3fv(loc, 1, &VK3_RndMaterials[mtl].Ks.X);
      if ((loc = glGetUniformLocation(VK3_RndPrg, "Ph")) != -1)
        glUniform1f(loc, VK3_RndMaterials[mtl].Ph);
      if ((loc = glGetUniformLocation(VK3_RndPrg, "Trans")) != -1)
        glUniform1f(loc, VK3_RndMaterials[mtl].Trans);
    }

    /* Setup global variables */
    if ((loc = glGetUniformLocation(VK3_RndPrg, "MatrWVP")) != -1)
      glUniformMatrix4fv(loc, 1, FALSE, M.A[0]);
    if ((loc = glGetUniformLocation(VK3_RndPrg, "MatrWorld")) != -1)
      glUniformMatrix4fv(loc, 1, FALSE, VK3_RndMatrWorld.A[0]);
    if ((loc = glGetUniformLocation(VK3_RndPrg, "MatrView")) != -1)
      glUniformMatrix4fv(loc, 1, FALSE, VK3_RndMatrView.A[0]);
    if ((loc = glGetUniformLocation(VK3_RndPrg, "MatrProj")) != -1)
      glUniformMatrix4fv(loc, 1, FALSE, VK3_RndMatrProj.A[0]);
    if ((loc = glGetUniformLocation(VK3_RndPrg, "Time")) != -1)
      glUniform1f(loc, VK3_Anim.Time);
    if ((loc = glGetUniformLocation(VK3_RndPrg, "PartNo")) != -1)
      glUniform1i(loc, i);

    /* Activete primitive vertex array */
    glBindVertexArray(Obj->Prims[i].VA);
    /* Activete primitive index buffer */
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Obj->Prims[i].IBuf);
    /* Draw primitive */
    glDrawElements(GL_TRIANGLES, Obj->Prims[i].NumOfI, GL_UNSIGNED_INT, NULL);
    glUseProgram(0);
    VK3_RndMatrWorld = MSave;
  }
} /* End of 'VK3_RndObjDraw' function */

/* Object free function.
 * ARGUMENTS:
 *   - object structure pointer:
 *       vk3OBJ *Obj;
 * RETURNS: None.
 */
VOID VK3_RndObjFree( vk3OBJ *Obj )
{
  INT i;

  for (i = 0; i < Obj->NumOfPrims; i++)
  {
    glBindVertexArray(Obj->Prims[i].VA);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &Obj->Prims[i].VBuf);
    glBindVertexArray(0);
    glDeleteVertexArrays(1, &Obj->Prims[i].VA);
    glDeleteBuffers(1, &Obj->Prims[i].IBuf);
  }
  free(Obj->Prims);
  memset(Obj, 0, sizeof(vk3OBJ));
} /* End of 'VK3_RndObjFree' function */

/* Material find by name function.
 * ARGUMENTS:
 *   - material name:
 *       CHAR *Name;
 * RETURNS:
 *   (INT) number of found material or -1 if no result.
 */
INT VK3_RndFindMaterial( CHAR *Name )
{
  INT i;

  for (i = 0; i < VK3_RndNumOfMaterials; i++)
    if (strcmp(Name, VK3_RndMaterials[i].Name) == 0)  /* #include <string.h> */
      return i;
  return -1;
} /* End of 'VK3_RndFindMaterial' function */

/* Material load function.
 * ARGUMENTS:
 *   - material file name:
 *       CHAR *FileName;
 * RETURNS: None.
*/
VOID VK3_RndLoadMaterials( CHAR *FileName )
{
  INT i, NumOfMaterials, x, y;
  UINT t;
  DWORD Sign;
  FILE *F;
  vk3`MTL M;
  BYTE *Image;

  if ((F = fopen(FileName, "rb")) == NULL)
    return;
  /* Read and check file signature */
  fread(&Sign, 4, 1, F);
  if (Sign != *(DWORD *)"GMT")
  {
    fclose(F);
    return;
  }

  /* Read all materials */
  fread(&NumOfMaterials, 4, 1, F);
  for (i = 0; i < NumOfMaterials; i++)
  {
    if (VK3_RndNumOfMaterials >= VK3_MAX_MATERIALS)
      break;

    /* Read illumination coefficients and texture parameters */
    fread(&M, sizeof(vk3MTL), 1, F);
    
    /* Read image */
    if (M.TexW != 0 && M.TexH != 0 && M.TexNo != 0)
    {
      /* Allocate memory for texture image */
      if ((Image = malloc(M.TexW * M.TexH * M.TexNo)) == NULL)
      {
        fclose(F);
        return;
      }
      fread(Image, M.TexNo, M.TexW * M.TexH, F);
      /* Flip image */
      /*
      for (y = 0; y < M.TexH / 2; y++)
        for (x = 0; x < M.TexW * M.TexNo; x++)
        {
          INT
            first = y * M.TexW * M.TexNo + x,
            last = (M.TexH - 1 - y) * M.TexW * M.TexNo + x;
          BYTE tmp = Image[first];

          Image[first] = Image[last];
          Image[last] = tmp;
        }
      */
      glGenTextures(1, &t);
      glBindTexture(GL_TEXTURE_2D, t);
      gluBuild2DMipmaps(GL_TEXTURE_2D, M.TexNo, M.TexW, M.TexH,
        M.TexNo == 3 ? GL_BGR_EXT : GL_BGRA, GL_UNSIGNED_BYTE, Image);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glBindTexture(GL_TEXTURE_2D, 0);
      M.TexNo = t;
    }
    /* Add material to animation and OpenGL */
    VK3_RndMaterials[VK3_RndNumOfMaterials++] = M;
  }
  fclose(F);
} /* End of 'VK3_RndLoadMaterials' function */

/* END OF 'RENDER.C' FILE */
