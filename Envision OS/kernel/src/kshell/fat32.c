////////////////////////////////////////////////////////////////////////////////
//-----------------------------------fat32.c----------------------------------//
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
#include <kernel.h>
#include "fat32.h"
#include "fs\fat.h"
#include "volume.h"
//#include <status.h>
//#include <heap.h>
//#include <debug.h>

#define VPTR VIRTUAL_PTR
#define EFAILED			STATUS_ERROR
#define ESUCCESS		STATUS_SUCCESS
#define ENOTSUPPORTED	STATUS_NOTSUPPORTED
#define HeapAlloc		malloc
#define HeapFree		free

VIRTUAL_PTR v;
FAT32_BOOTSECTOR BootSect;
UINT32 FatSect[128];
UINT32 FatSector;
UINT32 DataSector;
UINT32 FatLoaded;
UINT32 MinClus, MaxClus;

UINT32 ReadFat32Info()
{
    UINT32 e;
    e = VolReadSectors(v, (VIRTUAL_PTR)&BootSect, 0, 1);
    if(EFAIL(e)) return e;
    FatSector = BootSect.ReservedSectors;
    DataSector = FatSector + BootSect.NumberOfFats*BootSect.SectorsPerFatBig;
    e = VolReadSectors(v, (VIRTUAL_PTR)FatSect, FatSector, 1);
    if(EFAIL(e)) return e;
    FatLoaded = 0;
    MinClus = 2;
    MaxClus = 1 + (BootSect.TotalSectorsBig-BootSect.ReservedSectors-(BootSect.NumberOfFats*BootSect.SectorsPerFatBig))/BootSect.SectorsPerCluster;
    //DbgPrintf("Fat32Info: FatSect=%X, DataSect=%X, MinClus=%X, MaxClus=%X\n", FatSector, DataSector, MinClus, MaxClus);
    return ESUCCESS;
}

UINT32 NextFatEntry(UINT32 Num, UINT32 *Ent)
{
    UINT32 e;
    if(FatLoaded!=(Num/128))
    {
        e = VolReadSectors(v, (VIRTUAL_PTR)FatSect, FatSector+(Num/128), 1);
        if(EFAIL(e)) return e;
        FatLoaded = (Num/128);
    }
    *Ent = FatSect[Num%128];
    return ESUCCESS;
}

UINT32 ReadCluster(VIRTUAL_PTR Buffer, UINT32 Num)
{
    return VolReadSectors(v, Buffer, DataSector+((Num-2)*BootSect.SectorsPerCluster), BootSect.SectorsPerCluster);
}

UINT32 ReadFile(UINT32 Clus, VPTR Buffer, UINT32 FileSize)
{
    UINT32 e, i=0;
    VPTR ClusBuff = HeapAlloc(BootSect.SectorsPerCluster*512);
    while((Clus>=MinClus) && (Clus<=MaxClus))
    {
        e = ReadCluster(ClusBuff, Clus);
        if(EFAIL(e)) return e;
        e = NextFatEntry(Clus, &Clus);
        if(EFAIL(e)) return e;
        i++;
        if(i*BootSect.SectorsPerCluster*512>FileSize)
        {
            memcpy(Buffer, ClusBuff, FileSize-(i-1)*BootSect.SectorsPerCluster*512);
            HeapFree(ClusBuff);
            return ESUCCESS;
        }
		memcpy(Buffer, ClusBuff, BootSect.SectorsPerCluster*512);
        Buffer += BootSect.SectorsPerCluster*512;
    }
    HeapFree(ClusBuff);
    if(i*BootSect.SectorsPerCluster*512==FileSize)
    {
        return ESUCCESS;
    }
    else
    {
        return EFAILED;
    }
}

