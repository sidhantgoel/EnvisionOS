///////////////////////////////////////////////////////////////////////////////////
//----------------------------------KernelAPIC.c---------------------------------//
//                                      v 1.0                                    //
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
// Created by: Sidhant Goel                                                      //
// Created on: 27 feb 2007                                                       //
//                                                                               //
//--------------------------------------Notes------------------------------------//
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
///////////////////////////////////////////////////////////////////////////////////
#include <KernelAPIC.h>

#define MASTER_SLAVE	4

KESTATUS STDCALL KeAPICInit()
{
    KePICRemap(0x20, 0x28);
    return STATUS_SUCCESS;
}

KESTATUS STDCALL KePICRemap(UINT32 offset1, UINT32 offset2)
{
    _outp(PIC1_COMMAND, ICW1_INIT+ICW1_ICW4);  // starts the initialization sequence
    //io_wait();
    _outp(PIC2_COMMAND, ICW1_INIT+ICW1_ICW4);
    //io_wait();
    _outp(PIC1_DATA, offset1);                 // define the PIC vectors
    //io_wait();
    _outp(PIC2_DATA, offset2);
    //io_wait();
    _outp(PIC1_DATA, 4);                       // continue initialization sequence
    //io_wait();
    _outp(PIC2_DATA, 2);
    //io_wait();

    _outp(PIC1_DATA, ICW4_8086);
    //io_wait();
    _outp(PIC2_DATA, ICW4_8086);
    //io_wait();

    _outp(PIC1_DATA, 0xFF^MASTER_SLAVE);
    _outp(PIC2_DATA, 0xFF);

    return STATUS_SUCCESS;
}

KESTATUS STDCALL KePICIRQEnableAll()
{
    _outp(PIC1_DATA, 0);
    _outp(PIC2_DATA, 0);
    return STATUS_SUCCESS;
}

KESTATUS STDCALL KePICIRQDisableAll()
{
    _outp(PIC1_DATA, 0xFF^MASTER_SLAVE);
    _outp(PIC2_DATA, 0xFF);
    return STATUS_SUCCESS;
}

KESTATUS STDCALL KePICIRQEnable(BYTE Vector)
{
    BYTE Mask;
    BYTE a1;//,a2;
	//KeDebugPrintf("V_%d_", Vector);
    if(Vector < 8)
    {
		//KeDebugPrintf("V1_%d_", Vector);
        a1=_inp(PIC1_DATA);
        Mask = ~(1<<Vector);
        a1 = a1 & Mask;
        _outp(PIC1_DATA, a1);   // restore saved masks.
        return STATUS_SUCCESS;
    }
	else if(Vector <16)
	{
		//KeDebugPrintf("V2_%d_", Vector);
		a1=_inp(PIC2_DATA);
        Mask = ~(1<<(Vector-8));
        a1 = a1 & Mask;
        _outp(PIC2_DATA, a1);   // restore saved masks.
        return STATUS_SUCCESS;
	}
    return STATUS_ERROR;
}

KESTATUS STDCALL KePICIRQDisable(BYTE Vector)
{
    BYTE Mask;
    BYTE a1;//,a2;
    if(Vector < 8)
    {
        a1=_inp(PIC1_DATA);
        Mask = 1<<Vector;
        a1 = a1 | Mask;
        _outp(PIC1_DATA, a1);   // restore saved masks.
        return STATUS_SUCCESS;
    }
	else if(Vector <16)
	{
		a1=_inp(PIC2_DATA);
        Mask = 1<<(Vector-8);
        a1 = a1 | Mask;
        _outp(PIC2_DATA, a1);   // restore saved masks.
        return STATUS_SUCCESS;
	}
    return STATUS_ERROR;
}

///////////////////////////////////////////////////////////////////////////////////
//----------------------------------KernelAPIC.c---------------------------------//
//--------------------------Copyright © Sidhant Goel, 2007-----------------------//
///////////////////////////////////////////////////////////////////////////////////
