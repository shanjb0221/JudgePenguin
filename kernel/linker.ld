SECTIONS {
    . = 0;
    
    .header : { *(.header) }
    . = ALIGN(16);

    .text : {
        *(.text .text.*)
    }
    
    .rodata : {
        *(.rodata .rodata.*)
    }
    
    .data : {
        *(.data .data.*)
    }

    .bss : {
        *(.bss .bss.*)
    }

    PROVIDE(ebss = .);

    /DISCARD/ : {
        *(.eh_frame .note.GNU-stack .comment)
    }
}