typedef struct
{
  VEC *P;
  INT NumOfP;
  INT(*Edges)[2];
  INT NumOfE;
}vk3PRIM;



VK3PrimCube = 
{
  CubeP, sizeof(cubeP) / sizeof(CubeP[0]), CubeE,
  sizeof(CubeE) / sizeof(CubeE[0]);
}

VEC CubeP[] = 
{
  { -1,  -1,   1},
  {  1,  -1,   1},
  {  1,  -1,  -1},
  { -1,  -1,  -1},
  { -1,   1,   1},
  {  1,   1,   1},
  {  1,   1,  -1},
  { -1,   1,  -1}
};

INT Cube[][2] = 
{
  {0, 1}, {1, 2}, {2, 3}, {3, 0},
  {4, 5}, {5, 6}, {6, 7}, {7, 4},
  {0, 4}, {1, 5}, {2, 6}, {3, 7}
}