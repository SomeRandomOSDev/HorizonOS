#pragma once

void ShowCursor(bool show, uint8_t startScanline/* = 0x0f*/, uint8_t endScanline/* = 0x0f*/)
{
	if(show)
	{
		outb(0x3d4, 0x0a);
		outb(0x3d5, (inb(0x3d5) & 0xc0) | startScanline);
	 
		outb(0x3d4, 0x0b);
		outb(0x3d5, (inb(0x3d5) & 0xe0) | endScanline);
	}
	else
	{
		outb(0x3d4, 0x0a);
		outb(0x3d5, 0x20);
	}
}

void SetCursorPos(uint16_t pos)
{
	outb(0x3d4, 0x0f);
	outb(0x3d5, ((uint8_t)(pos & 0xff)));
	outb(0x3d4, 0x0e);
	outb(0x3d5, ((uint8_t)((pos & 0xff00) >> 8)));
}

void Sleep(uint32_t milliseconds)
{
	uint32_t timer = pitTimer;
	while(timer + milliseconds > pitTimer);
}

void text_endline(uint16_t* curs)
{
	*curs += 80;
	text_carriage(curs);
}

void text_carriage(uint16_t* curs)
{
	*curs = (*curs / 80) * 80;
}

void putchar(char c, uint8_t color, uint16_t* curs)
{
	if(c == '\n')
	{
		text_endline(curs);
	}
	else if(c== '\r')
	{
		text_carriage(curs);
	}
	else if(c == '\t')
	{
		for(uint8_t i = 0; i < TAB_LENGTH; i++)
		{
			putchar(' ', color, curs);
		}
	}
	else
	{
		*(char*)(0xb8000 + (2 * (*curs))) = c;
		*(char*)(0xb8001 + (2 * (*curs))) = color;
		(*curs)++;
		// *curs %= 80 * 25;
	}
}

void putc(uint16_t c)
{
	putchar((c & 0xff), 0x0f, &text_cursor);
}

bool kbhit()
{
	bool r = false;
	for(uint8_t i = 0; i < 128; i++)
		r |= GetScancodeKeyState(i);
	return r;
}

char getc()
{
	char c = 0;
	while(c == 0)
		c = GetChar(kb_Format);
	return c;
}

char* gets(uint16_t MAX_STR_SIZE)
{
	uint32_t entryDelay = 0;
	char lastC = 0;

	char* str = (char*)malloc(MAX_STR_SIZE * sizeof(char));

	char c = 0;

	uint16_t i = 0;

	uint32_t timer = 0;

	while(c != '\n' || i == 0)
	{
		c = GetChar(kb_Format);
		if(timer <= pitTimer || (lastC != c && lastC != ShiftChar(kb_Format, c)))
		{
			if(c != 0)
			{
				if(c != '\n')
				{
					if(c == '\b')
					{
						if(i > 0)
						{
							i--;
							text_cursor--;
							putc(' ');
							str[i] = ' ';
							text_cursor--;
						}
					}
					else if(c != '\n')
					{
						if(i < MAX_STR_SIZE - 1)
						{
							putc(c);
							str[i] = c;

							// text_cursor -= 2;

							i++;
						}
					}
					else
						putc('\n');
					if(lastC == c || lastC == 0)
						entryDelay++;
					updateCursor();
					if(entryDelay < 2)
						timer = pitTimer + 350;
					else
						timer = pitTimer + 30;
				}
			}
			else
				entryDelay = 0;
		}

		lastC = c;
	}
	str[i] = '\0';
	putc('\n');
	return str;
}

void printstr(const char* str, uint8_t color, uint16_t* text_cursor)
{
	for(uint32_t i = 0; str[i] != '\0'; i++)
	{
		putchar(str[i], color, text_cursor);
	}
}

void printf(char* format, ...)
{
	char* str = format;
	int* arg = (int*)&format;
	arg++;
	for(uint32_t i = 0; str[i] != '\0'; i++)
	{
		if(str[i] == '%')
		{
			switch(str[i + 1])
			{
			case 'c':
				{
					putc((char)*arg);
					arg ++;
					break;
				}
			case 's':
				{
					puts((char*)*arg);
					arg ++;
					break;
				}
			case 'd':
				{
					char* intStr = intToStr(*arg);
					puts(intStr);
					free(intStr);
					arg ++;
					break;
				}
			case 'u':
				{
					char* intStr = uintToStr(*arg);
					puts(intStr);
					free(intStr);
					arg ++;
					break;
				}
			case 'x':
				{
					char* hexStr = hexToStr(*arg);
					puts(hexStr);
					free(hexStr);
					arg ++;
					break;
				}
			case 'p':
				{
					char* hexStr = hexToStr((uint32_t)((void*)*arg));
					puts(hexStr);
					free(hexStr);
					arg ++;
					break;
				}
			case 'B':
				{
				caseB:
					puts(*arg ? "true" : "false");
					arg ++;
					break;
				}
			case 'b':
				{
					goto caseB;
					break;
				}
			case '%':
				{
					putc('%');
					break;
				}
			default:
				{
					break;
				}
			}
			i++;
		}
		else
			putc(str[i]);
	}
}

