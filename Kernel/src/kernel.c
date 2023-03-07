#include "Util/defs.h"

struct IDTRegisters
{
    uint32_t gs, fs, es, ds;
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint32_t int_no, err_code;
    uint32_t eip, cs, eflags, userEsp, ss;
} __attribute__((packed));

uint16_t cursor = 0;

#define KB 1024
#define MB (1024 * KB)
#define GB (1024 * MB)
#define TB (1024 * GB)
#define PB (1024 * TB)
#define EB (1024 * PB)

#define TAB_LENGTH      4
#define PIT_SPEED       100 // Hz
#define PIT_MILLISECOND (1000 / PIT_SPEED)
#define PIT_SECOND      (PIT_MILLISECOND / 1000.)

uint32_t pitTimer;
bool kb_extendedScancode = false,
     kb_capsLock = false, kb_numLock = false, kb_scrollLock = false;

typedef uint8_t           KB_FORMAT;
#define KB_FORMAT_QWERTY  0
#define KB_FORMAT_AZERTY  1

KB_FORMAT kb_Format;

int8_t time_second;
int8_t time_minute;
int8_t time_hour;
int8_t time_weekday;
int8_t time_dayOfMonth;
int8_t time_month;
int8_t time_year;
uint8_t time_timeMode;
bool time_hourFormat;
bool time_timeBinary;
int time_timeZone;

uint8_t apmVersionMajor;
uint8_t apmVersionMinor;
bool firstBoot;

#define HOURS_24 1
#define HOURS_12 0

void init2();
void kernelMain();

void PITTimerHandler(struct IDTRegisters* reg);
void KeyboardHandler(struct IDTRegisters* reg);

#include "Util/logo.h"
#include "Util/util.h"
#include "Util/io.h"
#include "Drivers/PS2/ps2.h"
#include "Drivers/PS2/keyboard.h"
#include "Util/CMOS.h"
#include "APM/apm.h"
#include "Util/math.h"
#include "Util/memory.h"

//#include "gdt.h"
#include "IRQ/irq.h"
#include "IDT/idt.h"
#include "Drivers/pit.h"
#include "Util/RTC.h"
#include "Drivers/ATA/ata.h"
#include "Drivers/parallel.h"
#include "Drivers/serial.h"
#include "Drivers/Sound/pcspk.h"
#include "Drivers/Sound/sb16.h"

#include "Util/io.c"
#include "Util/util.c"
#include "Drivers/PS2/keyboard.c"
#include "Util/memory.c"

#include "Drivers/pci.h"

uint32_t EBDAAddress = 0;
void* EBDA = NULL;

extern void init(uint16_t BCDApmVersion)
{
	puts("\n\n");
	cursor = 160 * 2;
	initMem();
	ShowCursor(true, 0x0f, 0x0f); // Init cursor shape

	updateCursor();

	if(BCDApmVersion == 0)
	{
		printstr("APM not supported...\n", (BG_BLACK | FG_LRED), &cursor);
		printstr("Could not boot HorizonOS properly.", (BG_BLACK | FG_LRED), &cursor);
		updateCursor();
		while(true);
	}
	apmVersionMajor = BCDToBinary(BCDApmVersion >> 8);
	apmVersionMinor = BCDToBinary(BCDApmVersion & 0xff);
	printf("APM Version : %u.%u\n", apmVersionMajor, apmVersionMinor);

	updateCursor();

	kb_Format = KB_FORMAT_AZERTY;
	//kb_Format = KB_FORMAT_QWERTY;

	// GDTInstall();  // Init GDT

	IDTInstall();  // and IDT
	ISRInstall();
	IRQRemap();    // Remap PIC's interrupts to not overwrite the ISRs

	pitTimer = 0;

	SetPITSpeed(PIT_SPEED, PIT_CHANNEL_0, PIT_SQUARE_WAVE_GENERATOR); // Init PIT

	RTCSetRate(0); // No RTC IRQ

	EnableInterrupts();

	firstBoot = true;

	init2();
}

