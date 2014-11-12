///////////////////////////////////////////////////////////////////////////////////
//-------------------------------KernelInterrupts.h------------------------------//
//                                      v 1.0                                    //
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
// Created by: Sidhant Goel                                                      //
// Created on: 17 feb 2007                                                       //
//                                                                               //
//--------------------------------------Notes------------------------------------//
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
///////////////////////////////////////////////////////////////////////////////////
#ifndef KERNELINTERRUPTS_H_INCLUDED
#define KERNELINTERRUPTS_H_INCLUDED
#include "Kernel.h"
#include "KernelDebug.h"
#include "KernelDescriptors.h"

extern IDT_ENTRY IDT[256];

void KeUnhandledInterrupt();

KESTATUS STDCALL KeInterruptsInit();
KESTATUS STDCALL KeSetIDTIntGate(UINT32 index,SELECTOR sel,UINT32 offset);
KESTATUS STDCALL KeSetIDTTrapGate(UINT32 index,SELECTOR sel,UINT32 offset);
KESTATUS STDCALL KeSetIDTTaskGate(UINT32 index,SELECTOR sel);

#endif // KERNELINTERRUPTS_H_INCLUDED
///////////////////////////////////////////////////////////////////////////////////
//-------------------------------KernelInterrupts.h------------------------------//
//--------------------------Copyright © Sidhant Goel, 2007-----------------------//
///////////////////////////////////////////////////////////////////////////////////
