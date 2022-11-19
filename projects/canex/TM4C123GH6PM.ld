MEMORY
{
    FLASH (rx) : ORIGIN = 0x00000000, LENGTH = 0x00040000
<<<<<<< HEAD
    SRAM (rwx) : ORIGIN = 0x20000000, LENGTH = 0X00008000
=======
    SRAM (rwx) : ORIGIN = 0x20000000, LENGTH = 0x00008000
>>>>>>> ac7f5bf2e1a5e94e3365253217efd271569c0882
}

SECTIONS
{
<<<<<<< HEAD
    /* code */
    .text :
    {
        _text = .;
        /* ensure ISR vectors are not removed by linker */
=======
    .text :
    {
        _text = .;
>>>>>>> ac7f5bf2e1a5e94e3365253217efd271569c0882
        KEEP(*(.isr_vector))
        *(.text*)
        *(.rodata*)
        _etext = .;
    } > FLASH

<<<<<<< HEAD
    /* static data */
    .data : AT(ADDR(.text) + SIZEOF(.text))
    {
        _data = .;
=======
    .data : AT(ADDR(.text) + SIZEOF(.text))
    {
        _data = .;
        _ldata = LOADADDR (.data);
>>>>>>> ac7f5bf2e1a5e94e3365253217efd271569c0882
        *(vtable)
        *(.data*)
        _edata = .;
    } > SRAM

<<<<<<< HEAD
    /* static uninitialized data */
=======
>>>>>>> ac7f5bf2e1a5e94e3365253217efd271569c0882
    .bss :
    {
        _bss = .;
        *(.bss*)
        *(COMMON)
        _ebss = .;
    } > SRAM
<<<<<<< HEAD

=======
>>>>>>> ac7f5bf2e1a5e94e3365253217efd271569c0882
}
