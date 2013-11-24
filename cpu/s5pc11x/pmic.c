#include <config.h>
#include <pmic.h>
#include <common.h> /* ktj */
#include <asm/io.h>

void Delay(void)
{
	unsigned long i,j;
	for(i=0;i<DELAY;i++);
}

void SCLH_SDAH()
{
	IIC_ESCL_Hi;
	IIC_ESDA_Hi;
	Delay();
}

void SCLH_SDAL()
{
	IIC_ESCL_Hi;
	IIC_ESDA_Lo;
	Delay();
}

void SCLL_SDAH()
{
	IIC_ESCL_Lo;
	IIC_ESDA_Hi;
	Delay();
}

void SCLL_SDAL()
{
	IIC_ESCL_Lo;
	IIC_ESDA_Lo;
	Delay();
}

void IIC_ELow()
{
	SCLL_SDAL();
	SCLH_SDAL();
	SCLH_SDAL();
	SCLL_SDAL();
}

void IIC_EHigh()
{
	SCLL_SDAH();
	SCLH_SDAH();
	SCLH_SDAH();
	SCLL_SDAH();
}

void IIC_EStart()
{
	SCLH_SDAH();
	SCLH_SDAL();
	Delay();
	SCLL_SDAL();
}

void IIC_EEnd()
{
	SCLL_SDAL();
	SCLH_SDAL();
	Delay();
	SCLH_SDAH();
}

void IIC_EAck()
{
	unsigned long ack;

	IIC_ESDA_INP;			// Function <- Input

	IIC_ESCL_Lo;
	Delay();
	IIC_ESCL_Hi;
	Delay();
	ack = GPD1DAT;
	IIC_ESCL_Hi;
	Delay();
	IIC_ESCL_Hi;
	Delay();

	IIC_ESDA_OUTP;			// Function <- Output (SDA)

	ack = (ack>>4)&0x1;
	while(ack!=0);

	SCLL_SDAL();
}

void IIC_ESetport(void)
{
	GPD1PUD &= ~(0xf<<8);	// Pull Up/Down Disable	SCL, SDA

	IIC_ESCL_Hi;
	IIC_ESDA_Hi;

	IIC_ESCL_OUTP;		// Function <- Output (SCL)
	IIC_ESDA_OUTP;		// Function <- Output (SDA)

	Delay();
}

void IIC_EWrite (unsigned char ChipId, unsigned char IicAddr, unsigned char IicData)
{
	unsigned long i;

	IIC_EStart();

////////////////// write chip id //////////////////
	for(i = 7; i>0; i--)
	{
		if((ChipId >> (i-1)) & 0x0001)
			IIC_EHigh();
		else
			IIC_ELow();
	}

	IIC_ELow();	// write 'W'

	IIC_EAck();	// ACK

////////////////// write reg. addr. //////////////////
	for(i = 8; i>0; i--)
	{
		if((IicAddr >> (i-1)) & 0x0001)
			IIC_EHigh();
		else
			IIC_ELow();
	}

	IIC_EAck();	// ACK

////////////////// write reg. data. //////////////////
	for(i = 8; i>0; i--)
	{
		if((IicData >> (i-1)) & 0x0001)
			IIC_EHigh();
		else
			IIC_ELow();
	}

	IIC_EAck();	// ACK

	IIC_EEnd();
}

void print_pmicinfo(void)
{
#if 0 
	printf("--- PMIC Vtg Info ---\n");
	printf("LDO2 : 1.10V \n");
	printf("LDO3 : 1.10V \n");
	printf("LDO4 : 3.30V \n");
	printf("LDO5 : 2.80V \n");
	printf("LDO6 : 3.30V \n");
	printf("LDO7 : 1.80V \n");
	printf("LDO8 : 3.30V \n");
	printf("LDO9 : 3.00V \n");
	printf("---------------------\n");
#endif
}

/*-------------------------------------------------------
 Buck1  :   DVFS : ARM 전원 (default 1.2V)						
 Buck2  :   DVFS : INT 전원 (default 1.1V)			
 Buck3	:   1.8V : Memory 전원						
 LDO1   :   3.0V : RTC전원						
 LDO2	:   1.1V : ALIVE 전원  		
*LDO3	:   1.1V : CPU USB OTG digital core 전원		
*LDO4	:   3.3V : LGD 7'' LCD 모듈 전원		
*LDO5	:   2.8V : camera & CPU camera 도메인 전원  		
*LDO6   :	3.3V : CPU LCD 도메인 및 HDMI OSC 전원		
*LDO7	:	1.8V : camera core 전원	
*LDO8	:	3.3V : CPU USB OTG analog IO 전원	
 LDO9	:	3.0V : CPU 및 device ADC, 오디오, SDcard	
 
 LCD -> LDO4, 6
-------------------------------------------------------*/

