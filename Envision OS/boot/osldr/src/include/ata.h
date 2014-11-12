////////////////////////////////////////////////////////////////////////////////
//-----------------------------------ata.h------------------------------------//
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
#ifndef __ata_h__
#define __ata_h__

#include <arch\arch.h>

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#pragma pack(push, 1)
typedef struct _AtaDevInfo
{
	UINT16 GenConf;
	UINT16 CylinderCount;
	UINT16 Word2;
	UINT16 HeadCount;
	UINT16 TrackSize;			//Retired
	UINT16 SectorSize;			//Retired
	UINT16 SectorsPerTrack;
	UINT16 Word7;				//Retired
	UINT16 Word8;				//Retired
	UINT16 Word9;				//Retired
	UINT16 SerialNumber[10];
	UINT16 Word20;				//Retired
	UINT16 Word21;				//Retired
	UINT16 Word22;				//Obsolete
	UINT16 FirmwareRev[4];
	UINT16 ModelNumber[20];
	UCHAR  MaxSectors;
	UCHAR  Word47_1;			//Retired;
	UINT16 Word48;				//Reserved;
	UINT32 Capabilities;
	UCHAR  Word51_0;			//Retired
	UCHAR  PioDataXferModeNum;
	UINT16 Word52;				//Retired;
	UINT16 Word53;
	UINT16 Word54;
	UINT16 Word55;
	UINT16 Word56;
	UINT32 NumSectors;
	UINT16 Word59;
	UINT16 Word60;
	UINT16 Word61;
	UINT16 Word62;
	UINT16 Word63;
	UINT16 Word64;
	UINT16 Word65;
	UINT16 Word66;
	UINT16 Word67;
	UINT16 Word68;
	UINT16 Word69;
	UINT16 Word70;
	UINT16 Word71;
	UINT16 Word72;
	UINT16 Word73;
	UINT16 Word74;
	UINT16 Word75;
	UINT16 Word76;
	UINT16 Word77;
	UINT16 Word78;
	UINT16 Word79;
	UINT16 MajorVer;
	UINT16 Word81;
	UINT16 Word82;
	UINT16 Word83;
	UINT16 Word84;
	UINT16 Word85;
	UINT16 Word86;
	UINT16 Word87;
	UINT16 Word88;
	UINT16 Word89;
	UINT16 Word90;
	UINT16 Word91;
	UINT16 Word92;
	UINT16 Word93;
	UINT16 Word94;
	UINT16 Word95;
	UINT16 Word96;
	UINT16 Word97;
	UINT16 Word98;
	UINT16 Word99;
	UINT16 Word100;
	UINT16 Word101;
	UINT16 Word102;
	UINT16 Word103;
	UINT16 Word104;
	UINT16 Word105;
	UINT16 Word106;
	UINT16 Word107;
	UINT16 Word108;
	UINT16 Word109;
	UINT16 Word110;
	UINT16 Word111;
	UINT16 Word112;
	UINT16 Word113;
	UINT16 Word114;
	UINT16 Word115;
	UINT16 Word116;
	UINT16 Word117;
	UINT16 Word118;
	UINT16 Word119;
	UINT16 Word120;
	UINT16 Word121;
	UINT16 Word122;
	UINT16 Word123;
	UINT16 Word124;
	UINT16 Word125;
	UINT16 Word126;
	UINT16 Word127;
	UINT16 Pad[128];
	//TODO: Complete AtaDevInfo
}AtaDevInfo;
/*
    ATA GenConf Field
F   15      0 = ATA device
F   14-8    Retired
F   7       1 = removable media device
F   6       1 = not removable controller and/or device
F   5-3     Retired
V   2       Response incomplete
F   1       Retired
F   0       Reserved
*/

