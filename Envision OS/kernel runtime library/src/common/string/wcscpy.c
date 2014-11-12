#include <string.h>

#ifdef _MSC_VER
#pragma function(wcscpy)
#endif  /* _MSC_VER */

wchar_t * __cdecl wcscpy(wchar_t * dst, const wchar_t * src)
{
    wchar_t * cp = dst;
    while ( *cp++ = *src++ )
        ;               /* Copy src over dst */
    return( dst );
}
