////////////////////////////////////////////////////////////////////////////////
//---------------------------------storage.c----------------------------------//
//                                   v 1.0                                    //
//                                                                            //
//                                                                            //
//                                                                            //
//                                                                            //
//                                                                            //
//                                                                            //
// Author: Sidhant Goel                                                       //
// Date Created: 08 July 2008                                                 //
//                                                                            //
//----------------------------------Notes-------------------------------------//
//                                                                            //
//                                                                            //
//                                                                            //
//                                                                            //
//                                                                            //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
#include <string.h>
//#include <debug.h>
//#include <status.h>
#include "storage.h"
#include "volume.h"
//#include <heap.h>

#define VPTR VIRTUAL_PTR
#define EFAILED			STATUS_ERROR
#define ESUCCESS		STATUS_SUCCESS
#define ENOTSUPPORTED	STATUS_NOTSUPPORTED
#define HeapAlloc		malloc
#define DbgPrintf		KeDebugPrintf

typedef struct _STORAGE_NODE STORAGE_NODE;

typedef struct _STORAGE_NODE
{
    char            Name[32];
    VIRTUAL_PTR     Device;
    DEV_FUNC        *DevFunc;
    UINT32          Type;
    STORAGE_NODE    *Next;
}STORAGE_NODE;

STORAGE_NODE *FirstNode;

UINT32 STDCALL StorageProbe()
{
    FirstNode = NULL;
    return AtaProbe();
}

UCHAR b[512];

UINT32 STDCALL StorageCreate(CHAR *Name, VIRTUAL_PTR Device, UINT32 Type, DEV_FUNC *df)
{
    STORAGE_NODE *Node = (STORAGE_NODE*)HeapAlloc(sizeof(STORAGE_NODE));
	//DbgPrintf("2");
    if(!Node) return EFAILED;
    strcpy(Node->Name, Name);
    Node->Device = Device;
    Node->Type = Type;
    Node->DevFunc = df;
    Node->Next = FirstNode;
    FirstNode = Node;
	//DbgPrintf("3");
    VolAddStorage(Type, (VIRTUAL_PTR)Node);
	//bgPrintf("4");
    return ESUCCESS;
}

UINT32 STDCALL DevRead(VIRTUAL_PTR Device, VIRTUAL_PTR Buffer, SIZE_T SectNum, SIZE_T Count)
{
    //DbgPrintf("DevRead %X\n", Device);
    if(Device)
    {
        //DbgPrintf("DevRead2 %X\n", Device);
        if(((STORAGE_NODE*)Device)->DevFunc->DevRead)
        {
            //DbgPrintf("DevRead3 %X\n", Device);
            return (*((STORAGE_NODE*)Device)->DevFunc->DevRead)(((STORAGE_NODE*)Device)->Device, Buffer, SectNum, Count);
        }
        else
        {
            return ENOTSUPPORTED;
        }
    }
    else
    {
        return EFAILED;
    }
}

UINT32 STDCALL DevGeom(VPTR Device, UINT32 *nHeads, UINT32 *nCylinders, UINT32 *nSectors)
{
    if(Device)
    {
        if(((STORAGE_NODE*)Device)->DevFunc->DevGeom)
        {
            return (*((STORAGE_NODE*)Device)->DevFunc->DevGeom)(((STORAGE_NODE*)Device)->Device, nHeads, nCylinders, nSectors);
        }
        else
        {
            return ENOTSUPPORTED;
        }
    }
    else
    {
        return EFAILED;
    }
}

UINT32 STDCALL DevSectSize(VPTR Device, UINT32 *Size)
{
    if(Device)
    {
        if(((STORAGE_NODE*)Device)->DevFunc->DevSectSize)
        {
            return (*((STORAGE_NODE*)Device)->DevFunc->DevSectSize)(((STORAGE_NODE*)Device)->Device, Size);
        }
        else
        {
            return ENOTSUPPORTED;
        }
    }
    else
    {
        return EFAILED;
    }
}

UINT32 STDCALL DevSectCount(VPTR Device, UINT32 *Count)
{
    if(Device)
    {
        if(((STORAGE_NODE*)Device)->DevFunc->DevSectCount)
        {
            return (*((STORAGE_NODE*)Device)->DevFunc->DevSectCount)(((STORAGE_NODE*)Device)->Device, Count);
        }
        else
        {
            return ENOTSUPPORTED;
        }
    }
    else
    {
        return EFAILED;
    }
}


UINT32 STDCALL MediaPresent(VPTR Device)
{
    if(Device)
    {
        if(((STORAGE_NODE*)Device)->DevFunc->MediaPresent)
        {
            return (*((STORAGE_NODE*)Device)->DevFunc->MediaPresent)(((STORAGE_NODE*)Device)->Device);
        }
        else
        {
            return ENOTSUPPORTED;
        }
    }
    else
    {
        return EFAILED;
    }
}

UINT32 STDCALL MediaEject(VPTR Device)
{
    if(Device)
    {
        if(((STORAGE_NODE*)Device)->DevFunc->MediaEject)
        {
            return (*((STORAGE_NODE*)Device)->DevFunc->MediaEject)(((STORAGE_NODE*)Device)->Device);
        }
        else
        {
            return ENOTSUPPORTED;
        }
    }
    else
    {
        return EFAILED;
    }
}

UINT32 STDCALL MediaClose(VPTR Device)
{
    if(Device)
    {
        if(((STORAGE_NODE*)Device)->DevFunc->MediaClose)
        {
            return (*((STORAGE_NODE*)Device)->DevFunc->MediaClose)(((STORAGE_NODE*)Device)->Device);
        }
        else
        {
            return ENOTSUPPORTED;
        }
    }
    else
    {
        return EFAILED;
    }
}

UINT32 STDCALL MediaLock(VPTR Device)
{
    if(Device)
    {
        if(((STORAGE_NODE*)Device)->DevFunc->MediaLock)
        {
            return (*((STORAGE_NODE*)Device)->DevFunc->MediaLock)(((STORAGE_NODE*)Device)->Device);
        }
        else
        {
            return ENOTSUPPORTED;
        }
    }
    else
    {
        return EFAILED;
    }
}

UINT32 STDCALL MediaUnlock(VPTR Device)
{
    if(Device)
    {
        if(((STORAGE_NODE*)Device)->DevFunc->MediaUnlock)
        {
            return (*((STORAGE_NODE*)Device)->DevFunc->MediaUnlock)(((STORAGE_NODE*)Device)->Device);
        }
        else
        {
            return ENOTSUPPORTED;
        }
    }
    else
    {
        return EFAILED;
    }
}

////////////////////////////////////////////////////////////////////////////////
//---------------------------------storage.c----------------------------------//
//-----------------------Copyright © Sidhant Goel, 2008-----------------------//
////////////////////////////////////////////////////////////////////////////////

