///////////////////////////////////////////////////////////////////////////////////
//-------------------------------KernelSystemTimer.h-----------------------------//
//                                      v 1.0                                    //
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
// Created by: Sidhant Goel                                                      //
// Created on: 10 march 2007                                                     //
//                                                                               //
//--------------------------------------Notes------------------------------------//
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
///////////////////////////////////////////////////////////////////////////////////
#ifndef KERNELSYSTEMTIMER_H_INCLUDED
#define KERNELSYSTEMTIMER_H_INCLUDED
#include "Kernel.h"
#include "KernelAPIC.h"

KESTATUS STDCALL KeTimerInit();
KESTATUS STDCALL KeTimerEnable();
KESTATUS STDCALL KeTimerSetSpeed(UINT32 hz);
KESTATUS STDCALL KeTimerDisable();

#endif // KERNELSYSTEMTIMER_H_INCLUDED
///////////////////////////////////////////////////////////////////////////////////
//-------------------------------KernelSystemTimer.h-----------------------------//
//--------------------------Copyright © Sidhant Goel, 2007-----------------------//
///////////////////////////////////////////////////////////////////////////////////
