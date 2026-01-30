; Kernel entry point in assembly
; This is called by the bootloader after switching to protected mode

[bits 32]
[extern kernel_main]    ; Declare C function

section .text
global _start
_start:
    ; Set up kernel stack
    mov esp, kernel_stack_top
    
    ; Call C kernel main function
    call kernel_main
    
    ; Halt if kernel returns
    cli
.hang:
    hlt
    jmp .hang

; Reserve space for kernel stack
section .bss
align 16
kernel_stack_bottom:
    resb 16384  ; 16 KB stack
kernel_stack_top:
