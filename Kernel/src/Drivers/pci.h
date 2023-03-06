#pragma once

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

void PCI_CheckDevice(uint8_t bus, uint8_t device, uint8_t* functions, uint16_t* deviceId, uint16_t* vendorId) 
{
    *functions = 0;

    *deviceId = (PCI_CAMRead32(bus, device, 0, 0) >> 16);
    *vendorId = (PCI_CAMRead32(bus, device, 0, 0) & 0xffff);
 
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
			uint16_t deviceId = 0, vendorId = 0;
			PCI_CheckDevice(i, j, &func, &deviceId, &vendorId);
			if(deviceId != 0xffff)
				printf("Bus :  %xh Slot : %xh Device ID : %xh Vendor ID : %xh Functions %u\n", i, j, deviceId, vendorId, func);
		}
	}
}