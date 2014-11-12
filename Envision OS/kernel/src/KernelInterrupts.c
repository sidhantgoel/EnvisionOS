///////////////////////////////////////////////////////////////////////////////////
//-------------------------------KernelInterrupts.c------------------------------//
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
#include <KernelInterrupts.h>

IDT_ENTRY IDT[256];
DESC_PTR IDT_PTR = {sizeof(IDT)-1,IDT};

KESTATUS STDCALL KeInterruptsInit()
{
    UINT32 i;
    for(i=0;i<256;i++)
    {
        IDT[i].low = MAKE_INTGATE_LOW(SEL_RING0_CODE,KeUnhandledInterrupt);
        IDT[i].high = MAKE_INTGATE_HIGH(KeUnhandledInterrupt,1,DESC_DPL0,1);
    }
    _lidt(&IDT_PTR);
    _enable();

    return STATUS_SUCCESS;
}

KESTATUS STDCALL KeSetIDTIntGate(UINT32 index,SELECTOR sel,UINT32 offset)
{
    if(index>=256)
    {
        return STATUS_ERROR;
    }
    IDT[index].low = MAKE_INTGATE_LOW(sel,offset);
    IDT[index].high = MAKE_INTGATE_HIGH(offset,1,DESC_DPL0,1);
    return STATUS_SUCCESS;
}

KESTATUS STDCALL KeSetIDTTrapGate(UINT32 index,SELECTOR sel,UINT32 offset)
{
    if(index>=256)
    {
        return STATUS_ERROR;
    }
    IDT[index].low = MAKE_TRAPGATE_LOW(sel,offset);
    IDT[index].high = MAKE_TRAPGATE_HIGH(offset,1,DESC_DPL0,1);
    return STATUS_SUCCESS;
}

KESTATUS STDCALL KeSetIDTTaskGate(UINT32 index,SELECTOR sel)
{
    if(index>=256)
    {
        return STATUS_ERROR;
    }
    IDT[index].low = MAKE_TASKGATE_LOW(sel);
    IDT[index].high = MAKE_TASKGATE_HIGH(1,DESC_DPL0);
    return STATUS_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////////
//-------------------------------KernelInterrupts.c------------------------------//
//--------------------------Copyright © Sidhant Goel, 2007-----------------------//
///////////////////////////////////////////////////////////////////////////////////
