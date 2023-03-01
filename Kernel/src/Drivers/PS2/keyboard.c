#pragma once

uint16_t KeyboardLookup(KB_FORMAT format, uint8_t key)
{
	if(format == KB_FORMAT_QWERTY)
		return keyboardLookupQWERTYSet2[key];
	if(format == KB_FORMAT_AZERTY)
		return keyboardLookupAZERTYSet2[key];
	return 0;
}

void SetScancodeKeyState(uint8_t key, bool state)
{
	if(kb_extendedScancode)
	{
		switch(key)
		{
		// Arrows
		case 0x4d:
			{
				key = 0x59;
				break;
			}
		case 0x48:
			{
				key = 0x5a;
				break;
			}
		case 0x4b:
			{
				key = 0x5b;
				break;
			}
		case 0x50:
			{
				key = 0x5c;
				break;
			}

		// Delete
		case 0x51:
			{
				key = 0x5d;
				break;
			}
		// Insert
		case 0x52:
			{
				key = 0x5e;
				break;
			}
		// Alt Gr
		case 0x38:
			{
				key = 0x5f;
				break;
			}
		default:
			{break;}
		}
	}
	keyboardLookupState[key] = state;
}

bool GetScancodeKeyState(uint8_t key)
{
	return keyboardLookupState[key];
}

void SetKeyState(KB_FORMAT format, uint16_t key, bool state)
{
	for(uint8_t i = 0; i < 128; i++)
	{
		if(KeyboardLookup(format, i) == key)
			SetScancodeKeyState(i, state);
	}
}

bool GetKeyState(KB_FORMAT format, uint16_t key)
{
	bool r = false;
	for(uint8_t i = 0; i < 128; i++)
	{
		if(KeyboardLookup(format, i) == key)
			r |= GetScancodeKeyState(i);
	}
	return r;
}

