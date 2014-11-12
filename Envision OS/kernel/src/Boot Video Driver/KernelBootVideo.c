///////////////////////////////////////////////////////////////////////////////////
//-------------------------------KernelBootVideo.h-------------------------------//
//                                      v 1.0                                    //
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
// Created by: Sidhant Goel                                                      //
// Created on: 8 march 2007                                                      //
//                                                                               //
//--------------------------------------Notes------------------------------------//
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
///////////////////////////////////////////////////////////////////////////////////
#include <KernelBootVideo.h>
#include <KernelResources.h>
#include <KernelSynchronization.h>

#define VBE_DISPI_IOPORT_INDEX          0x01CE
#define VBE_DISPI_IOPORT_DATA           0x01CF
#define VBE_DISPI_INDEX_ID              0x0
#define VBE_DISPI_INDEX_XRES            0x1
#define VBE_DISPI_INDEX_YRES            0x2
#define VBE_DISPI_INDEX_BPP             0x3
#define VBE_DISPI_INDEX_ENABLE          0x4
#define VBE_DISPI_INDEX_BANK            0x5
#define VBE_DISPI_INDEX_VIRT_WIDTH      0x6
#define VBE_DISPI_INDEX_VIRT_HEIGHT     0x7
#define VBE_DISPI_INDEX_X_OFFSET        0x8
#define VBE_DISPI_INDEX_Y_OFFSET        0x9

#define VBE_DISPI_DISABLED              0x00
#define VBE_DISPI_ENABLED               0x01
#define VBE_DISPI_GETCAPS               0x02
#define VBE_DISPI_8BIT_DAC              0x20
#define VBE_DISPI_LFB_ENABLED           0x40
#define VBE_DISPI_NOCLEARMEM            0x80

#define	VGA_AC_INDEX		0x3C0
#define	VGA_AC_WRITE		0x3C0
#define	VGA_AC_READ		    0x3C1
#define	VGA_MISC_WRITE		0x3C2
#define VGA_SEQ_INDEX		0x3C4
#define VGA_SEQ_DATA		0x3C5
#define	VGA_DAC_READ_INDEX	0x3C7
#define	VGA_DAC_WRITE_INDEX	0x3C8
#define	VGA_DAC_DATA		0x3C9
#define	VGA_MISC_READ		0x3CC
#define VGA_GC_INDEX 		0x3CE
#define VGA_GC_DATA 		0x3CF
/*			COLOR emulation		MONO emulation */
#define VGA_CRTC_INDEX		0x3D4		/* 0x3B4 */
#define VGA_CRTC_DATA		0x3D5		/* 0x3B5 */
#define	VGA_INSTAT_READ		0x3DA

#define	VGA_NUM_SEQ_REGS	5
#define	VGA_NUM_CRTC_REGS	25
#define	VGA_NUM_GC_REGS		9
#define	VGA_NUM_AC_REGS		21
#define	VGA_NUM_REGS		(1 + VGA_NUM_SEQ_REGS + VGA_NUM_CRTC_REGS + VGA_NUM_GC_REGS + VGA_NUM_AC_REGS)

KBOOTVIDEO_INFO *BootVideoInfo;
UINT32 local_panes0[4][640*480/32];
UINT32 local_panes1[4][640*480/32];
UINT32 *FrameBuffer;
UINT32 ConsoleX,ConsoleY;
UINT32 CursorX,CursorY;
UINT32 ConsoleWidth,ConsoleHeight;
UINT32 ProgressX,ProgressY;
UINT32 ProgressCount,ProgressDelay;
UINT32 FontWidth,FontHeight,FontCharLength;
VIRTUAL_PTR BmpBack;
VIRTUAL_PTR BmpProgress[16];
RGBQUAD *BmpPallete;
BYTE *Font;
KSPINLOCK Lock=0;

unsigned char REGS_640X480X16[] =
    {
        /* MISC */
        0xE3,
        /* STATUS */
        0x00,
        /* SEQ */
        0x03, 0x01, 0x0F, 0x00, 0x06,
        /* CRTC */
        0x5F, 0x4F, 0x50, 0x82, 0x54, 0x80, 0x0B, 0x3E,
        0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x59,
        0xEA, 0x8C, 0xDF, 0x28, 0x00, 0xE7, 0x04, 0xE3,
        0xFF,
        /* GC */
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x0F,
        0xFF,
        /* AC */
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
        0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
        0x01, 0x00, 0x0F, 0x00, 0x00
    };

