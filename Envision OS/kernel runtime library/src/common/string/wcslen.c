#include <string.h>

#ifdef _MSC_VER
#pragma function(wcslen)
#endif  /* _MSC_VER */

size_t __cdecl wcslen(const wchar_t * wcs)
{
    const wchar_t *eos = wcs;
    while ( *eos++ ) ;
    return( (size_t)(eos - wcs - 1) );
}
