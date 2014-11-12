///////////////////////////////////////////////////////////////////////////////////
//--------------------------KernelPhysicalMemoryManager.c------------------------//
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

/* TODO: New PMM Allocator
typedef struct _KRAM_MAP_128K
{
    UINT32  Bitmap;
    UINT32  Attributes;
}KRAM_MAP_128K;

typedef struct _KRAM_MAP_4096K
{
    UINT32 Bitmap;
    UINT32 BitmapPartial;
    UINT32 Attributes;
    UINT32 PrevFree;
    UINT32 PrevPartial;
    UINT32 NextFree;
    UINT32 NextPartial;
    KRAM_MAP_128K SubMaps[32];
}KRAM_MAP_4096K;

typedef struct _KRAM_INFO
{
    UINT64 MemorySize;                      //Memory Size in Bytes
    UINT64 MemortSize4k;                    //Memory Size in 4kb multiples
    UINT32 MemorySizeMb;                    //Memory Size in Mega Bytes
    UINT32 MemorySize4m;                    //Memory Size in 4mb multiples
    UINT32 FirstFree;                       //First Free Ram Region
    UINT32 FirstPartial;                    //First Partial Free Ram Region
    KRAM_MAP_4096K *MemoryMaps;             //Memory map of 4mb regions  
}KRAM_INFO;

KRAM_INFO RamInfo;      

#define MAP_ATTRIB_TYPE(a)              (a & 3)
#define MAP_ATTRIB_FREE_COUNT(a)        ((a>>2) & 0x3F)
#define MAP_ATTRIB_FREE_START(a)        ((a>>8) & 0x3F)
#define MAP_ATTRIB_FREE_END(a)          ((a>>14) & 0x3F)
#define MAP_ATTRIB_FREE_FIRST(a)        ((a>>20) & 0x3F)
#define MAP_ATTRIB_PARTIAL_COUNT(a)     ((a>>26) & 0x3F)

#define MAP_ATTRIB_TYPE_FREE            0
#define MAP_ATTRIB_TYPE_USED            1
#define MAP_ATTRIB_TYPE_MIXED           2
#define MAP_ATTRIB_TYPE_UNKNOWN         3

BYTE PmmTbl0[256];
BYTE PmmTbl1[256];

KESTATUS STDCALL KePmmBuildTables()
{
    UINT32 i,j,k;
    for(i=0;i<256;i++)
    {
        k=0;
        if(i & 0x01)  k++;
        if(i & 0x02)  k++;
        if(i & 0x04)  k++;
        if(i & 0x08)  k++;
        if(i & 0x10)  k++;
        if(i & 0x20)  k++;
        if(i & 0x40)  k++;
        if(i & 0x80)  k++;
        PmmTbl0[i] = 8-k;
        PmmTbl1[i] = k;
    }
}

KESTATUS STDCALL KePmmInit()
{
    UINT32 i,j;
    PHYSICAL_PTR RamSize;
    PHYSICAL_PTR RamSize4m;
    PHYSICAL_PTR RamSize128m;
    UINT64 temp1,temp2;
    UINT32 RamPagesCount;
    UINT32 RamInfoSize;
    PHYSICAL_PTR KernelHeap;
    VIRTUAL_PTR RamData;

	KernelHeap =  LoaderInfo->KernelPageData+LoaderInfo->KernelPageDataSize;
	KeDebugPrintf("A%x\n", LoaderInfo->KernelPageData);
    
    KePagesMap(0,KernelHeap,(VIRTUAL_PTR)KERNEL_HEAP_BASE,KERNEL_HEAP_SIZE>>12,MAP_WRITE);
    KeHeapManagerInit((VIRTUAL_PTR)KERNEL_HEAP_BASE,KERNEL_HEAP_SIZE);
	
	KeDebugPrintf("\nMemoryMapCount = %i",LoaderInfo->MemoryMapCount);

    for(i=0;i<LoaderInfo->MemoryMapCount;i++)
    {
        temp1 = LoaderInfo->MemoryMaps[i].Start;
        temp2 = temp1 + LoaderInfo->MemoryMaps[i].Size;
        if(LoaderInfo->MemoryMaps[i].Type==1)
        RamSize = temp2;
        
        KeDebugPrintf("\nMemory %0#10x%08x - %0#10x%08x Type %i",HIDWORD(temp1),LODWORD(temp1),HIDWORD(temp2),LODWORD(temp2),LoaderInfo->MemoryMaps[i].Type);
    }

    RamSize4m = ((RamSize-1)|0x3FFFFF)+1;
    RamSize128m = ((RamSize-1)|0x7FFFFFF)+1;

    RamPagesCount = RamSize>>12;

    KeDebugPrintf("\nRam Size = %08x",RamSize);
    KeDebugPrintf("\nRam Pages Count = %08x",RamPagesCount);
    
    RamInfoSize = sizeof(KRAM_MAP_4096K) * (RamSize4m>>22);

    KeDebugPrintf("\nRam Info Size = %08x",RamInfoSize);

    KeMemAlloc(&RamData,RamInfoSize);
    
    RamInfo.MemoryMaps = (KRAM_MAP_4096K*)RamData;
    RamInfo.MemorySize = RamSize4m;
    RamInfo.MemortSize4k = RamSize4m >> 12;
    RamInfo.MemorySizeMb = RamSize4m >> 20;
    RamInfo.MemorySize4m = RamSize4m >> 22;
    RamInfo.FirstFree = 0xFFFFFFFF;
    RamInfo.FirstPartial = 0xFFFFFFFF;

    for(i=0;i<RamInfo.MemorySize4m;i++)
    {
        RamInfo.MemoryMaps[i].Attributes = MAP_ATTRIB_TYPE_USED;
        RamInfo.MemoryMaps[i].Bitmap = 0xFFFFFFFF;
        RamInfo.MemoryMaps[i].BitmapPartial = 0xFFFFFFFF;
        for(j=0;j<32;j++)
        {
            RamInfo.MemoryMaps[i].SubMaps[j].Attributes = MAP_ATTRIB_TYPE_USED;
            RamInfo.MemoryMaps[i].SubMaps[j].Bitmap = 0xFFFFFFFF;
        }
    }


    for(i=0;i<LoaderInfo->MemoryMapCount;i++)
    {
        if(LoaderInfo->MemoryMaps[i].Type==MEM_RANGE_MEMORY)
        {
            KePmmSetState(LoaderInfo->MemoryMaps[i].Start,PMM_STATE_FREE,LoaderInfo->MemoryMaps[i].Size>>12);
            KeDebugPrintf("\nFree Memory %0#10x - %0#10x",(UINT32)LoaderInfo->MemoryMaps[i].Start,(UINT32)(LoaderInfo->MemoryMaps[i].Start+LoaderInfo->MemoryMaps[i].Size));
        }
    }

    KePmmSetState(KernelHeap,PMM_STATE_USED,KERNEL_HEAP_SIZE>>12);
    KePmmSetState(LoaderInfo->KernelAddress,PMM_STATE_USED,(LoaderInfo->KernelSize)>>12);
    KePmmSetState(LoaderInfo->KernelStack,PMM_STATE_USED,(LoaderInfo->KernelStackSize)>>12);
    KePmmSetState(LoaderInfo->KernelPageData,PMM_STATE_USED,(LoaderInfo->KernelPageDataSize)>>12);
    //KePmmSetState(LoaderInfo->KernelBSSData,PMM_STATE_USED,(LoaderInfo->KernelBSSDataSize)>>12);

    //_hang();
    return STATUS_SUCCESS;
}

KESTATUS STDCALL KePmmCheckState(PHYSICAL_PTR pAddr)
{
    UINT32 i,j,k,mask;

    if(pAddr > RamInfo.MemorySize4m || pAddr & 0xFFF)
    {
        return STATUS_INVALID_ADDRESS;
    }

    i = pAddr>>22;
    j = (pAddr>>17) & 0x1F;
    k = (pAddr>>12) & 0x1F;
    mask = 1 << k;

    if(RamInfo.MemoryMaps[i].SubMaps[j].Bitmap & mask)
    {
        return STATUS_TRUE;
    }

    return STATUS_FALSE;
}

KESTATUS STDCALL KePmmFixInfo(UINT32 i,UINT32 j)
{
    UINT32 FreeCount;
    UINT32 FreeStart;
    UINT32 FreeEnd;
    UINT32 FreeFirst;
    UINT32 PartialCount;
    UINT32 Mask,Mask2,Attrib;

    Mask = RamInfo.MemoryMaps[i].SubMaps[j].Bitmap;
    if(Mask==0)
    {
        RamInfo.MemoryMaps[i].SubMaps[j].Attributes = MAP_ATTRIB_TYPE_FREE | (32<<2) | (32<<8) | (32<<14) | (0<<20);
        RamInfo.MemoryMaps[i].Bitmap &= ~(1<<j);
        RamInfo.MemoryMaps[i].BitmapPartial &= ~(1<<j);
    }
    else if(Mask==0xFFFFFFFF)
    {
        RamInfo.MemoryMaps[i].SubMaps[j].Attributes = MAP_ATTRIB_TYPE_FREE | (0<<2) | (0<<8) | (0<<14) | (32<<20);
        RamInfo.MemoryMaps[i].Bitmap |= (1<<j);
        RamInfo.MemoryMaps[i].BitmapPartial |= (1<<j);
    }
    else
    {
        _BitScanForward(&FreeStart,Mask);
        _BitScanReverse(&FreeEnd,Mask);
        _BitScanForward(&FreeFirst,~Mask);
        FreeCount = PmmTbl0[((char*)&Mask)[0]];
        FreeCount += PmmTbl0[((char*)&Mask)[1]];
        FreeCount += PmmTbl0[((char*)&Mask)[2]];
        FreeCount += PmmTbl0[((char*)&Mask)[3]];

        RamInfo.MemoryMaps[i].SubMaps[j].Attributes = MAP_ATTRIB_TYPE_MIXED | (FreeCount<<2) | (FreeStart<<8) | (FreeEnd<<14) | (FreeFirst<<20);

        RamInfo.MemoryMaps[i].Bitmap |= (1<<j);
        RamInfo.MemoryMaps[i].BitmapPartial &= ~(1<<j);
    }
    
    Mask = RamInfo.MemoryMaps[i].Bitmap;
    Mask2 = RamInfo.MemoryMaps[i].BitmapPartial;

    _BitScanForward(&FreeStart,Mask);
    _BitScanReverse(&FreeEnd,Mask);
    _BitScanForward(&FreeFirst,~Mask);
    
    FreeCount = PmmTbl0[((char*)&Mask)[0]];
    FreeCount += PmmTbl0[((char*)&Mask)[1]];
    FreeCount += PmmTbl0[((char*)&Mask)[2]];
    FreeCount += PmmTbl0[((char*)&Mask)[3]];
    PartialCount = PmmTbl0[((char*)&Mask2)[0]];
    PartialCount += PmmTbl0[((char*)&Mask2)[1]];
    PartialCount += PmmTbl0[((char*)&Mask2)[2]];
    PartialCount += PmmTbl0[((char*)&Mask2)[3]];

    if(FreeCount==0)
        Attrib = MAP_ATTRIB_TYPE_FREE;
    else if(FreeCount==32 && PartialCount==32)
        Attrib = MAP_ATTRIB_TYPE_USED;
    else
        Attrib = MAP_ATTRIB_TYPE_MIXED;

    Attrib = Attrib | (FreeCount<<2) | (FreeStart<<8) | (FreeEnd<<14) | (FreeFirst<<20) | (PartialCount<<26);
    RamInfo.MemoryMaps[i].Attributes = Attrib;

    return STATUS_SUCCESS;
}

KESTATUS STDCALL KePmmSetState(PHYSICAL_PTR pAddr,UINT32 Count,UINT32 State)
{
    UINT32 i,j,i1,i2,j1,j2,k1,k2,mask1,mask2;
    PHYSICAL_PTR pAddr2 = pAddr + (Count<<12);

    if(pAddr > RamInfo.MemorySize4m || pAddr & 0xFFF)
        return STATUS_INVALID_ADDRESS;
    if(pAddr2 > RamInfo.MemorySize4m)
        return STATUS_INVALID_COUNT;
    if(Count==0)
        return STATUS_SUCCESS;

    i1 = pAddr >> 22;
    j1 = (pAddr >> 17) & 0x1F;
    k1 = (pAddr >> 12) & 0x1F;

    i2 = pAddr2 >> 22;
    j2 = (pAddr2 >> 17) & 0x1F;
    k2 = (pAddr2 >> 12) & 0x1F;

    if(i1==i2 && j1==j2)
    {
        mask1 = (0xFFFFFFFF<<k1)^(0xFFFFFFFF<<k2);
        if(State==PMM_STATE_FREE)
        {
            RamInfo.MemoryMaps[i1].SubMaps[j1].Bitmap &= ~mask1;
        }
        else
        {
            RamInfo.MemoryMaps[i1].SubMaps[j1].Bitmap |= mask1;
        }
        KePmmFixInfo(i1,j1);
        return STATUS_SUCCESS;
    }
    else
    {
        mask1 = 0xFFFFFFFF << k1;
        mask2 = ~(0xFFFFFFFF << k2);
        if(State==PMM_STATE_FREE)
        {
            RamInfo.MemoryMaps[i1].SubMaps[j1].Bitmap &= ~mask1;
            RamInfo.MemoryMaps[i2].SubMaps[j2].Bitmap &= ~mask2;
        }
        else
        {
            RamInfo.MemoryMaps[i1].SubMaps[j1].Bitmap |= mask1;
            RamInfo.MemoryMaps[i2].SubMaps[j2].Bitmap &= ~mask2;
        }
        KePmmFixInfo(i1,j1);
        KePmmFixInfo(i2,j2);
    }
    if(i1==i2)
    {
        if(State==PMM_STATE_FREE)
        {
            for(j=j1+1;j<j2;j++)
            {
                RamInfo.MemoryMaps[i1].SubMaps[j].Bitmap = 0;
                KePmmFixInfo(i1,j);
            }
        }
        else
        {
            for(j=j1+1;j<j2;j++)
            {
                RamInfo.MemoryMaps[i1].SubMaps[j].Bitmap = 0xFFFFFFFF;
                KePmmFixInfo(i1,j);
            }
        }
         return STATUS_SUCCESS;
    }
    else
    {
        if(State==PMM_STATE_FREE)
            for(j=j1+1;j<32;j++)
            {
                RamInfo.MemoryMaps[i1].SubMaps[j].Bitmap = 0;
                KePmmFixInfo(i1,j);
            }
        else
            for(j=j1+1;j<32;j++)
            {
                RamInfo.MemoryMaps[i1].SubMaps[j].Bitmap = 0xFFFFFFFF;
                KePmmFixInfo(i1,j);
            }
        for(i=i1+1;i<i2;i++)
        {
            if(State==PMM_STATE_FREE)
            {
                RamInfo.MemoryMaps[i].SubMaps[0].Bitmap = 0;
                RamInfo.MemoryMaps[i].SubMaps[1].Bitmap = 0;
                RamInfo.MemoryMaps[i].SubMaps[2].Bitmap = 0;
                RamInfo.MemoryMaps[i].SubMaps[3].Bitmap = 0;
                RamInfo.MemoryMaps[i].SubMaps[4].Bitmap = 0;
                RamInfo.MemoryMaps[i].SubMaps[5].Bitmap = 0;
                RamInfo.MemoryMaps[i].SubMaps[6].Bitmap = 0;
                RamInfo.MemoryMaps[i].SubMaps[7].Bitmap = 0;
                RamInfo.MemoryMaps[i].SubMaps[8].Bitmap = 0;
                RamInfo.MemoryMaps[i].SubMaps[9].Bitmap = 0;
                RamInfo.MemoryMaps[i].SubMaps[10].Bitmap = 0;
                RamInfo.MemoryMaps[i].SubMaps[11].Bitmap = 0;
                RamInfo.MemoryMaps[i].SubMaps[12].Bitmap = 0;
                RamInfo.MemoryMaps[i].SubMaps[13].Bitmap = 0;
                RamInfo.MemoryMaps[i].SubMaps[14].Bitmap = 0;
                RamInfo.MemoryMaps[i].SubMaps[15].Bitmap = 0;
                RamInfo.MemoryMaps[i].SubMaps[16].Bitmap = 0;
                RamInfo.MemoryMaps[i].SubMaps[17].Bitmap = 0;
                RamInfo.MemoryMaps[i].SubMaps[18].Bitmap = 0;
                RamInfo.MemoryMaps[i].SubMaps[19].Bitmap = 0;
                RamInfo.MemoryMaps[i].SubMaps[20].Bitmap = 0;
                RamInfo.MemoryMaps[i].SubMaps[21].Bitmap = 0;
                RamInfo.MemoryMaps[i].SubMaps[22].Bitmap = 0;
                RamInfo.MemoryMaps[i].SubMaps[23].Bitmap = 0;
                RamInfo.MemoryMaps[i].SubMaps[24].Bitmap = 0;
                RamInfo.MemoryMaps[i].SubMaps[25].Bitmap = 0;
                RamInfo.MemoryMaps[i].SubMaps[26].Bitmap = 0;
                RamInfo.MemoryMaps[i].SubMaps[27].Bitmap = 0;
                RamInfo.MemoryMaps[i].SubMaps[28].Bitmap = 0;
                RamInfo.MemoryMaps[i].SubMaps[29].Bitmap = 0;
                RamInfo.MemoryMaps[i].SubMaps[30].Bitmap = 0;
                RamInfo.MemoryMaps[i].SubMaps[31].Bitmap = 0;
            }
            else
            {
                RamInfo.MemoryMaps[i].SubMaps[0].Bitmap = 0xFFFFFFFF;
                RamInfo.MemoryMaps[i].SubMaps[1].Bitmap = 0xFFFFFFFF;
                RamInfo.MemoryMaps[i].SubMaps[2].Bitmap = 0xFFFFFFFF;
                RamInfo.MemoryMaps[i].SubMaps[3].Bitmap = 0xFFFFFFFF;
                RamInfo.MemoryMaps[i].SubMaps[4].Bitmap = 0xFFFFFFFF;
                RamInfo.MemoryMaps[i].SubMaps[5].Bitmap = 0xFFFFFFFF;
                RamInfo.MemoryMaps[i].SubMaps[6].Bitmap = 0xFFFFFFFF;
                RamInfo.MemoryMaps[i].SubMaps[7].Bitmap = 0xFFFFFFFF;
                RamInfo.MemoryMaps[i].SubMaps[8].Bitmap = 0xFFFFFFFF;
                RamInfo.MemoryMaps[i].SubMaps[9].Bitmap = 0xFFFFFFFF;
                RamInfo.MemoryMaps[i].SubMaps[10].Bitmap = 0xFFFFFFFF;
                RamInfo.MemoryMaps[i].SubMaps[11].Bitmap = 0xFFFFFFFF;
                RamInfo.MemoryMaps[i].SubMaps[12].Bitmap = 0xFFFFFFFF;
                RamInfo.MemoryMaps[i].SubMaps[13].Bitmap = 0xFFFFFFFF;
                RamInfo.MemoryMaps[i].SubMaps[14].Bitmap = 0xFFFFFFFF;
                RamInfo.MemoryMaps[i].SubMaps[15].Bitmap = 0xFFFFFFFF;
                RamInfo.MemoryMaps[i].SubMaps[16].Bitmap = 0xFFFFFFFF;
                RamInfo.MemoryMaps[i].SubMaps[17].Bitmap = 0xFFFFFFFF;
                RamInfo.MemoryMaps[i].SubMaps[18].Bitmap = 0xFFFFFFFF;
                RamInfo.MemoryMaps[i].SubMaps[19].Bitmap = 0xFFFFFFFF;
                RamInfo.MemoryMaps[i].SubMaps[19].Bitmap = 0xFFFFFFFF;
                RamInfo.MemoryMaps[i].SubMaps[20].Bitmap = 0xFFFFFFFF;
                RamInfo.MemoryMaps[i].SubMaps[21].Bitmap = 0xFFFFFFFF;
                RamInfo.MemoryMaps[i].SubMaps[22].Bitmap = 0xFFFFFFFF;
                RamInfo.MemoryMaps[i].SubMaps[23].Bitmap = 0xFFFFFFFF;
                RamInfo.MemoryMaps[i].SubMaps[24].Bitmap = 0xFFFFFFFF;
                RamInfo.MemoryMaps[i].SubMaps[25].Bitmap = 0xFFFFFFFF;
                RamInfo.MemoryMaps[i].SubMaps[26].Bitmap = 0xFFFFFFFF;
                RamInfo.MemoryMaps[i].SubMaps[27].Bitmap = 0xFFFFFFFF;
                RamInfo.MemoryMaps[i].SubMaps[28].Bitmap = 0xFFFFFFFF;
                RamInfo.MemoryMaps[i].SubMaps[29].Bitmap = 0xFFFFFFFF;
                RamInfo.MemoryMaps[i].SubMaps[30].Bitmap = 0xFFFFFFFF;
                RamInfo.MemoryMaps[i].SubMaps[31].Bitmap = 0xFFFFFFFF;
            }
            KePmmFixInfo(i,0);
            KePmmFixInfo(i,1);
            KePmmFixInfo(i,2);
            KePmmFixInfo(i,3);
            KePmmFixInfo(i,4);
            KePmmFixInfo(i,5);
            KePmmFixInfo(i,6);
            KePmmFixInfo(i,7);
            KePmmFixInfo(i,8);
            KePmmFixInfo(i,9);
            KePmmFixInfo(i,10);
            KePmmFixInfo(i,11);
            KePmmFixInfo(i,12);
            KePmmFixInfo(i,13);
            KePmmFixInfo(i,14);
            KePmmFixInfo(i,15);
            KePmmFixInfo(i,16);
            KePmmFixInfo(i,17);
            KePmmFixInfo(i,18);
            KePmmFixInfo(i,19);
            KePmmFixInfo(i,20);
            KePmmFixInfo(i,21);
            KePmmFixInfo(i,22);
            KePmmFixInfo(i,23);
            KePmmFixInfo(i,24);
            KePmmFixInfo(i,25);
            KePmmFixInfo(i,26);
            KePmmFixInfo(i,27);
            KePmmFixInfo(i,28);
            KePmmFixInfo(i,29);
            KePmmFixInfo(i,30);
            KePmmFixInfo(i,31);
        }
        if(State==PMM_STATE_FREE)
        {
            for(j=0;j<j2;j++)
            {
                RamInfo.MemoryMaps[i2].SubMaps[j].Bitmap = 0;
                KePmmFixInfo(i2,j);
            }
        }
        else
        {
            for(j=0;j<j2;j++)
            {
                RamInfo.MemoryMaps[i2].SubMaps[j].Bitmap = 0xFFFFFFFF;
                KePmmFixInfo(i2,j);
            }
        }
        return STATUS_SUCCESS;
    }
    return STATUS_ERROR;
}



KESTATUS STDCALL KePmmFindFreePages(UINT32 Count,UINT32 Options,PHYSICAL_PTR *pAddr)
{
    return STATUS_ERROR;
}

KESTATUS STDCALL KePmmAllocate(UINT32 Count,UINT32 Options,PHYSICAL_PTR *pAddr)
{
    return STATUS_ERROR;
}

KESTATUS STDCALL KePmmFree(UINT32 Count,PHYSICAL_PTR *pAddr)
{
    return STATUS_ERROR;
}

KESTATUS STDCALL KePageMap(PHYSICAL_PTR CR3, PHYSICAL_PTR pAddr, VIRTUAL_PTR vAddr, UINT32 Options)
{
    UINT32 *PTE;
    UINT32 Entry;
    UINT32 Index;
    if(CR3)
    {
        PTE = (UINT32*)0xFF800000;
    }
    else
    {
        PTE = (UINT32*)0xFFC00000;
    }
    Index = (UINT32)vAddr >> 12;
    if(vAddr >= (VIRTUAL_PTR)0x80000000)
    {
        Entry = (LODWORD(pAddr) & 0xFFFFF000)|(Options & MAP_OPTIONS)|PTE_GLOBAL|PTE_PRESENT;
    }
    else
    {
        Entry = (LODWORD(pAddr) & 0xFFFFF000)|(Options & MAP_OPTIONS)|PTE_USER|PTE_PRESENT;
    }
    PTE[Index] = Entry;
    return STATUS_SUCCESS;
}

KESTATUS STDCALL KePagesMap(PHYSICAL_PTR CR3, PHYSICAL_PTR pAddr,VIRTUAL_PTR vAddr,UINT32 Count, UINT32 Options)
{
    UINT32 *PTE;
    UINT32 Entry;
    UINT32 Index;
    UINT32 i;
    if(CR3)
    {
        PTE = (UINT32*)0xFF800000;
    }
    else
    {
       PTE = (UINT32*)0xFFC00000;
    }
    Index = (UINT32)vAddr >> 12;
    if(vAddr >= (VIRTUAL_PTR)0x80000000)
    {
        Entry = (Options & MAP_OPTIONS)|PTE_GLOBAL|PTE_PRESENT;
    }
    else
    {
        Entry = (Options & MAP_OPTIONS)|PTE_USER|PTE_PRESENT;
    }
    for(i=0;i < Count;i++)
    {
        //KeDebugPrintf("\nMAP %X %X %X",vAddr+(i<<12),pAddr,i);
        PTE[Index+i] = LODWORD(pAddr)|Entry;
        pAddr += 4096;
    }
    return STATUS_SUCCESS;
}*/


