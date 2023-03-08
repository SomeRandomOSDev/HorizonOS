#pragma once

struct IDTEntry 
{
   uint16_t baseLow;
   uint16_t selector;
   uint8_t zero;
   uint8_t flags;
   uint16_t baseHigh;
} __attribute__((packed));

struct IDTPtr
{
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

struct IDTEntry IDT[256];
struct IDTPtr idtPtr;

extern void LoadIDT();

void IDTSetGate(uint8_t gate, uint32_t base, uint16_t sel, uint8_t flags)
{
    IDT[gate].baseLow = (base & 0xffff);
    IDT[gate].baseHigh = ((base & 0xffff0000) >> 16);
    IDT[gate].selector = sel;
    IDT[gate].flags = flags;
    IDT[gate].zero = 0;
}

extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();

void IDTInstall()
{
    idtPtr.limit = (sizeof(struct IDTEntry) * 256) - 1;
    idtPtr.base = (unsigned int)&IDT;

    memset(&IDT, 0, sizeof(struct IDTEntry) * 256);

    LoadIDT();
}

void ISRInstall()
{
	// GDT_Code seg // Ring 0 (bit 5 and 6)
    IDTSetGate(0, (uint32_t)&isr0, 0x08, 0b10001110);
	IDTSetGate(1, (uint32_t)&isr1, 0x08, 0b10001110);
	IDTSetGate(2, (uint32_t)&isr2, 0x08, 0b10001110);
	IDTSetGate(3, (uint32_t)&isr3, 0x08, 0b10001110);
	IDTSetGate(4, (uint32_t)&isr4, 0x08, 0b10001110);
	IDTSetGate(5, (uint32_t)&isr5, 0x08, 0b10001110);
	IDTSetGate(6, (uint32_t)&isr6, 0x08, 0b10001110);
	IDTSetGate(7, (uint32_t)&isr7, 0x08, 0b10001110);
	IDTSetGate(8, (uint32_t)&isr8, 0x08, 0b10001110);
	IDTSetGate(9, (uint32_t)&isr9, 0x08, 0b10001110);
	IDTSetGate(10, (uint32_t)&isr10, 0x08, 0b10001110);
	IDTSetGate(11, (uint32_t)&isr11, 0x08, 0b10001110);
	IDTSetGate(12, (uint32_t)&isr12, 0x08, 0b10001110);
	IDTSetGate(13, (uint32_t)&isr13, 0x08, 0b10001110);
	IDTSetGate(14, (uint32_t)&isr14, 0x08, 0b10001110);
	IDTSetGate(15, (uint32_t)&isr15, 0x08, 0b10001110);
	IDTSetGate(16, (uint32_t)&isr16, 0x08, 0b10001110);
	IDTSetGate(17, (uint32_t)&isr17, 0x08, 0b10001110);
	IDTSetGate(18, (uint32_t)&isr18, 0x08, 0b10001110);
	IDTSetGate(19, (uint32_t)&isr19, 0x08, 0b10001110);
	IDTSetGate(20, (uint32_t)&isr20, 0x08, 0b10001110);
	IDTSetGate(21, (uint32_t)&isr21, 0x08, 0b10001110);
	IDTSetGate(22, (uint32_t)&isr22, 0x08, 0b10001110);
	IDTSetGate(23, (uint32_t)&isr23, 0x08, 0b10001110);
	IDTSetGate(24, (uint32_t)&isr24, 0x08, 0b10001110);
	IDTSetGate(25, (uint32_t)&isr25, 0x08, 0b10001110);
	IDTSetGate(26, (uint32_t)&isr26, 0x08, 0b10001110);
	IDTSetGate(27, (uint32_t)&isr27, 0x08, 0b10001110);
	IDTSetGate(28, (uint32_t)&isr28, 0x08, 0b10001110);
	IDTSetGate(29, (uint32_t)&isr29, 0x08, 0b10001110);
	IDTSetGate(30, (uint32_t)&isr30, 0x08, 0b10001110);
	IDTSetGate(31, (uint32_t)&isr31, 0x08, 0b10001110);

	IDTSetGate(32, (uint32_t)&irq0, 0x08, 0b10001110);
	IDTSetGate(33, (uint32_t)&irq1, 0x08, 0b10001110);
	IDTSetGate(34, (uint32_t)&irq2, 0x08, 0b10001110);
	IDTSetGate(35, (uint32_t)&irq3, 0x08, 0b10001110);
	IDTSetGate(36, (uint32_t)&irq4, 0x08, 0b10001110);
	IDTSetGate(37, (uint32_t)&irq5, 0x08, 0b10001110);
	IDTSetGate(38, (uint32_t)&irq6, 0x08, 0b10001110);
	IDTSetGate(39, (uint32_t)&irq7, 0x08, 0b10001110);
	IDTSetGate(40, (uint32_t)&irq8, 0x08, 0b10001110);
	IDTSetGate(41, (uint32_t)&irq9, 0x08, 0b10001110);
	IDTSetGate(42, (uint32_t)&irq10, 0x08, 0b10001110);
	IDTSetGate(43, (uint32_t)&irq11, 0x08, 0b10001110);
	IDTSetGate(44, (uint32_t)&irq12, 0x08, 0b10001110);
	IDTSetGate(45, (uint32_t)&irq13, 0x08, 0b10001110);
	IDTSetGate(47, (uint32_t)&irq14, 0x08, 0b10001110);
	IDTSetGate(48, (uint32_t)&irq15, 0x08, 0b10001110);
}

char* INTERRUPT_ERROR[32] = // https://wiki.osdev.org/Exceptions
{
	"DIVISION_BY_0",
	"DEBUG",
	"NON_MASKABLE_INTERRUPT",
	"BREAKPOINT",
	"OVERFLOW",
	"BOUND_RANGE_EXCEEDED",
	"INVALID_OPCODE",
	"DEVICE_NOT_AVAILABLE",
	"DOUBLE_FAULT",
	"COPROCESSOR_SEGMENT_OVERRUN",
	"INVALID_TSS",
	"SEGMENT_NOT_PRESENT",
	"STACK_SEGMENT_FAULT",
	"GENERAL_PROTECTION_FAULT",
	"PAGE_FAULT",
	"",
	"x87_FLOATING_POINT_EXCEPTION",
	"ALIGNMENT_CHECK",
	"MACHINE_CHECK",
	"SIMD_FLOATING_POINT_EXCEPTION",
	"VIRTUALIZATION_EXCEPTION",
	"CONTROL_PROTECTION_EXCEPTION",
	"",
	"",
	"",
	"",
	"",
	"",
	"HYPERVISOR_INJECTION_EXCEPTION",
	"VMM_COMMUNICATION_EXCEPTION",
	"SECURITY_EXCEPTION",
	""
};

extern void IDTHandler(struct IDTRegisters* r)
{
	ShowCursor(false, 0x0f, 0x0f);
	ClearScreenText(BG_BLUE);
	if(r->int_no < 32)
	{
		uint16_t cursor = (80 + 2);
		printstr("FATAL EXCEPTION : ", (BG_BLUE | FG_LRED), &cursor);

		text_endline(&cursor);
		text_endline(&cursor);
		cursor += 2;

		printstr("Kernel panic : ", (BG_BLUE | FG_WHITE), &cursor);

		char* exceptionStr = INTERRUPT_ERROR[r->int_no];
		printstr(exceptionStr, (BG_BLUE | FG_WHITE), &cursor);

		text_endline(&cursor);
		text_endline(&cursor);

		cursor += 4;

		printstr("Exception ID : ", (BG_BLUE | FG_WHITE), &cursor);

		char* exceptionNbStr = uintToStr1B(r->int_no);
		printstr(exceptionNbStr, (BG_BLUE | FG_WHITE), &cursor);
		free(exceptionNbStr);

		text_endline(&cursor);
		cursor += 4;

		printstr("Error code : ", (BG_BLUE | FG_WHITE), &cursor);

		char* errorCodeNbStr = uintToStr1B(r->err_code);
		printstr(errorCodeNbStr, (BG_BLUE | FG_WHITE), &cursor);
		free(errorCodeNbStr);
		while(true);
	}
}