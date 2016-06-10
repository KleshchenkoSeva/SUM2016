#include <windows.h>
typedef struct tagvk3UNIT vk3UNIT;
typedef struct                     
{
  vk3UNIT *
} vk3ANIM;
strict tagvk3UNIT
{
  vk3ANIM 
};

VOID VK3_AnimInit( HWND hWnd )
{
  HDC hDC;

}

VOID VK3_AnimClose( VOID )
{ 
  for (i = 0; i < VK3_Anim.NumOfUnits; i++)
  {
    VK3_Anim.Units[i]->Close(VK3_Anim.Units[i], &VK3_Anim);
    free(VK3_Anim.Units[i]);
  }
  VK3_Anim.NumOfUnits = 0;                                                                                            

}
VOID VK3_AnimRender( VOID )
{
  for (i = 0; i < VK3_Anim.NumOfUnits; i++)
    VK3_Anim.Units[i]->Response(VK3_Anim.Units[i], &VK3_Anim);
  for (i = 0; i < VK3_Anim.NumOfUnits; i++)
  {
    VK3_Anim.Units[i]->Render(VG4_Anim.Units[i], &VK3_Anim);
  }
}
typedef struct
{
  HWND hWnd;
  HDC hDC;   
  INT W, H;  
  HBITMAP hFrame; 
  SA2UNIT *Units[SA2_MAX_UNITS]
  INT NumOfUnits; 
} SA2ANIM;
