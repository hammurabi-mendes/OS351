; Copyright (c) 2017, Hammurabi Mendes.
; License: BSD 2-clause
;
; For copyright and credits, please refer to the corresponding files
; in the project's root directory.

;;;;;;;;;;;;;;;;;;
; MULTIBOOT DATA ;
;;;;;;;;;;;;;;;;;;

; Define multiboot data
MULTIBOOT_MAGIC        equ 0xE85250D6
MULTIBOOT_ARCHITECTURE equ 0x0
MULTIBOOT_HEADERSIZE   equ (multiboot_end - multiboot_start)
MULTIBOOT_CHECKSUM     equ -(MULTIBOOT_MAGIC + MULTIBOOT_ARCHITECTURE + MULTIBOOT_HEADERSIZE)

section .multiboot
	align 4

global multiboot_information

; Declare the multiboot input
multiboot_start:
	dd MULTIBOOT_MAGIC
	dd MULTIBOOT_ARCHITECTURE
	dd MULTIBOOT_HEADERSIZE
	dd MULTIBOOT_CHECKSUM
	; A sequence of 'tags' follow, terminated by the 'null tag', described below
	; Please refer to the multiboot specification for details
	dw 0
	dw 0
	dd 8
multiboot_end:

multiboot_information:
	dq 0

;;;;;;;;;;;;;;;;;;;;;;
; 32-bit TEXT (code) ;
;;;;;;;;;;;;;;;;;;;;;;

section .text
	align 4
	bits 32

; Kernel entry point: creates a stack and jumps into C
global start
extern main

start:
	; Let's create a stack: point to the beginning of the stack in %esp
	; Subtract 16 to leave room for the first call, keeping the stack 16B-aligned
	mov esp, kernel_stack_top - 16

	; Store the multiboot information in a well-known place to C
	mov dword [multiboot_information], ebx

	; We are 32-bit here
	mov eax, 0xc0dec0de

	call main

	; We should never return from main(), but, just in case, we have a "black hole":
	; disable interrupts (cli), halt (hlt), and halt again if some unmaskable interrupt appears
	cli
black_hole:
	hlt
	jmp black_hole

error:
	mov dword [0xB8000], 0x4f524f45
	mov dword [0xB8004], 0x4f3a4f52
	mov dword [0xB8008], 0x4f204f20
	mov byte  [0xB800A], al
	jmp black_hole

;;;;;;;;;;;;;;;;;;;;;;;;;;;
; DATA (initialized data) ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;

section .data
	align 4096

;;;;;;;;;;;;;;;;;;
; READ-ONLY DATA ;
;;;;;;;;;;;;;;;;;;

section .rodata
	align 4

;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; BSS (uninitialized data) ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;

KERNEL_STACK_SIZE		equ 4096

section .bss
	align 4096 ; System V ABI standard: align stack at 16-byte boundary

global kernel_stack_bot
global kernel_stack_top

; Reserve the kernel stack size

kernel_stack_bot:
	resb KERNEL_STACK_SIZE  
kernel_stack_top:
