#pragma once

typedef uint8_t PIT_CHANNEL;

#define PIT_CHANNEL_0 0
#define PIT_CHANNEL_1 1
#define PIT_CHANNEL_2 2

typedef uint8_t PIT_OPERATING_MODE;

#define PIT_SQUARE_WAVE_GENERATOR 0b011

void SetPITSpeed(uint16_t hz, PIT_CHANNEL channel, PIT_OPERATING_MODE mode)
{
    uint16_t ticks = 1193180 / hz;
    outb(0x43, (channel << 6) | (0b11 << 4) | (mode << 1)); // 0b00110110
    outb(0x40 + channel, (ticks & 0xff));
    outb(0x40 + channel, (ticks >> 8));
}

void PITTimerHandler(struct IDTRegisters* reg)
{
    pitTimer += PIT_MILLISECOND;
    float pitSecond = pitTimer / 1000.;
    if(floor(pitSecond) == pitSecond)
    {
        time_second++;
        TimeConvertCurrent();
    }
    updateCursor();
}
