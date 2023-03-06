# cd "/mnt/c/Users/PC/Desktop/OS/HorizonOS"
# mdir -i "horizonos.bin"

horizonos.bin: Boot/bin/boot.bin Kernel/bin/kernel.bin Makefile
	echo "Building HorizonOS..."

	dd if="/dev/zero" of="horizonos.bin" bs=512 count=2880 # 1.44Mio
	mkfs.fat -F 12 -n "HORIZONOS" "horizonos.bin"
	dd if="Boot/bin/boot.bin" of="horizonos.bin" conv=notrunc

	# mcopy -i "horizonos.bin" "Files/System" "::Sys32"
	# mcopy -i "horizonos.bin" "Kernel/bin/kernel.bin" "::Sys32/kernel.bin"
	mcopy -i "horizonos.bin" "Kernel/bin/kernel.bin" "::kernel.bin"
	mcopy -i "horizonos.bin" "test.txt" "::test.txt"

Kernel/bin/kernel.bin: Kernel/src/kernel.c Boot/bin/boot.bin Kernel/src/Util/* Kernel/src/Drivers/PS2/* Kernel/src/Drivers/* Kernel/src/Drivers/ATA/* Kernel/src/Drivers/Sound/* Makefile
	echo "Building kernel..."

	nasm "Kernel/src/IDT/idt.asm" -o "Kernel/bin/idt.o" -f elf
	nasm "Kernel/src/IRQ/irq.asm" -o "Kernel/bin/irq.o" -f elf
	# nasm "gdt.asm" -o "gdt.o" -f elf
	nasm "Kernel/src/Paging/paging.asm" -o "Kernel/bin/paging.o" -f elf
	nasm "Kernel/src/APM/apm.asm" -o "Kernel/bin/apm.o" -f elf

	# -mkernel 
	gcc -Wall -m32 -ffreestanding -fstrength-reduce\
    -finline-functions -fno-pic \
    -mno-red-zone -masm=intel -std=gnu99 -nostdlib -c \
    -static -fno-common -fno-exceptions  \
    -fno-non-call-exceptions -fno-builtin \
    -o "Kernel/bin/kernel.o" "Kernel/src/kernel.c"
	
	ld "Boot/bin/kernelentry.o" "Kernel/bin/kernel.o" "Kernel/bin/idt.o" "Kernel/bin/irq.o" "Kernel/bin/paging.o" "Kernel/bin/apm.o" "Boot/bin/pm.o" "Boot/bin/pci.o" -o "Kernel/bin/kernel.bin" -T "linker.ld" -m elf_i386 # --oformat binary  -e init -Ttext 0x7c00 --oformat binary -m elf_i386 "gdt.o" 

Boot/bin/boot.bin: Boot/src/* Makefile
	echo "Building bootloader..."

	nasm "Boot/src/boot.asm" -o "Boot/bin/boot.bin" -f bin
	nasm "Boot/src/kernelentry.asm" -o "Boot/bin/kernelentry.o" -f elf
	nasm "Boot/src/pm.asm" -o "Boot/bin/pm.o" -f elf
	nasm "Boot/src/pci.asm" -o "Boot/bin/pci.o" -f elf

clean:
	rm "horizonos.bin"  -v Boot/bin/* -v Kernel/bin/*