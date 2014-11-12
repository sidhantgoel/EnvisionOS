///////////////////////////////////////////////////////////////////////////////////
//--------------------------------KernelSyscalls.c-------------------------------//
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
#include "KernelSyscalls.h"

KSYSCALL_SERVICE_INFO *Services[MAX_SYSCALL_SERVICES];

KESTATUS STDCALL KeSyscallsInit()
{
    memset(Services,0,sizeof(Services));
    return STATUS_SUCCESS;
}

KESTATUS STDCALL KeSyscallsDefault()
{
    return 0;
}

KESTATUS STDCALL KeSyscallsRegisterCall(UINT32 Service,UINT32 Function,UINT32 Arguments,VIRTUAL_PTR Proc)
{
    KSYSCALL_SERVICE_INFO *SyscallService = Services[Service];
    if(SyscallService)
    {
        if(Function<SyscallService->SyscallsCount)
        {
            SyscallService->Calls[Function].Arguments = Arguments;
            SyscallService->Calls[Function].Routine = Proc;
            return STATUS_SUCCESS;
        }
    }
    return STATUS_ERROR;
}

KESTATUS STDCALL KeSyscallsRegisterService(UINT32 Index,UINT32 Count)
{
    KSYSCALL_SERVICE_INFO *SyscallService;
    UINT32 i;

    if(Services[Index])
    {
        return STATUS_ERROR;
    }
    KeMemAlloc((VIRTUAL_PTR*)&SyscallService,sizeof(KSYSCALL_SERVICE_INFO)+(Count*sizeof(VIRTUAL_PTR)));
    SyscallService->SyscallsCount = Count;
    Services[Index] = SyscallService;
    for(i=0;i<Count;i++)
    {
        KeSyscallsRegisterCall(Index,i,0,(VIRTUAL_PTR)&KeSyscallsDefault);
    }
    return STATUS_SUCCESS;
}

void __declspec(naked) KeSyscallsDispatcher()
{
    __asm
    {
        retn
    }
}

///////////////////////////////////////////////////////////////////////////////////
//--------------------------------KernelSyscalls.c-------------------------------//
//--------------------------Copyright © Sidhant Goel, 2007-----------------------//
///////////////////////////////////////////////////////////////////////////////////
