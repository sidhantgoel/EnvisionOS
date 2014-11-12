#ifndef __vbe_h__
#define __vbe_h__

#include <arch\arch.h>

#pragma pack(push,1)


/* Far Ponter */
typedef struct _VbeFarPtr
{
	union
	{
		struct
		{
			UINT16	Offset;
			UINT16	Segment;
		};
		UINT32 Linear;
	};
}VbeFarPtr;


/* SuperVGA Protected Mode Information Block */
typedef struct _VbePMInfoBlock
{
	CHAR	Signature[4];				/* PM Info Block Signature						*/
	UINT16	EntryPoint;					/* Offset of PM entry point within BIOS			*/
	UINT16	PMInitialize;				/* Offset of PM initialization entry point		*/
	UINT16	BIOSDataSel;				/* Selector to BIOS data area emulation block	*/
	UINT16	A0000Sel;					/* Selector to access A0000h physical mem		*/
	UINT16	B0000Sel;					/* Selector to access B0000h physical mem		*/
	UINT16	B8000Sel;					/* Selector to access B8000h physical mem		*/
	UINT16	CodeSegSel;					/* Selector to access code segment as data		*/
	UCHAR	InProtectMode;				/* Set to 1 when in protected mode				*/
	UCHAR	Checksum;					/* Checksum byte for structure					*/
}VbePMInfoBlock;


/* SuperVGA Information Block */
typedef struct _VbeInfoBlock
{
    CHAR		Signature[4];			/* 'VESA' 4 byte signature				*/
    UINT16		Version;				/* VBE version number					*/
    VbeFarPtr	OemStringPtr;			/* Pointer to OEM string				*/
    UINT32		Capabilities;           /* Capabilities of video card			*/
    VbeFarPtr	VideoModePtr;			/* Pointer to supported modes			*/
    UINT16		TotalMemory;            /* Number of 64kb memory blocks			*/
	/*		VBE 2.0+															*/
	UINT16		OemSoftwareRev;			/* VBE implementation Software revision	*/
	VbeFarPtr	OemVendorNamePtr;		/* VbeFarPtr to Vendor Name String		*/
	VbeFarPtr	OemProductNamePtr;		/* VbeFarPtr to Product Name String		*/
	VbeFarPtr	OemProductRevPtr;		/* VbeFarPtr to Product Revision String	*/
	CHAR		Reserved[222];			/* Reserved for VBE implementation		*/
	CHAR		OemData[256];			/* Data Area for OEM Strings			*/
} VbeInfoBlock;


/* SuperVGA Mode Information Block */
typedef struct _VbeModeInfoBlock
{
    UINT16		ModeAttributes;			/* Mode attributes										*/
    UCHAR		WinAAttributes;			/* Window A attributes									*/
    UCHAR		WinBAttributes;			/* Window B attributes									*/
    UINT16		WinGranularity;			/* Window granularity in k								*/
    UINT16		WinSize;				/* Window size in k										*/
    UINT16		WinASegment;			/* Window A segment										*/
    UINT16		WinBSegment;			/* Window B segment										*/
    VbeFarPtr	WinFuncPtr;				/* Pointer to window function							*/
    UINT16		BytesPerScanLine;		/* Bytes per scanline									*/
    UINT16		XResolution;			/* Horizontal resolution								*/
    UINT16		YResolution;			/* Vertical resolution									*/
    UCHAR		XCharSize;				/* Character cell width									*/
    UCHAR		YCharSize;				/* Character cell height								*/
    UCHAR		NumberOfPlanes;			/* Number of memory planes								*/
    UCHAR		BitsPerPixel;			/* Bits per pixel										*/
    UCHAR		NumberOfBanks;			/* Number of CGA style banks							*/
    UCHAR		MemoryModel;			/* Memory model type									*/
    UCHAR		BankSize;				/* Size of CGA style banks								*/
    UCHAR		NumberOfImagePages;		/* Number of images pages								*/
    UCHAR		res1;					/* Reserved												*/
    UCHAR		RedMaskSize;			/* Size of direct color red mask						*/
    UCHAR		RedFieldPosition;		/* Bit posn of lsb of red mask							*/
    UCHAR		GreenMaskSize;			/* Size of direct color green mask						*/
    UCHAR		GreenFieldPosition;		/* Bit posn of lsb of green mask						*/
    UCHAR		BlueMaskSize;			/* Size of direct color blue mask						*/
    UCHAR		BlueFieldPosition;		/* Bit posn of lsb of blue mask							*/
    UCHAR		RsvdMaskSize;			/* Size of direct color res mask						*/
    UCHAR		RsvdFieldPosition;		/* Bit posn of lsb of res mask							*/
    UCHAR		DirectColorModeInfo;	/* Direct color mode attributes							*/
	/*		VBE 2.0+																			*/
	UINT32		PhysBasePtr;			/* Physical Address for LFB								*/
	UINT32		res2;					/* Reserved - always set to 0							*/
	UINT16		res3;					/* Reserved - always set to 0							*/
	/*		VBE 3.0+																			*/
	UINT16		LinBytesPerScanLine;	/* Bytes per scan line for linear modes					*/
	UCHAR		BnkNumberOfImagePages;	/* Number of images for banked modes					*/
	UCHAR		LinNumberOfImagePages;	/* Number of images for linear modes					*/
	UCHAR		LinRedMaskSize;			/* Size of direct color red mask (linear modes)			*/
	UCHAR		LinRedFieldPosition;	/* Bit position of lsb of red mask (linear modes)		*/
	UCHAR		LinGreenMaskSize;		/* Size of direct color green mask (linear modes)		*/
	UCHAR		LinGreenFieldPosition;	/* Bit position of lsb of green mask (linear modes)		*/
	UCHAR		LinBlueMaskSize;		/* Size of direct color blue mask (linear modes)		*/
	UCHAR		LinBlueFieldPosition;	/* Bit position of lsb of blue mask (linear modes)		*/
	UCHAR		LinRsvdMaskSize;		/* Size of direct color reserved mask (linear modes)	*/
	UCHAR		LinRsvdFieldPosition;	/* Bit position of lsb of reserved mask (linear modes)	*/
	UINT32		MaxPixelClock;			/* Maximum pixel clock (in Hz) for graphics mode		*/
    UCHAR		res4[189];				/* Pad to 256 byte block size							*/
} VbeModeInfoBlock;


