VOID VK3_RndPrimFree( vk3PRIM *Pr )
{
  if (Pr->P != NULL)
    free(Pr->P);
  if (Pr->Edges != NULL)
    free(Pr->Edges);
  memset(Pr, 0, sizeof(vk3PRIM));
} /* End of 'VG4_RndPrimFree' function */


/* Vertex representation type */
typedef struct
{
  VEC  P;  /* Vertex position */
  VEC2 T;  /* Vertex texture coordinates */
  VEC  N;  /* Normal at vertex */
  VEC4 C;  /* Vertex color */
} vk3VERTEX;

/* Load primitive from '*.g3d' file function.
 * ARGUMENTS:
 *   - primitive structure pointer:
 *       vk3PRIM *Pr;
 *   - file name:
 *       CHAR *FileName;
 * RETURNS:
 *   (BOOL) TRUE is success, FALSE otherwise.
 */
BOOL VK3_RndPrimLoad( vk3PRIM *Pr, CHAR *FileName )
{
  FILE *F;
  DWORD Sign;
  INT NumOfPrimitives;
  CHAR MtlFile[300];
  INT NumOfP;
  INT NumOfF;
  CHAR Mtl[300];
  vk3VERTEX V;
  INT p, N0, N1, N2;

  memset(Pr, 0, sizeof(vk3PRIM));

  F = fopen(FileName, "rb");
  if (F == NULL)
    return FALSE;

  /* File structure:
   *   4b Signature: "G3D\0"    CHAR Sign[4];
   *   4b NumOfPrimitives       INT NumOfPrimitives;
   *   300b material file name: CHAR MtlFile[300];
   *   repeated NumOfPrimitives times:
   *     4b INT NumOfP; - vertex count
   *     4b INT NumOfF; - facet (triangles) count
   *     300b material name: CHAR Mtl[300];
   *     repeat NumOfP times - vertices:
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
  for (p = 0; p < NumOfPrimitives; p++)
  {
    INT i;

    /* Read primitive info */
    fread(&NumOfP, 4, 1, F);
    fread(&NumOfF, 4, 1, F);
    fread(Mtl, 1, 300, F);

    /* Allocate memory for primitive */
    if ((Pr->P = malloc(sizeof(VEC) * NumOfP)) == NULL)
    {
      fclose(F);
      return FALSE;
    }
    if ((Pr->Edges = malloc(sizeof(INT [2]) * NumOfF)) == NULL)
    {
      free(Pr->P);
      Pr->P = NULL;
      fclose(F);
      return FALSE;
    }
    Pr->NumOfP = NumOfP;
    Pr->NumOfE = NumOfF;
    for (i = 0; i < NumOfP; i++)
    {
      fread(&V, sizeof(vk3VERTEX), 1, F);
      Pr->P[i] = V.P;
    }
    for (i = 0; i < NumOfF / 3; i++)
    {
      fread(&N0, 4, 1, F);
      fread(&N1, 4, 1, F);
      fread(&N2, 4, 1, F);
      Pr->Edges[i * 3 + 0][0] = N0;
      Pr->Edges[i * 3 + 0][1] = N1;
      Pr->Edges[i * 3 + 1][0] = N1;
      Pr->Edges[i * 3 + 1][1] = N2;
      Pr->Edges[i * 3 + 2][0] = N2;
      Pr->Edges[i * 3 + 2][1] = N0;
    }
    break;
  }
  fclose(F);
  return TRUE;
} /* End of 'VG4_RndPrimLoad' function */

