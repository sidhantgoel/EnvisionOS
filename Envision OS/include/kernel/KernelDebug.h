///////////////////////////////////////////////////////////////////////////////////
//----------------------------------KernelDebug.h--------------------------------//
//                                      v 1.0                                    //
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
// Created by: Sidhant Goel                                                      //
// Created on: 14 feb 2007                                                       //
//                                                                               //
//--------------------------------------Notes------------------------------------//
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
///////////////////////////////////////////////////////////////////////////////////
#ifndef KERNELDEBUG_H_INCLUDED
#define KERNELDEBUG_H_INCLUDED

#include <Kernel.h>
#include <KernelPanic.h>
#include <KernelTextConsole.h>

KESTATUS STDCALL KeDebugPanic(void);

KESTATUS STDCALL KeDebugInit();
KESTATUS STDCALL KeDebugClearScreen();
KESTATUS STDCALL KeDebugUpdate();
KESTATUS STDCALL KeDebugScrollLine();
KESTATUS STDCALL KeDebugPutChar(CHAR ch);
KESTATUS STDCALL KeDebugPutStr(char *str,UINT32 flags,INT32 precision);
KESTATUS STDCALL KeDebugPutFloat(double num);
KESTATUS STDCALL KeDebugPutInt(UINT32 num,UINT32 base,UINT32 flags,UINT32 width,UINT32 precision);
KESTATUS STDCALL KeDebugShowStatus(KESTATUS Status);
KESTATUS __cdecl KeDebugPrintf(char *format,...);
KESTATUS STDCALL KeDebugConsoleHook(UINT32 Index,void* pfn);

KESTATUS STDCALL KeDebugSerialWriteHexByte(BYTE Bye);
KESTATUS STDCALL KeDebugSerialWriteHexWord(USHORT Word);
KESTATUS STDCALL KeDebugSerialWriteHexDword(UINT32 Dword);
KESTATUS STDCALL KeDebugSerialWriteHexQword(UINT64 Qword);
KESTATUS STDCALL KeDebugSerialWriteChar(CHAR ch);
KESTATUS STDCALL KeDebugSerialWriteString(CHAR *String);
KESTATUS STDCALL KeDebugSerialHexDump(VIRTUAL_PTR Addr,SIZE_T Size);

typedef KESTATUS (STDCALL *PFN_CLEAR_SCREEN)();
typedef KESTATUS (STDCALL *PFN_SCROLL_LINE)();
typedef KESTATUS (STDCALL *PFN_UPDATE)();
typedef KESTATUS (STDCALL *PFN_WRITE_CHAR)(CHAR ch);

KESTATUS KeDebugPanic2e(UINT32 EDI,UINT32 ESI,UINT32 EBP,UINT32 ESP,UINT32 EBX,UINT32 EDX,UINT32 ECX,UINT32 EAX,UINT32 ErrorCode,UINT32 EIP,UINT32 CS,UINT32 EFLAGS);
KESTATUS KeDebugPanic2(UINT32 EDI,UINT32 ESI,UINT32 EBP,UINT32 ESP,UINT32 EBX,UINT32 EDX,UINT32 ECX,UINT32 EAX,UINT32 EIP,UINT32 CS,UINT32 EFLAGS);

#define PUT32_SIGN      0x01
#define PUT32_CAPS      0x02
#define PUT32_PREFIX    0x04
#define PUT32_COUNT     0x08

#endif // KERNELDEBUG_H_INCLUDED
///////////////////////////////////////////////////////////////////////////////////
//----------------------------------KernelDebug.h--------------------------------//
//--------------------------Copyright © Sidhant Goel, 2007-----------------------//
///////////////////////////////////////////////////////////////////////////////////
