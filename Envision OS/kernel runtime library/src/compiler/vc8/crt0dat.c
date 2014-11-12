#include <stdlib.h>
#include "sect_attribs.h"
#include "internal.h"

static void __cdecl _initterm(_PVFV *, _PVFV *);
static int  __cdecl _initterm_e(_PIFV *, _PIFV *);

extern _CRTALLOC(".CRT$XIA") _PIFV __xi_a[];
extern _CRTALLOC(".CRT$XIZ") _PIFV __xi_z[];    /* C initializers */
extern _CRTALLOC(".CRT$XCA") _PVFV __xc_a[];
extern _CRTALLOC(".CRT$XCZ") _PVFV __xc_z[];    /* C++ initializers */
extern _CRTALLOC(".CRT$XPA") _PVFV __xp_a[];
extern _CRTALLOC(".CRT$XPZ") _PVFV __xp_z[];    /* C pre-terminators */
extern _CRTALLOC(".CRT$XTA") _PVFV __xt_a[];
extern _CRTALLOC(".CRT$XTZ") _PVFV __xt_z[];    /* C terminators */

/*
 * pointers to the start and finish of the _onexit/atexit table
 * NOTE - the pointers are stored encoded.
 */
static _PVFV onexitarray[32];
static _PVFV *onexitbegin=onexitarray;
static _PVFV *onexitend=onexitarray;


int __cdecl _cinit()
{
    int initret;

	/*
     * do C initializations
     */
    initret = _initterm_e( __xi_a, __xi_z );
    if(initret != 0) return initret;

    /*
     * do C++ initializations
     */
    _initterm( __xc_a, __xc_z );

    return 0;
}


void __cdecl _cexit()
{
    if(onexitbegin) 
	{
		while(--onexitend >= onexitbegin)
			if(*onexitend != NULL)
				(**onexitend)();
    }

    //do pre-terminators
    _initterm(__xp_a, __xp_z);

	//do terminators
    _initterm(__xt_a, __xt_z);
}


static void __cdecl _initterm(_PVFV * pfbegin, _PVFV * pfend)
{
        while ( pfbegin < pfend )
        {
            if (*pfbegin != NULL) (**pfbegin)();
            ++pfbegin;
        }
}

static int __cdecl _initterm_e(_PIFV * pfbegin, _PIFV * pfend)
{
        int ret = 0;
        while ( pfbegin < pfend  && ret == 0)
        {
            if (*pfbegin != NULL) ret = (**pfbegin)();
            ++pfbegin;
        }

        return ret;
}

int __cdecl atexit(_PVFV fn)
{
        if (32*4 < ((int)onexitend-(int)onexitbegin)+4)
                return 1;
        else
                *(onexitend++) = fn;
        return 0;
}
