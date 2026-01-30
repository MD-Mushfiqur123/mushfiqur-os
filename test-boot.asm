; Minimal test bootloader - just prints a message
[org 0x7c00]
[bits 16]

start:
    ; Set up segments
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7c00
    
    ; Clear screen
    mov ah, 0x00
    mov al, 0x03
    int 0x10
    
    ; Print test message
    mov si, msg
    call print_string
    
    ; Hang
    jmp $

print_string:
    mov ah, 0x0e
.loop:
    lodsb
    cmp al, 0
    je .done
    int 0x10
    jmp .loop
.done:
    ret

msg db 'BOOTLOADER TEST - If you see this, bootloader works!', 13, 10, 0

; Pad to 510 bytes
times 510-($-$$) db 0
dw 0xaa55
