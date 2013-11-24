/*
 * Copyright (c) 2011 iriver Co., Ltd.
 *              http://www.iriver.co.kr/
 * ktj
 * S5PC110 - Timer Driver for U-Boot
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include "mx100_inc.h"

#define Outp32(addr, data)	(*(volatile u32 *)(addr) = (data))
#define Outp16(addr, data)	(*(volatile u16 *)(addr) = (data))
#define Outp8(addr, data)	(*(volatile u8 *)(addr) = (data))
#define Inp32(addr)			(*(volatile u32 *)(addr))
#define Inp32_(addr, data) (data = (*(volatile u32 *)(addr))) //jhy20090326 for code merge

volatile u32 gTimer_uPclkPsys;

void PWM_stop(u32 uNum)
{
	u32 uTemp;

	uTemp = Inp32(rTCON);

	if(uNum == 0)
			uTemp &= ~(0x1);
	else
			uTemp &= ~((0x10)<<(uNum*4));
		
	Outp32(rTCON,uTemp);
	
	printf("PWM_stop %d\n", uNum);
}
static int g_lcd_duty;
/* JHLIM */
int get_PWM_duty(void)
{
	return g_lcd_duty;
}
void PWM_UserSelect(int duty)
{
	s32	sTimer;
	s32	sPrescaler;
	s32  sDivider;
	s32  sEnDz;
	s32  sDzlen;
	s32  sAutoreload;
	s32  sEnInverter;
	s32 	sEnInt;
	s32	sEnDMA;
	s32  sTCNTB;
	s32  sTCMPB;

	float fTimerclk;
	float fTimerfreq;
	u32	uTemp0;
	u32	uTemp1;
	u32	uTemp2;
	u32  uTemp3;
	s32  sContinue;
	u32 uOpClk;

	
    uOpClk = gTimer_uPclkPsys;			

    g_lcd_duty = duty;
	
//--------------------select timer -------------------------	
    sTimer = 0;
	GPIO_SetFunctionEach(eGPIO_D0,eGPIO_0,eGFunc_0);		

//--------------------select prescaler-----------------------	
//  Select prescaler value [1~256]
    sPrescaler = 28;   // 23.59KHz

//--------------------select dividerMUX---------------------	
//	0. 1/1     1. 1/2     2. 1/4     3. 1/8     4. 1/16    5. externalTCLK1
    sDivider = 0;

//--------------------select Deadzone----------------------
//	0.Disable     1.Enable
    sEnDz = 0;

//--------------------select Auto-reload---------------------
//	0. One-shot     1. Auto-reload
	sAutoreload = 1;

//--------------------select Inverter enable------------------
//	0. Disable     1. Enblae
    sEnInverter = 1;

//--------------------select interrupt enable------------------
//	0. Disable     1. Enblae
    sEnInt = 0;

//--------------------select DMA enable---------------------
//	0. Disable     1. Enblae
    sEnDMA = 0;

//--------------------select TCNTB, TCMPB -----------------------	
    sTCNTB = 100;
#if (MX100_HW_VER >= MX100_WS)
    sTCMPB = sTCNTB - duty;
#else
    sTCMPB = duty;
#endif

//-----------------------Set SFRs--------------------------
	
    uTemp0 = Inp32(rTCFG1);
    uTemp0 = (uTemp0 & (~(0xf<<4*sTimer))& (~(1<<20))) |(sEnDMA<<20)|(sDivider<<4*sTimer);
    Outp32(rTCFG1,uTemp0);
    
    uTemp0 = Inp32(rTINT_CSTAT);
    uTemp0 = (uTemp0 & (~(1<<sTimer)))|(sEnInt<<(sTimer));
    Outp32(rTINT_CSTAT,uTemp0);
    
    switch(sTimer)
    {
    	case 0:
    		uTemp0 = Inp32(rTCON);
    		uTemp0 = uTemp0 & (0xfffffffe);
    		Outp32(rTCON, uTemp0); 								// Timer0 stop
    
    		uTemp0 = Inp32(rTCFG0);
    		uTemp0 = (uTemp0 & (~(0xff00ff))) | ((sPrescaler-1)<<0) |(sDzlen<<16);
    		Outp32(rTCFG0, uTemp0);
    		
    		Outp32(rTCNTB0, sTCNTB);
    		Outp32(rTCMPB0, sTCMPB);
    
    		uTemp1 = Inp32(rTCON);
    		uTemp1 = (uTemp1 & (~(0x1f))) |(sEnDz<<4)|(sAutoreload<<3)|(sEnInverter<<2)|(1<<1)|(0<<0);
    		Outp32(rTCON, uTemp1);									//timer0 manual update
    		uTemp1 = (uTemp1 & (~(0x1f))) |(sEnDz<<4)|(sAutoreload<<3)|(sEnInverter<<2)|(0<<1)|(1<<0);				
    		Outp32(rTCON, uTemp1);									// timer0 start
    
    		break;
    	case 1:
    		uTemp0 = Inp32(rTCON);
    		uTemp0 = uTemp0 & (0xfffffeff);
    		Outp32(rTCON, uTemp0); 								// Timer1 stop
    
    		uTemp0 = Inp32(rTCFG0);
    		uTemp0 = (uTemp0 & (~(0xff00ff))) | ((sPrescaler-1)<<0) |(sDzlen<<16);
    		Outp32(rTCFG0, uTemp0);
    		
    		Outp32(rTCNTB1, sTCNTB);
    		Outp32(rTCMPB1, sTCMPB);
    
    		uTemp1 = Inp32(rTCON);
    		uTemp1 = (uTemp1 & (~(0xf00))) | (sAutoreload<<11)|(sEnInverter<<10)|(1<<9)|(0<<8);
    		Outp32(rTCON,uTemp1); 									// timer1 manual update
    		uTemp1 = (uTemp1 & (~(0xf00))) | (sAutoreload<<11)|(sEnInverter<<10)|(0<<9)|(1<<8);				
    		Outp32(rTCON,uTemp1); 									// timer1 start
    
    
    		break;
    			
    	case 2:
    		uTemp0 = Inp32(rTCON);
    		uTemp0 = uTemp0 & (0xffffefff);
    		Outp32(rTCON, uTemp0); 								// Timer2 stop
    
    		uTemp2 = Inp32(rTCFG0);
    		uTemp2 = (uTemp2 & (~(0xffff00)))|((sPrescaler-1)<<8)|(sDzlen<<16);
    		Outp32(rTCFG0, uTemp2);
    		
    		Outp32(rTCNTB2, sTCNTB);
    		Outp32(rTCMPB2, sTCMPB);
    
    		uTemp3 = Inp32(rTCON);
    		uTemp3 = (uTemp3 & (~(0xf000)))|(sAutoreload<<15)|(sEnInverter<<14)|(1<<13)|(0<<12);
    		Outp32(rTCON, uTemp3); 								// timer2 manual update
    
    		uTemp3 = (uTemp3 & (~(0xf000)))|(sAutoreload<<15)|(sEnInverter<<14)|(0<<13)|(1<<12);
    		Outp32(rTCON,uTemp3);									 // timer2 start
    
    		break;
    		
    	case 3:
    		uTemp0 = Inp32(rTCON);
    		uTemp0 = uTemp0 & (0xfffeffff);
    		Outp32(rTCON, uTemp0);									 // Timer3 stop
    
    		uTemp2 = Inp32(rTCFG0);
    		uTemp2 = (uTemp2 & (~(0xffff00)))|((sPrescaler-1)<<8)|(sDzlen<<16);
    		Outp32(rTCFG0, uTemp2);
    		
    		Outp32(rTCNTB3, sTCNTB);
    		Outp32(rTCMPB3, sTCMPB);
    
    		uTemp3 = Inp32(rTCON);
    		uTemp3 = (uTemp3 &(~(0xf0000)))|(sAutoreload<<19)|(sEnInverter<<18)|(1<<17)|(0<<16);
    		Outp32(rTCON, uTemp3); 								// timer3 manual update
    		uTemp3 = (uTemp3 &(~(0xf0000)))|(sAutoreload<<19)|(sEnInverter<<18)|(0<<17)|(1<<16);
    		Outp32(rTCON,uTemp3); 									// timer3 start
    
    		break;
    
    	case 4:
    		uTemp0 = Inp32(rTCON);
    		uTemp0 = uTemp0 & (0xffefffff);
    		Outp32(rTCON, uTemp0);									 // Timer4 stop
    		
    		uTemp2 = Inp32(rTCFG0);
    		uTemp2 = (uTemp2 & (~(0xffff00)))|((sPrescaler-1)<<8)|(sDzlen<<16);
    		Outp32(rTCFG0, uTemp2);
    		
    		Outp32(rTCNTB4, sTCNTB);
    
    		uTemp3 = Inp32(rTCON);
    		uTemp3 = (uTemp3 & (~(0xf00000)))|(sAutoreload<<22)|(1<<21)|(0<<20);
    		Outp32(rTCON,uTemp3); 									// timer4 manual update
    
    		uTemp3 = (uTemp3 & (~(0xf00000)))|(sAutoreload<<22)|(0<<21)|(1<<20);				
    		Outp32(rTCON, uTemp3);									 // timer4 start
    
    		break;
    }

	if (sDivider == 5)		// Using External TCLK
	{
		fTimerclk = (1/(float)uOpClk)*sTCNTB*1000;			
		fTimerfreq = 1/fTimerclk;
	}
	else
	{
		fTimerclk = (1/(float)((float)uOpClk/((float)sPrescaler)/(1<<(sDivider))))*sTCNTB*1000;
		fTimerfreq = 1/fTimerclk;
	}
}

int atoi(char *string)
{
	int length;
	int retval = 0;
	int i;
	int sign = 1;

	length = strlen(string);
	for (i = 0; i < length; i++) {
		if (0 == i && string[0] == '-') {
			sign = -1;
			continue;
		}
		if (string[i] > '9' || string[i] < '0') {
			break;
		}
		retval *= 10;
		retval += string[i] - '0';
	}
	retval *= sign;
	return retval;
}

int do_backlight(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	if ( argc != 2 )
	{
		printf("Usage: backlight <duty>\n");
		return 0;
	}

    if (atoi(argv[1]) == 100)
        PWM_stop(0);
    else
        PWM_UserSelect(atoi(argv[1]));

	return 0;
}

U_BOOT_CMD(
        backlight, 2, 0, do_backlight,
        "fdisk\t- fdisk for sd/mmc.\n",
        "-c <device_num>\t- create partition.\n"
        "fdisk -p <device_num>\t- print partition information\n"
);

/* end of file */