typedef struct _KRAM_INFO
{
    UINT64 Size;                      //Memory Size in Bytes
    UINT64 Size4k;                    //Memory Size in 4kb multiples
    UINT32 SizeMb;                    //Memory Size in Mega Bytes
    UINT32 Size4m;                    //Memory Size in 4mb multiples
    UINT32 *Bitmap;
}KRAM_INFO;

KRAM_INFO RamInfo;      


KESTATUS STDCALL KePmmInit()
{
    UINT32 i,j;
    PHYSICAL_PTR RamSize;
    UINT64 temp1,temp2;
    UINT32 RamPagesCount;
    UINT32 RamBitmapSize;
    PHYSICAL_PTR KernelHeap;
    UINT32 *RamBitmap;

	KernelHeap =  LoaderInfo->KernelPageData+LoaderInfo->KernelPageDataSize;
    
    KePagesMap(0,KernelHeap,(VIRTUAL_PTR)KERNEL_HEAP_BASE,KERNEL_HEAP_SIZE>>12,MAP_WRITE);
    KeHeapManagerInit((VIRTUAL_PTR)KERNEL_HEAP_BASE,KERNEL_HEAP_SIZE);
	
	KeDebugPrintf("\nMemoryMapCount = %i",LoaderInfo->MemoryMapCount);

    for(i=0;i<LoaderInfo->MemoryMapCount;i++)
    {
        temp1 = LoaderInfo->MemoryMaps[i].Start;
        temp2 = temp1 + LoaderInfo->MemoryMaps[i].Size;
        if(LoaderInfo->MemoryMaps[i].Type==1)
        RamSize = temp2;
        
        KeDebugPrintf("\nMemory %0#10x%08x - %0#10x%08x Type %i",HIDWORD(temp1),LODWORD(temp1),HIDWORD(temp2),LODWORD(temp2),LoaderInfo->MemoryMaps[i].Type);
    }

    RamPagesCount = RamSize>>12;

    KeDebugPrintf("\nRam Size = %08x",RamSize);
    KeDebugPrintf("\nRam Pages Count = %08x",RamPagesCount);
    
    RamBitmapSize = sizeof(KRAM_INFO) + (RamPagesCount>>3);

    KeDebugPrintf("\nRam Bitmap Size = %08x",RamBitmapSize);

    KeMemAlloc((VIRTUAL_PTR*)&RamBitmap,RamBitmapSize);
    
    RamInfo.Size = RamSize;
    RamInfo.Size4k = RamSize >> 12;
    RamInfo.SizeMb = RamSize >> 20;
    RamInfo.Size4m = RamSize >> 22;
    RamInfo.Bitmap = RamBitmap;
	
	memset(RamInfo.Bitmap,0xFF,RamSize>>15);

    for(i=0;i<LoaderInfo->MemoryMapCount;i++)
    {
        if(LoaderInfo->MemoryMaps[i].Type==MEM_RANGE_MEMORY)
        {
            KePmmSetState(LoaderInfo->MemoryMaps[i].Start,LoaderInfo->MemoryMaps[i].Size>>12, PMM_STATE_FREE);
            KeDebugPrintf("\nFree Memory %0#10x - %0#10x",(UINT32)LoaderInfo->MemoryMaps[i].Start,(UINT32)(LoaderInfo->MemoryMaps[i].Start+LoaderInfo->MemoryMaps[i].Size));
        }
    }

    KePmmSetState(KernelHeap,KERNEL_HEAP_SIZE>>12, PMM_STATE_USED);
    KePmmSetState(LoaderInfo->KernelAddress,(LoaderInfo->KernelSize)>>12, PMM_STATE_USED);
    KePmmSetState(LoaderInfo->KernelStack,(LoaderInfo->KernelStackSize)>>12, PMM_STATE_USED);
    KePmmSetState(LoaderInfo->KernelPageData,(LoaderInfo->KernelPageDataSize)>>12, PMM_STATE_USED);
    //KePmmSetState(LoaderInfo->KernelBSSData,(LoaderInfo->KernelBSSDataSize)>>12, PMM_STATE_USED);

    return STATUS_SUCCESS;
}

