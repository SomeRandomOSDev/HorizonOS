#pragma once

#define PCI_CONFIG_ADDRESS  0xcf8
#define PCI_CONFIG_DATA     0xcfc

void PCI_CAMSelectRegister(uint8_t bus, uint8_t slot, uint8_t func, uint8_t reg) 
{ 
    uint32_t address = (0x80000000
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

void PCI_CheckDevice(uint8_t bus, uint8_t device, uint8_t* functions) 
{
    uint8_t function = 0;

    functions = 0;
 
    if((PCI_CAMRead32(bus, device, function, 0) & 0xffff) == 0xffff) 
        return;

    functions = 1;

    // checkFunction(bus, device, function);
    uint8_t headerType = (PCI_CAMRead32(bus, device, function, 3) >> 16);
    if(!(headerType >> 7)) 
    {
        for(function = 1; function < 8; function++) 
        {
            if((PCI_CAMRead32(bus, device, function, 0) & 0xffff) != 0xffff) 
            {
                // checkFunction(bus, device, function);
                functions++;
            }
        }
    }
}