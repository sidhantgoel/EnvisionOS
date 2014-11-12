////////////////////////////////////////////////////////////////////////////////
//-----------------------------------ata.c------------------------------------//
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
//#include <status.h>
#include <KernelTasks.h>
#include "ata.h"
//#include <debug.h>
#include "storage.h"
//#include <heap.h>

#define EFAILED			STATUS_ERROR
#define ESUCCESS		STATUS_SUCCESS
#define ENOTSUPPORTED	STATUS_NOTSUPPORTED
#define EWRONGDEVICE	STATUS_WRONGDEVICE
#define ETIMEOUT		STATUS_TIMEOUT
#define ENOTDETECTED	STATUS_NOTDETECTED
#define EDETECTED		STATUS_DETECTED
#define EATA			STATUS_ATA
#define EATAPI			STATUS_ATAPI
#define ENOMEDIA		STATUS_NOMEDIA
#define EPRESENT		STATUS_PRESENT
#define ENOTPRESENT		STATUS_NOTPRESENT
#define VPTR			VIRTUAL_PTR
#define HeapAlloc		malloc
#define outb(prt, val)          _outp(prt, val)
#define inb(prt)                ((unsigned char)_inp(prt))
#define outw(prt, val)          _outpw(prt, val)
#define inw(prt)                ((unsigned short)_inpw(prt))
#define getRetAddr              _ReturnAddress
#define outbs					__outbytestring
#define outws					__outwordstring
#define inbs					__inbytestring
#define inws					__inwordstring
#define DbgPrintf		KeDebugPrintf
#define DelayMS(x)		outb(0xEB,0);outb(0xEB,0);outb(0xEB,0);outb(0xEB,0);outb(0xEB,0);outb(0xEB,0);outb(0xEB,0);outb(0xEB,0);outb(0xEB,0);outb(0xEB,0);outb(0xEB,0);outb(0xEB,0);

#define IDE0_COMMAND	0x01F0
#define IDE0_CONTROL	0x03F0
#define IDE1_COMMAND	0x0170
#define IDE1_CONTROL	0x0370

USHORT AtaRegs[2][2] =
	{
        {IDE0_COMMAND,IDE0_CONTROL},
        {IDE1_COMMAND,IDE1_CONTROL}
    };

/* Command Block Registers			*/
/* Read								*/
#define IDE_DATA				0 
#define IDE_ERROR				1 
#define IDE_SECTORCOUNT         2
#define IDE_INTREASON           2
#define IDE_SECTORNUMBER		3 
#define IDE_CYLINDERLOW         4 
#define IDE_CYLINDERHIGH		5 
#define IDE_DRIVEHEAD			6 
#define IDE_STATUS				7
/* Write							*/
#define IDE_FEAT                1
#define IDE_PRECOMP				1
#define IDE_COMMAND				7
/* Control Block Registers			*/
/* Read								*/
#define IDE_ALT_STATUS			6
/* Write							*/
#define IDE_CONTROL				6

/* Interrupt Reason Flags           */
#define REASON_CD               0x01 /* Command=1, Data=0 */
#define REASON_IO               0x02 /* Input=1, Output=0 */
#define REASON_REL              0x04
#define REASON_TAG              0xF8

/* Error Flags                      */
#define ERROR_AMNF				0x01
#define ERROR_TK0NF				0x02
#define ERROR_ABRT				0x04
#define ERROR_MCR				0x08
#define ERROR_IDNF				0x10
#define ERROR_MC				0x20
#define ERROR_UNC				0x40
#define ERROR_BBK				0x80
#define ERROR_ILI               0x01
#define ERROR_EOM               0x02
#define ERROR_ABRT              0x04
#define ERROR_SENSEKEY          0xF0

/* Status Flags                     */
#define ASTATUS_ERROR			0x01
#define ASTATUS_INDEX			0x02
#define ASTATUS_CORR			0x04
#define ASTATUS_DRQ				0x08
#define ASTATUS_DSC				0x10
#define ASTATUS_DWF				0x20
#define ASTATUS_DRDY			0x40
#define ASTATUS_BUSY			0x80
#define ASTATUS_CHK             0x01
#define ASTATUS_SERV            0x10
#define ASTATUS_DMRD            0x20

/* Control Flags                    */
#define CONTROL_IEN				0x02
#define CONTROL_SRST			0x04

