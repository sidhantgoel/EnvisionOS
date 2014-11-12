#ifndef __console_h__
#define __console_h__

#include <arch\arch.h>

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#define FGCOLOR_BLACK           0x0000
#define FGCOLOR_BLUE            0x0100
#define FGCOLOR_GREEN           0x0200
#define FGCOLOR_CYAN            0x0300
#define FGCOLOR_RED             0x0400
#define FGCOLOR_MAGENTA         0x0500
#define FGCOLOR_BROWN           0x0600
#define FGCOLOR_WHITE           0x0700
#define FGCOLOR_DARKGRAY        0x0800
#define FGCOLOR_BRIGHTBLUE      0x0900
#define FGCOLOR_BRIGHTGREEN     0x0A00
#define FGCOLOR_BRIGHTCYAN      0x0B00
#define FGCOLOR_PINK            0x0C00
#define FGCOLOR_BRIGHTMAGENTA   0x0D00
#define FGCOLOR_YELLOW          0x0E00
#define FGCOLOR_BRIGHTWHITE     0x0F00
#define BGCOLOR_BLACK           0x0000
#define BGCOLOR_BLUE            0x1000
#define BGCOLOR_GREEN           0x2000
#define BGCOLOR_CYAN            0x3000
#define BGCOLOR_RED             0x4000
#define BGCOLOR_MAGENTA         0x5000
#define BGCOLOR_BROWN           0x6000
#define BGCOLOR_WHITE           0x7000
#define BLINK                   0x8000

UINT32 STDCALL ConsoleInit();

UINT32 STDCALL TxtSetFlags(UINT32 Flags);
UINT32 STDCALL TxtClearScreen();
UINT32 STDCALL TxtHexDump(VPTR addr, SIZE_T size);
UINT32 STDCALL TxtScrollLine();
UINT32 STDCALL TxtPutChar(CHAR ch);
UINT32 STDCALL TxtUpdateCursor();
UINT32 STDCALL TxtPutStr(char *str,UINT32 flags,INT32 precision);
UINT32 STDCALL TxtPutFloat(double num);
UINT32 STDCALL TxtPutInt(UINT32 num,UINT32 base,UINT32 flags,UINT32 width,UINT32 precision);

UINT32 __cdecl Printf(char *format,...);

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif //__console_h__
