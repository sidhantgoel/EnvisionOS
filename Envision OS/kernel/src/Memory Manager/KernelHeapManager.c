///////////////////////////////////////////////////////////////////////////////////
//------------------------------KernelHeapManager.c------------------------------//
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

typedef struct _KMEM_HEAP;

typedef struct _KMEM_BIN KMEM_BIN;
typedef struct _KMEM_BIN
{
    UINT32 Size;
    UINT32 PrevSize;
    KMEM_BIN *Next;
    KMEM_BIN *Prev;
}KMEM_BIN;

typedef struct _KMEM_CHUNK KMEM_CHUNK;

typedef struct _KMEM_CHUNK
{
    UINT32 Size;
    KMEM_CHUNK *prev;
    KMEM_CHUNK *next;
}KMEM_CHUNK;

typedef struct _KMEM_POOL
{
    UINT32 PoolSize;
    UINT32 MemFree;
    UINT32 MemReserved;
    KMEM_CHUNK *FirstChunk;
    //KMEM_CHUNK *FirstFree;
    //KMEM_CHUNK *FirstMedium;
    //KMEM_CHUNK *FirstLarge;
    //KMEM_CHUNK *FirstHuge
}KMEM_POOL;

#define CHUNK_ALLOCATED     0x01

KMEM_POOL *MEM_POOL;
KESTATUS STDCALL KeHeapManagerInit(VIRTUAL_PTR mem,UINT32 size)
{
	//__asm int 0xcc
    KeDebugPrintf("\nInitializing Kernel Allocator");
    KeDebugPrintf("\nMemory = %p",mem);
    KeDebugPrintf("\nSize = %p",size);
    MEM_POOL = (KMEM_POOL*)mem;
    MEM_POOL->PoolSize = size;
    MEM_POOL->MemFree = size - (sizeof(KMEM_POOL) + sizeof(KMEM_CHUNK));
    MEM_POOL->FirstChunk = (KMEM_CHUNK*)(mem + sizeof(KMEM_POOL));
    MEM_POOL->FirstChunk->Size = size - (sizeof(KMEM_POOL) + sizeof(KMEM_CHUNK));
    MEM_POOL->FirstChunk->prev = NULL;
    MEM_POOL->FirstChunk->next = NULL;
    //MEM_POOL->FirstChunk->more = NULL;
    //MEM_POOL->FirstFree = MEM_POOL->FirstChunk;
    //MEM_POOL->FirstFree->prev = NULL;
    //MEM_POOL->FirstFree->next = NULL;
    KeDebugPrintf("\nDone Initializing Kernel Allocator");
    return STATUS_SUCCESS;
}

void *malloc(unsigned int size)
{
	VIRTUAL_PTR vptr;
	if(KESTATUS_SUCCESS(KeMemAlloc(&vptr, size)))
	{
		return (void*)vptr;
	}
	else
	{
		return NULL;
	}
}

void free(void *mem)
{
	KeMemFree((VIRTUAL_PTR)mem);
}



KESTATUS STDCALL KeMemAlloc(VIRTUAL_PTR *var_ptr,UINT32 size)
{
    KMEM_CHUNK *MemChunk = MEM_POOL->FirstChunk;
    UINT32 temp;
		
	//KeDebugPrintf("Allocating Memory %d, %d\n", size, a);
    if(size & 0x3)
    {
        size = (size|0x3)+1;
    }
    if(size < 16)
    {
        size = 16;
    }
    while(MemChunk)
    {
		//KeDebugPrintf("MALLOC %X %X %X\n",MemChunk, MemChunk->Size, MemChunk->next);
		//if(size==144 && a==2) _hang();
        if(MemChunk->Size >= size)
        {
            if(MemChunk->prev) MemChunk->prev->next = MemChunk->next;
            if(MemChunk->next) MemChunk->next->prev = MemChunk->prev;
			if(MemChunk==MEM_POOL->FirstChunk) MEM_POOL->FirstChunk=NULL;

            if(MemChunk->Size-size >= (sizeof(KMEM_CHUNK)+16))
            {
                temp = MemChunk->Size-size-sizeof(KMEM_CHUNK);
                MemChunk->Size = size|CHUNK_ALLOCATED;
                *var_ptr = (VIRTUAL_PTR)(MemChunk+1);
                
				MemChunk = (KMEM_CHUNK*)((VIRTUAL_PTR)MemChunk + sizeof(KMEM_CHUNK) + size);
                MemChunk->Size = temp;
                MemChunk->next = MEM_POOL->FirstChunk;
                MemChunk->prev = NULL;
                MEM_POOL->FirstChunk = MemChunk;
				//KeDebugPrintf("MALLOC2 %X %X %X\n",MemChunk, MemChunk->Size, MemChunk->next);
                return STATUS_SUCCESS;
            }
            else
            {
                MemChunk->Size |= CHUNK_ALLOCATED;
                *var_ptr = (VIRTUAL_PTR)(MemChunk+1);
                return STATUS_SUCCESS;
            }
        }
        MemChunk = MemChunk->next;
    }
    KeDebugPrintf("\nMemory Allocation Falure");
    KeDebugPrintf("\nSize : %p",size);
    _hang();
    return STATUS_ERROR;
}

KESTATUS STDCALL KeMemRealloc(VIRTUAL_PTR *var_ptr,UINT32 size)
{
    return STATUS_ERROR;
}

KESTATUS STDCALL KeMemFree(VIRTUAL_PTR var_ptr)
{
    KMEM_CHUNK *MemChunk = (KMEM_CHUNK*)(var_ptr-sizeof(KMEM_CHUNK));
	//KeDebugPrintf("FREE %X %X %X\n", var_ptr, ((KMEM_CHUNK*)(var_ptr-sizeof(KMEM_CHUNK)))->Size, ((KMEM_CHUNK*)(var_ptr-sizeof(KMEM_CHUNK)))->next);
    MemChunk->Size ^= CHUNK_ALLOCATED;
    MemChunk->next = MEM_POOL->FirstChunk;
    MemChunk->prev = NULL;
    MEM_POOL->MemFree += MemChunk->Size;
    return STATUS_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////////
//------------------------------KernelHeapManager.c------------------------------//
//--------------------------Copyright © Sidhant Goel, 2007-----------------------//
///////////////////////////////////////////////////////////////////////////////////
