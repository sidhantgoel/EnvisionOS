#include <arch\arch.h>
#include <kernel.h>
#include <osldr.h>
#include <rs232.h>
#include <vm86.h>
#include <console.h>
#include <debug.h>
#include <video.h>
#include <fonts.h>
#include <reboot.h>
#include <descriptors.h>
#include <timer.h>
#include <keyboard.h>
#include <pic.h>
#include <storage.h>
#include <volume.h>
#include <heap.h>

#pragma warning(disable : 4311 4312 4313 4018 4013)

void __cdecl ldr_logo()
{
    kboot_info *bi;
	unsigned char ch;
    UINT32 e;

    HeapInit();
	ConsoleInit();
    TxtSetFlags(BGCOLOR_BLACK|FGCOLOR_GREEN);
	TxtClearScreen();
    SerialInit(); 
	PicInit();
	DescriptorsInit();
    InterruptsInit();
    ExceptionsInit();
    DbgInit();
	_enable();
	IrqInit();
	TssInit();
	Vm86Init();
	TimerInit();
	KbdInit();
    //DbgStepEnable();
	StorageProbe();
    e = VolReadFile("A:\\OSLDR.EXE", (VPTR)0x300000);
    DbgPrintf("%s\n", DbgStatusStr(e));
	if(VideoInit())
	{
		DbgPrintf("\nError Initializing Video Subsystem");
	}
	while(1)
	{
		ch=KbdGetKey();
		_Putch(ch);
		if(ch==33) KbdCpuReset();
	}
    while(1);
}
