# cd "/mnt/c/Users/PC/Desktop/OS/HorizonOS"
# objdump -D -Mintel,data16 --adjust-vma=0x7c00 -b binary -m i386 boot.bin
# mdir -i "horizonos.bin"
# wc -l *.c *.h *.asm

horizonos.bin: Boot/bin/boot.bin Kernel/bin/kernel.bin
	echo "Building HorizonOS..."

	dd if="/dev/zero" of="horizonos.bin" bs=512 count=2880 # 1.44Mio
	mkfs.fat -F 12 -n "HORIZONOS" "horizonos.bin"
	dd if="Boot/bin/boot.bin" of="horizonos.bin" conv=notrunc

	mcopy -i "horizonos.bin" "Files/System" "::System"
	mcopy -i "horizonos.bin" "Kernel/bin/kernel.bin" "::System/kernel.bin"
	mcopy -i "horizonos.bin" "test.txt" "::test.txt"

Kernel/bin/kernel.bin: Kernel/src/kernel.c Boot/bin/boot.bin Kernel/src/Util/util.c Kernel/src/Util/memory.c Kernel/src/Util/CMOS.h Kernel/src/Util/math.h Kernel/src/Drivers/PS2/keyboard.c Kernel/src/Drivers/PS2/ps2.h  Kernel/src/Drivers/ATA/ata.h Kernel/src/Drivers/pci.h Kernel/src/Drivers/pit.h Kernel/src/Drivers/printer.h Kernel/src/Drivers/parallel.h Kernel/src/Drivers/serial.h
	echo "Building kernel..."

	nasm "Kernel/src/IDT/idt.asm" -o "Kernel/bin/idt.o" -f elf
	nasm "Kernel/src/IRQ/irq.asm" -o "Kernel/bin/irq.o" -f elf
	# nasm "gdt.asm" -o "gdt.o" -f elf
	nasm "Kernel/src/Paging/paging.asm" -o "Kernel/bin/paging.o" -f elf
	nasm "Kernel/src/APM/apm.asm" -o "Kernel/bin/apm.o" -f elf

	gcc -Wall -m32 -ffreestanding -fstrength-reduce \
    -fomit-frame-pointer -finline-functions -fno-pic \
    -mno-red-zone -fno-builtin -masm=intel -std=gnu99 -nostdlib -c -o "Kernel/bin/kernel.o" "Kernel/src/kernel.c" # -D stdio # -O 
	
	ld "Boot/bin/kernelentry.o" "Kernel/bin/kernel.o" "Kernel/bin/idt.o" "Kernel/bin/irq.o" "Kernel/bin/paging.o" "Kernel/bin/apm.o" "Boot/bin/pm.o" -o "Kernel/bin/kernel.bin" -T "linker.ld" -m elf_i386 # --oformat binary  -e init -Ttext 0x7c00 --oformat binary -m elf_i386 "gdt.o" 

Boot/bin/boot.bin: Boot/src/boot.asm Boot/src/kernelentry.asm 
	echo "Building bootloader..."

	nasm "Boot/src/boot.asm" -o "Boot/bin/boot.bin" -f bin
	nasm "Boot/src/kernelentry.asm" -o "Boot/bin/kernelentry.o" -f elf
	nasm "Boot/src/pm.asm" -o "Boot/bin/pm.o" -f elf

clean:
	rm "horizonos.bin" "Kernel/bin/kernel.bin" "Boot/bin/boot.bin" \
	"Boot/bin/kernelentry.o" "Boot/bin/pm.o" "Kernel/bin/apm.o" \
	"Kernel/bin/idt.o" "Kernel/bin/irq.o" "Kernel/bin/kernel.o" \
	"Kernel/bin/paging.o"