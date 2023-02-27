#pragma once

void CMOSSelectRegister(uint8_t reg)
{
	outb(0x70, (0x80 | reg)); // NMI Disable
}

uint8_t CMOSReadRegister()
{
	return inb(0x71);
}

void CMOSSetRegister(uint8_t val)
{
	outb(0x71, val);
}

#define RTC_SECONDS_REGISTER   0x00 // 0 - 59
#define RTC_MINUTES_REGISTER   0x02 // 0 - 59
#define RTC_HOURS_REGISTER     0x04 // 0 - 23 // 1 - 12
#define RTC_WEEKDAY_REGISTER   0x06 // 1 - 7
#define RTC_DAY_REGISTER       0x07 // 1 - 31
#define RTC_MONTH_REGISTER     0x08 // 1 - 12
#define RTC_YEAR_REGISTER      0x09 // 0 - 99
#define RTC_CENTURY_REGISTER   0x32 // 19 - 20
#define RTC_STATUS_A_REGISTER  0x0a
#define RTC_STATUS_B_REGISTER  0x0b