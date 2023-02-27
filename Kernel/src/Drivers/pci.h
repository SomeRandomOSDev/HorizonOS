#pragma once

#define PCI_CONFIG_ADDRESS  0xcf8
#define PCI_CONFIG_DATA     0xcfc

uint16_t PCIReadWord(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset) 
{
    uint32_t address;
    uint32_t lbus = bus;
    uint32_t lslot = slot;
    uint32_t lfunc = func;
    uint16_t tmp = 0;
 
    address = (uint32_t)((lbus << 16) | (lslot << 11) | (lfunc << 8) | (offset & 0xfc) | 0x80000000);
 
    outd(address, CONFIG_ADDRESS);

    tmp = (uint16_t)((ind(CONFIG_DATA) >> ((offset & 2) * 8)) & 0xffff);
    return tmp;
}

uint16_t PCICheckVendor(uint8_t bus, uint8_t slot) 
{
    uint16_t vendor, device;
    vendor = pciConfigReadWord(bus, slot, 0, 0);
    if(vendor != 0xffff) 
        device = pciConfigReadWord(bus, slot, 0, 2);
    return vendor;
}