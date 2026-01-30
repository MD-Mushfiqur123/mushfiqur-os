; Mushfiqur OS Bootloader
; 512-byte boot sector that loads the kernel into memory

[org 0x7c00]        ; BIOS loads boot sector at 0x7c00
[bits 16]           ; Start in 16-bit real mode

KERNEL_OFFSET equ 0x1000  ; Memory offset where kernel will be loaded

    ; Immediate visual test - print 'M' directly
    mov ah, 0x0e
    mov al, 'M'
    int 0x10
    
    mov [BOOT_DRIVE], dl    ; BIOS stores boot drive in DL
    
    ; Set up stack
    mov bp, 0x9000
    mov sp, bp
    
    ; Print boot message
    mov bx, MSG_REAL_MODE
    call print_string
    
    ; Load kernel from disk
    call load_kernel
    
    ; Switch to protected mode
    call switch_to_pm
    
    jmp $  ; Should never reach here

%include "boot/print.asm"
%include "boot/disk.asm"
%include "boot/gdt.asm"
%include "boot/switch_pm.asm"

[bits 16]
load_kernel:
    mov bx, MSG_LOAD_KERNEL
    call print_string
    
    ; Set up segment registers for loading
    xor ax, ax
    mov es, ax          ; ES = 0
    mov ds, ax          ; DS = 0
    
    mov bx, KERNEL_OFFSET   ; Read from disk and store in 0x1000
    mov dh, 20              ; Read 20 sectors (10KB kernel)
    mov dl, [BOOT_DRIVE]
    call disk_load
    
    mov bx, MSG_KERNEL_LOADED
    call print_string
    
    ret

[bits 32]
BEGIN_PM:
    mov ebx, MSG_PROT_MODE
    call print_string_pm
    
    call KERNEL_OFFSET      ; Jump to kernel
    
    jmp $

; Data
BOOT_DRIVE      db 0
MSG_REAL_MODE   db "Mushfiqur OS - Booting in 16-bit Real Mode...", 0
MSG_LOAD_KERNEL db "Loading kernel into memory...", 0
MSG_KERNEL_LOADED db "Kernel loaded successfully!", 0
MSG_PROT_MODE   db "Successfully switched to 32-bit Protected Mode", 0

; Boot sector padding
times 510-($-$$) db 0
dw 0xaa55  ; Boot signature