typedef struct _AtapiDevInfo
{
	UINT16 GenConf;
	UINT16 Word1to9[9];			//Reserved
	UINT16 SerialNumber[10];
	UINT16 Word20;				//Retired
	UINT16 Word21;				//Retired
	UINT16 Word22;				//Obsolete
	UINT16 FirmwareRev[4];
	UINT16 ModelNumber[20];
	UINT16 Word47;				//Reserved;
	UINT16 Word48;				//Reserved;
	UINT16 Capabilities;
	UINT16 Word50;				//Reserved
	UCHAR  Vendor0;				//Vendor Specific
	UCHAR  PioDataXferModeNum;
	UINT16 Word52;				//Retired;
	UINT16 Word53;
	UINT16 Word54;
	UINT16 Word55;
	UINT16 Word56;
	UINT16 Word57;
	UINT16 Word58;
	UINT16 Word59;
	UINT16 Word60;
	UINT16 Word61;
	UINT16 Word62;
	UINT16 Word63;
	UINT16 Word64;
	UINT16 Word65;
	UINT16 Word66;
	UINT16 Word67;
	UINT16 Word68;
	UINT16 Word69;
	UINT16 Word70;
	UINT16 Word71;
	UINT16 Word72;
	UINT16 Word73;
	UINT16 Word74;
	UINT16 Word75;
	UINT16 Word76;
	UINT16 Word77;
	UINT16 Word78;
	UINT16 Word79;
	UINT16 MajorVer;
	UINT16 MinorVer;
	UINT16 Word82;
	UINT16 Word83;
	UINT16 Word84;
	UINT16 Word85;
	UINT16 Word86;
	UINT16 Word87;
	UINT16 Word88;
	UINT16 Word89;
	UINT16 Word90;
	UINT16 Word91;
	UINT16 Word92;
	UINT16 Word93;
	UINT16 Word94;
	UINT16 Word95;
	UINT16 Word96;
	UINT16 Word97;
	UINT16 Word98;
	UINT16 Word99;
	UINT16 Word100;
	UINT16 Word101;
	UINT16 Word102;
	UINT16 Word103;
	UINT16 Word104;
	UINT16 Word105;
	UINT16 Word106;
	UINT16 Word107;
	UINT16 Word108;
	UINT16 Word109;
	UINT16 Word110;
	UINT16 Word111;
	UINT16 Word112;
	UINT16 Word113;
	UINT16 Word114;
	UINT16 Word115;
	UINT16 Word116;
	UINT16 Word117;
	UINT16 Word118;
	UINT16 Word119;
	UINT16 Word120;
	UINT16 Word121;
	UINT16 Word122;
	UINT16 Word123;
	UINT16 Word124;
	UINT16 Word125;
	UINT16 PktByteCnt;
	UINT16 Word127;
	UINT16 Pad[128];
	//TODO: Complete AtapiDevInfo
}AtapiDevInfo;
#pragma pack(pop)

/* 
    ATAPI GenConf Field 
F   15-14   10 = ATAPI device
            11 = Reserved
R           13 Reserved
F   12-8    Field indicates command packet set used by device
F   7       1 = removable media device
F   6-5     00 = Device shall set DRQ to one within 3 ms of receiving PACKET command.
            01 = Obsolete.
            10 = Device shall set DRQ to one within 50 us of receiving PACKET command.
            11 = Reserved
R   4-3     Reserved
V   2       Incomplete response
F   1-0     00 = 12 byte command packet
            01 = 16 byte command packet
            1x = Reserved
*/

typedef struct _AtaDevice
{
    CHAR Desc[32];
	UINT32 Drive;
	UINT32 Flags;
	UINT16 Command;
	UINT16 Control;
	CHAR SerialNumber[20];
	CHAR FirmwareRevision[8];
	CHAR ModelNumber[40];
    UINT32 HeadCount;
    UINT32 CylinderCount;
    UINT32 SectorCount;
    UINT32 SectorsPerTrack;
    UINT32 MaxSectors;
    UINT32 SectorSize;
    UINT32 AtapiFlags;
    UINT32 LastSession;
}AtaDevice;

/* ATA Device Flags */
#define DEVICE_ATAPI	0x00000001
#define DEVICE_FIXED    0x00000002

/* CD Flags */
#define ATAPI_DOOROPEN      0x00000001
#define ATAPI_MEDIAPRESENT  0x00000002
#define ATAPI_STARTED       0x00000004
#define ATAPI_LOCKED        0x00000008

/* ATA Interrupt Hooks */
void AtaIntPrimary();
void AtaIntSecondary();

/* ATA Bus Functions */ 
UINT32 STDCALL AtaProbe();
UINT32 STDCALL AtaReadSectors(AtaDevice *Device, VPTR Buffer, SIZE_T SectNum, SIZE_T Count);
UINT32 STDCALL AtaWriteSectors(AtaDevice *Device, VPTR Buffer, SIZE_T SectNum, SIZE_T Count);
UINT32 STDCALL AtaReset(AtaDevice *Device);
UINT32 STDCALL AtapiReset(AtaDevice *Device);
UINT32 STDCALL AtapiStart(AtaDevice *Device);
UINT32 STDCALL AtapiStop(AtaDevice *Device);
UINT32 STDCALL AtapiLock(AtaDevice *Device);
UINT32 STDCALL AtapiUnlock(AtaDevice *Device);
UINT32 STDCALL AtapiEject(AtaDevice *Device);
UINT32 STDCALL AtapiClose(AtaDevice *Device);

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif //__ata_h__
////////////////////////////////////////////////////////////////////////////////
//-----------------------------------ata.h------------------------------------//
//-----------------------Copyright © Sidhant Goel, 2008-----------------------//
////////////////////////////////////////////////////////////////////////////////
