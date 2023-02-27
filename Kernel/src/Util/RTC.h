#pragma once

void RTCSetRate(uint8_t rate) // No interrupts
{
	CMOSSelectRegister(RTC_STATUS_A_REGISTER);
	uint8_t regA = CMOSReadRegister();
	CMOSSelectRegister(RTC_STATUS_A_REGISTER);
	CMOSSetRegister((regA & 0xf0) | (rate & 0x0f));
}

bool RTCIsReady()
{
	CMOSSelectRegister(RTC_STATUS_A_REGISTER);
	return !(CMOSReadRegister() >> 7);
} 

uint8_t BCDToBinary(uint8_t bcd)
{
	//return (((bcd & 0xf0) >> 1) + ((bcd & 0xf0) >> 3) + (bcd & 0x0f));
	return ((bcd & 0x0f) + ((bcd >> 4) * 10));
}

#define WaitRTC() while(!RTCIsReady())