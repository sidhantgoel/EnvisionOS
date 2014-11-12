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
#include <KernelCpu.h>

KCPU *CPU[MAX_CPU];
UINT32 CPU_BS;

char *CpuFeatures[32] = {
	"CPU_FPU","CPU_VME","CPU_DE","CPU_PSE",
	"CPU_TSC","CPU_MSR","CPU_PAE","CPU_MCE",
	"CPU_CX8","CPU_APIC",NULL,"CPU_SEP",
	"CPU_MTRR","CPU_PGE","CPU_MCA","CPU_CMOV",
	"CPU_PAT","CPU_PSE36","CPU_PSN","CPU_CLFSH",
	NULL,"CPU_DS","CPU_ACPI","CPU_MMX",
	"CPU_FXSR","CPU_SSE","CPU_SSE2","CPU_SS",
	"CPU_HTT","CPU_TM",NULL,"CPU_PBE"
};

char *CpuFeatures1[32] = {
	"CPU_SSE3",NULL,NULL,"CPU_MONITOR",
	"CPU_DS_CPL","CPU_VMX",NULL,"CPU_EST",
	"CPU_TM2","CPU_SSSE3","CPU_CNXT_ID",NULL,
	NULL,"CPU_CMPXCHG16B","CPU_xTPR",NULL,
	NULL,NULL,NULL,NULL,
	NULL,NULL,NULL,NULL,
	NULL,NULL,NULL,NULL,
	NULL,NULL,NULL,NULL
};

KESTATUS STDCALL KeMultiProcessorInit()
{
	if(ACPI_SUCCESS(KeAcpiStatus()))
	{
		return STATUS_SUCCESS;
	}
	else
	{
		return STATUS_SUCCESS;
	}
}

KESTATUS STDCALL KeCpuBSInit()
{
	UINT32 i;
	UINT32 maxeax;
    _cpuid_args cpuid_args;

	KeMemAlloc((VIRTUAL_PTR*)&CPU[CPU_BS],sizeof(KCPU));

	cpuid_args.eax=0;
    _cpuid(&cpuid_args);
    ((UINT32*)CPU[CPU_BS]->VendorString)[0] = cpuid_args.ebx;
    ((UINT32*)CPU[CPU_BS]->VendorString)[1] = cpuid_args.edx;
    ((UINT32*)CPU[CPU_BS]->VendorString)[2] = cpuid_args.ecx;
    //_hang();
    KeDebugPrintf("\nCPU Vendor String : %s",CPU[CPU_BS]->VendorString);
    maxeax=cpuid_args.eax;
    KeDebugPrintf("\nHighest EAX for CPUID : %d",maxeax);
    if(maxeax>=1)
    {
        cpuid_args.eax=1;
        _cpuid(&cpuid_args);

        CPU[CPU_BS]->Stepping = cpuid_args.eax & 0xF;
        CPU[CPU_BS]->Model = (cpuid_args.eax>>4) & 0xF;
        CPU[CPU_BS]->Family = (cpuid_args.eax>>8) & 0xF;
        CPU[CPU_BS]->Type = (cpuid_args.eax>>12) & 0x3;
        CPU[CPU_BS]->ExtModelID = (cpuid_args.eax>>16) & 0xF;
        CPU[CPU_BS]->ExtFamilyID = (cpuid_args.eax>>20) & 0xFF;
        CPU[CPU_BS]->BrandID = cpuid_args.ebx & 0xFF;
        CPU[CPU_BS]->CFLUSHChunkSize = (cpuid_args.ebx>>8) & 0xFF;
        CPU[CPU_BS]->LogicalProcessorID = (cpuid_args.ebx>>16) & 0xFF;
        CPU[CPU_BS]->ApicID = (cpuid_args.ebx>>24) & 0xFF;
        CPU[CPU_BS]->Features = cpuid_args.edx;
        CPU[CPU_BS]->Features1 = cpuid_args.ecx;

        KeDebugPrintf("\nFamily : %d, Model : %d, Stepping %d, Type : %d, Extended Family ID : %d, Extended Model ID : %d",
        CPU[CPU_BS]->Family,
        CPU[CPU_BS]->Model,
        CPU[CPU_BS]->Stepping,
        CPU[CPU_BS]->Type,
        CPU[CPU_BS]->ExtFamilyID,
        CPU[CPU_BS]->ExtModelID
        );

        KeDebugPrintf("\nBrand ID : %d, CFLUSH Chunk Size : %d, Logical Processor ID %d, APIC ID : %d",
        CPU[CPU_BS]->BrandID,
        CPU[CPU_BS]->CFLUSHChunkSize,
        CPU[CPU_BS]->LogicalProcessorID,
        CPU[CPU_BS]->ApicID
        );

        KeDebugPrintf("\nCPU Features :");

		for(i=0;i<32;i++)
		{
			if(CPU[CPU_BS]->Features & (1<<i))
			{
				if(CpuFeatures[i])
				{
					KeDebugPrintf(" %s,",CpuFeatures[i]);
				}
			}
		}

		for(i=0;i<32;i++)
		{
			if(CPU[CPU_BS]->Features1 & (1<<i))
			{
				if(CpuFeatures1[i])
				{
					KeDebugPrintf(" %s,",CpuFeatures1[i]);
				}
			}
		}

		//_hang();

		return STATUS_SUCCESS;
    }
	return STATUS_ERROR;
}

KESTATUS STDCALL KeCpuInit()
{
	KESTATUS status;
	UINT32 i;
    //_hang();;
	for(i=0;i<MAX_CPU;i++)
	{
		CPU[i] = NULL;
	}

	CPU_BS=0;
	
	status = KeCpuBSInit();

	if(KESTATUS_ERROR(status))
	{
		return status;
	}

	status = KeMultiProcessorInit();

    return status;
}

///////////////////////////////////////////////////////////////////////////////////
//-----------------------------------KernelCpu.h---------------------------------//
//--------------------------Copyright © Sidhant Goel, 2007-----------------------//
///////////////////////////////////////////////////////////////////////////////////