KESTATUS STDCALL KePmmCheckState(PHYSICAL_PTR pAddr)
{
    UINT32 i,j,k,mask;

    if(pAddr >= RamInfo.Size || pAddr & 0xFFF)
    {
        return STATUS_INVALID_ADDRESS;
    }

    i = pAddr>>17;
    mask = 1 << ((pAddr>>12) & 0x1F);

    if(RamInfo.Bitmap[i] & mask)
    {
        return STATUS_TRUE;
    }
    return STATUS_FALSE;
}

KESTATUS STDCALL KePmmSetState(PHYSICAL_PTR pAddr,UINT32 Count,UINT32 State)
{
	UINT32 i, j, k, m1, m2;
	if((pAddr+(Count<<12)) >= RamInfo.Size || pAddr & 0xFFF)
    {
        return STATUS_INVALID_ADDRESS;
    }

	i = pAddr>>17;
	m1 = 0xFFFFFFFF << ((pAddr >> 12) & 0x1F);
	j = ((pAddr >> 12) + Count) >> 5;
	m2 = ~(0xFFFFFFFF << (((pAddr >> 12) + Count) & 0x1F));

	//KeDebugPrintf("%X %X %X %X %X\n", i, m1, j, m2, State);

	if(i==j)
	{
		if(State==PMM_STATE_FREE)
			RamInfo.Bitmap[i] &= ~(m1 & m2);
		else
			RamInfo.Bitmap[i] |= (m1 & m2);
	}
	else
	{
		if(State==PMM_STATE_FREE)
		{
			RamInfo.Bitmap[i] &= ~m1;
			RamInfo.Bitmap[j] &= ~m2;
			for(k=i+1;k<j;k++) RamInfo.Bitmap[j] = 0;
		}
		else
		{
			RamInfo.Bitmap[i] |= m1;
			RamInfo.Bitmap[j] |= m2;
			for(k=i+1;k<j;k++) RamInfo.Bitmap[j] = 0xFFFFFFFF;
		}
	}
    return STATUS_SUCCESS;
}

