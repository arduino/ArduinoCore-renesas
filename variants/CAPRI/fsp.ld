/*
                  Linker File for Renesas FSP
*/

INCLUDE memory_regions.ld

/* Define memory regions. */
MEMORY
{
  FLASH (rx)      : ORIGIN = FLASH_IMAGE_START, LENGTH = FLASH_LENGTH - FLASH_IMAGE_START
  RAM (rwx)       : ORIGIN = RAM_START, LENGTH = RAM_LENGTH
  DATA_FLASH (rx) : ORIGIN = DATA_FLASH_START, LENGTH = DATA_FLASH_LENGTH
  OPTION_SETTING (r): ORIGIN = OPTION_SETTING_START, LENGTH = OPTION_SETTING_LENGTH
  OPTION_SETTING_S (r): ORIGIN = OPTION_SETTING_S_START, LENGTH = OPTION_SETTING_S_LENGTH
  ID_CODE (rx)    : ORIGIN = ID_CODE_START, LENGTH = ID_CODE_LENGTH
}

/* Library configurations */
GROUP(libgcc.a libc.a libm.a libnosys.a)

/* Linker script to place sections and symbol values. Should be used together
 * with other linker script that defines memory regions FLASH and RAM.
 * It references following symbols, which must be DEFINED in code:
 *   Reset_Handler : Entry of reset handler
 *
 * It defines following symbols, which code can use without definition:
 *   __exidx_start
 *   __exidx_end
 *   __copy_table_start__
 *   __copy_table_end__
 *   __zero_table_start__
 *   __zero_table_end__
 *   __etext
 *   __data_start__
 *   __preinit_array_start
 *   __preinit_array_end
 *   __init_array_start
 *   __init_array_end
 *   __fini_array_start
 *   __fini_array_end
 *   __data_end__
 *   __bss_start__
 *   __bss_end__
 *   __HeapLimit
 *   __StackLimit
 *   __StackTop
 *   __stack
 *   __Vectors_End
 *   __Vectors_Size
 *   __qspi_flash_start__
 *   __qspi_flash_end__
 *   __qspi_flash_code_size__
 *   __qspi_region_max_size__
 *   __qspi_region_start_address__
 *   __qspi_region_end_address__
 *   __ospi_device_0_start__
 *   __ospi_device_0_end__
 *   __ospi_device_0_code_size__
 *   __ospi_device_0_region_max_size__
 *   __ospi_device_0_region_start_address__
 *   __ospi_device_0_region_end_address__
 *   __ospi_device_1_start__
 *   __ospi_device_1_end__
 *   __ospi_device_1_code_size__
 *   __ospi_device_1_region_max_size__
 *   __ospi_device_1_region_start_address__
 *   __ospi_device_1_region_end_address__
 */
ENTRY(Reset_Handler)

