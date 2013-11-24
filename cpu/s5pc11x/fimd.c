/*
 * Copyright (c) 2010 Samsung Electronics Co., Ltd.
 *              http://www.samsung.com/
 *
 * S5PC110 - LCD Driver for U-Boot
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <common.h>
#include <asm/io.h>
#include <asm/types.h>

#if defined(CONFIG_MX100) /* ktj */
#include <regs.h>
extern void PMIC_lcd_on(void);
extern void PMIC_lcd_off(void);
extern void PWM_UserSelect(int duty);
extern void PWM_stop(unsigned int uNum);
#endif

#define LCD_BGCOLOR		0x1428A0

static unsigned int gFgColor = 0xFF;
static unsigned int gLeftColor = LCD_BGCOLOR;

#define Inp32(_addr)		readl(_addr)
#define Outp32(addr, data)	(*(volatile u32 *)(addr) = (data))
#define Delay(_a)		udelay(_a*1000)


#if defined(CFG_LCD_TL2796)
/*
 * TL2796
 */

#define LCD_WIDTH		480
#define LCD_HEIGHT		800

#define IIC_SDA_GPIO_ADDRESS		(0xE02000C0)	//GPB
#define IIC_SDA_GPIO_BIT			(0)		//GPB[7]
#define IIC_SCL_GPIO_ADDRESS		(0xE02000C0)	//GPB
#define IIC_SCL_GPIO_BIT			(1)		//GPB[4]
#define SDA_GPIO_ADDRESS			(0xE0200040)	//GPB
#define SDA_GPIO_BIT				(7)		//GPB[7]
#define SCL_GPIO_ADDRESS			(0xE0200040)	//GPB
#define SCL_GPIO_BIT				(4)		//GPB[4]
#define CS_GPIO_ADDRESS				(0xE0200040)	//GPB
#define CS_GPIO_BIT					(5)		//GPB[5]
#define BACKLIGHT_GPIO_ADDRESS		(0xE02000A0)	//GPD0
#define BACKLIGHT_GPIO_BIT			(3)		//GPD0[3]
#define RESET_GPIO_ADDRESS			(0xE0200C00)	//GPH0
#define RESET_GPIO_BIT				(6)		//GPH0.6
#define POWER_GPIO_ADDRESS			(0xE0200040)
#define POWER_GPIO_BIT				(6)
#define POWER_GPIO_ADDRESS2			(0xE0200040)
#define POWER_GPIO_BIT2				(7)
#define BACKLIGHT_ACTIVE 			HIGH_ACTIVE
#define RESET_ACTIVE 				LOW_ACTIVE
#define POWER_ACTIVE 				HIGH_ACTIVE

//do not modify
#define LCDM_SDA_HIGH			Outp32(SDA_GPIO_ADDRESS+4,(Inp32(SDA_GPIO_ADDRESS+4) |= (1<<SDA_GPIO_BIT)))
#define LCDM_SDA_LOW			Outp32(SDA_GPIO_ADDRESS+4,(Inp32(SDA_GPIO_ADDRESS+4) &= ~(1<<SDA_GPIO_BIT)))
#define LCDM_SCL_HIGH			Outp32(SCL_GPIO_ADDRESS+4,(Inp32(SCL_GPIO_ADDRESS+4) |= (1<<SCL_GPIO_BIT)))
#define LCDM_SCL_LOW			Outp32(SCL_GPIO_ADDRESS+4,(Inp32(SCL_GPIO_ADDRESS+4) &= ~(1<<SCL_GPIO_BIT)))
#define LCDM_CS_HIGH			Outp32(CS_GPIO_ADDRESS+4,(Inp32(CS_GPIO_ADDRESS+4) |=(1<<CS_GPIO_BIT)))
#define LCDM_CS_LOW				Outp32(CS_GPIO_ADDRESS+4,(Inp32(CS_GPIO_ADDRESS+4) &= ~(1<<CS_GPIO_BIT)))
#define LCDM_BACKLIGHT_HIGH 	Outp32(BACKLIGHT_GPIO_ADDRESS+4,(Inp32(BACKLIGHT_GPIO_ADDRESS+4) |= (1<<BACKLIGHT_GPIO_BIT)))
#define LCDM_BACKLIGHT_LOW	Outp32(BACKLIGHT_GPIO_ADDRESS+4,(Inp32(BACKLIGHT_GPIO_ADDRESS+4) &= ~(1<<BACKLIGHT_GPIO_BIT)))
#define LCDM_RESET_HIGH		 	Outp32(RESET_GPIO_ADDRESS+4,(Inp32(RESET_GPIO_ADDRESS+4) |= (1<<RESET_GPIO_BIT)))
#define LCDM_RESET_LOW			Outp32(RESET_GPIO_ADDRESS+4,(Inp32(RESET_GPIO_ADDRESS+4) &= ~(1<<RESET_GPIO_BIT)))
#define LCDM_POWER_HIGH		 	Outp32(POWER_GPIO_ADDRESS+4,(Inp32(POWER_GPIO_ADDRESS+4) |= (1<<POWER_GPIO_BIT)))
#define LCDM_POWER_LOW			Outp32(POWER_GPIO_ADDRESS+4,(Inp32(POWER_GPIO_ADDRESS+4) &= ~(1<<POWER_GPIO_BIT)))
#define LCDM_POWER2_HIGH	 	Outp32(POWER_GPIO_ADDRESS+4,(Inp32(POWER_GPIO_ADDRESS+4) |= (1<<POWER_GPIO_BIT)))
#define LCDM_POWER2_LOW	 		Outp32(POWER_GPIO_ADDRESS+4,(Inp32(POWER_GPIO_ADDRESS+4) &= ~(1<<POWER_GPIO_BIT)))
#define LCDM_IIC_SCL_HIGH		Outp32(SCL_GPIO_ADDRESS+4,(Inp32(SCL_GPIO_ADDRESS+4) |= (1<<SCL_GPIO_BIT)))
#define LCDM_IIC_SCL_LOW		Outp32(SCL_GPIO_ADDRESS+4,(Inp32(SCL_GPIO_ADDRESS+4) &= ~(1<<SCL_GPIO_BIT)))
#define LCDM_IIC_SDA_HIGH		Outp32(SDA_GPIO_ADDRESS+4,(Inp32(SDA_GPIO_ADDRESS+4) |= (1<<SDA_GPIO_BIT)))
#define LCDM_IIC_SDA_LOW		Outp32(SDA_GPIO_ADDRESS+4,(Inp32(SDA_GPIO_ADDRESS+4) &= ~(1<<SDA_GPIO_BIT)))


