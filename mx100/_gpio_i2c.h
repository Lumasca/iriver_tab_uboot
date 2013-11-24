/********************************************************************
 *		gpio i2c for smdkc110.
 *		created by JH.LIM		
 *		first revision .2010.9 :
 *		
 *********************************************************************/
 

#ifndef __MX100_GPIO_I2C_H__
#define __MX100_GPIO_I2C_H__

struct gpio_i2c_slave_ctl
{
	unsigned long slave_write_addr;
	unsigned long slave_read_addr;
	unsigned int    sda_gpio_group;
	unsigned int    sda_gpio_num;

	unsigned int    scl_gpio_group;
	unsigned int    scl_gpio_num;
	
//	unsigned long i2c_ch;	
};


 __inline void GPIO_IIC_ESCL_Hi(struct gpio_i2c_slave_ctl *pi2c_ch_ctl);	
 __inline void GPIO_IIC_ESCL_Lo(struct gpio_i2c_slave_ctl *pi2c_ch_ctl);	
 __inline void GPIO_IIC_ESDA_Hi(struct gpio_i2c_slave_ctl *pi2c_ch_ctl);
 __inline void GPIO_IIC_ESDA_Lo(struct gpio_i2c_slave_ctl *pi2c_ch_ctl);
 __inline int GPIO_IIC_SDA_DATA(struct gpio_i2c_slave_ctl *pi2c_ch_ctl);
 __inline void GPIO_IIC_ESCL_PORT_SET(struct gpio_i2c_slave_ctl *pi2c_ch_ctl,int inout) ;
__inline void  GPIO_IIC_ESDA_PORT_SET(struct gpio_i2c_slave_ctl *pi2c_ch_ctl,int inout) ;



#define PRINT_GPIO_I2C_ERR

void IRIVER_GPIO_IIC_SlaveInit(struct gpio_i2c_slave_ctl *pi2c_ch_ctl,
							unsigned int    sda_gpio_group,
							unsigned int    sda_gpio_num,
							unsigned int    scl_gpio_group,
							unsigned int    scl_gpio_num,		
							unsigned long slave_write_addr,
							unsigned long slave_read_addr);

int IRIVER_GPIO_IIC_ReadByte (struct gpio_i2c_slave_ctl *pi2c_ch_ctlint, unsigned char RegAddr);
int IRIVER_GPIO_IIC_ReadData(struct gpio_i2c_slave_ctl *pi2c_ch_ctlint,unsigned char RegAddr ,unsigned char *data,int len);

int IRIVER_GPIO_IIC_WriteByte (struct gpio_i2c_slave_ctl *pi2c_ch_ctlint, unsigned char IicAddr, unsigned char IicData);
int IRIVER_GPIO_IIC_WriteData (struct gpio_i2c_slave_ctl *pi2c_ch_ctlint, unsigned char *pData, int iDataSize);

#endif /*__PMIC_H__*/

