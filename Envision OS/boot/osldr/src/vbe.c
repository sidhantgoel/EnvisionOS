#include <vbe.h>
#include <vm86.h>

VbeInfoBlock VBE_INFO;

/*
	INT 10 - VESA SuperVGA BIOS (VBE) - GET SuperVGA INFORMATION
		AX = 4F00h
		ES:DI -> buffer for SuperVGA information (see #00077)
	Return: 
		AL = 4Fh if function supported
		AH = status
	Desc:	determine whether VESA BIOS extensions are present and the capabilities
			supported by the display adapter
*/
UINT32 VbeGetInfo(VbeInfoBlock *VbeInfo)
{
	VM86_ARGS args;
	memset(&args,0,sizeof(VM86_ARGS));
	args.Eax = 0x00004F00;
	args.Es = 0x0000;
	args.Edi = 0x8000;
	*(UINT32*)VbeInfo->Signature = *(UINT32*)"VBE2";
	Vm86CallInt(&args, 0x10);
	if((args.Eax & 0xFFFF) == VBE_SUCCESSFUL)
	{
		memcpy(VbeInfo,(void*)to_linear(0x0000,0x8000),sizeof(VbeInfoBlock));
		if(*(UINT32*)VbeInfo->Signature==*(UINT32*)"VESA")
		{
			VbeInfo->OemStringPtr.Linear = to_linear(VbeInfo->OemStringPtr.Segment,VbeInfo->OemStringPtr.Offset);
			VbeInfo->VideoModePtr.Linear = to_linear(VbeInfo->VideoModePtr.Segment,VbeInfo->VideoModePtr.Offset);
			VbeInfo->OemStringPtr.Linear = (VbeInfo->OemStringPtr.Linear>=0x8000 && VbeInfo->OemStringPtr.Linear<0x8200) ? ((UINT32)VbeInfo + VbeInfo->OemStringPtr.Linear - 0x8000) : VbeInfo->OemStringPtr.Linear;
			VbeInfo->VideoModePtr.Linear = (VbeInfo->VideoModePtr.Linear>=0x8000 && VbeInfo->VideoModePtr.Linear<0x8200) ? ((UINT32)VbeInfo + VbeInfo->VideoModePtr.Linear - 0x8000) : VbeInfo->VideoModePtr.Linear;
			if(VbeInfo->Version >= 0x0200)
			{
				VbeInfo->OemVendorNamePtr.Linear = to_linear(VbeInfo->OemVendorNamePtr.Segment,VbeInfo->OemVendorNamePtr.Offset);
				VbeInfo->OemProductNamePtr.Linear = to_linear(VbeInfo->OemProductNamePtr.Segment,VbeInfo->OemProductNamePtr.Offset);
				VbeInfo->OemProductRevPtr.Linear = to_linear(VbeInfo->OemProductRevPtr.Segment,VbeInfo->OemProductRevPtr.Offset);
				VbeInfo->OemVendorNamePtr.Linear = (VbeInfo->OemVendorNamePtr.Linear>=0x8000 && VbeInfo->OemVendorNamePtr.Linear<0x8200) ? ((UINT32)VbeInfo + VbeInfo->OemVendorNamePtr.Linear - 0x8000) : VbeInfo->OemVendorNamePtr.Linear;
				VbeInfo->OemProductNamePtr.Linear = (VbeInfo->OemProductNamePtr.Linear>=0x8000 && VbeInfo->OemProductNamePtr.Linear<0x8200) ? ((UINT32)VbeInfo + VbeInfo->OemProductNamePtr.Linear - 0x8000) : VbeInfo->OemProductNamePtr.Linear;
				VbeInfo->OemProductRevPtr.Linear = (VbeInfo->OemProductRevPtr.Linear>=0x8000 && VbeInfo->OemProductRevPtr.Linear<0x8200) ? ((UINT32)VbeInfo + VbeInfo->OemProductRevPtr.Linear - 0x8000) : VbeInfo->OemProductRevPtr.Linear;
			}
		}
		return true;
	}
	else
	{
		return false;
	}
}

/*
	INT 10 - VESA SuperVGA BIOS - GET SuperVGA MODE INFORMATION
		AX = 4F01h
		CX = SuperVGA video mode (see #04082 for bitfields)
		ES:DI -> 256-byte buffer for mode information (see #00079)
	Return:
		AL = 4Fh if function supported
		AH = status
	Desc:	determine the attributes of the specified video mode
*/
UINT32 VbeGetModeInfo(UINT32 Mode,VbeModeInfoBlock *ModeInfo)
{
	VM86_ARGS args;
	memset(&args,0,sizeof(VM86_ARGS));
	args.Eax = 0x00004F01;
	args.Ecx = Mode & 0xFFFF;
	args.Es = 0x0000;
	args.Edi = 0x8000;
	Vm86CallInt(&args, 0x10);
	if((args.Eax & 0xFFFF) == VBE_SUCCESSFUL)
	{
		memcpy(ModeInfo,(void*)to_linear(0x0000,0x8000),sizeof(VbeModeInfoBlock));
		return true;
	}
	else
	{
		return false;
	}
}

