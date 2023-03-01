PROTECTED_MODE_ENABLE_FLAG equ (1 << 0)

global EnableProtectedMode
EnableProtectedMode:
	[bits 16]
	mov eax, cr0
	or eax, PROTECTED_MODE_ENABLE_FLAG
	mov cr0, eax
	ret

global DisableProtectedMode
DisableProtectedMode:
	[bits 32]
	cli
	jmp word 0x18:.pm16
.pm16:
	[bits 16]
	mov ax, 0x20
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax

	lidt [IDRPtr_BIOS]

	mov eax, cr0
	and eax, ~1
	mov cr0, eax

	jmp word 0:.rmode
.rmode:
	xor ax, ax
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax

	; mov bp, 0x7c00
    ; mov sp, bp

	ret
    ; retf

IDRPtr_BIOS:
	dw 0x3ff
	dd 0