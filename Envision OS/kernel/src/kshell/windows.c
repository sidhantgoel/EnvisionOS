#include "windows.h"
#include "volume.h"
#include "video.h"
#include <KernelDebug.h>
#include <KernelMemoryManager.h>

POINTER *Pointer;

WINDOW	*Desktop;
BYTE *VideoBuffer;
BYTE *UpdateRgn;

UINT32 posX, posY;
UINT32 ScreenWidth, ScreenHeight;

void HideMouse()
{
	UINT32 ptrX, ptrY, i, j, LineSz;
	ptrX = posX - Pointer->HotX;
	ptrY = posY - Pointer->HotY;
	LineSz = (((Pointer->Width-1)|0x07)+1)>>3;
	for(i=0;i<Pointer->Width;i++)
	{
		for(j=0;j<Pointer->Height;j++)
		{
			if(Pointer->Bitmap[LineSz*j+(i>>3)] & (1<<(i & 0x07)))
			{
				DrawPixel(ptrX+i, ptrY+j, RGB(VideoBuffer[3*ScreenWidth*(j+ptrY)+3*(i+ptrX)+0], VideoBuffer[3*ScreenWidth*(j+ptrY)+3*(i+ptrX)+1], VideoBuffer[3*ScreenWidth*(j+ptrY)+3*(i+ptrX)+2]));
			}
		}
	}
}

void DrawMouse()
{
	UINT32 ptrX, ptrY, i, j, LineSz;
	ptrX = posX - Pointer->HotX;
	ptrY = posY - Pointer->HotY;
	LineSz = (((Pointer->Width-1)|0x07)+1)>>3;
	for(i=0;i<Pointer->Width;i++)
	{
		for(j=0;j<Pointer->Height;j++)
		{
			if(Pointer->Bitmap[LineSz*j+(i>>3)] & (1<<(i & 0x07)))
			{
				DrawPixel(ptrX+i, ptrY+j, RGB(Pointer->Image[3*Pointer->Width*j+3*i+0], Pointer->Image[3*Pointer->Width*j+3*i+1], Pointer->Image[3*Pointer->Width*j+3*i+2]));
			}
		}
	}
}

UINT32 STDCALL BeginPaint(WINDOW *Window, DRAWCONTEXT *Context)
{
	UINT32 x, y;
	WINDOW *w = Window;
	x = y = 0;
	while(w)
	{
		x += w->x;
		y += w->y;
		w = w->Parent;
	}
	Context->Window = Window;
	Context->left = x;
	Context->top = y;
	Context->width = Window->cx;
	Context->height = Window->cy;
	return 0;
}

UINT32 STDCALL UpdateScreen()
{
	UINT32 i, j;
	for(i=0;i<ScreenHeight;i++)
	{
		for(j=0;j<ScreenWidth;j++)
		{
			if(UpdateRgn[(i*ScreenWidth+j)>>3] & (1<<((i*ScreenWidth+j) & 0x07)))
			{
				DrawPixel(j, i, RGB(VideoBuffer[i*ScreenWidth*3+j*3+0], VideoBuffer[i*ScreenWidth*3+j*3+1], VideoBuffer[i*ScreenWidth*3+j*3+2]));
			}
		}
	}
	memset(UpdateRgn, 0, (ScreenWidth*ScreenHeight)>>3);
}

UINT32 STDCALL EndPaint(DRAWCONTEXT *Context)
{
	WINDOW *w = Context->Window;
	UpdateScreen();
	w = w->FirstChild;
	while(w)
	{
		ReDrawWindow(w);
		w = w->Next;
	}
	return 0;
}

UINT32 STDCALL ReDrawWindow(WINDOW *Window)
{
	UINT32 i;
	WINDOW *w = Window;
	i = (*w->Draw)(w);
	/*w = Window->FirstChild;
	if(i==0)
	while(w)
	{
		ReDrawWindow(w);
		w = w->Next;
	}
	return 0;*/
	return i;
}

UINT32 STDCALL MouseMoveWindow(WINDOW *Window, UINT32 left, UINT32 top, UINT32 x, UINT32 y)
{
	UINT32 i;
	WINDOW *w = Window;
	i = (*w->MouseMove)(w, x-left, y-top);
	w = Window->FirstChild;
	if(i==0)
	while(w)
	{
		MouseMoveWindow(w, left+w->x, top+w->y, x, y);
		w = w->Next;
	}
	return 0;
}

UINT32 STDCALL MouseDownWindow(WINDOW *Window, UINT32 left, UINT32 top, UINT32 x, UINT32 y, UINT32 btn)
{
	UINT32 i;
	WINDOW *w = Window;
	i = (*w->MouseDown)(w, x-left, y-top, btn);
	w = Window->FirstChild;
	if(i==0)
	while(w)
	{
		MouseDownWindow(w, left+w->x, top+w->y, x, y, btn);
		w = w->Next;
	}
	return 0;
}

