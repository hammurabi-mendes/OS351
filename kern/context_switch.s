; This task switch mechanism is similar to the idea used by eduOS,
; where we create a fake interrupt stack and restore context using iretq

bits 64

extern handle_exception_context
extern print_interrupt_state

global prepare_context
align 16
; rdi rsi rdx rcx, r8, r9
; rdi: &(task->stack)
; rsi; code
; rdx: data
prepare_context:
   	mov rcx, rsp      ; Save stack in rcx
    mov rsp, [rdi]    ; Change temporarily to the new stack

    push qword 0x10   ; ss
    push qword [rdi]  ; rsp (1st parameter, derefernced)
    push qword 0x1202 ; RFLAGS
    push qword 0x08   ; cs
    push rsi          ; rip (2nd parameter)
    push qword 0x00   ; Error code
    push qword 0x00   ; Interrupt number

    push qword 0x00   ; rax
    push qword 0x00   ; rcx
    push qword 0x00   ; rdx
    push qword 0x00   ; rbx
    push qword [rdi]  ; rsp (1st parameter, dereferenced, ignored on context restore)
    push qword 0x00   ; rbp
    push qword 0x00   ; rsi
    push rdx          ; rdi (3rd parameter)
    push qword 0x00   ; r8
    push qword 0x00   ; r9
    push qword 0x00   ; r10
    push qword 0x00   ; r11
    push qword 0x00   ; r12
    push qword 0x00   ; r13
    push qword 0x00   ; r14
    push qword 0x00   ; r15

	mov [rdi], rsp    ; Overwrite new stack with the current stack
	mov rax, rsp      ; Return value is the current stack
	mov rsp, rcx      ; Restore original stack

	ret

global switch_context
align 16
; rdi rsi rdx rcx, r8, r9
; rdi: &(old_context->stack)
; rsi; new_context->stack
switch_context:
    mov rax, rsp                     ; Save stack in rax
    mov rsp, [rdi]                   ; Change temporarily to the new stack

    push qword 0x10                  ; ss
    push qword rax                   ; rsp (1st parameter)
    pushf                            ; RFLAGS
    push qword 0x08                  ; cs
    push qword return_switch_context ; rip
    push qword 0x00                  ; Error code
    push qword 0x00                  ; Interrupt number

    push rax
    push rcx
    push rdx
    push rbx
    push rsp
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

    ; Uncomment this to disable switch_context
    ; jmp restore_state

switch_stack:
    ; Store current rsp into the old tasks's stack
    mov [rdi], rsp

	mov rdi, rsp
	call print_interrupt_state

    ; Set the new tasks's stack as the current stack
    mov rsp, rsi

	mov rdi, rsp
	call print_interrupt_state

    ; Set the TS bit in CR0
    mov rcx, cr0
    or ecx, 8
    mov cr0, rcx

restore_state:
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

align 16
return_switch_context:
    ret
