#include <reboot.h>


void Reboot(int Warm)
{
    unsigned char temp;

	_disable();

	if(Warm) *(UINT16*)(0x0472) = 0x1234;
	else *(UINT16*)(0x0472) = 0x0000;

    KbdCpuReset();
}

void Reset(unsigned short ResetFlags, unsigned char ResetCode, unsigned int RestartAddr)
{
    unsigned char temp;

	_disable();
	
	*(UINT32*)(0x0467) = RestartAddr;
	*(UINT16*)(0x0472) = ResetFlags;

	outb(0x70,0x0F);
	outb(0x71,ResetCode);
	
    KbdCpuReset();
}

