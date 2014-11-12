////////////////////////////////////////////////////////////////////////////////
//-----------------------------------heap.c-----------------------------------//
//                                   v 1.0                                    //
//                                                                            //
//                                                                            //
//                                                                            //
//                                                                            //
//                                                                            //
//                                                                            //
// Author: Sidhant Goel                                                       //
// Date Created: 08 July 2008                                                 //
//                                                                            //
//----------------------------------Notes-------------------------------------//
//                                                                            //
//                                                                            //
//                                                                            //
//                                                                            //
//                                                                            //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
#include <heap.h>

VPTR pHeap;

UINT32 STDCALL HeapInit()
{
    pHeap = (VPTR)0x380000;
    return 0;
}

VPTR STDCALL HeapAlloc(SIZE_T Size)
{
    VPTR pMem = pHeap - Size;
    pHeap = pMem - 4;
    *(UINT32*)pHeap = Size;
    return pMem;
}

VPTR STDCALL HeapRelloc(VPTR Mem, SIZE_T Size)
{
    VPTR pMem = pHeap - Size;
    pHeap = pMem - 4;
    *(UINT32*)pHeap = Size;
    memcpy(pMem, Mem, ((*(UINT32*)(Mem-4))>Size)?Size:(*(UINT32*)(Mem-4)));
    return pMem;
}

void STDCALL HeapFree(VPTR Mem)
{
    return;
}

////////////////////////////////////////////////////////////////////////////////
//-----------------------------------heap.c-----------------------------------//
//-----------------------Copyright © Sidhant Goel, 2008-----------------------//
////////////////////////////////////////////////////////////////////////////////