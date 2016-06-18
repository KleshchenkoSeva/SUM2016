typedef struct
{
  VEC  P;  /* Vertex position */
  VEC2 T;  /* Vertex texture coordinates */
  VEC  N;  /* Normal at vertex */
  VEC4 C;  /* Vertex color */
} vk3VERTEX;

/* Primitive representation type */
typedef struct
{
  /* OpenGL buffers */
  INT
    VA,       /* Vertex array */
    VBuf,     /* Vertex buffer */
    IBuf;     /* Index buffer */
  INT NumOfI; /* Facets index array size */
  MATR M;     /* Primitive transformation matrix */
  INT MtlNo;  /* Material number */
  INT Id;     /* Primitive Id */
} vk3PRIM;

/* Object representation type */
typedef struct
{
  vk3PRIM *Prims; /* Primitives array */
  INT NumOfPrims; /* Primitives array size */
  MATR M;         /* Object transformation matrix */
} vk3OBJ;

/* Grid geometry representation type */
typedef struct
{
  INT W, H;     /* Grid size in vertices */
  vk3VERTEX *V; /* Grid points */
} vk3GRID;

/* Material representation type */
typedef struct
{
  CHAR Name[300]; /* Material name */
  VEC Ka, Kd, Ks; /* Illumination coefficients */
  FLT Ph, Trans;  /* Shininess and Phong, transparency */
  INT TexW, TexH; /* Textiure image size */
  INT TexNo;      /* For load: bytes per pixel, in use: OpenGL texture no */
} vk3MTL;