int32_t atoi(char* str)
{
	bool negative = false;
	if(str[0] == '-')
		negative = true;
	int i = 0;
	if(str[0] == '+')
		i++;
	if(negative)
		i++;

	int32_t r = 0;

	while(str[i] >= '0' && str[i] <= '9') 
	{
        r *= 10;
        r += str[i] - '0';
        i++;
    }

	if(negative)
	{
		r = -r;
		str--;
	}

	return r;
}

void puts(char* str)
{
	printstr(str, 0x0f, &text_cursor);
}

char hex4bToChar(uint8_t hex)
{
	uint8_t hex4b = (hex & 0x0f);
	if(hex4b < 10)
		return hex4b + 48;
	return hex4b - 10 + 97;
}

char* hex1BToStr(uint8_t hex)
{
	char* ptr = (char*)malloc(3 * sizeof(char));
	uint8_t hi = ((hex & 0xf0) >> 4), lo = (hex & 0x0f);
	ptr[0] = hex4bToChar(hi);
	ptr[1] = hex4bToChar(lo);
	ptr[2] = '\0';
	return ptr;
}

char* hex2BToStr(uint16_t hex)
{
	char* ptr = (char*)malloc(5 * sizeof(char));
	uint8_t hi0 = ((hex & 0xf0) >> 4), lo0 = (hex & 0x0f);
	uint8_t hi1 = ((hex & 0xf000) >> 12), lo1 = ((hex & 0x0f00) >> 8);
	ptr[0] = hex4bToChar(hi1);
	ptr[1] = hex4bToChar(lo1);
	ptr[2] = hex4bToChar(hi0);
	ptr[3] = hex4bToChar(lo0);
	ptr[4] = '\0';
	return ptr;
}

char* hex12bToStr(uint16_t hex)
{
	char* ptr = (char*)malloc(4 * sizeof(char));
	uint8_t hi0 = ((hex & 0xf0) >> 4), lo0 = (hex & 0x0f);
	uint8_t lo1 = ((hex & 0x0f00) >> 8);
	ptr[0] = hex4bToChar(lo1);
	ptr[1] = hex4bToChar(hi0);
	ptr[2] = hex4bToChar(lo0);
	ptr[3] = '\0';
	return ptr;
}

char* hex4BToStr(uint32_t hex)
{
	char* ptr = (char*)malloc(9 * sizeof(char));
	uint8_t hi0 = ((hex & 0xf0) >> 4), lo0 = (hex & 0x0f);
	uint8_t hi1 = ((hex & 0xf000) >> 12), lo1 = ((hex & 0x0f00) >> 8);
	uint8_t hi2 = ((hex & 0xf00000) >> 20), lo2 = ((hex & 0x0f0000) >> 16);
	uint8_t hi3 = ((hex & 0xf0000000) >> 28), lo3 = ((hex & 0x0f000000) >> 24);
	ptr[0] = hex4bToChar(hi3);
	ptr[1] = hex4bToChar(lo3);
	ptr[2] = hex4bToChar(hi2);
	ptr[3] = hex4bToChar(lo2);
	ptr[4] = hex4bToChar(hi1);
	ptr[5] = hex4bToChar(lo1);
	ptr[6] = hex4bToChar(hi0);
	ptr[7] = hex4bToChar(lo0);
	ptr[8] = '\0';
	return ptr;
}

char* hexToStr(uint32_t nb)
{
	uint8_t length = (nb < 16 ? 2 : (nb < 16 * 16 ? 3 : (nb < 16 * 16 * 16 ? 4 : (nb < 16 * 16 * 16 * 16 ? 5 : (nb < 16 * 16 * 16 * 16 * 16 ? 6 : (nb < 16 * 16 * 16 * 16 * 16 * 16 ? 7 : (nb < 16 * 16 * 16 * 16 * 16 * 16 * 16 ? 8 : 9)))))));
	char* str = malloc(sizeof(char) * length);
	str[max(0, length - 9)] = hex4bToChar(nb >> (4 * 7) & 0xff);
	str[max(0, length - 8)] = hex4bToChar(nb >> (4 * 6) & 0xff);
	str[max(0, length - 7)] = hex4bToChar(nb >> (4 * 5) & 0xff);
	str[max(0, length - 6)] = hex4bToChar(nb >> (4 * 4) & 0xff);
	str[max(0, length - 5)] = hex4bToChar(nb >> (4 * 3) & 0xff);
	str[max(0, length - 4)] = hex4bToChar(nb >> (4 * 2) & 0xff);
	str[max(0, length - 3)] = hex4bToChar(nb >> (4 * 1) & 0xff);
	str[max(0, length - 2)] = hex4bToChar(nb & 0xff);
	str[length - 1] = '\0';

	return str;
}