/* ATA Commands                     */
#define ATA_NOP                 0x00    //NOP
#define ATA_CFAREQEXTERRCODE    0x03    //CFA REQUEST EXTENDED ERROR CODE
#define ATA_DEVICERESET         0x08    //DEVICE RESET
#define ATA_RECALIBRATE         0x10    //RECALIBRATW
#define ATA_READSECTS_RET       0x20    //READ SECTOR(S)
#define ATA_READSECTS           0x21
#define ATA_READECC_RET         0x22
#define ATA_READECC             0x23
#define ATA_WRITESECTS_RET      0x30    //WRITE SECTOR(S)
#define ATA_WRITESECTS          0x31
#define ATA_WRITEECC_RET        0x32
#define ATA_WRITEECC            0x33
#define ATA_CFAWSWE             0x38    //CFA WRITE SECTORS WITHOUT ERASE
#define ATA_READVERIFYSECTORS   0x40    //READ VERIFY SECTOR(S)
#define ATA_VERIFYMULT          0x41
#define ATA_FORMATTRACK         0x50    //FORMAT TRACK
#define ATA_SEEK                0x70    //SEEK
#define ATA_CFATRANSLATESECT    0x87    //CFA TRANSLATE SECTOR
#define ATA_EXECDEVDIAG         0x90    //EXECUTE DEVICE DIAGNOSTIC
#define ATA_INITDEVPARAMS       0x91    //INNITIALIZE DEVICE PARAMETERS
#define ATA_DOWNLOADMICROCODE   0x92    //DOWNLOAD MICROCODE
#define ATA_PACKET              0xA0    //PACKET
#define ATA_IDENTIFYPACKETDEV   0xA1    //IDENTIFY PACKET DEVICE
#define ATA_SERVICE             0xA2    //SERVICE
#define ATA_SMART               0xB0    //SMART ______
#define ATA_CFAERASESECTORS     0xC0    //CFA ERASE SECTORS
#define ATA_READMULTIPLE        0xC4    //READ MULTIPLE
#define ATA_WRITEMULTIPLE       0xC5    //WRITE MULTIPLE
#define ATA_SETMULTIMODE        0xC6    //SET MULTIPLE MODE
#define ATA_READDMAQUEUED       0xC7    //READ DMA QUEUED
#define ATA_READDMA             0xC8    //READ DMA
#define ATA_WRITEDMA            0xCA    //WRITE DMA
#define ATA_WRITEDMAQUEUED      0xCC    //WRITE DMA QUEUED
#define ATA_CFAWMWE             0xCD    //WRITE MULTIPLE WITHOUT ERASE
#define ATA_GETMEADIASTATUS     0xDA    //GET MEDIA STATUS
#define ATA_MEDIALOCK           0xDE    //MEDIA LOCK
#define ATA_MEDIAUNLOCK         0xDF    //MEDIA UNLOCK
#define ATA_STANDBYIMMEDIATE    0xE0    //STANDBY IMMEDIATE
#define ATA_IDLEIMMEDIATE       0xE1    //IDLE IMMEDIATE
#define ATA_STANDBY             0xE2    //STANDBY
#define ATA_IDLE                0xE3    //IDLE
#define ATA_READBUFFER          0xE4    //READ BUFFER
#define ATA_CHECKPOWERMODE      0xE5    //CHECK POWER MODE
#define ATA_SLEEP               0xE6    //SLEEP
#define ATA_FLUSHCACHE          0xE7    //FLUSH CACHE
#define ATA_WRITEBUFFER         0xE8    //WRITE BUFFER
#define ATA_IDENTIFYDEVICE      0xEC    //IDENTIFY DEVICE
#define ATA_MEDIAEJECT          0xED    //MEDIA EJECT
#define ATA_SETFEAT             0xEF    //SET FEATURES
#define ATA_SECSETPWD           0xF1    //SECURITY SET PASSWORD
#define ATA_SECUNLOCK           0xF2    //SECURITY UNLOCK
#define ATA_SECERASEPREP        0xF3    //SECURITY ERASE PREPARE
#define ATA_SECERASEUNIT        0xF4    //SECURITY ERASE UNIT
#define ATA_SECFREEZELOCK       0xF5    //SECURITY FREEZE LOCK
#define ATA_SECDISABLEPWD       0xF6    //SECURITY DISABLE PASSWORD
#define ATA_READNATIVEMAXADDR   0xF8    //READ NATIVE MAX ADDRESS
#define ATA_SETMAX              0xF9    //SET MAX _____

/* ATAPI Commands                   */
#define ATAPI_TESTREADY      0x00
#define ATAPI_REQESTSENSE    0x03
#define ATAPI_INQUIRY        0x12
#define ATAPI_STARTSTOP      0x1B
#define ATAPI_PERMITREMOVAL  0x1E
#define ATAPI_READCAPACITY   0x25
#define ATAPI_READ10         0x28
#define ATAPI_SEEK           0x2B
#define ATAPI_READSUBCHAN    0x42
#define ATAPI_READTOC        0x43
#define ATAPI_READHEADER     0x44
#define ATAPI_PLAYAUDIO      0x45
#define ATAPI_PLAYAUDIOMSF   0x47
#define ATAPI_PAUSERESUME    0x4B
#define ATAPI_STOPPLAYSCAN   0x4E
#define ATAPI_MODESELECT     0x55
#define ATAPI_MODESENSE      0x5A
#define ATAPI_LOADUNLOAD     0xA6
#define ATAPI_READ12         0xA8
#define ATAPI_SCAN           0xBA
#define ATAPI_SETCDSPEED     0xBB
#define ATAPI_PLAYCD         0xBC
#define ATAPI_MECHSTATUS     0xBD
#define ATAPI_READCD         0xBE
#define ATAPI_READCDMSF      0xB9

