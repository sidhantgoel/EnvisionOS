////////////////////////////////////////////////////////////////////////////////
//---------------------------------storage.h----------------------------------//
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
#ifndef __storage_h__
#define __storage_h__

#include <kernel.h>
#include "ata.h"

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

typedef UINT32 (STDCALL *Fp_DevRead)(VIRTUAL_PTR Device, VIRTUAL_PTR Buffer, SIZE_T SectNum, SIZE_T Count);
typedef UINT32 (STDCALL *Fp_DevGeom)(VIRTUAL_PTR Device, UINT32 *nHeads, UINT32 *nCylinders, UINT32 *nSectors);
typedef UINT32 (STDCALL *Fp_DevSectSize)(VIRTUAL_PTR Device, UINT32 *Size);
typedef UINT32 (STDCALL *Fp_DevSectCount)(VIRTUAL_PTR Device, UINT32 *Count);
typedef UINT32 (STDCALL *Fp_MediaPresent)(VIRTUAL_PTR Device);
typedef UINT32 (STDCALL *Fp_MediaEject)(VIRTUAL_PTR Device);
typedef UINT32 (STDCALL *Fp_MediaClose)(VIRTUAL_PTR Device);
typedef UINT32 (STDCALL *Fp_MediaLock)(VIRTUAL_PTR Device);
typedef UINT32 (STDCALL *Fp_MediaUnlock)(VIRTUAL_PTR Device);

typedef struct _DEV_FUNC
{
    Fp_DevRead      DevRead;
    Fp_DevGeom      DevGeom;
    Fp_DevSectSize  DevSectSize;
    Fp_DevSectCount DevSectCount;
    Fp_MediaPresent MediaPresent;
    Fp_MediaEject   MediaEject;
    Fp_MediaClose   MediaClose;
    Fp_MediaLock    MediaLock;
    Fp_MediaUnlock  MediaUnlock;
}DEV_FUNC;

UINT32 STDCALL StorageProbe();
UINT32 STDCALL StorageCreate(CHAR *Name, VIRTUAL_PTR Device, UINT32 Type, DEV_FUNC *df);

UINT32 STDCALL DevRead(VIRTUAL_PTR Device, VIRTUAL_PTR Buffer, SIZE_T SectNum, SIZE_T Count);
UINT32 STDCALL DevGeom(VIRTUAL_PTR Device, UINT32 *nHeads, UINT32 *nCylinders, UINT32 *nSectors);
UINT32 STDCALL DevSectSize(VIRTUAL_PTR Device, UINT32 *Size);
UINT32 STDCALL DevSectCount(VIRTUAL_PTR Device, UINT32 *Count);
UINT32 STDCALL MediaPresent(VIRTUAL_PTR Device);
UINT32 STDCALL MediaEject(VIRTUAL_PTR Device);
UINT32 STDCALL MediaClose(VIRTUAL_PTR Device);
UINT32 STDCALL MediaLock(VIRTUAL_PTR Device);
UINT32 STDCALL MediaUnlock(VIRTUAL_PTR Device);

#define STORAGE_HDD     0x01
#define STORAGE_CDROM   0x02

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif //__storage_h__
////////////////////////////////////////////////////////////////////////////////
//---------------------------------storage.h----------------------------------//
//-----------------------Copyright © Sidhant Goel, 2008-----------------------//
////////////////////////////////////////////////////////////////////////////////