void Write_LDI_TL2796(u16 uAddr, u16 uData)
{
	s32 j;

	if(uAddr != 0x0000)
	{
		LCDM_CS_HIGH;
		LCDM_SCL_HIGH;
		LCDM_SDA_HIGH;

		LCDM_CS_LOW;
		LCDM_SCL_LOW;

	    // Command
	    for (j = 15 ; j >= 0; j--)
		{
			LCDM_SCL_LOW;

			if ((uAddr >> j) & 0x0001)
			{
				LCDM_SDA_HIGH;
			}
			else
			{
				LCDM_SDA_LOW;
			}
			LCDM_SCL_HIGH;
		}
	}

	if (uAddr == 0x0000)
	{
		LCDM_SDA_HIGH;
	}

	LCDM_CS_HIGH;
	LCDM_SCL_HIGH;
	LCDM_CS_LOW;
	LCDM_SCL_LOW;

	// parameter
	for (j = 15 ; j >= 0; j--)
	{
		LCDM_SCL_LOW;

		if ((uData >> j) & 0x0001)
		{
			LCDM_SDA_HIGH;
		}
		else
		{
			LCDM_SDA_LOW;
		}
		LCDM_SCL_HIGH;
	}
}

void LCDM_InitTL2796(void)
{
    Delay(50); // more than 20ms

	//[1] Panel Condition Set
	Write_LDI_TL2796(0x7031, 0x7208);
	Write_LDI_TL2796(0x7032, 0x7214);
	Write_LDI_TL2796(0x7030, 0x7202);

#if 1
	Write_LDI_TL2796(0x7027, 0x7203);
#else
	Write_LDI_TL2796(0x7027, 0x7201);
#endif

	//[2] Display Condition Set(RGB Data Interface)
	Write_LDI_TL2796(0x7012, 0x7208); //0x7208
	Write_LDI_TL2796(0x7013, 0x7208);
#if 1
	Write_LDI_TL2796(0x7015, 0x7210); //0x10
#else
	Write_LDI_TL2796(0x7015, 0x7200); //0x00
#endif
	Write_LDI_TL2796(0x7016, 0x7200);	//00 24bit 02 16bit RGB interface 
	Write_LDI_TL2796(0x70ef, 0x72d0);
	Write_LDI_TL2796(0x0000, 0x72e8);

	// set gamma control 230cd
	Write_LDI_TL2796(0x7039, 0x7244);
	Write_LDI_TL2796(0x7040, 0x7200);
	Write_LDI_TL2796(0x7041, 0x723F);
	Write_LDI_TL2796(0x7042, 0x722B);
	Write_LDI_TL2796(0x7043, 0x721f);
	Write_LDI_TL2796(0x7044, 0x7224);
	Write_LDI_TL2796(0x7045, 0x721b);
	Write_LDI_TL2796(0x7046, 0x7229);
	Write_LDI_TL2796(0x7050, 0x7200);
	Write_LDI_TL2796(0x7051, 0x7200);
	Write_LDI_TL2796(0x7052, 0x7200);
	Write_LDI_TL2796(0x7053, 0x721b);
	Write_LDI_TL2796(0x7054, 0x7222);
	Write_LDI_TL2796(0x7055, 0x721b);
	Write_LDI_TL2796(0x7056, 0x722a);
	Write_LDI_TL2796(0x7060, 0x7200);
	Write_LDI_TL2796(0x7061, 0x723F);
	Write_LDI_TL2796(0x7062, 0x7225);
	Write_LDI_TL2796(0x7063, 0x721c);
	Write_LDI_TL2796(0x7064, 0x7221);
	Write_LDI_TL2796(0x7065, 0x7218);
	Write_LDI_TL2796(0x7066, 0x723e);

	//Analog Power Condition Set
	Write_LDI_TL2796(0x7017, 0x7222);	//Boosting Freq
	Write_LDI_TL2796(0x7018, 0x7233);	//power AMP Medium
	Write_LDI_TL2796(0x7019, 0x7203);	//Gamma Amp Medium
	Write_LDI_TL2796(0x701a, 0x7201);	//Power Boosting
	Write_LDI_TL2796(0x7022, 0x72a4);	//Vinternal = 0.65*VCI
	Write_LDI_TL2796(0x7023, 0x7200);	//VLOUT1 Setting = 0.98*VCI
	Write_LDI_TL2796(0x7026, 0x72a0);	//Display Condition LTPS signal generation : Reference= DOTCLK

	//Stand-by Off Comman
	Write_LDI_TL2796(0x701d, 0x72a0);

	Delay(500); // more than 250ms

	//LCD ON
	Write_LDI_TL2796(0x7014, 0x7203);
}

