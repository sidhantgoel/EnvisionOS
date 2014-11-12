///////////////////////////////////////////////////////////////////////////////////
//------------------------------KernelMemoryManager.h----------------------------//
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
#ifndef KERNELVMM_H_INCLUDED
#define KERNELVMM_H_INCLUDED

#include <Kernel.h>
#include <KernelDebug.h>
#include <KernelImage.h>

/**********************************************************************************
 *      Kernel Virtual Memory Info Structure
 *
 *********************************************************************************/

//Memory State
#define MEM_COMMIT              0x0001000
#define MEM_RESERVE             0x0002000
#define MEM_FREE                0x0010000
//Memory Type
#define MEM_PRIVATE             0x0020000
#define MEM_MAPPED              0x0040000
#define MEM_IMAGE               0x1000000
//Page Protection
#define PAGE_NOACCESS           0x00000001
#define PAGE_READONLY           0x00000002
#define PAGE_READWRITE          0x00000004
#define PAGE_WRITECOPY          0x00000008
#define PAGE_EXECUTE            0x00000010
#define PAGE_EXECUTE_READ       0x00000020
#define PAGE_EXECUTE_READWRITE  0x00000040
#define PAGE_EXECUTE_WRITECOPY  0x00000080
#define MASK_PAGE_PROTECT       0x000000FF


/**********************************************************************************
 *      Kernel Physical Memory Info
 *
 *********************************************************************************/
#define PAGE_STATE_ALLOCATED    0x01
#define PAGE_STATE_FREE         0x00

#define MAP_NOCACHE             PTE_PCD
#define MAP_WRITE               PTE_READ_WRITE
#define MAP_OPTIONS             (MAP_NOCACHE|MAP_WRITE)

/**********************************************************************************
 *      Initialization Functions
 *
 *********************************************************************************/
KESTATUS STDCALL KeMemoryManagerInit();


/**********************************************************************************
 *      Kernel Physical Memory Management Functions
 *
 *********************************************************************************/
KESTATUS STDCALL KePmmInit();
KESTATUS STDCALL KePmmCheckState(PHYSICAL_PTR pAddr);
KESTATUS STDCALL KePmmSetState(PHYSICAL_PTR pAddr,UINT32 Count,UINT32 State);
KESTATUS STDCALL KePmmFindFreePages(UINT32 Count,UINT32 Options,PHYSICAL_PTR *pAddr);
KESTATUS STDCALL KePmmAllocate(UINT32 Count,UINT32 Options,PHYSICAL_PTR *pAddr);
KESTATUS STDCALL KePmmFree(UINT32 Count,PHYSICAL_PTR pAddr);

#define PMM_STATE_USED  0x1
#define PMM_STATE_FREE  0x0

KESTATUS STDCALL KePageMap(PHYSICAL_PTR CR3, PHYSICAL_PTR pAddr, VIRTUAL_PTR vAddr, UINT32 Options);
KESTATUS STDCALL KePagesMap(PHYSICAL_PTR CR3, PHYSICAL_PTR pAddr,VIRTUAL_PTR vAddr,UINT32 Count, UINT32 Options);

/**********************************************************************************
 *      Kernel Virtual Memory Management Functions
 *
 *********************************************************************************/
KESTATUS STDCALL KeVirtualMemoryManagerInit();
KESTATUS STDCALL KeVirtualAlloc(VIRTUAL_PTR pAddr, SIZE_T Size, UINT32 Options);
KESTATUS STDCALL KeIoSpaceMap(VIRTUAL_PTR *pAddr,PHYSICAL_PTR Addr,SIZE_T Size);


/**********************************************************************************
 *      Kernel Heap ManageMent Functions
 *
 *********************************************************************************/
KESTATUS STDCALL KeHeapManagerInit(VIRTUAL_PTR Base,SIZE_T Size);
KESTATUS STDCALL KeMemAlloc(VIRTUAL_PTR *var_ptr,UINT32 size);
KESTATUS STDCALL KeMemRealloc(VIRTUAL_PTR *var_ptr,UINT32 size);
KESTATUS STDCALL KeMemFree(VIRTUAL_PTR var_ptr);


/**********************************************************************************
 *      Kernel Page Fault Handler
 *
 *********************************************************************************/
KESTATUS STDCALL KePageFault(VIRTUAL_PTR addr);


/**********************************************************************************
 *      Global Variables
 *
 *********************************************************************************/
typedef struct _KMEMORY_INFO KMEMORY_INFO;
extern KMEMORY_INFO *KERNEL_MEMINFO;

#endif // KERNELVMM_H_INCLUDED
///////////////////////////////////////////////////////////////////////////////////
//------------------------------KernelMemoryManager.h----------------------------//
//--------------------------Copyright © Sidhant Goel, 2007-----------------------//
///////////////////////////////////////////////////////////////////////////////////
