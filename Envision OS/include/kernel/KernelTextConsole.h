///////////////////////////////////////////////////////////////////////////////////
//-------------------------------KernelTextConsole.h-----------------------------//
//                                      v 1.0                                    //
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
// Created by: Sidhant Goel                                                      //
// Created on: 14 march 2007                                                     //
//                                                                               //
//--------------------------------------Notes------------------------------------//
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
///////////////////////////////////////////////////////////////////////////////////
#ifndef KERNELTEXTCONSOLE_H_INCLUDED
#define KERNELTEXTCONSOLE_H_INCLUDED
#include <Kernel.h>

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

KESTATUS STDCALL KeTextConsoleInit();
KESTATUS STDCALL KeTextConsolePutch(CHAR ch);
KESTATUS STDCALL KeTextConsoleScrollLine(void);
KESTATUS STDCALL KeTextConsoleClearScreen(void);
KESTATUS STDCALL KeTextConsoleUpdate(void);

#endif // KERNELTEXTCONSOLE_H_INCLUDED
///////////////////////////////////////////////////////////////////////////////////
//-------------------------------KernelTextConsole.h-----------------------------//
//--------------------------Copyright © Sidhant Goel, 2007-----------------------//
///////////////////////////////////////////////////////////////////////////////////
