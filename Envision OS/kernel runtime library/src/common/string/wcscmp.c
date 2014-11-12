#include <string.h>

#ifdef _MSC_VER
#pragma function(wcscmp)
#endif  /* _MSC_VER */

int __cdecl wcscmp(const wchar_t * src, const wchar_t * dst)
{
    int ret = 0 ;
    while ( ! (ret = (int)(*src - *dst)) && *dst)
        ++src, ++dst;
    if ( ret < 0 )
        ret = -1 ;
    else if ( ret > 0 )
        ret = 1 ;
    return( ret );
}
