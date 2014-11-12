///////////////////////////////////////////////////////////////////////////////////
//------------------------------KernelObjectManager.h----------------------------//
//                                      v 1.0                                    //
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
// Created by: Sidhant Goel                                                      //
// Created on: 28 feb 2007                                                       //
//                                                                               //
//--------------------------------------Notes------------------------------------//
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
///////////////////////////////////////////////////////////////////////////////////
#ifndef KERNELOBJECTMANAGER_H_INCLUDED
#define KERNELOBJECTMANAGER_H_INCLUDED

#include <Kernel.h>
#include <KernelSecurity.h>

typedef struct _KTOKEN
{
    UINT32 RefCount;
    UINT32 Flags;
}KTOKEN;

#define TOKEN_KERNEL    0x80000000
#define TOKEN_SIGNALED  0x00000001

KESTATUS STDCALL KeObjectManagerInit();
KESTATUS STDCALL KeCreateToken();
KESTATUS STDCALL KeDestroyToken(HANDLE Token);

#endif // KERNELOBJECTMANAGER_H_INCLUDED
///////////////////////////////////////////////////////////////////////////////////
//------------------------------KernelObjectManager.h----------------------------//
//--------------------------Copyright © Sidhant Goel, 2007-----------------------//
///////////////////////////////////////////////////////////////////////////////////
