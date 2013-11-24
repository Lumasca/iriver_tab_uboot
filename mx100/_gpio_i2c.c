/********************************************************************
 *		gpio i2c for smdkc110.
 *		created by JH.LIM		
 *		first revision .2010.11.28
 *		
 *********************************************************************/
 
#include <config.h>
#include "_gpio_i2c.h"

#include "_gpio.h"
#include "gpio-mx100.h"

#define	GPIO_CON_INPUT				0x0
#define	GPIO_CON_OUTPUT			0x1



#define DELAY_VALUE			(100)
static  int DELAY = DELAY_VALUE;


static __inline void _Delay(void)
{
	unsigned long i;
	for(i=0;i<DELAY;i++);
}

//	GPIO_SetFunctionEach(U_GPIO_ETHERNET_ENABLE_OUT, eGPI);	
//	GPIO_SetPullUpDownEach(U_GPIO_ETHERNET_ENABLE_OUT ,eGPUen);
//	GPIO_SetDataEach(U_GPIO_ETHERNET_ENABLE_OUT,1);
 __inline void GPIO_IIC_ESCL_Hi(struct gpio_i2c_slave_ctl *pi2c_ch_ctl)	
{
	GPIO_SetDataEach(pi2c_ch_ctl->scl_gpio_group,pi2c_ch_ctl->scl_gpio_num,1);
}
 __inline void GPIO_IIC_ESCL_Lo(struct gpio_i2c_slave_ctl *pi2c_ch_ctl)	
{
	GPIO_SetDataEach(pi2c_ch_ctl->scl_gpio_group,pi2c_ch_ctl->scl_gpio_num,0);
}		

 __inline void GPIO_IIC_ESDA_Hi(struct gpio_i2c_slave_ctl *pi2c_ch_ctl)
{
	GPIO_SetDataEach(pi2c_ch_ctl->sda_gpio_group,pi2c_ch_ctl->sda_gpio_num,1);
}
 __inline void GPIO_IIC_ESDA_Lo(struct gpio_i2c_slave_ctl *pi2c_ch_ctl)
{
	GPIO_SetDataEach(pi2c_ch_ctl->sda_gpio_group,pi2c_ch_ctl->sda_gpio_num,0);
}

 __inline int GPIO_IIC_SDA_DATA(struct gpio_i2c_slave_ctl *pi2c_ch_ctl)
{
	return GPIO_GetDataEach(pi2c_ch_ctl->sda_gpio_group,pi2c_ch_ctl->sda_gpio_num);
}

 __inline void GPIO_IIC_ESCL_PORT_SET(struct gpio_i2c_slave_ctl *pi2c_ch_ctl,int inout) 
{

	if(inout==GPIO_CON_OUTPUT) {
		GPIO_SetFunctionEach(pi2c_ch_ctl->scl_gpio_group,pi2c_ch_ctl->scl_gpio_num, eGPO);	
	} else {
		GPIO_SetFunctionEach(pi2c_ch_ctl->scl_gpio_group,pi2c_ch_ctl->scl_gpio_num, eGPI);	
	}
}
__inline void  GPIO_IIC_ESDA_PORT_SET(struct gpio_i2c_slave_ctl *pi2c_ch_ctl,int inout) 
{	
	if(inout==GPIO_CON_OUTPUT) {
		GPIO_SetFunctionEach(pi2c_ch_ctl->sda_gpio_group,pi2c_ch_ctl->sda_gpio_num, eGPO);	
	} else {
		GPIO_SetFunctionEach(pi2c_ch_ctl->sda_gpio_group,pi2c_ch_ctl->sda_gpio_num, eGPI);	
	}
}

static __inline void _SCLH_SDAH(struct gpio_i2c_slave_ctl *pi2c_ch_ctl)
{
	GPIO_IIC_ESCL_Hi(pi2c_ch_ctl);
	GPIO_IIC_ESDA_Hi(pi2c_ch_ctl);
	_Delay();
}

static __inline void _SCLH_SDAL(struct gpio_i2c_slave_ctl *pi2c_ch_ctl)
{
	GPIO_IIC_ESCL_Hi(pi2c_ch_ctl);
	GPIO_IIC_ESDA_Lo(pi2c_ch_ctl);
	_Delay();
}

static __inline void _SCLL_SDAH(struct gpio_i2c_slave_ctl *pi2c_ch_ctl)
{
	GPIO_IIC_ESCL_Lo(pi2c_ch_ctl);
	GPIO_IIC_ESDA_Hi(pi2c_ch_ctl);
	_Delay();
}

