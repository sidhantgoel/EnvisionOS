#include "internal.h"

void KeMain(void *li);

void ep_kernel(void *li)
{
	__asm clts;
	fpinit();
	_cinit();
	KeMain(li);
	_cexit();
}