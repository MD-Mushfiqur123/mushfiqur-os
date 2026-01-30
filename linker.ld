ENTRY(_start)
OUTPUT_FORMAT(binary)

SECTIONS
{
    . = 0x1000;
    
    .text : {
        *(.text._start)
        *(.text)
        *(.text.*)
    }
    
    .rodata : {
        *(.rodata)
        *(.rodata.*)
    }
    
    .data : {
        *(.data)
        *(.data.*)
    }
    
    .bss : {
        *(COMMON)
        *(.bss)
        *(.bss.*)
    }
}
