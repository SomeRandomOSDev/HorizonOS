#pragma once

uint16_t LPT1,
         LPT2,
         LPT3;

void InitParallel()
{
    LPT1 = *(uint16_t*)0x408;
    LPT2 = *(uint16_t*)0x40a;
    LPT3 = *(uint16_t*)0x40c;
}

#define ParallelIsReady(port) (inb(port + 1) >> 7)
#define WaitParallel(port) while(!ParallelIsReady(port)) Sleep(10);

void ParallelSendByte(uint8_t byte)
{
 	uint8_t ctrl;
 
 	WaitParallel(LPT1);
 
 	outb(LPT1, byte);
 
 	ctrl = inb(LPT1 + 2);
 	outb(LPT1 + 2, (ctrl | 1));
 	Sleep(10);
 	outb(LPT1 + 2, ctrl);
 
 	WaitParallel(LPT1);
}