#pragma once

// https://wiki.osdev.org/ATA_PIO_Mode

#define ATA_BUS_1_IO 0x1f0
#define ATA_BUS_2_IO 0x170

#define ATA_BUS_3_IO 0x1e8
#define ATA_BUS_4_IO 0x168

#define ATA_BUS_1_CTRL 0x3f6
#define ATA_BUS_2_CTRL 0x376

#define ATA_BUS_3_CTRL 0x3e6
#define ATA_BUS_4_CTRL 0x366

#define ATA_DISK_MASTER 1
#define ATA_DISK_SLAVE  0

typedef uint8_t ATA_DISK;
typedef uint8_t ATA_BUS;
typedef uint8_t ATA_REGISTER;

#define ATA_REGISTER_DATA          0
#define ATA_REGISTER_ERROR         1
#define ATA_REGISTER_FEATURES      1
#define ATA_REGISTER_SECTOR_COUNT  2
#define ATA_REGISTER_SECTOR_NUMBER 3
#define ATA_REGISTER_CYLINDER_LOW  4
#define ATA_REGISTER_CYLINDER_HIGH 5

#define ATA_REGISTER_LBA_LOW       3
#define ATA_REGISTER_LBA_MIDDLE    4
#define ATA_REGISTER_LBA_HIGH      5

#define ATA_REGISTER_DRIVE_HEAD    6
#define ATA_REGISTER_STATUS        7
#define ATA_REGISTER_COMMAND       7

#define ATA_REGISTER_ALTERNATE_STATUS   0
#define ATA_REGISTER_DEVICE_CONTROL     0
#define ATA_REGISTER_DRIVE_ADDRESS      1


uint16_t ATAGetIOPort(ATA_DISK disk, ATA_BUS bus)
{
	if(disk == ATA_DISK_SLAVE)
	{
		if(bus == 0)
			return ATA_BUS_1_IO;
		return ATA_BUS_2_IO;
	}
	else
	{
		if(bus == 0)
			return ATA_BUS_3_IO;
		return ATA_BUS_4_IO;
	}
}

uint16_t ATAGetCtrlPort(ATA_DISK disk, ATA_BUS bus)
{
	if(disk == ATA_DISK_SLAVE)
	{
		if(bus == 0)
			return ATA_BUS_1_CTRL;
		return ATA_BUS_2_CTRL;
	}
	else
	{
		if(bus == 0)
			return ATA_BUS_3_CTRL;
		return ATA_BUS_4_CTRL;
	}
}

uint16_t ATAReadIORegister(ATA_REGISTER reg, ATA_DISK disk, ATA_BUS bus)
{
	return inw(ATAGetIOPort(disk, bus) + reg);
}

uint16_t ATAReadControlRegister(ATA_REGISTER reg, ATA_DISK disk, ATA_BUS bus)
{
	return inw(ATAGetCtrlPort(disk, bus) + reg);
}

void ATASetIORegister(ATA_REGISTER reg, ATA_DISK disk, ATA_BUS bus, uint16_t val)
{
	outw(ATAGetIOPort(disk, bus) + reg, val);
}

void ATASetControlRegister(ATA_REGISTER reg, ATA_DISK disk, ATA_BUS bus, uint16_t val)
{
	outw(ATAGetCtrlPort(disk, bus) + reg, val);
}

uint8_t ATAIdentify(ATA_DISK disk)
{
	if(disk == ATA_DISK_MASTER)
		ATASetIORegister(ATA_REGISTER_DRIVE_HEAD, ATA_DISK_MASTER, 0, 0xa0);
	else
		ATASetIORegister(ATA_REGISTER_DRIVE_HEAD, ATA_DISK_SLAVE, 0, 0xb0);

	outb(0x1f2, 0);
	outb(0x1f3, 0);
	outb(0x1f4, 0);
	outb(0x1f5, 0);

	outb(0x1f7, 0xec);

	return inb(0x1f7);
}