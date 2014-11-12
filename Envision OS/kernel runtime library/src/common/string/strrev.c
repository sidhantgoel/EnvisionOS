#include <string.h>

char * __cdecl strrev(char * string)
{
    char *start = string;
    char *left = string;
    char ch;
    while (*string++)                 /* find end of string */
        ;
    string -= 2;
    while (left < string)
    {
        ch = *left;
        *left++ = *string;
        *string-- = ch;
    }
    return(start);
}
