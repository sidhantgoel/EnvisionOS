#ifndef __loader_pe32_h__
#define __loader_pe32_h__

//#include <arch\arch.h>
#include <kernel.h>

int ldrpe32_MapPE(void *base,void *start,void *end);
int ldrpe32_FixPE(void *base);


#endif // __loader_pe32_h__
