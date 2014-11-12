///////////////////////////////////////////////////////////////////////////////////
//--------------------------------KernelSyscalls.h-------------------------------//
//                                      v 1.0                                    //
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
// Created by: Sidhant Goel                                                      //
// Created on: 18 march 2007                                                     //
//                                                                               //
//--------------------------------------Notes------------------------------------//
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
///////////////////////////////////////////////////////////////////////////////////
#ifndef KERNELSYSCALLS_H_INCLUDED
#define KERNELSYSCALLS_H_INCLUDED
#include <Kernel.h>
#include <KernelMemoryManager.h>

#define MAX_SYSCALL_SERVICES    32

typedef struct _KSYSCALL_INFO
{
    UINT32      Arguments;
    VIRTUAL_PTR Routine;
}KSYSCALL_INFO;

typedef struct _KSYSCALL_SERVICE_INFO
{
    UINT32          SyscallsCount;
    KSYSCALL_INFO   Calls[];
}KSYSCALL_SERVICE_INFO;

/* Syscalls Initialization */
KESTATUS STDCALL KeSyscallsInit();

/* System Calls and Service Registration */
KESTATUS STDCALL KeSyscallsRegisterService(UINT32 Index,UINT32 Count);
KESTATUS STDCALL KeSyscallsRegisterCall(UINT32 Service,UINT32 Function,UINT32 Arguments,VIRTUAL_PTR Proc);

/* Default System Call Handler */
KESTATUS STDCALL KeSyscallsDefault();

/* System Call Dispatcher */
void KeSyscallsDispatcher();

#endif // KERNELSYSCALLS_H_INCLUDED
///////////////////////////////////////////////////////////////////////////////////
//--------------------------------KernelSyscalls.h-------------------------------//
//--------------------------Copyright © Sidhant Goel, 2007-----------------------//
///////////////////////////////////////////////////////////////////////////////////
