///////////////////////////////////////////////////////////////////////////////////
//-------------------------------KernelInitialize.c------------------------------//
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
#include "KernelInitialize.h"
#include "kshell/storage.h"
#include "kshell/volume.h"

char *KeAsmTest(void);

#pragma alloc_text(INIT, KeInitialize)

KESTATUS STDCALL KeInitialize()
{
    KESTATUS status;

    status = KeDebugInit();
    //_hang();
    if(!KESTATUS_SUCCESS(status)) return status;
    KeDebugPrintf("\nDebugger Initialized");

    status = KeMemoryManagerInit();
    if(!KESTATUS_SUCCESS(status)) return status;
    KeDebugPrintf("\nMemory Manager Initialized");
    //_hang();

    //status = KeACPIInit();
    //if(!KESTATUS_SUCCESS(status)) return status;
    //KeDebugPrintf("\nACPI Initialized");

    status = KeCpuInit();
    if(!KESTATUS_SUCCESS(status)) return status;
    KeDebugPrintf("\nCPU Initialized");

    status = KeAPICInit();
    if(!KESTATUS_SUCCESS(status)) return status;
    KeDebugPrintf("\nAPIC Initialized");
    //_hang();

    status = KeDescriptorsInit();
    if(!KESTATUS_SUCCESS(status)) return status;
    KeDebugPrintf("\nDescriptors Initialized");

    status = KeInterruptsInit();
    if(!KESTATUS_SUCCESS(status)) return status;
    KeDebugPrintf("\nInterrupts Initialized");

    status = KeExceptionsInit();
    if(!KESTATUS_SUCCESS(status)) return status;
    KeDebugPrintf("\nExceptions Initialized");

    status = KeIRQInit();
    if(!KESTATUS_SUCCESS(status)) return status;
    KeDebugPrintf("\nIRQ's Initialized");

    status = KeNamespaceInit();
    if(!KESTATUS_SUCCESS(status)) return status;
    KeDebugPrintf("\nNamespace Initialized");

    status = KeObjectManagerInit();
    if(!KESTATUS_SUCCESS(status)) return status;
    KeDebugPrintf("\nObjectManager Initialized");

    status = KeTaskManagerInit();
    if(!KESTATUS_SUCCESS(status)) return status;
    KeDebugPrintf("\nProcesses Initialized");

    status = KeSyscallsInit();
    if(!KESTATUS_SUCCESS(status)) return status;
    KeDebugPrintf("\nSyscalls Initialized");

    //status = KeBootVideoInit();
    //if(!KESTATUS_SUCCESS(status)) return status;
    //KeDebugPrintf("\nBoot Video Initialized");
    //_hang();

    status = KeDevicesInit();
    if(!KESTATUS_SUCCESS(status)) return status;
    KeDebugPrintf("\nDevices Initialized");

    KeDebugPrintf("\nEverything Initialized");
    KeDebugPrintf("\nAssembly Test : %s",KeAsmTest());
    KeDebugPrintf("\nHurray\n");

    return STATUS_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////////
//-------------------------------KernelInitialize.c------------------------------//
//--------------------------Copyright © Sidhant Goel, 2007-----------------------//
///////////////////////////////////////////////////////////////////////////////////
