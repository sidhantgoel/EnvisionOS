///////////////////////////////////////////////////////////////////////////////////
//-----------------------------KernelSynchronization.h---------------------------//
//                                      v 1.0                                    //
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
// Created by: Sidhant Goel                                                      //
// Created on: 21 march 2007                                                     //
//                                                                               //
//--------------------------------------Notes------------------------------------//
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
///////////////////////////////////////////////////////////////////////////////////
#ifndef KERNELSYNCHRONIZATION_H_INCLUDED
#define KERNELSYNCHRONIZATION_H_INCLUDED
#include <Kernel.h>
#include <KernelTasks.h>

typedef struct _KMUTEX
{
    UINT32 State;
}KMUTEX;

#define MUTEX_LOCKED        0x01

typedef struct _KSEMAPHORE
{
    UINT32  State;
    KTHREAD *Waiting;
}KSEMAPHORE;

#define SEMAPHORE_LOCKED    0x01

typedef UINT32  KSPINLOCK;

#define SPINLOCK_LOCKED     0x01

KESTATUS STDCALL KeSpinlockLock(KSPINLOCK *Spinlock);
KESTATUS STDCALL KeSpinlockUnlock(KSPINLOCK *Spinlock);

#endif // KERNELSYNCHRONIZATION_H_INCLUDED
///////////////////////////////////////////////////////////////////////////////////
//-----------------------------KernelSynchronization.h---------------------------//
//--------------------------Copyright © Sidhant Goel, 2007-----------------------//
///////////////////////////////////////////////////////////////////////////////////
