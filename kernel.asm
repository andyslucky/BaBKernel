;;kernel.asm

;nasm directive - 32 bit
bits 32
section .text
        ;multiboot spec
        align 4
        dd 0x1BADB002            ;magic
        dd 0x00                  ;flags
        dd - (0x1BADB002 + 0x00) ;checksum. m+f+c should be zero
		errstring db "No key was pressed!",10,0
		shouldexit db 0;

global start
global idle
global sleep
global keyboard_handler
global read_port
global write_port
global load_idt
extern kmain
extern print	        ;kmain is defined in the c file
extern keyboard_handler_main

read_port:
	mov edx, [esp + 4]
			;al is the lower 8 bits of eax
	in al, dx	;dx is the lower 16 bits of edx
	ret

write_port:
	mov   edx, [esp + 4]    
	mov   al, [esp + 4 + 4]  
	out   dx, al  
	ret

load_idt:
	mov edx, [esp + 4]
	lidt [edx]
	sti 				;turn on interrupts
	ret
check_heap:
	mov eax, [esp+4]
	cmp eax,esp
	je nomem
	mov eax,1
	ret
	nomem:mov eax,0
	ret
keyboard_handler:                 
	call    keyboard_handler_main
	iretd
idle:
loopbegin:hlt
	jmp loopbegin
	exit:ret

start:
  cli 			;block interrupts
  mov esp, stack_space	;set stack pointer
  call kmain		 	;halt the CPU

section .bss
resb 8192		;8KB for stack
stack_space: