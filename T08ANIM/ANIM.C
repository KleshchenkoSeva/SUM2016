#include <stdio.h>
#include "anim.h"
#include <mmsystem.h>
#pragma comment(lib, "winmm")
#pragma comment(lib, "opengl32")
#pragma comment(lib, "glu32")
#pragma comment(lib, "glew32s")

#define VK3_GET_JOYSTICK_AXIS(A) \
  (2.0 * (ji.dw##A##pos - jc.w##A##min) / (jc.w##A##max - jc.w##A##min - 1) - 1)

INT VK3_MouseWheel;
vk3ANIM VK3_Anim;

static UINT64
  VK3_StartTime,
  VK3_OldTime,
  VK3_OldTimeFPS,
  VK3_PauseTime,
  VK3_TimePerSec,
  VK3_FrameCounter;



VOID VK3_AnimInit( HWND hWnd )
{
  HDC= hDC;
  INT i;
  PIXELFORMATDESCRIPTOR pfd = {0};

  /* Store window and create memory device context */
  VG4_Anim.hWnd = hWnd;
  VG4_Anim.hDC = GetDC(hWnd);
  /* OpenGL init: pixel format setup */
  pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
  pfd.nVersion = 1;
  pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL;
  pfd.cColorBits = 32;
  pfd.cDepthBits = 32;
  i = ChoosePixelFormat(VK3_Anim.hDC, &pfd);
  DescribePixelFormat(VK3_Anim.hDC, i, sizeof(pfd), &pfd);
  SetPixelFormat(VK3_Anim.hDC, i, &pfd);

  /* OpenGL init: setup rendering context */
  VK3_Anim.hGLRC = wglCreateContext(VK3_Anim.hDC);
  wglMakeCurrent(VK3_Anim.hDC, VK3_Anim.hGLRC);

  /* OpenGL init: setup extensions: GLEW library */
  if (glewInit() != GLEW_OK ||
      !(GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader))
  {
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(VK3_Anim.hGLRC);
    ReleaseDC(VK3_Anim.hWnd, VK3_Anim.hDC);
    exit(0);
  }
  glClearColor(0.3, 0.5, 0.7, 1);
  glEnable(GL_DEPTH_TEST);
}

VOID VK3_AnimClose( VOID )
{ 
  INT i;
  wglMakeCurrent(NULL, NULL);
  wglDeleteContext(VK3_Anim.hGLRC);

  for (i = 0; i < VK3_Anim.NumOfUnits; i++)
  {
    VK3_Anim.Units[i]->Close(VK3_Anim.Units[i], &VK3_Anim);
    free(VK3_Anim.Units[i]);
  }
  VK3_Anim.NumOfUnits = 0; 
  DeleteDC(VK3_Anim.hDC);
  DeleteObject(VK3_Anim.hFrame);
  memset(&VK3_Anim, 0, sizeof(vk3ANIM));
    /* Delete GDI data */
  ReleaseDC(VK3_Anim.hWnd, VK3_Anim.hDC);

}

VOID VK3_AnimResize(INT w, INT h)
{
  HDC hDC; 
  VK3_Anim.w = w;
  VK3_Anim.h = h;
  glViewport(0, 0, W, H);
  if (VK3_Anim.hFrame != NULL)
    DeleteObject(VK3_Anim.hFrame);
  hDC = GetDC(VK3_Anim.hWnd);
  VK3_Anim.hFrame  = CreateCompatibleBitmap(hDC, w, h);
  ReleaseDC(VK3_Anim.hWnd, hDC);
  SelectObject(VK3_Anim.hDC, VK3_Anim.hFrame);
  VK3_AnimCopyFrame():
  SwapBuffers(VK3_Anim.hDC);

}

VOID VK3_AnimCopyFrame( HDC hDC )
{
  BitBlt(hDC, 0, 0, VK3_Anim.w, VK3_Anim.h, VK3_Anim.hDC, 0, 0, SRCCOPY);
}

VOID VK3_AnimRender( VOID )
{
  INT i;
  POINT pt;
  LARGE_INTEGER t;
  VK3_FrameCounter++;
  QueryPerformanceCounter(&t);
  VK3_Anim.GlobalTime = (DBL)(t.QuadPart - VK3_StartTime) / VK3_TimePerSec;
  VK3_Anim.GlobalDeltaTime = (DBL)(t.QuadPart - VK3_OldTime) / VK3_TimePerSec;
  if (VK3_Anim.IsPause)
  {
    VK3_Anim.DeltaTime = 0;
    VK3_PauseTime += t.QuadPart - VK3_OldTime;
  }
  else
  {
    VK3_Anim.DeltaTime = VK3_Anim.GlobalDeltaTime;
    VK3_Anim.Time += (t.QuadPart - VK3_PauseTime - VK3_StartTime) / VK3_TimePerSec;
  }
  if (t.QuadPart - VK3_OldTimeFPS > VK3_TimePerSec )
  {
    CHAR str[100];
    VK3_Anim.FPS = VK3_FrameCounter * VK3_TimePerSec / (DBL)(t.QuadPart - VK3_OldTimeFPS);
    VK3_OldTimeFPS = t.QuadPart;
    sprintf(str, "FPS: %.5f", VK3_Anim.FPS);
    SetWindowText(VK3_Anim.hWnd, str);
    VK3_FrameCounter = 0;
  }
  VK3_OldTime = t.QuadPart;
  /* Keyboard */
  GetKeyboardState(VK3_Anim.Keys);
  for (i = 0; i < 256; i++)
  {
    VK3_Anim.Keys[i] >>= 7;
    if (!VK3_Anim.OldKeys[i] && VK3_Anim.Keys[i])
      VK3_Anim.KeysClick[i] = TRUE;
    else
      VK3_Anim.KeysClick[i] = FALSE;
  }
  memcpy(VK3_Anim.OldKeys, VK3_Anim.Keys, 256);
  /* Mouse */
  GetCursorPos(&pt);
  ScreenToClient(VK3_Anim.hWnd, &pt);
  VK3_Anim.mdx = pt.x - VK3_Anim.mx;
  VK3_Anim.mdy = pt.y - VK3_Anim.my;
  VK3_Anim.mx = pt.x;
  VK3_Anim.my = pt.y;
  /* JoyStick */
  if (joyGetNumDevs() > 0)
  {
    JOYCAPS jc;

    if (joyGetDevCaps(JOYSTICKID1, &jc, sizeof(jc)) == JOYERR_NOERROR)
    {
      JOYINFOEX ji;

      ji.dwSize = sizeof(JOYINFOEX);
      ji.dwFlags = JOY_RETURNALL;
      if (joyGetPosEx(JOYSTICKID1, &ji) == JOYERR_NOERROR)
      {
        for (i = 0; i < 32; i++)
          VK3_Anim.JBut[i] = (ji.dwButtons >> i) & 1;
        VK3_Anim.jx = VK3_GET_JOYSTICK_AXIS(X);
        VK3_Anim.jy = VK3_GET_JOYSTICK_AXIS(Y);
        VK3_Anim.jz = VK3_GET_JOYSTICK_AXIS(Z);
        VK3_Anim.jr = VK3_GET_JOYSTICK_AXIS(R);
      }
    }
  }
  for (i = 0; i < VK3_Anim.NumOfUnits; i++)
    VK3_Anim.Units[i]->Response(VK3_Anim.Units[i], &VK3_Anim);
  SelectObject(VK3_Anim.hDC, GetStockObject(NULL_PEN));
  SelectObject(VK3_Anim.hDC, GetStockObject(DC_BRUSH));
  SetDCBrushColor(VK3_Anim.hDC, RGB(50, 100, 150));
  Rectangle(VK3_Anim.hDC, 0, 0, VK3_Anim.w + 1, VK3_Anim.h + 1);
  for (i = 0; i < VK3_Anim.NumOfUnits; i++)
  {
    SelectObject(VK3_Anim.hDC, GetStockObject(DC_PEN));
    SelectObject(VK3_Anim.hDC, GetStockObject(DC_BRUSH));
    SetDCPenColor(VK3_Anim.hDC, RGB( 0, 0, 0));
    SetDCBrushColor(VK3_Anim.hDC, RGB( 210, 210, 210));
    VK3_Anim.Units[i]->Render(VK3_Anim.Units[i], &VK3_Anim);
  }
}

VOID VK3_AnimAddUnit( vk3UNIT *Uni )
{
  if ( VK3_Anim.NumOfUnits < VK3_MAX_UNITS )
  {
    VK3_Anim.Units[VK3_Anim.NumOfUnits] = Uni;
    VK3_Anim.Units[VK3_Anim.NumOfUnits]->Init(VK3_Anim.Units[VK3_Anim.NumOfUnits], &VK3_Anim);
    VK3_Anim.NumOfUnits++;
  }
}

VOID VK3_AnimDoExit( VOID )
{
  static BOOL IsExit = FALSE;
  if (IsExit)
    return;
  IsExit = TRUE;
  PostMessage(VK3_Anim.hWnd, WM_CLOSE, 0, 0);
}

VOID VK3AnimFlipFullScreen( VOID )
{
  static BOOL IsFullScreen = FALSE;
  static RECT SaveRect;
  if (IsFullScreen)
  {
    SetWindowPos(VK3_Anim.hWnd, HWND_TOP,
      SaveRect.left, SaveRect.top, 
      SaveRect.right, SaveRect.bottom - SaveRect.top, 
      SWP_NOOWNERZORDER);
  }
  else
  {
    HMONITOR hmon;
    MONITORINFOEX moninfo;
    RECT rc;

    GetWindowRect(VK3_Anim.hWnd, &SaveRect);
    hmon = MonitorFromWindow(VK3_Anim.hWnd, MONITOR_DEFAULTTONEAREST);
    moninfo.cbSize = sizeof(moninfo);
    GetMonitorInfo(hmon, (MONITORINFO *)&moninfo);
    rc = moninfo.rcMonitor;
    AdjustWindowRect(&rc, GetWindowLong(VK3_Anim.hWnd, GWL_STYLE), FALSE);
    SetWindowPos(VK3_Anim.hWnd, HWND_TOPMOST, 
      rc.left, rc.top, rc.right, rc.bottom - rc.top, 
      SWP_NOOWNERZORDER);
  }
  IsFullScreen = !IsFullScreen;
}
vk3VERTEX
