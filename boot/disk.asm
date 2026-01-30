; Disk loading routines

[bits 16]

; Load DH sectors to ES:BX from drive DL
disk_load:
    pusha               ; Save all registers
    push dx
    
    ; Reset disk system first
    mov ah, 0x00
    int 0x13
    jc disk_error
    
    pop dx
    push dx
    
    ; Set up segment registers
    mov ah, 0x02        ; BIOS read sector function
    mov al, dh          ; Number of sectors to read
    mov ch, 0x00        ; Cylinder 0
    mov cl, 0x02        ; Start from sector 2 (sector 1 is boot sector)
    mov dh, 0x00        ; Head 0
    ; DL already contains drive number
    
    int 0x13            ; BIOS interrupt
    
    jc disk_error       ; Jump if error (carry flag set)
    
    pop dx
    cmp al, dh          ; AL = sectors actually read
    jne disk_error
    
    popa                ; Restore all registers
    ret

disk_error:
    mov bx, DISK_ERROR_MSG
    call print_string
    mov bx, 0x0007      ; Print error code
    call print_hex
    jmp $

DISK_ERROR_MSG: db "Disk read error! Code: ", 0
