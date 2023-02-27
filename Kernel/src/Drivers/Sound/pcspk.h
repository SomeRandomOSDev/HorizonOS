#pragma once

void PCSPKPlayFrequency(uint32_t nFrequence) 
{
 	uint32_t div = 1193180 / nFrequence;
 
 	SetPITSpeed(div, PIT_CHANNEL_2, PIT_SQUARE_WAVE_GENERATOR);
 
 	uint8_t tmp;
 	tmp = inb(0x61);
  	if(tmp != (tmp | 3)) 
 		outb(0x61, tmp | 0b11);
}
 
void PCSPKStopSound() 
{
 	outb(0x61, inb(0x61) & 0xfc);
}