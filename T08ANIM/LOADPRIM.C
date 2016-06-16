#include <stdio.h>
#include <string.h>

#include "anim.h"

/* Load object from '*.g3d' file function.
 * ARGUMENTS:
 *   - object structure pointer:
 *       vk3OBJ *Obj;
 *   - file name:
 *       CHAR *FileName;
 * RETURNS:
 *   (BOOL) TRUE is success, FALSE otherwise.
 */
BOOL VK3_RndObjLoad( vk3OBJ *Obj, CHAR *FileName )
{
  FILE *F;
  DWORD Sign;
  INT NumOfPrimitives;
  CHAR MtlFile[300];
  INT NumOfV;
  INT NumOfI;
  CHAR Mtl[300];
  INT p;
  vk3VERTEX *V;
  INT *I;

  memset(Obj, 0, sizeof(vg4OBJ));

  F = fopen(FileName, "rb");
  if (F == NULL)
    return FALSE;

  /* File structure:
   *   4b Signature: "G3D\0"    CHAR Sign[4];
   *   4b NumOfPrimitives       INT NumOfPrimitives;
   *   300b material file name: CHAR MtlFile[300];
   *   repeated NumOfPrimitives times:
   *     4b INT NumOfV; - vertex count
   *     4b INT NumOfI; - index (triangles * 3) count
   *     300b material name: CHAR Mtl[300];
   *     repeat NumOfV times - vertices:
   *         !!! float point -> FLT
   *       typedef struct
   *       {
   *         VEC  P;  - Vertex position
   *         VEC2 T;  - Vertex texture coordinates
   *         VEC  N;  - Normal at vertex
   *         VEC4 C;  - Vertex color
   *       } VERTEX;
   *     repeat (NumOfF / 3) times - facets (triangles):
   *       INT N0, N1, N2; - for every triangle (N* - vertex number)
   */
  fread(&Sign, 4, 1, F);
  if (Sign != *(DWORD *)"G3D")
  {
    fclose(F);
    return FALSE;
  }
  fread(&NumOfPrimitives, 4, 1, F);
  fread(MtlFile, 1, 300, F);
  VG4_RndLoadMaterials(MtlFile);

  /* Allocate mnemory for primitives */
  if ((Obj->Prims = malloc(sizeof(vk3PRIM) * NumOfPrimitives)) == NULL)
  {
    fclose(F);
    return FALSE;
  }
  Obj->NumOfPrims = NumOfPrimitives;

  for (p = 0; p < NumOfPrimitives; p++)
  {
    /* Read primitive info */
    fread(&NumOfV, 4, 1, F);
    fread(&NumOfI, 4, 1, F);
    fread(Mtl, 1, 300, F);

    /* Allocate memory for primitive */
    if ((V = malloc(sizeof(vk3VERTEX) * NumOfV + sizeof(INT) * NumOfI)) == NULL)
    {
      while (p-- > 0)
      {
        glBindVertexArray(Obj->Prims[p].VA);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDeleteBuffers(1, &Obj->Prims[p].VBuf);
        glBindVertexArray(0);
        glDeleteVertexArrays(1, &Obj->Prims[p].VA);
        glDeleteBuffers(1, &Obj->Prims[p].IBuf);
      }
      free(Obj->Prims);
      memset(Obj, 0, sizeof(vg4OBJ));
      fclose(F);
      return FALSE;
    }
    I = (INT *)(V + NumOfV);
    Obj->Prims[p].NumOfI = NumOfI;
    Obj->Prims[p].M = MatrIdentity();
    Obj->Prims[p].MtlNo = VK3_RndFindMaterial(Mtl);
    fread(V, sizeof(vk3VERTEX), NumOfV, F);
    fread(I, sizeof(INT), NumOfI, F);

    /* Create OpenGL buffers */
    glGenVertexArrays(1, &Obj->Prims[p].VA);
    glGenBuffers(1, &Obj->Prims[p].VBuf);
    glGenBuffers(1, &Obj->Prims[p].IBuf);

    /* Activate vertex array */
    glBindVertexArray(Obj->Prims[p].VA);
    /* Activate vertex buffer */
    glBindBuffer(GL_ARRAY_BUFFER, Obj->Prims[p].VBuf);
    /* Store vertex data */
    glBufferData(GL_ARRAY_BUFFER, sizeof(vk3VERTEX) * NumOfV, V, GL_STATIC_DRAW);

    /* Setup data order */
    /*                    layout,
     *                      components count,
     *                          type
     *                                    should be normalize,
     *                                           vertex structure size in bytes (stride),
     *                                               offset in bytes to field start */
    glVertexAttribPointer(0, 3, GL_FLOAT, FALSE, sizeof(vk3VERTEX),
                          (VOID *)0); /* position */
    glVertexAttribPointer(1, 2, GL_FLOAT, FALSE, sizeof(vk3VERTEX),
                          (VOID *)sizeof(VEC)); /* texture coordinates */
    glVertexAttribPointer(2, 3, GL_FLOAT, FALSE, sizeof(vk3VERTEX),
                          (VOID *)(sizeof(VEC) + sizeof(VEC2))); /* normal */
    glVertexAttribPointer(3, 4, GL_FLOAT, FALSE, sizeof(vk3VERTEX),
                          (VOID *)(sizeof(VEC) * 2 + sizeof(VEC2))); /* color */

    /* Enable used attributes */
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);

    /* Indices */
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Obj->Prims[p].IBuf);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(INT) * NumOfI, I, GL_STATIC_DRAW);

    /* Disable vertex array */
    glBindVertexArray(0);

    free(V);
  }
  fclose(F);
  return TRUE;
} /* End of 'VK3_RndObjLoad' function */

/* END OF 'LOADPRIM.C' FILE */