/* SuperVGA CRTC Information Block */
typedef struct _VbeCRTCInfoBlock
{
	UINT16	HorizontalTotal;			/* Horizontal total in pixels					*/
	UINT16	HorizontalSyncStart;		/* Horizontal sync start in pixels				*/
	UINT16	HorizontalSyncEnd;			/* Horizontal sync end in pixels				*/
	UINT16	VerticalTotal;				/* Vertical total in lines						*/
	UINT16	VerticalSyncStart;			/* Vertical sync start in lines					*/
	UINT16	VerticalSyncEnd;			/* Vertical sync end in lines					*/
	UCHAR	Flags;						/* Flags (Interlaced, Double Scan etc)			*/
	UINT32	PixelClock;					/* Pixel clock in units of Hz					*/
	UINT16	RefreshRate;				/* Refresh rate in units of 0.01 Hz				*/
	UCHAR	Reserved[40];				/* Remainder of ModeInfoBlock					*/
}VbeCRTCInfoBlock;

#pragma pack(pop)


/*		VBE SIGNATURES																					*/
#define VBE_PMINFO_SIG				"PMID"	/* PMInfo Block Signature									*/
#define VBE_INFO_SIG				"VESA"  /* Vbe Information Block Signature							*/


/*		VBE INFORMATION BLOCK CAPABILITIES																*/
#define VBE_CAP_DAC_NOT_FIXED	0x00000001	/* DAC width is switchable to 8 bits per primary color		*/
#define VBE_CAP_NOT_VGA			0x00000002	/* Controller is not VGA compatible							*/
#define VBE_CAP_RAMDAC			0x00000004	/* Program the RAMDAC using the blank bit in Function 09h	*/
#define VBE_CAP_STEREO_SIG		0x00000008	/* Hardware stereoscopic signaling supported by controller	*/
#define VBE_CAP_VESA_EVC		0x00000010	/* Stereo signaling supported via VESA EVC connector		*/
#define VBE_CAP_RESERVED		0xFFFFFFE0	/* Reserved													*/


/*		VBE MODE NUMBER FORMAT																		*/
#define VBE_MODE_NUMBER			0x000001FF	/* Mode number											*/
#define VBE_MODE_VESA			0x00000100	/* This is a VESA defined VBE mode						*/
#define VBE_MODE_RES0			0x00000200	/* Reserved by VESA for future expansion (= 0)			*/
#define VBE_MODE_RES1			0x00000400	/* Reserved by VESA for future expansion (= 0)			*/
#define VBE_MODE_RRCS			0x00000800	/* Use user specified CRTC values for refresh rate		*/
#define VBE_MODE_RES_VBE_AF0	0x00001000	/* Reserved for VBE/AF (must be 0)						*/
#define VBE_MODE_RES_VBE_AF1	0x00002000	/* Reserved for VBE/AF (must be 0)						*/
#define VBE_MODE_LFB			0x00004000	/* Use Linear/Flat Frame Buffer							*/
#define VBE_MODE_PDM			0x00008000	/* Preserve Display Memory								*/


