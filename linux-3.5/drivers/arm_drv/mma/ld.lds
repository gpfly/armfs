OUTPUT_FORMAT("elf32-littlearm", "elf32-bigarm","elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(millet9527)
SECTIONS
{
	. = 0x50008000;

	.text : {
		start.o(.text);
		*(.text);
	}
	. = ALIGN(4);

	.data : 
	{
		*(.data)
	}
	. = ALIGN(4); 

	_bss_start = .;
	.bss : 
	{
		*(.bss)
	}
	. = ALIGN(4);
	_bss_end = .;
}