UINT32 STDCALL MouseUpWindow(WINDOW *Window, UINT32 left, UINT32 top, UINT32 x, UINT32 y, UINT32 btn)
{
	UINT32 i;
	WINDOW *w = Window;
	i = (*w->MouseUp)(w, x-left, y-top, btn);
	w = Window->FirstChild;
	if(i==0)
	while(w)
	{
		MouseUpWindow(w, left+w->x, top+w->y, x, y, btn);
		w = w->Next;
	}
	return 0;
}

/*void STDCALL InvalidateRgn(UINT32 x, UINT32 y, UINT32 cx, UINT32 cy)
{
	UINT32 i, j;
	for(i=0;i<cx;i++)
	{
		for(j=0;j<cy;j++)
		{
			if((x+i)>=0 && (x+i)<ScreenWidth && (y+j)>=0 && (y+j)<ScreenHeight)
			UpdateRgn[(ScreenWidth*(y+j)+(x+i))>>3] |= 1<<(((ScreenWidth*(y+j)+(x+i))) & 0x07);
		}
	}
	ReDrawWindow(Desktop);
	memset(UpdateRgn, 0, (ScreenWidth*ScreenHeight)>>3);
}*/

void STDCALL MouseMove(int x, int y)
{
	HideMouse();
	posX = x;
	posY = y;
	MouseMoveWindow(Desktop, 0, 0, posX, posY);
	DrawMouse();
}

void STDCALL MouseDown(int button)
{
	MouseDownWindow(Desktop, 0, 0, posX, posY, button);
}

void STDCALL MouseUp(int button)
{
	MouseUpWindow(Desktop, 0, 0, posX, posY, button);
}

BYTE PtrImage[] = {
	0x00, 0x00, 0x00,	0x00, 0x00, 0x00,	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,
	0x00, 0x00, 0x00,	0xFF, 0xFF, 0xFF,	0x00, 0x00, 0x00,	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,
	0x00, 0x00, 0x00,	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,	0x00, 0x00, 0x00,	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,
	0x00, 0x00, 0x00,	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,	0x00, 0x00, 0x00,	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,
	0x00, 0x00, 0x00,	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,	0x00, 0x00, 0x00,	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,
	0x00, 0x00, 0x00,	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,	0x00, 0x00, 0x00,	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,
	0x00, 0x00, 0x00,	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,	0x00, 0x00, 0x00,	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,
	0x00, 0x00, 0x00,	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,	0x00, 0x00, 0x00,	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,
	0x00, 0x00, 0x00,	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,	0x00, 0x00, 0x00,	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,
	0x00, 0x00, 0x00,	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,	0x00, 0x00, 0x00,	0xFF, 0xFF, 0xFF,
	0x00, 0x00, 0x00,	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,	0x00, 0x00, 0x00,	0x00, 0x00, 0x00,	0x00, 0x00, 0x00,	0x00, 0x00, 0x00,	0x00, 0x00, 0x00,
	0x00, 0x00, 0x00,	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,	0x00, 0x00, 0x00,	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,	0x00, 0x00, 0x00,	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,
	0x00, 0x00, 0x00,	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,	0x00, 0x00, 0x00,	0x00, 0x00, 0x00,	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,	0x00, 0x00, 0x00,	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,
	0x00, 0x00, 0x00,	0xFF, 0xFF, 0xFF,	0x00, 0x00, 0x00,	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,	0x00, 0x00, 0x00,	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,	0x00, 0x00, 0x00,	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,
	0x00, 0x00, 0x00,	0x00, 0x00, 0x00,	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,	0x00, 0x00, 0x00,	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,	0x00, 0x00, 0x00,	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,
	0x00, 0x00, 0x00,	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,	0x00, 0x00, 0x00,	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,	0x00, 0x00, 0x00,	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,	0x00, 0x00, 0x00,	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,	0x00, 0x00, 0x00,	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,	0x00, 0x00, 0x00,	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,	0x00, 0x00, 0x00,	0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,	0x00, 0x00, 0x00,	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,	0x00, 0x00, 0x00,	0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF,	0x00, 0x00, 0x00,	0x00, 0x00, 0x00,	0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF
	};

BYTE PtrBitmap[] = {
	0x03, 0x00,
	//0b00000011, 0b00000000,
	0x07, 0x00,
	//0b00000101, 0b00000000,
	0x0F, 0x00,
	//0b00001001, 0b00000000,
	0x1F, 0x00,
	//0b00010001, 0b00000000,
	0x3F, 0x00,
	//0b00100001, 0b00000000,
	0x7F, 0x00,
	//0b01000001, 0b00000000,
	0xFF, 0x00,
	//0b10000001, 0b00000000,
	0xFF, 0x01,
	//0b00000001, 0b00000001,
	0xFF, 0x03,
	//0b00000001, 0b00000010,
	0xFF, 0x07,
	//0b00000001, 0b00000100,
	0xFF, 0x0F,
	//0b10000001, 0b00001111,
	0xFF, 0x00,
	//0b10010001, 0b00000000,
	0xFF, 0x00,
	//0b10011001, 0b00000000,
	0xE7, 0x01,
	//0b00100101, 0b00000001,
	0xE3, 0x01,
	//0b00100011, 0b00000001,
	0xC1, 0x03,
	//0b01000001, 0b00000010,
	0xC0, 0x03,
	//0b01000000, 0b00000010,
	0x80, 0x07,
	//0b10000000, 0b00000100,
	0x80, 0x07,
	//0b10000000, 0b00000100,
	0x00, 0x03
	//0b00000000, 0b00000011
};

