#ifndef __pic_h__
#define __pic_h__

#include <arch\arch.h>

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

extern BYTE Pic0Mask;
extern BYTE Pic1Mask;

void PicInit();
void PicVectorDisable(BYTE Vector);
void PicVectorEnable(BYTE Vector);
void PicDisable();
void PicEnable();

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif //__pic_h__