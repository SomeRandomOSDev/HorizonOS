#pragma once

void IRQRemap()
{
    outb(0x20, 0x11);
    outb(0xa0, 0x11);
    outb(0x21, 0x20);
    outb(0xa1, 0x28);
    outb(0x21, 0x04);
    outb(0xa1, 0x02);
    outb(0x21, 0x01);
    outb(0xa1, 0x01);
    outb(0x21, 0x0);
    outb(0xa1, 0x0);
}

extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();

#define SendEndOfInterrupt() if(reg->int_no >= 8) \
						     {                         \
						         outb(0xa0, 0x20);     \
						     }						   \
						     outb(0x20, 0x20);

extern void IRQHandler(struct IDTRegisters* reg)
{
	switch(reg->int_no)
	{
	case 0:
		{
			PITTimerHandler(reg);
			break;
		}
	case 1:
		{
			KeyboardHandler(reg);
			break;
		}
	default:
		{break;}
	}

	SendEndOfInterrupt();
}