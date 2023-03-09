#pragma once

#define DSP_MIXER         0x224
#define DSP_MIXER_DATA    0x225
#define DSP_RESET         0x226
#define DSP_READ          0x22a
#define DSP_WRITE         0x22c
#define DSP_READ_STATUS   0x22e
#define DSP_INT_ACK       0x22f

void SB16_DSPReset()
{
    outb(DSP_RESET, 1);
    Sleep(1); // 3 microseconds
    outb(DSP_RESET, 0);
}

bool SB16_IsConnected()
{
    SB16_DSPReset();
    return (inb(DSP_READ) == 0xaa);
}