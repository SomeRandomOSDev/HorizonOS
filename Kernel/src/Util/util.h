#pragma once

void EnableInterrupts(){asm("sti");}
void DisableInterrupts(){asm("cli");}

void ShowCursor(bool show, uint8_t startScanline/* = 0x0f*/, uint8_t endScanline/* = 0x0f*/);
void SetCursorPos(uint16_t pos);
void Sleep(uint32_t milliseconds);
void endline(uint16_t* curs);
void carriage(uint16_t* curs);
void putchar(char c, uint8_t color, uint16_t* curs);
void putc(uint16_t c);
bool kbhit();
char getc();
char* gets(uint16_t MAX_STR_SIZE);
void printstr(const char* str, uint8_t color, uint16_t* cursor);
void printf(char* format, ...);
int32_t atoi(char* str);
void puts(char* str);
char hex4bToChar(uint8_t hex);
char* hex1BToStr(uint8_t hex);
char* hex2BToStr(uint16_t hex);
char* hex12bToStr(uint16_t hex);
char* hex4BToStr(uint32_t hex);
char* hexToStr(uint32_t nb);
void ClearScreenText(uint8_t color);
uint8_t Dabble(uint8_t nb);
uint16_t BinToBCD1B(uint8_t nb);
char* uintToStr1B(uint8_t nb);
char* uintToStr2B(uint16_t nb);
char* uintToStr4B(uint32_t nb);
char* intToStr(int nb);
char* uintToStr(uint32_t nb);
uint32_t rand();
void srand(uint32_t seed);
void Reboot();
void Shutdown();
uint8_t getDaysInMonth(int8_t month, bool leapYear);
void TimeConvert(int8_t* year, int8_t* month, int8_t* dayOfMonth, int8_t* hours, int8_t* minutes, int8_t* seconds);

#define max(nb0, nb1) ((nb0 > nb1) ? nb0 : nb1)
#define min(nb0, nb1) ((nb0 < nb1) ? nb0 : nb1)
#define updateCursor() SetCursorPos(cursor / 2)
#define RAND_MAX 32767
#define time(x) (year + month + dayOfMonth + weekday + hours + minutes + seconds + pitTimer - x)
#define TimeConvertCurrent() TimeConvert(&time_year, &time_month, &time_dayOfMonth, &time_hour, &time_minute, &time_second)
#define Halt() DisableInterrupts(); \
			   asm("hlt")
#define ClearScreen() ClearScreenText(FG_WHITE | BG_BLACK); \
					  cursor = 0; \
					  updateCursor();

#define FG_BLACK    0x00
#define FG_BLUE     0x01
#define FG_GREEN    0x02
#define FG_CYAN     0x03
#define FG_RED      0x04
#define FG_MAGENTA  0x05
#define FG_BROWN    0x06
#define FG_LGRAY    0x07
#define FG_DGRAY    0x08
#define FG_LBLUE    0x09
#define FG_LGREEN   0x0a
#define FG_LCYAN    0x0b
#define FG_LRED     0x0c
#define FG_LMAGENTA 0x0d
#define FG_YELLOW   0x0e
#define FG_WHITE    0x0f

#define BG_BLACK    (FG_BLACK    * 0x10)
#define BG_BLUE     (FG_BLUE     * 0x10)
#define BG_GREEN    (FG_GREEN    * 0x10)
#define BG_CYAN     (FG_CYAN     * 0x10)
#define BG_RED      (FG_RED      * 0x10)
#define BG_MAGENTA  (FG_MAGENTA  * 0x10)
#define BG_BROWN    (FG_BROWN    * 0x10)
#define BG_LGRAY    (FG_LGRAY    * 0x10)
#define BG_DGRAY    (FG_DGRAY    * 0x10)
#define BG_LBLUE    (FG_LBLUE    * 0x10)
#define BG_LGREEN   (FG_LGREEN   * 0x10)
#define BG_LCYAN    (FG_LCYAN    * 0x10)
#define BG_LRED     (FG_LRED     * 0x10)
#define BG_LMAGENTA (FG_LMAGENTA * 0x10)
#define BG_YELLOW   (FG_YELLOW   * 0x10)
#define BG_WHITE    (FG_WHITE    * 0x10)