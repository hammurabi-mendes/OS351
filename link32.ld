OUTPUT_FORMAT("elf32-i386")
OUTPUT_ARCH("i386")
ENTRY(start)

entryaddress = 0x00100000;

SECTIONS {
    . = entryaddress;        /* The Kernel is loaded at 1MB */

    .multiboot :
    {
        *(.multiboot)
    }

    .text ALIGN (0x1000) :   /* Sections are 4KB-aligned */
    {
        *(.text) 
    }

    .rodata ALIGN (0x1000) :
    {
        *(.rodata*)
    }

    .data ALIGN (0x1000) :
    {
        *(.data) 
    }

    .bss ALIGN (0x1000) :
    {
        *(COMMON)
        *(.bss) 
    }
}