KESTATUS STDCALL KePmmFindFreePages(UINT32 Count,UINT32 Options,PHYSICAL_PTR *pAddr)
{
	int i, m;
	if(Count==1)
	{
		for(i=0;i<RamInfo.Size>>17;i++)
		{
			//KeDebugPrintf("%08X", RamInfo.Bitmap[i]);
			if(_BitScanForward(&m, ~RamInfo.Bitmap[i]))
			{

				//KeDebugPrintf("i%X %X %Xi\n", i, m, (i<<17) + (m<<12));
				*pAddr = (i<<17) + (m<<12);
				return STATUS_SUCCESS;
			}
		}
	}
	//KeDebugPrintf("Error Free Pages");
    return STATUS_ERROR;
}

KESTATUS STDCALL KePmmAllocate(UINT32 Count,UINT32 Options,PHYSICAL_PTR *pAddr)
{
	PHYSICAL_PTR pptr;
	if(KESTATUS_SUCCESS(KePmmFindFreePages(Count, Options, &pptr)))
	{
		//KeDebugPrintf("%X\n", RamInfo.Bitmap[0]);
		KePmmSetState(pptr, Count, PMM_STATE_USED);
		//KeDebugPrintf("%X\n", RamInfo.Bitmap[0]);
		*pAddr = pptr;
		return STATUS_SUCCESS;
	}
    return STATUS_ERROR;
}

