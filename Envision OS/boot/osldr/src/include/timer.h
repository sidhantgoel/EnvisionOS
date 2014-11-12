#ifndef __timer_h__
#define __timer_h__

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#include <arch\arch.h>

extern volatile UINT32 TimerCount;

void TimerInit();
void TimerTimer();
void TimerEnable();
void TimerDisable();
void TimerSetSpeed(UINT32 hz);
void TimerInterrupt();

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif //__timer_h__