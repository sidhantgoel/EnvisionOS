////////////////////////////////////////////////////////////////////////////////
//-----------------------------------iso.h------------------------------------//
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

#ifndef __iso_h__
#define __iso_h__

#define ISO_VD_START                16
#define ISO_STD_ID                  "CD001"
#define ISO_VDT_BOOT                0
#define ISO_VDT_PRIMARY             1
#define ISO_VDT_SUPPLEMENTARY       2
#define ISO_VDT_PARTITION           3
#define ISO_VDT_TERMINATOR          255

#define ISO_FLAG_HIDDEN             0x01
#define ISO_FLAG_DIRECTORY          0x02
#define ISO_FLAG_ASSOCIATED         0x04
#define ISO_FLAG_RECORD             0x08
#define ISO_FLAG_PROTECTION         0x10
#define ISO_FLAG_MULTIEXTENT        0x80

#pragma pack(push, 1)

typedef struct _UINT32BE
{
    UINT32 LE;
    UINT32 BE;
}UINT32BE;

typedef struct _UINT16BE
{
    UINT16 LE;
    UINT16 BE;
}UINT16BE;

typedef struct _IsoDirRecord{
  UCHAR     RecordLength;
  UCHAR     ExtAttrLength;
  UINT32BE  BlockNumber;
  UINT32BE  Size;
  UCHAR     Date[7];
  UCHAR     Flags;
  UCHAR     UnitSize;
  UCHAR     IntrGapSize;
  UINT16BE  VolSeqNum;
  UCHAR     NameLength;
  CHAR      Name[1];
}IsoDirRecord;

typedef struct _IsoVolDesc
{
    UCHAR   Type;
    CHAR    StdId[5];
    UCHAR   Version;
    union
    {
        struct
        {
            UCHAR BootSysId[32];
            UCHAR BootId[32];
            UCHAR BootSysUse[1977];
        }Boot;
        struct
        {
            UCHAR Unused0;
            UCHAR SysId[32];
            UCHAR VolId[32];
            UCHAR Unused1[8];
            UINT32BE VolSpaceSize;
            UCHAR Unused2[32];
            UINT16BE VolSetSize;
            UINT16BE VolSeqNum;
            UINT16BE LogicalBlockSize;
            UINT32BE PathTableSize;
            UINT16BE PathTableL1;
            UINT16BE PathTableL2;
            UINT16BE PathTableM1;
            UINT16BE PathTableM2;
            IsoDirRecord RootDirRecord;
            CHAR VolSetId[128];
            CHAR PublisherId[128];
            CHAR PreparerId[128];
            CHAR ApplicationId[128];
            CHAR CopyrightFileId[37];
            CHAR AbstractFileId[37];
            CHAR BibloFileId[37];
            UCHAR CreationDateTime;
            UCHAR ModificationDateTime;
            UCHAR ExpirationDateTime;
            UCHAR EffectiveDateTime;
            UCHAR FileStructVer;
            UCHAR Unused3;
            UCHAR ApplicationUse[512];
            UCHAR Unused4[653];
        }Primary;
        struct
        {
            UCHAR VolFlags;
            UCHAR SysId[32];
            UCHAR VolId[32];
            UCHAR Unused1[8];
            UINT32BE VolSpaceSize;
            UCHAR EscapeSequences[32];
            UINT16BE VolSetSize;
            UINT16BE VolSeqNum;
            UINT16BE LogicalBlockSize;
            UINT32BE PathTableSize;
            UINT16BE PathTableL1;
            UINT16BE PathTableL2;
            UINT16BE PathTableM1;
            UINT16BE PathTableM2;
            IsoDirRecord RootDirRecord;
            CHAR VolSetId[128];
            CHAR PublisherId[128];
            CHAR PreparerId[128];
            CHAR ApplicationId[128];
            CHAR CopyrightFileId[37];
            CHAR AbstractFileId[37];
            CHAR BibloFileId[37];
            UCHAR CreationDateTime;
            UCHAR ModificationDateTime;
            UCHAR ExpirationDateTime;
            UCHAR EffectiveDateTime;
            UCHAR FileStructVer;
            UCHAR Unused3;
            UCHAR ApplicationUse[512];
            UCHAR Unused4[653];
        }Supplementary;
        struct
        {
            UCHAR Reserved[2041];
        }Partition;
        struct
        {
            UCHAR Reserved[2041];
        }Terminator;
    };
}IsoVolDesc;

typedef struct _IsoPathTableRecord
{
    UCHAR   NameLength;
    UCHAR   ExtAtrRecLength;
    UINT32  ExtentLocation;
    UINT16  ParentDirNum;
    CHAR    DirId[1];

}IsoPathTableRecord;

#pragma pack(pop)

#pragma pack(pop)

#endif //__iso_h__
////////////////////////////////////////////////////////////////////////////////
//-----------------------------------iso.h------------------------------------//
//-----------------------Copyright © Sidhant Goel, 2008-----------------------//
////////////////////////////////////////////////////////////////////////////////
