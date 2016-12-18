#ifndef MILLET_MMU_H_
#define MILLET_MMU_H_

#include <common.h>

#define create_desciptor(ttb, va, pa) \
	ttb[va >> 20] = (pa & 0xfff00000) | 0x2;

extern void init_ttb(u32 *ttb_base);
extern void section_map(u32 *ttb, u32 va_start, u32 va_end, u32 pa); 
extern void mmu_enable(u32 *ttb);
extern void memset(char *buf, char ch, u32 size);
extern void strcpy(char *p1, char *p2);
extern void memcpy(char *dst, char *src, u32 size);

#endif
