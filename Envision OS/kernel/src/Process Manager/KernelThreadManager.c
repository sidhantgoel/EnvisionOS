///////////////////////////////////////////////////////////////////////////////////
//------------------------------KernelThreadManager.c----------------------------//
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

KTHREAD *Threads[MAX_THREAD];
UINT32 ThreadCount;
VIRTUAL_PTR StackFree;

KESTATUS STDCALL KeThreadCreate(KTHREAD_FUNC ThreadFunc,VIRTUAL_PTR Arg)
{
    UINT32 ESP;
    KTHREAD *KernelThread;
    PHYSICAL_PTR StackPage;

    KeMemAlloc((VIRTUAL_PTR*)&KernelThread,sizeof(KTHREAD));

    StackFree -= 0x100000;

    KeVirtualAlloc(StackFree,0x100000,PAGE_READWRITE);
    //ESP=KePmmAllocate(1,0,&StackPage);
	//KeDebugPrintf("%X %X %X ", StackFree, (UINT32)StackPage, ESP);
    //ESP=KePageMap(0,StackPage,StackFree+0xFF000,MAP_WRITE);
	//KeDebugPrintf("%X %X %X ", StackFree, (UINT32)StackPage, ESP);
	//_hang();

    KernelThread->Stack = (UINT32*)(StackFree+0x100000);
    KernelThread->StartRoutine = ThreadFunc;
    KernelThread->Argument = Arg;
    KernelThread->Flags = THREAD_RUNNABLE;
    KernelThread->Owner = Processes[0];
    KernelThread->TimeOffset = 0;
    KernelThread->WaitingNext = NULL;

	//__asm jmp $

    *(--KernelThread->Stack) = (UINT32)KernelThread;      //Argument
    //*(--KernelThread->Stack) = 0;                         //Return Address
    
	*(--KernelThread->Stack) = EFLAGS_1|EFLAGS_IF;        //push EFLAG
    *(--KernelThread->Stack) = SEL_RING0_CODE;            //push CS
    *(--KernelThread->Stack) = (UINT32)&KeThreadStart;    //push EIP

    ESP = (UINT32)KernelThread->Stack;                    //Save ESP
    *(--KernelThread->Stack) = 0;                         //push EAX
    *(--KernelThread->Stack) = 0;                         //push ECX
    *(--KernelThread->Stack) = 0;                         //push EBX
    *(--KernelThread->Stack) = 0;                         //push EDX
    *(--KernelThread->Stack) = ESP;                       //push Orignal ESP
    *(--KernelThread->Stack) = 0;                         //push EBP
    *(--KernelThread->Stack) = 0;                         //push ESI
    *(--KernelThread->Stack) = 0;                         //push EDI
    
	*(--KernelThread->Stack) = SEL_RING0_DATA;            //push DS
    *(--KernelThread->Stack) = SEL_RING0_DATA;            //push ES
    *(--KernelThread->Stack) = SEL_RING0_DATA;            //push FS
    *(--KernelThread->Stack) = SEL_RING0_DATA;            //push GS
    
	*(--KernelThread->Stack) = LODWORD(Processes[0]->PageDir);  //push CR3

    Threads[ThreadCount] = KernelThread;
    ThreadCount++;

    __asm pushfd
	__asm cli

    KernelThread->Prev = Processes[0]->ThreadLast;
    KernelThread->Next = NULL;
    KernelThread->Prev->Next = KernelThread;
    Processes[0]->ThreadLast = KernelThread;

    KernelThread->SchedulerNext = CurrentThread;
    KernelThread->SchedulerPrev = CurrentThread->SchedulerPrev;

    KernelThread->SchedulerNext->SchedulerPrev = KernelThread;
    KernelThread->SchedulerPrev->SchedulerNext = KernelThread;

    __asm popfd

    return STATUS_SUCCESS;
}

KESTATUS STDCALL KeThreadManagerInit()
{
    KPROCESS *KernelProcess = Processes[0];
    KTHREAD *KernelThread;

    KeMemAlloc((VIRTUAL_PTR*)&KernelThread,sizeof(KTHREAD));

    memset(Threads,0,sizeof(Threads));

    KernelProcess->ThreadFirst = KernelThread;
    KernelProcess->ThreadLast = KernelThread;

    KernelThread->Stack = NULL;
    KernelThread->StartRoutine = NULL;
    KernelThread->Argument = NULL;
    KernelThread->Flags = THREAD_RUNNABLE;
    KernelThread->Owner = KernelProcess;
    KernelThread->Prev = NULL;
    KernelThread->Next = NULL;
    KernelThread->SchedulerNext = NULL;
    KernelThread->SchedulerPrev = NULL;
    KernelThread->TimeOffset = 0;
    KernelThread->WaitingNext = NULL;

    Threads[0] = KernelThread;

    ThreadCount = 1;

    StackFree = (VIRTUAL_PTR)KERNEL_STACK;

    return STATUS_SUCCESS;
}
///////////////////////////////////////////////////////////////////////////////////
//------------------------------KernelThreadManager.c----------------------------//
//--------------------------Copyright © Sidhant Goel, 2007-----------------------//
///////////////////////////////////////////////////////////////////////////////////
