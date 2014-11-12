#include <string.h>

#ifdef _MSC_VER
#pragma function(memset)
#endif  /* _MSC_VER */

void * __cdecl memset(void *dst, int val, size_t count)
{
    void *start = dst;
    while (count--)
    {
        *(char *)dst = (char)val;
        dst = (char *)dst + 1;
    }
    return(start);
}

