PHDRS
{
  headers PT_PHDR PHDRS ;
  //interp PT_INTERP ;
  text PT_LOAD FILEHDR PHDRS ;
  data PT_LOAD ;
  //dynamic PT_DYNAMIC ;
}

SECTIONS {
    . = SIZEOF_HEADERS;
    
    .header : { *(.header) } :headers
    . = ALIGN(16);

    .text : {
        *(.text .text.*)
    } :text
    
    .rodata : {
        *(.rodata .rodata.*)
    } :text
    
    .preinit_array : {
        PROVIDE_HIDDEN(__preinit_array_start = .);
        KEEP (*(SORT(.preinit_array.*)))
        KEEP (*(.preinit_array*))
        PROVIDE_HIDDEN (__preinit_array_end = .);
    } :text
    
    .init_array : {
        PROVIDE_HIDDEN (__init_array_start = .);
        KEEP (*(SORT(.init_array.*)))
        KEEP (*(.init_array*))
        PROVIDE_HIDDEN (__init_array_end = .);
    } :text
    
    .fini_array : {
        PROVIDE_HIDDEN (__fini_array_start = .);
        KEEP (*(SORT(.fini_array.*)))
        KEEP (*(.fini_array*))
        PROVIDE_HIDDEN (__fini_array_end = .);
    } :text
    
    .tdata : {
        *(.tdata)
    } :data
    .tbss : {
        *(.tbss)
    } :data
    
    .data : {
        *(.data .data.*)
    } :data
    
    .bss : {
        *(.bss .bss.*)
    } :data
    
    PROVIDE(ebss = .);
    
    /DISCARD/ : {
        *(.eh_frame .note.GNU-stack .comment)
    }
}