void ClearScreenText(uint8_t color)
{
	for(uint32_t i = 0xb8000; i < 0xb8000 + 2 * 80 * 25; i += 2)
	{
		*(char*)i = ' ';
		*(char*)(i + 1) = color;
	}
}

uint8_t Dabble(uint8_t nb)
{
	if((nb & 0b1111) > 4)
		nb += 3;
	return (nb & 0b1111);
}

uint16_t BinToBCD1B(uint8_t nb)
{
	uint8_t dT = Dabble((nb & 0b11100000) >> 5);
	uint8_t dU = Dabble(((dT & 0b111) << 1) | ((nb & 0b00010000) >> 4));
	uint8_t dV = Dabble(((dU & 0b111) << 1) | ((nb & 0b00001000) >> 3));
	uint8_t dW = Dabble(((dT >> 3) << 2) | ((dU >> 3) << 1) | (dV >> 3));
	uint8_t dX = Dabble(((dV & 0b0111) << 1) | ((nb & 0b00000100) >> 2));
	uint8_t dY = Dabble(((dW & 0b0111) << 1) | (dX >> 3));
	uint8_t dZ = Dabble(((dX & 0b0111) << 1) | ((nb & 0b00000010) >> 1));
	return (((dW >> 3) << 9) | (dY << 5) | (dZ << 1) | (nb & 1));
}

char* uintToStr1B(uint8_t nb)
{
	if(nb < 10)
	{
		char* ptr = (char*)malloc(2 * sizeof(char));
		ptr[0] = hex4bToChar(nb);
		ptr[1] = '\0';
		return ptr;
	}
	else
	{
		uint16_t BCD = BinToBCD1B(nb);
		if(nb < 100)
		{
			return hex1BToStr(BCD);
		}
		else
		{
			return hex12bToStr(BCD);
		}
	}
}

char* uintToStr2B(uint16_t nb)
{
	uint8_t length = (nb < 10 ? 2 : (nb < 100 ? 3 : (nb < 1000 ? 4 : (nb < 10000 ? 5 : 6))));
	char* str = malloc(sizeof(char) * length);
	str[max(0, length - 6)] = '0' + ((nb / 10000) % 10);
	str[max(0, length - 5)] = '0' + ((nb / 1000) % 10);
	str[max(0, length - 4)] = '0' + ((nb / 100) % 10);
	str[max(0, length - 3)] = '0' + ((nb / 10) % 10);
	str[max(0, length - 2)] = '0' + (nb % 10);
	str[length - 1] = '\0';

	return str;
}

char* uintToStr4B(uint32_t nb)
{
	uint8_t length = (nb < 10 ? 2 : (nb < 100 ? 3 : (nb < 1000 ? 4 : (nb < 10000 ? 5 : (nb < 100000 ? 6 : (nb < 1000000 ? 7 : (nb < 10000000 ? 8 : (nb < 100000000 ? 9 : (nb < 1000000000 ? 10 : 11)))))))));
	char* str = malloc(sizeof(char) * length);
	str[max(0, length - 11)] = '0' + ((nb / 1000000000) % 10);
	str[max(0, length - 10)] = '0' + ((nb / 100000000) % 10);
	str[max(0, length - 9)]  = '0' + ((nb / 10000000) % 10);
	str[max(0, length - 8)]  = '0' + ((nb / 1000000) % 10);
	str[max(0, length - 7)]  = '0' + ((nb / 100000) % 10);
	str[max(0, length - 6)]  = '0' + ((nb / 10000) % 10);
	str[max(0, length - 5)]  = '0' + ((nb / 1000) % 10);
	str[max(0, length - 4)]  = '0' + ((nb / 100) % 10);
	str[max(0, length - 3)]  = '0' + ((nb / 10) % 10);
	str[max(0, length - 2)]  = '0' + (nb % 10);
	str[length - 1] = '\0';

	return str;
}