UINT32 FindFile(UINT32 Clus, char *Name, UINT32 *FileSize, UINT32 *FileClus)
{
    UINT32 e, i;
    DIRENTRY *Buff = (DIRENTRY*)HeapAlloc(BootSect.SectorsPerCluster*512);
    while((Clus>=MinClus) && (Clus<=MaxClus))
    {
        e = ReadCluster((VIRTUAL_PTR)Buff, Clus);
        if(EFAIL(e)) return e;
        e = NextFatEntry(Clus, &Clus);
        if(EFAIL(e)) return e;
        for(i=0;i<(BootSect.SectorsPerCluster*512/sizeof(DIRENTRY));i++)
        {
            if((Buff[i].FileName[0]!=0)&&(Buff[i].FileName[0]!=0xE5))
            {
                if(Buff[i].Attr!=ATTR_LONG_NAME)
                {
                    if(strnicmp(Buff[i].FileName, Name, 11)==0)
                    {
                        *FileSize = Buff[i].Size;
                        ((UINT16*)FileClus)[0] = Buff[i].ClusterLow;
                        ((UINT16*)FileClus)[1] = Buff[i].ClusterHigh;
                        HeapFree(Buff);
                        return ESUCCESS;
                    }
                }
            }
        }
    }
    HeapFree(Buff);
    return EFAILED;
}

UINT32 ValidatePath(char *Path)
{
    UINT32 i;
    if(Path[0]!='\\') return EFAILED;
    Path++;
    while(1)
    {
        i=0;
        while(Path[0]!='\\' && Path[0]!='\0' && Path[0]!='.')
        {
            Path++;
            i++;
        }
        if(i>8) return EFAILED;
        i=0;
        if(Path[0]=='.')
        {
            Path++;
            while(Path[0]!='\\' && Path[0]!='\0')
            {
                Path++;
                i++;
            }
            if(i>3) return EFAILED;
        }
        if(Path[0]=='\\')
        {
            continue;
        }
        if(Path[0]=='\0')
        {
            return ESUCCESS;
        }
    }
}

UINT32 ExtractName(char **Path, char *Name)
{
    UINT32 i=0;
    (*Path)++;
    while((*Path)[0]!='\\' && (*Path)[0]!='\0' && (*Path)[0]!='.')
    {
        Name[0] = (*Path)[0];
        Name++;
        (*Path)++;
        i++;
    }
    while(i<8)
    {
        Name[0] = ' ';
        Name++;
        i++;
    }
    if((*Path)[0]=='.')
    {
        (*Path)++;
        memcpy(Name, (*Path), 3);
        (*Path) += 3;
    }
    else
    {
        memset(Name, ' ', 3);
    }
    return ESUCCESS;
}

UINT32 STDCALL Fat32ReadFile(VPTR Vol, char *Path, VPTR Buffer)
{
    UINT32 e;
    UINT32 Dir, Size;
    char Name[11];
    v = Vol;
    e = ValidatePath(Path);
    if(EFAIL(e)) return e;
    e = ReadFat32Info();
    if(EFAIL(e)) return e;
    Dir = BootSect.RootDirStartCluster;
    while(1)
    {
        ExtractName(&Path, Name);
        e = FindFile(Dir, Name, &Size, &Dir);
        if(EFAIL(e)) return e;
        if(Path[0]=='\0')
        {
            e = ReadFile(Dir, Buffer, Size);
            return e;
        }
    }
}

UINT32 STDCALL Fat32FileSize(VPTR Vol, char *Path, UINT32 *FileSize)
{
    UINT32 e;
    UINT32 Dir, Size;
    char Name[11];
    v = Vol;
    e = ValidatePath(Path);
    if(EFAIL(e)) return e;
    e = ReadFat32Info();
    if(EFAIL(e)) return e;
    Dir = BootSect.RootDirStartCluster;
    while(1)
    {
        ExtractName(&Path, Name);
        e = FindFile(Dir, Name, &Size, &Dir);
        if(EFAIL(e)) return e;
        if(Path[0]=='\0')
        {
            *FileSize = Size;
            return ESUCCESS;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
//-----------------------------------fat32.c----------------------------------//
//-----------------------Copyright © Sidhant Goel, 2008-----------------------//
////////////////////////////////////////////////////////////////////////////////