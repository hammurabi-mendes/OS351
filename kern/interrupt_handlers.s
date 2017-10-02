;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Interrupt service request handlers ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

bits 64

extern handle_exception_context
extern handle_irq_context

%macro ISR_NOERRCODE 1
	global exception%1
	exception%1:
	push byte 0  ; Pushes dummy error
	push byte %1 ; Pushes interrupt number
	jmp handle_exception
%endmacro

%macro ISR_ERRCODE 1
	global exception%1
	exception%1:
	push byte %1 ; The hardware pushed the error, we just push the interrupt number
	jmp handle_exception
%endmacro

; Define the handlers for the first 32 interrupts
ISR_NOERRCODE 0
ISR_NOERRCODE 1
ISR_NOERRCODE 2
ISR_NOERRCODE 3
ISR_NOERRCODE 4
ISR_NOERRCODE 5
ISR_NOERRCODE 6
ISR_NOERRCODE 7
ISR_ERRCODE 8
ISR_NOERRCODE 9
ISR_ERRCODE 10
ISR_ERRCODE 11
ISR_ERRCODE 12
ISR_ERRCODE 13
ISR_ERRCODE 14
ISR_NOERRCODE 15
ISR_NOERRCODE 16
ISR_NOERRCODE 17
ISR_NOERRCODE 18
ISR_NOERRCODE 19
ISR_NOERRCODE 20
ISR_NOERRCODE 21
ISR_NOERRCODE 22
ISR_NOERRCODE 23
ISR_NOERRCODE 24
ISR_NOERRCODE 25
ISR_NOERRCODE 26
ISR_NOERRCODE 27
ISR_NOERRCODE 28
ISR_NOERRCODE 29
ISR_NOERRCODE 30
ISR_NOERRCODE 31

%macro ISR_IRQ 2
	global irq%1
	irq%1:
	push byte 0   ; Pushes dummy error
	push byte %2  ; Pushes interrupt number (which is a remapped IRQ number)
	jmp handle_irq
%endmacro

ISR_IRQ 0, 32
ISR_IRQ 1, 33
ISR_IRQ 2, 34
ISR_IRQ 3, 35
ISR_IRQ 4, 36
ISR_IRQ 5, 37
ISR_IRQ 6, 38
ISR_IRQ 7, 39
ISR_IRQ 8, 40
ISR_IRQ 9, 41
ISR_IRQ 10, 42
ISR_IRQ 11, 43
ISR_IRQ 12, 44
ISR_IRQ 13, 45
ISR_IRQ 14, 46
ISR_IRQ 15, 47

handle_exception:
    push rax
    push rcx
    push rdx
    push rbx
    push rsp   ; This will be ignored, because the interrupt sets its own stack
    push rbp
    push rsi
    push rdi
    push r8
    push r9
    push r10
    push r11
    push r12
    push r13
    push r14
    push r15

    mov rdi, rsp
    call handle_exception_context

    pop r15
    pop r14
    pop r13
    pop r12
    pop r11
    pop r10
    pop r9
    pop r8
    pop rdi
    pop rsi
    pop rbp
    add rsp, 8  ; Pop value without changing rsp
    pop rbx
    pop rdx
    pop rcx
    pop rax

    add rsp, 16 ; Clear interrupt number and error code
    iretq

handle_irq:
    push rax
    push rcx
    push rdx
    push rbx
    push rsp   ; This will be ignored, because the interrupt sets its own stack
    push rbp
    push rsi
    push rdi
    push r8
    push r9
    push r10
    push r11
    push r12
    push r13
    push r14
    push r15

    mov rdi, rsp
    call handle_irq_context

    pop r15
    pop r14
    pop r13
    pop r12
    pop r11
    pop r10
    pop r9
    pop r8
    pop rdi
    pop rsi
    pop rbp
    add rsp, 8  ; Pop value without changing rsp
    pop rbx
    pop rdx
    pop rcx
    pop rax

    add rsp, 16 ; Clear interrupt number and error code
    iretq

global load_idt

load_idt:
	lidt [rdi]
	ret

global disable_interrupts

disable_interrupts:
	cli
	ret

global enable_interrupts

enable_interrupts:
	sti
	ret
