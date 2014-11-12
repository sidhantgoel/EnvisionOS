#include "internal.h"

void __cdecl ldr_main(void* config);

void ep_osldr(void* config)
{
	_cinit();
	ldr_main(config);
	_cexit();
}