KESTATUS STDCALL KeBootVideoWriteRegs(BYTE *regs)
{
    UINT32 i;

    _outp(VGA_MISC_WRITE, *regs);           //write MISCELLANEOUS reg
    regs++;

    _outp(VGA_INSTAT_READ, *regs);          //write STATUS reg
    regs++;

    for (i = 0; i < VGA_NUM_SEQ_REGS; i++)  //write SEQUENCER regs
    {
        _outp(VGA_SEQ_INDEX, i);
        _outp(VGA_SEQ_DATA, *regs);
        regs++;
    }

    _outp(VGA_CRTC_INDEX, 0x03);            //unlock CRTC registers
    _outp(VGA_CRTC_DATA, _inp(VGA_CRTC_DATA) | 0x80);
    _outp(VGA_CRTC_INDEX, 0x11);
    _outp(VGA_CRTC_DATA, _inp(VGA_CRTC_DATA) & ~0x80);

    regs[0x03] |= 0x80;                     //make sure they remain unlocked
    regs[0x11] &= ~0x80;

    for (i = 0; i < VGA_NUM_CRTC_REGS; i++) //write CRTC regs
    {
        _outp(VGA_CRTC_INDEX, i);
        _outp(VGA_CRTC_DATA, *regs);
        regs++;
    }

    for (i = 0; i < VGA_NUM_GC_REGS; i++)   //write GRAPHICS CONTROLLER regs
    {
        _outp(VGA_GC_INDEX, i);
        _outp(VGA_GC_DATA, *regs);
        regs++;
    }

    for (i = 0; i < VGA_NUM_AC_REGS; i++)   //write ATTRIBUTE CONTROLLER regs
    {
        (void)_inp(VGA_INSTAT_READ);
        _outp(VGA_AC_INDEX, i);
        _outp(VGA_AC_WRITE, *regs);
        regs++;
    }

    (void)_inp(VGA_INSTAT_READ);            //lock 16-color palette and unblank display
    _outp(VGA_AC_INDEX, 0x20);

    return STATUS_SUCCESS;
}

KESTATUS STDCALL KeBootVideoClearScreen()
{
    _outp(VGA_SEQ_INDEX, 2);
    _outp(VGA_SEQ_DATA, 0xF);
    memset(FrameBuffer,0,640*480/8);

    memset(local_panes0,0,4*640*480/8);
    memset(local_panes1,0,4*640*480/8);

    return STATUS_SUCCESS;
}

KESTATUS STDCALL KeBootVideoPutpixel(UINT32 x, UINT32 y, BYTE c)
{
    UINT32 off;
    UINT32 mask;

    off = (y<<4) + (y<<2) + (x>>5); //y*640/32 + x/32

    mask = (0x80 >> (x & 0x07)) << (x & 0x18);

	//KeDebugPrintf("\n %X %X %X %X %X %X",0xabcdef,x,y,off,mask,0xabcdef);

    if (c & 0x1)
    {
        local_panes1[0][off] |= mask;
    }
    else
    {
        local_panes1[0][off] &= ~mask;
    }

    if (c & 0x2)
    {
        local_panes1[1][off] |= mask;
    }
    else
    {
        local_panes1[1][off] &= ~mask;
    }

    if (c & 0x4)
    {
        local_panes1[2][off] |= mask;
    }
    else
    {
        local_panes1[2][off] &= ~mask;
    }

    if (c & 0x8)
    {
        local_panes1[3][off] |= mask;
    }
    else
    {
        local_panes1[3][off] &= ~mask;
    }

    return STATUS_SUCCESS;
}

KESTATUS STDCALL KeBootVideoVLine(UINT32 x, UINT32 y,UINT32 Height, BYTE c)
{
    UINT32 cy;
    UINT32 i;
    cy = Height;

    if (x >= 640)
    {
        return STATUS_ERROR;
    }

    if (y >= 480)
    {
        return STATUS_ERROR;
    }

    if (y+cy > 480)
    {
        cy = 480 - y;
    }

    for (i=y;i < y+cy;i++)
    {
        KeBootVideoPutpixel(x,i,c);
    }

    return STATUS_SUCCESS;
}

KESTATUS STDCALL KeBootVideoHLine(UINT32 x, UINT32 y,UINT32 Width, BYTE c)
{
    UINT32 cx;
    UINT32 i;
    cx = Width;

    if (x >= 640)
    {
        return STATUS_ERROR;
    }

    if (y >= 480)
    {
        return STATUS_ERROR;
    }

    if (x+cx > 640)
    {
        cx = 640 - x;
    }

    for (i=x;i < x+cx;i++)
    {
        KeBootVideoPutpixel(i,y,c);
    }

    return STATUS_SUCCESS;
}

