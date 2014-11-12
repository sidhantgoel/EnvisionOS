#include <string.h>

#ifdef _MSC_VER
#pragma function(strset)
#endif  /* _MSC_VER */

char * __cdecl strset(char * string, int val)
{
    char *start = string;
    while (*string)
        *string++ = (char)val;
    return(start);
}
