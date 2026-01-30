; Real mode print functions

[bits 16]

; Print string in real mode
; BX = address of null-terminated string
print_string:
    pusha
    mov ah, 0x0e        ; BIOS teletype mode
.loop:
    mov al, [bx]
    cmp al, 0
    je .done
    int 0x10            ; BIOS interrupt
    add bx, 1
    jmp .loop
.done:
    ; Print newline
    mov al, 0x0d
    int 0x10
    mov al, 0x0a
    int 0x10
    popa
    ret

; Print hex value
; DX = value to print
print_hex:
    pusha
    mov cx, 4           ; 4 hex digits
.loop:
    dec cx
    mov ax, dx
    shr dx, 4
    and ax, 0xf
    add al, 0x30
    cmp al, 0x39
    jle .print
    add al, 7
.print:
    mov bx, HEX_OUT + 2
    add bx, cx
    mov [bx], al
    cmp cx, 0
    jne .loop
    
    mov bx, HEX_OUT
    call print_string
    popa
    ret

HEX_OUT: db '0x0000', 0