void PMIC_InitIp(void)
{
	IIC_ESetport();

#if defined(CHARGE_ADAPTOR_1A) /* ktj */
    IIC_EWrite(MAX8698_ADDR, 0x00, 0x38);   // LDO4, LDO5 Off
    IIC_EWrite(MAX8698_ADDR, 0x01, 0x31);   // LDO6, LDO7 Off, battery monitor on, active low
#else
    IIC_EWrite(MAX8698_ADDR, 0x00, 0x30);   // LDO3, LDO4, LDO5 Off
    IIC_EWrite(MAX8698_ADDR, 0x01, 0x11);   // LDO6, LDO7, LDO8 Off, battery monitor on, active low
#endif

	// Buck1(VCC_ARM)
#if defined(CONFIG_CLK_1200_200_166_133)
	IIC_EWrite(MAX8698_ADDR, 0x04, 0xBB);	// DVSARM2 = 1.30V, DVSARM1 = 1.30V
	IIC_EWrite(MAX8698_ADDR, 0x05, 0xBB);	// DVSARM4 = 1.30V, DVSARM3 = 1.30V
#elif defined(CONFIG_CLK_1000_200_166_133)
	IIC_EWrite(MAX8698_ADDR, 0x04, 0xAA);	// DVSARM2 = 1.25V, DVSARM1 = 1.25V
	IIC_EWrite(MAX8698_ADDR, 0x05, 0xAA);	// DVSARM4 = 1.25V, DVSARM3 = 1.25V
#else
    #if defined(CHANGE_SYS_CLK) /* ktj */    
	IIC_EWrite(MAX8698_ADDR, 0x04, 0x44);	// 0.95V
	IIC_EWrite(MAX8698_ADDR, 0x05, 0x44);	// 0.95V
    #else
	IIC_EWrite(MAX8698_ADDR, 0x04, 0x99);	// DVSARM2 = 1.20V, DVSARM1 = 1.20V
	IIC_EWrite(MAX8698_ADDR, 0x05, 0x99);	// DVSARM4 = 1.20V, DVSARM3 = 1.20V
    #endif
#endif

	// Buck2(vcc_internal)
#if defined(CONFIG_CLK_1200_200_166_133)
	IIC_EWrite(MAX8698_ADDR, 0x06, 0x88);	// DVSINT2 = 1.15V, DVSINT1 = 1.15V
#else
	IIC_EWrite(MAX8698_ADDR, 0x06, 0x77);	// DVSINT2 = 1.10V, DVSINT1 = 1.10V
#endif

	IIC_EWrite(MAX8698_ADDR, 0x07, 0x02);   // Buck3(vcc_mem) = 1.8V

	IIC_EWrite(MAX8698_ADDR, 0x08, 0x66);   // LDO3 = 1.1V, LDO2(vcc_alive) = 1.1V 
	
	IIC_EWrite(MAX8698_ADDR, 0x09, 0x11);   // LDO4 = 3.3V
	
	IIC_EWrite(MAX8698_ADDR, 0x0A, 0x0C);   // LDO5 = 2.8V
	
	IIC_EWrite(MAX8698_ADDR, 0x0B, 0x11);   // LDO6 = 3.3V
	
	IIC_EWrite(MAX8698_ADDR, 0x0C, 0x02);   // LDO7 = 1.8V
	
	IIC_EWrite(MAX8698_ADDR, 0x0D, 0x33);   // LDO8 = 3.3V, Backup Charger(vcc_coin) = 3.2V
	
	IIC_EWrite(MAX8698_ADDR, 0x0E, 0x0E);   // LDO9 = 3.0V

	IIC_EWrite(MAX8698_ADDR, 0x0F, 0x1C);   // Low battery 3.4V
}

void PMIC_dvsarm(void)
{
	IIC_EWrite(MAX8698_ADDR, 0x04, 0x99);	// DVSARM2 = 1.20V, DVSARM1 = 1.20V
	IIC_EWrite(MAX8698_ADDR, 0x05, 0x99);	// DVSARM4 = 1.20V, DVSARM3 = 1.20V
}

void PMIC_lcd_on(void)
{
    IIC_EWrite(MAX8698_ADDR, 0x00, 0x3C);   // LDO4 On
    IIC_EWrite(MAX8698_ADDR, 0x01, 0xB1);   // LDO6 On
}

void PMIC_lcd_off(void)
{
#if defined(CHARGE_ADAPTOR_1A) /* ktj */
    IIC_EWrite(MAX8698_ADDR, 0x00, 0x38);   // LDO4, LDO5 Off
    IIC_EWrite(MAX8698_ADDR, 0x01, 0x31);   // LDO6, LDO7 Off
#else
    IIC_EWrite(MAX8698_ADDR, 0x00, 0x30);   // LDO3, LDO4, LDO5 Off
    IIC_EWrite(MAX8698_ADDR, 0x01, 0x11);   // LDO6, LDO7, LDO8 Off
#endif
}

/* end of file */