void LCD_Initialize_TL2796(void)
{
	u32 uFbAddr = CFG_LCD_FBUFFER;

	u32 i,uTmpReg;
	u32* pBuffer = (u32*)uFbAddr;


	LCD_setprogress(0);


	uTmpReg = Inp32(0xE0107008);		// need to be changed later (09.01.23 WJ.Kim)	
	uTmpReg = (uTmpReg & ~(0x3)) | 0x2 ;
	Outp32(0xE0107008, uTmpReg);

	uTmpReg = Inp32(0xf800027c);
	uTmpReg = (uTmpReg & ~(0x3)) | 0 ;
	Outp32(0xf800027c,uTmpReg);


	Outp32(0xE0200120, 0x22222222);	//set GPF0 as LVD_HSYNC,VSYNC,VCLK,VDEN,VD[3:0]
	Outp32(0xE0200128,0x0);			//set pull-up,down disable
	Outp32(0xE0200140, 0x22222222);	//set GPF1 as VD[11:4]
	Outp32(0xE0200148,0x0);			//set pull-up,down disable
	Outp32(0xE0200160, 0x22222222);	//set GPF2 as VD[19:12]
	Outp32(0xE0200168,0x0);			//set pull-up,down disable
	Outp32(0xE0200180, 0x00002222);	//set GPF3 as VD[23:20]
	Outp32(0xE0200188,0x0);			//set pull-up,down disable

	Outp32(0xe02000A0, 0x11111111);	//set GPF0 as LVD_HSYNC,VSYNC,VCLK,VDEN,VD[3:0]
	Outp32(0xe02000A8,0x0);			//set pull-up,down disable
	Outp32(0xe02000A4,(1<<3));

	Outp32(0xE0200040, 0x11111111);	//set GPF0 as LVD_HSYNC,VSYNC,VCLK,VDEN,VD[3:0]
	Outp32(0xE0200048,0x0);			//set pull-up,down disable
	Outp32(0xE0200044,0);

	Outp32(0xE02000c0, 0x11111111);	//set GPF0 as LVD_HSYNC,VSYNC,VCLK,VDEN,VD[3:0]
	Outp32(0xE02000c8,0x0);			//set pull-up,down disable
	Outp32(0xE02000c4,0);

	Outp32(0xE0200C00, 0x11111111);	//set GPF0 as LVD_HSYNC,VSYNC,VCLK,VDEN,VD[3:0]
	Outp32(0xE0200C08,0x0);			//set pull-up,down disable
	Outp32(0xE0200C04,0);
	Delay(50);
	Outp32(0xE0200C04,0xffffffff);

	Outp32(SDA_GPIO_ADDRESS+4,(Inp32(SDA_GPIO_ADDRESS+4) |= (1<<SDA_GPIO_BIT)));
	Outp32(SCL_GPIO_ADDRESS+4,(Inp32(SCL_GPIO_ADDRESS+4) |= (1<<SCL_GPIO_BIT)));
	Outp32(CS_GPIO_ADDRESS+4,(Inp32(CS_GPIO_ADDRESS+4) |=(1<<CS_GPIO_BIT)));
	Outp32(SCL_GPIO_ADDRESS+4,(Inp32(SCL_GPIO_ADDRESS+4) |= (1<<SCL_GPIO_BIT)));
	Outp32(SDA_GPIO_ADDRESS+4,(Inp32(SDA_GPIO_ADDRESS+4) |= (1<<SDA_GPIO_BIT)));

	Outp32(0xf8000004, 0xf0);
	Outp32(0xf8000010, 0x50505);
	Outp32(0xf8000014, 0x50505);
	Outp32(0xf8000170, 0x0);
	Outp32(0xf8000018, 0x18f9df);
	Outp32(0xf8000000, 0x0);
	Outp32(0xf8000000, 0x190);
	Outp32(0xf8000130, 0x20);
	Outp32(0xf8000020, 0x0);
	Outp32(0xf8000024, 0x0);
	Outp32(0xf8000028, 0x0);
	Outp32(0xf800002c, 0x0);
	Outp32(0xf8000030, 0x0);
	Outp32(0xf8000034, 0x0);
	Outp32(0xf8000180, 0x0);
	Outp32(0xf8000184, 0x0);
	Outp32(0xf8000188, 0x0);
	Outp32(0xf800018c, 0x0);
	Outp32(0xf8000190, 0x0);
	Outp32(0xf8000140, 0x0);
	Outp32(0xf8000148, 0x0);
	Outp32(0xf8000150, 0x0);
	Outp32(0xf8000158, 0x0);
	Outp32(0xf8000058, 0x0);
	Outp32(0xf8000208, 0x0);
	Outp32(0xf800020c, 0x0);
	Outp32(0xf8000068, 0x0);
	Outp32(0xf8000210, 0x0);
	Outp32(0xf8000214, 0x0);
	Outp32(0xf8000078, 0x0);
	Outp32(0xf8000218, 0x0);
	Outp32(0xf800021c, 0x0);
	Outp32(0xf8000088, 0x0);
	Outp32(0xf8000220, 0x0);
	Outp32(0xf8000224, 0x0);
	Outp32(0xf8000260, 0x1);
	Outp32(0xf8000034, 0x0);
	Outp32(0xf80000a4, uFbAddr + 0x0);
	Outp32(0xf80000d4, uFbAddr + 0x177000);
	Outp32(0xf80000a0, uFbAddr + 0x0);
	Outp32(0xf80000d0, uFbAddr + 0x177000);
	Outp32(0xf80020a0, uFbAddr + 0x0);
	Outp32(0xf80020d0, uFbAddr + 0x177000);
	Outp32(0xf8000100, 0x780);
	Outp32(0xf8000020, 0x8000);
	Outp32(0xf8000020, 0x802c);
	Outp32(0xf8000040, 0x0);
	Outp32(0xf8000044, 0xefb1f);
	Outp32(0xf8000200, 0xffffff);
	Outp32(0xf8000204, 0xffffff);
	Outp32(0xf8000034, 0x400);
	Outp32(0xf8000020, 0x802d);
	Outp32(0xf8000034, 0x1);
	Outp32(0xf8000034, 0x1);
	Outp32(0xf8000034, 0x401);
	Outp32(0xf8000020, 0x802d);
	Outp32(0xf8000034, 0x1);
	Outp32(0xf8000034, 0x1);

	Outp32(0xf8000000, 0x193);

//	LCDM_InitLdi(LCD_TL2796, 0);
	LCDM_InitTL2796();
}
#elif defined(CFG_LCD_NONAME1)
/*
 *
 */

#define LCD_WIDTH		1366
#define LCD_HEIGHT		500	//766

