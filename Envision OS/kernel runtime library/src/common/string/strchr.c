#include <string.h>

char * __cdecl strchr(const char * string,int ch)
{
    while (*string && *string != (char)ch)
        string++;
    if (*string == (char)ch)
        return((char *)string);
    return(NULL);
}
