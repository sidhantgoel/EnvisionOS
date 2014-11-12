#include <string.h>

#ifdef _MSC_VER
#pragma function(memcpy)
#endif  /* _MSC_VER */

void * __cdecl memcpy(void * dst, const void * src, size_t count)
{
    void * ret = dst;
    while(count--)
    {
        *(char *)dst = *(char *)src;
        dst = (char *)dst + 1;
        src = (char *)src + 1;
    }
    return(ret);
}