void LCD_Initialize_NONAME1(void)
{
	u32 uFbAddr = CFG_LCD_FBUFFER;

	u32 i;
	u32* pBuffer = (u32*)uFbAddr;

	Outp32(0xe0200040, 0x10000000);
	Outp32(0xe0200048, 0x1555);
	Outp32(0xe020004c, 0xc000);
	Outp32(0xe0200040, 0x10010000);
	Outp32(0xe0200048, 0x1455);
	Outp32(0xe020004c, 0xc300);
	Outp32(0xe0200040, 0x10110000);
	Outp32(0xe0200048, 0x1055);
	Outp32(0xe020004c, 0xcf00);
	Outp32(0xe02000c0, 0x1);
	Outp32(0xe02000c8, 0x54);
	Outp32(0xe02000cc, 0x3);
	Outp32(0xe02000c0, 0x11);
	Outp32(0xe02000c8, 0x50);
	Outp32(0xe02000cc, 0xf);
	Outp32(0xe02000a0, 0x1001);
	Outp32(0xe02000a8, 0x15);
	Outp32(0xe02000ac, 0xc0);
	Outp32(0xe0200c00, 0x1000010);
	Outp32(0xe0200c08, 0x4455);
	Outp32(0xe0200c0c, 0x3000);
	Outp32(0xe0200040, 0x11110000);
	Outp32(0xe0200048, 0x55);
	Outp32(0xe020004c, 0xff00);
	Outp32(0xe0200040, 0x11110100);
	Outp32(0xe0200048, 0x55);
	Outp32(0xe020004c, 0xff00);
	Outp32(0xe0200044, 0x80);
	Outp32(0xe0200044, 0x98);
	Outp32(0xe0200044, 0xb9);
	Outp32(0xe0200044, 0xbb);
	Outp32(0xe0200044, 0xbb);
	Outp32(0xe02000a4, 0xd);
	Outp32(0xe0200c04, 0xd1);
	Outp32(0xe0200044, 0xfb);
	Outp32(0xe0200044, 0xff);
	Outp32(0xe0200c04, 0x91);
	Outp32(0xe0200c04, 0xd1);
	Outp32(0xe0200c04, 0xd3);


	Outp32(0xe0200120, 0x22222222);	//set GPF0 as LVD_HSYNC,VSYNC,VCLK,VDEN,VD[3:0]
	Outp32(0xe0200128,0x0);			//set pull-up,down disable
	Outp32(0xe0200140, 0x22222222);	//set GPF1 as VD[11:4]
	Outp32(0xe0200148,0x0);			//set pull-up,down disable
	Outp32(0xe0200160, 0x22222222);	//set GPF2 as VD[19:12]
	Outp32(0xe0200168,0x0);			//set pull-up,down disable
	Outp32(0xe0200180, 0x00002222);	//set GPF3 as VD[23:20]
	Outp32(0xe0200188,0x0);			//set pull-up,down disable

	//--------- S5PC110 EVT0 needs MAX drive strength	---------//
	Outp32(0xe020012c,0xffffffff);			//set GPF0 drive strength max by WJ.KIM(09.07.17)
	Outp32(0xe020014c,0xffffffff);			//set GPF1 drive strength max by WJ.KIM(09.07.17)
	Outp32(0xe020016c,0xffffffff);			//set GPF2 drive strength max by WJ.KIM(09.07.17)
	Outp32(0xe020018c,0x3ff);					//set GPF3 drive strength max by WJ.KIM(09.07.17)

	Outp32(0xf8000004, 0x60);
	Outp32(0xf8000010, 0xe0e0305);
	Outp32(0xf8000014, 0x3103020);
	Outp32(0xf8000170, 0x0);
	Outp32(0xf8000018, 0x17fd55);
	Outp32(0xf8000000, 0x0);
	Outp32(0xf8000000, 0x254);
	Outp32(0xf8000130, 0x20);
	Outp32(0xf8000020, 0x0);
	Outp32(0xf8000024, 0x0);
	Outp32(0xf8000028, 0x0);
	Outp32(0xf800002c, 0x0);
	Outp32(0xf8000030, 0x0);
	Outp32(0xf8000034, 0x0);
	Outp32(0xf8000180, 0x0);
	Outp32(0xf8000184, 0x0);
	Outp32(0xf8000188, 0x0);
	Outp32(0xf800018c, 0x0);
	Outp32(0xf8000190, 0x0);
	Outp32(0xf8000140, 0x0);
	Outp32(0xf8000148, 0x0);
	Outp32(0xf8000150, 0x0);
	Outp32(0xf8000158, 0x0);
	Outp32(0xf8000058, 0x0);
	Outp32(0xf8000208, 0x0);
	Outp32(0xf800020c, 0x0);
	Outp32(0xf8000068, 0x0);
	Outp32(0xf8000210, 0x0);
	Outp32(0xf8000214, 0x0);
	Outp32(0xf8000078, 0x0);
	Outp32(0xf8000218, 0x0);
	Outp32(0xf800021c, 0x0);
	Outp32(0xf8000088, 0x0);
	Outp32(0xf8000220, 0x0);
	Outp32(0xf8000224, 0x0);
	Outp32(0xf8000260, 0x1);
	Outp32(0xf8000048, 0x100200);
	Outp32(0xf8000200, 0xffffff);
	Outp32(0xf8000204, 0xffffff);
	Outp32(0xf8000034, 0x0);
	Outp32(0xf8000020, 0x802c);
	Outp32(0xf80000a0, uFbAddr + 0x00000000);
	Outp32(0xf80000d0, uFbAddr + 0x00400800);
	Outp32(0xf80000a4, uFbAddr + 0x00000000);
	Outp32(0xf80000d4, uFbAddr + 0x00400800);
	Outp32(0xf80020a0, uFbAddr + 0x00000000);
	Outp32(0xf80020d0, uFbAddr + 0x00400800);
	Outp32(0xf8000100, 0x1558);
	Outp32(0xf8000040, 0x0);
	Outp32(0xf8000044, 0x2aaaff);
	Outp32(0xf8000020, 0x802d);
	Outp32(0xf8000034, 0x1);
	Outp32(0xf8000020, 0x802d);
	Outp32(0xf8000034, 0x1);
//	Outp32(0xf8000000, 0x257);
//	Outp32(0xf8000000, 0x57); //===> MPLL should be 667 !!!!
	Outp32(0xf8000000, 0x53);
	Outp32(0xf8000010, 0x60400);
	Outp32(0xf80001a4, 0x3);

	Outp32(0xe0107008,0x2); //syscon output path
	Outp32(0xe0100204,0x700000); //syscon fimdclk = mpll

	LCD_setprogress(0);
}

#elif defined(CFG_LCD_LMS700) /* ktj */
/*
    ktj_info
    - fastboot 실행 안하면 lcd 초기화 안함
    - kernel 에서 다시 lcd 초기화 다시 함
    - [TODO] spi로 reg 설정할 경우 spi write 코드 작성할 것
*/

#define LCD_WIDTH		1024
#define LCD_HEIGHT		600	

/*
#define GPJ0CON		*(volatile unsigned long *)(0xE0200240)
#define GPJ0DAT		*(volatile unsigned long *)(0xE0200244)
#define GPJ0PUD		*(volatile unsigned long *)(0xE0200248)

#define GPJ1CON		*(volatile unsigned long *)(0xE0200260)
#define GPJ1DAT		*(volatile unsigned long *)(0xE0200264)
#define GPJ1PUD		*(volatile unsigned long *)(0xE0200268)
*/

