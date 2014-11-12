#ifndef __windows_h__
#define __windows_h__

#include <kernel.h>

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */


typedef UINT32 (STDCALL *FP_DRAW)(struct _WINDOW *Window);
typedef UINT32 (STDCALL *FP_MOUSEMOVE)(struct _WINDOW *Window, UINT32 x, UINT32 y);
typedef UINT32 (STDCALL *FP_MOUSEDOWN)(struct _WINDOW *Window, UINT32 x, UINT32 y, UINT32 btn);
typedef UINT32 (STDCALL *FP_MOUSEUP)(struct _WINDOW *Window, UINT32 x, UINT32 y, UINT32 btn);

typedef struct _WINDOW
{
	UINT32			x, y, cx, cy;
	FP_DRAW			Draw;
	FP_MOUSEMOVE	MouseMove;
	FP_MOUSEDOWN	MouseDown;
	FP_MOUSEUP		MouseUp;
	VIRTUAL_PTR		Data;
	struct _WINDOW	*Parent;
	struct _WINDOW	*Next;
	struct _WINDOW	*Prev;
	struct _WINDOW	*FirstChild;
}WINDOW;

typedef struct _POINTER
{
	UINT32	Width, Height;
	UINT32	HotX, HotY;
	BYTE	*Image;
	BYTE	*Bitmap;
}POINTER;

typedef struct _DRAWCONTEXT
{
	WINDOW *Window;
	UINT32 left, top;
	UINT32 width, height;
}DRAWCONTEXT;

UINT32 STDCALL ReDrawWindow(WINDOW *Window);
void STDCALL MouseMove(int x, int y);
void STDCALL MouseDown(int button);
void STDCALL MouseUp(int button);

UINT32 WindowsInit();

UINT32 STDCALL DesktopDraw(WINDOW *Window);
UINT32 STDCALL DesktopMouseMove(WINDOW *Window, UINT32 x, UINT32 y);
UINT32 STDCALL DesktopMouseDown(WINDOW *Window, UINT32 x, UINT32 y, UINT32 btn);
UINT32 STDCALL DesktopMouseUp(WINDOW *Window, UINT32 x, UINT32 y, UINT32 btn);

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif //__windows_h__