/********************************************************************
 *		gpio i2c for smdkc110.
 *		created by JH.LIM		
 *		first revision .2010.10
 *		
 *********************************************************************/
 
#include "mx100_inc.h"

#define GPD1CON		*(volatile unsigned long *)(0xE02000C0)
#define GPD1DAT		*(volatile unsigned long *)(0xE02000C4)
#define GPD1PUD		*(volatile unsigned long *)(0xE02000C8)

#define	GPIO_CON_PORT_MASK		0xF
#define	GPIO_CON_PORT_OFFSET		0x4
#define	GPIO_CON_PORT_OFFSET_SHIFT		0x2

#define	GPIO_CON_INPUT				0x0
#define	GPIO_CON_OUTPUT			0x1


#define IIC_ESCL_Hi(ch)	GPD1DAT |= (0x1<<((ch<<1) +1))
#define IIC_ESCL_Lo(ch)	GPD1DAT &= ~(0x1<<((ch<<1) +1))
#define IIC_ESDA_Hi(ch)	GPD1DAT |= (0x1<<(ch<<1))
#define IIC_ESDA_Lo(ch)	GPD1DAT &= ~(0x1<<(ch<<1))


#define IIC_SDA_DATA(ch)	  ((GPD1DAT >> (ch<<1)) &0x1)

#define IIC_ESCL_PORT_SET(ch,inout) 	GPD1CON &= (~(GPIO_CON_PORT_MASK << ( ((ch<<1) +1)   << GPIO_CON_PORT_OFFSET_SHIFT))); \
					 		GPD1CON |=  ( (inout << (((ch<<1) + 1)  << GPIO_CON_PORT_OFFSET_SHIFT)));

#define IIC_ESDA_PORT_SET(ch,inout) 	GPD1CON &= (~(GPIO_CON_PORT_MASK << ( (ch<<1)   << GPIO_CON_PORT_OFFSET_SHIFT))); \
					 		GPD1CON |=  ( ( inout << ((ch<<1) << GPIO_CON_PORT_OFFSET_SHIFT)));

#define DELAY_VALUE			(100)
static  int DELAY = DELAY_VALUE;

int modify_delay(int updown)
{
DELAY += updown;
return DELAY;
}
__inline void _Delay(void)
{
	unsigned long i,j;
	for(i=0;i<DELAY;i++);
}

__inline void _SCLH_SDAH(int ch)
{
	IIC_ESCL_Hi(ch);
	IIC_ESDA_Hi(ch);
	_Delay();
}

__inline void _SCLH_SDAL(int ch)
{
	IIC_ESCL_Hi(ch);
	IIC_ESDA_Lo(ch);
	_Delay();
}

__inline void _SCLL_SDAH(int ch)
{
	IIC_ESCL_Lo(ch);
	IIC_ESDA_Hi(ch);
	_Delay();
}

__inline void _SCLL_SDAL(int ch)
{
	IIC_ESCL_Lo(ch);
	IIC_ESDA_Lo(ch);
	_Delay();
}

__inline void _IIC_ELow(int ch)
{
	_SCLL_SDAL(ch);
	_SCLH_SDAL(ch);
	_SCLH_SDAL(ch);
	_SCLL_SDAL(ch);
}

__inline void _IIC_EHigh(int ch)
{
	_SCLL_SDAH(ch);
	_SCLH_SDAH(ch);
	_SCLH_SDAH(ch);
	_SCLL_SDAH(ch);
}

__inline int _IIC_GET_SDA(int ch)
{
	int read_data;
	
	IIC_ESDA_PORT_SET(ch,GPIO_CON_INPUT);			// Function <- Input

	IIC_ESCL_Lo(ch);
	_Delay();
	IIC_ESCL_Hi(ch);
	_Delay();

	read_data=IIC_SDA_DATA(ch);
	
	IIC_ESCL_Hi(ch);
	_Delay();
	IIC_ESCL_Hi(ch);
	_Delay();
	return read_data;
}

__inline void _IIC_EStart(int ch)
{
	_SCLH_SDAH(ch);
	_SCLH_SDAL(ch);
	_Delay();
	_SCLL_SDAL(ch);
}

__inline void _IIC_EEnd(int ch)
{
	_SCLL_SDAL(ch);
	_SCLH_SDAL(ch);
	_Delay();
	_SCLH_SDAH(ch);
}

__inline int _IIC_EAck(int ch)
{
	unsigned long icount;
	unsigned long ack;
	
	IIC_ESDA_PORT_SET(ch,GPIO_CON_INPUT);			// Function <- Input

	IIC_ESCL_Lo(ch);
	_Delay();
	IIC_ESCL_Hi(ch);
	_Delay();
	ack = GPD1DAT;
	IIC_ESCL_Hi(ch);
	_Delay();
	IIC_ESCL_Hi(ch);
	_Delay();

	IIC_ESDA_PORT_SET(ch,GPIO_CON_OUTPUT);			// Function <- Output (SDA)

	icount =0;
	ack = (ack>>(ch << 1))&0x1;


	for(icount = 0;icount<1000;icount++) {
		if(ack==0) break;
	}

	_SCLL_SDAL(ch);

	if(icount==1000) return 0;
	else return 1;
}