void LCD_Initialize_LMS700(void)
{
	u32 uFbAddr = CFG_LCD_FBUFFER;
	u32 i;
	u32* pBuffer = (u32*)uFbAddr;
    u32 uFbSize = LCD_WIDTH * LCD_HEIGHT * 4;   
    
    
/*
    // gpio register
        //Outp32(0xe0200040, 0x0);
	        //Outp32(0xe0200048, 0x5555);
	            //Outp32(0xe020004c, 0x0);
	    //Outp32(0xe0200040, 0x0);
	        //Outp32(0xe0200048, 0x5555);
	            //Outp32(0xe020004c, 0x0);
	    //Outp32(0xe0200040, 0x0);
	        //Outp32(0xe0200048, 0x5555);
	            //Outp32(0xe020004c, 0x0);
	                //Outp32(0xe02000c0, 0x222222);
	                    //Outp32(0xe02000c8, 0x54);
	                        //Outp32(0xe02000cc, 0x3);
	                Outp32(0xe02000c0, 0x222222);
	                    Outp32(0xe02000c8, 0x0);
	                        Outp32(0xe02000cc, 0x0);
	                            Outp32(0xe02000a0, 0x2222);
	                                Outp32(0xe02000a8, 0x55);
	                                    Outp32(0xe02000ac, 0x0);
Outp32(0xe0200c00, 0x11100);
Outp32(0xe0200c08, 0x5505);
Outp32(0xe0200c0c, 0x0);

	    //Outp32(0xe0200040, 0x11110000);
	        //Outp32(0xe0200048, 0x55);
	            //Outp32(0xe020004c, 0xff00);
	   
	    Outp32(0xe0200040, 0x0);
	        Outp32(0xe0200048, 0x5555);
	            Outp32(0xe020004c, 0x0);
	
	//Outp32(0xe0200044, 0x80);
	//Outp32(0xe0200044, 0x98);
	//Outp32(0xe0200044, 0xb9);
	//Outp32(0xe0200044, 0xbb);
	Outp32(0xe0200044, 0x0);
	
Outp32(0xe02000a4, 0x0);
	//Outp32(0xe0200c04, 0xd1);
	//Outp32(0xe0200044, 0xfb);
Outp32(0xe0200044, 0x0);
	//Outp32(0xe0200c04, 0x91);
	//Outp32(0xe0200c04, 0xd1);
Outp32(0xe0200c04, 0x1d);
*/

	Outp32(0xe0200120, 0x22222222);	//set GPF0 as LVD_HSYNC,VSYNC,VCLK,VDEN,VD[3:0]
	Outp32(0xe0200128,0x0);			//set pull-up,down disable
	Outp32(0xe0200140, 0x22222222);	//set GPF1 as VD[11:4]
	Outp32(0xe0200148,0x0);			//set pull-up,down disable
	Outp32(0xe0200160, 0x22222222);	//set GPF2 as VD[19:12]
	Outp32(0xe0200168,0x0);			//set pull-up,down disable
	Outp32(0xe0200180, 0x2222);	    //set GPF3 as VD[23:20]
	Outp32(0xe0200188,0x0);			//set pull-up,down disable

	//--------- S5PC110 EVT0 needs MAX drive strength	---------//
	Outp32(0xe020012c,0xffffff);	//set GPF0 drive strength max by WJ.KIM(09.07.17)
	Outp32(0xe020014c,0xffffff);	//set GPF1 drive strength max by WJ.KIM(09.07.17)
	Outp32(0xe020016c,0xffffff);	//set GPF2 drive strength max by WJ.KIM(09.07.17)
	Outp32(0xe020018c,0xff);		//set GPF3 drive strength max by WJ.KIM(09.07.17)

//--------------------------------------------
	Outp32(0xf8000004, 0x60);  

    Outp32(0xf8000010, 0x10a0909);    // 0xe0e0305
    Outp32(0xf8000014, 0x13b231d);    // 0x3103020
	Outp32(0xf8000170, 0x0);
    Outp32(0xf8000018, 0x12bbff);    // 0x17fd55 , 479 799

	Outp32(0xf8000000, 0x0);
//  Outp32(0xf8000000, 0x4d7);      // 0x254
    Outp32(0xf8000000, 0x254);      // 0x254        disable video output

//  Outp32(0xf8000130, 0x1001);     // 0x20
	Outp32(0xf8000130, 0x20);       

    Outp32(0xf8000020, 0x0);     // 0x0
	    
	Outp32(0xf8000024, 0x0);
	Outp32(0xf8000028, 0x0);
	Outp32(0xf800002c, 0x0);
	Outp32(0xf8000030, 0x0);
	
//  Outp32(0xf8000034, 0x1);        // 0x0
	Outp32(0xf8000034, 0x0);
	
	Outp32(0xf8000180, 0x0);
	Outp32(0xf8000184, 0x0);
	Outp32(0xf8000188, 0x0);
	Outp32(0xf800018c, 0x0);
	Outp32(0xf8000190, 0x0);
	
	Outp32(0xf8000140, 0x0);
	Outp32(0xf8000148, 0x0);
	Outp32(0xf8000150, 0x0);
	Outp32(0xf8000158, 0x0);
	
	Outp32(0xf8000058, 0x0);
	
	Outp32(0xf8000208, 0x0);
	Outp32(0xf800020c, 0x0);
	
	Outp32(0xf8000068, 0x0);
	
	Outp32(0xf8000210, 0x0);
	Outp32(0xf8000214, 0x0);
	
	Outp32(0xf8000078, 0x0);
	
	Outp32(0xf8000218, 0x0);
	Outp32(0xf800021c, 0x0);
	
	Outp32(0xf8000088, 0x0);
	
	Outp32(0xf8000220, 0x0);
	Outp32(0xf8000224, 0x0);
	
	
//  Outp32(0xf8000260, 0x0);        // 0x1
	Outp32(0xf8000260, 0x1);
	
    Outp32(0xf8000048, 0x96000);        // 0x100200
	
//  Outp32(0xf8000200, 0x0);   // 0xffffff
//  Outp32(0xf8000204, 0x0);   // 0xffffff
    Outp32(0xf8000200, 0xffffff);   
    Outp32(0xf8000204, 0xffffff);   
	
	Outp32(0xf8000034, 0x0);        // 0
	
    Outp32(0xf8000020, 0x8035);         // 0x802c
//  Outp32(0xf8000020, 0x802c);         // 0x802c


	Outp32(0xf80000a0, uFbAddr + 0x00000000);
	Outp32(0xf80000d0, uFbAddr + uFbSize);   
	Outp32(0xf80000a4, uFbAddr + 0x00000000);
	Outp32(0xf80000d4, uFbAddr + uFbSize);
	Outp32(0xf80020a0, uFbAddr + 0x00000000);
	Outp32(0xf80020d0, uFbAddr + uFbSize);


	Outp32(0xf8000100, 0x1000);      //0x1558
	
	Outp32(0xf8000040, 0x0);
	Outp32(0xf8000044, 0x1ffa57);   //0x2aaaff
	
    Outp32(0xf8000020, 0x8035);     //0x802d ----> 이거 바꾸고 정상동작, bpp_mode sdmk 24bpp(RGB), ktj 32bpp(ARGB)
//  Outp32(0xf8000020, 0x802d);     //0x802d

	Outp32(0xf8000034, 0x1);

    Outp32(0xf8000020, 0x8035);     //0x802d
//  Outp32(0xf8000020, 0x802d);     //0x802d, Window 0 Control Register

	Outp32(0xf8000034, 0x1);        // Selects Window 0’s channel
	
//	Outp32(0xf8000000, 0x257);
//	Outp32(0xf8000000, 0x57); //===> MPLL should be 667 !!!!
	
/* 이렇게 하면 벌벌 대는것 없어짐 */
    Outp32(0xf8000000, 0x53);       // 0x53
//  Outp32(0xf8000000, 0x4d7);      // 0x53

/* 위에서 선언했음 */
//  Outp32(0xf8000010, 0x60400);    //0x60400
	
//  Outp32(0xf80001a4, 0);          //0x3, Enables I80 start trigger
    Outp32(0xf80001a4, 0x3);        //0x3, Enables I80 start trigger

/* 없으면 정상동작 안됨 */
	Outp32(0xe0107008,0x2);         //syscon output path
	Outp32(0xe0100204,0x700000);    //syscon fimdclk = mpll

#if 1
    /* LVDS power on, backlight enable */
    unsigned int tmp;

	/* blu_en output GPJ0[4]*/
	tmp = readl(GPJ0CON);
	tmp = (tmp & ~(0xf<<16))|(0x1<<16);
	writel(tmp,GPJ0CON);
	
	/* lvds_pwdn# output, GPJ1[1] */
	tmp = readl(GPJ1CON);
	tmp = (tmp & ~(0xf<<4))|(0x1<<4);
	writel(tmp,GPJ1CON);

	/* lvds_pwdn# high */
	tmp = readl(GPJ1DAT);
	tmp |= 0x1 << 1;	
	writel(tmp,GPJ1DAT);
	
	/* blu_en high */
	tmp = readl(GPJ0DAT);
	tmp |= 0x1 << 4;	
	writel(tmp,GPJ0DAT);

    /* led_pwm output, GPD0_0 */
	tmp = readl(GPD0CON);
	tmp = (tmp & ~(0xf<<0))|(0x1<<0);
	writel(tmp,GPD0CON);

	tmp = readl(GPD0DAT);
#if (MX100_HW_VER >= MX100_WS)
    /* led_pwm set high */
	tmp |= 0x1 << 0;
#else
    /* led_pwm set low */
	tmp &= ~(0x1 << 0);	
#endif
	writel(tmp,GPD0DAT);
#endif
	udelay(1000 *200);
//	LCD_setprogress(0);
}

