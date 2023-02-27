qemu-system-i386 \
-drive format=raw,file="horizonos.bin",index=0,if=floppy \
-m 64M -soundhw pcspk -device sb16 
# -drive format=raw,id=disk,file="disk.bin",if=none \
# -device ahci,id=ahci \
# -device ide-hd,drive=disk,bus=ahci.0 \