__inline void _IIC_ReadAck(int ch)
{
	IIC_ESDA_PORT_SET(ch,GPIO_CON_OUTPUT);			// Function <- Output (SDA)
	_SCLL_SDAL(ch);
}

void IRIVER_IIC_SlaveInit(struct i2c_slave_ctl *pi2c_ch_ctl,
							unsigned long ch,
							unsigned long slave_write_addr,
							unsigned long slave_read_addr)
{
#if 0
2.2.8.3 Port Group GPD1 Control Register (GPD1PUD, R/W, Address = 0xE020_00C8)
GPD1PUD
Bit
Description
Initial State
GPD1PUD[n]
[2n+1:2n]
00 = Pull-up/ down disabled 01 = Pull-down enabled 10 = Pull-up enabled 11 = Reserved
0x0555
#endif
pi2c_ch_ctl->i2c_ch = ch;
pi2c_ch_ctl->slave_read_addr = slave_read_addr;
pi2c_ch_ctl->slave_write_addr = slave_write_addr;

	GPD1PUD &= ~(0xf<< (pi2c_ch_ctl->i2c_ch << 2));	// Pull Up/Down Disable	SCL, SDA
//	GPD1PUD &= ~(0b0101 << (pi2c_ch_ctl->i2c_ch << 1));	// Pull Up/Down Disable	SCL, SDA

	IIC_ESCL_Hi(pi2c_ch_ctl->i2c_ch);
	IIC_ESDA_Hi(pi2c_ch_ctl->i2c_ch);

//	IIC_ESCL_OUTP;	
	IIC_ESCL_PORT_SET(pi2c_ch_ctl->i2c_ch,GPIO_CON_OUTPUT);
	// Function <- Output (SCL)
	IIC_ESDA_PORT_SET(pi2c_ch_ctl->i2c_ch,GPIO_CON_OUTPUT);		

	_Delay();
}

#if 1
int IRIVER_IIC_ReadByte (struct i2c_slave_ctl *pi2c_ch_ctlint, unsigned char RegAddr)
{
	unsigned char read_data = 0;
	unsigned long i;
	unsigned long ack_value = 1;
	int ret=1;

	IIC_ESDA_PORT_SET(pi2c_ch_ctlint->i2c_ch,GPIO_CON_OUTPUT);			// Function <- Input
	
	_IIC_EStart(pi2c_ch_ctlint->i2c_ch);


////////////////// write chip id //////////////////
	for(i = 7; i>0; i--)
	{
		if((pi2c_ch_ctlint->slave_write_addr>> (i-1)) & 0x0001)
			_IIC_EHigh(pi2c_ch_ctlint->i2c_ch);
		else
			_IIC_ELow(pi2c_ch_ctlint->i2c_ch);
	}

	_IIC_ELow(pi2c_ch_ctlint->i2c_ch);	// write 'W'

	ack_value=_IIC_EAck(pi2c_ch_ctlint->i2c_ch);	// ACK

	if(ack_value==0) {
		#ifdef PRINT_I2C_ERR
		printf("%d : i2c slave addr Wr Err :CH:%d SA:%x RegAddr:%x\n",__LINE__,pi2c_ch_ctlint->i2c_ch,pi2c_ch_ctlint->slave_write_addr,RegAddr);
		#endif
		ret = -1;
		goto fail_exit;
	} 
////////////////// write reg. addr. //////////////////
	for(i = 8; i>0; i--)
	{
		if((RegAddr >> (i-1)) & 0x0001)
			_IIC_EHigh(pi2c_ch_ctlint->i2c_ch);
		else
			_IIC_ELow(pi2c_ch_ctlint->i2c_ch);
	}

	ack_value=_IIC_EAck(pi2c_ch_ctlint->i2c_ch);	// ACK

	if(ack_value==0) {
		#ifdef PRINT_I2C_ERR
		printf("%d : i2c reg addr Wr Err :CH:%d SA:%x RegAddr:%x\n",__LINE__,pi2c_ch_ctlint->i2c_ch,pi2c_ch_ctlint->slave_write_addr,RegAddr);
		#endif
		ret = -2;
	
		goto fail_exit;
	
	} 

	_IIC_EStart(pi2c_ch_ctlint->i2c_ch);


	////////////////// write chip addr. //////////////////

	for(i = 7; i>0; i--)
	{
		if((pi2c_ch_ctlint->slave_read_addr>> (i-1)) & 0x0001)
			_IIC_EHigh(pi2c_ch_ctlint->i2c_ch);
		else
			_IIC_ELow(pi2c_ch_ctlint->i2c_ch);
	}

	_IIC_EHigh(pi2c_ch_ctlint->i2c_ch);	// read 'R'
	
	ack_value=_IIC_EAck(pi2c_ch_ctlint->i2c_ch);	// ACK

	if(ack_value==0) {
		#ifdef PRINT_I2C_ERR
		printf("%d: i2c slave addr2 Wr Err :CH:%d SA:%x RegAddr:%x\n",__LINE__,pi2c_ch_ctlint->i2c_ch,pi2c_ch_ctlint->slave_write_addr,RegAddr);
		#endif
		ret = -3;
	
		goto fail_exit;
	
	} 
	

 	 IIC_ESDA_PORT_SET(pi2c_ch_ctlint->i2c_ch,GPIO_CON_INPUT);			// Function <- Input


////////////////// read data. //////////////////
	for(i = 8; i>0; i--)
	{
		read_data <<=1;		
		read_data |= _IIC_GET_SDA(pi2c_ch_ctlint->i2c_ch);
	}

	_IIC_ReadAck(pi2c_ch_ctlint->i2c_ch);
		

	_IIC_EEnd(pi2c_ch_ctlint->i2c_ch);

	return read_data;

	fail_exit:
		
	if(ack_value==0) return ret;

	return 0;	
}
#endif