#elif defined(CFG_LCD_LD070) /* ktj */
/*
 * LGD LD070
 */

#define LCD_WIDTH		1024
#define LCD_HEIGHT		 600

#if 0 // change lcd spi port for T/P
#define SCL_GPIO_ADDRESS			(0xE0200060)	
#define SCL_GPIO_BIT				(3)		        // GPC0[3] SPI_0_CLK
#define CS_GPIO_ADDRESS				(0xE0200C40)	
#define CS_GPIO_BIT					(6)		        // GPH2[6] SPI_0_nSS
#define SDA_GPIO_ADDRESS			(0xE0200060)	
#define SDA_GPIO_BIT				(4)		        // GPC0[4] SPI_0_MOSI
#else
#define SCL_GPIO_ADDRESS			(0xE0200040)	
#define SCL_GPIO_BIT				(0)		        // GPB[0] SPI_0_CLK
#define CS_GPIO_ADDRESS				(0xE0200040)	
#define CS_GPIO_BIT					(1)		        // GPB[1] SPI_0_nSS
#define SDA_GPIO_ADDRESS			(0xE0200040)	
#define SDA_GPIO_BIT				(3)		        // GPB[3] SPI_0_MOSI
#endif

#define LCDM_SDA_HIGH			Outp32(SDA_GPIO_ADDRESS+4,(Inp32(SDA_GPIO_ADDRESS+4) |=  (1<<SDA_GPIO_BIT)))
#define LCDM_SDA_LOW			Outp32(SDA_GPIO_ADDRESS+4,(Inp32(SDA_GPIO_ADDRESS+4) &= ~(1<<SDA_GPIO_BIT)))
#define LCDM_SCL_HIGH			Outp32(SCL_GPIO_ADDRESS+4,(Inp32(SCL_GPIO_ADDRESS+4) |=  (1<<SCL_GPIO_BIT)))
#define LCDM_SCL_LOW			Outp32(SCL_GPIO_ADDRESS+4,(Inp32(SCL_GPIO_ADDRESS+4) &= ~(1<<SCL_GPIO_BIT)))
#define LCDM_CS_HIGH			Outp32(CS_GPIO_ADDRESS+4 ,(Inp32(CS_GPIO_ADDRESS+4)  |=  (1<<CS_GPIO_BIT)))
#define LCDM_CS_LOW				Outp32(CS_GPIO_ADDRESS+4 ,(Inp32(CS_GPIO_ADDRESS+4)  &= ~(1<<CS_GPIO_BIT)))

void Write_LDI_LD070(u16 uData)
{
	s32 j;

    LCDM_CS_HIGH;   
    LCDM_SCL_HIGH;  

    LCDM_CS_LOW;   

    for (j = 15 ; j >= 0; j--)
    {
        LCDM_SCL_LOW;
    
    	if ((uData >> j) & 0x0001)
    	{
    		LCDM_SDA_HIGH;
    	}
    	else
    	{
    		LCDM_SDA_LOW;
    	}
            	
    	// data read
    	LCDM_SCL_HIGH;
    }

    LCDM_CS_HIGH;
}

void LCDM_disable_spi_LD070(void) /* ktj */
{
    LCDM_CS_LOW;
    LCDM_SDA_LOW;
    LCDM_SCL_LOW;    
}

void LCDM_Init_LD070(void)
{
    Write_LDI_LD070(0x0021);
    Write_LDI_LD070(0x00A5);
    Write_LDI_LD070(0x0430);    // cabc off
//  Write_LDI_LD070(0x04F0);    // cabc mov -> LGD issue
    Write_LDI_LD070(0x0840);
    Write_LDI_LD070(0x385F);
    Write_LDI_LD070(0x3CA4);
//  Write_LDI_LD070(0x1000);    // R4 -> lcd flashing error       
    Write_LDI_LD070(0x3400);
    Write_LDI_LD070(0x0843);
    Write_LDI_LD070(0x2828);
    Write_LDI_LD070(0x4041);
    Write_LDI_LD070(0x00AD);
}

void LCD_PowerOn(void)
{
    /* LVDS power on, backlight enable */
    unsigned int tmp;

	/* blu_en output GPJ0[4]*/
	tmp = readl(GPJ0CON);
	tmp = (tmp & ~(0xf<<16))|(0x1<<16);
	writel(tmp,GPJ0CON);
	
	/* lvds_pwdn# output, GPJ1[1] */
	tmp = readl(GPJ1CON);
	tmp = (tmp & ~(0xf<<4))|(0x1<<4);
	writel(tmp,GPJ1CON);

    /* led_pwm output, GPD0_0 */
	tmp = readl(GPD0CON);
	tmp = (tmp & ~(0xf<<0))|(0x1<<0);
	writel(tmp,GPD0CON);


    /* led_pwm */
	tmp = readl(GPD0DAT);
#if (MX100_HW_VER >= MX100_WS)
    /* led_pwm set low for brightness 0 */
	tmp &= ~(0x1 << 0);	
#else
    /* led_pwm set low */
	tmp &= ~(0x1 << 0);	
#endif
	writel(tmp,GPD0DAT);

	/* lvds_pwdn# high */
	tmp = readl(GPJ1DAT);
	tmp |= 0x1 << 1;	
	writel(tmp,GPJ1DAT);
	
	/* blu_en high */
	tmp = readl(GPJ0DAT);
	tmp |= 0x1 << 4;	
	writel(tmp,GPJ0DAT);

}

