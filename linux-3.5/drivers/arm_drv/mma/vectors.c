#include <mmu.h>
#include <common.h>
#include <vectors.h>

void build_vectors(void)
{
	u32 *tt_base = (void *)TTB_BASE;

	init_ttb(tt_base);	

	//异常向量表高矢量虚拟地址映射到物理地址0x70000000处
	create_desciptor(tt_base, HIGH_VECTOR_VA_BASE, HIGH_VECTOR_PA_BASE);
	
	mmu_enable(tt_base);

	memcpy((char *)HIGH_VECTOR_VA_BASE, (char *)vectors_start, vectors_end - vectors_start);
}