int IRIVER_IIC_WriteByte (struct i2c_slave_ctl *pi2c_ch_ctlint, unsigned char RegAddr, unsigned char IicData)
{
	unsigned long i;
	unsigned long ack_value = 1;
	int ret;
	
	_IIC_EStart(pi2c_ch_ctlint->i2c_ch);

	IIC_ESDA_PORT_SET(pi2c_ch_ctlint->i2c_ch,GPIO_CON_OUTPUT);		

////////////////// write chip id //////////////////
	for(i = 7; i>0; i--)
	{
		if((pi2c_ch_ctlint->slave_write_addr>> (i-1)) & 0x0001)
			_IIC_EHigh(pi2c_ch_ctlint->i2c_ch);
		else
			_IIC_ELow(pi2c_ch_ctlint->i2c_ch);
	}

	_IIC_ELow(pi2c_ch_ctlint->i2c_ch);	// write 'W'

	ack_value=_IIC_EAck(pi2c_ch_ctlint->i2c_ch);	// ACK

////////////////// write reg. addr. //////////////////
	for(i = 8; i>0; i--)
	{
		if((RegAddr >> (i-1)) & 0x0001)
			_IIC_EHigh(pi2c_ch_ctlint->i2c_ch);
		else
			_IIC_ELow(pi2c_ch_ctlint->i2c_ch);
	}

	ack_value=_IIC_EAck(pi2c_ch_ctlint->i2c_ch);	// ACK

////////////////// write reg. data. //////////////////
	for(i = 8; i>0; i--)
	{
		if((IicData >> (i-1)) & 0x0001)
			_IIC_EHigh(pi2c_ch_ctlint->i2c_ch);
		else
			_IIC_ELow(pi2c_ch_ctlint->i2c_ch);
	}

	ack_value=_IIC_EAck(pi2c_ch_ctlint->i2c_ch);	// ACK

	_IIC_EEnd(pi2c_ch_ctlint->i2c_ch);

	if(ack_value==0) {
		#ifdef PRINT_I2C_ERR
		printf("i2c Wr Err :CH:%d SA:%x RegAddr:%x\n",pi2c_ch_ctlint->i2c_ch,pi2c_ch_ctlint->slave_write_addr,RegAddr);
		#endif
		
		ret = -1;
	} else {
		ret = ack_value;
	}

	return ret;
}

int IRIVER_IIC_WriteData (struct i2c_slave_ctl *pi2c_ch_ctlint, unsigned char *pData, int iDataSize)
{
	unsigned long i,j;
	unsigned long ack_value = 1;
	int iWriteSize = 0;
	
	_IIC_EStart(pi2c_ch_ctlint->i2c_ch);

	IIC_ESDA_PORT_SET(pi2c_ch_ctlint->i2c_ch,GPIO_CON_OUTPUT);		

////////////////// write chip id //////////////////
	for(i = 7; i>0; i--)
	{
		if((pi2c_ch_ctlint->slave_write_addr>> (i-1)) & 0x0001)
			_IIC_EHigh(pi2c_ch_ctlint->i2c_ch);
		else
			_IIC_ELow(pi2c_ch_ctlint->i2c_ch);
	}

	_IIC_ELow(pi2c_ch_ctlint->i2c_ch);	// write 'W'

	ack_value=_IIC_EAck(pi2c_ch_ctlint->i2c_ch);	// ACK

////////////////// write reg. addr. //////////////////

	for(j=0;j<iDataSize;j++) {
		
		for(i = 8; i>0; i--)
		{
			if(((*pData) >> (i-1)) & 0x0001)
				_IIC_EHigh(pi2c_ch_ctlint->i2c_ch);
			else
				_IIC_ELow(pi2c_ch_ctlint->i2c_ch);
		}

		ack_value=_IIC_EAck(pi2c_ch_ctlint->i2c_ch);	// ACK
		if(ack_value==1) {
			iWriteSize++;
		} else {

		}
		pData++;
	}
	
	_IIC_EEnd(pi2c_ch_ctlint->i2c_ch);

	if(ack_value==0) {
		#ifdef PRINT_I2C_ERR
		printf("i2c Wr Err :CH:%d SA:%x\n",pi2c_ch_ctlint->i2c_ch,pi2c_ch_ctlint->slave_write_addr);
		#endif
	}

	return	iWriteSize ;
}