char* intToStr(int nb)
{
	bool negative = false;
	if(nb < 0)
	{
		nb = -nb;
		negative = true;
	}
	uint8_t length = (nb < 10 ? 2 : (nb < 100 ? 3 : (nb < 1000 ? 4 : (nb < 10000 ? 5 : (nb < 100000 ? 6 : (nb < 1000000 ? 7 : (nb < 10000000 ? 8 : (nb < 100000000 ? 9 : (nb < 1000000000 ? 10 : 11)))))))));
	char* str = malloc(sizeof(char) * length);
	str[max(0, length - 11 + negative)] = '0' + ((nb / 1000000000) % 10);
	str[max(0, length - 10 + negative)] = '0' + ((nb / 100000000) % 10);
	str[max(0, length - 9 + negative)]  = '0' + ((nb / 10000000) % 10);
	str[max(0, length - 8 + negative)]  = '0' + ((nb / 1000000) % 10);
	str[max(0, length - 7 + negative)]  = '0' + ((nb / 100000) % 10);
	str[max(0, length - 6 + negative)]  = '0' + ((nb / 10000) % 10);
	str[max(0, length - 5 + negative)]  = '0' + ((nb / 1000) % 10);
	str[max(0, length - 4 + negative)]  = '0' + ((nb / 100) % 10);
	str[max(0, length - 3 + negative)]  = '0' + ((nb / 10) % 10);
	str[max(0, length - 2 + negative)]  = '0' + (nb % 10);
	str[length - 1 + negative] = '\0';

	if(negative)
		str[0] = '-';

	return str;
}

char* uintToStr(uint32_t nb)
{
	uint8_t length = (nb < 10 ? 2 : (nb < 100 ? 3 : (nb < 1000 ? 4 : (nb < 10000 ? 5 : (nb < 100000 ? 6 : (nb < 1000000 ? 7 : (nb < 10000000 ? 8 : (nb < 100000000 ? 9 : (nb < 1000000000 ? 10 : 11)))))))));
	char* str = malloc(sizeof(char) * length);
	str[max(0, length - 11)] = '0' + ((nb / 1000000000) % 10);
	str[max(0, length - 10)] = '0' + ((nb / 100000000) % 10);
	str[max(0, length - 9)]  = '0' + ((nb / 10000000) % 10);
	str[max(0, length - 8)]  = '0' + ((nb / 1000000) % 10);
	str[max(0, length - 7)]  = '0' + ((nb / 100000) % 10);
	str[max(0, length - 6)]  = '0' + ((nb / 10000) % 10);
	str[max(0, length - 5)]  = '0' + ((nb / 1000) % 10);
	str[max(0, length - 4)]  = '0' + ((nb / 100) % 10);
	str[max(0, length - 3)]  = '0' + ((nb / 10) % 10);
	str[max(0, length - 2)]  = '0' + (nb % 10);
	str[length - 1] = '\0';

	return str;
}

uint32_t next = 1;
uint32_t rand()
{
    next = next * 1103515245 + 12345;
    return (uint32_t)(next / 65536) % (RAND_MAX + 1);
}
 
void srand(uint32_t seed)
{
    next = seed;
}

void Reboot()
{
////////// Reboot
	PS2Wait();
	outb(PS2_COMMAND_REGISTER, 0xfe);
}
void Shutdown()
{
////////// Shutdown
	APMSetPowerStateOff();
}

uint8_t getDaysInMonth(int8_t month, bool leapYear)
{
	if(month == 1)
		return 28 + leapYear;
	return 30 + (month >= 7 ? month % 2 : (month + 1) % 2);
}

void TimeConvert(int8_t* year, int8_t* month, int8_t* dayOfMonth, int8_t* hours, int8_t* minutes, int8_t* seconds)
{
	while(*seconds >= 60)
	{
		(*minutes)++;
		*seconds -= 60;
	}
	while(*minutes >= 60)
	{
		(*hours)++;
		*minutes -= 60;
	}
	while(*hours >= 24)
	{
		(*dayOfMonth)++;
		*hours -= 24;
	}
	uint8_t daysInMonth = getDaysInMonth((*month) - 1, (*year / 4 == *year / 4.));
	while(*dayOfMonth - 1 >= daysInMonth)
	{
		(*month)++;
		*dayOfMonth -= daysInMonth;
	}
	while(*month > 12)
	{
		(*year)++;
		*month -= 12;
	}
	while(*year >= 100)
	{
		*year -= 100;
	}

////////////////////////////////////////////////////////////

	while(*seconds < 0)
	{
		(*minutes)--;
		*seconds += 60;
	}
	while(*minutes < 0)
	{
		(*hours)--;
		*minutes += 60;
	}
	while(*hours < 0)
	{
		(*dayOfMonth)--;
		*hours += 24;
	}
	while(*dayOfMonth < 1)
	{
		(*month)--;
		*dayOfMonth += daysInMonth;
	}
	while(*month < 1)
	{
		(*year)--;
		*month += 12;
	}
	while(*year < 0)
	{
		*year += 100;
	}
}