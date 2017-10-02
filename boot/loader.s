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

	; Check if we have support for the CPUID command
	call check_cpuid
	; Check if we have support for 64-bit long mode
	call check_long_mode
	; Setup page tables: identity-maps the first 2GB of real memory using a P3 huge table
	call init_page_tables
	; Enable paging
	call enable_paging

	; Loads a minimal 64-bit GDT, and jumps to it
	lgdt [GDT64.pointer]
	jmp GDT64.code:start64

	; If we are here, something above did not work
 	mov al, "0"
	jmp error

; 64-bit setup comes from OSDev (Aug 2017):
; http://wiki.osdev.org/Setting_Up_Long_Mode

; Check if CPUID is supported by attempting to flip the ID bit (bit 21)
; in the FLAGS register. Halts if false.
check_cpuid:
	; Copy FLAGS in to EAX via stack
	pushfd
	pop eax

	; Copy to ECX as well for comparing later on
	mov ecx, eax

	; Flip the ID bit
	xor eax, (1 << 21)

	; Copy EAX to FLAGS via the stack
	push eax
	popfd

	; Copy FLAGS back to EAX (with the flipped bit if CPUID is supported)
	pushfd
	pop eax

	; Restore FLAGS from the old version stored in ECX (i.e. flipping the
	; ID bit back if it was ever flipped).
	push ecx
	popfd

	; Compare EAX and ECX. If they are equal then that means the bit
	; wasn't flipped, and CPUID isn't supported.
	cmp eax, ecx
	je .cpuid_failed

	ret

.cpuid_failed:
 	mov al, "1"
	jmp error

check_long_mode:
	; Get highest extended function supported
	mov eax, 0x80000000
	cpuid
	; The largest calling parameter is returned in EAX. We need 0x80000001 (see below)
	cmp eax, 0x80000001
	jb .long_mode_failed

	; Get extended processor nformation
	mov eax, 0x80000001
	cpuid
	; Bit 29 means long mode is available
	test edx, 1 << 29
	jz .long_mode_failed
	ret

.long_mode_failed:
	mov al, "2"
	jmp error

init_page_tables:
	; P4's first entry is p3_table
	mov eax, p3_table
	; Set present and writable bits
	or eax, 0b11
	mov [p4_table], eax

	; P3's first entry is a huge (1GB) page starting at 2MB
	mov eax, 0x0

	; Set present, writable, and huge-page bits
	or eax, 0b10000011
	mov [p3_table], eax

	ret

enable_paging:
	; Load P4 to CR3 register
	mov eax, p4_table
	mov cr3, eax

	; Enable PAE bit in CR4
	mov eax, cr4
	or eax, (1 << 5)
	mov cr4, eax

	; Set LME (Long Mode Enable) bit in the EFER model-specific register

	; Loads the ECX-named register into EAX
	mov ecx, 0xC0000080
	rdmsr
	; Writes EAX into the ECX-named register
	or eax, (1 << 8)
	wrmsr

	; Enable protected mode bit in CR0
	mov eax, cr0
	or eax, (1 << 31)
	mov cr0, eax

	ret

;;;;;;;;;;;;;;;;;;;;;;
; 64-bit TEXT (code) ;
;;;;;;;;;;;;;;;;;;;;;;

bits 64
start64:
	; Clears up remains from the old GDT (loaded by multiboot) in data segment registers
	mov ax, GDT64.data
	mov ds, ax
	mov es, ax
	mov ss, ax
	mov ax, 0x00
	mov fs, ax
	mov gs, ax

	; We are 64-bit here
	mov rax, 0xc0dec0dec0dec0de

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

; For all but the last page table,
; the first entry has the present (first bit) and writable (second bit) are set
; the last entry has the self-refernce (10th bit), present (first bit) and writable (second bit) are set
p4_table:
  dq (p3_table + 0b11)
  times 510 dq 0
  dq p4_table + 0b1000000011

p3_table:
  dq (p2_table + 0b11)
  times 510 dq 0
  dq p4_table + 0b1000000011
p2_table:
  dq (p1_table + 0b11)
  times 510 dq 0
  dq p4_table + 0b1000000011
p1_table:
  times 512 dq 0

;;;;;;;;;;;;;;;;;;
; READ-ONLY DATA ;
;;;;;;;;;;;;;;;;;;

section .rodata
	align 4

; 64-bit GDT comes from OSDev (Aug 2017):
; http://wiki.osdev.org/Setting_Up_Long_Mode

; In 64-bit, we only need a code segment entry
GDT64:                           ; Global Descriptor Table (64-bit).
    .null: equ $ - GDT64         ; The null descriptor.
    dw 0                         ; Limit (low).
    dw 0                         ; Base (low).
    db 0                         ; Base (middle)
    db 0                         ; Access.
    db 0                         ; Granularity.
    db 0                         ; Base (high).
    .code: equ $ - GDT64         ; The code descriptor.
    dw 0                         ; Limit (low).
    dw 0                         ; Base (low).
    db 0                         ; Base (middle)
    db 10011010b                 ; Access (exec/read).
    db 00100000b                 ; Granularity.
    db 0                         ; Base (high).
    .data: equ $ - GDT64         ; The data descriptor.
    dw 0                         ; Limit (low).
    dw 0                         ; Base (low).
    db 0                         ; Base (middle)
    db 10010010b                 ; Access (read/write).
    db 00000000b                 ; Granularity.
    db 0                         ; Base (high).
    .pointer:                    ; The GDT-pointer.
    dw $ - GDT64 - 1             ; Limit.
    dq GDT64

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