KESTATUS STDCALL KePmmFree(UINT32 Count,PHYSICAL_PTR pAddr)
{
	int i;
	for(i=0;i<Count;i++)
	{
		if(KePmmCheckState(pAddr + (i<<12))==STATUS_TRUE)
		{
			KePmmSetState(pAddr + (i<<12), 1, PMM_STATE_FREE);
		}
	}
    return STATUS_SUCCESS;
}

KESTATUS STDCALL KePageMap(PHYSICAL_PTR CR3, PHYSICAL_PTR pAddr, VIRTUAL_PTR vAddr, UINT32 Options)
{
    UINT32 *PTE;
    UINT32 Entry;
    UINT32 Index;
    if(CR3)
    {
        PTE = (UINT32*)0xFF800000;
    }
    else
    {
        PTE = (UINT32*)0xFFC00000;
    }
    Index = (UINT32)vAddr >> 12;
    if(vAddr >= (VIRTUAL_PTR)0x80000000)
    {
        Entry = (LODWORD(pAddr) & 0xFFFFF000)|(Options & MAP_OPTIONS)|PTE_GLOBAL|PTE_PRESENT;
    }
    else
    {
        Entry = (LODWORD(pAddr) & 0xFFFFF000)|(Options & MAP_OPTIONS)|PTE_USER|PTE_PRESENT;
    }
    PTE[Index] = Entry;
	//__asm invlpg [vAddr]
    return STATUS_SUCCESS;
}

