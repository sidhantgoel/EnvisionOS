///////////////////////////////////////////////////////////////////////////////////
//-------------------------------KernelExceptions.c------------------------------//
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
#include "KernelExceptions.h"

void KeHandlerDE();
void KeHandlerDB();
void KeHandlerBP();
void KeHandlerOF();
void KeHandlerBR();
void KeHandlerUD();
void KeHandlerNM();
void KeHandlerDF();
void KeHandlerTS();
void KeHandlerNP();
void KeHandlerSS();
void KeHandlerGP();
void KeHandlerPF();
void KeHandlerMF();
void KeHandlerAC();
void KeHandlerMC();
void KeHandlerXF();

KESTATUS STDCALL KeExceptionsInit()
{
    KeSetIDTIntGate(0,SEL_RING0_CODE,(UINT32)&KeHandlerDE);
    KeSetIDTIntGate(1,SEL_RING0_CODE,(UINT32)&KeHandlerDB);
    //KeSetIDTTrapGate(2,SEL_RING0_CODE,(UINT32)KeHandler);
    KeSetIDTIntGate(3,SEL_RING0_CODE,(UINT32)&KeHandlerBP);
    KeSetIDTIntGate(4,SEL_RING0_CODE,(UINT32)&KeHandlerOF);
    KeSetIDTIntGate(5,SEL_RING0_CODE,(UINT32)&KeHandlerBR);
    KeSetIDTIntGate(6,SEL_RING0_CODE,(UINT32)&KeHandlerUD);
    KeSetIDTIntGate(7,SEL_RING0_CODE,(UINT32)&KeHandlerNM);
    KeSetIDTIntGate(8,SEL_RING0_CODE,(UINT32)&KeHandlerDF);
    //KeSetIDTTrapGate(9,SEL_RING0_CODE,(UINT32)KeHandler);
    KeSetIDTIntGate(10,SEL_RING0_CODE,(UINT32)&KeHandlerTS);
    KeSetIDTIntGate(11,SEL_RING0_CODE,(UINT32)&KeHandlerNP);
    KeSetIDTIntGate(12,SEL_RING0_CODE,(UINT32)&KeHandlerSS);
    KeSetIDTIntGate(13,SEL_RING0_CODE,(UINT32)&KeHandlerGP);
    KeSetIDTIntGate(14,SEL_RING0_CODE,(UINT32)&KeHandlerPF);
    //KeSetIDTTrapGate(15,SEL_RING0_CODE,(UINT32)KeHandler);
    KeSetIDTIntGate(16,SEL_RING0_CODE,(UINT32)&KeHandlerMF);
    KeSetIDTIntGate(17,SEL_RING0_CODE,(UINT32)&KeHandlerAC);
    KeSetIDTIntGate(18,SEL_RING0_CODE,(UINT32)&KeHandlerMC);
    KeSetIDTIntGate(19,SEL_RING0_CODE,(UINT32)&KeHandlerXF);
    return STATUS_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////////
//-------------------------------KernelExceptions.c------------------------------//
//--------------------------Copyright © Sidhant Goel, 2007-----------------------//
///////////////////////////////////////////////////////////////////////////////////
