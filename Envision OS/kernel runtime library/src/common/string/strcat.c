#include <string.h>

#ifdef _MSC_VER
#pragma function(strcat)
#endif  /* _MSC_VER */

char * __cdecl strcat(char * dst, const char * src)
{
    char * cp = dst;
    while ( *cp )
        cp++;                   /* find end of dst */
    while ( *cp++ = *src++ ) ;       /* Copy src to end of dst */
    return( dst );                  /* return dst */

}


