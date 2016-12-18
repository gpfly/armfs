#include <common.h>
#include <mmu.h>

void mmu_enable(u32 *ttb)
{	
	unsigned long c1_flags;

	init_ttb(ttb);

	c1_flags = 1 | (1 << 3) | ( 1 << 11) | ( 1 << 13) |  (1 << 28);

	__asm__ __volatile__ (
		"mvn r0, #0 \n"			
		"mcr p15, 0, r0, c3, c0, 0\n"

		"mcr p15, 0, %1, c2, c0, 0\n" //configure ttb

		"mrc p15, 0, r0, c1, c0, 0\n"
		"orr %0, r0, %0\n"
		"mcr p15, 0, %0, c1, c0, 0\n" //enable mmu
		:
		: "r" (c1_flags), "r" (ttb)
		: "r0"
	);
}

void section_map(u32 *ttb, u32 vir_start, u32 vir_end, u32 phy) 
{
    while(vir_start < vir_end)
    { 
        create_desciptor(ttb, vir_start, phy);
        vir_start += 0x100000;
        phy       += 0x100000;
    }   
}


void init_ttb(u32 *ttb_base)
{
	section_map(ttb_base, 0x00000000, 0x10000000, 0x00000000);
	
	section_map(ttb_base, 0x10000000, 0x14000000, 0x10000000);

	section_map(ttb_base, 0x40000000, 0x80000000, 0x40000000);
}

void memset(char *buf, char ch, u32 size)
{
	int i;
	for (i = 0; i < size; i ++)
		buf[i] = ch;
}

void memcpy(char *dst, char *src, u32 size)
{
	int i;
	for (i = 0; i < size; i ++) 
		dst[i] = src[i];	
}
