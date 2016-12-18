
#include <common.h>
#include <irq.h>

static isr_t isres[160];
static int flag = 0;

void do_irq(void)
{
	u32 cpuid = ICCIAR_CPU0;
	isr_t irq_deal = NULL;	

	ICCEOIR_CPU0 = cpuid; //clear irq

	irq_deal = isres[cpuid&0x3ff];

	irq_deal();
}

void irq_request_cpu0_(int irqnum, void (*handler)(void), int pri)
{
	if(!flag){
		ICCICR_CPU0 = 0x1;
		ICCPMR_CPU0 = 0xff;
		ICDDCR = 0x1;

		flag = 1;
	}

	if(irqnum >= 0 && irqnum <= 159){
		

		ICDIPR0_CPU0[irqnum/4] &= ~(0xff << (irqnum%4)*8); 
		ICDIPR0_CPU0[irqnum/4] |= pri << (irqnum%4)*8;

		ICDIPTR0_CPU0[irqnum/4] &= ~(0xff << (irqnum%4)*8);
		ICDIPTR0_CPU0[irqnum/4] |= 0x1 << (irqnum%4)*8;

		ICDISER0_CPU0[irqnum/32] |= 0x1 << (irqnum%32);

		isres[irqnum] = handler;

		__asm__ __volatile__(
			"cpsie i\n"
		);
	}else{
		printf("resgist irq error ...\n");
	}
}