SECTIONS
{
    .text :
    {
        __tz_FLASH_S = ABSOLUTE(FLASH_START);
        __ROM_Start = .;

        /* Even though the vector table is not 256 entries (1KB) long, we still allocate that much
         * space because ROM registers are at address 0x400 and there is very little space
         * in between. */
        KEEP(*(.fixed_vectors*))
        KEEP(*(.application_vectors*))
        __Vectors_End = .;

        *(.text*)

        KEEP(*(.version))
        KEEP(*(.init))
        KEEP(*(.fini))

        /* .ctors */
        *crtbegin.o(.ctors)
        *crtbegin?.o(.ctors)
        *(EXCLUDE_FILE(*crtend?.o *crtend.o) .ctors)
        *(SORT(.ctors.*))
        *(.ctors)

        /* .dtors */
        *crtbegin.o(.dtors)
        *crtbegin?.o(.dtors)
        *(EXCLUDE_FILE(*crtend?.o *crtend.o) .dtors)
        *(SORT(.dtors.*))
        *(.dtors)

        *(.rodata*)

        KEEP(*(.eh_frame*))

        __ROM_End = .;
    } > FLASH = 0xFF

    __Vectors_Size = __Vectors_End - __Vectors;

    .ARM.extab :
    {
        *(.ARM.extab* .gnu.linkonce.armextab.*)
    } > FLASH

    __exidx_start = .;
    .ARM.exidx :
    {
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
    } > FLASH
    __exidx_end = .;

    /* To copy multiple ROM to RAM sections,
     * uncomment .copy.table section and,
     * define __STARTUP_COPY_MULTIPLE in startup_ARMCMx.S */
    /*
    .copy.table :
    {
        . = ALIGN(4);
        __copy_table_start__ = .;
        LONG (__etext)
        LONG (__data_start__)
        LONG (__data_end__ - __data_start__)
        LONG (__etext2)
        LONG (__data2_start__)
        LONG (__data2_end__ - __data2_start__)
        __copy_table_end__ = .;
    } > FLASH
    */

    /* To clear multiple BSS sections,
     * uncomment .zero.table section and,
     * define __STARTUP_CLEAR_BSS_MULTIPLE in startup_ARMCMx.S */
    /*
    .zero.table :
    {
        . = ALIGN(4);
        __zero_table_start__ = .;
        LONG (__bss_start__)
        LONG (__bss_end__ - __bss_start__)
        LONG (__bss2_start__)
        LONG (__bss2_end__ - __bss2_start__)
        __zero_table_end__ = .;
    } > FLASH
    */

    __etext = .;

    __tz_RAM_S = ORIGIN(RAM);

    /* If DTC is used, put the DTC vector table at the start of SRAM.
       This avoids memory holes due to 1K alignment required by it. */
    .fsp_dtc_vector_table (NOLOAD) :
    {
        . = ORIGIN(RAM);
        *(.fsp_dtc_vector_table)
    } > RAM

    /* Initialized data section. */
    .data :
    {
        __data_start__ = .;
        . = ALIGN(4);

        __Code_In_RAM_Start = .;

        KEEP(*(.code_in_ram*))
        __Code_In_RAM_End = .;

        *(vtable)
        /* Don't use *(.data*) because it will place data meant for .data_flash in this section. */
        *(.data.*)
        *(.data)

        . = ALIGN(4);
        /* preinit data */
        PROVIDE_HIDDEN (__preinit_array_start = .);
        KEEP(*(.preinit_array))
        PROVIDE_HIDDEN (__preinit_array_end = .);

        . = ALIGN(4);
        /* init data */
        PROVIDE_HIDDEN (__init_array_start = .);
        KEEP(*(SORT(.init_array.*)))
        KEEP(*(.init_array))
        PROVIDE_HIDDEN (__init_array_end = .);


        . = ALIGN(4);
        /* finit data */
        PROVIDE_HIDDEN (__fini_array_start = .);
        KEEP(*(SORT(.fini_array.*)))
        KEEP(*(.fini_array))
        PROVIDE_HIDDEN (__fini_array_end = .);

        KEEP(*(.jcr*))

        . = ALIGN(4);

        /* All data end */
        __data_end__ = .;

    } > RAM AT > FLASH

    .noinit (NOLOAD):
    {
        . = ALIGN(4);
        __noinit_start = .;
        KEEP(*(.noinit*))
        . = ALIGN(8);
        /* Place the FreeRTOS heap here so that the __HeapLimit calculation does not include the freertos heap. */
        KEEP(*(.heap.*))
        __noinit_end = .;
    } > RAM

    .bss :
    {
        . = ALIGN(4);
        __bss_start__ = .;
        *(.bss*)
        *(COMMON)
        . = ALIGN(4);
        __bss_end__ = .;
    } > RAM

    .heap (NOLOAD):
    {
        . = ALIGN(8);
        __HeapBase = .;
        end = .;
        KEEP(*(.heap))
    } > RAM

	__StackTop = ORIGIN(RAM) + LENGTH(RAM) - VECTOR_TABLE_LENGTH;

	/* .stack_dummy section doesn't contains any symbols. It is only
	 * used for linker to calculate size of stack sections, and assign
	 * values to stack symbols later */
	.stack_dummy ABSOLUTE(__StackTop - BSP_CFG_STACK_MAIN_BYTES) (NOLOAD):
	{
        . = ALIGN(8);
        __HeapLimit = .;
        __StackLimit = .;
        KEEP(*(.stack))
		KEEP(*(.stack*))
        __StackTopAll = .;
	} > RAM

    .vector_table ABSOLUTE(__StackTop) (NOLOAD):
    {
        . = ALIGN(4);
        APPLICATION_VECTOR_TABLE_ADDRESS_RAM = .;
        . = ABSOLUTE(RAM_START + RAM_LENGTH);
    } > RAM

	PROVIDE(__stack = __StackTop);

    .ns_buffer (NOLOAD):
    {
        KEEP(*(.ns_buffer*))
    } > RAM

    /* Data flash. */
    .data_flash :
    {
        . = ORIGIN(DATA_FLASH);
        __tz_DATA_FLASH_S = .;
        __Data_Flash_Start = .;
        KEEP(*(.data_flash*))
        __Data_Flash_End = .;

        __tz_DATA_FLASH_N = ALIGN(1024);
    } > DATA_FLASH

    /* Note: There are no secure/non-secure boundaries for ID_CODE.  These symbols are provided for the RA configuration tool. */
    __tz_ID_CODE_S = ORIGIN(ID_CODE);

    .id_code :
    {
        __ID_Code_Start = .;
        KEEP(*(.id_code*))
        __ID_Code_End = .;
    } > ID_CODE

    /* Note: There are no secure/non-secure boundaries for ID_CODE.  These symbols are provided for the RA configuration tool. */
    __tz_ID_CODE_N = __ID_Code_End;

    /* Symbol required for RA Configuration tool. */
    __tz_OPTION_SETTING_S = ORIGIN(OPTION_SETTING);

    .option_setting :
    {
        __OPTION_SETTING_Start = .;
        KEEP(*(.option_setting_ofs0))
        . = __OPTION_SETTING_Start;
        KEEP(*(.option_setting_dualsel))
        . = __OPTION_SETTING_Start;
        KEEP(*(.option_setting_sas))
        __OPTION_SETTING_End = .;
    } > OPTION_SETTING = 0xFF

    .option_setting_ns :
    {
        __OPTION_SETTING_NS_Start = .;
        KEEP(*(.option_setting_ofs1))
        . = __OPTION_SETTING_NS_Start;
        KEEP(*(.option_setting_banksel))
        . = __OPTION_SETTING_NS_Start;
        KEEP(*(.option_setting_bps0))
        . = __OPTION_SETTING_NS_Start;
        KEEP(*(.option_setting_bps1))
        . = __OPTION_SETTING_NS_Start;
        KEEP(*(.option_setting_bps2))
        . = __OPTION_SETTING_NS_Start;
        KEEP(*(.option_setting_pbps0))
        . = __OPTION_SETTING_NS_Start;
        KEEP(*(.option_setting_pbps1))
        . = __OPTION_SETTING_NS_Start;
        KEEP(*(.option_setting_pbps2))
        __OPTION_SETTING_NS_End = .;
    } > OPTION_SETTING = 0xFF

    /* Symbol required for RA Configuration tool. */
    __tz_OPTION_SETTING_S_S = ORIGIN(OPTION_SETTING_S);

    .option_setting_s :
    {
        __OPTION_SETTING_S_Start = .;
        KEEP(*(.option_setting_ofs1_sec))
        . = __OPTION_SETTING_S_Start;
        KEEP(*(.option_setting_banksel_sec))
        . = __OPTION_SETTING_S_Start;
        KEEP(*(.option_setting_bps_sec0))
        . = __OPTION_SETTING_S_Start;
        KEEP(*(.option_setting_bps_sec1))
        . = __OPTION_SETTING_S_Start;
        KEEP(*(.option_setting_bps_sec2))
        . = __OPTION_SETTING_S_Start;
        KEEP(*(.option_setting_pbps_sec0))
        . = __OPTION_SETTING_S_Start;
        KEEP(*(.option_setting_pbps_sec1))
        . = __OPTION_SETTING_S_Start;
        KEEP(*(.option_setting_pbps_sec2))
        . = __OPTION_SETTING_S_Start;
        KEEP(*(.option_setting_ofs1_sel))
        . = __OPTION_SETTING_S_Start;
        KEEP(*(.option_setting_banksel_sel))
        . = __OPTION_SETTING_S_Start;
        KEEP(*(.option_setting_bps_sel0))
        . = __OPTION_SETTING_S_Start;
        KEEP(*(.option_setting_bps_sel1))
        . = __OPTION_SETTING_S_Start;
        KEEP(*(.option_setting_bps_sel2))
        __OPTION_SETTING_S_End = .;
    } > OPTION_SETTING_S = 0xFF

    /* Symbol required for RA Configuration tool. */
    __tz_OPTION_SETTING_S_N = __OPTION_SETTING_S_End;
}
