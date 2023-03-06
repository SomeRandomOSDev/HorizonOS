[global PCIIsSupported]
PCIIsSupported:
	mov ax, 0xb101
	int 0x1a
	mov ax, 0
	ret