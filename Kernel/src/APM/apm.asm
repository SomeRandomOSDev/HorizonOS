; https://wiki.osdev.org/APM
[bits 16]

[extern EnableProtectedMode]
[extern DisableProtectedMode]

global APMGetVersion
APMGetVersion:
	mov ah, 0x53
	mov al, 0x00
	xor bx, bx
	int 0x15
	jc .notSupported
	jmp .noError
.notSupported:
	cmp ah, 0x03
	jne Error
.noError:
	ret

Error:
	xor ax, ax
	ret	

global APMConnect32bitPM
APMConnect32bitPM:
	mov ah, 0x53
	mov al, 0x03
	xor bx, bx
	int 0x15
	jc Error
	ret

global APMEnable
APMEnable:
	mov ah, 0x53
	mov al, 0x08
	mov bx, 0x0001
	mov cx, 0x0001
	int 0x15
	jc Error
	ret

global APMSetDriverVersion1_1
APMSetDriverVersion1_1:
	mov al, 0x0e
	mov bx, 0x0000  ; Device ID of system BIOS
	mov ch, 0x01    ; Version major
	mov cl, 0x01    ; Version minor
	int 0x15
	jc .versionError
	jmp .noError
.versionError:
	ret
.noError:
	ret

global APMSetPowerStateOff
; APMSetPowerStateOff:
; 	call DisableProtectedMode
; 	sti
; 	mov ah, 0x53
; 	mov al, 0x07
; 	mov bx, 0x0001
; 	mov cx, 0x03
; 	int 0x15
; 	jc Error
; 	mov ax, 1
; 	ret

APMSetPowerStateOff:
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

	mov bp, 0x7c00
	mov sp, bp

	sti
	mov ah, 0x53
	mov al, 0x07
	mov bx, 0x0001
	mov cx, 0x03
	int 0x15
	jc Error
	mov ax, 1
	ret

IDRPtr_BIOS:
	dw 0x3ff
	dd 0