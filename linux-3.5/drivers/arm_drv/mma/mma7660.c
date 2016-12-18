
#define DEV_ADDR  0x4c

#include <iic.h>
#include <common.h>
#include <irq.h>

int wait_until_iic_irq(I2C_REG_T *i2c);
void resume_iic_scl(I2C_REG_T *i2c);
void delay(int val);
int iic_write(unsigned char reg_addr, unsigned char data);
int __iic_write(unsigned char reg_addr, unsigned char data);
static void do_eint25(void);

//1st:	first step
static inline void i2c3_init(void)
{
	// mma7660 : i2csda3/i2cscl3(GPA1_2/ GPA1_3)  eint25(GPX3_1)
	GPA1CON &= ~(0xff<<8);
	GPA1CON |= 0x33<<8;

	IIC3_reg->I2CCONn = (1<<7)|(1<<6)|(1<<5); //enable ack, 100MHz/512, enable T/R irq, 196KHz
	IIC3_reg->I2CSTATn = 1<<4; //Enable Tx/Rx
}

//2end:  second step
static inline void mma_init(void)
{
	///mma7660_init
	
	//在设置mma7660内部的寄存器之前应首先将0x07寄存器设置为备用模式
	iic_write(0x07, 0); //MODE register: Standby Mode;Auto-Wake is disabled;Auto-Sleep is disabled; 	
	delay(10000);
	
	//SR register:64 Samples/Second Active and Auto-Sleep Mode;16 Samples/Second Auto-Wake Mode;3 measurement samples
	iic_write(0x08, ((7<<5) | (1<<3) | 1));  

	iic_write(0x05, 0xa0); //SPCNT register  internal sleep counter = 0xa0 = 160
	
	//PDET register: XYZ-axis is enabled for tap detection;Tap detection threshold is +-4 counts
	iic_write(0x09, 0x4); 

	//Tap detection debounce filtering requires 15 adjacent tap detection
	iic_write(0x0a, 15); //PD register

	//Front/Back position change causes an interrupt
	//Up/Down/Right/Left position change causes an interrupt
	//Successful tap detection causes an interrupt
	//Shake detected on the XYZ-axis causes an interrupt, and sets the Shake bit in the TILT register
	iic_write(0x06, 0xe7); //INTSU register 0xe7 = 11100111

	//set the device into Active mode
	//Auto-Wake is enabled
	//Auto-Sleep is enabled
	//Interrupt output INT is push-pull
	//Interrupt output INT is active low
	iic_write(0x07, 0x59); //MODE register: 0101 1001
	delay(10000);
}

static inline void eint25_init(void)
{
	GPX3CON |= 0xf<<4;

	EXT_INT43CON &= ~(7 << 4); //low level
	EXT_INT43CON |= 2<<4; //falling edge

	EXT_INT43_FLTCON0 |= 0xff<<8;

	EXT_INT43_MASK &= ~(1<<1);

	irq_request_cpu0_(64, do_eint25, 0);
}

void i2c_mma_eint25_init(void)
{
	i2c3_init();
	mma_init();
	eint25_init();	
}

void resume_iic_scl(I2C_REG_T *i2c)
{
	i2c->I2CCONn &= ~(1<<4);
}

int wait_until_iic_irq(I2C_REG_T *i2c)
{
	int i;

	#define MAX_WAIT  0x3fff

	for (i = 0; i < MAX_WAIT; i++)
	{	
		if (i2c->I2CCONn & (1<<4))	
			return 0;
	}
	return -1;
}

/*
 *	@reg_addr: 要写值的寄存器的地址
 *	@data    : 要写入的值
 * */
int iic_write(unsigned char reg_addr, unsigned char data)
{
	int ret;

	ret = __iic_write(reg_addr, data);
	if (ret < 0)
		return __iic_write(reg_addr, data);

	return 0;
}