KESTATUS STDCALL KePagesMap(PHYSICAL_PTR CR3, PHYSICAL_PTR pAddr,VIRTUAL_PTR vAddr,UINT32 Count, UINT32 Options)
{
    UINT32 *PTE;
    UINT32 Entry;
    UINT32 Index;
    UINT32 i;
    if(CR3)
    {
        PTE = (UINT32*)0xFF800000;
    }
    else
    {
        PTE = (UINT32*)0xFFC00000;
    }
    Index = (UINT32)vAddr >> 12;
    if(vAddr >= (VIRTUAL_PTR)0x80000000)
    {
        Entry = (Options & MAP_OPTIONS)|PTE_GLOBAL|PTE_PRESENT;
    }
    else
    {
        Entry = (Options & MAP_OPTIONS)|PTE_USER|PTE_PRESENT;
    }
    for(i=0;i < Count;i++)
    {
        //KeDebugPrintf("\nMAP %X %X %X",vAddr+(i<<12),pAddr,i);
        PTE[Index+i] = LODWORD(pAddr)|Entry;
        pAddr += 4096;
    }
    return STATUS_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////////
//--------------------------KernelPhysicalMemoryManager.c------------------------//
//--------------------------Copyright © Sidhant Goel, 2007-----------------------//
///////////////////////////////////////////////////////////////////////////////////
