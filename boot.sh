qemu-system-i386 \
-drive format=raw,file="horizonos.bin",index=0,if=floppy \
-m 64M -soundhw pcspk -device sb16 