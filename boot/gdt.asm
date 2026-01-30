; Global Descriptor Table for protected mode

gdt_start:

gdt_null:               ; Mandatory null descriptor
    dd 0x0
    dd 0x0

gdt_code:               ; Code segment descriptor
    dw 0xffff           ; Limit (bits 0-15)
    dw 0x0              ; Base (bits 0-15)
    db 0x0              ; Base (bits 16-23)
    db 10011010b        ; Flags: present, ring 0, code segment, executable, readable
    db 11001111b        ; Flags: granularity, 32-bit, limit (bits 16-19)
    db 0x0              ; Base (bits 24-31)

gdt_data:               ; Data segment descriptor
    dw 0xffff           ; Limit (bits 0-15)
    dw 0x0              ; Base (bits 0-15)
    db 0x0              ; Base (bits 16-23)
    db 10010010b        ; Flags: present, ring 0, data segment, writable
    db 11001111b        ; Flags: granularity, 32-bit, limit (bits 16-19)
    db 0x0              ; Base (bits 24-31)

gdt_end:

; GDT descriptor
gdt_descriptor:
    dw gdt_end - gdt_start - 1  ; Size
    dd gdt_start                 ; Start address

; Segment selector offsets
CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start