KESTATUS STDCALL KeBootVideoRect(UINT32 x, UINT32 y,UINT32 Width, UINT32 Height, BYTE c)
{
    KeBootVideoHLine(x,y,Width,c);
    KeBootVideoVLine(x,y,Height,c);
    KeBootVideoVLine(x+Width,y,Height,c);
    KeBootVideoHLine(x,y+Height,Width,c);
    return STATUS_SUCCESS;
}

KESTATUS STDCALL KeBootVideoRectFill(UINT32 x, UINT32 y,UINT32 Width, UINT32 Height, BYTE c)
{
    UINT32 cx, cy;
    UINT32 i,j;
    cx = Width;
    cy = Height;

    if (x >= 640)
    {
        return STATUS_ERROR;
    }

    if (y >= 480)
    {
        return STATUS_ERROR;
    }
    if (x+cx > 640)
    {
        cx = 640 - x;
    }

    if (y+cy > 480)
    {
        cy = 480 - y;
    }

    for (i=x;i < x+cx;i++)
    {
        for (j=y;j < y+cy;j++)
        {
            KeBootVideoPutpixel(i,j,c);
        }
    }

    return STATUS_SUCCESS;
}

KESTATUS STDCALL KeBootVideoUpdate()
{
    UINT32 i,j;
    UINT32 temp;

    //_pushfd();
    //_disable();
    //while(lock[2])
    //{
    //    KeThreadSleep(0);
    //}
    //lock[2]=1;

    //KeSpinlockLock(&Lock);

	__asm pushfd
	__asm cli

    for (i=0;i<4;i++)
    {
        _outp(VGA_GC_INDEX, 4);
        _outp(VGA_GC_DATA, i);

        _outp(VGA_SEQ_INDEX, 2);
        _outp(VGA_SEQ_DATA, 1<<i);

        for (j=0;j<(640*480/32);j++)
        {
            temp = local_panes0[i][j] ^ local_panes1[i][j];
            local_panes0[i][j] ^= temp;
            if (temp)
            {
                FrameBuffer[j] ^= temp;
            }
        }
    }

	__asm popfd

    //KeSpinlockUnlock(&Lock);

    //_popfd();
    //lock[2]=0;

    return STATUS_SUCCESS;
}

KESTATUS STDCALL KeBootVideoSetPalleteEntry(BYTE Index,BYTE Red,BYTE Green, BYTE Blue)
{
    _outp(0x03c8, Index);
    _outp(0x03c9, Red>>2);
    _outp(0x03c9, Green>>2);
    _outp(0x03c9, Blue>>2);
    return STATUS_SUCCESS;
}

KESTATUS STDCALL KeBootVideoAnimatePallete(RGBQUAD *Pallete,UINT32 Count)
{
    UINT32 j;
    double i;
    double r,b,g;
    for (i=0;i<=1;i+=0.2)
    {
        for (j=0;j<Count;j++)
        {
            r = Pallete[j].rgbRed * i;
            g = Pallete[j].rgbGreen * i;
            b = Pallete[j].rgbBlue * i;
            KeBootVideoSetPalleteEntry((BYTE)j,(BYTE)r,(BYTE)g,(BYTE)b);
            KeThreadSleep(0);
        }
    }
    return STATUS_SUCCESS;
}

KESTATUS STDCALL KeBootVideoSetNullPallete(UINT32 Count)
{
    UINT32 i;
    for (i=0;i<Count;i++)
    {
        KeBootVideoSetPalleteEntry(i,0,0,0);
    }
    return STATUS_SUCCESS;
}

KESTATUS STDCALL KeBootVideoSetPallete(RGBQUAD *Pallete,UINT32 Count)
{
    UINT32 i;
    for (i=0;i<Count;i++)
    {
        KeBootVideoSetPalleteEntry(i,Pallete[i].rgbRed,Pallete[i].rgbGreen,Pallete[i].rgbBlue);
    }
    return STATUS_SUCCESS;
}

KESTATUS STDCALL KeBootVideoSetBitmapPallete(BYTE *Bitmap)
{
    BITMAPFILEHEADER *FileHeader;
    BITMAPINFOHEADER *InfoHeader;
    RGBQUAD *Pallete;

    FileHeader = (BITMAPFILEHEADER*)Bitmap;
    InfoHeader = (BITMAPINFOHEADER*)(FileHeader + 1);
    Pallete = (RGBQUAD*)(InfoHeader + 1);

    KeBootVideoSetPallete(Pallete,16);

    return STATUS_SUCCESS;
}

