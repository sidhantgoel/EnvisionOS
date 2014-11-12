///////////////////////////////////////////////////////////////////////////////////
//-------------------------------KernelDescriptors.c-----------------------------//
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
#include <KernelDescriptors.h>

#define MAX_GDTE    0x100

GDT_ENTRY GDT[MAX_GDTE];

DESC_PTR GDT_PTR = {sizeof(GDT)-1,GDT};

KESTATUS STDCALL KeDescriptorsInit()
{
    UINT32 i=0;

    GDT[i].low = 0;
    GDT[i].high = 0;
    i++;
    GDT[i].low = MAKE_DESC_LOW(0,0xFFFFF);
    GDT[i].high = MAKE_DESC_HIGH(0,0xFFFFF,DESC_G_4K,DESC_DB_32,1,DESC_DPL0,DESC_S_CODE_DATA,SEG_CODE);
    i++;
    GDT[i].low = MAKE_DESC_LOW(0,0xFFFFF);
    GDT[i].high = MAKE_DESC_HIGH(0,0xFFFFF,DESC_G_4K,DESC_DB_32,1,DESC_DPL0,DESC_S_CODE_DATA,SEG_DATA_WRITE);
    i++;
    GDT[i].low = MAKE_DESC_LOW(0,0xFFFFF);
    GDT[i].high = MAKE_DESC_HIGH(0,0xFFFFF,DESC_G_4K,DESC_DB_32,1,DESC_DPL3,DESC_S_CODE_DATA,SEG_CODE);
    i++;
    GDT[i].low = MAKE_DESC_LOW(0,0xFFFFF);
    GDT[i].high = MAKE_DESC_HIGH(0,0xFFFFF,DESC_G_4K,DESC_DB_32,1,DESC_DPL3,DESC_S_CODE_DATA,SEG_DATA_WRITE);
    i++;

    for(;i<MAX_GDTE;i++)
    {
        GDT[i].low = 0;
        GDT[i].high = 0;
    }

    _lgdt(&GDT_PTR);

	__asm
	{
		mov ax,SEL_RING0_DATA
		mov ds,ax
		mov es,ax
		mov fs,ax
		mov gs,ax
		mov ss,ax
	}
    return STATUS_SUCCESS;
}

KESTATUS STDCALL KeGdtePresent(UINT32 Index)
{
    if(Index < MAX_GDTE)
    {
        if(GDT[Index].high & 0x8000)
        {
            return STATUS_GDTE_PRESENT;
        }
        else
        {
            return STATUS_GDTE_FREE;
        }
    }
    return STATUS_INDEX_INVALID;
}

KESTATUS STDCALL KeGdteAdd(UINT32 Index,UINT32 LoDword,UINT32 HiDword)
{
    if(Index < MAX_GDTE)
    {
        if(GDT[Index].high & 0x8000)
        {
            return STATUS_GDTE_EXIST;
        }
        else
        {
            GDT[Index].low = LoDword;
            GDT[Index].high = HiDword;
            return STATUS_SUCCESS;
        }
    }
    return STATUS_INDEX_INVALID;
}

KESTATUS STDCALL KeGdteFree(UINT32 Index)
{
    if(Index < MAX_GDTE)
    {
        if(GDT[Index].high & 0x8000)
        {
            GDT[Index].low = 0;
            GDT[Index].high = 0;
            return STATUS_SUCCESS;
        }
        else
        {
            return STATUS_GDTE_NOT_PRESENT;
        }
    }
    return STATUS_INDEX_INVALID;
}


///////////////////////////////////////////////////////////////////////////////////
//-------------------------------KernelDescriptors.c-----------------------------//
//--------------------------Copyright © Sidhant Goel, 2007-----------------------//
///////////////////////////////////////////////////////////////////////////////////
