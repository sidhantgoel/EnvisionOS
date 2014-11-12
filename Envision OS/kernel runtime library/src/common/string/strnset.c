#include <string.h>

char * __cdecl strnset(char * string, int val, size_t count)
{
    char *start = string;
    while (count-- && *string)
        *string++ = (char)val;
    return(start);
}