KESTATUS STDCALL KeBootVideoDrawBitmap(BYTE *Bitmap,UINT32 x,UINT32 y)
{
    BITMAPFILEHEADER *FileHeader;
    BITMAPINFOHEADER *InfoHeader;
    RGBQUAD *Pallete;
    BYTE *Data;
    UINT32 Width,Height;
    UINT32 i,j,k,l;
    UINT32 Scanline;

    //while(lock[0])
    //{
    //    KeThreadSleep(0);
    //}
    //lock[0]=1;

    FileHeader = (BITMAPFILEHEADER*)Bitmap;
    InfoHeader = (BITMAPINFOHEADER*)(FileHeader + 1);
    Pallete = (RGBQUAD*)(InfoHeader + 1);
    Data = Bitmap + FileHeader->bfOffBits;

    Width = InfoHeader->biWidth;
    Height = InfoHeader->biHeight;

    Scanline = Width/2;
    if (Scanline & 3)
    {
        Scanline = (Scanline|3)+1;
    }

    if (InfoHeader->biBitCount!=4)
    {
        return STATUS_ERROR;
    }

    if (x >= 640)
    {
        return STATUS_ERROR;
    }

    if (y >= 480)
    {
        return STATUS_ERROR;
    }
    if (x+Width > 640)
    {
        Width = 640 - x;
    }

    if (y+Height > 480)
    {
        Height = 480 - y;
    }

    __asm pushfd
	__asm cli

    for (i=x,k=0;i < x+Width;i++,k++)
    {
        for (j=y,l=Height-1;j < y+Height;j++,l--)
        {
            if (i & 1)
            {
                KeBootVideoPutpixel(i,j,Data[l*Scanline + k/2] & 0xF);
            }
            else
            {
                KeBootVideoPutpixel(i,j,(Data[l*Scanline + k/2]>>4) & 0xF);
            }
        }
    }

    __asm popfd
    //lock[0]=0;

    return STATUS_SUCCESS;
}

KESTATUS STDCALL KeBootVideoDrawBitmapResource(BYTE *Bitmap,UINT32 x,UINT32 y)
{
    BITMAPINFOHEADER *InfoHeader;
    RGBQUAD *Pallete;
    BYTE *Data;
    UINT32 Width,Height;
    UINT32 i,j,k,l;
    UINT32 Scanline;

    InfoHeader = (BITMAPINFOHEADER*)(Bitmap);
    Pallete = (RGBQUAD*)(Bitmap + InfoHeader->biSize);
    Data = (BYTE*)((BYTE*)Pallete + 16*4);

    KeDebugSerialHexDump(Bitmap,0x100);

    Width = InfoHeader->biWidth;
    Height = InfoHeader->biHeight;

    Scanline = Width/2;
    if (Scanline & 3)
    {
        Scanline = (Scanline|3)+1;
    }

    if (InfoHeader->biBitCount!=4)
    {
        return STATUS_ERROR;
    }

    if (x >= 640)
    {
        return STATUS_ERROR;
    }

    if (y >= 480)
    {
        return STATUS_ERROR;
    }
    if (x+Width > 640)
    {
        Width = 640 - x;
    }

    if (y+Height > 480)
    {
        Height = 480 - y;
    }

    __asm pushfd
	__asm cli

    for (i=x,k=0;i < x+Width;i++,k++)
    {
        for (j=y,l=Height-1;j < y+Height;j++,l--)
        {
            if (i & 1)
            {
                KeBootVideoPutpixel(i,j,Data[l*Scanline + k/2] & 0xF);
            }
            else
            {
                KeBootVideoPutpixel(i,j,(Data[l*Scanline + k/2]>>4) & 0xF);
            }
        }
    }

    __asm popfd
    //lock[0]=0;

    return STATUS_SUCCESS;
}

