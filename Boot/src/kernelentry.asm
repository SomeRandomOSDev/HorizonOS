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

    lgdt [GDT_Ptr]
    
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

GDT_Begin:
    GDT_NULL:
        dq 0x0

    GDT_CODE_32:
        dw 0xffff
        dw 0x0
        db 0x0
        db 0b10011010
        db 0b11001111
        db 0x0

    GDT_DATA_32:
        dw 0xffff
        dw 0x0
        db 0x0
        db 0b10010010
        db 0b11001111
        db 0x0

    GDT_CODE_16:
        dw 0xffff
        dw 0x0
        db 0x0
        db 0b10011010
        db 0b00001111
        db 0x0

    GDT_DATA_16:
        dw 0xffff
        dw 0x0
        db 0x0
        db 0b10010010
        db 0b00001111
        db 0x0
GDT_End:

GDT_Ptr:
    dw GDT_End - GDT_Begin - 1
    dd GDT_Begin

; %include "apm.asm"
; %include "pm.asm"