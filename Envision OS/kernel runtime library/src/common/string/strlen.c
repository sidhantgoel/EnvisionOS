#include <string.h>

#ifdef _MSC_VER
#pragma function(strlen)
#endif  /* _MSC_VER */

size_t __cdecl strlen(const char * str)
{
    const char *eos = str;
    while ( *eos++ ) ;
    return( (int)(eos - str - 1) );
}

