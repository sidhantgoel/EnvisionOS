#include <stdlib.h>

typedef (__cdecl *_purecall_handler)();
_purecall_handler purecall = NULL;


void __cdecl _purecall(void)
{
    if(purecall != NULL)
    {
        purecall();
    }

    while(1);
}
