///////////////////////////////////////////////////////////////////////////////////
//----------------------------------KernelPanic.c--------------------------------//
//                                      v 1.0                                    //
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
// Created by: Sidhant Goel                                                      //
// Created on: 12 march 2007                                                     //
//                                                                               //
//--------------------------------------Notes------------------------------------//
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
///////////////////////////////////////////////////////////////////////////////////
#include "KernelPanic.h"

KESTATUS __declspec(naked) KernelPanic()
{
    _hang();
    __asm
    {
        mov eax,0;
    }
}

///////////////////////////////////////////////////////////////////////////////////
//----------------------------------KernelPanic.c--------------------------------//
//--------------------------Copyright � Sidhant Goel, 2007-----------------------//
///////////////////////////////////////////////////////////////////////////////////