void LCD_PowerOff(void)
{
    unsigned int tmp;

    PWM_stop(0);

    /* led_pwm output, GPD0_0 */
	tmp = readl(GPD0CON);
	tmp = (tmp & ~(0xf<<0))|(0x1<<0);
	writel(tmp,GPD0CON);

    /* led_pwm off */
	tmp = readl(GPD0DAT);
#if (MX100_HW_VER >= MX100_WS)
    /* led_pwm set low */
	tmp &= ~(0x1 << 0);
#else
    /* led_pwm set high */
	tmp |= 0x1 << 0;	
#endif
	writel(tmp,GPD0DAT);

	/* blu_en low */
	tmp = readl(GPJ0DAT);
	tmp &= ~(0x1 << 4);	
	writel(tmp,GPJ0DAT);

	/* lvds_pwdn# low */
	tmp = readl(GPJ1DAT);
	tmp &= ~(0x1 << 1);	
	writel(tmp,GPJ1DAT);
}

void LCD_Initialize_LD070(void)
{
	u32 uFbAddr = CFG_LCD_FBUFFER;
	u32 i;
	u32* pBuffer = (u32*)uFbAddr;
    u32 uFbSize = LCD_WIDTH * LCD_HEIGHT * 4;   
    unsigned int tmp;

#if 0 // change lcd spi port for T/P
	tmp = readl(GPH2CON);
	tmp = (tmp & ~(0xf<<24))|(0x1<<24); // LCD_SPI_CS  :  GPH2 [6]
	writel(tmp,GPH2CON);

	tmp = readl(GPC0CON);
	tmp = (tmp & ~(0xf<<12))|(0x1<<12); // LCD_SPI_CLK :  GPC0 [3]
	tmp = (tmp & ~(0xf<<16))|(0x1<<16); // LCD_SPI_SDA :  GPC0 [4]
	writel(tmp,GPC0CON);
#else
	tmp = readl(GPBCON);
	tmp = (tmp & ~(0xf<<0))|(0x1<<0);
	tmp = (tmp & ~(0xf<<4))|(0x1<<4);
	tmp = (tmp & ~(0xf<<12))|(0x1<<12);
	writel(tmp,GPBCON);
#endif

	Outp32(SDA_GPIO_ADDRESS+4,(Inp32(SDA_GPIO_ADDRESS+4) |= (1<<SDA_GPIO_BIT)));
	Outp32(SCL_GPIO_ADDRESS+4,(Inp32(SCL_GPIO_ADDRESS+4) |= (1<<SCL_GPIO_BIT)));
	Outp32(CS_GPIO_ADDRESS+4,(Inp32(CS_GPIO_ADDRESS+4) |=(1<<CS_GPIO_BIT)));
	Outp32(SCL_GPIO_ADDRESS+4,(Inp32(SCL_GPIO_ADDRESS+4) |= (1<<SCL_GPIO_BIT)));
	Outp32(SDA_GPIO_ADDRESS+4,(Inp32(SDA_GPIO_ADDRESS+4) |= (1<<SDA_GPIO_BIT)));

    
	Outp32(0xe0200120, 0x22222222);	//set GPF0 as LVD_HSYNC,VSYNC,VCLK,VDEN,VD[3:0]
	Outp32(0xe0200128,0x0);			//set pull-up,down disable
	Outp32(0xe0200140, 0x22222222);	//set GPF1 as VD[11:4]
	Outp32(0xe0200148,0x0);			//set pull-up,down disable
	Outp32(0xe0200160, 0x22222222);	//set GPF2 as VD[19:12]
	Outp32(0xe0200168,0x0);			//set pull-up,down disable
	Outp32(0xe0200180, 0x2222);	    //set GPF3 as VD[23:20]
	Outp32(0xe0200188,0x0);			//set pull-up,down disable

	//--------- S5PC110 EVT0 needs MAX drive strength	---------//
	Outp32(0xe020012c,0xffffff);	//set GPF0 drive strength max by WJ.KIM(09.07.17)
	Outp32(0xe020014c,0xffffff);	//set GPF1 drive strength max by WJ.KIM(09.07.17)
	Outp32(0xe020016c,0xffffff);	//set GPF2 drive strength max by WJ.KIM(09.07.17)
	Outp32(0xe020018c,0xff);		//set GPF3 drive strength max by WJ.KIM(09.07.17)

//--------------------------------------------
	Outp32(0xf8000004, 0x60);  

    // VIDTCON0, VIDTCON1, timing register
#if 1
    Outp32(0xf8000010, 0x1160b09);    // 0xe0e0305
    Outp32(0xf8000014, 0x19f9f1d);    // 0x3103020
#else    
    Outp32(0xf8000010, 0x10a0909);    // 0xe0e0305
    Outp32(0xf8000014, 0x13b231d);    // 0x3103020
#endif

	Outp32(0xf8000170, 0x0);
    Outp32(0xf8000018, 0x12bbff);     // 0x17fd55 , 479 799

	Outp32(0xf8000000, 0x0);
//  Outp32(0xf8000000, 0x4d7);      // 0x254
    Outp32(0xf8000000, 0x254);      // 0x254        disable video output

//  Outp32(0xf8000130, 0x1001);     // 0x20
	Outp32(0xf8000130, 0x20);       

    Outp32(0xf8000020, 0x0);     // 0x0
	    
	Outp32(0xf8000024, 0x0);
	Outp32(0xf8000028, 0x0);
	Outp32(0xf800002c, 0x0);
	Outp32(0xf8000030, 0x0);
	
//  Outp32(0xf8000034, 0x1);        // 0x0
	Outp32(0xf8000034, 0x0);
	
	Outp32(0xf8000180, 0x0);
	Outp32(0xf8000184, 0x0);
	Outp32(0xf8000188, 0x0);
	Outp32(0xf800018c, 0x0);
	Outp32(0xf8000190, 0x0);
	
	Outp32(0xf8000140, 0x0);
	Outp32(0xf8000148, 0x0);
	Outp32(0xf8000150, 0x0);
	Outp32(0xf8000158, 0x0);
	
	Outp32(0xf8000058, 0x0);
	
	Outp32(0xf8000208, 0x0);
	Outp32(0xf800020c, 0x0);
	
	Outp32(0xf8000068, 0x0);
	
	Outp32(0xf8000210, 0x0);
	Outp32(0xf8000214, 0x0);
	
	Outp32(0xf8000078, 0x0);
	
	Outp32(0xf8000218, 0x0);
	Outp32(0xf800021c, 0x0);
	
	Outp32(0xf8000088, 0x0);
	
	Outp32(0xf8000220, 0x0);
	Outp32(0xf8000224, 0x0);
	
	
//  Outp32(0xf8000260, 0x0);        // 0x1
	Outp32(0xf8000260, 0x1);
	
    Outp32(0xf8000048, 0x96000);        // 0x100200
	
//  Outp32(0xf8000200, 0x0);   // 0xffffff
//  Outp32(0xf8000204, 0x0);   // 0xffffff
    Outp32(0xf8000200, 0xffffff);   
    Outp32(0xf8000204, 0xffffff);   
	
	Outp32(0xf8000034, 0x0);        // 0
	
    Outp32(0xf8000020, 0x8035);         // 0x802c
//  Outp32(0xf8000020, 0x802c);         // 0x802c


	Outp32(0xf80000a0, uFbAddr + 0x00000000);
	Outp32(0xf80000d0, uFbAddr + uFbSize);   
	Outp32(0xf80000a4, uFbAddr + 0x00000000);
	Outp32(0xf80000d4, uFbAddr + uFbSize);
	Outp32(0xf80020a0, uFbAddr + 0x00000000);
	Outp32(0xf80020d0, uFbAddr + uFbSize);


	Outp32(0xf8000100, 0x1000);      //0x1558
	
	Outp32(0xf8000040, 0x0);
	Outp32(0xf8000044, 0x1ffa57);   //0x2aaaff
	
    Outp32(0xf8000020, 0x8035);     //0x802d ----> 이거 바꾸고 정상동작, bpp_mode sdmk 24bpp(RGB), ktj 32bpp(ARGB)
//  Outp32(0xf8000020, 0x802d);     //0x802d

	Outp32(0xf8000034, 0x1);

    Outp32(0xf8000020, 0x8035);     //0x802d
//  Outp32(0xf8000020, 0x802d);     //0x802d, Window 0 Control Register

	Outp32(0xf8000034, 0x1);        // Selects Window 0’s channel
	
//	Outp32(0xf8000000, 0x257);
//	Outp32(0xf8000000, 0x57); //===> MPLL should be 667 !!!!
	
/* 이렇게 하면 벌벌 대는것 없어짐 */
    Outp32(0xf8000000, 0x93);       // set vclk 55MHz       
//  Outp32(0xf8000000, 0x97);       
//  Outp32(0xf8000000, 0x53);       // 0x53
//  Outp32(0xf8000000, 0x4d7);      // 0x53

/* 위에서 선언했음 */
//  Outp32(0xf8000010, 0x60400);    //0x60400
	
//  Outp32(0xf80001a4, 0);          //0x3, Enables I80 start trigger
    Outp32(0xf80001a4, 0x3);        //0x3, Enables I80 start trigger

/* 없으면 정상동작 안됨 */
	Outp32(0xe0107008,0x2);         //syscon output path
	Outp32(0xe0100204,0x700000);    //syscon fimdclk = mpll

/* yoon */
#if defined(CONFIG_MX100)
    PWM_UserSelect(0);             /* ktj 80 -> 60 -> 40 -> 30 -> 20 */
#endif

    LCD_PowerOn();
  Delay(200); // ms         

#if defined(CONFIG_MX100)
    PWM_UserSelect(10);             /* ktj 80 -> 60 -> 40 -> 30 -> 20 */
#endif

    LCDM_Init_LD070();
//  Delay(100); // ms         
    Delay(10); // ms         

/// LCDM_disable_spi_LD070(); /* ktj */

//	LCD_setprogress(0);
}

