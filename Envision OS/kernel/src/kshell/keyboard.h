#ifndef __keyboard_h__
#define __keyboard_h__

#include <kernel.h>

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

void KeKeyboardIrq();
void KbdCpuReset();
void KbdKbd();
int KbdInit();
char KbdGetKey();

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif //__keyboard_h__