static __inline void _SCLL_SDAL(struct gpio_i2c_slave_ctl *pi2c_ch_ctl)
{
	GPIO_IIC_ESCL_Lo(pi2c_ch_ctl);
	GPIO_IIC_ESDA_Lo(pi2c_ch_ctl);
	_Delay();
}

static __inline void _GPIO_IIC_ELow(struct gpio_i2c_slave_ctl *pi2c_ch_ctl)
{
	_SCLL_SDAL(pi2c_ch_ctl);
	_SCLH_SDAL(pi2c_ch_ctl);
	_SCLH_SDAL(pi2c_ch_ctl);
	_SCLL_SDAL(pi2c_ch_ctl);
}

static __inline void _GPIO_IIC_EHigh(struct gpio_i2c_slave_ctl *pi2c_ch_ctl)
{
	_SCLL_SDAH(pi2c_ch_ctl);
	_SCLH_SDAH(pi2c_ch_ctl);
	_SCLH_SDAH(pi2c_ch_ctl);
	_SCLL_SDAH(pi2c_ch_ctl);
}

static __inline int _GPIO_IIC_GET_SDA(struct gpio_i2c_slave_ctl *pi2c_ch_ctl)
{
	int read_data;
	
	GPIO_IIC_ESDA_PORT_SET(pi2c_ch_ctl,GPIO_CON_INPUT);			// Function <- Input

	GPIO_IIC_ESCL_Lo(pi2c_ch_ctl);
	_Delay();
	GPIO_IIC_ESCL_Hi(pi2c_ch_ctl);
	_Delay();

	read_data=GPIO_IIC_SDA_DATA(pi2c_ch_ctl);
	
	GPIO_IIC_ESCL_Hi(pi2c_ch_ctl);
	_Delay();
	GPIO_IIC_ESCL_Hi(pi2c_ch_ctl);
	_Delay();
	return read_data;
}

static __inline void _GPIO_IIC_EStart(struct gpio_i2c_slave_ctl *pi2c_ch_ctl)
{
	_SCLH_SDAH(pi2c_ch_ctl);
	_SCLH_SDAL(pi2c_ch_ctl);
	_Delay();
	_SCLL_SDAL(pi2c_ch_ctl);
}

static __inline void _GPIO_IIC_EEnd(struct gpio_i2c_slave_ctl *pi2c_ch_ctl)
{
	_SCLL_SDAL(pi2c_ch_ctl);
	_SCLH_SDAL(pi2c_ch_ctl);
	_Delay();
	_SCLH_SDAH(pi2c_ch_ctl);
}

static __inline int _GPIO_IIC_EAck(struct gpio_i2c_slave_ctl *pi2c_ch_ctl)
{
	unsigned long icount;
	unsigned long ack;
	
	GPIO_IIC_ESDA_PORT_SET(pi2c_ch_ctl,GPIO_CON_INPUT);			// Function <- Input

	GPIO_IIC_ESCL_Lo(pi2c_ch_ctl);
	_Delay();
	GPIO_IIC_ESCL_Hi(pi2c_ch_ctl);
	_Delay();

	ack = GPIO_IIC_SDA_DATA(pi2c_ch_ctl);

	GPIO_IIC_ESCL_Hi(pi2c_ch_ctl);
	_Delay();
	GPIO_IIC_ESCL_Hi(pi2c_ch_ctl);
	_Delay();

	GPIO_IIC_ESDA_PORT_SET(pi2c_ch_ctl,GPIO_CON_OUTPUT);			// Function <- Output (SDA)

	icount =0;

	for(icount = 0;icount<1000;icount++) {
		if(ack==0) break;
	}

	_SCLL_SDAL(pi2c_ch_ctl);

	if(icount==1000) return 0;
	else return 1;
}

static __inline void _GPIO_IIC_ReadAck(struct gpio_i2c_slave_ctl *pi2c_ch_ctl)
{
	GPIO_IIC_ESDA_PORT_SET(pi2c_ch_ctl,GPIO_CON_OUTPUT);			// Function <- Output (SDA)
	_SCLL_SDAL(pi2c_ch_ctl);
}

