#pragma once

void initMem()
{
	struct MemHeader* firstBlock = (struct MemHeader*)RAM_BEGIN;
	firstBlock->size = RAM_SIZE; // 512 bytes
	firstBlock->free = true;
	firstBlock->lastBlock = true;
}

void memset(void* ptr, uint8_t val, size_t size)
{
	for(uint32_t i = 0; i < size; i++)
		*((uint8_t*)ptr + i) = val;
}

void* memcpy(void* dest, const void* src, size_t size)
{
	for(uint32_t i = 0; i < size; i++)
		*((uint8_t*)dest + i) = *((uint8_t*)src + i);
	return dest;
}

bool memcmp(void* str, void* str2, size_t size)
{
	bool r = false;
	for(uint32_t i = 0; i < size; i++)
		r |= (((uint8_t*)str)[i] == ((uint8_t*)str2)[i]);
	return !r;
}

char* strcpy(char *dest, const char *src)
{
	uint32_t i = 0;
	while(src[i] != '\0')
	{
		dest[i] = src[i];
		i++;
	}
	return dest;
}

bool strcmp(char* str, char* str2)
{
	bool r = false;
	for(uint32_t i = 0; str[i] != '\0' && str2[i] != '\0'; i++)
		r |= (str[i] == str2[i]);
	return !r;
}

uint32_t strlen(char* str)
{
	uint32_t size = 0;
	if(str[0] == '\0')
		return 0;
	while(str[size] != '\0')
		size++;
	return size - 1;
}
 
void* malloc(size_t size)
{
	void* ptr = (void*)RAM_BEGIN;
	while(ptr < ((void*)RAM_END - (size + sizeof(struct MemHeader))))
	{
		struct MemHeader* hdr = (struct MemHeader*)ptr;
		if(hdr->free)
		{
			if(hdr->lastBlock)
			{
				hdr->size = size + sizeof(struct MemHeader);
				hdr->free = false;
				return ptr + sizeof(struct MemHeader);
			}
			else if(hdr->size == size)
			{
				hdr->free = false;
				return ptr + sizeof(struct MemHeader);
			}
			/*else if(hdr->size > size + sizeof(struct MemHeader)) // Not sure
			{
				hdr->free = false;
				struct MemHeader* newHdr = (struct MemHeader*)(ptr + size + sizeof(struct MemHeader));
				newHdr->free = true;
				newHdr->size = hdr->size - (size + sizeof(struct MemHeader));
				hdr->size = size;
				return ptr + sizeof(struct MemHeader);
			}*/
		}
		else if(hdr->lastBlock)
		{
			ptr += hdr->size + sizeof(struct MemHeader);
			struct MemHeader* newHdr = (struct MemHeader*)ptr;
			newHdr->size = size + sizeof(struct MemHeader);
			newHdr->free = false;
			newHdr->lastBlock = true;
			hdr->lastBlock = false;
			return ptr + sizeof(struct MemHeader);
		}
		else
		{
			ptr += hdr->size + sizeof(struct MemHeader);
		}
	}
	return NULL;
}

void free(void* ptr)
{
	struct MemHeader* hdr = (struct MemHeader*)ptr;
	hdr->free = true;
}