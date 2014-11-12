#include "video.h"
#include <KernelMemoryManager.h>

UINT32 BitsPerPixel;
BYTE *VideoMem;
UINT32 XRes;
UINT32 YRes;
UINT32 LineSize;

UINT32 RedRShift=0;
UINT32 RedLShift=0;
UINT32 BlueRShift=0;
UINT32 BlueLShift=0;
UINT32 GreenRShift=0;
UINT32 GreenLShift=0;

UINT32 STDCALL DrawVGradient(UINT32 x, UINT32 y, UINT32 cx, UINT32 cy, COLOR clr1, COLOR clr2)
{
	UINT32 i,j;
	BYTE r1,r2,g1,g2,b1,b2;
	BYTE r,g,b;
	BYTE dr,dg,db;
	r1 = RGB_R(clr1);
	r2 = RGB_R(clr2);
	g1 = RGB_G(clr1);
	g2 = RGB_G(clr2);
	b1 = RGB_B(clr1);
	b2 = RGB_B(clr2);
	dr = (r2 > r1) ? (r2 - r1) : (r1 - r2);
	dg = (g2 > g1) ? (g2 - g1) : (g1 - g2);
	db = (b2 > b1) ? (b2 - b1) : (b1 - b2);

	for(i=0;i<cy;i++)
	{
		r = (r2 > r1) ? (r1 + (dr*i)/(cy-1)) : (r1 - (dr*i)/(cy-1));
		g = (g2 > g1) ? (g1 + (dg*i)/(cy-1)) : (g1 - (dg*i)/(cy-1));
		b = (b2 > b1) ? (b1 + (db*i)/(cy-1)) : (b1 - (db*i)/(cy-1));
		DrawHLine(x,y+i,cx,RGB(r,g,b));
	}
	return 0;
}

UINT32 STDCALL DrawHGradient(UINT32 x, UINT32 y, UINT32 cx, UINT32 cy, COLOR clr1, COLOR clr2)
{
	UINT32 i,j;
	BYTE r1,r2,g1,g2,b1,b2;
	BYTE r,g,b;
	BYTE dr,dg,db;
	r1 = RGB_R(clr1);
	r2 = RGB_R(clr2);
	g1 = RGB_G(clr1);
	g2 = RGB_G(clr2);
	b1 = RGB_B(clr1);
	b2 = RGB_B(clr2);
	dr = (r2 > r1) ? (r2 - r1) : (r1 - r2);
	dg = (g2 > g1) ? (g2 - g1) : (g1 - g2);
	db = (b2 > b1) ? (b2 - b1) : (b1 - b2);

	for(i=0;i<cx;i++)
	{
		r = (r2 > r1) ? (r1 + (dr*i)/(cx-1)) : (r1 - (dr*i)/(cx-1));
		g = (g2 > g1) ? (g1 + (dg*i)/(cx-1)) : (g1 - (dg*i)/(cx-1));
		b = (b2 > b1) ? (b1 + (db*i)/(cx-1)) : (b1 - (db*i)/(cx-1));
		DrawVLine(x+i,y,cy,RGB(r,g,b));
	}
	return 0;
}

UINT32 STDCALL DrawRect(UINT32 x, UINT32 y, UINT32 cx, UINT32 cy, UINT32 width, COLOR clr)
{
	DrawSolidRect(x,y,width,cy,clr);
	DrawSolidRect(x+cx-width,y,width,cy,clr);
	DrawSolidRect(x,y,cx,width,clr);
	DrawSolidRect(x,y+cy-width,cx,width,clr);
	return 0;
}

UINT32 STDCALL DrawSolidRect(UINT32 x, UINT32 y, UINT32 cx, UINT32 cy, COLOR clr)
{
	UINT32 i,j,m;
	m = ((RGB_R(clr)>>RedRShift)<<RedLShift)|((RGB_G(clr)>>GreenRShift)<<GreenLShift)|((RGB_B(clr)>>BlueRShift)<<BlueLShift);
	if(BitsPerPixel==24)
	{
		for(i=y;i<y+cy;i++)
		{
			for(j=x;j<x+cx;j++)
			{
				*(VideoMem + (LineSize*i) + (3*j) + 0) = *(((BYTE*)&m) + 0);
				*(VideoMem + (LineSize*i) + (3*j) + 1) = *(((BYTE*)&m) + 1);
				*(VideoMem + (LineSize*i) + (3*j) + 2) = *(((BYTE*)&m) + 2);
			}
		}
	}
	else
	{
		for(i=0;i<YRes;i++)
		{
			for(j=0;j<XRes;j++)
			{
				*(UINT32*)(VideoMem + (LineSize*i) + (4*j)) = m;
			}
		}
	}
	return 0;
}

UINT32 STDCALL DrawPixel(UINT32 x, UINT32 y, COLOR clr)
{
	UINT32 m = ((RGB_R(clr)>>RedRShift)<<RedLShift)|((RGB_G(clr)>>GreenRShift)<<GreenLShift)|((RGB_B(clr)>>BlueRShift)<<BlueLShift);
	if(x>=XRes || y>=YRes) return 0;
	if(BitsPerPixel==24)
	{
		*(UINT16*)(VideoMem + (LineSize*y) + (3*x) + 0) = *((UINT16*)&m);
		*(VideoMem + (LineSize*y) + (3*x) + 2) = *(((BYTE*)&m) + 2);
	}
	else
	{
		*(UINT32*)(VideoMem + (LineSize*y) + (4*x)) = m;
	}
	return 0;
}

UINT32 STDCALL DrawHLine(UINT32 x, UINT32 y, UINT32 width, COLOR clr)
{
	UINT32 i;
	for(i=0;i<width;i++)
	{
		DrawPixel(x+i,y,clr);
	}
	return 0;
}

