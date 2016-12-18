#include <vectors.h>
#include <mma7660.h>

int main(void)
{
	build_vectors();
	i2c_mma_eint25_init();	

	printf("start shake mma7660 ba!\n");

	return 0;
}
