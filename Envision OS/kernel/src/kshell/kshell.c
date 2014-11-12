#include <kernel.h>
#include "kshell.h"
//#include <rs232.h>
//#include <vm86.h>
//#include <console.h>
//#include <debug.h>
#include "video.h"
#include "fonts.h"
//#include <reboot.h>
//#include <descriptors.h>
//#include <timer.h>
#include "keyboard.h"
#include "mouse.h"
//#include <pic.h>
#include "storage.h"
#include "volume.h"
#include "windows.h"
//#include <heap.h>

#define DbgPrintf	KeDebugPrintf

void __cdecl kshell()
{
	VolInit();
	StorageProbe();
	VolList();
	KbdInit();
	VideoInit();
	WindowsInit();
	MouseInit();
}
