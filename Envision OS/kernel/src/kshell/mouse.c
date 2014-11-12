#include "mouse.h"
#include "windows.h"
#include <kernel.h>
#include <kernelDescriptors.h>
#include <KernelApic.h>
//#include <debug.h>

#define false 0
#define true 1
#define outb			_outp
#define inb				_inp
#define inw				_inpw
#define DbgPrintf		KeDebugPrintf
#define ESUCCESS		STATUS_SUCCESS
#define EFAILED			STATUS_ERROR


#define MOUSE_DATA		0x60
//#define KBD_CMD		0x60
//#define KBD_ACK		0x61
#define MOUSE_STATUS	0x64
#define MOUSE_CONTROL	0x64

#define CTRL_READBYTE		0x20
#define CTRL_WRITEBYTE		0x60
#define CTRL_ENABLEMOUSE	0xA8
#define CTRL_DEACTIVATE		0xAD
#define CTRL_ACTIVATE		0xAE
#define CTRL_CMD			0xD4

#define MOUSE_ACK			0xFA

#define CMD_RESET			0xFF	//The mouse sends ACK (0xFA) and resets.
#define CMD_RESEND			0xFE	//this command makes the mouse sends its second last packet to the host again.
#define CMD_SETDEFAULTS		0xF6	//The mouse sends ACK (0xFA) and applies default values
#define CMD_REPORTINGOFF	0xF5	//The mouse sends ACK (0xFA) and disable data reporting.
#define CMD_REPORTINGON		0xF4	//The mouse sends ACK (0xFA) and enables data reporting
#define CMD_SETRATE			0xF3	//The mouse sends ACK (0xFA) and waits for the next data, written to the mouse, to be the sample rate
#define CMD_GETID			0xF2	//The mouse sends ACK (0xFA) and sends its ID, though the DATA port
#define CMD_SETREMOTE		0xF0	//The mouse sends ACK (0xFA) and then reset its movement counters, and enters remote mode
#define CMD_SETWRAP			0xEE	//The mouse sends ACK (0xFA) and then reset its movement counters, and enters warp mode
#define CMD_RESETWRAP		0xEC	//The mouse sends ACK, and then enters the last mode, before entering wrap mode, it also resets its movement counters
#define CMD_READDATA		0xEB	//The mouse sends ACK, followed by the first packet, and so on, until It have sent all.
#define CMD_SETSTREAM		0xEA	//The mouse sends ACK (0xFA) and then reset its movement counters, and enters reporting mode
#define CMD_STATUS			0xE9	//The mouse sends ACK, and sends afterwards the status bytes ( 3 bytes in a row, remember to wait each time for new data)
#define CMD_RESOLUTION		0xE8	//The mouse sends ACK and waits for the next data, written to the mouse, to be the resolution
#define CMD_SCALING2		0xE7	//The mouse sends ACK and set scaling 2:1
#define CMD_SCALING1		0xE6	//The mouse sends ACK and set scaling 1:1

#define STATUS_OBF			0x01
#define STATUS_IBF			0x02
#define STATUS_SELFTESTOK	0x04
#define STATUS_LOCKED		0x10
#define STATUS_GTO			0x40
#define STATUS_PERR			0x80


/*Status Byte 1 */
#define STB1_RIGHTBTN		0x01	//Right Button
#define STB1_MIDDLEBTN		0x02	//Middle Button
#define STB1_LEFTBTN		0x04	//Left Button
#define STB1_3				0x08	//Always 0
#define STB1_SCALING		0x10	//if it is 1, scaling 2:1 is enabled; if 0 then scaling 1:1 is enabled.
#define STB1_ENABLE			0x20	//if it is 1, then data reporting is enabled; if 0 then data reporting is disabled.
#define STB1_MODE			0x40	//if it is 1, the current mode is remote mode; if 0 then it is stream mode .
#define STB1_7				0x80	//Always 0

/*Status Byte 2 = Resolution */
#define RESOL_1	0x00	//1 count /mm
#define RESOL_2 0x01	//2 count /mm
#define RESOL_3	0x02	//4 count /mm
#define RESOL_4	0x03	//8 count /mm

