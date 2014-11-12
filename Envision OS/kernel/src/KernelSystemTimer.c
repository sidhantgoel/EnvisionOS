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
#include "KernelSystemTimer.h"

#define TIMER_CONTROL_PORT      0x43
#define TIMER_DATA_PORT         0x40
#define SYSTEM_TIMER_SETUP      0x36

KESTATUS STDCALL KeTimerInit()
{
    KeTimerSetSpeed(1000);
    KeTimerEnable();
    return STATUS_SUCCESS;
}

KESTATUS STDCALL KeTimerEnable()
{
    return KePICIRQEnable(0);
}

KESTATUS STDCALL KeTimerDisable()
{
    return KePICIRQDisable(0);
}

KESTATUS STDCALL KeTimerSetSpeed(UINT32 hz)
{
    UINT32 Divisor;

    Divisor = 1193180 / hz;

    if(!Divisor) Divisor++;

    _outp(TIMER_CONTROL_PORT, SYSTEM_TIMER_SETUP);
    _outp(TIMER_DATA_PORT, Divisor & 0xFF);
    _outp(TIMER_DATA_PORT, Divisor >> 8);
    return STATUS_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////////
//-------------------------------KernelSystemTimer.h-----------------------------//
//--------------------------Copyright © Sidhant Goel, 2007-----------------------//
///////////////////////////////////////////////////////////////////////////////////