void IRIVER_GPIO_IIC_SlaveInit(struct gpio_i2c_slave_ctl *pi2c_ch_ctl,
							unsigned int    sda_gpio_group,
							unsigned int    sda_gpio_num,
							unsigned int    scl_gpio_group,
							unsigned int    scl_gpio_num,		
							unsigned long slave_write_addr,
							unsigned long slave_read_addr)
{


//pi2c_ch_ctl->i2c_ch = ch;

pi2c_ch_ctl->sda_gpio_group  = sda_gpio_group;
pi2c_ch_ctl->sda_gpio_num= sda_gpio_num;
pi2c_ch_ctl->scl_gpio_group = scl_gpio_group;
pi2c_ch_ctl->scl_gpio_num = scl_gpio_num;

pi2c_ch_ctl->slave_read_addr = slave_read_addr;
pi2c_ch_ctl->slave_write_addr = slave_write_addr;


GPIO_SetPullUpDownEach(pi2c_ch_ctl->sda_gpio_group ,pi2c_ch_ctl->sda_gpio_num,eGPUDdis);
GPIO_SetPullUpDownEach(pi2c_ch_ctl->scl_gpio_group ,pi2c_ch_ctl->scl_gpio_num,eGPUDdis);


//	GPD1PUD &= ~(0xf<< (pi2c_ch_ctl->i2c_ch << 2));	// Pull Up/Down Disable	SCL, SDA
//	GPD1PUD &= ~(0b0101 << (pi2c_ch_ctl->i2c_ch << 1));	// Pull Up/Down Disable	SCL, SDA

GPIO_IIC_ESCL_Hi(pi2c_ch_ctl);
GPIO_IIC_ESDA_Hi(pi2c_ch_ctl);

//	GPIO_IIC_ESCL_OUTP;	
GPIO_IIC_ESCL_PORT_SET(pi2c_ch_ctl,GPIO_CON_OUTPUT);
// Function <- Output (SCL)
GPIO_IIC_ESDA_PORT_SET(pi2c_ch_ctl,GPIO_CON_OUTPUT);		

_Delay();
}

int IRIVER_GPIO_IIC_ReadByte (struct gpio_i2c_slave_ctl *pi2c_ch_ctlint, unsigned char RegAddr)
{
	unsigned char read_data = 0;
	unsigned long i;
	unsigned long ack_value = 1;
	int ret=1;

	GPIO_IIC_ESDA_PORT_SET(pi2c_ch_ctlint,GPIO_CON_OUTPUT);			// Function <- Input
	
	_GPIO_IIC_EStart(pi2c_ch_ctlint);


////////////////// write chip id //////////////////
	for(i = 7; i>0; i--)
	{
		if((pi2c_ch_ctlint->slave_write_addr>> (i-1)) & 0x0001)
			_GPIO_IIC_EHigh(pi2c_ch_ctlint);
		else
			_GPIO_IIC_ELow(pi2c_ch_ctlint);
	}

	_GPIO_IIC_ELow(pi2c_ch_ctlint);	// write 'W'

	ack_value=_GPIO_IIC_EAck(pi2c_ch_ctlint);	// ACK

	if(ack_value==0) {
		#ifdef PRINT_GPIO_I2C_ERR
		printf("%d : i2c slave addr Wr Err :CH:%d SA:%x RegAddr:%x\n",__LINE__,pi2c_ch_ctlint,pi2c_ch_ctlint->slave_write_addr,RegAddr);
		#endif
		ret = -1;
		goto fail_exit;
	} 
////////////////// write reg. addr. //////////////////
	for(i = 8; i>0; i--)
	{
		if((RegAddr >> (i-1)) & 0x0001)
			_GPIO_IIC_EHigh(pi2c_ch_ctlint);
		else
			_GPIO_IIC_ELow(pi2c_ch_ctlint);
	}

	ack_value=_GPIO_IIC_EAck(pi2c_ch_ctlint);	// ACK

	if(ack_value==0) {
		#ifdef PRINT_GPIO_I2C_ERR
		printf("%d : i2c reg addr Wr Err :CH:%d SA:%x RegAddr:%x\n",__LINE__,pi2c_ch_ctlint,pi2c_ch_ctlint->slave_write_addr,RegAddr);
		#endif
		ret = -2;
	
		goto fail_exit;
	
	} 

	_GPIO_IIC_EStart(pi2c_ch_ctlint);


	////////////////// write chip addr. //////////////////

	for(i = 7; i>0; i--)
	{
		if((pi2c_ch_ctlint->slave_read_addr>> (i-1)) & 0x0001)
			_GPIO_IIC_EHigh(pi2c_ch_ctlint);
		else
			_GPIO_IIC_ELow(pi2c_ch_ctlint);
	}

	_GPIO_IIC_EHigh(pi2c_ch_ctlint);	// read 'R'
	
	ack_value=_GPIO_IIC_EAck(pi2c_ch_ctlint);	// ACK

	if(ack_value==0) {
		#ifdef PRINT_GPIO_I2C_ERR
		printf("%d: i2c slave addr2 Wr Err :CH:%d SA:%x RegAddr:%x\n",__LINE__,pi2c_ch_ctlint,pi2c_ch_ctlint->slave_write_addr,RegAddr);
		#endif
		ret = -3;
	
		goto fail_exit;
	
	} 
	

 	 GPIO_IIC_ESDA_PORT_SET(pi2c_ch_ctlint,GPIO_CON_INPUT);			// Function <- Input


////////////////// read data. //////////////////
	for(i = 8; i>0; i--)
	{
		read_data <<=1;		
		read_data |= _GPIO_IIC_GET_SDA(pi2c_ch_ctlint);
	}

	_GPIO_IIC_ReadAck(pi2c_ch_ctlint);
		

	_GPIO_IIC_EEnd(pi2c_ch_ctlint);

	return read_data;

	fail_exit:
		
	if(ack_value==0) return ret;
	
}


