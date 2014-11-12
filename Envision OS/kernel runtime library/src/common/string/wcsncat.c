#include <string.h>

wchar_t * __cdecl wcsncat(wchar_t * front, const wchar_t * back, size_t count)
{
    wchar_t *start = front;
    while (*front++)
        ;
    front--;
    while (count--)
        if (!(*front++ = *back++))
            return(start);
    *front = L'\0';
    return(start);
}