KESTATUS STDCALL KeBootVideoDrawBitmapResourceEx(BYTE *Bitmap,UINT32 ix,UINT32 iy,UINT32 x,UINT32 y,UINT32 cx,UINT32 cy)
{
    BITMAPINFOHEADER *InfoHeader;
    RGBQUAD *Pallete;
    BYTE *Data;
    UINT32 Width,Height;
    UINT32 i,j,k,l;
    UINT32 Scanline;

    InfoHeader = (BITMAPINFOHEADER*)(Bitmap);
    Pallete = (RGBQUAD*)(Bitmap + InfoHeader->biSize);
    Data = (BYTE*)((BYTE*)Pallete + 16*4);

    KeDebugSerialHexDump(Bitmap,0x100);

    Width = InfoHeader->biWidth;
    Height = InfoHeader->biHeight;

    Scanline = Width/2;
    if (Scanline & 3)
    {
        Scanline = (Scanline|3)+1;
    }

    if (InfoHeader->biBitCount!=4)
    {
        return STATUS_ERROR;
    }

    if (x >= 640)
    {
        return STATUS_ERROR;
    }

    if (y >= 480)
    {
        return STATUS_ERROR;
    }

    if (x+cx > 640)
    {
        cx = 640 - x;
    }

    if (y+cy > 480)
    {
        cy = 480 - y;
    }

    if(cx > Width)
    {
        cx = Width;
    }
    if(cy > Height)
    {
        cy = Height;
    }

    __asm pushfd
	__asm cli

    for (i=x,k=ix;i < x+cx;i++,k++)
    {
        for (j=y,l=Height-(iy+1);j < y+cy;j++,l--)
        {
            if (i & 1)
            {
                KeBootVideoPutpixel(i,j,Data[l*Scanline + k/2] & 0xF);
            }
            else
            {
                KeBootVideoPutpixel(i,j,(Data[l*Scanline + k/2]>>4) & 0xF);
            }
        }
    }

    __asm popfd
    //lock[0]=0;

    return STATUS_SUCCESS;
}

KESTATUS STDCALL KeBootVideoDrawChar(BYTE *Data,UINT32 x,UINT32 y,UINT32 Width,UINT32 Height,BYTE c)
{
    UINT32 i,j;
    UINT32 byte = 0;
    BYTE Mask = 0x80;

    if (x >= 640)
    {
        return STATUS_ERROR;
    }

    if (y >= 480)
    {
        return STATUS_ERROR;
    }

    if (x+Width > 640)
    {
        return STATUS_ERROR;
    }

    if (y+Height > 480)
    {
        return STATUS_ERROR;
    }

    __asm pushfd
	__asm cli

    for (i=0;i<Height;i++)
    {
        for (j=0;j<Width;j++)
        {
            if (Data[byte] & Mask)
            {
                KeBootVideoPutpixel(x+j,y+i,c);
            }
            Mask = Mask>>1;
            if (Mask==0)
            {
                byte++;
                Mask=0x80;
            }
        }
    }

    __asm popfd

    return STATUS_SUCCESS;
}

KESTATUS STDCALL KeBootVideoClearConsole()
{
    //KeBootVideoDrawBitmapResource(BmpConsole,ConsoleBackX,ConsoleBackY);
    KeBootVideoDrawBitmapResourceEx(BmpBack,ConsoleX,ConsoleY,ConsoleX,ConsoleY,ConsoleWidth,ConsoleHeight);
    CursorX = ConsoleX;
    CursorY = ConsoleY;
    return STATUS_SUCCESS;
}

KESTATUS STDCALL KeBootVideoNewLine()
{
    //KeBootVideoDrawBitmapResource(BmpConsole,ConsoleBackX,ConsoleBackY);
    KeBootVideoDrawBitmapResourceEx(BmpBack,ConsoleX,ConsoleY,ConsoleX,ConsoleY,ConsoleWidth,ConsoleHeight);
    CursorX = ConsoleX;
    CursorY = ConsoleY;
    return STATUS_SUCCESS;
}

KESTATUS STDCALL KeBootVideoPutch(CHAR ch,BYTE c)
{
    //while(lock[1])
    //{
    //    KeThreadSleep(0);
    //}
    //lock[1]=1;

    __asm pushfd
	__asm cli

    if (CursorX >= ConsoleX+ConsoleWidth)
    {
        CursorX = ConsoleX;
        CursorY += FontHeight;
    }
    if (CursorY >= ConsoleY+ConsoleHeight)
    {
        KeBootVideoNewLine();
    }
    switch (ch)
    {
    case '\n':
        CursorX = ConsoleX;
        CursorY += FontHeight;
        if (CursorY >= ConsoleY+ConsoleHeight)
        {
            KeBootVideoNewLine();
        }
        break;
    default:
        KeBootVideoDrawChar(Font + ch*FontCharLength,CursorX,CursorY,FontWidth,FontHeight,c);
        CursorX += FontWidth;
    }
	
	__asm popfd

    //lock[1]=0;
    return STATUS_SUCCESS;
}