void init2()
{
 	// InitPaging();

	// InitPage0();

	// InitPage(page768, 768);

	// loadPageDirectory(pageDir);
	// EnablePaging();

	// uint8_t* ptr = (uint8_t*)0xa0000000;

	// uint8_t val = *ptr;
	// if(val == 0);

	// //isr14();
	// printf("Abcdef");

	// while(true);

	puts("Detecting memory...\n");

	EBDAAddress = (uint32_t)(*(uint16_t*)0x40e) << 4;
	EBDA = (void*)EBDAAddress;

	printf("%u bytes found.\n", RAM_SIZE);

	printf("EBDA adress : 0x%x\n", EBDAAddress);

	puts("Initialysing keyboard...\n");

	InitKeyboard();

	if(!ResetKeyboard())
	{
		printstr("Error while initialysing keyboard...\n", (BG_BLACK | FG_LRED), &cursor);
		printstr("Could not boot HorizonOS properly.", (BG_BLACK | FG_LRED), &cursor);
		// ShowCursor(false, 0, 0);
		while(true);
	}
	else
	{
		puts("Selecting scancode set 2...\n");
		SetScancodeSet(2);
	}

	printf("PATA Slave Drive Identify :  %xh (%s)\n", ATAIdentify(ATA_DISK_SLAVE), (ATAIdentify(ATA_DISK_SLAVE) == 0 ? "Disconnected" : "Connected"));
	printf("PATA Master Drive Identify : %xh (%s)\n", ATAIdentify(ATA_DISK_SLAVE), (ATAIdentify(ATA_DISK_MASTER) == 0 ? "Disconnected" : "Connected"));

	updateCursor();

	CMOSSelectRegister(RTC_STATUS_B_REGISTER);
	Sleep(10);
	time_timeMode = CMOSReadRegister();

	time_hourFormat = (time_timeMode & 1);
	time_timeBinary = ((time_timeMode >> 1) & 1);

	printf("Getting RTC time...\n");

	WaitRTC();
	Sleep(10);

	CMOSSelectRegister(RTC_SECONDS_REGISTER);
	Sleep(10);
	time_second = BCDToBinary(CMOSReadRegister());

	CMOSSelectRegister(RTC_MINUTES_REGISTER);
	Sleep(10);
	time_minute = BCDToBinary(CMOSReadRegister());

	CMOSSelectRegister(RTC_HOURS_REGISTER);
	Sleep(10);
	time_hour = BCDToBinary(CMOSReadRegister());

	CMOSSelectRegister(RTC_WEEKDAY_REGISTER);
	Sleep(10);
	time_weekday = BCDToBinary(CMOSReadRegister());

	CMOSSelectRegister(RTC_DAY_REGISTER);
	Sleep(10);
	time_dayOfMonth = BCDToBinary(CMOSReadRegister());

	CMOSSelectRegister(RTC_MONTH_REGISTER);
	Sleep(10);
	time_month = BCDToBinary(CMOSReadRegister());

	CMOSSelectRegister(RTC_YEAR_REGISTER);
	Sleep(10);
	time_year = BCDToBinary(CMOSReadRegister()) + 2000;

	InitParallel();
	InitSerial();

	kb_capsLock = false;
	kb_numLock = false;
	kb_scrollLock = false;
	kb_extendedScancode = false;

	updateCursor();

	// Halt();

	ClearScreen();

	printf("Scanning PCI Buses...\n\n");
	PCI_ScanBuses();

	updateCursor();

	// Halt();

	ClearScreen();

	kernelMain();
}

///////////////////////////////////////////////////////

void kernelMain()
{
	printf("%s\n\n", asciiLogo);

	if(firstBoot)
	{
		printf("This is the first time HorizonOS boots on this computer.\n");

		printf("Choose your time zone : UTC + ");
		updateCursor();

		char* timeZoneStr = gets(3 + 1);
		time_timeZone = atoi(timeZoneStr);
		free(timeZoneStr);

		time_hour += time_timeZone;
		TimeConvertCurrent();

		printf("It is %u:%u\n", time_hour, time_minute);

		printf("Choose a username : ");

		updateCursor();

		char* userNameStr = gets(25);

		printf("Hi %s ! \n", userNameStr);

		free(userNameStr);

		putc('\n');

		updateCursor();
	}
}
