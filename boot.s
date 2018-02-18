.set ALIGN, 1<<0
.set MEMINFO, 1<<1
.set FLAGS, ALIGN | MEMINFO
.set MAGIC, 0x1BADB002
.set CHECKSUM, -(MAGIC + FLAGS)

.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

.section .bss
.align 16
stack_bottom:
.skip 16384 #16 KiB
stack_top:


.section .text
.global _start
.type _start, @function
_start:
	mov $stack_top, %esp
	call kernel_main

	cli

1:	hlt
	jmp 1b

.global gdt_flush
.global gdt_p
gdt_flush:
	lgdt gdt_p
	mov $0x10, %ax
	mov %ax, %ds
	mov %ax, %ss
	mov %ax, %es
	mov %ax, %fs
	mov %ax, %gs
	ljmp $0x08, $flush

flush:
	ret

.global idt_flush
.global idt_p
idt_flush:
	lidt idt_p
	int $5
	ret

.global fault_handler
.global isr_common
isr_common:
	pusha
	push %ds
	push %es
	push %fs
	push %gs
	mov $0x10, %ax
	mov %ax, %ds
	mov %ax, %es
	mov %ax, %fs
	mov %ax, %gs
	mov %esp, %eax
	push %eax
	call fault_handler
	pop %eax
	pop %gs
	pop %fs
	pop %es
	pop %ds
	popa
	add $0x08, %esp
	iret

.global isr0
.global isr1
.global isr2
.global isr3
.global isr4
.global isr5
.global isr6
.global isr7
.global isr8
.global isr9
.global isr10
.global isr11
.global isr12
.global isr13
.global isr14
.global isr15
.global isr16
.global isr17
.global isr18
.global isr19
.global isr20
.global isr21
.global isr22
.global isr23
.global isr24
.global isr25
.global isr26
.global isr27
.global isr28
.global isr29
.global isr30
.global isr31

isr0:
	cli
	push $0x00
	push $0x00
	jmp isr_common 

isr1:
	cli
	push $0x00
	push $0x01
	jmp isr_common 

isr2:
	cli
	push $0x00
	push $0x02
	jmp isr_common 

isr3:
	cli
	push $0x00
	push $0x03
	jmp isr_common 

isr4:
	cli
	push $0x00
	push $0x04
	jmp isr_common 

isr5:
	cli
	push $0x00
	push $0x05
	jmp isr_common 

isr6:
	cli
	push $0x00
	push $0x06
	jmp isr_common 

isr7:
	cli
	push $0x00
	push $0x07
	jmp isr_common 

isr8:
	cli
	push $0x08
	jmp isr_common 

isr9:
	cli
	push $0x00
	push $0x09
	jmp isr_common 

isr10:
	cli
	push $0x0A
	jmp isr_common 

isr11:
	cli
	push $0x0B
	jmp isr_common 

isr12:
	cli
	push $0x0C
	jmp isr_common 

isr13:
	cli
	push $0x0D
	jmp isr_common 

isr14:
	cli
	push $0x0E
	jmp isr_common 

isr15:
	cli
	push $0x00
	push $0x0F
	jmp isr_common 

isr16:
	cli
	push $0x00
	push $0x10
	jmp isr_common 

isr17:
	cli
	push $0x00
	push $0x11
	jmp isr_common 

isr18:
	cli
	push $0x00
	push $0x12
	jmp isr_common 

isr19:
	cli
	push $0x00
	push $0x13
	jmp isr_common 

isr20:
	cli
	push $0x00
	push $0x14
	jmp isr_common 

isr21:
	cli
	push $0x00
	push $0x15
	jmp isr_common 

isr22:
	cli
	push $0x00
	push $0x16
	jmp isr_common 

isr23:
	cli
	push $0x00
	push $0x17
	jmp isr_common 

isr24:
	cli
	push $0x00
	push $0x18
	jmp isr_common 

isr25:
	cli
	push $0x00
	push $0x19
	jmp isr_common 

isr26:
	cli
	push $0x00
	push $0x1A
	jmp isr_common 

isr27:
	cli
	push $0x00
	push $0x1B
	jmp isr_common 

isr28:
	cli
	push $0x00
	push $0x1C
	jmp isr_common 

isr29:
	cli
	push $0x00
	push $0x1D
	jmp isr_common 

isr30:
	cli
	push $0x00
	push $0x1E
	jmp isr_common 

isr31:
	cli
	push $0x00
	push $0x1F
	jmp isr_common 


.size _start, . - _start
