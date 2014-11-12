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

char mem_types[][32] = {"", "Usable (normal) RAM", "Reserved - unusable", "ACPI reclaimable memory", "ACPI NVS memory", "Area containing bad memory"};

void __cdecl ldr_main(char* config)
{
	int  i;
	kmem_map *k = (kmem_map*)(config+4);
    kboot_info *bi;
	unsigned char ch;
    UINT32 e;
	UINT32 Size;

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

	DbgPrintf("========================================================================================\n");
	DbgPrintf("\t\t\t\tOSLDR.EXE v1.0\n");
	DbgPrintf("========================================================================================\n");
	DbgPrintf("\t\t\t\tSystem Memory Map\n");
	DbgPrintf("========================================================================================\n");
	DbgPrintf("Start\t\t\t\tSize\t\t\t\tType\n");
	for(i=0;i<*(unsigned int*)config;i++)
	{
		DbgPrintf("%08X%08X\t\t%08X%08X\t\t%s\n", k[i].start_high, k[i].start_low, k[i].size_high, k[i].size_low, mem_types[k[i].type]);
	}
	DbgPrintf("========================================================================================\n\n");
	
	DbgPrintf("========================================================================================\n");
	DbgPrintf("\t\t\tInitializing Graphics, ");
	if(VideoInit())
		DbgPrintf("Failed! :-(\n");
	else
		DbgPrintf("Done! :-)\n");
	DbgPrintf("========================================================================================\n\n");
	
	DbgPrintf("========================================================================================\n");
	DbgPrintf("\t\t\tProbing Storage Devices\n");
	DbgPrintf("========================================================================================\n");
	StorageProbe();

	VolList();
	
	DbgPrintf("========================================================================================\n");
	DbgPrintf("\t\t\tLoading Kernel\n");
	DbgPrintf("========================================================================================\n");
	DbgPrintf("Reading kernel into memory...              ");
	e = VolFileSize("A:\\KERNEL.EXE", &Size);
	e = VolReadFile("A:\\KERNEL.EXE", (VPTR)0x200000);
	if(e)
	{
		_Puts("Error Loading Kernel");
		DbgPrintf("Failed %s!\n", DbgStatusStr(e));
	}
	else
	{
		_Puts("Kernel Successfully Loaded\n");
		DbgPrintf("Done!\n");
	}
	
	DbgPrintf("Fixing kernel image...                     ");
	Size = ldrpe32_FixAlign((VPTR)0x200000,(VPTR)0x200000+Size);
	DbgPrintf("Done!\n");
	
	DbgPrintf("Innitializing Page Tables...               ");
	ldrpe32_MakeTables((VPTR)0x400000);
	DbgPrintf("Done!\n");

    DbgPrintf("Mapping kernel to higher address...        ");
	ldrpe32_LoadKernel((VPTR)0x200000,(VPTR)0x200000+Size);
	DbgPrintf("Done!\n");
	
	DbgPrintf("Preparing boot info...                     ");
	bi = (kboot_info*)HeapAlloc(sizeof(kboot_info));
	bi->imap = NULL;
	bi->imap_cnt = 0;
	bi->kernel_paddr = 0x200000;
	bi->kernel_size = Size;
	bi->mmap = (kmem_map*)(config+4);
	bi->mmap_cnt = *(unsigned int *)config;
	bi->page_data = 0x400000;
	bi->page_data_size = 0x201000;
	bi->stack_paddr = 0x3FC000;
	bi->stack_size = 0x400000-0x3FC000;
	bi->stack_top = (void*)0xE0000000;
	GetModeInfo(&bi->video_mode);
	DbgPrintf("Done!\n");

	DbgPrintf("Setting up kernel stack...                 ");
    ldrpe32_SetupStack((VPTR)0x3FC000,(VPTR)0x400000);
	DbgPrintf("Done!\n");
	DbgPrintf("========================================================================================\n\n");

 	DbgPrintf("========================================================================================\n");
	DbgPrintf("All Set, Jumping to Kernel Entry Point :-)\n");
	DbgPrintf("========================================================================================\n\n");
    ldrpe32_JumpToKernel(bi);
	/*while(1)
	{
		ch=KbdGetKey();
		_Putch(ch);
		if(ch==33) KbdCpuReset();
	}*/
    while(1);
}