unsigned char *data_bitmap;

UINT32 WindowsInit()
{
	UINT32 Size, e, i, j;
	posX = LoaderInfo->VideoMode.ModeWidth/2;
	posY = LoaderInfo->VideoMode.ModeHeight/2;
	ScreenWidth = LoaderInfo->VideoMode.ModeWidth;
	ScreenHeight = LoaderInfo->VideoMode.ModeHeight;
	Pointer = (POINTER*)malloc(sizeof(POINTER));
	Pointer->Height = 20;
	Pointer->Width = 12;
	Pointer->HotX = 0;
	Pointer->HotY = 0;
	Pointer->Image = PtrImage;
	Pointer->Bitmap = PtrBitmap;
	VideoBuffer = malloc(LoaderInfo->VideoMode.ModeHeight*LoaderInfo->VideoMode.ModeWidth*3);
	UpdateRgn = malloc(LoaderInfo->VideoMode.ModeHeight*LoaderInfo->VideoMode.ModeWidth/8);
	memset(VideoBuffer, 0xCC, LoaderInfo->VideoMode.ModeHeight*LoaderInfo->VideoMode.ModeWidth*3);
	memset(UpdateRgn, 0x00, LoaderInfo->VideoMode.ModeHeight*LoaderInfo->VideoMode.ModeWidth/8);
	data_bitmap = malloc(600*800*3);
	e = VolFileSize("A:\\WALL.DAT", &Size);
	e = VolReadFile("A:\\WALL.DAT", data_bitmap);
	Desktop = malloc(sizeof(WINDOW));
	Desktop->cx = ScreenWidth;
	Desktop->cy = ScreenHeight;
	Desktop->Data = NULL;
	Desktop->Draw = &DesktopDraw;
	Desktop->Parent = NULL;
	Desktop->FirstChild = NULL;
	Desktop->MouseDown = &DesktopMouseDown;
	Desktop->MouseMove = &DesktopMouseMove;
	Desktop->MouseUp = &DesktopMouseUp;
	Desktop->Next = NULL;
	Desktop->Prev = NULL;
	Desktop->x = 0;
	Desktop->y = 0;
	ReDrawWindow(Desktop);
	return STATUS_SUCCESS;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////   Drawing Functions  ////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

UINT32 STDCALL PutPixel(DRAWCONTEXT *Context, UINT32 x, UINT32 y, COLOR clr)
{
	//KeDebugPrintf("%X, %X, %X, %X, %X, %X\n", x, y, Context->width, Context->height, ScreenWidth, ScreenHeight);
	//if((x<Context->width) && (y<Context->height) && ((x+Context->left)<ScreenWidth) && ((y+Context->height)<ScreenHeight))
	//{
		//KeDebugPrintf("ss");
		VideoBuffer[ScreenWidth*3*(y+Context->top) + (x+Context->left)*3 + 0] = RGB_R(clr);
		VideoBuffer[ScreenWidth*3*(y+Context->top) + (x+Context->left)*3 + 1] = RGB_G(clr);
		VideoBuffer[ScreenWidth*3*(y+Context->top) + (x+Context->left)*3 + 2] = RGB_B(clr);
		UpdateRgn[((y+Context->top)*ScreenWidth+(x+Context->left))>>3] |= (1<<(((y+Context->top)*ScreenWidth+(x+Context->left)) & 0x07));
	//}
	return 0;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////   Desktop Window  ///////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

UINT32 STDCALL DesktopDraw(WINDOW *Window)
{
	UINT32 i,j;
	DRAWCONTEXT c;
	BYTE *p=data_bitmap+((ScreenHeight-1)*ScreenWidth*3);
	BeginPaint(Window, &c);
	KeDebugPrintf("LEFT %d, TOP %d\n", c.left, c.top);
	for(i=0;i<ScreenHeight;i++)
	{
		for(j=0;j<ScreenWidth;j++)
		{
			PutPixel(&c, j, i, RGB(p[0], p[1], p[2]));
			p += 3;
		}
		p -= ScreenWidth*3;
		p -= ScreenWidth*3;
	}
	EndPaint(&c);
	return 0;
}

UINT32 STDCALL DesktopMouseMove(WINDOW *Window, UINT32 x, UINT32 y)
{
	return 0;
}

UINT32 STDCALL DesktopMouseDown(WINDOW *Window, UINT32 btn)
{
	return 0;
}

UINT32 STDCALL DesktopMouseUp(WINDOW *Window, UINT32 btn)
{
	return 0;
}