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
#include "KernelTextConsole.h"

USHORT *Buffer;
UINT32 ScreenX,ScreenY;
UINT32 Width,Height,Flags;

//#define TEXT_CONSOLE_DISABLE

#ifdef TEXT_CONSOLE_DISABLE

KESTATUS STDCALL KeTextConsoleClearScreen(){return STATUS_SUCCESS;}
KESTATUS STDCALL KeTextConsoleScrollLine(){return STATUS_SUCCESS;}
KESTATUS STDCALL KeTextConsolePutch(CHAR ch){return STATUS_SUCCESS;}
KESTATUS STDCALL KeTextConsoleUpdate(){return STATUS_SUCCESS;}
KESTATUS STDCALL KeTextConsoleInit(){return STATUS_SUCCESS;}

#else

KESTATUS STDCALL KeTextConsoleClearScreen()
{
    UINT32 i;
    for (i = 0;i<Width*Height;i++) Buffer[i] = Flags|' ';
    ScreenX = 0;
    ScreenY = 0;
    KeTextConsoleUpdate();
    return STATUS_SUCCESS;
}

KESTATUS STDCALL KeTextConsoleScrollLine()
{
    UINT32 i;
    memcpy(Buffer,Buffer+Width,Width*(Height-1)*2);
    for (i=Width*(Height-1);i<(Width*Height);i++) Buffer[i] = Flags|' ';
    ScreenX = 0;
    KeTextConsoleUpdate();
    return STATUS_SUCCESS;
}

KESTATUS STDCALL KeTextConsolePutch(CHAR ch)
{
    switch (ch)
    {
        case '\r':
            ScreenX=0;
            break;
        case '\n':
            ScreenX=0;
            ScreenY++;
            if (ScreenY==Height)
            {
                ScreenY--;
                KeTextConsoleScrollLine();
            }
            break;
        case '\t':
            //TODO : Tab Handler
            break;
        case '\b':
            if (ScreenX!=0) ScreenX--;
            break;
        default:
            Buffer[ScreenY*Width+ScreenX] = Flags|ch;
            ScreenX++;
            if (ScreenX==Width)
            {
                ScreenX=0;
                ScreenY++;
                if (ScreenY==Height)
                {
                    ScreenY--;
                    KeTextConsoleScrollLine();
                }
            }
    }
    KeTextConsoleUpdate();
    return STATUS_SUCCESS;
}

KESTATUS STDCALL KeTextConsoleUpdate()
{
    USHORT temp = ScreenY*80+ScreenX;
    _outp(0x3D4 + 0, 14);
    _outp(0x3D4 + 1, temp >> 8);
    _outp(0x3D4 + 0, 15);
    _outp(0x3D4 + 1, temp);
    return STATUS_SUCCESS;
}

KESTATUS STDCALL KeTextConsoleInit()
{
    Buffer = (USHORT*)0xB8000;
    Width = 80;
    Height = 25;
    Flags = BGCOLOR_RED|FGCOLOR_BRIGHTWHITE;
    KeTextConsoleClearScreen();
    return STATUS_SUCCESS;
}

#endif

///////////////////////////////////////////////////////////////////////////////////
//-------------------------------KernelTextConsole.h-----------------------------//
//--------------------------Copyright © Sidhant Goel, 2007-----------------------//
///////////////////////////////////////////////////////////////////////////////////
