[bits 16]
[extern init]

[extern EnableProtectedMode]

[extern APMGetVersion]
[extern APMConnect32bitPM]
[extern APMSetDriverVersion1_1]
[extern APMEnable]
[extern APMSetPowerStateOff]

[extern PCIIsSupported]

section .text

GDT_CODE_SEG_32 equ 0x08
GDT_DATA_SEG_32 equ 0x10
GDT_CODE_SEG_16 equ 0x18
GDT_DATA_SEG_16 equ 0x20

jmp kernelEntry

pmEntry:
    [bits 32]

    pop ax
    and eax, 0xffff
    mov ebx, eax

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

    add esp, 4

    cli
    hlt

kernelEntry:
    [bits 16]
    
	call APMGetVersion
	push ax
	call APMConnect32bitPM
	call APMSetDriverVersion1_1
    call APMEnable

    ; call PCIIsSupported
    ; push ax

	cli
	call EnableProtectedMode

	jmp dword GDT_CODE_SEG_32:pmEntry

; %include "apm.asm"
; %include "pm.asm"