/*
	INT 10 - VESA SuperVGA BIOS - SET SuperVGA VIDEO MODE
		AX = 4F02h
		BX = new video mode (see #04082,#00083,#00084)
		ES:DI -> (VBE 3.0+) CRTC information block, bit mode bit 11 set
	Return: 
		AL = 4Fh if function supported
		AH = status
	Notes:	
		bit 13 may only be set if the video mode is present in the list of
		accelerated video modes returned by AX=4F00h
		if the DAC supports both 8 bits per primary color and 6 bits, it will
		be reset to 6 bits after a mode set; use AX=4F08h to restore 8 bits
*/
UINT32 VbeSetMode(UINT32 Mode)	//FIX: CRTC Information Block
{
	VM86_ARGS args;
	memset(&args,0,sizeof(VM86_ARGS));
	args.Eax = 0x00004F02;
	args.Ebx = Mode & 0xFFFF;
	Vm86CallInt(&args, 0x10);
	if((args.Eax & 0xFFFF) == VBE_SUCCESSFUL)
	{
		return true;
	}
	else
	{
		return false;
	}
}

/*
	INT 10 - VESA SuperVGA BIOS - GET CURRENT VIDEO MODE
		AX = 4F03h
	Return: 
		AL = 4Fh if function supported
		AH = status
	    00h successful
		BX = video mode (see #00083,#00084)
		    bit 13: VBE/AF v1.0P accelerated video mode
		    bit 14: linear frame buffer enabled (VBE v2.0+)
		    bit 15: don't clear video memory
	    01h failed
*/
UINT32 VbeGetMode(UINT32 *Mode)
{
    VM86_ARGS args;
	memset(&args,0,sizeof(VM86_ARGS));
	args.Eax = 0x00004F03;
	Vm86CallInt(&args, 0x10);
	if((args.Eax & 0xFFFF) == VBE_SUCCESSFUL)
	{
		*Mode = args.Ebx & 0xFFFF;
		return true;
	}
	else
	{
		*Mode = 0xFFFF;
		return false;
	}
}

/*
	INT 10 - VESA SuperVGA BIOS - SAVE/RESTORE SuperVGA VIDEO STATE
		AX = 4F04h
		DL = subfunction
			00h get state buffer size
			Return: 
				BX = number of 64-byte blocks needed
			01h save video states
				ES:BX -> buffer
			02h restore video states
				ES:BX -> buffer
				CX = states to save/restore (see #00085)
			Return: 
				AL = 4Fh if function supported
				AH = status
*/

/*
INT 10 - VESA SuperVGA BIOS - CPU VIDEO MEMORY CONTROL
	AX = 4F05h
	BH = subfunction
	    00h select video memory window
		DX = window address in video memory (in granularity units)
	    01h get video memory window
		Return: DX = window address in video memory (in gran. units)
	BL = window number
	    00h window A
	    01h window B
	ES = selector for memory-mapped registers (VBE 2.0+, when called from
	      32-bit protected mode)
Return: AL = 4Fh if function supported
	AH = status
	    00h successful
	    01h failed
Note:	when using an accelerated video mode under VBE/AF v1.0P, the
	  application must call EnableDirectAccess before switching banks if
	  bit 9 of the video mode attributes flag is set (see #00080)
BUG:	Phoenix S3 Trio64V+ v1.02-02 reportedly returns garbage in DX for
	  subfunction 01h
*/

/*
INT 10 - VESA SuperVGA BIOS v1.1+ - GET/SET LOGICAL SCAN LINE LENGTH (PIXELS)
	AX = 4F06h
	BL = subfunction
	    00h set scan line length
		CX = desired width in pixels
	    01h get scan line length
Return: AL = 4Fh if function supported
	AH = status
	    00h successful
	    01h failed
	BX = bytes per scan line
	CX = number of pixels per scan line
	DX = maximum number of scan lines
Notes:	if the desired width is not achievable, the next larger width will be
	  set
	the scan line may be wider than the visible area of the screen
	this function is valid in text modes, provided that pixel values are
	  multiplied by the character cell width/height
*/

