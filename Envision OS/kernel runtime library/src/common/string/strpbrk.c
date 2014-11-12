#include <string.h>

char * __cdecl strpbrk(const char * string, const char * control)
{
        const unsigned char *str = string;
        const unsigned char *ctrl = control;

        unsigned char map[32];
        int count;

        /* Clear out bit map */
        for (count=0; count<32; count++)
                map[count] = 0;

        /* Set bits in control map */
        while (*ctrl)
        {
                map[*ctrl >> 3] |= (1 << (*ctrl & 7));
                ctrl++;
        }


        /* 1st char in control map stops search */
        while (*str)
        {
                if (map[*str >> 3] & (1 << (*str & 7)))
                        return((char *)str);
                str++;
        }
        return(NULL);
}