/*		VBE MODE ATTRIBUTES																			*/
#define VBE_MODEA_HARD_SUPP			0x0001	/* Mode supported in hardware							*/
#define VBE_MODEA_RESERVED0			0x0002	/* 1 (Reserved)											*/
#define VBE_MODEA_TTY				0x0004	/* TTY Output functions supported by BIOS				*/
#define VBE_MODEA_COLOR				0x0008	/* Color mode											*/
#define VBE_MODEA_GRAPHICS			0x0010	/* Graphics mode										*/
#define VBE_MODEA_NOT_VGA			0x0020	/* Not VGA compatible mode								*/
#define VBE_MODEA_NOT_WINDOWED		0x0040	/* VGA compatible windowed memory mode unavailable		*/
#define VBE_MODEA_LFB				0x0080	/* Linear frame buffer mode is available				*/
#define VBE_MODEA_DSM				0x0100	/* Double scan mode is available						*/
#define VBE_MODEA_INTERLACED		0x0200	/* Interlaced mode is available							*/
#define VBE_MODEA_HTBS				0x0400	/* Hardware triple buffering support					*/
#define VBE_MODEA_HSDS				0x0800	/* Hardware stereoscopic display support				*/
#define VBE_MODEA_DDSAS				0x1000	/* Dual display start address support					*/
#define VBE_MODEA_RESERVED1			0xE000	/* Reserved												*/


/*		VBE WINDOW ATTRIBUTES																		*/
#define VBE_WINA_RELOCATABLE		  0x01	/* Relocatable window(s) are supported					*/
#define VBE_WINA_READABLE			  0x02	/* Window is readable									*/
#define VBE_WINA_WRITEABLE		      0x04	/* Window is writeable									*/
#define VBE_WINA_RESERVED			  0xF8	/* Reserved												*/


/*		VBE MEMORY MODEL																			*/
#define VBE_MM_TEXT					  0x00  /* Text mode											*/
#define VBE_MM_CGA					  0x01  /* CGA graphics											*/
#define VBE_MM_HERCULES				  0x02  /* Hercules graphics									*/
#define VBE_MM_PLANAR				  0x03  /* Planar memory model									*/
#define VBE_MM_PACKED				  0x04  /* Packed pixel memory model							*/
#define VBE_MM_NC4					  0x05  /* Non-chain 4, 256 color								*/
#define VBE_MM_RGB					  0x06  /* Direct color RGB memory model						*/
#define VBE_MM_YUV					  0x07  /* Direct color YUV memory model						*/
/*								    08h-0F     Reserved, to be defined by VESA						*/
/*								    10h-FF     To be defined by OEM									*/


/*		VBE DIRECT COLOR MODE INFO																	*/
#define VBE_DCMI_CRAMP_PROG			  0x01	/* Color ramp is programmable							*/
#define VBE_DCMI_RSVD_AVAIL			  0x02	/* Bits in Rsvd field are usable by the application		*/


/*		VBE CRTC FLAGS																				*/
#define VBE_CRTCF_DOUBLE_SCANNED	  0x01	/* Graphics mode is double scanned						*/
#define VBE_CRTCF_INTERLACED		  0x02	/* Graphics mode is interlaced							*/
#define VBE_CRTCF_HSYNC_NEG			  0x04	/* Horizontal sync polarity is negative (-)				*/
#define VBE_CRTCF_VSYNC_NEG			  0x08	/* Vertical sync polarity is negative (-)				*/


/*		VBE RETURN STATUS = AX																					*/
#define VBE_SUCCESSFUL				0x004F	/* Function call successful											*/
#define VBE_FAILED					0x014F	/* Function call failed												*/
#define VBE_INVALID_CONFIG			0x024F	/* Function is not supported in the current hardware configuration	*/
#define VBE_INVALID_MODE			0x034F	/* Function call invalid in current video mode						*/

/*		VBE PUBLIC FUNCTIONS																					*/
UINT32 VbeGetInfo(VbeInfoBlock *VbeInfo);
UINT32 VbeGetModeInfo(UINT32 Mode,VbeModeInfoBlock *ModeInfo);
UINT32 VbeSetMode(UINT32 Mode);
UINT32 VbeGetMode(UINT32 *Mode);
UINT32 VbeFindMode(VbeModeInfoBlock *ModeInfo, UINT32 *Mode,UINT32 XRes, UINT32 YRes);

#endif //__vbe_h__