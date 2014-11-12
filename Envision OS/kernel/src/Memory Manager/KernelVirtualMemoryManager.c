///////////////////////////////////////////////////////////////////////////////////
//---------------------------KernelVirtualMemoryManager.c------------------------//
//                                      v 1.0                                    //
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
// Created by: Sidhant Goel                                                      //
// Created on: 14 feb 2007                                                       //
//                                                                               //
//--------------------------------------Notes------------------------------------//
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
///////////////////////////////////////////////////////////////////////////////////
#include <KernelMemoryManager.h>

typedef struct _KIO_MAP KIO_MAP;

typedef struct _KIO_MAP
{
	PHYSICAL_PTR	IoBase;
	VIRTUAL_PTR		VirtBase;
	SIZE_T			Size;
	UINT32			RefCount;
	KIO_MAP			*Next;
}KIO_MAP;

typedef struct _KMEMORY_INFO
{
    VIRTUAL_PTR     Base;
    UINT32          Type;
    void            *More;
    SIZE_T          Size;
    UINT32          Flags;
    KMEMORY_INFO    *Prev;
    KMEMORY_INFO    *Next;
}KMEMORY_INFO;

#define MEMORY_TYPE_FREE        0x00000000
#define MEMORY_TYPE_ALLOCATED   0x00000001
#define MEMORY_TYPE_FILE        0x00000002
#define MEMORY_TYPE_IMAGE       0x00000003
#define MEMORY_TYPE_IO          0x00000004
#define MEMORY_TYPE_UNKNOWN     0xFFFFFFFF

KMEMORY_INFO *KERNEL_MEMINFO=NULL;
VIRTUAL_PTR LastIoMap;

KIO_MAP *IoFirst=NULL;
KIO_MAP *IoLast=NULL;

KESTATUS STDCALL KeVirtualMemory(KMEMORY_INFO **MemInfoPtr,VIRTUAL_PTR Base,UINT32 Type, SIZE_T Size, UINT32 Flags,void *More)
{
	KMEMORY_INFO *mi = *MemInfoPtr;
	while(mi)
	{
		if((Base>=mi->Base && Base<(mi->Base+mi->Size)) || (mi->Base>=Base && mi->Base<(Base+Size)))
		{
			return STATUS_ERROR;
		}
		mi = mi->Next;
	}
	KeMemAlloc((VIRTUAL_PTR*)&mi, sizeof(KMEMORY_INFO));
	mi->Base = Base;
	mi->Flags = Flags;
	mi->Size = Size;
	mi->Type = Type;
	mi->More = More;
	mi->Next = *MemInfoPtr;
	mi->Prev = NULL;
	*MemInfoPtr = mi;
    return STATUS_SUCCESS;
}

KESTATUS STDCALL KeVirtualAlloc(VIRTUAL_PTR Base, SIZE_T Size, UINT32 Protection)
{
    return KeVirtualMemory(&KERNEL_MEMINFO,Base,0,Size,MEM_PRIVATE|(Protection&MASK_PAGE_PROTECT),NULL);
}

KESTATUS STDCALL KeIoSpaceMap(VIRTUAL_PTR *pAddr,PHYSICAL_PTR Addr,SIZE_T Size)
{
	KIO_MAP *iomap = IoFirst;
	KESTATUS s;
	UINT32 delta;
	delta = Addr - (Addr & (~0xFFF));
	Addr = Addr - delta;
	Size = ((Size-1)|0xFFF)+1;
	while(iomap)
	{
		if(Addr>=iomap->IoBase && Addr+Size<=iomap->IoBase+iomap->Size)
		{
			iomap->RefCount++;
			*pAddr = iomap->VirtBase + Addr - iomap->IoBase + delta;
			return STATUS_SUCCESS;
		}
	}
	if(Size<=0xFF800000-(SIZE_T)LastIoMap)
	{
		if(KESTATUS_SUCCESS(s=KePagesMap(0, Addr, LastIoMap, (((Size-1)|0xFFF)+1)>>12, MAP_NOCACHE|MAP_WRITE)))
		{
			KeMemAlloc((VIRTUAL_PTR*)&iomap, sizeof(KIO_MAP));
			iomap->IoBase = Addr;
			iomap->RefCount = 1;
			iomap->Size = Size;
			iomap->VirtBase = LastIoMap;
			*pAddr = LastIoMap + delta;
			LastIoMap += Size;
			if(IoFirst)
			{
				iomap->Next = IoFirst;
				IoFirst = iomap;
			}
			else
			{
				iomap->Next = NULL;
				IoFirst = IoLast = iomap;
			}
			return STATUS_SUCCESS;
		}
		else
			return s;
	}
	return STATUS_SUCCESS;
}

KESTATUS STDCALL KeVirtualMemoryManagerInit()
{
    SIZE_T KernelSize;

    KeImageGetSize(&KernelSize,(VIRTUAL_PTR)0xC0000000);

    KeDebugPrintf("\nAllocating Memory %08x",KernelSize);
	
	KERNEL_MEMINFO=NULL;
	IoLast=NULL;
	IoFirst=NULL;

	KeVirtualAlloc((VIRTUAL_PTR)0xC0000000,KernelSize,PAGE_EXECUTE_READWRITE);
    KeVirtualAlloc((VIRTUAL_PTR)KERNEL_HEAP_BASE,KERNEL_HEAP_SIZE,PAGE_READWRITE);
    KeVirtualAlloc((VIRTUAL_PTR)0xDFF00000,KERNEL_HEAP_SIZE,PAGE_READWRITE);

    LastIoMap = (VIRTUAL_PTR)0xE0000000;

    return STATUS_SUCCESS;
}

KESTATUS STDCALL KePageFault(VIRTUAL_PTR addr)
{
    KMEMORY_INFO *MemInfo = KERNEL_MEMINFO;
    PHYSICAL_PTR paddr;
    addr = (VIRTUAL_PTR)((SIZE_T)addr & 0xFFFFF000);
    //KeDebugPrintf("\nKERNEL_MEMINFO : %p",KERNEL_MEMINFO);
    while(MemInfo)
    {
        //KeDebugPrintf("\nVirtual Mapping");
        //KeDebugPrintf("\nBase : %p",MemInfo->Base);
        //KeDebugPrintf("\nSize : %p",MemInfo->Size);
        if((addr >= MemInfo->Base )&&(addr < MemInfo->Base+MemInfo->Size))
        {
            //KeDebugPrintf("\nMapping Found");
            KePmmAllocate(1,0,&paddr);
            KePageMap(0,paddr,addr,MAP_WRITE);
            //KeDebugPrintf("\nPage Mapped Successfuly");
            //KeDebugPrintf("\nPhysical Address : %#10x",paddr);
            //K//eDebugPrintf("\nVirtual Address : %#10x",addr);
            //KeDebugPrintf("\nError Code : %p",ec);
            return STATUS_SUCCESS;
        }
        MemInfo = MemInfo->Next;
    }
    KeDebugPrintf("\nPage Falt At : %#10x",addr);
    _hang();
    return STATUS_ERROR;
}

///////////////////////////////////////////////////////////////////////////////////
//---------------------------KernelVirtualMemoryManager.c------------------------//
//--------------------------Copyright © Sidhant Goel, 2007-----------------------//
///////////////////////////////////////////////////////////////////////////////////
