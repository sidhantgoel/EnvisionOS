////////////////////////////////////////////////////////////////////////////////
//----------------------------------volume.c----------------------------------//
//                                   v 1.0                                    //
//                                                                            //
//                                                                            //
//                                                                            //
//                                                                            //
//                                                                            //
//                                                                            //
// Author: Sidhant Goel                                                       //
// Date Created: 15 July 2008                                                 //
//                                                                            //
//----------------------------------Notes-------------------------------------//
//                                                                            //
//                                                                            //
//                                                                            //
//                                                                            //
//                                                                            //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
#include "volume.h"
#include "storage.h"
#include "fat32.h"
#include "iso.h"

#define EFAILED			STATUS_ERROR
#define ESUCCESS		STATUS_SUCCESS
#define ENOTSUPPORTED	STATUS_NOTSUPPORTED
#define HeapAlloc		malloc
#define DbgPrintf		KeDebugPrintf

struct
{
    UCHAR ID;
    char *Desc;
} PartitionTypes[] = {
  { 0x01, "FAT12"},
  { 0x02, "XENIX root"},
  { 0x03, "XENIX /usr"},
  { 0x04, "FAT16 (small)"},
  { 0x05, "Extended"},
  { 0x06, "FAT16"},
  { 0x07, "NTFS or HPFS"},
  { 0x08, "OS/2 or AIX boot"},
  { 0x09, "AIX data"},
  { 0x0A, "OS/2 Boot Manager"},
  { 0x0B, "FAT32"},
  { 0x0C, "FAT32 (LBA)"},
  { 0x0E, "FAT16 (LBA)"},
  { 0x0F, "Extended (LBA)"},
  { 0x11, "Hidden FAT12"},
  { 0x12, "FAT diagnostic"},
  { 0x14, "Hidden FAT16 (small)"},
  { 0x16, "Hidden FAT16"},
  { 0x17, "Hidden HPFS or NTFS"},
  { 0x1B, "Hidden FAT32"},
  { 0x1C, "Hidden FAT32 (LBA)"},
  { 0x1E, "Hidden FAT16 (LBA)"},
  { 0x35, "JFS" },
  { 0x39, "Plan 9" },
  { 0x3C, "PartitionMagic" },
  { 0x3D, "Hidden Netware" },
  { 0x4D, "QNX4.x" },
  { 0x4D, "QNX4.x 2nd" },
  { 0x4D, "QNX4.x 3rd" },
  { 0x52, "CP/M" },
  { 0x63, "GNU HURD"},
  { 0x64, "Netware 2"},
  { 0x65, "Netware 3/4"},
  { 0x80, "Minix"},
  { 0x81, "Linux or Minix"},
  { 0x82, "Linux swap or Solaris"},
  { 0x83, "Linux"},
  { 0x84, "Hibernation"},
  { 0x85, "Linux extended"},
  { 0x86, "HPFS or NTFS mirrored"},
  { 0x87, "HPFS or NTFS mirrored"},
  { 0x8E, "Linux LVM"},
  { 0x93, "Hidden Linux"},
  { 0x9F, "BSD/OS"},
  { 0xA0, "Hibernation"},
  { 0xA1, "Hibernation"},
  { 0xA5, "BSD, NetBSD, FreeBSD"},
  { 0xA6, "OpenBSD"},
  { 0xA7, "NeXTSTEP"},
  { 0xA8, "Darwin UFS"},
  { 0xA9, "NetBSD"},
  { 0xAB, "OS-X boot"},
  { 0xB7, "BSDI"},
  { 0xB8, "BSDI swap"},
  { 0xBE, "Solaris boot"},
  { 0xC1, "DR-DOS FAT12"},
  { 0xC4, "DR-DOS FAT16 (small)"},
  { 0xC5, "DR-DOS Extended"},
  { 0xC6, "DR-DOS FAT16"},
  { 0xC7, "HPFS mirrored"},
  { 0xCB, "DR-DOS FAT32"},
  { 0xCC, "DR-DOS FAT32 (LBA)"},
  { 0xCE, "DR-DOS FAT16 (LBA)"},
  { 0xEB, "BeOS BFS"},
  { 0xEE, "EFI GPT protective"},
  { 0xF2, "DOS 3.3+ second"},
  { 0xFA, "Bochs"},
  { 0xFB, "VmWare"},
  { 0xFC, "VmWare swap"},
  { 0xFD, "Linux RAID"},
  { 0xFE, "NT hidden or Veritas VM"},
  { 0xFF, "Veritas VM"},
  { 0, "" }};

#pragma pack(push, 1)
typedef struct
{
    UCHAR Active;
    UCHAR StartCHS[3];
    UCHAR PartType;
    UCHAR EndCHS[3];
    UINT32 StartLBA;
    UINT32 SectCount;
}PartRecord;
#pragma pack(pop)

#define FS_FAT16        0x01
#define FS_FAT32        0x02
#define FS_CDROM		0x03
#define FS_NTFS         0x04
#define FS_EXT2         0x05
#define FS_UNKNOWN      0xFF

char FsTypesStr[][32] = {"", "FAT16", "FAT32", "CDROM", "NTFS", "EXT2", "UNKNOWN"};

