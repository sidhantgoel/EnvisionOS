#include <timer.h>

volatile UINT32 TimerCount;

#define TIMER_CONTROL_PORT      0x43
#define TIMER_DATA_PORT         0x40
#define SYSTEM_TIMER_SETUP      0x36

void TimerTimer()
{
	TimerCount++;
}

void TimerInit()
{
	TimerCount=0;
	TimerSetSpeed(1000);
    TimerEnable();
}

void TimerDelay(UINT32 _Ticks)
{
    UINT32 Ticks = TimerCount;
    Ticks += _Ticks;
    while(TimerCount<Ticks);
}

void DelayMS(UINT32 Count)
{
    TimerDelay(Count);
}

void TimerEnable()
{
    PicVectorEnable(0);
}

void TimerDisable()
{
    PicVectorDisable(0);
}

void TimerSetSpeed(UINT32 hz)
{
    UINT32 Divisor;

    Divisor = 0x1234DD / hz;

    if(!Divisor) Divisor++;

    _outp(TIMER_CONTROL_PORT, SYSTEM_TIMER_SETUP);
    _outp(TIMER_DATA_PORT, Divisor & 0xFF);
    _outp(TIMER_DATA_PORT, Divisor >> 8);
}