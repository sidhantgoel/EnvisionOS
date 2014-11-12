///////////////////////////////////////////////////////////////////////////////////
//-------------------------------KernelBootVideo.h-------------------------------//
//                                      v 1.0                                    //
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
// Created by: Sidhant Goel                                                      //
// Created on: 8 march 2007                                                      //
//                                                                               //
//--------------------------------------Notes------------------------------------//
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
///////////////////////////////////////////////////////////////////////////////////
#ifndef KERNELBOOTVIDEO_H_INCLUDED
#define KERNELBOOTVIDEO_H_INCLUDED

#include <Kernel.h>
#include <KernelBitmap.h>
#include <KernelTasks.h>
#include <KernelMemoryManager.h>


/**********************************************************************************
 *      Boot Video Driver Functions
 *
 *********************************************************************************/
KESTATUS STDCALL KeBootVideoInit();
KESTATUS STDCALL KeBootVideoWriteRegs(BYTE *regs);
KESTATUS STDCALL KeBootVideoClearScreen();
KESTATUS STDCALL KeBootVideoPutpixel(UINT32 x, UINT32 y, BYTE c);
KESTATUS STDCALL KeBootVideoVLine(UINT32 x, UINT32 y,UINT32 Height, BYTE c);
KESTATUS STDCALL KeBootVideoHLine(UINT32 x, UINT32 y,UINT32 Width, BYTE c);
KESTATUS STDCALL KeBootVideoRect(UINT32 x, UINT32 y,UINT32 Width, UINT32 Height, BYTE c);
KESTATUS STDCALL KeBootVideoRectFill(UINT32 x, UINT32 y,UINT32 Width, UINT32 Height, BYTE c);
KESTATUS STDCALL KeBootVideoUpdate();
KESTATUS STDCALL KeBootVideoSetPalleteEntry(BYTE Index,BYTE Red,BYTE Green, BYTE Blue);
KESTATUS STDCALL KeBootVideoAnimatePallete(RGBQUAD *Pallete,UINT32 Count);
KESTATUS STDCALL KeBootVideoSetNullPallete(UINT32 Count);
KESTATUS STDCALL KeBootVideoSetPallete(RGBQUAD *Pallete,UINT32 Count);
KESTATUS STDCALL KeBootVideoSetBitmapPallete(BYTE *Bitmap);
KESTATUS STDCALL KeBootVideoDrawBitmap(BYTE *Bitmap,UINT32 x,UINT32 y);
KESTATUS STDCALL KeBootVideoDrawChar(BYTE *Data,UINT32 x,UINT32 y,UINT32 Width,UINT32 Height,BYTE c);
KESTATUS STDCALL KeBootVideoClearConsole();
KESTATUS STDCALL KeBootVideoNewLine();
KESTATUS STDCALL KeBootVideoPutch(CHAR ch,BYTE c);
KESTATUS STDCALL KeBootVideoPrints(CHAR *str,BYTE c);
KESTATUS STDCALL KeBootVideoAnimation(VIRTUAL_PTR Arg);
KESTATUS STDCALL KeBootVideoConsolePutch(CHAR ch);


/**********************************************************************************
 *      Boot Resource Information Structure
 *
 *********************************************************************************/
typedef struct _KBOOTVIDEO_INFO
{
    UINT32  BgResource;
    UINT32  PalleteResource;
    UINT32  FontResource;
    UINT32  FontWidth;
    UINT32  FontHeight;
    UINT32  ConsoleX;
    UINT32  ConsoleY;
    UINT32  ProgressX;
    UINT32  ProgressY;
    UINT32  ConsoleWidth;
    UINT32  ConsoleHeight;
    UINT32  ProgressCount;
    UINT32  ProgressDelay;
    UINT32  ProgressResources[];
}KBOOTVIDEO_INFO;

#endif // KERNELBOOTVIDEO_H_INCLUDED
///////////////////////////////////////////////////////////////////////////////////
//-------------------------------KernelBootVideo.h-------------------------------//
//--------------------------Copyright © Sidhant Goel, 2007-----------------------//
///////////////////////////////////////////////////////////////////////////////////
