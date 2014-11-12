#include <string.h>

#ifdef _MSC_VER
#pragma function(wcscat)
#endif  /* _MSC_VER */

wchar_t * __cdecl wcscat(wchar_t * dst, const wchar_t * src)
{
    wchar_t * cp = dst;
    while ( *cp )
        cp++;                   /* find end of dst */
    while ( *cp++ = *src++ ) ;       /* Copy src to end of dst */
    return( dst );                  /* return dst */
}