int IRIVER_GPIO_IIC_ReadData(struct gpio_i2c_slave_ctl *pi2c_ch_ctlint,unsigned char RegAddr ,unsigned char *data,int len)
{
	unsigned char read_data = 0;
	unsigned long i,j;
	unsigned long ack_value = 1;
	int ret=1;

	GPIO_IIC_ESDA_PORT_SET(pi2c_ch_ctlint,GPIO_CON_OUTPUT);			// Function <- Input
	
	_GPIO_IIC_EStart(pi2c_ch_ctlint);


////////////////// write chip id //////////////////
	for(i = 7; i>0; i--)
	{
		if((pi2c_ch_ctlint->slave_write_addr>> (i-1)) & 0x0001)
			_GPIO_IIC_EHigh(pi2c_ch_ctlint);
		else
			_GPIO_IIC_ELow(pi2c_ch_ctlint);
	}

	_GPIO_IIC_ELow(pi2c_ch_ctlint);	// write 'W'

	ack_value=_GPIO_IIC_EAck(pi2c_ch_ctlint);	// ACK

	if(ack_value==0) {
		#ifdef PRINT_GPIO_I2C_ERR
		printf("%d : i2c slave addr Wr Err :CH:%d SA:%x RegAddr:%x\n",__LINE__,pi2c_ch_ctlint,pi2c_ch_ctlint->slave_write_addr,RegAddr);
		#endif
		ret = -1;
		goto fail_exit;
	} 
////////////////// write reg. addr. //////////////////
	for(i = 8; i>0; i--)
	{
		if((RegAddr >> (i-1)) & 0x0001)
			_GPIO_IIC_EHigh(pi2c_ch_ctlint);
		else
			_GPIO_IIC_ELow(pi2c_ch_ctlint);
	}

	ack_value=_GPIO_IIC_EAck(pi2c_ch_ctlint);	// ACK

	if(ack_value==0) {
		#ifdef PRINT_GPIO_I2C_ERR
		printf("%d : i2c reg addr Wr Err :CH:%d SA:%x RegAddr:%x\n",__LINE__,pi2c_ch_ctlint,pi2c_ch_ctlint->slave_write_addr,RegAddr);
		#endif
		ret = -2;
	
		goto fail_exit;
	
	} 

	_GPIO_IIC_EStart(pi2c_ch_ctlint);


	////////////////// write chip addr. //////////////////

	for(i = 7; i>0; i--)
	{
		if((pi2c_ch_ctlint->slave_read_addr>> (i-1)) & 0x0001)
			_GPIO_IIC_EHigh(pi2c_ch_ctlint);
		else
			_GPIO_IIC_ELow(pi2c_ch_ctlint);
	}

	_GPIO_IIC_EHigh(pi2c_ch_ctlint);	// read 'R'
	
	ack_value=_GPIO_IIC_EAck(pi2c_ch_ctlint);	// ACK

	if(ack_value==0) {
		#ifdef PRINT_GPIO_I2C_ERR
		printf("%d: i2c slave addr2 Wr Err :CH:%d SA:%x RegAddr:%x\n",__LINE__,pi2c_ch_ctlint,pi2c_ch_ctlint->slave_write_addr,RegAddr);
		#endif
		ret = -3;
	
		goto fail_exit;
	
	} 
	

 	 GPIO_IIC_ESDA_PORT_SET(pi2c_ch_ctlint,GPIO_CON_INPUT);			// Function <- Input


	for(j=0;j<len;j++) {
		read_data = 0;

	////////////////// read data. //////////////////
		for(i = 8; i>0; i--)
		{
			read_data <<=1;		
			read_data |= _GPIO_IIC_GET_SDA(pi2c_ch_ctlint);
		}

		_GPIO_IIC_ReadAck(pi2c_ch_ctlint);
		data[j]= read_data;
	}

	
	_GPIO_IIC_EEnd(pi2c_ch_ctlint);

	return j;

	fail_exit:
		
	if(ack_value==0) return ret;
	
}


