#pragma once

uint16_t COM1 = 0,
         COM2 = 0,
         COM3 = 0,
         COM4 = 0,
         COM5 = 0x5f8,
         COM6 = 0x4f8,
         COM7 = 0x5e8,
         COM8 = 0x4e8;

bool DLAB = false;

void InitSerial()
{
	COM1 = *(uint16_t*)0x400;
	COM2 = *(uint16_t*)0x402;
	COM3 = *(uint16_t*)0x404;
	COM4 = *(uint16_t*)0x406;
	DLAB = (inb(COM1 + 3) >> 7);
}