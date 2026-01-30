; Switch from 16-bit real mode to 32-bit protected mode

[bits 16]
switch_to_pm:
    cli                     ; Disable interrupts
    lgdt [gdt_descriptor]   ; Load GDT
    
    mov eax, cr0
    or eax, 0x1             ; Set PE (Protection Enable) bit
    mov cr0, eax
    
    jmp CODE_SEG:init_pm    ; Far jump to flush pipeline

[bits 32]
init_pm:
    mov ax, DATA_SEG        ; Update segment registers
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    
    mov ebp, 0x90000        ; Update stack position
    mov esp, ebp
    
    call BEGIN_PM           ; Call 32-bit main

; Protected mode print function
; EBX = address of null-terminated string
print_string_pm:
    pusha
    mov edx, VIDEO_MEMORY

.loop:
    mov al, [ebx]
    mov ah, WHITE_ON_BLACK
    
    cmp al, 0
    je .done
    
    mov [edx], ax
    add ebx, 1
    add edx, 2
    
    jmp .loop

.done:
    popa
    ret

; Constants
VIDEO_MEMORY equ 0xb8000
WHITE_ON_BLACK equ 0x0f
