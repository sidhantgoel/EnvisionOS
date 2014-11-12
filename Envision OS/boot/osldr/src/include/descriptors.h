#ifndef __descriptors_h__
#define __descriptors_h__

#include <arch\arch.h>

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

void UnhandledInterrupt();
void KeHandlerDE();
void KeHandlerDB();
void KeHandlerBP();
void KeHandlerOF();
void KeHandlerBR();
void KeHandlerUD();
void KeHandlerNM();
void KeHandlerDF();
void KeHandlerTS();
void KeHandlerNP();
void KeHandlerSS();
void KeHandlerGP();
void KeHandlerPF();
void KeHandlerMF();
void KeHandlerAC();
void KeHandlerMC();
void KeHandlerXF();
void TimerIrq();
void KbdIrq();
void KeHandlerIRQ2();
void KeHandlerIRQ3();
void KeHandlerIRQ4();
void KeHandlerIRQ5();
void KeHandlerIRQ6();
void KeHandlerIRQ7();
void KeHandlerIRQ8();
void KeHandlerIRQ9();
void KeHandlerIRQ10();
void KeHandlerIRQ11();
void KeHandlerIRQ12();
void KeHandlerIRQ13();
void Ide0Irq();
void Ide1Irq();

#define SEL_RING0_CODE  0x08
#define SEL_RING0_DATA  0x10
#define SEL_RING3_CODE  0x18|SELECTOR_RPL3
#define SEL_RING3_DATA  0x20|SELECTOR_RPL3
#define SEL_KERNEL_TSS  0x28
#define SEL_VM86_TSS    0x30

BOOLEAN STDCALL DescriptorsInit();
void TssInit();
BOOLEAN STDCALL InterruptsInit();
BOOLEAN STDCALL SetIDTIntGate(UINT32 index,SELECTOR sel,UINT32 offset);
BOOLEAN STDCALL ExceptionsInit();
BOOLEAN STDCALL IrqInit();

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif //__descriptors_h__