///////////////////////////////////////////////////////////////////////////////////
//------------------------------KernelProcessManager.c---------------------------//
//                                      v 1.0                                    //
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
// Created by: Sidhant Goel                                                      //
// Created on: 5 march 2007                                                      //
//                                                                               //
//--------------------------------------Notes------------------------------------//
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
///////////////////////////////////////////////////////////////////////////////////
#include <KernelTasks.h>

KPROCESS *Processes[MAX_PROCESS];
UINT32 ProcessCount;

KESTATUS STDCALL KeProcessManagerInit()
{
    KPROCESS *KernelProcess;

    KeMemAlloc((VIRTUAL_PTR*)&KernelProcess,sizeof(KPROCESS));

    memset(Processes,0,sizeof(Processes));

    KernelProcess->PageDir = _read_cr3();;
    KernelProcess->MemInfo = KERNEL_MEMINFO;
    KernelProcess->ThreadFirst = NULL;
    KernelProcess->ThreadLast = NULL;

    Processes[0] = KernelProcess;
    ProcessCount = 1;

    return STATUS_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////////
//-------------------------------KernelImageManager.c----------------------------//
//--------------------------Copyright © Sidhant Goel, 2007-----------------------//
///////////////////////////////////////////////////////////////////////////////////
