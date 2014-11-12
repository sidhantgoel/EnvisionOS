#ifndef __mouse_h__
#define __mouse_h__

#include <kernel.h>

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

void KeMouseIrq();
int MouseInit();

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif //__mouse_h__