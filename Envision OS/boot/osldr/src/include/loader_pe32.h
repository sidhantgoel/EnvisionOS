#ifndef __loader_pe32_h__
#define __loader_pe32_h__

#include <arch\arch.h>
#include <kernel.h>

int ldrpe32_LoadTables(void *addr);
int ldrpe32_MapPhysical(void* vaddr,void *paddr,int count);
int ldrpe32_MapPE(void *base,void *start,void *end);
int ldrpe32_FixPE(void *base);
int ldrpe32_LoadKernel(void *start,void *end);
int ldrpe32_LoadModule(void *start,void *end);
int ldrpe32_SetupStack(void *start,void *end);
int ldrpe32_JumpKernel(kboot_info *binf);


#define KERNEL_BASE     0xC0000000
#define KERNEL_STACK    0xDFFFC000
#define MODULES_BASE    0xE0000000
#define MODULES_ALIGN   0x00100000
#define IMAGE_ALIGN		0x00001000


#endif // __loader_pe32_h__
