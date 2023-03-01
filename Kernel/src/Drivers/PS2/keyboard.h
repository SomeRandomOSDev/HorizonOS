#pragma once

// https://wiki.osdev.org/PS/2_Keyboard
// https://wiki.osdev.org/"8042"_PS/2_Controller

#define KB_ESCAPE      27
#define KB_CTRL        (0x0100 + 0)
#define KB_LSHIFT      (0x0100 + 1)
#define KB_RSHIFT      (0x0100 + 2)
#define KB_ALT         (0x0100 + 3)
#define KB_CAPS_LOCK   (0x0100 + 4)
#define KB_F1          (0x0100 + 5)
#define KB_F2          (0x0100 + 6)
#define KB_F3          (0x0100 + 7)
#define KB_F4          (0x0100 + 8)
#define KB_F5          (0x0100 + 9)
#define KB_F6          (0x0100 + 10)
#define KB_F7          (0x0100 + 11)
#define KB_F8          (0x0100 + 12)
#define KB_F9          (0x0100 + 13)
#define KB_F10         (0x0100 + 14)
#define KB_F11         (0x0100 + 15)
#define KB_F12         (0x0100 + 16)
#define KB_NUMLOCK     (0x0100 + 17)
#define KB_SCROLLLOCK  (0x0100 + 18)
#define KB_INSERT      (0x0100 + 27)
#define KB_DELETE      (0x0100 + 28)
#define KB_NUMPAD_5    (0x0100 + 29)
#define KB_É           ('e' + 0x1000)
#define KB_È           ('e' + 0x2000)
#define KB_Ç           ('c' + 0x1000)
#define KB_À           ('a' + 0x1000)

#define KB_NUMPAD_0    (0x0100 + 38)
#define KB_NUMPAD_1    (0x0100 + 26)
#define KB_NUMPAD_2    (0x0100 + 23)
#define KB_NUMPAD_3    (0x0100 + 25)
#define KB_NUMPAD_4    (0x0100 + 21)

#define KB_NUMPAD_6    (0x0100 + 22)
#define KB_NUMPAD_7    (0x0100 + 19)
#define KB_NUMPAD_8    (0x0100 + 20)
#define KB_NUMPAD_9    (0x0100 + 24)

// Extended keys
#define KB_HOME        (0x0100 + 30)
#define KB_UP          (0x0100 + 31)
#define KB_LEFT        (0x0100 + 32)
#define KB_RIGHT       (0x0100 + 33)
#define KB_DOWN        (0x0100 + 34)
#define KB_PAGEUP      (0x0100 + 35)
#define KB_PAGEDOWN    (0x0100 + 36)
#define KB_END         (0x0100 + 37)

#define KB_ALT_GR      (0x0100 + 38)

#define LED_SCROLL_LOCK (1 << 0)
#define LED_NUM_LOCK    (1 << 1)
#define LED_CAPS_LOCK   (1 << 2)

#define KB_ACK      0xfa
#define KB_RESEND   0xfe

#define KB_SCANCODE_SET_1   1
#define KB_SCANCODE_SET_2   2
#define KB_SCANCODE_SET_3   3

uint16_t keyboardLookupQWERTYSet2[128] =
{
    0,  KB_ESCAPE, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', 
    '-', '=', '\b', '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 
    'i', 'o', 'p', '[', ']', '\n', KB_CTRL, 'a', 's', 'd', 
    'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', KB_LSHIFT, 
    '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 
    KB_RSHIFT, '*', KB_ALT, ' ', KB_CAPS_LOCK, KB_F1, KB_F2, 
    KB_F3, KB_F4, KB_F5, KB_F6, KB_F7, KB_F8, KB_F9, KB_F10,
    KB_NUMLOCK, KB_SCROLLLOCK, KB_NUMPAD_7, KB_NUMPAD_8, KB_NUMPAD_9, '-', 
    KB_NUMPAD_4, KB_NUMPAD_5, KB_NUMPAD_6, '+', KB_NUMPAD_1, KB_NUMPAD_2, KB_NUMPAD_3,
    KB_NUMPAD_0, '.', 0, 0, 0, KB_F11, KB_F12,
    //Extended :
    KB_RIGHT, KB_UP, KB_LEFT, KB_DOWN, KB_DELETE, KB_INSERT, KB_ALT_GR
};	

uint16_t keyboardLookupAZERTYSet2[128] =
{
    0,  KB_ESCAPE, '&', KB_É, '\"', '\'', '(', '-', KB_È, '_', KB_Ç, KB_À, ')', '=', '\b', '\t', 
    'a', 'z', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '^', '$', '\n', KB_CTRL, 'q', 's', 
    'd', 'f', 'g', 'h', 'j', 'k', 'l', 'm', '%', 0, KB_LSHIFT, '*', 'w', 'x', 'c', 'v', 
    'b', 'n', ',', ';', ':', '!', KB_RSHIFT, '*', KB_ALT, ' ', KB_CAPS_LOCK, KB_F1, KB_F2, KB_F3, KB_F4, KB_F5, 
    KB_F6, KB_F7, KB_F8, KB_F9, KB_F10, KB_NUMLOCK, KB_SCROLLLOCK, KB_NUMPAD_7, KB_NUMPAD_8, KB_NUMPAD_9, '-', KB_NUMPAD_4, KB_NUMPAD_5, KB_NUMPAD_6, '+', KB_NUMPAD_1, 
    KB_NUMPAD_2, KB_NUMPAD_3, KB_NUMPAD_0, '.', 0, 0, 0, KB_F11, KB_F12,
    //Extended :
    KB_RIGHT, KB_UP, KB_LEFT, KB_DOWN, KB_DELETE, KB_INSERT, KB_ALT_GR
};	

bool keyboardLookupState[128] = 
{
    false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, 
    false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, 
    false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, 
    false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, 
    false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, 
    false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, 
    false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, 
    false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, 
    /*false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, 
    false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, 
    false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, 
    false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, 
    false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, 
    false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, 
    false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, 
    false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false*/
};

uint16_t KeyboardLookup(KB_FORMAT format, uint8_t key);
void SetScancodeKeyState(uint8_t key, bool state);
bool GetScancodeKeyState(uint8_t key);
void SetKeyState(KB_FORMAT format, uint16_t key, bool state);
bool GetKeyState(KB_FORMAT format, uint16_t key);
uint16_t AltGrChar(KB_FORMAT f, uint16_t c);
uint16_t ShiftChar(KB_FORMAT f, uint16_t c);
char KeypadNumberToASCII(uint16_t key);
char GetChar(KB_FORMAT format);
void SetLEDState(uint8_t state);
bool ResetKeyboard();
void SetScancodeSet(uint8_t set);
uint8_t GetScancodeSet();
void KeyboardHandler(struct IDTRegisters* reg);

void InitKeyboard()
{
    for(uint8_t i = 0; i < 128; i++)
        keyboardLookupState[i] = false;
}