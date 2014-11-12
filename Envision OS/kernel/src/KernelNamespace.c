///////////////////////////////////////////////////////////////////////////////////
//--------------------------------KernelNamespace.c------------------------------//
//                                      v 1.0                                    //
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
// Created by: Sidhant Goel                                                      //
// Created on: 16 feb 2007                                                       //
//                                                                               //
//--------------------------------------Notes------------------------------------//
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
///////////////////////////////////////////////////////////////////////////////////
#include "KernelNamespace.h"

KNAMESPACE *KernelNamespace=NULL;

KESTATUS STDCALL KeNamespaceInit()
{
    KNAMESPACE_DIR *RootDir;
    KeMemAlloc((VIRTUAL_PTR*)&KernelNamespace,sizeof(KNAMESPACE));
    KeMemAlloc((VIRTUAL_PTR*)&RootDir,sizeof(KNAMESPACE_DIR));
    RootDir->Name = L"\\";
    RootDir->FileCount = 0;
    RootDir->DirCount = 0;
    RootDir->FirstFile = NULL;
    RootDir->FirstDir = NULL;
    RootDir->Parent = NULL;
    RootDir->Next = NULL;
    RootDir->Prev = NULL;
    KernelNamespace->RootDir = RootDir;
    return STATUS_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////////
//--------------------------------KernelNamespace.c------------------------------//
//--------------------------Copyright © Sidhant Goel, 2007-----------------------//
///////////////////////////////////////////////////////////////////////////////////
