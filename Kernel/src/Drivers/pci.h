#pragma once

// https://wiki.osdev.org/PCI

#define PCI_CONFIG_ADDRESS  0xcf8
#define PCI_CONFIG_DATA     0xcfc

void PCI_CAMSelectRegister(uint8_t bus, uint8_t slot, uint8_t func, uint8_t reg) 
{ 
    uint32_t address = 
    ( ((uint32_t)1 << 31)
    | (bus << 16)
    | (slot << 11)
    | (func << 8)
    | (reg * 4));
 
    outd(PCI_CONFIG_ADDRESS, address);
}

uint32_t PCI_CAMReadSelected32() 
{ 
    return ind(PCI_CONFIG_DATA);
}

uint32_t PCI_CAMRead32(uint8_t bus, uint8_t slot, uint8_t func, uint8_t reg) 
{ 
    PCI_CAMSelectRegister(bus, slot, func, reg);
    return ind(PCI_CONFIG_DATA);
}

void PCI_CheckDevice(uint8_t bus, uint8_t device, uint8_t* functions, uint16_t* deviceId, uint16_t* vendorId, uint16_t* classCode) 
{
    *functions = 0;

    *deviceId = (PCI_CAMRead32(bus, device, 0, 0) >> 16);
    *vendorId = (PCI_CAMRead32(bus, device, 0, 0) & 0xffff);

    *classCode = (PCI_CAMRead32(bus, device, 0, 2) >> 16);
 
    if((PCI_CAMRead32(bus, device, 0, 0) & 0xffff) == 0xffff) 
    {
        return;
    }

    *functions = 1;

    // checkFunction(bus, device, function);
    uint8_t headerType = (PCI_CAMRead32(bus, device, 0, 3) >> 16);
    if(!(headerType >> 7)) 
    {
        for(uint8_t function = 2; function < 8; function++) 
        {
            if((PCI_CAMRead32(bus, device, function, 0) & 0xffff) != 0xffff) 
            {
                // checkFunction(bus, device, function);
                (*functions)++;
            }
        }
    }
}

void PCI_ScanBuses()
{
    for(uint16_t i = 0; i < 256; i++)
	{
		for(uint8_t j = 0; j < 32; j++)
		{
			uint8_t func = 0;
			uint16_t deviceId = 0, vendorId = 0, class = 0;
            char* vendorStr = "", *classStr = "";
			PCI_CheckDevice(i, j, &func, &deviceId, &vendorId, &class);
            switch(vendorId)
            {
            case 0x1234:
                vendorStr = "Intel Corporation";
                break;
            case 0x8086:
                vendorStr = "Intel Corporation";
                break;
            default:
                vendorStr = "Unknown Vendor   ";
                break;
            }
            switch(class >> 8)
            {
            case 0:
                classStr = "Unclassified Device";
                break;
            case 1:
                classStr = "Mass Storage Controller";
                break;
            case 2:
                classStr = "Network Controller";
                break;
            case 3:
                classStr = "Display Controller";
                break;
            case 4:
                classStr = "Multimedia Controller";
                break;
            case 5:
                classStr = "Memory Controller";
                break;
            case 6:
                classStr = "Bridge";
                break;
            case 7:
                classStr = "Simple Communication Controller";
                break;
            case 8:
                classStr = "Base System Peripheral";
                break;
            case 9:
                classStr = "Input Device Controller";
                break;
            case 0xa:
                classStr = "Docking Station";
                break;
            case 0xb:
                classStr = "Processor";
                break;
            case 0xc:
                classStr = "Serial Bus Controller";
                break;
            case 0xd:
                classStr = "Wireless Controller";
                break;
            case 0xe:
                classStr = "Intelligent Controller";
                break;
            case 0xf:
                classStr = "Satellite Communication Controller";
                break;
            case 0x10:
                classStr = "Encryption Controller";
                break;
            case 0x11:
                classStr = "Signal Processing Controller";
                break;
            case 0x12:
                classStr = "Processing Accelerator";
                break;
            case 0x13:
                classStr = "Non-Essential Instrumentation";
                break;
            case 0x14:
                classStr = "Reserved";
                break;
            case 0x40:
                classStr = "Co-Processor";
                break;
            case 0x41:
                classStr = "Reserved";
                break;
            case 0xff:
                classStr = "Unassigned Class";
                break;
            default:
                classStr = "Unknown Class";
                break;
            }
			if(deviceId != 0xffff)
				printf("%x:%x %x: %x:%x | %s | %s\n", i, j, class, vendorId, deviceId, vendorStr, classStr);
		}
	}
}