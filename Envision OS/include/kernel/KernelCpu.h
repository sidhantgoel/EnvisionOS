///////////////////////////////////////////////////////////////////////////////////
//-----------------------------------KernelCpu.h---------------------------------//
//                                      v 1.0                                    //
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
// Created by: Sidhant Goel                                                      //
// Created on: 28 feb 2007                                                       //
//                                                                               //
//--------------------------------------Notes------------------------------------//
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
///////////////////////////////////////////////////////////////////////////////////
#ifndef KERNELCPU_H_INCLUDED
#define KERNELCPU_H_INCLUDED
#include <Kernel.h>
#include <KernelDebug.h>
#include <KernelSynchronization.h>
#include <KernelAcpi.h>
#include <KernelTasks.h>

typedef struct _KCPU
{
    CHAR		VendorString[12];
    UINT32		Type;
    UINT32		Family;
    UINT32		Model;
    UINT32		Stepping;
    UINT32		ExtFamilyID;
    UINT32		ExtModelID;
    UINT32		BrandID;
    UINT32		CFLUSHChunkSize;
    UINT32		LogicalProcessorID;
    UINT32		ApicID;
    UINT32		Features;
    UINT32		Features1;
	KTHREAD		*ThreaQue;
	KSPINLOCK	ThreadQueLock;
}KCPU;

#define CPU_FPU     0x00000001
#define CPU_VME     0x00000002
#define CPU_DE      0x00000004
#define CPU_PSE     0x00000008
#define CPU_TSC     0x00000010
#define CPU_MSR     0x00000020
#define CPU_PAE     0x00000040
#define CPU_MCE     0x00000080
#define CPU_CX8     0x00000100
#define CPU_APIC    0x00000200
#define CPU_SEP     0x00000800
#define CPU_MTRR    0x00001000
#define CPU_PGE     0x00002000
#define CPU_MCA     0x00004000
#define CPU_CMOV    0x00008000
#define CPU_PAT     0x00010000
#define CPU_PSE36   0x00020000
#define CPU_PSN     0x00040000
#define CPU_CLFSH   0x00080000
#define CPU_DS      0x00200000
#define CPU_ACPI    0x00400000
#define CPU_MMX     0x00800000
#define CPU_FXSR    0x01000000
#define CPU_SSE     0x02000000
#define CPU_SSE2    0x04000000
#define CPU_SS      0x08000000
#define CPU_HTT     0x10000000
#define CPU_TM      0x20000000
#define CPU_PBE     0x80000000

#define CPU_SSE3        0x00000001
#define CPU_MONITOR     0x00000008
#define CPU_DS_CPL      0x00000010 //CPL-qualified Debug Store
#define CPU_VMX         0x00000020 //Enhanced SpeedStep Technology
#define CPU_EST         0x00000080
#define CPU_TM2         0x00000100
#define CPU_SSSE3       0x00000200
#define CPU_CNXT_ID     0x00000400
#define CPU_CMPXCHG16B  0x00002000
#define CPU_xTPR        0x00004000

KESTATUS STDCALL KeCpuInit();

#define MAX_CPU			256

#endif // KERNELCPU_H_INCLUDED
///////////////////////////////////////////////////////////////////////////////////
//-----------------------------------KernelCpu.h---------------------------------//
//--------------------------Copyright © Sidhant Goel, 2007-----------------------//
///////////////////////////////////////////////////////////////////////////////////
