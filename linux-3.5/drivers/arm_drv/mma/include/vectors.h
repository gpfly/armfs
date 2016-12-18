#ifndef MILLET_VECTORS_H_
#define MILLET_VECTORS_H_

#include <common.h>

#define HIGH_VECTOR_VA_BASE	0xffff0000
#define HIGH_VECTOR_PA_BASE 0x70000000
#define TTB_BASE			0x73000000

extern u32 vectors_start, vectors_end;
extern void build_vectors(void);

#endif