KESTATUS STDCALL KeBootVideoPrints(CHAR *str,BYTE c)
{
    CHAR *s=str;
    while (*s)
    {
        KeBootVideoPutch(*s,c);
        s++;
    }
    return STATUS_SUCCESS;
}

KESTATUS STDCALL KeBootVideoAnimation(VIRTUAL_PTR Arg)
{
    UINT32 i,j;
    VIRTUAL_PTR KernelBase = (VIRTUAL_PTR)0xC0000000;

    KeDebugPrintf("\nAnimation Function Called");

    while (1)
    {
        for (i=0;i<ProgressCount;i++)
        {
            KeBootVideoDrawBitmapResource(BmpProgress[i],ProgressX,ProgressY);
            KeBootVideoUpdate();
            //KeThreadSleep(ProgressDelay);
            for(j=0;j<ProgressDelay;j++) KeThreadSleep(0);
        }
    }
}

KESTATUS STDCALL KeBootVideoConsolePutch(CHAR ch)
{
    return KeBootVideoPutch(ch,14);
}

KESTATUS STDCALL KeBootVideoInit()
{
    VIRTUAL_PTR KernelBase = (VIRTUAL_PTR)0xC0000000;
    UINT32 i;
    KESTATUS status;

    KeResFindRCData(KernelBase,(VIRTUAL_PTR*)&BootVideoInfo,401,0x409);

    ConsoleX = BootVideoInfo->ConsoleX;
    ConsoleY = BootVideoInfo->ConsoleY;
    ConsoleWidth = BootVideoInfo->ConsoleWidth;
    ConsoleHeight = BootVideoInfo->ConsoleHeight;
    ProgressX = BootVideoInfo->ProgressX;
    ProgressY = BootVideoInfo->ProgressY;
    CursorX = ConsoleX;
    CursorY = ConsoleY;
    ProgressCount = BootVideoInfo->ProgressCount;
    ProgressDelay = BootVideoInfo->ProgressDelay;

    FontWidth = BootVideoInfo->FontWidth;
    FontHeight = BootVideoInfo->FontHeight;
    FontCharLength = FontWidth*FontHeight;
    if(FontCharLength & 0x7)
    {
        FontCharLength = (FontCharLength>>3)+1;
    }
    else
    {
        FontCharLength = FontCharLength>>3;
    }

    if(ProgressCount>16) ProgressCount=16;

    KeResFindBitmap(KernelBase,&BmpBack,BootVideoInfo->BgResource,0x409);
    for(i=0;i<ProgressCount;i++)
    {
        KeResFindBitmap(KernelBase,&BmpProgress[i],BootVideoInfo->ProgressResources[i],0x409);
    }
    KeResFindRCData(KernelBase,(VIRTUAL_PTR*)&BmpPallete,BootVideoInfo->PalleteResource,0x409);
    KeResFindRCData(KernelBase,(VIRTUAL_PTR*)&Font,BootVideoInfo->FontResource,0x409);

    //KeIoSpaceMap(&FrameBuffer,0xA0000,64 * 0x1000);

    KeBootVideoWriteRegs(REGS_640X480X16);
    KeBootVideoClearScreen();
    KeBootVideoSetNullPallete(16);
    KeBootVideoDrawBitmapResource(BmpBack,0,0);
    KeBootVideoUpdate();

    status = KeBootVideoSetPallete(BmpPallete,16);

    KeDebugConsoleHook(0,&KeBootVideoClearConsole);
    KeDebugConsoleHook(1,&KeBootVideoNewLine);
    KeDebugConsoleHook(2,&KeBootVideoConsolePutch);
    KeDebugConsoleHook(3,&KeBootVideoUpdate);

    KeDebugPrintf("Video Console Created");

    if(!KESTATUS_SUCCESS(status))
    {
        KeDebugPrintf("\nDrawing Failed");
    }

    KeThreadCreate(&KeBootVideoAnimation,NULL);


    //KeDebugSerialHexDump((VIRTUAL_PTR)0xD0000000,0x100000);
	
	__asm int 3
    //_int(3);

    //KeDebugSerialHexDump(0xD0000000,0x100000);

    return STATUS_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////////
//-------------------------------KernelBootVideo.h-------------------------------//
//--------------------------Copyright © Sidhant Goel, 2007-----------------------//
///////////////////////////////////////////////////////////////////////////////////
