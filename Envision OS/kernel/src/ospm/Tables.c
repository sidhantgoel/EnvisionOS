///////////////////////////////////////////////////////////////////////////////////
//------------------------------------Tables.c-----------------------------------//
//                                      v 1.0                                    //
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
// Created by: Sidhant Goel                                                      //
// Created on: 27 feb 2007                                                       //
//                                                                               //
//--------------------------------------Notes------------------------------------//
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
///////////////////////////////////////////////////////////////////////////////////
#include <KernelACPI.h>

typedef struct _ACPI_MEM_INFO ACPI_MEM_INFO;

typedef struct _ACPI_MEM_INFO
{
    PHYSICAL_PTR    Address;
    VIRTUAL_PTR     Base;
    SIZE_T          Size;
    UINT32          Type;
    ACPI_MEM_INFO   *Next;
}ACPI_MEM_INFO;

VIRTUAL_PTR EBDA_SEG;
PHYSICAL_PTR EBDA_PTR;
VIRTUAL_PTR EBDA;
VIRTUAL_PTR ROM_SPACE;
ACPI_MEM_INFO *AcpiMemFirst;
ACPI_TABLE_RSDP *RsdpPtr;
ACPI_TABLE_RSDT	*RsdtPtr;

KESTATUS KeAcpiPhyToVirt(VIRTUAL_PTR *Addr,PHYSICAL_PTR PhyAddr)
{
    ACPI_MEM_INFO *AcpiMemInfo=AcpiMemFirst;
    while(AcpiMemInfo)
    {
        if(PhyAddr>=AcpiMemInfo->Address && PhyAddr<AcpiMemInfo->Address+AcpiMemInfo->Size)
        {
            *Addr = AcpiMemInfo->Base + (PhyAddr - AcpiMemInfo->Address);
            return ACPI_OK;
        }
    }
    return ACPI_ADDR_OUT_OF_RANGE;
}

KESTATUS KeAcpiChkRsdp(ACPI_TABLE_RSDP *Rsdp)
{
    BYTE chksum=0;
    UINT32 i;
    for(i=0;i<ACPI_RSDP_REV0_SIZE;i++)
    {
        chksum += ((BYTE*)Rsdp)[i];
    }
    if(chksum!=0)
    {
        KeDebugPrintf("\nRSDP 1.0 checksum invalid");
        return ACPI_RSDP_INVALID;
    }

    chksum=0;
    if(Rsdp->Revision==2)
    {
        KeDebugPrintf("\nRSDP 2.0 found");
        for(i=0;i<Rsdp->Length;i++)
        {
            chksum += ((BYTE*)Rsdp)[i];
        }
        if(chksum!=0)
        {
            KeDebugPrintf("\nRSDP 2.0 checksum invalid");
            return ACPI_RSDP_INVALID;
        }
    }
    
    KeDebugPrintf("\nRSDP checksum OK");
    return ACPI_OK;
}

KESTATUS KeAcpiTbInit()
{
    UINT32 i;
    BOOLEAN Found=FALSE;
    KEMEM_MAP *MemMap;
    ACPI_MEM_INFO *MemLast=NULL;
    KESTATUS status;
	
	KeIoSpaceMap(&EBDA_SEG,0x40E,2);
	KeDebugPrintf("\nEBDA Segment %X",*(USHORT*)EBDA_SEG);
	EBDA_PTR = (*(USHORT*)EBDA_SEG)<<4;

    KeIoSpaceMap(&EBDA,EBDA_PTR,0x400);
    KeIoSpaceMap(&ROM_SPACE,0xE0000,0x20000);

    MemMap = LoaderInfo->MemoryMaps;

    for(i=0;i<LoaderInfo->MemoryMapCount;i++)
    {
        if(MemMap[i].Type==MEM_RANGE_ACPI || MemMap[i].Type==MEM_RANGE_NVS)
        {
            if(MemLast)
            {
                KeMemAlloc((VIRTUAL_PTR*)&MemLast->Next,sizeof(ACPI_MEM_INFO));
                MemLast = MemLast->Next;
                MemLast->Address = MemMap[i].Start;
                MemLast->Size = MemMap[i].Size;
                MemLast->Type = MemMap[i].Type;
                KeIoSpaceMap(&MemLast->Base,MemLast->Address,MemLast->Size);
                MemLast->Next = NULL;
            }
            else
            {
                KeMemAlloc((VIRTUAL_PTR*)&MemLast,sizeof(ACPI_MEM_INFO));
                MemLast->Address = MemMap[i].Start;
                MemLast->Size = MemMap[i].Size;
                MemLast->Type = MemMap[i].Type;
                KeIoSpaceMap(&MemLast->Base,MemLast->Address,MemLast->Size);
                MemLast->Next = NULL;
                AcpiMemFirst = MemLast;
            }
        }
    }

    KeDebugPrintf("\nSearching EDBA for RSDP");

    for(i=0;i<0x1000;i+=16)
    {
        if(memcmp(EBDA+i,ACPI_SIG_RSDP,8)==0)
        {
            status = KeAcpiChkRsdp((ACPI_TABLE_RSDP*)(EBDA+i));
            if(ACPI_SUCCESS(status))
            {
                RsdpPtr = (ACPI_TABLE_RSDP*)(EBDA+i);
                Found=TRUE;
                break;
            }
        }
    }

    if(!Found)
    {
        KeDebugPrintf("\nSearching ROM AREA for RSDP");
        for(i=0;i<0x20000;i+=16)
        {
            if(memcmp(ROM_SPACE+i,ACPI_SIG_RSDP,8)==0)
            {
                status = KeAcpiChkRsdp((ACPI_TABLE_RSDP*)(ROM_SPACE+i));
                if(ACPI_SUCCESS(status))
                {
                    RsdpPtr = (ACPI_TABLE_RSDP*)(ROM_SPACE+i);
                    Found=TRUE;
                    break;
                }
            }
        }
    }

    if(!Found)
    {
        KeDebugPrintf("\nRSDP Not Found");
    }
    else
    {
        KeDebugPrintf("\nRSDP Found");
        KeDebugPrintf(""
        "\nSignature : %.8s"
        "\nChecksum : %02x"
        "\nOemId : %.6s"
        "\nRevision : %02x"
        "\nRsdtPhysicalAddress : %#010x"
        "\nLength : %#010x"
        "\nXsdtPhysicalAddress : %#010x%08x"
        "\nExtendedChecksum : %02x",
        RsdpPtr->Signature,
        0xFF & (UINT32)RsdpPtr->Checksum,
        RsdpPtr->OemId,
        0xFF & (UINT32)RsdpPtr->Revision,
        RsdpPtr->RsdtPhysicalAddress,
        RsdpPtr->Length,
        HIDWORD(RsdpPtr->XsdtPhysicalAddress),LODWORD(RsdpPtr->XsdtPhysicalAddress),
        0xFF & (UINT32)RsdpPtr->ExtendedChecksum);
    }

	//KeIoSpaceMap(&RsdtPtr,RsdpPtr->RsdtPhysicalAddress,RsdpPtr->

    _hang();
    return STATUS_SUCCESS;
}



///////////////////////////////////////////////////////////////////////////////////
//------------------------------------Tables.c-----------------------------------//
//--------------------------Copyright © Sidhant Goel, 2007-----------------------//
///////////////////////////////////////////////////////////////////////////////////
