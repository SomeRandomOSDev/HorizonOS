global LoadIDT
extern idtPtr
LoadIDT:
	lidt[idtPtr]
	ret

%macro ISR_NO_PARAMS 1
global isr%1
isr%1:
	cli
    push byte 0  ; No error code
    push byte %1

	jmp isrCommon
%endmacro

%macro ISR_PARAMS 1
global isr%1
isr%1:
	cli
    push byte %1

	jmp isrCommon
%endmacro

ISR_NO_PARAMS 0
ISR_NO_PARAMS 1
ISR_NO_PARAMS 2
ISR_NO_PARAMS 3
ISR_NO_PARAMS 4
ISR_NO_PARAMS 5
ISR_NO_PARAMS 6
ISR_NO_PARAMS 7
ISR_PARAMS 8
ISR_NO_PARAMS 9
ISR_PARAMS 10
ISR_PARAMS 11
ISR_PARAMS 12
ISR_PARAMS 13
ISR_PARAMS 14
ISR_NO_PARAMS 15
ISR_NO_PARAMS 16
ISR_NO_PARAMS 17
ISR_NO_PARAMS 18

ISR_NO_PARAMS 19
ISR_NO_PARAMS 20
ISR_NO_PARAMS 21
ISR_NO_PARAMS 22
ISR_NO_PARAMS 23
ISR_NO_PARAMS 24
ISR_NO_PARAMS 25
ISR_NO_PARAMS 26
ISR_NO_PARAMS 27
ISR_NO_PARAMS 28
ISR_NO_PARAMS 29
ISR_NO_PARAMS 30
ISR_NO_PARAMS 31

extern IDTHandler
isrCommon:
	pusha
    push ds
    push es
    push fs
    push gs
    mov ax, 0x10 ; GDT_Data seg
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov eax, esp
    push eax

    mov eax, IDTHandler
    call eax ; Special call, preserves the 'eip' register
    
    pop eax
    pop gs
    pop fs
    pop es
    pop ds
    popa
    add esp, 8
    iret ; pop CS, EIP, EFLAGS, SS, ESP