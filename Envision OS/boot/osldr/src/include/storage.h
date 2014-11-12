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

#include <arch\arch.h>
#include <ata.h>

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

typedef UINT32 (STDCALL *Fp_DevRead)(VPTR Device, VPTR Buffer, SIZE_T SectNum, SIZE_T Count);
typedef UINT32 (STDCALL *Fp_DevGeom)(VPTR Device, UINT32 *nHeads, UINT32 *nCylinders, UINT32 *nSectors);
typedef UINT32 (STDCALL *Fp_DevSectSize)(VPTR Device, UINT32 *Size);
typedef UINT32 (STDCALL *Fp_DevSectCount)(VPTR Device, UINT32 *Count);
typedef UINT32 (STDCALL *Fp_MediaPresent)(VPTR Device);
typedef UINT32 (STDCALL *Fp_MediaEject)(VPTR Device);
typedef UINT32 (STDCALL *Fp_MediaClose)(VPTR Device);
typedef UINT32 (STDCALL *Fp_MediaLock)(VPTR Device);
typedef UINT32 (STDCALL *Fp_MediaUnlock)(VPTR Device);

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
UINT32 STDCALL StorageCreate(CHAR *Name, VPTR Device, UINT32 Type, DEV_FUNC *df);

UINT32 STDCALL DevRead(VPTR Device, VPTR Buffer, SIZE_T SectNum, SIZE_T Count);
UINT32 STDCALL DevGeom(VPTR Device, UINT32 *nHeads, UINT32 *nCylinders, UINT32 *nSectors);
UINT32 STDCALL DevSectSize(VPTR Device, UINT32 *Size);
UINT32 STDCALL DevSectCount(VPTR Device, UINT32 *Count);
UINT32 STDCALL MediaPresent(VPTR Device);
UINT32 STDCALL MediaEject(VPTR Device);
UINT32 STDCALL MediaClose(VPTR Device);
UINT32 STDCALL MediaLock(VPTR Device);
UINT32 STDCALL MediaUnlock(VPTR Device);

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
