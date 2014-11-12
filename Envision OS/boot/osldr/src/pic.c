#include <pic.h>

#define PIC1            0x20           /* IO base address for master PIC */
#define PIC2            0xA0           /* IO base address for slave PIC */
#define PIC1_COMMAND    PIC1
#define PIC1_DATA       (PIC1+1)
#define PIC2_COMMAND    PIC2
#define PIC2_DATA       (PIC2+1)
#define PIC_EOI         0x20            /* End - of - interrupt command code */

#define ICW1_ICW4       0x01            /* ICW4 (not) needed */
#define ICW1_SINGLE     0x02            /* Single (cascade) mode */
#define ICW1_INTERVAL4  0x04            /* Call address interval 4 (8) */
#define ICW1_LEVEL      0x08            /* Level triggered (edge) mode */
#define ICW1_INIT       0x10            /* Initialization - required! */

#define ICW4_8086       0x01            /* 8086/88 (MCS-80/85) mode */
#define ICW4_AUTO       0x02            /* Auto (normal) EOI */
#define ICW4_BUF_SLAVE  0x08            /* Buffered mode/slave */
#define ICW4_BUF_MASTER 0x0C            /* Buffered mode/master */
#define ICW4_SFNM       0x10            /* Special fully nested (not) */

BYTE Pic0Mask;
BYTE Pic1Mask;

void PicRemap(UINT32 offset1, UINT32 offset2)
{
    _outp(PIC1_COMMAND, ICW1_INIT+ICW1_ICW4);  // starts the initialization sequence
    //io_wait();
    _outp(PIC2_COMMAND, ICW1_INIT+ICW1_ICW4);
    //io_wait();
    _outp(PIC1_DATA, offset1);                 // define the PIC vectors
    //io_wait();
    _outp(PIC2_DATA, offset2);
    //io_wait();
    _outp(PIC1_DATA, 4);                       // continue initialization sequence
    //io_wait();
    _outp(PIC2_DATA, 2);
    //io_wait();

    _outp(PIC1_DATA, ICW4_8086);
    //io_wait();
    _outp(PIC2_DATA, ICW4_8086);
    //io_wait();

	Pic0Mask = _inp(PIC1_DATA);
	Pic1Mask = _inp(PIC2_DATA);

    _outp(PIC1_DATA, 0xFF);
    _outp(PIC2_DATA, 0xFF);
}

void PicEnable()
{
    _outp(PIC1_DATA, 0);
    _outp(PIC2_DATA, 0);
}

void PicDisable()
{
    _outp(PIC1_DATA, 0xFF);
    _outp(PIC2_DATA, 0xFF);
}

void PicVectorEnable(BYTE Vector)
{
    BYTE Mask;
    BYTE a1;//,a2;
    a1=_inp(PIC1_DATA);
	Mask = ~(1<<Vector);
	a1 = a1 & Mask;
	_outp(PIC1_DATA, a1);   // restore saved masks.
}

void PicVectorDisable(BYTE Vector)
{
    BYTE Mask;
    BYTE a1;//,a2;
    a1=_inp(PIC1_DATA);
	Mask = 1<<Vector;
	a1 = a1 | Mask;
	_outp(PIC1_DATA, a1);   // restore saved masks.
}

void PicInit()
{
	PicRemap(0x20, 0x28);
}