UINT32 STDCALL DrawVLine(UINT32 x, UINT32 y, UINT32 height, COLOR clr)
{
	UINT32 i;
	for(i=0;i<height;i++)
	{
		DrawPixel(x,y+i,clr);
	}
	return 0;
}

UINT32 STDCALL DrawBitmap(UINT32 x,UINT32 y,UINT32 cx, UINT32 cy, BYTE* bmp, COLOR clr)
{
	UINT32 /*m,*/i,j;
	BYTE *dbyte=bmp;
	BYTE mask=0x80;
	//m = ((RGB_R(clr)>>RedRShift)<<RedLShift)|((RGB_G(clr)>>GreenRShift)<<GreenLShift)|((RGB_B(clr)>>BlueRShift)<<BlueLShift);
	for(i=0;i<cy;i++)
	{
		for(j=0;j<cx;j++)
		{
			if((*dbyte) & mask)
			{
				DrawPixel(x+j,y+i,clr);
			}
			mask = mask>>1;
			if(mask==0)
			{
				mask=0x80;
				dbyte++;
			}
		}
	}
	return 0;
}

UINT32 STDCALL DrawChar(UINT32 x, UINT32 y, CHAR ch, COLOR clr, Font *pFont)
{
	BYTE *dbyte = pFont->FontData + pFont->OffsetFont + pFont->BytesPerChar*ch;
	UINT32 width = pFont->CharWidth;
	UINT32 height = pFont->CharHeight;
	DrawBitmap(x,y,width,height,dbyte,clr);
	return 0;
}

UINT32 STDCALL DrawString(UINT32 x, UINT32 y,CHAR *str, COLOR clr, Font *pFont)
{
	CHAR *s = str;
	UINT32 cx=x;
	while(*s)
	{
		DrawChar(cx,y,*s,clr,pFont);
		cx += pFont->CharWidth;
		if(cx>XRes) return 0;
		s++;
	}
	return 0;
}


UINT32 VideoInit()
{
	XRes = LoaderInfo->VideoMode.ModeWidth;
	YRes = LoaderInfo->VideoMode.ModeHeight;
	LineSize = LoaderInfo->VideoMode.ScanLineSize;
	BitsPerPixel = LoaderInfo->VideoMode.BitsPerPixel;
	RedRShift = RedLShift = 0;
	GreenRShift = GreenLShift = 0;
	BlueRShift = BlueLShift = 0;
	if(LoaderInfo->VideoMode.RedMaskSize < 8) RedRShift = 8 - LoaderInfo->VideoMode.RedMaskSize;
	if(LoaderInfo->VideoMode.GreenMaskSize < 8) GreenRShift = 8 - LoaderInfo->VideoMode.GreenMaskSize;
	if(LoaderInfo->VideoMode.BlueMaskSize < 8) BlueRShift = 8 - LoaderInfo->VideoMode.BlueMaskSize;
	if(LoaderInfo->VideoMode.RedMaskSize > 8) RedLShift = LoaderInfo->VideoMode.RedMaskSize - 8;
	if(LoaderInfo->VideoMode.GreenMaskSize > 8) GreenLShift = LoaderInfo->VideoMode.GreenMaskSize - 8;
	if(LoaderInfo->VideoMode.BlueMaskSize > 8) BlueLShift = LoaderInfo->VideoMode.BlueMaskSize - 8;
	RedLShift += LoaderInfo->VideoMode.RedFieldPosition;
	GreenLShift += LoaderInfo->VideoMode.GreenFieldPosition;
	BlueLShift += LoaderInfo->VideoMode.BlueFieldPosition;
	KeDebugPrintf("_22");
	//KeDebugPrintf("XRes %d, YRes %d, LineSize %d, BitsPerPixel %d\n", XRes, YRes, LineSize, BitsPerPixel);
	if(KESTATUS_SUCCESS(KeIoSpaceMap(&VideoMem, LoaderInfo->VideoMode.FrameBuffer, LoaderInfo->VideoMode.FrameBufferSize)))
	{
		KeDebugPrintf("_23");
		//KeDebugPrintf("Mapping %X %X %X", (UINT32)LoaderInfo->VideoMode.FrameBuffer, VideoMem, LoaderInfo->VideoMode.FrameBufferSize);
		DrawSolidRect(0,0,XRes,YRes,0x000000);
		KeDebugPrintf("_24");
	}
	else
	{
		return STATUS_ERROR;
		//KeDebugPrintf("Mapping Failed");
	}
	//KeDebugPrintf("Done!");
	return STATUS_SUCCESS;
}

extern Font Font1;
extern Font Font2;
extern Font Font3;
extern Font Font4;
extern Font Font5;

int X_POS=0;
int Y_POS=0;

void _Putch(char ch)
{
	if(ch=='\n')
	{
		X_POS=0;
		Y_POS+=12;
		if(Y_POS>=YRes)
		{
			Y_POS=0;
			DrawVGradient(0,0,XRes,YRes,0xFFFFFF,0xFF0000);
		}
		return;
	}
	DrawChar(X_POS,Y_POS,ch,0xFF,&Font3);
	X_POS+=8;
	if(X_POS>=XRes)
	{
		X_POS=0;
		Y_POS+=12;
		if(Y_POS>=YRes)
		{
			Y_POS=0;
			DrawVGradient(0,0,XRes,YRes,0xFFFFFF,0xFF0000);
		}
	}
}

void _Puts(char *str)
{
	char *s=str;
	while(*s)
	{
		_Putch(*s);
		s++;
	}
}