/*Status Byte 3 = Sampling Rate */
// Value = Samples per Second


#define CMDB_EKI			0x01	//enables IRQ1 on keyboard output buffer full
#define CMDB_EIRQ12			0x02	//enables IRQ12 from PS/2 mouse when set
#define CMDB_SYS			0x04	//System Flag (same as b2 in Status register, it seems)
#define CMDB_INGNORELOCK	0x08	//ignore keyboard lock switch when set
#define CMDB_DISABLE		0x10	//disables keyboard when set
#define CMDB_DMS			0x20	//disables PS/2 mouse when set
#define CMDB_KCC			0x40	//convert set 2 scancodes to set 1 ("IBM PC compatibility mode")

#define BYTE1_LEFT			0x01	//Left button state
#define BYTE1_RIGHT			0x02	//Right button state
#define BYTE1_MIDDLE		0x04	//Middle button state
#define BYTE1_8				0x08	//Always 1
#define BYTE1_SIGNX			0x10	//X Sign
#define BYTE1_SIGNY			0x20	//Y Sign
#define BYTE1_OVRFX			0x40	//X Overflow
#define BYTE1_OVRFY			0x08	//Y Overflow

#define AUX_SEND_ID		0xF2
#define AUX_ID_ERROR		-1
#define AUX_ID_PS2		0
#define AUX_ID_IMPS2		3

void MouseOut(UINT16 port, BYTE data)
{
	UINT32 timeout;
	BYTE stat;
	for(timeout = 500000L; timeout != 0; timeout--)
	{
		stat = inb(MOUSE_STATUS);
		if((stat & STATUS_IBF) == 0) break;
	}
	if(timeout == 0)
	{
		return;
	}
	outb(port, data);
}

BYTE MouseIn()
{
	UINT32 timeout;
	BYTE stat, data;
	for(timeout = 50000000L; timeout != 0; timeout--)
	{
		stat = inb(MOUSE_STATUS);
		if(stat & STATUS_OBF)
		{
			data = inb(MOUSE_DATA);
			if((stat & (STATUS_GTO|STATUS_PERR)) == 0) return data;
		}
	}
	return 0;
}

unsigned char MouseOutIn(UINT16 port, BYTE data)
{
    unsigned int retval;
    
    MouseOut( port, data );
    retval = MouseIn();
	
    if ( retval != MOUSE_ACK )
    {
        return retval;
    }
    return 0;
}

int ReadMouseID()
{
    unsigned char id;

    MouseOut(MOUSE_CONTROL, CTRL_ENABLEMOUSE);
    MouseOutIn(MOUSE_DATA, CMD_GETID);
    id = MouseIn();

    return id;
}

int WriteToMouse( unsigned char *data, unsigned int len )
{
    int index;

    if ( len > 0 )
    {
	for ( index = 0; index < len; index++ )
	{
	    MouseOut( MOUSE_CONTROL, CTRL_CMD );
	    MouseOutIn( MOUSE_DATA, data[ index ] );
	} 
	return ESUCCESS;
    }
    return EFAILED;
}

int wheel;
int MouseX, MouseY, ButtonL, ButtonM, ButtonR;

