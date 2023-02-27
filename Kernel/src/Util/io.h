#pragma once

uint8_t inb(uint16_t address);
void outb(uint16_t address, uint8_t byte);
uint16_t inw(uint16_t address);
void outw(uint16_t address, uint16_t word);
uint32_t ind(uint16_t address);
void outd(uint16_t address, uint32_t dWord);