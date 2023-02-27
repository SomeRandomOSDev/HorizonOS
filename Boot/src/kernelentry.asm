[bits 16]
[extern init]

[extern EnableProtectedMode]

[extern APMGetVersion]
[extern APMConnect32bitPM]
[extern APMSetDriverVersion1_1]
[extern APMEnable]
[extern APMSetPowerStateOff]

section .text

GDT_CODE_SEG_32 equ 0x08
GDT_DATA_SEG_32 equ 0x10
GDT_CODE_SEG_16 equ 0x18
GDT_DATA_SEG_16 equ 0x20

jmp kernelEntry

pmEntry:
    [bits 32]

    pop ax
    mov bx, ax

    mov ax, GDT_DATA_SEG_32
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
        
    mov ebp, 0x90000
    mov esp, ebp

    push ebx

    call init

    cli
    hlt
    [bits 16]

kernelEntry:
    [bits 16]
    
	call APMGetVersion
	push ax
	call APMConnect32bitPM
	call APMSetDriverVersion1_1
    call APMEnable

	cli 
	call EnableProtectedMode

	; mov ah, 0x0f
	; mov al, 'A'
	; mov [0xb8000], ax

	jmp dword GDT_CODE_SEG_32:pmEntry

; %include "apm.asm"
; %include "pm.asm"