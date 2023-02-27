#pragma once

// https://wiki.osdev.org/Memory_Map_(x86)
#define RAM_BEGIN 0x01000000
#define RAM_SIZE  (64 * MB) // TODO : Detect memory
#define RAM_END   (RAM_SIZE + RAM_BEGIN)

struct MemHeader
{
	size_t size;
	bool free,
	     lastBlock;
} __attribute__((packed));

void initMem();
void memset(void* ptr, uint8_t val, size_t size);
void* memcpy(void* dest, const void* src, size_t size);
bool memcmp(void* str, void* str2, size_t size);
char* strcpy(char *dest, const char *src);
bool strcmp(char* str, char* str2);
uint32_t strlen(char* str);
void* malloc(size_t size);
void free(void* ptr);