/*
INT 10 - VESA SuperVGA BIOS v1.1+ - GET/SET DISPLAY START
	AX = 4F07h
	BL = subfunction
	    00h,80h set display start
		CX = leftmost displayed pixel in scan line
		DX = first displayed scan line
	    01h get display start
		Return: BH = 00h
			CX = leftmost displayed pixel in scan line
			DX = first displayed scan line
	BH = 00h (reserved)
Return: AL = 4Fh if function supported
	AH = status
	    00h successful
	    01h failed
Notes:	this function is valid in text modes, provided that values are
	  multiplied by the character cell width/height
	subfunction 80h waits until vertical retrace before changing the
	  display start address (VBE v2.0+)
*/

/*
INT 10 - VESA SuperVGA BIOS v1.2+ - GET/SET DAC PALETTE CONTROL
	AX = 4F08h
	BL = subfunction
	    00h set DAC palette width
		BH = desired number of bits per primary color
	    01h get DAC palette width
Return: AL = 4Fh if function supported
	AH = status
	    00h successful
		BH = current number of bits per primary (06h = standard VGA)
	    01h failed
*/

/*
INT 10 - VESA SuperVGA BIOS v2.0+ - GET/SET PALETTE ENTRIES
	AX = 4F09h
	BL = subfunction
	    00h set (primary) palette
	    01h get (primary) palette
	    02h set secondary palette data
	    03h get secondary palette data
	    80h set palette during vertical retrace
	CX = number of entries to change
	DX = starting palette index
	ES:DI -> palette buffer, array of DAC entries (see #00086)
Return: AL = 4Fh if function supported
	AH = status
	    00h successful
	    01h failed
*/

/*
INT 10 - VESA SuperVGA BIOS v2.0+ - GET PROTECTED-MODE INTERFACE
	AX = 4F0Ah
	BL = 00h
Return: AL = 4Fh if function supported
	AH = status
	    00h successful
		ES:DI -> protected-mode table (see #00087)
		CX = length of table in bytes, included protected-mode code
	    01h failed
*/

/*
INT 10 - VESA SuperVGA BIOS v3.0 - GET NEAREST PIXEL CLOCK
	AX = 4F0Bh
	BL = 00h get closest pixel clock
	ECX = requested clock frequency in Hz
	DX = video mode number
Return: AL = 4Fh if function supported
	AH = status
	    00h successful
		ECX = nearest clock rate
	    01h failed
*/

UINT32 VbeInit()
{
	//printf("\nVBE::VbeInit() ");
	if(VbeGetInfo(&VBE_INFO))
	{
		//printf("Done");
		return true;
	}
	//printf("Failed");
	return false;
}

UINT32 VbeFindMode(VbeModeInfoBlock *ModeInfo, UINT32 *Mode,UINT32 XRes, UINT32 YRes)
{
	UINT32 i;
	UINT16 *Modes = (UINT16*)VBE_INFO.VideoModePtr.Linear;
	UINT32 BestMode = 0x0000FFFF;
	UINT32 BestDiff = 0x7FFFFFFF;
	UINT32 Diff;
	
	//printf("\nVBE_INFO.VideoModePtr.Linear=%x",VBE_INFO.VideoModePtr.Linear-(UINT32)&VBE_INFO);
	//for(;;);
	for(i=0;Modes[i]!=0xFFFF;i++)
	{
		if(VbeGetModeInfo(Modes[i],ModeInfo))
		{
			//printf("\nSearching, XRes=%d, YRes=%d, Attrib=%x",ModeInfo->XResolution,ModeInfo->YResolution,ModeInfo->ModeAttributes);
			if(!(ModeInfo->ModeAttributes & VBE_MODEA_LFB) && !(ModeInfo->ModeAttributes & VBE_MODEA_GRAPHICS)) continue;
			if(ModeInfo->MemoryModel != VBE_MM_PACKED && ModeInfo->MemoryModel != VBE_MM_RGB) continue;
			if(ModeInfo->BitsPerPixel != 24 && ModeInfo->BitsPerPixel != 32) continue;
			if(ModeInfo->XResolution == XRes && ModeInfo->YResolution == YRes)
			{
				*Mode = Modes[i];
				return true;
			}
			else
			{
				Diff = ModeInfo->XResolution * ModeInfo->YResolution;
				Diff = (Diff > XRes * YRes) ? (Diff - XRes * YRes) : (XRes * YRes - Diff);
				if(Diff < BestDiff)
				{
					BestDiff = Diff;
					BestMode = Modes[i];
				}
			}
		}
		else
		{
			//printf("\nVBE::GetModeInfo() Failed");
		}
	}

	if(BestMode != 0xFFFF)
	{
		if(VbeGetModeInfo(BestMode,ModeInfo))
		{
			*Mode = BestMode;
			return true;
		}
	}

	*Mode = 0xFFFF;
	return false;
}