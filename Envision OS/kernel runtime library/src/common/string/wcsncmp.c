#include <string.h>

int __cdecl wcsncmp(const wchar_t * first, const wchar_t * last, size_t count)
{
    if (!count)
        return(0);
    while (--count && *first && *first == *last)
    {
        first++;
        last++;
    }
    return((int)(*first - *last));
}
