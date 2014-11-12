///////////////////////////////////////////////////////////////////////////////////
//----------------------------------KernelTasks.c--------------------------------//
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
#ifndef KERNELTASKS_H_INCLUDED
#define KERNELTASKS_H_INCLUDED

#include "Kernel.h"
#include "KernelDescriptors.h"
#include "KernelMemoryManager.h"
#include "KernelAPIC.h"
#include "KernelSystemTimer.h"

/**********************************************************************************
 *      Kernel Thread Entry Point Function Pointer Type
 *
 *********************************************************************************/
typedef KESTATUS (STDCALL *KTHREAD_FUNC)(VIRTUAL_PTR Arg);


/**********************************************************************************
 *      Kernel Process Structure
 *
 *********************************************************************************/
typedef struct _KPROCESS
{
    PHYSICAL_PTR    PageDir;
    KMEMORY_INFO    *MemInfo;
    KTHREAD         *ThreadFirst;
    KTHREAD         *ThreadLast;
}KPROCESS;


/**********************************************************************************
 *      Kernel Thread Structure
 *
 *********************************************************************************/
typedef struct _KTHREAD
{
    UINT32          *Stack;             /* Stack Pointer of The Thread              */
    KTHREAD_FUNC    StartRoutine;       /* Thread Start Routine                     */
    VIRTUAL_PTR     Argument;           /* Argument Passed To Thread Start Routine  */
    UINT32          Flags;              /* Thread Flags                             */
    UINT32          ExitCode;           /* Exit Code of The Thread                  */
    KPROCESS        *Owner;             /* Owner Process of Thread                  */
    UINT32          Priority;           /* Thread Priority                          */
    /* For Process Thread Linked List                                               */
    KTHREAD         *Prev;              /* Previous Thread                          */
    KTHREAD         *Next;              /* Next Thread                              */
    /* For Sheduler Linked List                                                     */
    KTHREAD         *SchedulerNext;     /* Next Thread in Scheduler Linked List     */
    KTHREAD         *SchedulerPrev;     /* Previous Thread in Scheduler Linked List */
    /* For Thread Waiting                                                           */
    HANDLE          WaitObject;         /* Object for Which Thread is Waiting       */
    KTHREAD         *WaitingNext;       /* Next Thread in Waiting Linked List       */
    KTHREAD         *WaitingPrev;       /* Previous Thread in Waiting Linked List   */
    UINT32          TimeOffset;         /* Time offset if Thread is Sleeping        */
}KTHREAD;

#define THREAD_RUNNING          0x01
#define THREAD_RUNNABLE         0x02
#define THREAD_WAITING          0x04
#define THREAD_SLEEPING         0x08


/**********************************************************************************
 *      Macros
 *
 *********************************************************************************/
#define MAX_PROCESS             1024
#define MAX_THREAD              1024

#define KERNEL_STACK            0xDFF00000
#define KERNEL_STACK_SIZE       0x00100000


/**********************************************************************************
 *      Kernel Task Initialization Functions
 *
 *********************************************************************************/
KESTATUS STDCALL KeTaskManagerInit();


/**********************************************************************************
 *      Kernel Thread Management Functions
 *
 *********************************************************************************/
KESTATUS STDCALL KeThreadManagerInit();
KESTATUS STDCALL KeThreadCreate(KTHREAD_FUNC ThreadFunc,VIRTUAL_PTR Arg);
KESTATUS STDCALL KeThreadPause(HANDLE Thread);
KESTATUS STDCALL KeThreadResume(HANDLE Thread);
KESTATUS STDCALL KeThreadSleep(UINT32 ms);


/**********************************************************************************
 *      Kernel Process Management Functions
 *
 *********************************************************************************/
KESTATUS STDCALL KeProcessManagerInit();
KESTATUS STDCALL KeProcessCreate(HANDLE *Handle, WCHAR *CmdLine);


/**********************************************************************************
 *      Kernel Scheduler Functions
 *
 *********************************************************************************/
KESTATUS STDCALL KeSchedulerInit();


/**********************************************************************************
 *      Internal Functions
 *
 *********************************************************************************/
void KeThreadStart(void);
/*Interrupt Functions*/
void KeTasksIntTimer();
void KeTasksIntSleep();


/**********************************************************************************
 *      Global Variables
 *
 *********************************************************************************/
extern VIRTUAL_PTR StackFree;
extern KPROCESS *Processes[MAX_PROCESS];
extern UINT32 ProcessCount;
extern KTHREAD *Threads[MAX_THREAD];
extern UINT32 ThreadCount;
extern KTHREAD *CurrentThread;
extern KTHREAD *WaitingThreads;

#endif // KERNELTASKS_H_INCLUDED
///////////////////////////////////////////////////////////////////////////////////
//----------------------------------KernelTasks.c--------------------------------//
//--------------------------Copyright © Sidhant Goel, 2007-----------------------//
///////////////////////////////////////////////////////////////////////////////////
