#ifndef __video_h__
#define __video_h__
#include <fonts.h>

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

UINT32 VideoInit();

typedef UINT32	COLOR;

#define RGB(r,g,b)	(COLOR)((b & 0xFF)|((g & 0xFF)<<8)|((r & 0xFF)<<16))
#define RGB_B(x)	(BYTE)((x) & 0xFF)
#define RGB_G(x)	(BYTE)((x>>8) & 0xFF)
#define RGB_R(x)	(BYTE)((x>>16) & 0xFF)

UINT32 VideoInit();

UINT32 STDCALL GetModeInfo(kvideo_mode *video_mode);
UINT32 STDCALL DrawBitmap(UINT32 x,UINT32 y,UINT32 cx, UINT32 cy, BYTE* bmp, COLOR clr);
UINT32 STDCALL DrawChar(UINT32 x, UINT32 y, CHAR ch, COLOR clr, Font *pFont);
UINT32 STDCALL DrawHGradient(UINT32 x, UINT32 y, UINT32 cx, UINT32 cy, COLOR clr1, COLOR clr2);
UINT32 STDCALL DrawRect(UINT32 x, UINT32 y, UINT32 cx, UINT32 cy, UINT32 width, COLOR clr);
UINT32 STDCALL DrawVGradient(UINT32 x, UINT32 y, UINT32 cx, UINT32 cy, COLOR clr1, COLOR clr2);
UINT32 STDCALL DrawPixel(UINT32 x, UINT32 y, COLOR clr);
UINT32 STDCALL DrawString(UINT32 x, UINT32 y,CHAR *str, COLOR clr, Font *pFont);
UINT32 STDCALL DrawHLine(UINT32 x, UINT32 y, UINT32 width, COLOR clr);
UINT32 STDCALL DrawVLine(UINT32 x, UINT32 y, UINT32 height, COLOR clr);
UINT32 STDCALL DrawSolidRect(UINT32 x, UINT32 y, UINT32 cx, UINT32 cy, COLOR clr);

void _Putch(char ch);
void _Puts(char *str);

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif //__video_h__