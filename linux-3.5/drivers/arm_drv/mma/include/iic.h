
#ifndef __IIC_REG_H
#define __IIC_REG_H

#define IIC0_BASE	0x13860000
#define IIC1_BASE	0x13870000 
#define IIC2_BASE	0x13880000 
#define IIC3_BASE	0x13890000
#define IIC4_BASE	0x138A0000 
#define IIC5_BASE	0x138B0000 
#define IIC6_BASE	0x138C0000 
#define IIC7_BASE	0x138D0000 
#define IIC8_BASE	0x138E0000


typedef struct {
	unsigned int I2CCONn;
	unsigned int I2CSTATn;
	unsigned int I2CADDn;
	unsigned int I2CDSn;
	unsigned int I2CLCn;
}I2C_REG_T;

I2C_REG_T *IIC0_reg = (I2C_REG_T *)IIC0_BASE; 
I2C_REG_T *IIC1_reg = (I2C_REG_T *)IIC1_BASE; 
I2C_REG_T *IIC2_reg = (I2C_REG_T *)IIC2_BASE; 
I2C_REG_T *IIC3_reg = (I2C_REG_T *)IIC3_BASE; 
I2C_REG_T *IIC4_reg = (I2C_REG_T *)IIC4_BASE; 
I2C_REG_T *IIC5_reg = (I2C_REG_T *)IIC5_BASE; 
I2C_REG_T *IIC6_reg = (I2C_REG_T *)IIC6_BASE; 
I2C_REG_T *IIC7_reg = (I2C_REG_T *)IIC7_BASE; 
I2C_REG_T *IIC8_reg = (I2C_REG_T *)IIC8_BASE; 


#define GPA1CON  *(volatile unsigned int *)(0x11400000 + 0x0020)
#define GPD1CON  *(volatile unsigned int *)(0x11400000 + 0x00C0)

#define GPX3CON 		*(u32 *)(0x11000000 + 0x0C60) // gpx3_1 -->eint25
#define EXT_INT43CON 		*(u32 *)(0x11000000 + 0x0E0C)
#define EXT_INT43_FLTCON0 	*(u32 *)(0x11000000 + 0x0E98)
#define EXT_INT43_MASK 		*(u32 *)(0x11000000 + 0x0F0C)
#define EXT_INT43_PEND 		*(u32 *)(0x11000000 + 0x0F4c)
              
#endif /* __IIC_REG_H */