uint16_t AltGrChar(KB_FORMAT f, uint16_t c)
{
	if((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
		return 0;
	if(f == KB_FORMAT_AZERTY)
	{
		switch(c)
		{
		case '&':return 0;
		case KB_É:return 0;//'~';
		case '\"':return '#';
		case '\'':return '{';
		case '(':return '[';
		case '-':return '|';
		case KB_È:return 0;//'`';
		case '_':return '\\';
		case KB_Ç:return '^';
		case KB_À:return '@';
		case ')':return ']';
		case '=':return '}';
		default:break;
		}
	}
	return c;
}

uint16_t ShiftChar(KB_FORMAT f, uint16_t c)
{
	if(c >= 'a' && c <= 'z')
		return c - 'a' + 'A';
	else if(c >= 'A' && c <= 'Z')
		return c - 'A' + 'a';
	if(f == KB_FORMAT_AZERTY)
	{
		switch(c)
		{
		case '&':
			return '1';
		case KB_É:
			return '2';
		case '\"':
			return '3';
		case '\'':
			return '4';
		case '(':
			return '5';
		case '-':
			return '6';
		case KB_È:
			return '7';
		case '_':
			return '8';
		case KB_Ç:
			return '9';
		case KB_À:
			return '0';
		case '=':
			return '+';
		case ':':
			return '/';
		/*case '!':
			return '§';*/
		case ',':
			return '?';
		case ';':
			return '.';
		/*case '^':
			return '¨';*/

		case '1':
			return '&';
		case '2':
			return KB_É;
		case '3':
			return '\"';
		case '4':
			return '\'';
		case '5':
			return '(';
		case '6':
			return '-';
		case '7':
			return KB_È;
		case '8':
			return '_';
		case '9':
			return KB_Ç;
		case '0':
			return KB_À;
		case '+':
			return '=';
		case '/':
			return ':';
		/*case '§':
			return '!';*/
		case '?':
			return ',';
		case '.':
			return ';';
		/*case '^':
			return '¨';*/
		default:
			return c;
		}
	}
	return 0;
}

char KeypadNumberToASCII(uint16_t key)
{
	switch(key)
	{
	case KB_NUMPAD_0:
		return '0';
		break;
	case KB_NUMPAD_1:
		return '1';
		break;
	case KB_NUMPAD_2:
		return '2';
		break;
	case KB_NUMPAD_3:
		return '3';
		break;
	case KB_NUMPAD_4:
		return '4';
		break;
	case KB_NUMPAD_5:
		return '5';
		break;
	case KB_NUMPAD_6:
		return '6';
		break;
	case KB_NUMPAD_7:
		return '7';
		break;
	case KB_NUMPAD_8:
		return '8';
		break;
	case KB_NUMPAD_9:
		return '9';
		break;
	default:
		return key;
		break;
	}
}

char GetChar(KB_FORMAT format)
{
	char c = 0;
	for(uint8_t i = 0; i < 128; i++)
	{
		uint16_t currC = KeyboardLookup(format, i);

		if(GetScancodeKeyState(i))
		{
			if(kb_numLock && (currC == KB_NUMPAD_0 || currC == KB_NUMPAD_1 || currC == KB_NUMPAD_2 || currC == KB_NUMPAD_3 || currC == KB_NUMPAD_4 || currC == KB_NUMPAD_5 || currC == KB_NUMPAD_6 || currC == KB_NUMPAD_7 || currC == KB_NUMPAD_8 || currC == KB_NUMPAD_9))
				c = KeypadNumberToASCII(currC);
			if((currC == (char)currC) || currC == KB_É || currC == KB_È || currC == KB_Ç || currC == KB_À)
			{
				c = currC;
				if(GetKeyState(format, KB_LSHIFT) || GetKeyState(format, KB_RSHIFT))
					c = ShiftChar(format, c);
				if(kb_capsLock)
					c = ShiftChar(format, c);
				if(GetKeyState(format, KB_ALT_GR))
					c = AltGrChar(format, c);
			}
		}
	}
	return c;
}

void SetLEDState(uint8_t state)
{
	PS2Wait();
	outb(PS2_DATA_PORT, 0xed);
	outb(PS2_DATA_PORT, state);
	if(inb(PS2_DATA_PORT) == KB_RESEND)
		SetLEDState(state);
}

bool ResetKeyboard()
{
	PS2Wait();
	outb(PS2_DATA_PORT, 0xff);
	if(inb(PS2_DATA_PORT) == KB_RESEND)
		return ResetKeyboard();
	return (inb(PS2_DATA_PORT) == 0xaa);
}

void SetScancodeSet(uint8_t set)
{
	PS2Wait();
	outb(PS2_DATA_PORT, 0xf0);
	outb(PS2_DATA_PORT, set);
	if(inb(PS2_DATA_PORT) == KB_RESEND)
		SetScancodeSet(set);
}

uint8_t GetScancodeSet()
{
	PS2Wait();
	outb(PS2_DATA_PORT, 0xf0);
	outb(PS2_DATA_PORT, 0);
	if(inb(PS2_DATA_PORT) == KB_RESEND)
		return GetScancodeSet();
	uint8_t r = inb(PS2_DATA_PORT);
	return r == 0x43 ? KB_SCANCODE_SET_1 : (r == 0x41 ? KB_SCANCODE_SET_2 : KB_SCANCODE_SET_3 /*3f*/);
}

void KeyboardHandler(struct IDTRegisters* reg)
{
    uint8_t scancode = inb(PS2_DATA_PORT);
    uint16_t key = KeyboardLookup(kb_Format, scancode);

    if(scancode == 0xe0)
    {
    	kb_extendedScancode = true;
    	return;
    }

    if(scancode >> 7)
    {
        SetScancodeKeyState(scancode - 0x80, false);
    }
    else
    {
        SetScancodeKeyState(scancode, true);
        if(key == KB_CAPS_LOCK)
    		kb_capsLock ^= 1;
    	if(key == KB_NUMLOCK)
    		kb_numLock ^= 1;
    	if(key == KB_SCROLLLOCK)
    		kb_scrollLock ^= 1;
    	updateCursor();
    }

    kb_extendedScancode = false;
}