int __iic_write(unsigned char reg_addr, unsigned char data)
{
	int ret = -1;

	IIC3_reg->I2CDSn = (DEV_ADDR<<1)|0; //master tx
	IIC3_reg->I2CSTATn = 0xf0;
	//resume_iic_scl(IIC3_reg);

	if (wait_until_iic_irq(IIC3_reg) < 0)
		goto END;
	if (IIC3_reg->I2CSTATn & 1)
		goto END;

	IIC3_reg->I2CDSn = reg_addr;
	resume_iic_scl(IIC3_reg);
	if (wait_until_iic_irq(IIC3_reg) < 0)
		goto END;
	if (IIC3_reg->I2CSTATn & 1)
		goto END;

	IIC3_reg->I2CDSn = data;
	resume_iic_scl(IIC3_reg);
	if (wait_until_iic_irq(IIC3_reg) < 0)
		goto END;
	if (IIC3_reg->I2CSTATn & 1)
		goto END;

	ret = 0;
END:   // stop signal
	IIC3_reg->I2CSTATn = 0xd0;
	resume_iic_scl(IIC3_reg);
	delay(1000);

	return ret;
}

unsigned char read_data(unsigned char reg_addr)
{
	unsigned char ret = -1;
	
	IIC3_reg->I2CDSn = (DEV_ADDR<<1)|0; //master tx
	IIC3_reg->I2CSTATn = 0xf0;
	resume_iic_scl(IIC3_reg);
	if (wait_until_iic_irq(IIC3_reg) < 0)
		goto END;
	if (IIC3_reg->I2CSTATn & 1)
		goto END;

	IIC3_reg->I2CDSn = reg_addr;
	resume_iic_scl(IIC3_reg);
	if (wait_until_iic_irq(IIC3_reg) < 0)
		goto END;
	if (IIC3_reg->I2CSTATn & 1)
		goto END;
	
	IIC3_reg->I2CDSn = (DEV_ADDR<<1)|1; //master rx
	IIC3_reg->I2CSTATn = 0xb0;
	resume_iic_scl(IIC3_reg);
	if (wait_until_iic_irq(IIC3_reg) < 0)
		goto END;
	if (IIC3_reg->I2CSTATn & 1)
		goto END;

	resume_iic_scl(IIC3_reg);
	if (wait_until_iic_irq(IIC3_reg) < 0)
		goto END;
	
	ret = IIC3_reg->I2CDSn;

END:
	IIC3_reg->I2CSTATn = 0x90; // stop signal
	resume_iic_scl(IIC3_reg);
	delay(1000);
	
	return ret;
}

static void do_eint25(void)
{
	unsigned int ret;

	EXT_INT43_PEND = EXT_INT43_PEND;
	
	ret = read_data(0x03);
	if (ret < 0)
	{
		ret = read_data(0x03);
		if (ret < 0)
			return;
	}

	if ((ret & 3) == 1) {//Bafro
		printf("front ");
	}else if((ret & 3) == 2){
		printf("back  ");
	}
	
	switch ((ret>>2)&7)
	{
		case 1:
			printf("left  ");
			break;
		case 2:
			printf("right ");
			break;
		case 5:
			printf("down  ");
			break;
		case 6:
			printf("up    ");
			break;
		default:
			break;
	}
#if 0
	////////
	while (1)
	{
		ret = read_data(0x0);
		if (!(ret & (1<<6)))
			break;
	}
	printf("x=%d ", ret&0x1f);
	while (1)
	{
		ret = read_data(0x1);
		if (!(ret & (1<<6)))
			break;
	}
	printf("y=%d ", ret&0x1f);
	while (1)
	{
		ret = read_data(0x2);
		if (!(ret & (1<<6)))
			break;
	}
	printf("z=%d ", ret&0x1f);
	
#endif

	printf("\n");
}

void delay(int val)
{
        val *= 66; 

        while (val--)
                ;   
}