#else
// "No LCD Type is defined!"
#endif

/*
 */

static int gLCDOn = 0;

void LCD_turnon(void)
{
    int i;
	
    if(gLCDOn == 1)
        return;

    printf("\nLCD_turnon\n"); 		/* ktj */

#if defined(CONFIG_MX100)
    PMIC_lcd_on();
//  Delay(100); // ms 
    Delay(10); // ms 
#endif

#if defined(CFG_LCD_TL2796)
	LCD_Initialize_TL2796();
#elif defined(CFG_LCD_NONAME1)
	LCD_Initialize_NONAME1();
#elif defined(CFG_LCD_LMS700)
	LCD_Initialize_LMS700();        /* ktj */
#elif defined(CFG_LCD_LD070)
	LCD_Initialize_LD070();         /* ktj */
#endif
    
#if defined(CONFIG_MX100)
  Delay(200); // ms 
    PWM_UserSelect(20);             /* ktj 80 -> 60 -> 40 -> 30 -> 20 */
#endif

   if(get_cur_soc() <6) {

   } else {
	   for(i=20;i<=80;i+=10)  {  /* 2011.05.23 req by cto  */
	   	PWM_UserSelect(i);
		udelay(200);
	   }
   }
   gLCDOn = 1;
}

/* ktj 2010-12-28 */
void LCD_turnoff(void)
{
    if(gLCDOn == 0)
        return;

    printf("\nLCD_turnoff\n"); 		/* ktj */

    LCD_PowerOff();   

#if defined(CONFIG_MX100)
    PMIC_lcd_off();
#endif

    gLCDOn = 0;
}

/* JHLIM 2011.05.04 */
int is_lcd_on(void)
{
	return gLCDOn;
}
void LCD_setfgcolor(unsigned int color)
{
	gFgColor = color;
}

void LCD_setleftcolor(unsigned int color)
{
	gLeftColor = color;
}

void LCD_setprogress(int percentage)
{
#if defined(CFG_LCD_TL2796) || defined(CFG_LCD_NONAME1) || defined(CFG_LCD_LMS700) || defined(CFG_LCD_LD070)
	u32 i, j;
	u32* pBuffer = (u32*)CFG_LCD_FBUFFER;

	for (i=0; i < (LCD_HEIGHT/100)*percentage; i++)
	{
		for (j=0; j < LCD_WIDTH; j++)
		{
			*pBuffer++ = gFgColor;
		}
	}

	for (; i < LCD_HEIGHT; i++)
	{
		for (j=0; j < (LCD_WIDTH >> 5); j++)
		{
			*pBuffer++ = gLeftColor;
		}
		for (; j < LCD_WIDTH; j++)
		{
			*pBuffer++ = LCD_BGCOLOR;
		}
	}
#endif
}

/* ktj make boot command */
/////////////////////////////////////////////////////////////////
int do_lcd(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	if ( argc != 3 )
	{
		printf("Usage: lcd <cmd> <parameter>\n");
		return 0;
	}

	if ( strcmp(argv[1], "turn") == 0 )
    {
        if ( strcmp(argv[2], "on") == 0 )
	        LCD_turnon();
        else if ( strcmp(argv[2], "off") == 0 )
	        LCD_turnoff();
	    
	    return 0;
    }
	else if ( strcmp(argv[1], "backlight") == 0 )
	{
		return 0;
	}
	
	printf("Usage: lcd <cmd> <parameter>\n");
	return 0;
}

U_BOOT_CMD(
        lcd, 3, 0, do_lcd,
        "fdisk\t- fdisk for sd/mmc.\n",
        "-c <device_num>\t- create partition.\n"
        "fdisk -p <device_num>\t- print partition information\n"
);