/* ATAPI Packets                    */
UCHAR ATAPI_PACKET_UNLOCK[] =		{ATAPI_PERMITREMOVAL,	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
UCHAR ATAPI_PACKET_LOCK[] =			{ATAPI_PERMITREMOVAL,	0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0};
UCHAR ATAPI_PACKET_STOP[] =			{ATAPI_STARTSTOP,		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
UCHAR ATAPI_PACKET_START[] =		{ATAPI_STARTSTOP,		0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0};
UCHAR ATAPI_PACKET_EJECT[] =		{ATAPI_STARTSTOP,		0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0};
UCHAR ATAPI_PACKET_CLOSE[] =		{ATAPI_STARTSTOP,		0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0};
UCHAR ATAPI_PACKET_READCAPACITY[] = {ATAPI_READCAPACITY,	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
UCHAR ATAPI_PACKET_READTOC[] =		{ATAPI_READTOC,			0, 1, 0, 0, 0, 0, 0, 12, 0x40, 0, 0};

/* ATA Delay Macro                  */
#define AtaDelay() outb(0xEB,0);outb(0xEB,0);outb(0xEB,0);outb(0xEB,0);

#define WaitDrvRdy(Base, Timeout)       AtaWaitStatus(Base, ASTATUS_DRDY|ASTATUS_BUSY, ASTATUS_DRDY, Timeout);
#define WaitReadyDrqOff(Base, Timeout)  AtaWaitStatus(Base, ASTATUS_BUSY|ASTATUS_DRQ, 0, Timeout)
#define WaitReadyDrq(Base, Timeout)     AtaWaitStatus(Base, ASTATUS_BUSY|ASTATUS_DRQ, ASTATUS_DRQ, Timeout)
#define WaitReady(Base, Timeout)        AtaWaitStatus(Base, ASTATUS_BUSY, 0, Timeout)
#define WaitDrq(Base, Timeout)          AtaWaitStatus(Base, ASTATUS_DRQ, ASTATUS_DRQ, Timeout)
#define WaitDrqOff(Base, Timeout)       AtaWaitStatus(Base, ASTATUS_DRQ, 0, Timeout)


UINT32 STDCALL AtaWaitStatus(UINT32 Base, UINT32 _Mask, UINT32 _Value, UINT32 Timeout)
{
    UINT8 i;
    while(Timeout--)
    {
        i = inb(Base + IDE_STATUS);
        if((i & _Mask) == _Value) return ESUCCESS;
    }
    //DbgPrintf("%X\n", inb(Base+IDE_STATUS));
    //DbgPrintf("%X\n", inb(Base+IDE_ERROR));
    return ETIMEOUT;
}

UINT32 STDCALL SelectDevice(UINT32 Base, UINT32 Device)
{
	UINT32 e;
	e = WaitReadyDrqOff(Base, 0xFF);
	if(EFAIL(e)) return e;
	outb(Base + IDE_DRIVEHEAD, 0xA0 | (Device<<4));
	AtaDelay();
	e = WaitReadyDrqOff(Base, 0xFF);
	if(EFAIL(e)) return e;
	return ESUCCESS;
}

UINT32 STDCALL AtapiSendPacket(UINT32 Base, UINT32 Device, UINT32 Count, USHORT *Data)
{
	UINT32 e;
    e = WaitReadyDrqOff(Base, 0xFFFFFF);
	if(EFAIL(e)) return e;
	outb(Base + IDE_FEAT, 0);
	outb(Base + IDE_CYLINDERLOW, Count & 0xFF);
	outb(Base + IDE_CYLINDERHIGH, (Count >> 8) & 0xFF);
	e = WaitReady(Base,0xFFFFFF);
	if(EFAIL(e)) return e;
	outb(Base + IDE_COMMAND, ATA_PACKET);
	AtaDelay();
    e = WaitReadyDrq(Base,0xFFFFFF);
	if(EFAIL(e)) return e;
	outws(Base + IDE_DATA,Data,6);
	return ESUCCESS;
}

UINT32 STDCALL GetAtaDevInfo(UINT32 Base, UINT32 Drive, AtaDevInfo *DevInfo)
{
	UINT32 e;
	e = SelectDevice(Base,Drive);
	if(EFAIL(e)) return e;
	outb(Base + IDE_COMMAND, ATA_IDENTIFYDEVICE);
	AtaDelay();
	e = WaitReadyDrq(Base,10);
	if(EFAIL(e)) return e;
	inws(Base + IDE_DATA,(UINT16*)DevInfo,256);
	return ESUCCESS;
}

UINT32 STDCALL GetAtapiDevInfo(UINT32 Base, UINT32 Drive, AtapiDevInfo *DevInfo)
{
	UINT32 e;
	e = SelectDevice(Base,Drive);
	if(EFAIL(e)) return e;
	outb(Base + IDE_COMMAND, ATA_IDENTIFYPACKETDEV);
	AtaDelay();
	e = WaitReadyDrq(Base,10);
	if(EFAIL(e)) return e;
	inws(Base + IDE_DATA,(UINT16*)DevInfo,256);
	return ESUCCESS;
}

void AtaStrCpy(CHAR *Dest, UINT16 *Source, UINT32 Count)
{
	UINT32 i;
	for(i=0;i<Count;i++)
	{
		Dest[i<<1] = (Source[i] >> 8) & 0xFF;
		Dest[(i<<1)+1] = Source[i] & 0xFF;
	}
}

void PrintDeviceInfo(AtaDevice *Device)
{
	DbgPrintf("========================================================================================\n");
	DbgPrintf("\t\t\t%s\n", Device->Desc);
	DbgPrintf("========================================================================================\n");
	DbgPrintf(
		"Serial Number          : %.20s\n"
		"Firmware Revision      : %.8s\n"
		"Model Number           : %.40s\n"
        "Fixed                  : %s\n"
        "ATAPI Packet Interface : %s\n",
	    Device->SerialNumber,
		Device->FirmwareRevision,
		Device->ModelNumber,
        (Device->Flags & DEVICE_FIXED) ? "YES" : "NO",
        (Device->Flags & DEVICE_ATAPI) ? "YES" : "NO");
	DbgPrintf("========================================================================================\n\n");
}

UINT32 STDCALL AtaProbeCtrlr(UINT32 i)
{
    UINT32 Base = AtaRegs[i][0];
	outb(Base + IDE_SECTORNUMBER, 0x55);
    outb(Base + IDE_CYLINDERLOW, 0xAA);
    outb(Base + IDE_SECTORNUMBER, 0xAA);
    outb(Base + IDE_CYLINDERLOW, 0x55);
    outb(Base + IDE_SECTORNUMBER, 0x55);
    outb(Base + IDE_CYLINDERLOW, 0xAA);
	//DbgPrintf("A%X %XA", inb(Base + IDE_SECTORNUMBER), inb(Base + IDE_CYLINDERLOW));
    if((inb(Base + IDE_SECTORNUMBER)!=0x55) || (inb(Base + IDE_CYLINDERLOW)!=0xAA))
    {
        return ENOTDETECTED;
    }
    return EDETECTED;
}
void TryEject(AtaDevice *pDevice)
{
    volatile UINT32 a=0xFFFF;
    DbgPrintf("Ejecting\n");
    while(inp(pDevice->Command+IDE_STATUS) & ASTATUS_BUSY);
    DbgPrintf("Ejecting2 %X\n", inp(pDevice->Command+IDE_STATUS));
    while((~inp(pDevice->Command+IDE_STATUS)) & ASTATUS_DRDY);
    DbgPrintf("Ejecting3\n");
    outb(pDevice->Command + IDE_DRIVEHEAD, pDevice->Drive<<4);
    outb(pDevice->Control + IDE_CONTROL, 10);
    outb(pDevice->Command + IDE_COMMAND, 0xA0);
    while(a) a--;
    while(inp(pDevice->Command+IDE_STATUS) & ASTATUS_BUSY);
    DbgPrintf("Ejecting4\n");
    while((~inp(pDevice->Command+IDE_STATUS)) & ASTATUS_DRQ);
    DbgPrintf("Ejecting5\n");
    outws(pDevice->Command + IDE_DATA, ATAPI_PACKET_EJECT, 6);
    a = inb(pDevice->Command + IDE_COMMAND);
    while(inp(pDevice->Command+IDE_STATUS) & ASTATUS_BUSY);
    DbgPrintf("Done\n");
}

UINT32 STDCALL AtaProbeAt(UINT32 i, UINT32 j, AtaDevice **ppDevice)
{
    AtaDevice *pDevice;
    UINT32 Base =  AtaRegs[i][0];
    UINT32 e;
    AtapiDevInfo DevAtapi;
    AtaDevInfo DevAta;

    *ppDevice = NULL;

    e = SelectDevice(Base,j);
    if(EFAIL(e))
    {
        return ENOTDETECTED;
    }

    e = GetAtaDevInfo(Base,j,&DevAta);
	//DbgPrintf("APA %X\n", e);
    if(EOK(e))
    {
        pDevice = (AtaDevice*)HeapAlloc(sizeof(AtaDevice));
        strcpy(pDevice->Desc, "IDE HARD DISK DRIVE");
        pDevice->Flags = DEVICE_FIXED;
        pDevice->Command = AtaRegs[i][0];
        pDevice->Control = AtaRegs[i][1];
        pDevice->Drive = j;
        AtaStrCpy(pDevice->SerialNumber, DevAta.SerialNumber, 10);
        AtaStrCpy(pDevice->FirmwareRevision, DevAta.FirmwareRev, 4);
        AtaStrCpy(pDevice->ModelNumber, DevAta.ModelNumber, 20);
        pDevice->HeadCount = DevAta.HeadCount;
        pDevice->CylinderCount = DevAta.CylinderCount;
        pDevice->SectorsPerTrack = DevAta.SectorsPerTrack;
        pDevice->MaxSectors = (DevAta.MaxSectors & 0xFF==0)?1:(DevAta.MaxSectors & 0xFF);
        pDevice->SectorSize = DevAta.SectorSize;
        pDevice->SectorCount = DevAta.NumSectors;
        pDevice->AtapiFlags = 0;
        *ppDevice = pDevice;
        PrintDeviceInfo(pDevice);
        return EATA;
    }

	e = GetAtapiDevInfo(Base,j,&DevAtapi);
    if(EOK(e))
    {
        if((DevAtapi.GenConf & 0xC000)!=0x8000) return EFAILED; //NOT A ATAPI DEVICE
        pDevice = (AtaDevice*)HeapAlloc(sizeof(AtaDevice));
        strcpy(pDevice->Desc, "ATAPI CD-ROM DRIVE");
        pDevice[0].Flags = DEVICE_FIXED|DEVICE_ATAPI;
        pDevice[0].AtapiFlags = 0;
        if(DevAtapi.GenConf & 0x80) pDevice[0].Flags ^= DEVICE_FIXED;
        pDevice[0].Command = AtaRegs[i][0];
        pDevice[0].Control = AtaRegs[i][1];
        pDevice->Drive = j;
        AtaStrCpy(pDevice[0].SerialNumber, DevAtapi.SerialNumber, 10);
        AtaStrCpy(pDevice[0].FirmwareRevision, DevAtapi.FirmwareRev, 4);
        AtaStrCpy(pDevice[0].ModelNumber, DevAtapi.ModelNumber, 20);
        pDevice->SectorSize = 2048;
        pDevice->AtapiFlags = 0;
        *ppDevice = pDevice;
        AtapiReset(pDevice);
        PrintDeviceInfo(pDevice);
        return EATAPI;
    }
    return EFAILED;
}



UINT32 STDCALL AtaSetupCHS(UINT32 Base, UINT32 Drive, UINT32 Head, UINT32 Cylinder, UINT32 Sector)
{
    outb(Base + IDE_DRIVEHEAD, ((Drive & 0x01) << 4) | (Head & 0x0F));
    outb(Base + IDE_CYLINDERLOW, Cylinder & 0xFF);
    outb(Base + IDE_CYLINDERHIGH, (Cylinder >> 8) & 0xFF);
    outb(Base + IDE_SECTORNUMBER, Sector & 0x000000FF);
    outb(Base + IDE_PRECOMP, 0xFF);
    return ESUCCESS;
}

UINT32 STDCALL AtaSetupLBA(UINT32 Base, UINT32 Drive, UINT32 _LBA)
{
    outb(Base + IDE_DRIVEHEAD, (0xE0|((Drive & 0x00000001)<<4)|((_LBA >> 24) & 0x0000000F)));
    outb(Base + IDE_CYLINDERLOW, (_LBA >> 8) & 0x000000FF);
    outb(Base + IDE_CYLINDERHIGH, (_LBA >> 16) & 0x000000FF);
    outb(Base + IDE_SECTORNUMBER, _LBA & 0x000000FF);
    outb(Base + IDE_PRECOMP, 0xFF);
    return ESUCCESS;
}

UINT32 STDCALL AtaResetCtrlr(UINT32 _Ctrl)
{
	//int i;
    outb(_Ctrl+IDE_CONTROL, CONTROL_SRST);
    //DelayMS(2);
	KeThreadSleep(2);
	//for(i=0;i<0xFFFFFFFF;i++);
    outb(_Ctrl+IDE_CONTROL, CONTROL_IEN);
	KeThreadSleep(2);
    //DelayMS(2);
	//for(i=0;i<0xFFFFFFFF;i++);
    return ESUCCESS;
}

UINT32 STDCALL AtaReset(AtaDevice *Device)
{
    return -1;
}

UINT32 STDCALL AtapiReset(AtaDevice *Device)
{
    UINT32 e;
    if((~Device->Flags) & DEVICE_ATAPI) return EWRONGDEVICE;
    e = WaitReady(Device->Command, 10);
    if(EFAIL(e)) return e;
    outb(Device->Command+IDE_ERROR, 0xCC);
    outb(Device->Command+IDE_COMMAND, ATA_SETFEAT);
    AtaDelay();
    e = WaitReady(Device->Command, 10);
    if(EFAIL(e)) return e;
    outb(Device->Command+IDE_COMMAND, ATA_DEVICERESET);
    AtaDelay();
    e = WaitReady(Device->Command, 10);
    if(EFAIL(e)) return e;
    DbgPrintf("%X\n", inb(Device->Command+IDE_ERROR));
    return ESUCCESS;
}

UINT32 STDCALL AtapiStart(AtaDevice *Device)
{
    //AtapiClose(Device);
    UINT32 e;
    UINT16 data;
    DbgPrintf("Staring Device\n");
    /*if((~Device->Flags) & DEVICE_ATAPI) return EWRONGDEVICE;*/
    e = SelectDevice(Device->Command, Device->Drive);
    if(EFAIL(e)) return e;
    //e = WaitReady(Device->Command, 10);
    //if(EFAIL(e)) return e;
    //if(Device->AtapiFlags & ATAPI_DOOROPEN)
    //{
        DbgPrintf("Closing Door\n");
        e = AtapiSendPacket(Device->Command, Device->Drive, 0, (USHORT*)ATAPI_PACKET_CLOSE);
        if(EFAIL(e)) return e;
        e = WaitReady(Device->Command, 0x0FFFFFFF);
        if(EFAIL(e)) return e;
    /*    Device->AtapiFlags &= ~ATAPI_DOOROPEN;
    }*/
    e = AtapiSendPacket(Device->Command, Device->Drive, 0, (USHORT*)ATAPI_PACKET_START);
    if(EFAIL(e)) return e;
    e = WaitReady(Device->Command, 0x0FFFFFFF);
    //DbgPrintf("1\n");
    if(EFAIL(e)) return e;
    Device->AtapiFlags |= ATAPI_STARTED;
    e = AtapiSendPacket(Device->Command, Device->Drive, 8, (USHORT*)ATAPI_PACKET_READCAPACITY);
    DbgPrintf("3\n");
    if(EFAIL(e)) return e;
    e = WaitReady(Device->Command, 0x0FFFF);
    DbgPrintf("%X\n", inb(Device->Command+IDE_ERROR));
    e = WaitReady(Device->Command, 0x0FFFFfff);
    DbgPrintf("4\n");
    if(EFAIL(e)) return e;
    data = inw(Device->Command+IDE_DATA);
    Device->SectorCount = (data & 0x00FF)<<24;
    Device->SectorCount |= (data & 0xFF00)<<8;
    e = WaitDrq(Device->Command, 0x0FFFFFFF);
    if(EFAIL(e)) return e;
    data = inw(Device->Command+IDE_DATA);
    Device->SectorCount |= (data & 0x00FF)<<8;
    Device->SectorCount |= (data & 0xFF00)>>8;
    e = WaitDrq(Device->Command, 0x0FFFFFFF);
    if(EFAIL(e)) return e;
    data = inw(Device->Command+IDE_DATA);
    Device->SectorSize = (data & 0x00FF)<<24;
    Device->SectorSize |= (data & 0xFF00)<<8;
    e = WaitDrq(Device->Command, 0x0FFFFFFF);
    if(EFAIL(e)) return e;
    data = inw(Device->Command+IDE_DATA);
    Device->SectorSize |= (data & 0x00FF)<<8;
    Device->SectorSize |= (data & 0xFF00)>>8;
    if((Device->SectorCount==0)||(Device->SectorCount==0xFFFFFFFF))
    {
        DbgPrintf("No Media Present\n");
        Device->AtapiFlags &= ~ATAPI_MEDIAPRESENT;
        return ENOMEDIA;
    }
    e = AtapiSendPacket(Device->Command, Device->Drive, 12, (USHORT*)ATAPI_PACKET_READTOC);
    if(EFAIL(e)) return e;
    e = WaitDrq(Device->Command, 0x0FFFFFFF);
    if(EFAIL(e)) return e;
    data = inw(Device->Command+IDE_DATA);
    e = WaitDrq(Device->Command, 0x0FFFFFFF);
    if(EFAIL(e)) return e;
    data = inw(Device->Command+IDE_DATA);
    e = WaitDrq(Device->Command, 0x0FFFFFFF);
    if(EFAIL(e)) return e;
    data = inw(Device->Command+IDE_DATA);
    e = WaitDrq(Device->Command, 0x0FFFFFFF);
    if(EFAIL(e)) return e;
    data = inw(Device->Command+IDE_DATA);
    e = WaitDrq(Device->Command, 0x0FFFFFFF);
    if(EFAIL(e)) return e;
    data = inw(Device->Command+IDE_DATA);
    Device->LastSession = (data & 0x00FF)<<24;
    Device->LastSession |= (data & 0xFF00)<<8;
    e = WaitDrq(Device->Command, 0x0FFFFFFF);
    if(EFAIL(e)) return e;
    data = inw(Device->Command+IDE_DATA);
    Device->LastSession |= (data & 0x00FF)<<8;
    Device->LastSession |= (data & 0xFF00)>>8;
    DbgPrintf("Device Started\n");
    return ESUCCESS;
}

UINT32 STDCALL AtapiReadSect(AtaDevice *Device, VPTR Buffer, SIZE_T SectNum, SIZE_T SectCnt)
{
    UINT32 e, BytesToRead, Bytes;
    UCHAR packet[12], data;

    if((~Device->Flags) & DEVICE_ATAPI) return EWRONGDEVICE;
    if(Device->AtapiFlags & ATAPI_STARTED)
    {
        e = SelectDevice(Device->Command, Device->Drive);
    }
    else
    {
        e = AtapiStart(Device);
    }
    if(EFAIL(e)) return e;
    BytesToRead = SectCnt * Device->SectorSize;
    packet[0] = ATAPI_READ12;
    packet[1] = 0;
    packet[2] = (SectNum & 0xFF000000) >> 24;
    packet[3] = (SectNum & 0x00FF0000) >> 16;
    packet[4] = (SectNum & 0x0000FF00) >> 8;
    packet[5] = (SectNum & 0x000000FF);
    packet[6] = (SectCnt & 0xFF000000) >> 24;
    packet[7] = (SectCnt & 0x00FF0000) >> 16;
    packet[8] = (SectCnt & 0x0000FF00) >> 8;
    packet[9] = (SectCnt & 0x000000FF);
    packet[10] = 0;
    packet[12] = 0;
    e = AtapiSendPacket(Device->Command, Device->Drive, 0, (USHORT*)packet);
    if(EFAIL(e)) return e;
    while(BytesToRead)
    {
        e = WaitReadyDrq(Device->Command, 0x0FFFFFFF);
        data = inb(Device->Command+IDE_CYLINDERLOW);
        Bytes = data;
        data = inb(Device->Command+IDE_CYLINDERHIGH);
        Bytes |= (data<<8);
        inws(Device->Command+IDE_DATA, (UINT16*)Buffer, Bytes>>1);
        if(Bytes & 0x01)
            data = inb(Device->Command+IDE_DATA);
        Buffer[Bytes-1] = data;

        Buffer += Bytes;
        BytesToRead -= Bytes;
    }
    if(EFAIL(e)) return e;
    return ESUCCESS;
}

UINT32 STDCALL AtaReadSect(AtaDevice *Device, VPTR Buffer, SIZE_T SectNum, SIZE_T SectCnt)
{
    UCHAR e;
    if((Device->Flags) & DEVICE_ATAPI) return EWRONGDEVICE;
    while(SectCnt)
    {
		//DbgPrintf("9");
        //DbgPrintf("ATA Reading Sector %X\n", SectNum);
        e = WaitReady(Device->Command, 0x0FFFFFFF);
        if(EFAIL(e)) return e;
		//DbgPrintf(" 10");
        AtaSetupLBA(Device->Command, Device->Drive, SectNum);
        outb(Device->Command+IDE_SECTORCOUNT, 1);
        e = WaitReady(Device->Command, 0x0FFFFFFF);
		//DbgPrintf(" 11");
        if(EFAIL(e)) return e;
        outb(Device->Command+IDE_COMMAND, ATA_READSECTS_RET);
        e = WaitReadyDrq(Device->Command, 0x0FFFFFFF);
		//DbgPrintf(" 12");
        if(EFAIL(e)) return e;
        inws(Device->Command+IDE_DATA, (UINT16*)Buffer, 256);
		//DbgPrintf(" 13");
        Buffer += 512;
        SectCnt--;
        SectNum++;
    }
    return ESUCCESS;
}

UINT32 STDCALL AtapiStop(AtaDevice *Device)
{
    UINT32 e;
    if((~Device->Flags) & DEVICE_ATAPI) return EWRONGDEVICE;
    e = SelectDevice(Device->Command, Device->Drive);
    if(EFAIL(e)) return e;
    e = AtapiSendPacket(Device->Command, Device->Drive, 0, (USHORT*)ATAPI_PACKET_STOP);
    if(EFAIL(e)) return e;
    e = WaitReady(Device->Command, 0x0FFFFFFF);
    if(EFAIL(e)) return e;
    Device->AtapiFlags &= ~ATAPI_STARTED;
    return ESUCCESS;
}

UINT32 STDCALL AtapiLock(AtaDevice *Device)
{
    UINT32 e;
    if((~Device->Flags) & DEVICE_ATAPI) return EWRONGDEVICE;
    e = SelectDevice(Device->Command, Device->Drive);
    if(EFAIL(e)) return e;
    e = AtapiSendPacket(Device->Command, Device->Drive, 0, (USHORT*)ATAPI_PACKET_LOCK);
    if(EFAIL(e)) return e;
    e = WaitReady(Device->Command, 0x0FFFFFFF);
    if(EFAIL(e)) return e;
    Device->AtapiFlags |= ATAPI_LOCKED;
    return ESUCCESS;
}

UINT32 STDCALL AtapiUnlock(AtaDevice *Device)
{
    UINT32 e;
    if((~Device->Flags) & DEVICE_ATAPI) return EWRONGDEVICE;
    e = SelectDevice(Device->Command, Device->Drive);
    if(EFAIL(e)) return e;
    e = AtapiSendPacket(Device->Command, Device->Drive, 0, (USHORT*)ATAPI_PACKET_UNLOCK);
    if(EFAIL(e)) return e;
    e = WaitReady(Device->Command, 0x0FFFFFFF);
    if(EFAIL(e)) return e;
    Device->AtapiFlags &= ~ATAPI_LOCKED;
    return ESUCCESS;
}

UINT32 STDCALL AtapiEject(AtaDevice *Device)
{
    UINT32 e;
    if((~Device->Flags) & DEVICE_ATAPI) return EWRONGDEVICE;
    //DbgPrintf("SELECT DEVICE\n");
    e = SelectDevice(Device->Command, Device->Drive);
    if(EFAIL(e)) return e;
    //DbgPrintf("SEND PACKET\n");
    e = AtapiSendPacket(Device->Command, Device->Drive, 0, (USHORT*)ATAPI_PACKET_EJECT);
    if(EFAIL(e)) return e;
    //DbgPrintf("WAIT READY\n");
    e = WaitReady(Device->Command, 0x0FFFFFFF);
    if(EFAIL(e)) return e;
    Device->AtapiFlags |= ATAPI_DOOROPEN;
    return ESUCCESS;
}

UINT32 STDCALL AtapiClose(AtaDevice *Device)
{
    UINT32 e;
    if((~Device->Flags) & DEVICE_ATAPI) return EWRONGDEVICE;
    e = SelectDevice(Device->Command, Device->Drive);
    if(EFAIL(e)) return e;
    e = AtapiSendPacket(Device->Command, Device->Drive, 0, (USHORT*)ATAPI_PACKET_CLOSE);
    if(EFAIL(e)) return e;
    e = WaitReady(Device->Command, 0x0FFFFFFF);
    if(EFAIL(e)) return e;
    Device->AtapiFlags &= ~ATAPI_DOOROPEN;
    return ESUCCESS;
}

///////////////////////////////////////////////////////////////////////////////////
////////////////////////////// DRIVER SECTION /////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////

void AtaIntPrimary()
{
}

void AtaIntSecondary()
{
}

UINT32 STDCALL ATA_DevRead(VPTR Device, VPTR Buffer, SIZE_T SectNum, SIZE_T Count)
{
    AtaDevice *pDevice = (AtaDevice*)Device;
    //DbgPrintf("ATA_DevRead\n");
    if(pDevice)
    {
        if(pDevice->Flags & DEVICE_ATAPI)
        {
            return AtapiReadSect(pDevice, Buffer, SectNum, Count);
        }
        else
        {
			//DbgPrintf("7");
            return AtaReadSect(pDevice, Buffer, SectNum, Count);
			//DbgPrintf("8");
        }
    }
    else
    {
        return EFAILED;
    }
}

UINT32 STDCALL ATA_DevGeom(VPTR Device, UINT32 *nHeads, UINT32 *nCylinders, UINT32 *nSectors)
{
    AtaDevice *pDevice = (AtaDevice*)Device;
    if(pDevice)
    {
        if(pDevice->Flags & DEVICE_ATAPI)
        {
            return ENOTSUPPORTED;
        }
        else
        {
            *nHeads = pDevice->HeadCount;
            *nCylinders = pDevice->CylinderCount;
            *nSectors = pDevice->SectorsPerTrack;
            return ESUCCESS;
        }
    }
    else
    {
        return EFAILED;
    }
}

UINT32 STDCALL ATA_DevSectSize(VPTR Device, UINT32 *Size)
{
    AtaDevice *pDevice = (AtaDevice*)Device;
    if(pDevice)
    {
        *Size = pDevice->SectorSize;
        return ESUCCESS;
    }
    else
    {
        return EFAILED;
    }
}

UINT32 STDCALL ATA_DevSectCount(VPTR Device, UINT32 *Count)
{
    AtaDevice *pDevice = (AtaDevice*)Device;
    if(pDevice)
    {
        *Count = pDevice->SectorCount;
        return ESUCCESS;
    }
    else
    {
        return EFAILED;
    }
}

UINT32 STDCALL ATAPI_MediaPresent(VPTR Device)
{
    UCHAR e;
    AtaDevice *pDevice = (AtaDevice*)Device;
    if(pDevice)
    {
        if(pDevice->Flags & DEVICE_ATAPI)
        {
            if((~(pDevice->AtapiFlags)) & ATAPI_STARTED)
            {
                DbgPrintf("%X\n", pDevice->AtapiFlags);
                e = AtapiStart(pDevice);
                if(EFAIL(e)) return e;
            }
            DbgPrintf("%X\n", pDevice->AtapiFlags);
            if(pDevice->AtapiFlags & ATAPI_MEDIAPRESENT)
                return EPRESENT;
            else
                return ENOTPRESENT;
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

UINT32 STDCALL ATAPI_MediaEject(VPTR Device)
{
    AtaDevice *pDevice = (AtaDevice*)Device;
    if(pDevice)
    {
        if(pDevice->Flags & DEVICE_ATAPI)
        {
            return AtapiEject(pDevice);
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

UINT32 STDCALL ATAPI_MediaClose(VPTR Device)
{
    AtaDevice *pDevice = (AtaDevice*)Device;
    if(pDevice)
    {
        if(pDevice->Flags & DEVICE_ATAPI)
        {
            return AtapiClose(pDevice);
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

UINT32 STDCALL ATAPI_MediaLock(VPTR Device)
{
    AtaDevice *pDevice = (AtaDevice*)Device;
    if(pDevice)
    {
        if(pDevice->Flags & DEVICE_ATAPI)
        {
            return AtapiLock(pDevice);
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

UINT32 STDCALL ATAPI_MediaUnlock(VPTR Device)
{
    AtaDevice *pDevice = (AtaDevice*)Device;
    if(pDevice)
    {
        if(pDevice->Flags & DEVICE_ATAPI)
        {
            return AtapiUnlock(pDevice);
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


DEV_FUNC AtaDevFunc =
{
    &ATA_DevRead,
    &ATA_DevGeom,
    &ATA_DevSectSize,
    &ATA_DevSectCount,
    &ATAPI_MediaPresent,
    &ATAPI_MediaEject,
    &ATAPI_MediaClose,
    &ATAPI_MediaLock,
    &ATAPI_MediaUnlock
};

UINT32 STDCALL AtaProbe()
{
	char _sn[21];
    AtaDevice *pDevice;
	UINT32 i,j,e;
	_sn[20] = '\0';
	for(i=0;i<2;i++)
	{
		AtaResetCtrlr(AtaRegs[i][1]);
        e = AtaProbeCtrlr(i);
			//DbgPrintf("dsfgdsfg%X", e);
        if(EFAIL(e)) continue;

		for(j=0;j<2;j++)
        {
			//DbgPrintf("i=%d, j=%d\n", i, j);
			//AtaResetCtrlr(AtaRegs[i][1]);
            e = AtaProbeAt(i, j, &pDevice);
			//DbgPrintf("i=%d, j=%d\n", i, j);
            if(EFAIL(e)) continue;
			//DbgPrintf("i=%d, j=%d\n", i, j);
			memcpy(_sn, pDevice->SerialNumber, 20);
            StorageCreate(_sn, (VPTR)pDevice, (pDevice->Flags & DEVICE_ATAPI) ? STORAGE_CDROM : STORAGE_HDD, &AtaDevFunc);
			//DbgPrintf("i=%d, j=%d\n", i, j);
		}
	}
	return ESUCCESS;
}

////////////////////////////////////////////////////////////////////////////////
//-----------------------------------ata.c------------------------------------//
//-----------------------Copyright © Sidhant Goel, 2008-----------------------//
////////////////////////////////////////////////////////////////////////////////
