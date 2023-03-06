[bits 16]
[org 0x7c00]

ENDLINE                    equ 0x0a
CARRIAGE_RET               equ 0x0d
%define NEWLINE            ENDLINE, CARRIAGE_RET
VGA_TEXT_80x25_16_COLORS   equ 0x03

section .text
global _start

; FAT Header
jmp short _start     ; db 0xeb, 0x3c, 0x90
nop
oemIdentifier:       db "MSWIN4.1"    ; Oem identifier
bytesperSector:      dw 512           ; Bytes per sector
sectorsPerCluster:   db 1             ; Sectors per cluster
reservedSectors:     dw 1             ; Reserved sectors
fatCount:            db 2             ; FAT count
rootDirEntries:      dw 224           ; Root dir entries count
numberofSectors:     dw 2880          ; 1.44Mib total space
mediaDescriptorType: db 0xf0          ; Media descriptor type
sectorsPerFAT:       dw 9             ; Sectors per FAT ; Max 3072 sectors disk
sectorsPerTrack:     dw 18            ; Sectors per track
headCount:           dw 2             ; Head count
hiddenSectors:       dd 0             ; Hidden sectors
largeSectorCount:    dd 0             ; Large sector count
; FAT Header
; Extended FAT header
driveType:           db 0             ; Floppy disk
                     db 0             ; Reserved
                     db 0x28          ; Signature
                     db 0, 0, 0, 0    ; Volume ID ; Ignore this
volumeLabel:         db "HorizonOS  " ; Volume label
sysID:               db "FAT12   "    ; System ID
; Extended FAT header

_start: ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; Entry point
    sti

    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax

    mov bp, 0x7c00
    mov sp, bp

    push es
    push word .bootloader
    retf
.bootloader:
    mov al, VGA_TEXT_80x25_16_COLORS
    call SetVGAVideoMode

    mov [driveType], dl ; Drive number
    ; mov [driveType], byte 0x0

    mov si, loadingText
    call puts

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    mov ax, 0
    mov es, ax
    mov bx, 0x7e00
    ; es:bx = 0x7e00

    ; ax = LBA
    mov ah, 0
    mov al, byte [fatCount]
    mul word [sectorsPerFAT]
    add ax, [reservedSectors]

    push ax

    mov ax, 0
    mov es, ax
    mov bx, 0x7e00

    mov ax, [rootDirEntries]
    mov dx, 32
    mul dx
    add ax, [bytesperSector]
    dec ax
    div word [bytesperSector]
    pop cx
    add ax, cx

    add ax, 1

    mov cl, 0x40
    mov dl, byte [driveType]
    call ReadDisk
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

    mov si, diskReadText
    call puts

    lgdt [GDT_Ptr]

    ; mov ax, 0x07e0
    ; mov es, ax
    ; mov di, 0x0000
    ; mov [es:di], byte 0xeb
    ; inc di
    ; mov [es:di], byte 0xfe

    jmp 0x0000:0x7e00

; // input
; ; al = mode
SetVGAVideoMode:
    push ax
    mov ah, 0
    int 0x10
    pop ax
    ret

; // input
; ; al = char
putc:
    push ax
    mov ah, 0x0e
    int 0x10
    pop ax
    ret

; // input
; ; si = ptr to text
puts:
    push ax
    mov ah, 0x0e
.printLoop:
    mov al, [si]
    cmp al, 0
    je .endPrintLoop
    int 0x10
    inc si
    jmp .printLoop
.endPrintLoop:
    pop ax
    ret

driveResetError:
    mov si, driveResetErrorText
    call puts
    jmp FATAL_ERROR

FATAL_ERROR:
    mov si, fatalErrorText
    call puts
    jmp $

; // input
; ; dl = drive
ResetDrive:
    pusha
    mov ah, 0
    stc
    int 0x13
    jc driveResetError
    ; jmp driveResetError
    popa
    ret

; // input
; ; ax = lba
; //output
; ; C = (lba / spt) / heads
; ; H = (lba / spt) % heads
; ; S = (lba % spt) + 1
; ; cx (6-15) = C
; ; dh = H
; ; cx (0-5) = S
LBAToCHS:
    push ax
    push dx

    xor dx, dx
    div word [sectorsPerTrack]

    inc dx
    mov cx, dx

    xor dx, dx
    div word [headCount]

    mov dh, dl
    mov ch, al
    shl ah, 6
    or cl, ah

    pop ax
    mov dl, al
    pop ax
    
    ret

; // input
; ; ax = LBA
; ; cl = sectors count
; ; dl = drive number
; ; es:bx = where to load data
; // output
; ; [es:bx] = data
ReadDisk:
    pusha

    push cx
    call LBAToCHS
    pop ax
    
    mov ah, 0x02
    mov di, 5
.ReadDiskLoop:
    pusha
    stc
    int 0x13
    jnc .endReadDisk

    popa
    call ResetDrive

    dec di
    cmp di, 0
    jne .ReadDiskLoop
.diskReadError:
    mov si, diskReadErrorText
    call puts
    jmp FATAL_ERROR
.endReadDisk:
    popa
    popa

    ret

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

loadingText:           db "Loading kernel...", NEWLINE, 0
driveResetErrorText:   db "Error while resetting disk.", NEWLINE, 0
diskReadErrorText:     db "Error while reading disk.", NEWLINE, 0
diskReadText:          db "Jumping to the kernel...", NEWLINE, 0
fatalErrorText:        db "FATAL ERROR : Cannot continue", 0

times 510 - ($ - $$) db 0x00
dw 0xaa55
