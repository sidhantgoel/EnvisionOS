///////////////////////////////////////////////////////////////////////////////////
//-------------------------------KernelTaskManager.c-----------------------------//
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


KESTATUS STDCALL KeTestThread0(VIRTUAL_PTR Arg)
{
    while(1)
    {
        //kshell();
    }
    return STATUS_SUCCESS;
}

KESTATUS STDCALL KeTestThread1(VIRTUAL_PTR Arg)
{
    while(1)
    {
        //KeDebugPrintf("1");
    }
    return STATUS_SUCCESS;
}

KESTATUS STDCALL KeTestThread2(VIRTUAL_PTR Arg)
{
    while(1)
    {
        //KeDebugPrintf("2");
    }
    return STATUS_SUCCESS;
}

KESTATUS STDCALL KeTestThread3(VIRTUAL_PTR Arg)
{
    while(1)
    {
        //KeDebugPrintf("3");
    }
    return STATUS_SUCCESS;
}

KESTATUS STDCALL KeTaskManagerInit()
{

    KeProcessManagerInit();
	//_hang();
    KeThreadManagerInit();
	//_hang();
    KeSchedulerInit();
	//_hang();

    KeThreadCreate(&KeTestThread0,NULL);
	//_hang();
    KeThreadCreate(&KeTestThread1,NULL);
    KeThreadCreate(&KeTestThread2,NULL);
    KeThreadCreate(&KeTestThread3,NULL);
    //_hang();

    return STATUS_SUCCESS;
}
///////////////////////////////////////////////////////////////////////////////////
//-------------------------------KernelTaskManager.c-----------------------------//
//--------------------------Copyright © Sidhant Goel, 2007-----------------------//
///////////////////////////////////////////////////////////////////////////////////
