#include <string.h>

#ifdef _MSC_VER
#pragma function(strcpy)
#endif  /* _MSC_VER */

char * __cdecl strcpy(char * dst, const char * src)
{
    char * cp = dst;
    while ( *cp++ = *src++ )
        ;               /* Copy src over dst */
    return( dst );
}