void MouseMouse()
{
	unsigned char byte1 = 0, byte2 = 0, byte3 = 0;
	int xChange, yChange, lButton, mButton, rButton;

	// Disable keyboard output here, because our data reads are not atomic
	MouseOut(MOUSE_CONTROL, CTRL_DEACTIVATE);

	byte1 = MouseIn();
	if(byte1==-1)
	{
		MouseOut(MOUSE_CONTROL, CTRL_CMD);
		MouseOut(MOUSE_DATA, CMD_RESEND);
		MouseOut(MOUSE_CONTROL, CTRL_DEACTIVATE);
		return;
	}
	byte2 = MouseIn();
	byte3 = MouseIn();

	// Re-enable keyboard output
	MouseOut(MOUSE_CONTROL, CTRL_DEACTIVATE);

	lButton = ((byte1 & BYTE1_LEFT) == BYTE1_LEFT);
	mButton = ((byte1 & BYTE1_MIDDLE) == BYTE1_MIDDLE);
	rButton = ((byte1 & BYTE1_RIGHT) == BYTE1_RIGHT);

	if(byte1 & BYTE1_SIGNX)
		xChange = (int)(((unsigned int)~0xFF)|(byte2 & 0xFF));
	else
		xChange = (int)byte2;
	
	if(byte1 & BYTE1_SIGNY)
		yChange = (int)(((unsigned int)~0xFF)|(byte3 & 0xFF));
	else
		yChange = (int)byte3;
	MouseX += xChange;
	MouseY -= yChange;
	if(MouseX < 0) MouseX = 0;
	if(MouseY < 0) MouseY = 0;
	if(MouseX >= LoaderInfo->VideoMode.ModeWidth) MouseX = LoaderInfo->VideoMode.ModeWidth - 1;
	if(MouseY >= LoaderInfo->VideoMode.ModeHeight) MouseY = LoaderInfo->VideoMode.ModeHeight - 1;
	MouseMove(MouseX, MouseY);

	if(lButton ^ ButtonL)
		if(lButton)
			MouseDown(0);
		else
			MouseUp(0);

	if(mButton ^ ButtonM)
		if(mButton)
			MouseDown(1);
		else
			MouseUp(1);

	if(rButton ^ ButtonR)
		if(rButton)
			MouseDown(2);
		else
			MouseUp(2);

	ButtonL = lButton;
	ButtonM = mButton;
	ButtonR = rButton;
}

int MouseProgram(int scaling, int interrupts, int reporting)
{
	BYTE response;
	unsigned int count, b;
	
	DbgPrintf("_09");
	//Disable Keyboard
	MouseOut(MOUSE_CONTROL, CTRL_DEACTIVATE);

	for(count=0;count<2;count++)
    {
		DbgPrintf("_10");
		// Send reset command
		MouseOut(MOUSE_CONTROL, CTRL_CMD);
		MouseOut(MOUSE_DATA, CMD_RESET);
		response=MouseIn();
		if (response != MOUSE_ACK)
		goto exit;
		DbgPrintf("_11");

		// Should be 'self test passed' 0xAA
		response=MouseIn();
		if (response != 0xAA)
		goto exit;
		DbgPrintf("_12");

		// Get the device ID.  0x00 for normal PS/2 mouse
		response=MouseIn();
		if (response != 0)
		goto exit;
		DbgPrintf("_13");
    }
	
	//Set scaling
	MouseOut(MOUSE_CONTROL, CTRL_CMD);
	MouseOut(MOUSE_DATA, scaling==2?CMD_SCALING2:CMD_SCALING1);
	response=MouseIn();
	if (response != MOUSE_ACK)
	goto exit;
	DbgPrintf("_14");
	
	MouseOut(MOUSE_CONTROL, CTRL_CMD);
	MouseOut(MOUSE_DATA, reporting?CMD_REPORTINGON:CMD_REPORTINGOFF);
	response = MouseIn();
	if (response != MOUSE_ACK)
	goto exit;
	DbgPrintf("_16");

exit:
	//Enable Keyboard
	//DbgPrintf("_19%X", response);
	MouseOut(MOUSE_CONTROL, CTRL_ACTIVATE);
	DbgPrintf("_17");
	return 0;
}

int MouseInit()
{
	MouseX = LoaderInfo->VideoMode.ModeWidth/2;
	MouseY = LoaderInfo->VideoMode.ModeHeight/2;
	ButtonL = ButtonM = ButtonR = 0;
	MouseProgram(1, 1, 1);
	DbgPrintf("_18");
	KeSetIDTTrapGate(0x2C,SEL_RING0_CODE,(UINT32)&KeMouseIrq);
	DbgPrintf("_19");
	KePICIRQEnable(12);
	DbgPrintf("_20");
	return 0;
}