typedef struct _Volume
{
    VIRTUAL_PTR Storage;
    UINT32 SectNum;
    UINT32 SectCount;
    UINT32 FsType;
}Volume;

#define MAX_VOLUMES     26

Volume *Volumes[MAX_VOLUMES];
UINT32 VolumesLoaded = 0;

UINT32 STDCALL VolInit()
{
	VolumesLoaded = 0;
	return ESUCCESS;
}

UINT32 STDCALL VolList()
{
	unsigned int i;
	DbgPrintf("========================================================================================\n");
	DbgPrintf("                                     Volumes                                            \n");
	DbgPrintf("========================================================================================\n");
	DbgPrintf("ID\t\tFile System\n");
	for(i=0;i<VolumesLoaded;i++)
	{
		DbgPrintf("%c\t\t%s\n", 'A'+i, FsTypesStr[Volumes[i]->FsType]);
	}
	DbgPrintf("========================================================================================\n\n");
	return STATUS_SUCCESS;
}

UINT32 STDCALL VolAddStorage(UINT32 Type, VIRTUAL_PTR Device)
{
    Volume *v;
    UCHAR Buffer[512];
    UINT32 e, i;
    PartRecord *pRec;
    if(Type == STORAGE_HDD)
    {
		//DbgPrintf("5");
        e=DevRead(Device, Buffer, 0, 1);
		//DbgPrintf("6");
        if(EFAIL(e)) return e;
        if((Buffer[511] != 0xAA) || (Buffer[510] != 0x55)) return EFAILED;
        pRec = (PartRecord*)(Buffer+0x1BE);
        for(i=0;i<4;i++)
        {
            switch(pRec[i].PartType)
            {
            case 0x00:
                break;
            case 0x0B:
            case 0x0C:
                v = (Volume*)HeapAlloc(sizeof(Volume));
				v->Storage = Device;
                v->SectNum = pRec[i].StartLBA;
                v->SectCount = pRec[i].SectCount;
                v->FsType = FS_FAT32;
                Volumes[VolumesLoaded] = v;
                VolumesLoaded++;
                //DbgPrintf("Fat32 Parition Found, StartLBA=%X, Sectors=%X\n", pRec[i].StartLBA, pRec[i].SectCount);
                break;
            default:
                DbgPrintf("Unknown Parition Found, StartLBA=%X, Sectors=%X\n", pRec[i].StartLBA, pRec[i].SectCount);
            }
        }
        return STATUS_SUCCESS;
    }
    else if(Type == STORAGE_CDROM)
    {
        v = (Volume*)HeapAlloc(sizeof(Volume));
        v->Storage = Device;
        v->SectNum = 0;
        v->SectCount = 0xFFFFFFFF;
        v->FsType = FS_CDROM;
        Volumes[VolumesLoaded] = v;
        VolumesLoaded++;
        return STATUS_SUCCESS;
    }
    else
    {
        return STATUS_NOTSUPPORTED;
    }
}

UINT32 STDCALL VolGetStorage(VIRTUAL_PTR Vol, VIRTUAL_PTR *Storage)
{
    Volume *v = (Volume*)Vol;
    if(v)
    {
        *Storage = v->Storage;
        return STATUS_SUCCESS;
    }
    return EFAILED;
}

UINT32 STDCALL VolReadSectors(VIRTUAL_PTR Vol, VIRTUAL_PTR Buffer, SIZE_T nSect, SIZE_T SectCnt)
{
    UINT32 e;
    Volume *v = (Volume*)Vol;
    if(v)
    {
        //DbgPrintf("VolReadSectors %X\n", nSect);
        e = DevRead(v->Storage, Buffer, v->SectNum+nSect, SectCnt);
        //DbgPrintf("VolReadSectors DONE\n");
        return e;
    }
    return EFAILED;
}

UINT32 STDCALL VolReadFile(char *Path, VIRTUAL_PTR Buffer)
{
    if((Path[0]<'A') || (Path[0]>('A'+VolumesLoaded))) return EFAILED;
    switch(Volumes[Path[0]-'A']->FsType)
    {
    case FS_FAT32:
        return Fat32ReadFile((VIRTUAL_PTR)Volumes[Path[0]-'A'], Path+2, Buffer);
        break;
    }
    //DbgPrintf("Type=%X\n", Volumes[Path[0]-'A']->FsType);
    return ENOTSUPPORTED;
}

UINT32 STDCALL VolFileSize(char *Path, UINT32 *FileSize)
{
    if((Path[0]<'A') || (Path[0]>('A'+VolumesLoaded))) return EFAILED;
    switch(Volumes[Path[0]-'A']->FsType)
    {
    case FS_FAT32:
        return Fat32FileSize((VIRTUAL_PTR)Volumes[Path[0]-'A'], Path+2, FileSize);
        break;
    }
    return ENOTSUPPORTED;
}





////////////////////////////////////////////////////////////////////////////////
//----------------------------------volume.c----------------------------------//
//-----------------------Copyright © Sidhant Goel, 2008-----------------------//
////////////////////////////////////////////////////////////////////////////////