///////////////////////////////////////////////////////////////////////////////////
//--------------------------------KernelScheduler.c------------------------------//
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
#include <KernelTasks.h>

KTHREAD *ScheduledThreads;
KTHREAD *CurrentThread;

KTHREAD *WaitingThreads;

UINT32* STDCALL KeTasksNextThread(UINT32 *ESP)
{
	
    //KTHREAD *KernelThread;
    UINT32 *Stack;
	//KeDebugPrintf("Hi");
    CurrentThread->Stack = ESP;
    CurrentThread = CurrentThread->SchedulerNext;
    Stack = CurrentThread->Stack;
	//KeDebugPrintf("_%X", _inp(0x64));
    return Stack;
}

KESTATUS STDCALL KeThreadSleep(UINT32 ms)
{
    __asm int 30h
    return STATUS_SUCCESS;
}

KESTATUS STDCALL KeSchedulerInit()
{
    KTHREAD *KernelThread = Threads[0];

    KernelThread->SchedulerNext = KernelThread;
    KernelThread->SchedulerPrev = KernelThread;
    KernelThread->TimeOffset = 0;
    KernelThread->WaitingNext = NULL;

    CurrentThread = KernelThread;
    WaitingThreads = NULL;

    KeSetIDTTrapGate(0x20,SEL_RING0_CODE,(UINT32)&KeTasksIntTimer);
    KeSetIDTTrapGate(0x30,SEL_RING0_CODE,(UINT32)&KeTasksIntSleep);

    KeTimerInit();

    return STATUS_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////////
//--------------------------------KernelScheduler.c------------------------------//
//--------------------------Copyright © Sidhant Goel, 2007-----------------------//
///////////////////////////////////////////////////////////////////////////////////
