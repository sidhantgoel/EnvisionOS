#ifndef __reboot_h__
#define __reboot_h__
#include <arch\arch.h>

void Reboot(int Warm);
void Reset(unsigned short ResetFlags, unsigned char ResetCode, unsigned int RestartAddr);

#endif //__reboot_h__