int IRIVER_GPIO_IIC_WriteByte (struct gpio_i2c_slave_ctl *pi2c_ch_ctlint, unsigned char RegAddr, unsigned char IicData)
{
	unsigned long i;
	unsigned long ack_value = 1;
	int ret;
	
	_GPIO_IIC_EStart(pi2c_ch_ctlint);

	GPIO_IIC_ESDA_PORT_SET(pi2c_ch_ctlint,GPIO_CON_OUTPUT);		

////////////////// write chip id //////////////////
	for(i = 7; i>0; i--)
	{
		if((pi2c_ch_ctlint->slave_write_addr>> (i-1)) & 0x0001)
			_GPIO_IIC_EHigh(pi2c_ch_ctlint);
		else
			_GPIO_IIC_ELow(pi2c_ch_ctlint);
	}

	_GPIO_IIC_ELow(pi2c_ch_ctlint);	// write 'W'

	ack_value=_GPIO_IIC_EAck(pi2c_ch_ctlint);	// ACK

////////////////// write reg. addr. //////////////////
	for(i = 8; i>0; i--)
	{
		if((RegAddr >> (i-1)) & 0x0001)
			_GPIO_IIC_EHigh(pi2c_ch_ctlint);
		else
			_GPIO_IIC_ELow(pi2c_ch_ctlint);
	}

	ack_value=_GPIO_IIC_EAck(pi2c_ch_ctlint);	// ACK

////////////////// write reg. data. //////////////////
	for(i = 8; i>0; i--)
	{
		if((IicData >> (i-1)) & 0x0001)
			_GPIO_IIC_EHigh(pi2c_ch_ctlint);
		else
			_GPIO_IIC_ELow(pi2c_ch_ctlint);
	}

	ack_value=_GPIO_IIC_EAck(pi2c_ch_ctlint);	// ACK

	_GPIO_IIC_EEnd(pi2c_ch_ctlint);

	if(ack_value==0) {
		#ifdef PRINT_GPIO_I2C_ERR
		printf("i2c Wr Err :CH:%d SA:%x RegAddr:%x\n",pi2c_ch_ctlint,pi2c_ch_ctlint->slave_write_addr,RegAddr);
		#endif
		
		ret = -1;
	} else {
		ret = ack_value;
	}

	return ret;
}

int IRIVER_GPIO_IIC_WriteData (struct gpio_i2c_slave_ctl *pi2c_ch_ctlint, unsigned char *pData, int iDataSize)
{
	unsigned long i,j;
	unsigned long ack_value = 1;
	int iWriteSize = 0;
	
	_GPIO_IIC_EStart(pi2c_ch_ctlint);

	GPIO_IIC_ESDA_PORT_SET(pi2c_ch_ctlint,GPIO_CON_OUTPUT);		

////////////////// write chip id //////////////////
	for(i = 7; i>0; i--)
	{
		if((pi2c_ch_ctlint->slave_write_addr>> (i-1)) & 0x0001)
			_GPIO_IIC_EHigh(pi2c_ch_ctlint);
		else
			_GPIO_IIC_ELow(pi2c_ch_ctlint);
	}

	_GPIO_IIC_ELow(pi2c_ch_ctlint);	// write 'W'

	ack_value=_GPIO_IIC_EAck(pi2c_ch_ctlint);	// ACK

////////////////// write reg. addr. //////////////////

	for(j=0;j<iDataSize;j++) {
		
		for(i = 8; i>0; i--)
		{
			if(((*pData) >> (i-1)) & 0x0001)
				_GPIO_IIC_EHigh(pi2c_ch_ctlint);
			else
				_GPIO_IIC_ELow(pi2c_ch_ctlint);
		}

		ack_value=_GPIO_IIC_EAck(pi2c_ch_ctlint);	// ACK
		if(ack_value==1) {
			iWriteSize++;
		} else {

		}
		pData++;
	}
	
	_GPIO_IIC_EEnd(pi2c_ch_ctlint);

	if(ack_value==0) {
		#ifdef PRINT_GPIO_I2C_ERR
		printf("i2c Wr Err :CH:%d SA:%x\n",pi2c_ch_ctlint,pi2c_ch_ctlint->slave_write_addr);
		#endif
	}

	return	iWriteSize ;
}

