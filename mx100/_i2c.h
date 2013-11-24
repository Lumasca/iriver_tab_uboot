/********************************************************************
 *		gpio i2c for smdkc110.
 *		created by JH.LIM		
 *		first revision .2010.9 :
 *		
 *********************************************************************/
 

#ifndef __MX100_I2C_H__
#define __MX100_I2C_H__

enum I2C_CH
{
	I2C_CH0				= 0,	 //for Gerneral
	I2C_CH_HDMI_DDC	= 1,  //for DDC
	I2C_CH2				= 2,	 //for PMIC		
	I2C_CH_HDMI_PHY	= 3,	 //Internally only
	
	I2C_CH_NUM_END,
};

struct i2c_slave_ctl
{
	unsigned long slave_write_addr;
	unsigned long slave_read_addr;
	unsigned long i2c_ch;	
};

//#define PRINT_I2C_ERR

void IRIVER_IIC_SlaveInit(struct i2c_slave_ctl *pi2c_ch_ctl,unsigned long ch,unsigned long slave_write_addr,unsigned long slave_read_addr);
int IRIVER_IIC_ReadByte (struct i2c_slave_ctl *pi2c_ch_ctlint, unsigned char RegAddr);
int IRIVER_IIC_WriteByte (struct i2c_slave_ctl *pi2c_ch_ctlint, unsigned char IicAddr, unsigned char IicData);
int IRIVER_IIC_WriteData (struct i2c_slave_ctl *pi2c_ch_ctlint, unsigned char *pData, int iDataSize);

int modify_delay(int updown);

#endif /*__PMIC_H__*/

