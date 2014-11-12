///////////////////////////////////////////////////////////////////////////////////
//-----------------------------------KernelIRQ.c---------------------------------//
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
#include "KernelIRQ.h"


void KeHandlerIRQ0();
void KeHandlerIRQ1();
void KeHandlerIRQ2();
void KeHandlerIRQ3();
void KeHandlerIRQ4();
void KeHandlerIRQ5();
void KeHandlerIRQ6();
void KeHandlerIRQ7();
void KeHandlerIRQ8();
void KeHandlerIRQ9();
void KeHandlerIRQ10();
void KeHandlerIRQ11();
void KeHandlerIRQ12();
void KeHandlerIRQ13();
void KeHandlerIRQ14();
void KeHandlerIRQ15();

KESTATUS STDCALL KeIRQInit()
{
    KeSetIDTTrapGate(0x20,SEL_RING0_CODE,(UINT32)KeHandlerIRQ0);
    KeSetIDTTrapGate(0x21,SEL_RING0_CODE,(UINT32)KeHandlerIRQ1);
    KeSetIDTTrapGate(0x22,SEL_RING0_CODE,(UINT32)KeHandlerIRQ2);
    KeSetIDTTrapGate(0x23,SEL_RING0_CODE,(UINT32)KeHandlerIRQ3);
    KeSetIDTTrapGate(0x24,SEL_RING0_CODE,(UINT32)KeHandlerIRQ4);
    KeSetIDTTrapGate(0x25,SEL_RING0_CODE,(UINT32)KeHandlerIRQ5);
    KeSetIDTTrapGate(0x26,SEL_RING0_CODE,(UINT32)KeHandlerIRQ6);
    KeSetIDTTrapGate(0x27,SEL_RING0_CODE,(UINT32)KeHandlerIRQ7);
    KeSetIDTTrapGate(0x28,SEL_RING0_CODE,(UINT32)KeHandlerIRQ8);
    KeSetIDTTrapGate(0x29,SEL_RING0_CODE,(UINT32)KeHandlerIRQ9);
    KeSetIDTTrapGate(0x2A,SEL_RING0_CODE,(UINT32)KeHandlerIRQ10);
    KeSetIDTTrapGate(0x2B,SEL_RING0_CODE,(UINT32)KeHandlerIRQ11);
    KeSetIDTTrapGate(0x2C,SEL_RING0_CODE,(UINT32)KeHandlerIRQ12);
    KeSetIDTTrapGate(0x2D,SEL_RING0_CODE,(UINT32)KeHandlerIRQ13);
    KeSetIDTTrapGate(0x2E,SEL_RING0_CODE,(UINT32)KeHandlerIRQ14);
    KeSetIDTTrapGate(0x2F,SEL_RING0_CODE,(UINT32)KeHandlerIRQ15);
    //KePICsRemap(0x20, 0x28);
    return STATUS_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////////
//-----------------------------------KernelIRQ.c---------------------------------//
//--------------------------Copyright © Sidhant Goel, 2007-----------------------//
///////////////////////////////////////////////////////////////////////////////////
