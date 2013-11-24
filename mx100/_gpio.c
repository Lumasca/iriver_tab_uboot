/**************************************************************************************
* 
* 
*	Project Name : S5PC110 Validation
*
*	Copyright 2009 by Samsung Electronics, Inc.
*	All rights reserved.
*
*	Project Description :
*		This software is only for validating functions of the S5PC110.
*		Anybody can use this software without our permission.
*  
*--------------------------------------------------------------------------------------
* 
*	File Name : gpio.c
*  
*	File Description : This file declares prototypes of GPIO API funcions.
*
*	Author : chansik.Jeon
*	Dept. : AP Development Team
*	Created Date : 2009/02/07
*	Version : 0.1 
* 
*	History
*	- Created(chansik.Jeon 2009/02/07)

*  
**************************************************************************************/

//#include "option.h"
#include "smdkc110_library.h"
#include "c110_sfr.h"
//#include "system.h"
//#include "intc.h"
#include "_gpio.h"

#define GPIO 		( ( volatile oGPIO_REGS * ) (GPIO_pBase) )


typedef struct tag_GPIO_REGS
{
	unsigned int rGPIOA0CON;			//0xE0200000
	unsigned int rGPIOA0DAT;
	unsigned int rGPIOA0PUD;
	unsigned int rGPIOA0DRV_SR;
	unsigned int rGPIOA0CONPDN;
	unsigned int rGPIOA0PUDPDN;
	unsigned int reservedA0[2];

	unsigned int rGPIOA1CON;			//0xE0200020
	unsigned int rGPIOA1DAT;
	unsigned int rGPIOA1PUD;
	unsigned int rGPIOA1DRV_SR;
	unsigned int rGPIOA1CONPDN;
	unsigned int rGPIOA1PUDPDN;
	unsigned int reservedA1[2];
	
	unsigned int rGPIOBCON;			//0xE0200040
	unsigned int rGPIOBDAT;
	unsigned int rGPIOBPUD;
	unsigned int rGPIOBDRV_SR;
	unsigned int rGPIOBCONPDN;
	unsigned int rGPIOBPUDPDN;	
	unsigned int reservedB[2];

	unsigned int rGPIOC0CON;			//0xE0200060
	unsigned int rGPIOC0DAT;
	unsigned int rGPIOC0PUD;
	unsigned int rGPIOC0DRV_SR;
	unsigned int rGPIOC0CONPDN;
	unsigned int rGPIOC0PUDPDN;
	unsigned int reservedC0[2];

	unsigned int rGPIOC1CON;			//0xE0200080
	unsigned int rGPIOC1DAT;
	unsigned int rGPIOC1PUD;
	unsigned int rGPIOC1DRV_SR;
	unsigned int rGPIOC1CONPDN;
	unsigned int rGPIOC1PUDPDN;
	unsigned int reservedC1[2];

	unsigned int rGPIOD0CON;			//0xE02000A0
	unsigned int rGPIOD0DAT;
	unsigned int rGPIOD0PUD;
	unsigned int rGPIOD0DRV_SR;
	unsigned int rGPIOD0CONPDN;
	unsigned int rGPIOD0PUDPDN;
	unsigned int reservedD0[2];

	unsigned int rGPIOD1CON;			//0xE02000C0
	unsigned int rGPIOD1DAT;
	unsigned int rGPIOD1PUD;
	unsigned int rGPIOD1DRV_SR;
	unsigned int rGPIOD1CONPDN;
	unsigned int rGPIOD1PUDPDN;
	unsigned int reservedD1[2];

	unsigned int rGPIOE0CON;			//0xE02000E0
	unsigned int rGPIOE0DAT;
	unsigned int rGPIOE0PUD;
	unsigned int rGPIOE0DRV_SR;
	unsigned int rGPIOE0CONPDN;
	unsigned int rGPIOE0PUDPDN;
	unsigned int reservedE0[2];

	unsigned int rGPIOE1CON;			//0xE0200100
	unsigned int rGPIOE1DAT;
	unsigned int rGPIOE1PUD;
	unsigned int rGPIOE1DRV_SR;
	unsigned int rGPIOE1CONPDN;
	unsigned int rGPIOE1PUDPDN;
	unsigned int reservedE1[2];

	unsigned int rGPIOF0CON;			//0xE0200120
	unsigned int rGPIOF0DAT;
	unsigned int rGPIOF0PUD;
	unsigned int rGPIOF0DRV_SR;
	unsigned int rGPIOF0CONPDN;
	unsigned int rGPIOF0PUDPDN;
	unsigned int reservedF0[2];

	unsigned int rGPIOF1CON;			//0xE0200140
	unsigned int rGPIOF1DAT;
	unsigned int rGPIOF1PUD;
	unsigned int rGPIOF1DRV_SR;
	unsigned int rGPIOF1CONPDN;
	unsigned int rGPIOF1PUDPDN;
	unsigned int reservedF1[2];

	unsigned int rGPIOF2CON;			//0xE0200160
	unsigned int rGPIOF2DAT;
	unsigned int rGPIOF2PUD;
	unsigned int rGPIOF2DRV_SR;
	unsigned int rGPIOF2CONPDN;
	unsigned int rGPIOF2PUDPDN;
	unsigned int reservedF2[2];

	unsigned int rGPIOF3CON;			//0xE0200180
	unsigned int rGPIOF3DAT;
	unsigned int rGPIOF3PUD;
	unsigned int rGPIOF3DRV_SR;
	unsigned int rGPIOF3CONPDN;
	unsigned int rGPIOF3PUDPDN;
	unsigned int reservedF3[2];

	unsigned int rGPIOG0CON;			//0xE02001A0
	unsigned int rGPIOG0DAT;
	unsigned int rGPIOG0PUD;
	unsigned int rGPIOG0DRV_SR;
	unsigned int rGPIOG0CONPDN;
	unsigned int rGPIOG0PUDPDN;
	unsigned int reservedG0[2];

	unsigned int rGPIOG1CON;			//0xE02001C0
	unsigned int rGPIOG1DAT;
	unsigned int rGPIOG1PUD;
	unsigned int rGPIOG1DRV_SR;
	unsigned int rGPIOG1CONPDN;
	unsigned int rGPIOG1PUDPDN;
	unsigned int reservedG1[2];

	unsigned int rGPIOG2CON;			//0xE02001E0
	unsigned int rGPIOG2DAT;
	unsigned int rGPIOG2PUD;
	unsigned int rGPIOG2DRV_SR;
	unsigned int rGPIOG2CONPDN;
	unsigned int rGPIOG2PUDPDN;
	unsigned int reservedG2[2];

	unsigned int rGPIOG3CON;			//0xE0200200
	unsigned int rGPIOG3DAT;
	unsigned int rGPIOG3PUD;
	unsigned int rGPIOG3DRV_SR;
	unsigned int rGPIOG3CONPDN;
	unsigned int rGPIOG3PUDPDN;
	unsigned int reservedG3[2];
	
	unsigned int rGPIOICON;			//0xE0200220
	unsigned int rGPIOIDAT;
	unsigned int rGPIOIPUD;
	unsigned int rGPIOIDRV_SR;
	unsigned int rGPIOICONPDN;
	unsigned int rGPIOIPUDPDN;
	unsigned int reservedI[2];

	unsigned int rGPIOJ0CON;			//0xE0200240
	unsigned int rGPIOJ0DAT;
	unsigned int rGPIOJ0PUD;
	unsigned int rGPIOJ0DRV_SR;
	unsigned int rGPIOJ0CONPDN;
	unsigned int rGPIOJ0PUDPDN;
	unsigned int reservedJ0[2];

	unsigned int rGPIOJ1CON;			//0xE0200260
	unsigned int rGPIOJ1DAT;
	unsigned int rGPIOJ1PUD;
	unsigned int rGPIOJ1DRV_SR;
	unsigned int rGPIOJ1CONPDN;
	unsigned int rGPIOJ1PUDPDN;
	unsigned int reservedJ1[2];

	unsigned int rGPIOJ2CON;			//0xE0200280
	unsigned int rGPIOJ2DAT;
	unsigned int rGPIOJ2PUD;
	unsigned int rGPIOJ2DRV_SR;
	unsigned int rGPIOJ2CONPDN;
	unsigned int rGPIOJ2PUDPDN;
	unsigned int reservedJ2[2];

	unsigned int rGPIOJ3CON;			//0xE02002A0
	unsigned int rGPIOJ3DAT;
	unsigned int rGPIOJ3PUD;
	unsigned int rGPIOJ3DRV_SR;
	unsigned int rGPIOJ3CONPDN;
	unsigned int rGPIOJ3PUDPDN;
	unsigned int reservedJ3[2];

	unsigned int rGPIOJ4CON;			//0xE02002C0
	unsigned int rGPIOJ4DAT;
	unsigned int rGPIOJ4PUD;
	unsigned int rGPIOJ4DRV_SR;
	unsigned int rGPIOJ4CONPDN;
	unsigned int rGPIOJ4PUDPDN;
	unsigned int reservedJ4[2];

		
       unsigned int rGPIOMP0_1CON;			//0xE02002E0
	unsigned int rGPIOMP0_1DAT;
	unsigned int rGPIOMP0_1PUD;
	unsigned int rGPIOMP0_1DRV_SR;
	unsigned int rGPIOMP0_1CONPDN;
	unsigned int rGPIOMP0_1PUDPDN;
	unsigned int reservedMP0_1[2];

	unsigned int rGPIOMP0_2CON;			//0xE0200300
	unsigned int rGPIOMP0_2DAT;
	unsigned int rGPIOMP0_2PUD;
	unsigned int rGPIOMP0_2DRV_SR;
	unsigned int rGPIOMP0_2CONPDN;
	unsigned int rGPIOMP0_2PUDPDN;
	unsigned int reservedMP0_2[2];

	unsigned int rGPIOMP0_3CON;			//0xE0200320
	unsigned int rGPIOMP0_3DAT;
	unsigned int rGPIOMP0_3PUD;
	unsigned int rGPIOMP0_3DRV_SR;
	unsigned int rGPIOMP0_3CONPDN;
	unsigned int rGPIOMP0_3PUDPDN;
	unsigned int reservedMP0_3[2];

	unsigned int rGPIOMP0_4CON;			//0xE0200340
	unsigned int rGPIOMP0_4DAT;
	unsigned int rGPIOMP0_4PUD;
	unsigned int rGPIOMP0_4DRV_SR;
	unsigned int rGPIOMP0_4CONPDN;
	unsigned int rGPIOMP0_4PUDPDN;
	unsigned int reservedMP0_4[2];

	unsigned int rGPIOMP0_5CON;			//0xE0200360
	unsigned int rGPIOMP0_5DAT;
	unsigned int rGPIOMP0_5PUD;
	unsigned int rGPIOMP0_5DRV_SR;
	unsigned int rGPIOMP0_5CONPDN;
	unsigned int rGPIOMP0_5PUDPDN;
	unsigned int reservedMP0_5[2];

	unsigned int rGPIOMP0_6CON;			//0xE0200380
	unsigned int rGPIOMP0_6DAT;
	unsigned int rGPIOMP0_6PUD;
	unsigned int rGPIOMP0_6DRV_SR;
	unsigned int rGPIOMP0_6CONPDN;
	unsigned int rGPIOMP0_6PUDPDN;
	unsigned int reservedMP0_6[2];

	unsigned int rGPIOMP0_7CON;			//0xE02003A0
	unsigned int rGPIOMP0_7DAT;
	unsigned int rGPIOMP0_7PUD;
	unsigned int rGPIOMP0_7DRV_SR;
	unsigned int rGPIOMP0_7CONPDN;
	unsigned int rGPIOMP0_7PUDPDN;
	unsigned int reservedMP0_7[2];

	unsigned int rGPIOMP1_0CON;			//0xE02003C0
	unsigned int rGPIOMP1_0DAT;
	unsigned int rGPIOMP1_0PUD;
	unsigned int rGPIOMP1_0DRV_SR;
	unsigned int rGPIOMP1_0CONPDN;
	unsigned int rGPIOMP1_0PUDPDN;
	unsigned int reservedMP1_0[2];

	unsigned int rGPIOMP1_1CON;			//0xE02003E0
	unsigned int rGPIOMP1_1DAT;
	unsigned int rGPIOMP1_1PUD;
	unsigned int rGPIOMP1_1DRV_SR;
	unsigned int rGPIOMP1_1CONPDN;
	unsigned int rGPIOMP1_1PUDPDN;
	unsigned int reservedMP1_1[2];

	unsigned int rGPIOMP1_2CON;			//0xE0200400
	unsigned int rGPIOMP1_2DAT;
	unsigned int rGPIOMP1_2PUD;
	unsigned int rGPIOMP1_2DRV_SR;
	unsigned int rGPIOMP1_2CONPDN;
	unsigned int rGPIOMP1_2PUDPDN;
	unsigned int reservedMP1_2[2];

	unsigned int rGPIOMP1_3CON;			//0xE0200420
	unsigned int rGPIOMP1_3DAT;
	unsigned int rGPIOMP1_3PUD;
	unsigned int rGPIOMP1_3DRV_SR;
	unsigned int rGPIOMP1_3CONPDN;
	unsigned int rGPIOMP1_3PUDPDN;
	unsigned int reservedMP1_3[2];

	unsigned int rGPIOMP1_4CON;			//0xE0200440
	unsigned int rGPIOMP1_4DAT;
	unsigned int rGPIOMP1_4PUD;
	unsigned int rGPIOMP1_4DRV_SR;
	unsigned int rGPIOMP1_4CONPDN;
	unsigned int rGPIOMP1_4PUDPDN;
	unsigned int reservedMP1_4[2];

	unsigned int rGPIOMP1_5CON;			//0xE0200460
	unsigned int rGPIOMP1_5DAT;
	unsigned int rGPIOMP1_5PUD;
	unsigned int rGPIOMP1_5DRV_SR;
	unsigned int rGPIOMP1_5CONPDN;
	unsigned int rGPIOMP1_5PUDPDN;
	unsigned int reservedMP1_5[2];

	unsigned int rGPIOMP1_6CON;			//0xE0200480
	unsigned int rGPIOMP1_6DAT;
	unsigned int rGPIOMP1_6PUD;
	unsigned int rGPIOMP1_6DRV_SR;
	unsigned int rGPIOMP1_6CONPDN;
	unsigned int rGPIOMP1_6PUDPDN;
	unsigned int reservedMP1_6[2];

	unsigned int rGPIOMP1_7CON;			//0xE02004A0
	unsigned int rGPIOMP1_7DAT;
	unsigned int rGPIOMP1_7PUD;
	unsigned int rGPIOMP1_7DRV_SR;
	unsigned int rGPIOMP1_7CONPDN;
	unsigned int rGPIOMP1_7PUDPDN;
	unsigned int reservedMP1_7[2];

	unsigned int rGPIOMP1_8CON;			//0xE02004C0
	unsigned int rGPIOMP1_8DAT;
	unsigned int rGPIOMP1_8PUD;
	unsigned int rGPIOMP1_8DRV_SR;
	unsigned int rGPIOMP1_8CONPDN;
	unsigned int rGPIOMP1_8PUDPDN;
	unsigned int reservedMP1_8[2];

	unsigned int rGPIOMP2_0CON;			//0xE02004E0
	unsigned int rGPIOMP2_0DAT;
	unsigned int rGPIOMP2_0PUD;
	unsigned int rGPIOMP2_0DRV_SR;
	unsigned int rGPIOMP2_0CONPDN;
	unsigned int rGPIOMP2_0PUDPDN;
	unsigned int reservedMP2_0[2];

	unsigned int rGPIOMP2_1CON;			//0xE0200500
	unsigned int rGPIOMP2_1DAT;
	unsigned int rGPIOMP2_1PUD;
	unsigned int rGPIOMP2_1DRV_SR;
	unsigned int rGPIOMP2_1CONPDN;
	unsigned int rGPIOMP2_1PUDPDN;
	unsigned int reservedMP2_1[2];

	unsigned int rGPIOMP2_2CON;			//0xE0200520
	unsigned int rGPIOMP2_2DAT;
	unsigned int rGPIOMP2_2PUD;
	unsigned int rGPIOMP2_2DRV_SR;
	unsigned int rGPIOMP2_2CONPDN;
	unsigned int rGPIOMP2_2PUDPDN;
	unsigned int reservedMP2_2[2];

	unsigned int rGPIOMP2_3CON;			//0xE0200540
	unsigned int rGPIOMP2_3DAT;
	unsigned int rGPIOMP2_3PUD;
	unsigned int rGPIOMP2_3DRV_SR;
	unsigned int rGPIOMP2_3CONPDN;
	unsigned int rGPIOMP2_3PUDPDN;
	unsigned int reservedMP2_3[2];

	unsigned int rGPIOMP2_4CON;			//0xE0200560
	unsigned int rGPIOMP2_4DAT;
	unsigned int rGPIOMP2_4PUD;
	unsigned int rGPIOMP2_4DRV_SR;
	unsigned int rGPIOMP2_4CONPDN;
	unsigned int rGPIOMP2_4PUDPDN;
	unsigned int reservedMP2_4[2];

	unsigned int rGPIOMP2_5CON;			//0xE0200580
	unsigned int rGPIOMP2_5DAT;
	unsigned int rGPIOMP2_5PUD;
	unsigned int rGPIOMP2_5DRV_SR;
	unsigned int rGPIOMP2_5CONPDN;
	unsigned int rGPIOMP2_5PUDPDN;
	unsigned int reservedMP2_5[2];

	unsigned int rGPIOMP2_6CON;			//0xE02005A0
	unsigned int rGPIOMP2_6DAT;
	unsigned int rGPIOMP2_6PUD;
	unsigned int rGPIOMP2_6DRV_SR;
	unsigned int rGPIOMP2_6CONPDN;
	unsigned int rGPIOMP2_6PUDPDN;
	unsigned int reservedMP2_6[2];

	unsigned int rGPIOMP2_7CON;			//0xE02005C0
	unsigned int rGPIOMP2_7DAT;
	unsigned int rGPIOMP2_7PUD;
	unsigned int rGPIOMP2_7DRV_SR;
	unsigned int rGPIOMP2_7CONPDN;
	unsigned int rGPIOMP2_7PUDPDN;
	unsigned int reservedMP2_7[2];

	unsigned int rGPIOMP2_8CON;			//0xE02005E0
	unsigned int rGPIOMP2_8DAT;
	unsigned int rGPIOMP2_8PUD;
	unsigned int rGPIOMP2_8DRV_SR;
	unsigned int rGPIOMP2_8CONPDN;
	unsigned int rGPIOMP2_8PUDPDN;
	unsigned int reservedMP2_8[2];
	
	unsigned int reservedETC0_1[2];
	unsigned int rGPIOETC0PUD;			//0xE0200608
	unsigned int rGPIOETC0DRV_SR;
	unsigned int reservedETC0_2[4];

	unsigned int reservedETC1_1[2];
	unsigned int rGPIOETC1PUD;			//0xE0200628
	unsigned int rGPIOETC1DRV_SR;
	unsigned int reservedETC1_2[4];

	unsigned int reservedETC2_1[2];
	unsigned int rGPIOETC2PUD;			//0xE0200648
	unsigned int rGPIOETC2DRV_SR;
	unsigned int reservedETC2_2[4];

	unsigned int reservedETC3_1[2];
	unsigned int rGPIOETC3PUD;			//0xE0200668
	unsigned int rGPIOETC3DRV_SR;
	unsigned int reservedETC3_2[4];

	unsigned int reserved1[32];

	unsigned int rEINT1CON;				//0xE0200700
	unsigned int rEINT2CON;	
	unsigned int rEINT3CON;	
	unsigned int rEINT4CON;	
	unsigned int rEINT5CON;	
	unsigned int rEINT6CON;	
	unsigned int rEINT7CON;	
	unsigned int rEINT8CON;	
	unsigned int rEINT9CON;	
	unsigned int rEINT10CON;	
	unsigned int rEINT11CON;	
	unsigned int rEINT12CON;	
	unsigned int rEINT13CON;	
	unsigned int rEINT14CON;	
	unsigned int rEINT15CON;	
	unsigned int rEINT16CON;	
	unsigned int rEINT17CON;	
	unsigned int rEINT18CON;	
	unsigned int rEINT19CON;	
	unsigned int rEINT20CON;	
	unsigned int rEINT21CON;	
	unsigned int rEINT22CON;	
	unsigned int rEINT23CON;	

	unsigned int reserved2[41];

	unsigned int rEINT1FLTCON0;			//0xE0200800
	unsigned int rEINT1FLTCON1;
	unsigned int rEINT2FLTCON0;
	unsigned int rEINT2FLTCON1;
	unsigned int rEINT3FLTCON0;
	unsigned int rEINT3FLTCON1;
	unsigned int rEINT4FLTCON0;
	unsigned int rEINT4FLTCON1;
	unsigned int rEINT5FLTCON0;
	unsigned int rEINT5FLTCON1;
	unsigned int rEINT6FLTCON0;
	unsigned int rEINT6FLTCON1;
	unsigned int rEINT7FLTCON0;
	unsigned int rEINT7FLTCON1;
	unsigned int rEINT8FLTCON0;
	unsigned int rEINT8FLTCON1;
	unsigned int rEINT9FLTCON0;
	unsigned int rEINT9FLTCON1;
	unsigned int rEINT10FLTCON0;
	unsigned int rEINT10FLTCON1;
	unsigned int rEINT11FLTCON0;
	unsigned int rEINT11FLTCON1;
	unsigned int rEINT12FLTCON0;
	unsigned int rEINT12FLTCON1;
	unsigned int rEINT13FLTCON0;
	unsigned int rEINT13FLTCON1;
	unsigned int rEINT14FLTCON0;
	unsigned int rEINT14FLTCON1;
	unsigned int rEINT15FLTCON0;
	unsigned int rEINT15FLTCON1;
	unsigned int rEINT16FLTCON0;
	unsigned int rEINT16FLTCON1;
	unsigned int rEINT17FLTCON0;
	unsigned int rEINT17FLTCON1;
	unsigned int rEINT18FLTCON0;
	unsigned int rEINT18FLTCON1;
	unsigned int rEINT19FLTCON0;
	unsigned int rEINT19FLTCON1;
	unsigned int rEINT20FLTCON0;
	unsigned int rEINT20FLTCON1;
	unsigned int rEINT21FLTCON0;
	unsigned int rEINT21FLTCON1;
	unsigned int rEINT22FLTCON0;
	unsigned int rEINT22FLTCON1;
	unsigned int rEINT23FLTCON0;
	unsigned int rEINT23FLTCON1;

	unsigned int reserved3[18];

	unsigned int rEINT1MASK;			//0xE0200900
	unsigned int rEINT2MASK;
	unsigned int rEINT3MASK;
	unsigned int rEINT4MASK;
	unsigned int rEINT5MASK;
	unsigned int rEINT6MASK;
	unsigned int rEINT7MASK;
	unsigned int rEINT8MASK;
	unsigned int rEINT9MASK;
	unsigned int rEINT10MASK;
	unsigned int rEINT11MASK;
	unsigned int rEINT12MASK;
	unsigned int rEINT13MASK;
	unsigned int rEINT14MASK;
	unsigned int rEINT15MASK;
	unsigned int rEINT16MASK;
	unsigned int rEINT17MASK;
	unsigned int rEINT18MASK;
	unsigned int rEINT19MASK;
	unsigned int rEINT20MASK;
	unsigned int rEINT21MASK;
	unsigned int rEINT22MASK;
	unsigned int rEINT23MASK;

	unsigned int reserved4[41];

	unsigned int rEINT1PEND;			//0xE0200A00
	unsigned int rEINT2PEND;
	unsigned int rEINT3PEND;
	unsigned int rEINT4PEND;
	unsigned int rEINT5PEND;
	unsigned int rEINT6PEND;
	unsigned int rEINT7PEND;
	unsigned int rEINT8PEND;
	unsigned int rEINT9PEND;
	unsigned int rEINT10PEND;
	unsigned int rEINT11PEND;
	unsigned int rEINT12PEND;
	unsigned int rEINT13PEND;
	unsigned int rEINT14PEND;
	unsigned int rEINT15PEND;
	unsigned int rEINT16PEND;
	unsigned int rEINT17PEND;
	unsigned int rEINT18PEND;
	unsigned int rEINT19PEND;
	unsigned int rEINT20PEND;
	unsigned int rEINT21PEND;
	unsigned int rEINT22PEND;
	unsigned int rEINT23PEND;	

	unsigned int reserved5[41];

	unsigned int rEINTGRPPRI;			//0xE0200B00
	unsigned int rEINTPRIORITY;
	unsigned int rEINTSERVICE;
	unsigned int rEINTSERVICEPEND;
	unsigned int rEINTGRPFIXPRI;
	
	unsigned int rEINT1FIXPRI;			//0xE0200B14
	unsigned int rEINT2FIXPRI;
	unsigned int rEINT3FIXPRI;
	unsigned int rEINT4FIXPRI;
	unsigned int rEINT5FIXPRI;
	unsigned int rEINT6FIXPRI;
	unsigned int rEINT7FIXPRI;
	unsigned int rEINT8FIXPRI;
	unsigned int rEINT9FIXPRI;
	unsigned int rEINT10FIXPRI;
	unsigned int rEINT11FIXPRI;
	unsigned int rEINT12FIXPRI;
	unsigned int rEINT13FIXPRI;
	unsigned int rEINT14FIXPRI;
	unsigned int rEINT15FIXPRI;
	unsigned int rEINT16FIXPRI;
	unsigned int rEINT17FIXPRI;
	unsigned int rEINT18FIXPRI;
	unsigned int rEINT19FIXPRI;
	unsigned int rEINT20FIXPRI;
	unsigned int rEINT21FIXPRI;
	unsigned int rEINT22FIXPRI;
	unsigned int rEINT23FIXPRI;

	unsigned int reserved6[36];
	
	unsigned int rGPIOH0CON;			//0xE0200C00
	unsigned int rGPIOH0DAT;
	unsigned int rGPIOH0PUD;
	unsigned int rGPIOH0DRV_SR;
	unsigned int rGPIOH0CONPDN;
	unsigned int rGPIOH0PUDPDN;
	unsigned int reservedH0[2];

	unsigned int rGPIOH1CON;			//0xE0200C20
	unsigned int rGPIOH1DAT;
	unsigned int rGPIOH1PUD;
	unsigned int rGPIOH1DRV_SR;
	unsigned int rGPIOH1CONPDN;
	unsigned int rGPIOH1PUDPDN;
	unsigned int reservedH1[2];

	unsigned int rGPIOH2CON;			//0xE0200C40
	unsigned int rGPIOH2DAT;
	unsigned int rGPIOH2PUD;
	unsigned int rGPIOH2DRV_SR;
	unsigned int rGPIOH2CONPDN;
	unsigned int rGPIOH2PUDPDN;
	unsigned int reservedH2[2];

	unsigned int rGPIOH3CON;			//0xE0200C60
	unsigned int rGPIOH3DAT;
	unsigned int rGPIOH3PUD;
	unsigned int rGPIOH3DRV_SR;
	unsigned int rGPIOH3CONPDN;
	unsigned int rGPIOH3PUDPDN;
	unsigned int reservedH3[2];

	unsigned int reserved7[96];

	unsigned int rEINT30CON;			//0xE0200E00
	unsigned int rEINT31CON;
	unsigned int rEINT32CON;
	unsigned int rEINT33CON;

	unsigned int reserved8[28];

	unsigned int rEINT30FLTCON0;			//0xE0200E80
	unsigned int rEINT30FLTCON1;
	unsigned int rEINT31FLTCON0;
	unsigned int rEINT31FLTCON1;
	unsigned int rEINT32FLTCON0;
	unsigned int rEINT32FLTCON1;
	unsigned int rEINT33FLTCON0;
	unsigned int rEINT33FLTCON1;

	unsigned int reserved9[24];

	unsigned int rEINT30MASK;			//0xE0200F00
	unsigned int rEINT31MASK;
	unsigned int rEINT32MASK;
	unsigned int rEINT33MASK;

	unsigned int reserved10[12];

	unsigned int rEINT30PEND;			//0xE0200F40
	unsigned int rEINT31PEND;
	unsigned int rEINT32PEND;
	unsigned int rEINT33PEND;

	unsigned int reserved11[12];

	unsigned int rPDEN;					//0xE0200F80
	

} 
oGPIO_REGS;

static void *GPIO_pBase;

unsigned int  g_IntCnt;



//////////
// Function Name : GPIO_Init
// Function Desctiption : This function initializes gpio sfr base address
// Input : NONE
// Output : NONE
// Version :
//
// Version : v0.1
void GPIO_Init(void)
{
	GPIO_pBase = (void *)GPIO_BASE;
}


//////////
// Function Name : GPIO_SetFunctionEach
// Function Desctiption : This function set each GPIO function
// Input : 	Id : GPIO port
//			eBitPos : GPIO bit
//			uFunction : Select the function
// Output : NONE
//
// Version : v0.0
void GPIO_SetFunctionEach(GPIO_eId Id, GPIO_eBitPos eBitPos, unsigned int uFunction)
{
	volatile unsigned int *pGPIOx_Reg0;
	volatile unsigned int *pGPIO_Base_Addr;
	unsigned int uMuxBit,  uOffset;
	unsigned int uConValue;

	uMuxBit = 4; // 4bit
	uOffset = Id&0xFFF;  

	pGPIO_Base_Addr = &(GPIO->rGPIOA0CON);
	
	pGPIOx_Reg0 = pGPIO_Base_Addr + uOffset/4;
	uConValue = *pGPIOx_Reg0;
	uConValue = (uConValue & ~(0xF<<(uMuxBit*eBitPos))) | (uFunction<<(uMuxBit*eBitPos));
	*pGPIOx_Reg0 = uConValue;
	
}



//////////
// Function Name : GPIO_SetFunctionAll
// Function Desctiption : This function set all GPIO function selection
// Input : 	Id : GPIO port
//			uValue0 : Write value(control register 0)
// Output : NONE
//
// Version : v0.0
void GPIO_SetFunctionAll(GPIO_eId Id, unsigned int uValue0)
{
	volatile unsigned int *pGPIOx_Reg0;
	volatile unsigned int *pGPIO_Base_Addr;
	unsigned int uConRegNum, uOffset;


	uOffset = Id&0xFFF;

	pGPIO_Base_Addr = &(GPIO->rGPIOA0CON);
	
	pGPIOx_Reg0 = pGPIO_Base_Addr + uOffset/4;
	*pGPIOx_Reg0 = uValue0;

}



//////////
// Function Name : GPIO_SetDataEach
// Function Desctiption : This function set each GPIO data bit
// Input : 	Id : GPIO port
//			eBitPos : GPIO bit
//			uValue : value
// Output : NONE
//
// Version : v0.0
void GPIO_SetDataEach(GPIO_eId Id, GPIO_eBitPos eBitPos, unsigned int uValue)
{
	volatile unsigned int *pGPIOx_DataReg;
	volatile unsigned int *pGPIO_Base_Addr;
	unsigned int  uOffset, uConRegNum;
	unsigned int uDataValue;

	uOffset = Id&0xFFF;
	uConRegNum = 1;
	

	pGPIO_Base_Addr = &(GPIO->rGPIOA0CON);
	
	pGPIOx_DataReg = pGPIO_Base_Addr + (uOffset/4) +uConRegNum;
	uDataValue = *pGPIOx_DataReg;
	uDataValue = (uDataValue & ~(0x1<<eBitPos)) | (uValue<<eBitPos);
	*pGPIOx_DataReg = uDataValue;
}



//////////
// Function Name : GPIO_SetDataAll
// Function Desctiption : This function set all GPIO data bit
// Input : 	Id : GPIO port
//			uValue : value
// Output : NONE
//
// Version : v0.0
void GPIO_SetDataAll(GPIO_eId Id, unsigned int uValue)
{
	volatile unsigned int *pGPIOx_DataReg;
	volatile unsigned int *pGPIO_Base_Addr;
	unsigned int  uOffset, uConRegNum;

	uOffset = Id&0xFFF;
	uConRegNum = 1;

	pGPIO_Base_Addr = &(GPIO->rGPIOA0CON);
	
	pGPIOx_DataReg = pGPIO_Base_Addr + (uOffset/4) +uConRegNum;
	*pGPIOx_DataReg = uValue;
}

//added by jhlim

unsigned int GPIO_GetDataEach(GPIO_eId Id,GPIO_eBitPos eBitPos)
{
	volatile unsigned int *pGPIOx_DataReg;
	volatile unsigned int *pGPIO_Base_Addr;
	unsigned int uConRegNum, uOffset,uDataValue;


	uOffset = Id&0xFFF;
	uConRegNum = 1;
	

	pGPIO_Base_Addr = &(GPIO->rGPIOA0CON);
	
	pGPIOx_DataReg = pGPIO_Base_Addr + (uOffset/4)+ uConRegNum;

	uDataValue = ((*pGPIOx_DataReg) >> eBitPos ) &0x1;
	
	return uDataValue;
}

unsigned int GPIO_GetDataAll(GPIO_eId Id)
{
	volatile unsigned int *pGPIOx_DataReg;
	volatile unsigned int *pGPIO_Base_Addr;
	unsigned int uConRegNum, uOffset;


	uOffset = Id&0xFFF;
	uConRegNum = 1;
	

	pGPIO_Base_Addr = &(GPIO->rGPIOA0CON);
	
	pGPIOx_DataReg = pGPIO_Base_Addr + (uOffset/4)+ uConRegNum;
	return (*pGPIOx_DataReg);
}


//////////
// Function Name : GPIO_SetPullUpDownEach
// Function Desctiption : This function set each GPIO Pull-up/Down bits
// Input : 	Id : GPIO port
//			eBitPos : GPIO bit
//			uValue : value(2bit)
// Output : NONE
//
// Version : v0.0
void GPIO_SetPullUpDownEach(GPIO_eId Id, GPIO_eBitPos eBitPos, unsigned int uValue)
{
	volatile unsigned int *pGPIOx_PullUDReg;
	volatile unsigned int *pGPIO_Base_Addr;
	unsigned int uConRegNum, uDataRegNum,  uOffset;
	unsigned int uPullValue;

	
	uOffset = Id&0xFFF;
	uConRegNum = 1;
	uDataRegNum = 1;
	

	pGPIO_Base_Addr = &(GPIO->rGPIOA0CON);
	
	pGPIOx_PullUDReg = pGPIO_Base_Addr + (uOffset/4) + uConRegNum+uDataRegNum;
	uPullValue = *pGPIOx_PullUDReg;
	uPullValue = (uPullValue & ~(0x3<<(0x02*eBitPos))) | (uValue<<(0x02*eBitPos));
	*pGPIOx_PullUDReg = uPullValue;
}



//////////
// Function Name : GPIO_SetPullUpDownAll
// Function Desctiption : This function set all GPIO Pull-up/Down bits
// Input : 	Id : GPIO port
//			uValue : value(32bit)
// Output : NONE
//
// Version : v0.0
void GPIO_SetPullUpDownAll(GPIO_eId Id, unsigned int uValue)
{
	volatile unsigned int *pGPIOx_PullUDReg;
	volatile unsigned int *pGPIO_Base_Addr;
	unsigned int  uConRegNum, uDataRegNum, uOffset;

	uOffset = Id&0xFFF;
	uConRegNum = 1;
	uDataRegNum = 1;
	

	pGPIO_Base_Addr = &(GPIO->rGPIOA0CON);
	
	pGPIOx_PullUDReg = pGPIO_Base_Addr + (uOffset/4) + uConRegNum+uDataRegNum;
	*pGPIOx_PullUDReg = uValue;
}


//////////
// Function Name : ETC_SetPullUpDownEach
// Function Desctiption : This function set each ETC Pull-up/Down bits
// Input : 	Id : ETC port
//			eBitPos : ETC bit
//			uValue : value(2bit)
// Output : NONE
//
// Version : v0.0
void ETC_SetPullUpDownEach(GPIO_eId Id, GPIO_eBitPos eBitPos, unsigned int uValue)
{
	volatile unsigned int *pETCx_PullUDReg;
	volatile unsigned int *pGPIO_Base_Addr;
	unsigned int uOffset;
	unsigned int uPullValue;

	
	uOffset = Id&0xFFF;
	

	pGPIO_Base_Addr = &(GPIO->rGPIOA0CON);
	
	pETCx_PullUDReg = pGPIO_Base_Addr + (uOffset/4) ;
	uPullValue = *pETCx_PullUDReg;
	uPullValue = (uPullValue & ~(0x3<<(0x02*eBitPos))) | (uValue<<(0x02*eBitPos));
	*pETCx_PullUDReg = uPullValue;
}


//////////
// Function Name : ETC_SetPullUpDownAll
// Function Desctiption : This function set all ETC Pull-up/Down bits
// Input : 	Id : ETC port
//			uValue : value(16bit)
// Output : NONE
//
// Version : v0.0
void ETC_SetPullUpDownAll(GPIO_eId Id, unsigned int uValue)
{
	volatile unsigned int *pETCx_PullUDReg;
	volatile unsigned int *pGPIO_Base_Addr;
	unsigned int  uOffset;

	uOffset = Id&0xFFF;
	
	pGPIO_Base_Addr = &(GPIO->rGPIOA0CON);
	
	pETCx_PullUDReg = pGPIO_Base_Addr + (uOffset/4);
	*pETCx_PullUDReg = uValue;
}



//////////
// Function Name : GPIO_SetDSEach
// Function Desctiption : This function set each GPIO Driving Strength bits
// Input : 	Id : GPIO port
//			eBitPos : GPIO bit
//			uValue : value(2bit)
// Output : NONE
//
// Version : v0.0
void GPIO_SetDSEach(GPIO_eId Id, GPIO_eBitPos eBitPos, unsigned int uValue)
{
	volatile unsigned int *pGPIOx_DSReg;
	volatile unsigned int *pGPIO_Base_Addr;
	unsigned int uConRegNum, uDataRegNum,  uPullUDRegNum, uOffset;
	unsigned int uDSValue;

	
	uOffset = Id&0xFFF;
	uConRegNum = 1;
	uDataRegNum = 1;
	uPullUDRegNum = 1;
	

	pGPIO_Base_Addr = &(GPIO->rGPIOA0CON);
	
	pGPIOx_DSReg = pGPIO_Base_Addr + (uOffset/4) + uConRegNum+uDataRegNum+uPullUDRegNum;
	uDSValue = *pGPIOx_DSReg;
	uDSValue = (uDSValue & ~(0x3<<(0x02*eBitPos))) | (uValue<<(0x02*eBitPos));
	*pGPIOx_DSReg = uDSValue;
}


//////////
// Function Name : GPIO_SetDSAll
// Function Desctiption : This function set All GPIO Driving Strength bits
// Input : 	Id : GPIO port
//			uValue : value(16bit)
// Output : NONE
//
// Version : v0.0
void GPIO_SetDSAll(GPIO_eId Id, unsigned int uValue)
{
	volatile unsigned int *pGPIOx_DSReg;
	volatile unsigned int *pGPIO_Base_Addr;
	unsigned int uConRegNum, uDataRegNum,  uPullUDRegNum, uOffset;
	unsigned int uDSValue;

	
	uOffset = Id&0xFFF;
	uConRegNum = 1;
	uDataRegNum = 1;
	uPullUDRegNum = 1;
	

	pGPIO_Base_Addr = &(GPIO->rGPIOA0CON);
	
	pGPIOx_DSReg = pGPIO_Base_Addr + (uOffset/4) + uConRegNum+uDataRegNum+uPullUDRegNum;
	uDSValue = *pGPIOx_DSReg;
	uDSValue = (uDSValue & ~(0xffff)) | uValue;
	*pGPIOx_DSReg = uDSValue;
}


//////////
// Function Name : GPIO_SetSREach
// Function Desctiption : This function set each GPIO Slew Rate bits
// Input : 	Id : GPIO port
//			eBitPos : GPIO bit
//			uValue : value(2bit)
// Output : NONE
//
// Version : v0.0
void GPIO_SetSREach(GPIO_eId Id, GPIO_eBitPos eBitPos, unsigned int uValue)
{
	volatile unsigned int *pGPIOx_SRReg;
	volatile unsigned int *pGPIO_Base_Addr;
	unsigned int uConRegNum, uDataRegNum,  uPullUDRegNum, uOffset;
	unsigned int uSRValue;

	
	uOffset = Id&0xFFF;
	uConRegNum = 1;
	uDataRegNum = 1;
	uPullUDRegNum = 1;
	

	pGPIO_Base_Addr = &(GPIO->rGPIOA0CON);
	
	pGPIOx_SRReg = pGPIO_Base_Addr + (uOffset/4) + uConRegNum+uDataRegNum+uPullUDRegNum;
	uSRValue = *pGPIOx_SRReg;
	uSRValue = (uSRValue & ~(0x1<<(eBitPos+16))) | (uValue<<(eBitPos+16));
	*pGPIOx_SRReg = uSRValue;
}


//////////
// Function Name : GPIO_SetSRAll
// Function Desctiption : This function set All GPIO Slew Rate bits
// Input : 	Id : GPIO port
//			uValue : value(16bit)
// Output : NONE
//
// Version : v0.0
void GPIO_SetSRAll(GPIO_eId Id, unsigned int uValue)
{
	volatile unsigned int *pGPIOx_SRReg;
	volatile unsigned int *pGPIO_Base_Addr;
	unsigned int uConRegNum, uDataRegNum,  uPullUDRegNum, uOffset;
	unsigned int uSRValue;

	
	uOffset = Id&0xFFF;
	uConRegNum = 1;
	uDataRegNum = 1;
	uPullUDRegNum = 1;
	

	pGPIO_Base_Addr = &(GPIO->rGPIOA0CON);
	
	pGPIOx_SRReg = pGPIO_Base_Addr + (uOffset/4) + uConRegNum+uDataRegNum+uPullUDRegNum;
	uSRValue = *pGPIOx_SRReg;
	uSRValue = (uSRValue & ~(0xffff0000)) | (uValue<<16);
	*pGPIOx_SRReg = uSRValue;
}




//////////
// Function Name : GPIO_SetConRegPDNAll
// Function Desctiption : This function set all GPIO function when system enter to Power Down mode
// Input : 	Id : GPIO port
//			uValue : value(16bit)
// Output : NONE
//
// Version : v0.0
void GPIO_SetConRegPDNAll(GPIO_eId Id, unsigned int uValue)
{
	volatile unsigned int *pGPIOx_ConPDNReg;
	volatile unsigned int *pGPIO_Base_Addr;
	unsigned int uConRegNum, uDataRegNum, uPullUDRegNum, uConDSRegNum, uOffset;

	uOffset = Id&0xFFF;
	uConRegNum = 1;	
	uDataRegNum = 1;
	uPullUDRegNum = 1;
	uConDSRegNum =1;

	pGPIO_Base_Addr = &(GPIO->rGPIOA0CON);
	
	pGPIOx_ConPDNReg = pGPIO_Base_Addr + (uOffset/4) + (uConRegNum + uDataRegNum + uPullUDRegNum+uConDSRegNum);
	*pGPIOx_ConPDNReg = uValue;
}


//////////
// Function Name : GPIO_SetPullUDPDNAll
// Function Desctiption : This function set all GPIO Pull-up/down when system enter to Power Down mode
// Input : 	Id : GPIO port
//			uValue : value(16bit)
// Output : NONE
//
// Version : v0.1
void GPIO_SetPullUDPDNAll(GPIO_eId Id, unsigned int uValue)
{
	volatile unsigned int *pGPIOx_PullUDPDNReg;
	volatile unsigned int *pGPIO_Base_Addr;
	unsigned int uConRegNum, uDataRegNum, uPullUDRegNum, uConDSRegNum, uConPDNRegNum, uOffset;

       uOffset = Id&0xFFF;
	uConRegNum = 1;
	uDataRegNum = 1;
	uPullUDRegNum = 1;
	uConDSRegNum =1;
	uConPDNRegNum = 1;

	pGPIO_Base_Addr = &(GPIO->rGPIOA0CON);
	
	pGPIOx_PullUDPDNReg = pGPIO_Base_Addr + (uOffset/4) + (uConRegNum+uDataRegNum+uPullUDRegNum+uConDSRegNum+uConPDNRegNum);
	*pGPIOx_PullUDPDNReg = uValue;
}


#ifdef ENABLE_EINT

//////////
// Function Name : GPIO_SetEint1
// Function Desctiption : This function setup Eint1[7:0]=> GPA0[7:0]
// Input : 			uEINT_No: EINT No.
//					uINTType: Select EINT Type. 
//								Low, High, Falling, Rising, Both
//					uFltType :  Select Filter Type
//								DisFLT(Disable Filter), DLYFLT(Delay Filter), DIGFLT(Digital Filter)
//					uFltWidth : Digital Filter Width ( 1~0x7F)
// Output : NONE
//
// Version : v0.0
void GPIO_SetEint1(unsigned int uEINT_No , unsigned int uIntType, FLT_eTYPE eFltType,  unsigned int uFltWidth)
{

	volatile unsigned int *pEINTx_Reg, *pFLTx_Reg;
	volatile unsigned int *pGPIO_EINT_Addr;
	unsigned int uGpioPort, uFunc, uType ;	
	unsigned int uConValue;

	GPIO_pBase = (void *)GPIO_BASE;

	pGPIO_EINT_Addr = &(GPIO->rEINT1CON);

	if (uEINT_No > 7)
	{
		printf("Error Eint No. \n");
		Assert(0);
	}
	
	// Check Filter Width 
	if(uFltWidth >0x7f)
	{
		printf("Error Filter Width. \n");
		Assert(0);
	}

	//  Interrupt Type 
	if( uEINT_No <= 7)
	{
	uType =uEINT_No;
	pEINTx_Reg = pGPIO_EINT_Addr ; 	
	uConValue = *pEINTx_Reg;
	uConValue = (uConValue & ~(0xF<<(uType*4))) | (uIntType<<(uType*4));
	*pEINTx_Reg = uConValue;
	}


	// EINT Port
	if( uEINT_No <= 7)
	{
	uGpioPort = uEINT_No;
	uFunc = 0xf;  					// EINT Function 
	GPIO_SetFunctionEach(eGPIO_A0, (GPIO_eBitPos)uGpioPort, uFunc);		// ??
	// GPIO_SetPullUpDownEach(eGPIO_N,(GPIO_eBitPos) uGpioPort, 0x0);		// disable Pull-up/dn
	}		

	// Fliter Type & Filter Width 
	if( uEINT_No <= 3)
	{
		uType =uEINT_No;
		pFLTx_Reg = &(GPIO->rEINT1FLTCON0) ; 	    // EINT1FLTCON0
		uConValue = *pFLTx_Reg;
		uConValue = (uConValue & ~(0xFF<<(uType*8))) |((uFltWidth<<(uType*8))|(eFltType<<(uType*8+7)));
		*pFLTx_Reg = uConValue;	
	}
	else if(uEINT_No > 3 && uEINT_No <= 7)
	{
		uType =uEINT_No-4;
		pFLTx_Reg =&(GPIO->rEINT1FLTCON1); 	    // EINT1FLTCON1
		uConValue = *pFLTx_Reg;
		uConValue = (uConValue & ~(0xFF<<(uType*8))) |((uFltWidth<<(uType*8))|(eFltType<<(uType*8+7)));
		*pFLTx_Reg = uConValue;	
	}
	
}



//////////
// Function Name : GPIO_EINT1ClrPend
// Function Desctiption : Clear Eint pending bit of the Eint1[7:0]
// Input : 			uEINT_No: EINT No.
// Output : NONE
//
// Version : v0.0
void GPIO_EINT1ClrPend(unsigned int uEINT_No )
{

	volatile unsigned int *pEINT1PEND;
	unsigned int uConValue;

	pEINT1PEND = &(GPIO->rEINT1PEND);

	uConValue = (1<<uEINT_No);
	*pEINT1PEND = uConValue;

}

//////////
// Function Name : GPIO_EINT1UnMask
// Function Desctiption : UnMask the Eint1[7:0]
// Input : 			uEINT_No: EINT No.
// Output : NONE
//
// Version : v0.0
void GPIO_EINT1UnMask(unsigned int uEINT_No )
{

	volatile unsigned int *pMASK_Reg;
	unsigned int uConValue;

	pMASK_Reg = &(GPIO->rEINT1MASK);
	
	uConValue = *pMASK_Reg;
       uConValue = (uConValue & ~(0x1<<(uEINT_No)));
	*pMASK_Reg = uConValue;

}

//////////
// Function Name : GPIO_EINT1Mask
// Function Desctiption : Mask the Eint1[7:0]
// Input : 			uEINT_No: EINT No.
// Output : NONE
//
// Version : v0.0
void GPIO_EINT1Mask(unsigned int uEINT_No )
{

	volatile unsigned int *pMASK_Reg;
	unsigned int uConValue;

	pMASK_Reg = &(GPIO->rEINT1MASK);
	
	uConValue = *pMASK_Reg;
       uConValue = (uConValue & ~(0x1<<(uEINT_No)))|(1<<uEINT_No);
	*pMASK_Reg = uConValue;

}


//////////
// Function Name : GPIO_SetEint2
// Function Desctiption : This function setup Eint2[3:0]=> GPA1[3:0]
// Input : 			uEINT_No: EINT No.
//					uINTType: Select EINT Type. 
//								Low, High, Falling, Rising, Both
//					uFltType :  Select Filter Type
//								DisFLT(Disable Filter), DLYFLT(Delay Filter), DIGFLT(Digital Filter)
//					uFltWidth : Digital Filter Width ( 1~0x7F)
// Output : NONE
//
// Version : v0.0
void GPIO_SetEint2(unsigned int uEINT_No , unsigned int uIntType, FLT_eTYPE eFltType,  unsigned int uFltWidth)
{

	volatile unsigned int *pEINTx_Reg, *pFLTx_Reg;
	volatile unsigned int *pGPIO_EINT_Addr;
	unsigned int uGpioPort, uFunc, uType ;	
	unsigned int uConValue;

	GPIO_pBase = (void *)GPIO_BASE;

	pGPIO_EINT_Addr = &(GPIO->rEINT2CON);

	
	if (uEINT_No >3)
	{
		printf("Error Eint No. \n");
		Assert(0);
	}
	

	// Check Filter Width 
	if(uFltWidth >0x7f)
	{
		printf("Error Filter Width. \n");
		Assert(0);
	}
	

	//  Interrupt Type 
	if( uEINT_No <= 3)
	{
		uType =uEINT_No;
		pEINTx_Reg = pGPIO_EINT_Addr ; 	
		uConValue = *pEINTx_Reg;
		uConValue = (uConValue & ~(0xF<<(uType*4))) | (uIntType<<(uType*4));
		*pEINTx_Reg = uConValue;
	}


	// EINT Port
	if( uEINT_No <= 3)
	{
		uGpioPort = uEINT_No;
		uFunc = 0xf;  					// EINT Function 
		GPIO_SetFunctionEach(eGPIO_A1, (GPIO_eBitPos)uGpioPort, uFunc);		// ??
		// GPIO_SetPullUpDownEach(eGPIO_N,(GPIO_eBitPos) uGpioPort, 0x0);		// disable Pull-up/dn
	}

	// Fliter Type & Filter Width 
	if( uEINT_No <= 3)
	{
		uType =uEINT_No;
		pFLTx_Reg = &(GPIO->rEINT2FLTCON0) ; 	    // EINT2FLTCON0
		uConValue = *pFLTx_Reg;
		uConValue = (uConValue & ~(0xFF<<(uType*8))) |((uFltWidth<<(uType*8))|(eFltType<<(uType*8+7)));
		*pFLTx_Reg = uConValue;	
	}
	
	
}



//////////
// Function Name : GPIO_EINT2ClrPend
// Function Desctiption : Clear Eint pending bit of the Eint2[3:0]
// Input : 			uEINT_No: EINT No.
// Output : NONE
//
// Version : v0.0
void GPIO_EINT2ClrPend(unsigned int uEINT_No )
{

	volatile unsigned int *pEINT2PEND;
	unsigned int uConValue;

	pEINT2PEND = &(GPIO->rEINT2PEND);

	uConValue = (1<<uEINT_No);
	*pEINT2PEND = uConValue;

}

//////////
// Function Name : GPIO_EINT2UnMask
// Function Desctiption : UnMask the Eint2[3:0]
// Input : 			uEINT_No: EINT No.
// Output : NONE
//
// Version : v0.0
void GPIO_EINT2UnMask(unsigned int uEINT_No )
{

	volatile unsigned int *pMASK_Reg;
	unsigned int uConValue;

	pMASK_Reg = &(GPIO->rEINT2MASK);
	
	uConValue = *pMASK_Reg;
       uConValue = (uConValue & ~(0x1<<(uEINT_No)));
	*pMASK_Reg = uConValue;

}

//////////
// Function Name : GPIO_EINT2Mask
// Function Desctiption : Mask the Eint2[3:0]
// Input : 			uEINT_No: EINT No.
// Output : NONE
//
// Version : v0.0
void GPIO_EINT2Mask(unsigned int uEINT_No )
{

	volatile unsigned int *pMASK_Reg;
	unsigned int uConValue;

	pMASK_Reg = &(GPIO->rEINT2MASK);
	
	uConValue = *pMASK_Reg;
       uConValue = (uConValue & ~(0x1<<(uEINT_No)))|(1<<uEINT_No);
	*pMASK_Reg = uConValue;

}



//////////
// Function Name : GPIO_SetEint3
// Function Desctiption : This function setup Eint3[7:0]=> GPB[7:0]
// Input : 			uEINT_No: EINT No.
//					uINTType: Select EINT Type. 
//								Low, High, Falling, Rising, Both
//					uFltType :  Select Filter Type
//								DisFLT(Disable Filter), DLYFLT(Delay Filter), DIGFLT(Digital Filter)
//					uFltWidth : Digital Filter Width ( 1~0x7F)
// Output : NONE
//
// Version : v0.0
void GPIO_SetEint3(unsigned int uEINT_No , unsigned int uIntType, FLT_eTYPE eFltType,  unsigned int uFltWidth)
{

	volatile unsigned int *pEINTx_Reg, *pFLTx_Reg;
	volatile unsigned int *pGPIO_EINT_Addr;
	unsigned int uGpioPort, uFunc, uType ;	
	unsigned int uConValue;

	GPIO_pBase = (void *)GPIO_BASE;

	pGPIO_EINT_Addr = &(GPIO->rEINT3CON);

	if (uEINT_No > 7)
	{
		printf("Error Eint No. \n");
		Assert(0);
	}
	
	// Check Filter Width 
	if(uFltWidth >0x7f)
	{
		printf("Error Filter Width. \n");
		Assert(0);
	}

	//  Interrupt Type 
	if( uEINT_No <= 7)
	{
		uType =uEINT_No;
		pEINTx_Reg = pGPIO_EINT_Addr ; 	
		uConValue = *pEINTx_Reg;
		uConValue = (uConValue & ~(0xF<<(uType*4))) | (uIntType<<(uType*4));
		*pEINTx_Reg = uConValue;
	}


	// EINT Port
	if( uEINT_No <= 7)
	{
		uGpioPort = uEINT_No;
		uFunc = 0xf;  					// EINT Function 
		GPIO_SetFunctionEach(eGPIO_B, (GPIO_eBitPos)uGpioPort, uFunc);		// ??
		// GPIO_SetPullUpDownEach(eGPIO_N,(GPIO_eBitPos) uGpioPort, 0x0);		// disable Pull-up/dn
	}
		

	// Fliter Type & Filter Width 
	if( uEINT_No <= 3)
	{
		uType =uEINT_No;
		pFLTx_Reg = &(GPIO->rEINT3FLTCON0) ; 	    // EINT3FLTCON0
		uConValue = *pFLTx_Reg;
		uConValue = (uConValue & ~(0xFF<<(uType*8))) |((uFltWidth<<(uType*8))|(eFltType<<(uType*8+7)));
		*pFLTx_Reg = uConValue;	
	}
	else if(uEINT_No >3 && uEINT_No <= 7)
	{
		uType =uEINT_No-4;
		pFLTx_Reg =&(GPIO->rEINT3FLTCON1); 	    // EINT3FLTCON1
		uConValue = *pFLTx_Reg;
		uConValue = (uConValue & ~(0xFF<<(uType*8))) |((uFltWidth<<(uType*8))|(eFltType<<(uType*8+7)));
		*pFLTx_Reg = uConValue;	
	}
	
}



//////////
// Function Name : GPIO_EINT3ClrPend
// Function Desctiption : Clear Eint pending bit of the Eint3[7:0]
// Input : 			uEINT_No: EINT No.
// Output : NONE
//
// Version : v0.0
void GPIO_EINT3ClrPend(unsigned int uEINT_No )
{

	volatile unsigned int *pEINT3PEND;
	unsigned int uConValue;

	pEINT3PEND = &(GPIO->rEINT3PEND);

	uConValue = (1<<uEINT_No);
	*pEINT3PEND = uConValue;

}

//////////
// Function Name : GPIO_EINT3UnMask
// Function Desctiption : UnMask the Eint3[7:0]
// Input : 			uEINT_No: EINT No.
// Output : NONE
//
// Version : v0.0
void GPIO_EINT3UnMask(unsigned int uEINT_No )
{

	volatile unsigned int *pMASK_Reg;
	unsigned int uConValue;

	pMASK_Reg = &(GPIO->rEINT3MASK);
	
	uConValue = *pMASK_Reg;
       uConValue = (uConValue & ~(0x1<<(uEINT_No)));
	*pMASK_Reg = uConValue;

}

//////////
// Function Name : GPIO_EINT3Mask
// Function Desctiption : Mask the Eint3[7:0]
// Input : 			uEINT_No: EINT No.
// Output : NONE
//
// Version : v0.0
void GPIO_EINT3Mask(unsigned int uEINT_No )
{

	volatile unsigned int *pMASK_Reg;
	unsigned int uConValue;

	pMASK_Reg = &(GPIO->rEINT3MASK);
	
	uConValue = *pMASK_Reg;
       uConValue = (uConValue & ~(0x1<<(uEINT_No)))|(1<<uEINT_No);
	*pMASK_Reg = uConValue;

}




//////////
// Function Name : GPIO_SetEint4
// Function Desctiption : This function setup Eint4[4:0]=> GPC0[4:0]
// Input : 			uEINT_No: EINT No.
//					uINTType: Select EINT Type. 
//								Low, High, Falling, Rising, Both
//					uFltType :  Select Filter Type
//								DisFLT(Disable Filter), DLYFLT(Delay Filter), DIGFLT(Digital Filter)
//					uFltWidth : Digital Filter Width ( 1~0x7F)
// Output : NONE
//
// Version : v0.0
void GPIO_SetEint4(unsigned int uEINT_No , unsigned int uIntType, FLT_eTYPE eFltType,  unsigned int uFltWidth)
{

	volatile unsigned int *pEINTx_Reg, *pFLTx_Reg;
	volatile unsigned int *pGPIO_EINT_Addr;
	unsigned int uGpioPort, uFunc, uType ;	
	unsigned int uConValue;

	GPIO_pBase = (void *)GPIO_BASE;

	pGPIO_EINT_Addr = &(GPIO->rEINT4CON);

	if (uEINT_No > 4)
	{
		printf("Error Eint No. \n");
		Assert(0);
	}
	
	// Check Filter Width 
	if(uFltWidth > 0x7f)
	{
		printf("Error Filter Width. \n");
		Assert(0);
	}

	//  Interrupt Type 
	if( uEINT_No <= 4)
	{
		uType =uEINT_No;
		pEINTx_Reg = pGPIO_EINT_Addr ; 	
		uConValue = *pEINTx_Reg;
		uConValue = (uConValue & ~(0xF<<(uType*4))) | (uIntType<<(uType*4));
		*pEINTx_Reg = uConValue;
	}


	// EINT Port
	if( uEINT_No <= 4)
	{
		uGpioPort = uEINT_No;
		uFunc = 0xf;  					// EINT Function 
		GPIO_SetFunctionEach(eGPIO_C0, (GPIO_eBitPos)uGpioPort, uFunc);		// ??
		// GPIO_SetPullUpDownEach(eGPIO_N,(GPIO_eBitPos) uGpioPort, 0x0);		// disable Pull-up/dn
	}
		

	// Fliter Type & Filter Width 
	if( uEINT_No <= 3)
	{
		uType =uEINT_No;
		pFLTx_Reg = &(GPIO->rEINT4FLTCON0) ; 	    // EINT3FLTCON0
		uConValue = *pFLTx_Reg;
		uConValue = (uConValue & ~(0xFF<<(uType*8))) |((uFltWidth<<(uType*8))|(eFltType<<(uType*8+7)));
		*pFLTx_Reg = uConValue;	
	}
	else if(uEINT_No = 4)
	{
		uType =uEINT_No-4;
		pFLTx_Reg =&(GPIO->rEINT4FLTCON1); 	    // EINT3FLTCON1
		uConValue = *pFLTx_Reg;
		uConValue = (uConValue & ~(0xFF<<(uType*8))) |((uFltWidth<<(uType*8))|(eFltType<<(uType*8+7)));
		*pFLTx_Reg = uConValue;	
	}
	
}



//////////
// Function Name : GPIO_EINT4ClrPend
// Function Desctiption : Clear Eint pending bit of the Eint4[4:0]
// Input : 			uEINT_No: EINT No.
// Output : NONE
//
// Version : v0.0
void GPIO_EINT4ClrPend(unsigned int uEINT_No )
{

	volatile unsigned int *pEINT4PEND;
	unsigned int uConValue;

	pEINT4PEND = &(GPIO->rEINT4PEND);

	uConValue = (1<<uEINT_No);
	*pEINT4PEND = uConValue;

}

//////////
// Function Name : GPIO_EINT4UnMask
// Function Desctiption : UnMask the Eint4[4:0]
// Input : 			uEINT_No: EINT No.
// Output : NONE
//
// Version : v0.0
void GPIO_EINT4UnMask(unsigned int uEINT_No )
{

	volatile unsigned int *pMASK_Reg;
	unsigned int uConValue;

	pMASK_Reg = &(GPIO->rEINT4MASK);
	
	uConValue = *pMASK_Reg;
       uConValue = (uConValue & ~(0x1<<(uEINT_No)));
	*pMASK_Reg = uConValue;

}

//////////
// Function Name : GPIO_EINT4Mask
// Function Desctiption : Mask the Eint4[4:0]
// Input : 			uEINT_No: EINT No.
// Output : NONE
//
// Version : v0.0
void GPIO_EINT4Mask(unsigned int uEINT_No )
{

	volatile unsigned int *pMASK_Reg;
	unsigned int uConValue;

	pMASK_Reg = &(GPIO->rEINT4MASK);
	
	uConValue = *pMASK_Reg;
       uConValue = (uConValue & ~(0x1<<(uEINT_No)))|(1<<uEINT_No);
	*pMASK_Reg = uConValue;

}



//////////
// Function Name : GPIO_SetEint5
// Function Desctiption : This function setup Eint5[4:0]=> GPC1[4:0]
// Input : 			uEINT_No: EINT No.
//					uINTType: Select EINT Type. 
//								Low, High, Falling, Rising, Both
//					uFltType :  Select Filter Type
//								DisFLT(Disable Filter), DLYFLT(Delay Filter), DIGFLT(Digital Filter)
//					uFltWidth : Digital Filter Width ( 1~0x7F)
// Output : NONE
//
// Version : v0.0
void GPIO_SetEint5(unsigned int uEINT_No , unsigned int uIntType, FLT_eTYPE eFltType,  unsigned int uFltWidth)
{

	volatile unsigned int *pEINTx_Reg, *pFLTx_Reg;
	volatile unsigned int *pGPIO_EINT_Addr;
	unsigned int uGpioPort, uFunc, uType ;	
	unsigned int uConValue;

	GPIO_pBase = (void *)GPIO_BASE;

	pGPIO_EINT_Addr = &(GPIO->rEINT5CON);

	if (uEINT_No > 4)
	{
		printf("Error Eint No. \n");
		Assert(0);
	}
	
	// Check Filter Width 
	if(uFltWidth > 0x7f)
	{
		printf("Error Filter Width. \n");
		Assert(0);
	}

	//  Interrupt Type 
	if( uEINT_No <= 4)
	{
		uType =uEINT_No;
		pEINTx_Reg = pGPIO_EINT_Addr ; 	
		uConValue = *pEINTx_Reg;
		uConValue = (uConValue & ~(0xF<<(uType*4))) | (uIntType<<(uType*4));
		*pEINTx_Reg = uConValue;
	}


	// EINT Port
	if( uEINT_No <= 4)
	{
		uGpioPort = uEINT_No;
		uFunc = 0xf;  					// EINT Function 
		GPIO_SetFunctionEach(eGPIO_C1, (GPIO_eBitPos)uGpioPort, uFunc);		// ??
		// GPIO_SetPullUpDownEach(eGPIO_N,(GPIO_eBitPos) uGpioPort, 0x0);		// disable Pull-up/dn
	}
		

	// Fliter Type & Filter Width 
	if( uEINT_No <= 3)
	{
		uType =uEINT_No;
		pFLTx_Reg = &(GPIO->rEINT5FLTCON0) ; 	    // EINT3FLTCON0
		uConValue = *pFLTx_Reg;
		uConValue = (uConValue & ~(0xFF<<(uType*8))) |((uFltWidth<<(uType*8))|(eFltType<<(uType*8+7)));
		*pFLTx_Reg = uConValue;	
	}
	else if(uEINT_No = 4)
	{
		uType =uEINT_No-4;
		pFLTx_Reg =&(GPIO->rEINT5FLTCON1); 	    // EINT3FLTCON1
		uConValue = *pFLTx_Reg;
		uConValue = (uConValue & ~(0xFF<<(uType*8))) |((uFltWidth<<(uType*8))|(eFltType<<(uType*8+7)));
		*pFLTx_Reg = uConValue;	
	}
	
}



//////////
// Function Name : GPIO_EINT5ClrPend
// Function Desctiption : Clear Eint pending bit of the Eint5[4:0]
// Input : 			uEINT_No: EINT No.
// Output : NONE
//
// Version : v0.0
void GPIO_EINT5ClrPend(unsigned int uEINT_No )
{

	volatile unsigned int *pEINT5PEND;
	unsigned int uConValue;

	pEINT5PEND = &(GPIO->rEINT5PEND);

	uConValue = (1<<uEINT_No);
	*pEINT5PEND = uConValue;

}

//////////
// Function Name : GPIO_EINT5UnMask
// Function Desctiption : UnMask the Eint5[4:0]
// Input : 			uEINT_No: EINT No.
// Output : NONE
//
// Version : v0.0
void GPIO_EINT5UnMask(unsigned int uEINT_No )
{

	volatile unsigned int *pMASK_Reg;
	unsigned int uConValue;

	pMASK_Reg = &(GPIO->rEINT5MASK);
	
	uConValue = *pMASK_Reg;
       uConValue = (uConValue & ~(0x1<<(uEINT_No)));
	*pMASK_Reg = uConValue;

}

//////////
// Function Name : GPIO_EINT5Mask
// Function Desctiption : Mask the Eint5[4:0]
// Input : 			uEINT_No: EINT No.
// Output : NONE
//
// Version : v0.0
void GPIO_EINT5Mask(unsigned int uEINT_No )
{

	volatile unsigned int *pMASK_Reg;
	unsigned int uConValue;

	pMASK_Reg = &(GPIO->rEINT5MASK);
	
	uConValue = *pMASK_Reg;
       uConValue = (uConValue & ~(0x1<<(uEINT_No)))|(1<<uEINT_No);
	*pMASK_Reg = uConValue;

}



//////////
// Function Name : GPIO_SetEint6
// Function Desctiption : This function setup Eint6[3:0]=> GPD0[3:0]
// Input : 			uEINT_No: EINT No.
//					uINTType: Select EINT Type. 
//								Low, High, Falling, Rising, Both
//					uFltType :  Select Filter Type
//								DisFLT(Disable Filter), DLYFLT(Delay Filter), DIGFLT(Digital Filter)
//					uFltWidth : Digital Filter Width ( 1~0x7F)
// Output : NONE
//
// Version : v0.0
void GPIO_SetEint6(unsigned int uEINT_No , unsigned int uIntType, FLT_eTYPE eFltType,  unsigned int uFltWidth)
{

	volatile unsigned int *pEINTx_Reg, *pFLTx_Reg;
	volatile unsigned int *pGPIO_EINT_Addr;
	unsigned int uGpioPort, uFunc, uType ;	
	unsigned int uConValue;

	GPIO_pBase = (void *)GPIO_BASE;

	pGPIO_EINT_Addr = &(GPIO->rEINT6CON);

	if (uEINT_No > 3)
	{
		printf("Error Eint No. \n");
		Assert(0);
	}
	
	// Check Filter Width 
	if(uFltWidth > 0x7f)
	{
		printf("Error Filter Width. \n");
		Assert(0);
	}

	//  Interrupt Type 
	if( uEINT_No <= 3)
	{
		uType =uEINT_No;
		pEINTx_Reg = pGPIO_EINT_Addr ; 	
		uConValue = *pEINTx_Reg;
		uConValue = (uConValue & ~(0xF<<(uType*4))) | (uIntType<<(uType*4));
		*pEINTx_Reg = uConValue;
	}


	// EINT Port
	if( uEINT_No <= 3)
	{
		uGpioPort = uEINT_No;
		uFunc = 0xf;  					// EINT Function 
		GPIO_SetFunctionEach(eGPIO_D0, (GPIO_eBitPos)uGpioPort, uFunc);		// ??
		// GPIO_SetPullUpDownEach(eGPIO_N,(GPIO_eBitPos) uGpioPort, 0x0);		// disable Pull-up/dn
	}
		

	// Fliter Type & Filter Width 
	if( uEINT_No <= 3)
	{
		uType =uEINT_No;
		pFLTx_Reg = &(GPIO->rEINT6FLTCON0) ; 	    // EINT3FLTCON0
		uConValue = *pFLTx_Reg;
		uConValue = (uConValue & ~(0xFF<<(uType*8))) |((uFltWidth<<(uType*8))|(eFltType<<(uType*8+6)));
		*pFLTx_Reg = uConValue;	
	}
	
	
}



//////////
// Function Name : GPIO_EINT6ClrPend
// Function Desctiption : Clear Eint pending bit of the Eint6[3:0]
// Input : 			uEINT_No: EINT No.
// Output : NONE
//
// Version : v0.0
void GPIO_EINT6ClrPend(unsigned int uEINT_No )
{

	volatile unsigned int *pEINT6PEND;
	unsigned int uConValue;

	pEINT6PEND = &(GPIO->rEINT6PEND);

	uConValue = (1<<uEINT_No);
	*pEINT6PEND = uConValue;

}

//////////
// Function Name : GPIO_EINT6UnMask
// Function Desctiption : UnMask the Eint6[3:0]
// Input : 			uEINT_No: EINT No.
// Output : NONE
//
// Version : v0.0
void GPIO_EINT6UnMask(unsigned int uEINT_No )
{

	volatile unsigned int *pMASK_Reg;
	unsigned int uConValue;

	pMASK_Reg = &(GPIO->rEINT6MASK);
	
	uConValue = *pMASK_Reg;
       uConValue = (uConValue & ~(0x1<<(uEINT_No)));
	*pMASK_Reg = uConValue;

}

//////////
// Function Name : GPIO_EINT6Mask
// Function Desctiption : Mask the Eint6[3:0]
// Input : 			uEINT_No: EINT No.
// Output : NONE
//
// Version : v0.0
void GPIO_EINT6Mask(unsigned int uEINT_No )
{

	volatile unsigned int *pMASK_Reg;
	unsigned int uConValue;

	pMASK_Reg = &(GPIO->rEINT6MASK);
	
	uConValue = *pMASK_Reg;
       uConValue = (uConValue & ~(0x1<<(uEINT_No)))|(1<<uEINT_No);
	*pMASK_Reg = uConValue;

}



//////////
// Function Name : GPIO_SetEint7
// Function Desctiption : This function setup Eint7[5:0]=> GPD1[5:0]
// Input : 			uEINT_No: EINT No.
//					uINTType: Select EINT Type. 
//								Low, High, Falling, Rising, Both
//					uFltType :  Select Filter Type
//								DisFLT(Disable Filter), DLYFLT(Delay Filter), DIGFLT(Digital Filter)
//					uFltWidth : Digital Filter Width ( 1~0x7F)
// Output : NONE
//
// Version : v0.0
void GPIO_SetEint7(unsigned int uEINT_No , unsigned int uIntType, FLT_eTYPE eFltType,  unsigned int uFltWidth)
{

	volatile unsigned int *pEINTx_Reg, *pFLTx_Reg;
	volatile unsigned int *pGPIO_EINT_Addr;
	unsigned int uGpioPort, uFunc, uType ;	
	unsigned int uConValue;

	GPIO_pBase = (void *)GPIO_BASE;

	pGPIO_EINT_Addr = &(GPIO->rEINT7CON);

	if (uEINT_No > 5)
	{
		printf("Error Eint No. \n");
		Assert(0);
	}
	
	// Check Filter Width 
	if(uFltWidth > 0x7f)
	{
		printf("Error Filter Width. \n");
		Assert(0);
	}

	//  Interrupt Type 
	if( uEINT_No <= 5)
	{
		uType =uEINT_No;
		pEINTx_Reg = pGPIO_EINT_Addr ; 	
		uConValue = *pEINTx_Reg;
		uConValue = (uConValue & ~(0xF<<(uType*4))) | (uIntType<<(uType*4));
		*pEINTx_Reg = uConValue;
	}


	// EINT Port
	if( uEINT_No <= 5)
	{
		uGpioPort = uEINT_No;
		uFunc = 0xf;  					// EINT Function 
		GPIO_SetFunctionEach(eGPIO_D1, (GPIO_eBitPos)uGpioPort, uFunc);		// ??
		// GPIO_SetPullUpDownEach(eGPIO_N,(GPIO_eBitPos) uGpioPort, 0x0);		// disable Pull-up/dn
	}
		

	// Fliter Type & Filter Width 
	if( uEINT_No <= 3)
	{
		uType =uEINT_No;
		pFLTx_Reg = &(GPIO->rEINT7FLTCON0) ; 	    // EINT3FLTCON0
		uConValue = *pFLTx_Reg;
		uConValue = (uConValue & ~(0xFF<<(uType*8))) |((uFltWidth<<(uType*8))|(eFltType<<(uType*8+7)));
		*pFLTx_Reg = uConValue;	
	}
	else if(uEINT_No > 3 && uEINT_No <= 5)
	{
		uType =uEINT_No-4;
		pFLTx_Reg =&(GPIO->rEINT7FLTCON1); 	    // EINT3FLTCON1
		uConValue = *pFLTx_Reg;
		uConValue = (uConValue & ~(0xFF<<(uType*8))) |((uFltWidth<<(uType*8))|(eFltType<<(uType*8+7)));
		*pFLTx_Reg = uConValue;	
	}
	
}



//////////
// Function Name : GPIO_EINT7ClrPend
// Function Desctiption : Clear Eint pending bit of the Eint7[5:0]
// Input : 			uEINT_No: EINT No.
// Output : NONE
//
// Version : v0.0
void GPIO_EINT7ClrPend(unsigned int uEINT_No )
{

	volatile unsigned int *pEINT7PEND;
	unsigned int uConValue;

	pEINT7PEND = &(GPIO->rEINT7PEND);

	uConValue = (1<<uEINT_No);
	*pEINT7PEND = uConValue;

}

//////////
// Function Name : GPIO_EINT7UnMask
// Function Desctiption : UnMask the Eint7[5:0]
// Input : 			uEINT_No: EINT No.
// Output : NONE
//
// Version : v0.0
void GPIO_EINT7UnMask(unsigned int uEINT_No )
{

	volatile unsigned int *pMASK_Reg;
	unsigned int uConValue;

	pMASK_Reg = &(GPIO->rEINT7MASK);
	
	uConValue = *pMASK_Reg;
       uConValue = (uConValue & ~(0x1<<(uEINT_No)));
	*pMASK_Reg = uConValue;

}

//////////
// Function Name : GPIO_EINT7Mask
// Function Desctiption : Mask the Eint7[5:0]
// Input : 			uEINT_No: EINT No.
// Output : NONE
//
// Version : v0.0
void GPIO_EINT7Mask(unsigned int uEINT_No )
{

	volatile unsigned int *pMASK_Reg;
	unsigned int uConValue;

	pMASK_Reg = &(GPIO->rEINT7MASK);
	
	uConValue = *pMASK_Reg;
       uConValue = (uConValue & ~(0x1<<(uEINT_No)))|(1<<uEINT_No);
	*pMASK_Reg = uConValue;

}




//////////
// Function Name : GPIO_SetEint8
// Function Desctiption : This function setup Eint8[7:0]=> GPE0[7:0]
// Input : 			uEINT_No: EINT No.
//					uINTType: Select EINT Type. 
//								Low, High, Falling, Rising, Both
//					uFltType :  Select Filter Type
//								DisFLT(Disable Filter), DLYFLT(Delay Filter), DIGFLT(Digital Filter)
//					uFltWidth : Digital Filter Width ( 1~0x7F)
// Output : NONE
//
// Version : v0.0
void GPIO_SetEint8(unsigned int uEINT_No , unsigned int uIntType, FLT_eTYPE eFltType,  unsigned int uFltWidth)
{

	volatile unsigned int *pEINTx_Reg, *pFLTx_Reg;
	volatile unsigned int *pGPIO_EINT_Addr;
	unsigned int uGpioPort, uFunc, uType ;	
	unsigned int uConValue;

	GPIO_pBase = (void *)GPIO_BASE;

	pGPIO_EINT_Addr = &(GPIO->rEINT8CON);

	if (uEINT_No > 7)
	{
		printf("Error Eint No. \n");
		Assert(0);
	}
	
	// Check Filter Width 
	if(uFltWidth > 0x7f)
	{
		printf("Error Filter Width. \n");
		Assert(0);
	}

	//  Interrupt Type 
	if( uEINT_No <= 7)
	{
		uType =uEINT_No;
		pEINTx_Reg = pGPIO_EINT_Addr ; 	
		uConValue = *pEINTx_Reg;
		uConValue = (uConValue & ~(0xF<<(uType*4))) | (uIntType<<(uType*4));
		*pEINTx_Reg = uConValue;
	}


	// EINT Port
	if( uEINT_No <= 7)
	{
		uGpioPort = uEINT_No;
		uFunc = 0xf;  					// EINT Function 
		GPIO_SetFunctionEach(eGPIO_E0, (GPIO_eBitPos)uGpioPort, uFunc);		// ??
		// GPIO_SetPullUpDownEach(eGPIO_N,(GPIO_eBitPos) uGpioPort, 0x0);		// disable Pull-up/dn
	}
		

	// Fliter Type & Filter Width 
	if( uEINT_No <= 3)
	{
		uType =uEINT_No;
		pFLTx_Reg = &(GPIO->rEINT8FLTCON0) ; 	    // EINT3FLTCON0
		uConValue = *pFLTx_Reg;
		uConValue = (uConValue & ~(0xFF<<(uType*8))) |((uFltWidth<<(uType*8))|(eFltType<<(uType*8+7)));
		*pFLTx_Reg = uConValue;	
	}
	else if(uEINT_No > 3 && uEINT_No <= 7)
	{
		uType =uEINT_No-4;
		pFLTx_Reg =&(GPIO->rEINT8FLTCON1); 	    // EINT3FLTCON1
		uConValue = *pFLTx_Reg;
		uConValue = (uConValue & ~(0xFF<<(uType*8))) |((uFltWidth<<(uType*8))|(eFltType<<(uType*8+7)));
		*pFLTx_Reg = uConValue;	
	}
	
}



//////////
// Function Name : GPIO_EINT8ClrPend
// Function Desctiption : Clear Eint pending bit of the Eint8[7:0]
// Input : 			uEINT_No: EINT No.
// Output : NONE
//
// Version : v0.0
void GPIO_EINT8ClrPend(unsigned int uEINT_No )
{

	volatile unsigned int *pEINT8PEND;
	unsigned int uConValue;

	pEINT8PEND = &(GPIO->rEINT8PEND);

	uConValue = (1<<uEINT_No);
	*pEINT8PEND = uConValue;

}

//////////
// Function Name : GPIO_EINT8UnMask
// Function Desctiption : UnMask the Eint8[7:0]
// Input : 			uEINT_No: EINT No.
// Output : NONE
//
// Version : v0.0
void GPIO_EINT8UnMask(unsigned int uEINT_No )
{

	volatile unsigned int *pMASK_Reg;
	unsigned int uConValue;

	pMASK_Reg = &(GPIO->rEINT8MASK);
	
	uConValue = *pMASK_Reg;
       uConValue = (uConValue & ~(0x1<<(uEINT_No)));
	*pMASK_Reg = uConValue;

}

//////////
// Function Name : GPIO_EINT8Mask
// Function Desctiption : Mask the Eint8[7:0]
// Input : 			uEINT_No: EINT No.
// Output : NONE
//
// Version : v0.0
void GPIO_EINT8Mask(unsigned int uEINT_No )
{

	volatile unsigned int *pMASK_Reg;
	unsigned int uConValue;

	pMASK_Reg = &(GPIO->rEINT8MASK);
	
	uConValue = *pMASK_Reg;
       uConValue = (uConValue & ~(0x1<<(uEINT_No)))|(1<<uEINT_No);
	*pMASK_Reg = uConValue;

}


//////////
// Function Name : GPIO_SetEint9
// Function Desctiption : This function setup Eint9[4:0]=> GPE1[4:0]
// Input : 			uEINT_No: EINT No.
//					uINTType: Select EINT Type. 
//								Low, High, Falling, Rising, Both
//					uFltType :  Select Filter Type
//								DisFLT(Disable Filter), DLYFLT(Delay Filter), DIGFLT(Digital Filter)
//					uFltWidth : Digital Filter Width ( 1~0x7F)
// Output : NONE
//
// Version : v0.0
void GPIO_SetEint9(unsigned int uEINT_No , unsigned int uIntType, FLT_eTYPE eFltType,  unsigned int uFltWidth)
{

	volatile unsigned int *pEINTx_Reg, *pFLTx_Reg;
	volatile unsigned int *pGPIO_EINT_Addr;
	unsigned int uGpioPort, uFunc, uType ;	
	unsigned int uConValue;

	GPIO_pBase = (void *)GPIO_BASE;

	pGPIO_EINT_Addr = &(GPIO->rEINT9CON);

	if (uEINT_No > 4)
	{
		printf("Error Eint No. \n");
		Assert(0);
	}
	
	// Check Filter Width 
	if(uFltWidth > 0x7f)
	{
		printf("Error Filter Width. \n");
		Assert(0);
	}

	//  Interrupt Type 
	if( uEINT_No <= 4)
	{
		uType =uEINT_No;
		pEINTx_Reg = pGPIO_EINT_Addr ; 	
		uConValue = *pEINTx_Reg;
		uConValue = (uConValue & ~(0xF<<(uType*4))) | (uIntType<<(uType*4));
		*pEINTx_Reg = uConValue;
	}


	// EINT Port
	if( uEINT_No <= 4)
	{
		uGpioPort = uEINT_No;
		uFunc = 0xf;  					// EINT Function 
		GPIO_SetFunctionEach(eGPIO_E1, (GPIO_eBitPos)uGpioPort, uFunc);		// ??
		// GPIO_SetPullUpDownEach(eGPIO_N,(GPIO_eBitPos) uGpioPort, 0x0);		// disable Pull-up/dn
	}
		

	// Fliter Type & Filter Width 
	if( uEINT_No <= 3)
	{
		uType =uEINT_No;
		pFLTx_Reg = &(GPIO->rEINT9FLTCON0) ; 	    // EINT3FLTCON0
		uConValue = *pFLTx_Reg;
		uConValue = (uConValue & ~(0xFF<<(uType*8))) |((uFltWidth<<(uType*8))|(eFltType<<(uType*8+7)));
		*pFLTx_Reg = uConValue;	
	}
	else if(uEINT_No = 4)
	{
		uType =uEINT_No-4;
		pFLTx_Reg =&(GPIO->rEINT9FLTCON1); 	    // EINT3FLTCON1
		uConValue = *pFLTx_Reg;
		uConValue = (uConValue & ~(0xFF<<(uType*8))) |((uFltWidth<<(uType*8))|(eFltType<<(uType*8+7)));
		*pFLTx_Reg = uConValue;	
	}
	
}



//////////
// Function Name : GPIO_EINT9ClrPend
// Function Desctiption : Clear Eint pending bit of the Eint9[4:0]
// Input : 			uEINT_No: EINT No.
// Output : NONE
//
// Version : v0.0
void GPIO_EINT9ClrPend(unsigned int uEINT_No )
{

	volatile unsigned int *pEINT9PEND;
	unsigned int uConValue;

	pEINT9PEND = &(GPIO->rEINT9PEND);

	uConValue = (1<<uEINT_No);
	*pEINT9PEND = uConValue;

}

//////////
// Function Name : GPIO_EINT9UnMask
// Function Desctiption : UnMask the Eint9[4:0]
// Input : 			uEINT_No: EINT No.
// Output : NONE
//
// Version : v0.0
void GPIO_EINT9UnMask(unsigned int uEINT_No )
{

	volatile unsigned int *pMASK_Reg;
	unsigned int uConValue;

	pMASK_Reg = &(GPIO->rEINT9MASK);
	
	uConValue = *pMASK_Reg;
       uConValue = (uConValue & ~(0x1<<(uEINT_No)));
	*pMASK_Reg = uConValue;

}

//////////
// Function Name : GPIO_EINT9Mask
// Function Desctiption : Mask the Eint9[4:0]
// Input : 			uEINT_No: EINT No.
// Output : NONE
//
// Version : v0.0
void GPIO_EINT9Mask(unsigned int uEINT_No )
{

	volatile unsigned int *pMASK_Reg;
	unsigned int uConValue;

	pMASK_Reg = &(GPIO->rEINT9MASK);
	
	uConValue = *pMASK_Reg;
       uConValue = (uConValue & ~(0x1<<(uEINT_No)))|(1<<uEINT_No);
	*pMASK_Reg = uConValue;

}



//////////
// Function Name : GPIO_SetEint10
// Function Desctiption : This function setup Eint10[7:0]=> GPF0[7:0]
// Input : 			uEINT_No: EINT No.
//					uINTType: Select EINT Type. 
//								Low, High, Falling, Rising, Both
//					uFltType :  Select Filter Type
//								DisFLT(Disable Filter), DLYFLT(Delay Filter), DIGFLT(Digital Filter)
//					uFltWidth : Digital Filter Width ( 1~0x7F)
// Output : NONE
//
// Version : v0.0
void GPIO_SetEint10(unsigned int uEINT_No , unsigned int uIntType, FLT_eTYPE eFltType,  unsigned int uFltWidth)
{

	volatile unsigned int *pEINTx_Reg, *pFLTx_Reg;
	volatile unsigned int *pGPIO_EINT_Addr;
	unsigned int uGpioPort, uFunc, uType ;	
	unsigned int uConValue;

	GPIO_pBase = (void *)GPIO_BASE;

	pGPIO_EINT_Addr = &(GPIO->rEINT10CON);

	if (uEINT_No > 7)
	{
		printf("Error Eint No. \n");
		Assert(0);
	}
	
	// Check Filter Width 
	if(uFltWidth > 0x7f)
	{
		printf("Error Filter Width. \n");
		Assert(0);
	}

	//  Interrupt Type 
	if( uEINT_No <= 7)
	{
		uType =uEINT_No;
		pEINTx_Reg = pGPIO_EINT_Addr ; 	
		uConValue = *pEINTx_Reg;
		uConValue = (uConValue & ~(0xF<<(uType*4))) | (uIntType<<(uType*4));
		*pEINTx_Reg = uConValue;
	}


	// EINT Port
	if( uEINT_No <= 7)
	{
		uGpioPort = uEINT_No;
		uFunc = 0xf;  					// EINT Function 
		GPIO_SetFunctionEach(eGPIO_F0, (GPIO_eBitPos)uGpioPort, uFunc);		// ??
		// GPIO_SetPullUpDownEach(eGPIO_N,(GPIO_eBitPos) uGpioPort, 0x0);		// disable Pull-up/dn
	}
		

	// Fliter Type & Filter Width 
	if( uEINT_No <= 3)
	{
		uType =uEINT_No;
		pFLTx_Reg = &(GPIO->rEINT10FLTCON0) ; 	    // EINT3FLTCON0
		uConValue = *pFLTx_Reg;
		uConValue = (uConValue & ~(0xFF<<(uType*8))) |((uFltWidth<<(uType*8))|(eFltType<<(uType*8+7)));
		*pFLTx_Reg = uConValue;	
	}
	else if(uEINT_No >3 && uEINT_No <=7)
	{
		uType =uEINT_No-4;
		pFLTx_Reg =&(GPIO->rEINT10FLTCON1); 	    // EINT3FLTCON1
		uConValue = *pFLTx_Reg;
		uConValue = (uConValue & ~(0xFF<<(uType*8))) |((uFltWidth<<(uType*8))|(eFltType<<(uType*8+7)));
		*pFLTx_Reg = uConValue;	
	}
	
}



//////////
// Function Name : GPIO_EINT10ClrPend
// Function Desctiption : Clear Eint pending bit of the Eint10[7:0]
// Input : 			uEINT_No: EINT No.
// Output : NONE
//
// Version : v0.0
void GPIO_EINT10ClrPend(unsigned int uEINT_No )
{

	volatile unsigned int *pEINT10PEND;
	unsigned int uConValue;

	pEINT10PEND = &(GPIO->rEINT10PEND);

	uConValue = (1<<uEINT_No);
	*pEINT10PEND = uConValue;

}

//////////
// Function Name : GPIO_EINT10UnMask
// Function Desctiption : UnMask the Eint10[7:0]
// Input : 			uEINT_No: EINT No.
// Output : NONE
//
// Version : v0.0
void GPIO_EINT10UnMask(unsigned int uEINT_No )
{

	volatile unsigned int *pMASK_Reg;
	unsigned int uConValue;

	pMASK_Reg = &(GPIO->rEINT10MASK);
	
	uConValue = *pMASK_Reg;
       uConValue = (uConValue & ~(0x1<<(uEINT_No)));
	*pMASK_Reg = uConValue;

}

//////////
// Function Name : GPIO_EINT10Mask
// Function Desctiption : Mask the Eint10[7:0]
// Input : 			uEINT_No: EINT No.
// Output : NONE
//
// Version : v0.0
void GPIO_EINT10Mask(unsigned int uEINT_No )
{

	volatile unsigned int *pMASK_Reg;
	unsigned int uConValue;

	pMASK_Reg = &(GPIO->rEINT10MASK);
	
	uConValue = *pMASK_Reg;
       uConValue = (uConValue & ~(0x1<<(uEINT_No)))|(1<<uEINT_No);
	*pMASK_Reg = uConValue;

}





//////////
// Function Name : GPIO_SetEint11
// Function Desctiption : This function setup Eint11[7:0]=> GPF1[7:0]
// Input : 			uEINT_No: EINT No.
//					uINTType: Select EINT Type. 
//								Low, High, Falling, Rising, Both
//					uFltType :  Select Filter Type
//								DisFLT(Disable Filter), DLYFLT(Delay Filter), DIGFLT(Digital Filter)
//					uFltWidth : Digital Filter Width ( 1~0x7F)
// Output : NONE
//
// Version : v0.0
void GPIO_SetEint11(unsigned int uEINT_No , unsigned int uIntType, FLT_eTYPE eFltType,  unsigned int uFltWidth)
{

	volatile unsigned int *pEINTx_Reg, *pFLTx_Reg;
	volatile unsigned int *pGPIO_EINT_Addr;
	unsigned int uGpioPort, uFunc, uType ;	
	unsigned int uConValue;

	GPIO_pBase = (void *)GPIO_BASE;

	pGPIO_EINT_Addr = &(GPIO->rEINT11CON);

	if (uEINT_No > 7)
	{
		printf("Error Eint No. \n");
		Assert(0);
	}
	
	// Check Filter Width 
	if(uFltWidth >0x7f)
	{
		printf("Error Filter Width. \n");
		Assert(0);
	}

	//  Interrupt Type 
	if( uEINT_No <= 7)
	{
		uType =uEINT_No;
		pEINTx_Reg = pGPIO_EINT_Addr ; 	
		uConValue = *pEINTx_Reg;
		uConValue = (uConValue & ~(0xF<<(uType*4))) | (uIntType<<(uType*4));
		*pEINTx_Reg = uConValue;
	}


	// EINT Port
	if( uEINT_No <= 7)
	{
		uGpioPort = uEINT_No;
		uFunc = 0xf;  					// EINT Function 
		GPIO_SetFunctionEach(eGPIO_F1, (GPIO_eBitPos)uGpioPort, uFunc);		// ??
		// GPIO_SetPullUpDownEach(eGPIO_N,(GPIO_eBitPos) uGpioPort, 0x0);		// disable Pull-up/dn
	}
		

	// Fliter Type & Filter Width 
	if( uEINT_No <= 3)
	{
		uType =uEINT_No;
		pFLTx_Reg = &(GPIO->rEINT11FLTCON0) ; 	    // EINT11FLTCON0
		uConValue = *pFLTx_Reg;
		uConValue = (uConValue & ~(0xFF<<(uType*8))) |((uFltWidth<<(uType*8))|(eFltType<<(uType*8+7)));
		*pFLTx_Reg = uConValue;	
	}
	else if(uEINT_No > 3 && uEINT_No <= 7)
	{
		uType =uEINT_No-4;
		pFLTx_Reg =&(GPIO->rEINT11FLTCON1); 	    // EINT11FLTCON1
		uConValue = *pFLTx_Reg;
		uConValue = (uConValue & ~(0xFF<<(uType*8))) |((uFltWidth<<(uType*8))|(eFltType<<(uType*8+7)));
		*pFLTx_Reg = uConValue;	
	}
	
}



//////////
// Function Name : GPIO_EINT11ClrPend
// Function Desctiption : Clear Eint pending bit of the Eint11[7:0]
// Input : 			uEINT_No: EINT No.
// Output : NONE
//
// Version : v0.0
void GPIO_EINT11ClrPend(unsigned int uEINT_No )
{

	volatile unsigned int *pEINT11PEND;
	unsigned int uConValue;

	pEINT11PEND = &(GPIO->rEINT11PEND);

	uConValue = (1<<uEINT_No);
	*pEINT11PEND = uConValue;

}

//////////
// Function Name : GPIO_EINT11UnMask
// Function Desctiption : UnMask the Eint11[7:0]
// Input : 			uEINT_No: EINT No.
// Output : NONE
//
// Version : v0.0
void GPIO_EINT11UnMask(unsigned int uEINT_No )
{

	volatile unsigned int *pMASK_Reg;
	unsigned int uConValue;

	pMASK_Reg = &(GPIO->rEINT11MASK);
	
	uConValue = *pMASK_Reg;
       uConValue = (uConValue & ~(0x1<<(uEINT_No)));
	*pMASK_Reg = uConValue;

}

//////////
// Function Name : GPIO_EINT11Mask
// Function Desctiption : Mask the Eint11[7:0]
// Input : 			uEINT_No: EINT No.
// Output : NONE
//
// Version : v0.0
void GPIO_EINT11Mask(unsigned int uEINT_No )
{

	volatile unsigned int *pMASK_Reg;
	unsigned int uConValue;

	pMASK_Reg = &(GPIO->rEINT11MASK);
	
	uConValue = *pMASK_Reg;
       uConValue = (uConValue & ~(0x1<<(uEINT_No)))|(1<<uEINT_No);
	*pMASK_Reg = uConValue;

}




//////////
// Function Name : GPIO_SetEint12
// Function Desctiption : This function setup Eint12[7:0]=> GPF2[7:0]
// Input : 			uEINT_No: EINT No.
//					uINTType: Select EINT Type. 
//								Low, High, Falling, Rising, Both
//					uFltType :  Select Filter Type
//								DisFLT(Disable Filter), DLYFLT(Delay Filter), DIGFLT(Digital Filter)
//					uFltWidth : Digital Filter Width ( 1~0x7F)
// Output : NONE
//
// Version : v0.0
void GPIO_SetEint12(unsigned int uEINT_No , unsigned int uIntType, FLT_eTYPE eFltType,  unsigned int uFltWidth)
{

	volatile unsigned int *pEINTx_Reg, *pFLTx_Reg;
	volatile unsigned int *pGPIO_EINT_Addr;
	unsigned int uGpioPort, uFunc, uType ;	
	unsigned int uConValue;

	GPIO_pBase = (void *)GPIO_BASE;

	pGPIO_EINT_Addr = &(GPIO->rEINT12CON);

	
	if (uEINT_No >7)
	{
		printf("Error Eint No. \n");
		Assert(0);
	}
	

	// Check Filter Width 
	if(uFltWidth >0x7f)
	{
		printf("Error Filter Width. \n");
		Assert(0);
	}
	

	//  Interrupt Type 
	if( uEINT_No <= 7)
	{
		uType =uEINT_No;
		pEINTx_Reg = pGPIO_EINT_Addr ; 	
		uConValue = *pEINTx_Reg;
		uConValue = (uConValue & ~(0xF<<(uType*4))) | (uIntType<<(uType*4));
		*pEINTx_Reg = uConValue;
	}


	// EINT Port
	if( uEINT_No <= 7)
	{
		uGpioPort = uEINT_No;
		uFunc = 0xf;  					// EINT Function 
		GPIO_SetFunctionEach(eGPIO_F2, (GPIO_eBitPos)uGpioPort, uFunc);		// ??
		// GPIO_SetPullUpDownEach(eGPIO_N,(GPIO_eBitPos) uGpioPort, 0x0);		// disable Pull-up/dn
	}

	// Fliter Type & Filter Width 
	if( uEINT_No <= 3)
	{
		uType =uEINT_No;
		pFLTx_Reg = &(GPIO->rEINT12FLTCON0) ; 	    // EINT12FLTCON0
		uConValue = *pFLTx_Reg;
		uConValue = (uConValue & ~(0xFF<<(uType*8))) |((uFltWidth<<(uType*8))|(eFltType<<(uType*8+7)));
		*pFLTx_Reg = uConValue;	
	}
	else if(uEINT_No > 3 && uEINT_No <= 7)
	{
		uType =uEINT_No-4;
		pFLTx_Reg =&(GPIO->rEINT12FLTCON1); 	    // EINT12FLTCON1
		uConValue = *pFLTx_Reg;
		uConValue = (uConValue & ~(0xFF<<(uType*8))) |((uFltWidth<<(uType*8))|(eFltType<<(uType*8+7)));
		*pFLTx_Reg = uConValue;	
	}
	
	
}



//////////
// Function Name : GPIO_EINT12ClrPend
// Function Desctiption : Clear Eint pending bit of the Eint12[7:0]
// Input : 			uEINT_No: EINT No.
// Output : NONE
//
// Version : v0.0
void GPIO_EINT12ClrPend(unsigned int uEINT_No )
{

	volatile unsigned int *pEINT12PEND;
	unsigned int uConValue;

	pEINT12PEND = &(GPIO->rEINT12PEND);

	uConValue = (1<<uEINT_No);
	*pEINT12PEND = uConValue;

}

//////////
// Function Name : GPIO_EINT12UnMask
// Function Desctiption : UnMask the Eint12[7:0]
// Input : 			uEINT_No: EINT No.
// Output : NONE
//
// Version : v0.0
void GPIO_EINT12UnMask(unsigned int uEINT_No )
{

	volatile unsigned int *pMASK_Reg;
	unsigned int uConValue;

	pMASK_Reg = &(GPIO->rEINT12MASK);
	
	uConValue = *pMASK_Reg;
       uConValue = (uConValue & ~(0x1<<(uEINT_No)));
	*pMASK_Reg = uConValue;

}

//////////
// Function Name : GPIO_EINT12Mask
// Function Desctiption : Mask the Eint12[7:0]
// Input : 			uEINT_No: EINT No.
// Output : NONE
//
// Version : v0.0
void GPIO_EINT12Mask(unsigned int uEINT_No )
{

	volatile unsigned int *pMASK_Reg;
	unsigned int uConValue;

	pMASK_Reg = &(GPIO->rEINT12MASK);
	
	uConValue = *pMASK_Reg;
       uConValue = (uConValue & ~(0x1<<(uEINT_No)))|(1<<uEINT_No);
	*pMASK_Reg = uConValue;

}



//////////
// Function Name : GPIO_SetEint13
// Function Desctiption : This function setup Eint13[5:0]=> GPF3[5:0]
// Input : 			uEINT_No: EINT No.
//					uINTType: Select EINT Type. 
//								Low, High, Falling, Rising, Both
//					uFltType :  Select Filter Type
//								DisFLT(Disable Filter), DLYFLT(Delay Filter), DIGFLT(Digital Filter)
//					uFltWidth : Digital Filter Width ( 1~0x7F)
// Output : NONE
//
// Version : v0.0
void GPIO_SetEint13(unsigned int uEINT_No , unsigned int uIntType, FLT_eTYPE eFltType,  unsigned int uFltWidth)
{

	volatile unsigned int *pEINTx_Reg, *pFLTx_Reg;
	volatile unsigned int *pGPIO_EINT_Addr;
	unsigned int uGpioPort, uFunc, uType ;	
	unsigned int uConValue;

	GPIO_pBase = (void *)GPIO_BASE;

	pGPIO_EINT_Addr = &(GPIO->rEINT13CON);

	if (uEINT_No > 5)
	{
		printf("Error Eint No. \n");
		Assert(0);
	}
	
	// Check Filter Width 
	if(uFltWidth >0x7f)
	{
		printf("Error Filter Width. \n");
		Assert(0);
	}

	//  Interrupt Type 
	if( uEINT_No <= 5)
	{
		uType =uEINT_No;
		pEINTx_Reg = pGPIO_EINT_Addr ; 	
		uConValue = *pEINTx_Reg;
		uConValue = (uConValue & ~(0xF<<(uType*4))) | (uIntType<<(uType*4));
		*pEINTx_Reg = uConValue;
	}


	// EINT Port
	if( uEINT_No <= 5)
	{
		uGpioPort = uEINT_No;
		uFunc = 0xf;  					// EINT Function 
		GPIO_SetFunctionEach(eGPIO_F3, (GPIO_eBitPos)uGpioPort, uFunc);		// ??
		// GPIO_SetPullUpDownEach(eGPIO_N,(GPIO_eBitPos) uGpioPort, 0x0);		// disable Pull-up/dn
	}
		

	// Fliter Type & Filter Width 
	if( uEINT_No <= 3)
	{
		uType =uEINT_No;
		pFLTx_Reg = &(GPIO->rEINT13FLTCON0) ; 	    // EINT13FLTCON0
		uConValue = *pFLTx_Reg;
		uConValue = (uConValue & ~(0xFF<<(uType*8))) |((uFltWidth<<(uType*8))|(eFltType<<(uType*8+7)));
		*pFLTx_Reg = uConValue;	
	}
	else if(uEINT_No >3 && uEINT_No <= 5)
	{
		uType =uEINT_No-4;
		pFLTx_Reg =&(GPIO->rEINT13FLTCON1); 	    // EINT13FLTCON1
		uConValue = *pFLTx_Reg;
		uConValue = (uConValue & ~(0xFF<<(uType*8))) |((uFltWidth<<(uType*8))|(eFltType<<(uType*8+7)));
		*pFLTx_Reg = uConValue;	
	}
	
}



//////////
// Function Name : GPIO_EINT13ClrPend
// Function Desctiption : Clear Eint pending bit of the Eint13[5:0]
// Input : 			uEINT_No: EINT No.
// Output : NONE
//
// Version : v0.0
void GPIO_EINT13ClrPend(unsigned int uEINT_No )
{

	volatile unsigned int *pEINT13PEND;
	unsigned int uConValue;

	pEINT13PEND = &(GPIO->rEINT13PEND);

	uConValue = (1<<uEINT_No);
	*pEINT13PEND = uConValue;

}

//////////
// Function Name : GPIO_EINT13UnMask
// Function Desctiption : UnMask the Eint13[5:0]
// Input : 			uEINT_No: EINT No.
// Output : NONE
//
// Version : v0.0
void GPIO_EINT13UnMask(unsigned int uEINT_No )
{

	volatile unsigned int *pMASK_Reg;
	unsigned int uConValue;

	pMASK_Reg = &(GPIO->rEINT13MASK);
	
	uConValue = *pMASK_Reg;
       uConValue = (uConValue & ~(0x1<<(uEINT_No)));
	*pMASK_Reg = uConValue;

}

//////////
// Function Name : GPIO_EINT13Mask
// Function Desctiption : Mask the Eint13[5:0]
// Input : 			uEINT_No: EINT No.
// Output : NONE
//
// Version : v0.0
void GPIO_EINT13Mask(unsigned int uEINT_No )
{

	volatile unsigned int *pMASK_Reg;
	unsigned int uConValue;

	pMASK_Reg = &(GPIO->rEINT13MASK);
	
	uConValue = *pMASK_Reg;
       uConValue = (uConValue & ~(0x1<<(uEINT_No)))|(1<<uEINT_No);
	*pMASK_Reg = uConValue;

}




//////////
// Function Name : GPIO_SetEint14
// Function Desctiption : This function setup Eint14[6:0]=> GPG0[6:0]
// Input : 			uEINT_No: EINT No.
//					uINTType: Select EINT Type. 
//								Low, High, Falling, Rising, Both
//					uFltType :  Select Filter Type
//								DisFLT(Disable Filter), DLYFLT(Delay Filter), DIGFLT(Digital Filter)
//					uFltWidth : Digital Filter Width ( 1~0x7F)
// Output : NONE
//
// Version : v0.0
void GPIO_SetEint14(unsigned int uEINT_No , unsigned int uIntType, FLT_eTYPE eFltType,  unsigned int uFltWidth)
{

	volatile unsigned int *pEINTx_Reg, *pFLTx_Reg;
	volatile unsigned int *pGPIO_EINT_Addr;
	unsigned int uGpioPort, uFunc, uType ;	
	unsigned int uConValue;

	GPIO_pBase = (void *)GPIO_BASE;

	pGPIO_EINT_Addr = &(GPIO->rEINT14CON);

	if (uEINT_No > 6)
	{
		printf("Error Eint No. \n");
		Assert(0);
	}
	
	// Check Filter Width 
	if(uFltWidth > 0x7f)
	{
		printf("Error Filter Width. \n");
		Assert(0);
	}

	//  Interrupt Type 
	if( uEINT_No <= 6)
	{
		uType =uEINT_No;
		pEINTx_Reg = pGPIO_EINT_Addr ; 	
		uConValue = *pEINTx_Reg;
		uConValue = (uConValue & ~(0xF<<(uType*4))) | (uIntType<<(uType*4));
		*pEINTx_Reg = uConValue;
	}


	// EINT Port
	if( uEINT_No <= 6)
	{
		uGpioPort = uEINT_No;
		uFunc = 0xf;  					// EINT Function 
		GPIO_SetFunctionEach(eGPIO_G0, (GPIO_eBitPos)uGpioPort, uFunc);		// ??
		// GPIO_SetPullUpDownEach(eGPIO_N,(GPIO_eBitPos) uGpioPort, 0x0);		// disable Pull-up/dn
	}
		

	// Fliter Type & Filter Width 
	if( uEINT_No <= 3)
	{
		uType =uEINT_No;
		pFLTx_Reg = &(GPIO->rEINT14FLTCON0) ; 	    // EINT14FLTCON0
		uConValue = *pFLTx_Reg;
		uConValue = (uConValue & ~(0xFF<<(uType*8))) |((uFltWidth<<(uType*8))|(eFltType<<(uType*8+7)));
		*pFLTx_Reg = uConValue;	
	}
	else if(uEINT_No >3 && uEINT_No <= 6)
	{
		uType =uEINT_No-4;
		pFLTx_Reg =&(GPIO->rEINT14FLTCON1); 	    // EINT14FLTCON1
		uConValue = *pFLTx_Reg;
		uConValue = (uConValue & ~(0xFF<<(uType*8))) |((uFltWidth<<(uType*8))|(eFltType<<(uType*8+7)));
		*pFLTx_Reg = uConValue;	
	}
	
}



//////////
// Function Name : GPIO_EINT14ClrPend
// Function Desctiption : Clear Eint pending bit of the Eint14[6:0]
// Input : 			uEINT_No: EINT No.
// Output : NONE
//
// Version : v0.0
void GPIO_EINT14ClrPend(unsigned int uEINT_No )
{

	volatile unsigned int *pEINT14PEND;
	unsigned int uConValue;

	pEINT14PEND = &(GPIO->rEINT14PEND);

	uConValue = (1<<uEINT_No);
	*pEINT14PEND = uConValue;

}

//////////
// Function Name : GPIO_EINT14UnMask
// Function Desctiption : UnMask the Eint14[6:0]
// Input : 			uEINT_No: EINT No.
// Output : NONE
//
// Version : v0.0
void GPIO_EINT14UnMask(unsigned int uEINT_No )
{

	volatile unsigned int *pMASK_Reg;
	unsigned int uConValue;

	pMASK_Reg = &(GPIO->rEINT14MASK);
	
	uConValue = *pMASK_Reg;
       uConValue = (uConValue & ~(0x1<<(uEINT_No)));
	*pMASK_Reg = uConValue;

}

//////////
// Function Name : GPIO_EINT14Mask
// Function Desctiption : Mask the Eint14[6:0]
// Input : 			uEINT_No: EINT No.
// Output : NONE
//
// Version : v0.0
void GPIO_EINT14Mask(unsigned int uEINT_No )
{

	volatile unsigned int *pMASK_Reg;
	unsigned int uConValue;

	pMASK_Reg = &(GPIO->rEINT14MASK);
	
	uConValue = *pMASK_Reg;
       uConValue = (uConValue & ~(0x1<<(uEINT_No)))|(1<<uEINT_No);
	*pMASK_Reg = uConValue;

}



//////////
// Function Name : GPIO_SetEint15
// Function Desctiption : This function setup Eint15[6:0]=> GPG1[6:0]
// Input : 			uEINT_No: EINT No.
//					uINTType: Select EINT Type. 
//								Low, High, Falling, Rising, Both
//					uFltType :  Select Filter Type
//								DisFLT(Disable Filter), DLYFLT(Delay Filter), DIGFLT(Digital Filter)
//					uFltWidth : Digital Filter Width ( 1~0x7F)
// Output : NONE
//
// Version : v0.0
void GPIO_SetEint15(unsigned int uEINT_No , unsigned int uIntType, FLT_eTYPE eFltType,  unsigned int uFltWidth)
{

	volatile unsigned int *pEINTx_Reg, *pFLTx_Reg;
	volatile unsigned int *pGPIO_EINT_Addr;
	unsigned int uGpioPort, uFunc, uType ;	
	unsigned int uConValue;

	GPIO_pBase = (void *)GPIO_BASE;

	pGPIO_EINT_Addr = &(GPIO->rEINT15CON);

	if (uEINT_No > 6)
	{
		printf("Error Eint No. \n");
		Assert(0);
	}
	
	// Check Filter Width 
	if(uFltWidth > 0x7f)
	{
		printf("Error Filter Width. \n");
		Assert(0);
	}

	//  Interrupt Type 
	if( uEINT_No <= 6)
	{
		uType =uEINT_No;
		pEINTx_Reg = pGPIO_EINT_Addr ; 	
		uConValue = *pEINTx_Reg;
		uConValue = (uConValue & ~(0xF<<(uType*4))) | (uIntType<<(uType*4));
		*pEINTx_Reg = uConValue;
	}


	// EINT Port
	if( uEINT_No <= 6)
	{
		uGpioPort = uEINT_No;
		uFunc = 0xf;  					// EINT Function 
		GPIO_SetFunctionEach(eGPIO_G1, (GPIO_eBitPos)uGpioPort, uFunc);		// ??
		// GPIO_SetPullUpDownEach(eGPIO_N,(GPIO_eBitPos) uGpioPort, 0x0);		// disable Pull-up/dn
	}
		

	// Fliter Type & Filter Width 
	if( uEINT_No <= 3)
	{
		uType =uEINT_No;
		pFLTx_Reg = &(GPIO->rEINT15FLTCON0) ; 	    // EINT3FLTCON0
		uConValue = *pFLTx_Reg;
		uConValue = (uConValue & ~(0xFF<<(uType*8))) |((uFltWidth<<(uType*8))|(eFltType<<(uType*8+7)));
		*pFLTx_Reg = uConValue;	
	}
	else if(uEINT_No >3 && uEINT_No <= 6)
	{
		uType =uEINT_No-4;
		pFLTx_Reg =&(GPIO->rEINT15FLTCON1); 	    // EINT3FLTCON1
		uConValue = *pFLTx_Reg;
		uConValue = (uConValue & ~(0xFF<<(uType*8))) |((uFltWidth<<(uType*8))|(eFltType<<(uType*8+7)));
		*pFLTx_Reg = uConValue;	
	}
	
}



//////////
// Function Name : GPIO_EINT15ClrPend
// Function Desctiption : Clear Eint pending bit of the Eint15[6:0]
// Input : 			uEINT_No: EINT No.
// Output : NONE
//
// Version : v0.0
void GPIO_EINT15ClrPend(unsigned int uEINT_No )
{

	volatile unsigned int *pEINT15PEND;
	unsigned int uConValue;

	pEINT15PEND = &(GPIO->rEINT15PEND);

	uConValue = (1<<uEINT_No);
	*pEINT15PEND = uConValue;

}

//////////
// Function Name : GPIO_EINT15UnMask
// Function Desctiption : UnMask the Eint15[6:0]
// Input : 			uEINT_No: EINT No.
// Output : NONE
//
// Version : v0.0
void GPIO_EINT15UnMask(unsigned int uEINT_No )
{

	volatile unsigned int *pMASK_Reg;
	unsigned int uConValue;

	pMASK_Reg = &(GPIO->rEINT15MASK);
	
	uConValue = *pMASK_Reg;
       uConValue = (uConValue & ~(0x1<<(uEINT_No)));
	*pMASK_Reg = uConValue;

}

//////////
// Function Name : GPIO_EINT15Mask
// Function Desctiption : Mask the Eint15[6:0]
// Input : 			uEINT_No: EINT No.
// Output : NONE
//
// Version : v0.0
void GPIO_EINT15Mask(unsigned int uEINT_No )
{

	volatile unsigned int *pMASK_Reg;
	unsigned int uConValue;

	pMASK_Reg = &(GPIO->rEINT15MASK);
	
	uConValue = *pMASK_Reg;
       uConValue = (uConValue & ~(0x1<<(uEINT_No)))|(1<<uEINT_No);
	*pMASK_Reg = uConValue;

}



//////////
// Function Name : GPIO_SetEint16
// Function Desctiption : This function setup Eint16[6:0]=> GPG2[6:0]
// Input : 			uEINT_No: EINT No.
//					uINTType: Select EINT Type. 
//								Low, High, Falling, Rising, Both
//					uFltType :  Select Filter Type
//								DisFLT(Disable Filter), DLYFLT(Delay Filter), DIGFLT(Digital Filter)
//					uFltWidth : Digital Filter Width ( 1~0x7F)
// Output : NONE
//
// Version : v0.0
void GPIO_SetEint16(unsigned int uEINT_No , unsigned int uIntType, FLT_eTYPE eFltType,  unsigned int uFltWidth)
{

	volatile unsigned int *pEINTx_Reg, *pFLTx_Reg;
	volatile unsigned int *pGPIO_EINT_Addr;
	unsigned int uGpioPort, uFunc, uType ;	
	unsigned int uConValue;

	GPIO_pBase = (void *)GPIO_BASE;

	pGPIO_EINT_Addr = &(GPIO->rEINT16CON);

	if (uEINT_No > 6)
	{
		printf("Error Eint No. \n");
		Assert(0);
	}
	
	// Check Filter Width 
	if(uFltWidth > 0x7f)
	{
		printf("Error Filter Width. \n");
		Assert(0);
	}

	//  Interrupt Type 
	if( uEINT_No <= 6)
	{
		uType =uEINT_No;
		pEINTx_Reg = pGPIO_EINT_Addr ; 	
		uConValue = *pEINTx_Reg;
		uConValue = (uConValue & ~(0xF<<(uType*4))) | (uIntType<<(uType*4));
		*pEINTx_Reg = uConValue;
	}


	// EINT Port
	if( uEINT_No <= 6)
	{
		uGpioPort = uEINT_No;
		uFunc = 0xf;  					// EINT Function 
		GPIO_SetFunctionEach(eGPIO_G2, (GPIO_eBitPos)uGpioPort, uFunc);		// ??
		// GPIO_SetPullUpDownEach(eGPIO_N,(GPIO_eBitPos) uGpioPort, 0x0);		// disable Pull-up/dn
	}
		

	// Fliter Type & Filter Width 
	if( uEINT_No <= 3)
	{
		uType =uEINT_No;
		pFLTx_Reg = &(GPIO->rEINT16FLTCON0) ; 	    // EINT3FLTCON0
		uConValue = *pFLTx_Reg;
		uConValue = (uConValue & ~(0xFF<<(uType*8))) |((uFltWidth<<(uType*8))|(eFltType<<(uType*8+7)));
		*pFLTx_Reg = uConValue;	
	}
	else if(uEINT_No >3 && uEINT_No <= 6)
	{
		uType =uEINT_No-4;
		pFLTx_Reg =&(GPIO->rEINT16FLTCON1); 	    // EINT3FLTCON1
		uConValue = *pFLTx_Reg;
		uConValue = (uConValue & ~(0xFF<<(uType*8))) |((uFltWidth<<(uType*8))|(eFltType<<(uType*8+7)));
		*pFLTx_Reg = uConValue;	
	}
	
	
}



//////////
// Function Name : GPIO_EINT16ClrPend
// Function Desctiption : Clear Eint pending bit of the Eint16[6:0]
// Input : 			uEINT_No: EINT No.
// Output : NONE
//
// Version : v0.0
void GPIO_EINT16ClrPend(unsigned int uEINT_No )
{

	volatile unsigned int *pEINT16PEND;
	unsigned int uConValue;

	pEINT16PEND = &(GPIO->rEINT16PEND);

	uConValue = (1<<uEINT_No);
	*pEINT16PEND = uConValue;

}

//////////
// Function Name : GPIO_EINT16UnMask
// Function Desctiption : UnMask the Eint16[6:0]
// Input : 			uEINT_No: EINT No.
// Output : NONE
//
// Version : v0.0
void GPIO_EINT16UnMask(unsigned int uEINT_No )
{

	volatile unsigned int *pMASK_Reg;
	unsigned int uConValue;

	pMASK_Reg = &(GPIO->rEINT16MASK);
	
	uConValue = *pMASK_Reg;
       uConValue = (uConValue & ~(0x1<<(uEINT_No)));
	*pMASK_Reg = uConValue;

}

//////////
// Function Name : GPIO_EINT16Mask
// Function Desctiption : Mask the Eint16[6:0]
// Input : 			uEINT_No: EINT No.
// Output : NONE
//
// Version : v0.0
void GPIO_EINT16Mask(unsigned int uEINT_No )
{

	volatile unsigned int *pMASK_Reg;
	unsigned int uConValue;

	pMASK_Reg = &(GPIO->rEINT16MASK);
	
	uConValue = *pMASK_Reg;
       uConValue = (uConValue & ~(0x1<<(uEINT_No)))|(1<<uEINT_No);
	*pMASK_Reg = uConValue;

}



//////////
// Function Name : GPIO_SetEint17
// Function Desctiption : This function setup Eint17[6:0]=> GPG3[6:0]
// Input : 			uEINT_No: EINT No.
//					uINTType: Select EINT Type. 
//								Low, High, Falling, Rising, Both
//					uFltType :  Select Filter Type
//								DisFLT(Disable Filter), DLYFLT(Delay Filter), DIGFLT(Digital Filter)
//					uFltWidth : Digital Filter Width ( 1~0x7F)
// Output : NONE
//
// Version : v0.0
void GPIO_SetEint17(unsigned int uEINT_No , unsigned int uIntType, FLT_eTYPE eFltType,  unsigned int uFltWidth)
{

	volatile unsigned int *pEINTx_Reg, *pFLTx_Reg;
	volatile unsigned int *pGPIO_EINT_Addr;
	unsigned int uGpioPort, uFunc, uType ;	
	unsigned int uConValue;

	GPIO_pBase = (void *)GPIO_BASE;

	pGPIO_EINT_Addr = &(GPIO->rEINT17CON);

	if (uEINT_No > 6)
	{
		printf("Error Eint No. \n");
		Assert(0);
	}
	
	// Check Filter Width 
	if(uFltWidth > 0x7f)
	{
		printf("Error Filter Width. \n");
		Assert(0);
	}

	//  Interrupt Type 
	if( uEINT_No <= 6)
	{
		uType =uEINT_No;
		pEINTx_Reg = pGPIO_EINT_Addr ; 	
		uConValue = *pEINTx_Reg;
		uConValue = (uConValue & ~(0xF<<(uType*4))) | (uIntType<<(uType*4));
		*pEINTx_Reg = uConValue;
	}


	// EINT Port
	if( uEINT_No <= 6)
	{
		uGpioPort = uEINT_No;
		uFunc = 0xf;  					// EINT Function 
		GPIO_SetFunctionEach(eGPIO_G3, (GPIO_eBitPos)uGpioPort, uFunc);		// ??
		// GPIO_SetPullUpDownEach(eGPIO_N,(GPIO_eBitPos) uGpioPort, 0x0);		// disable Pull-up/dn
	}
		

	// Fliter Type & Filter Width 
	if( uEINT_No <= 3)
	{
		uType =uEINT_No;
		pFLTx_Reg = &(GPIO->rEINT17FLTCON0) ; 	    // EINT3FLTCON0
		uConValue = *pFLTx_Reg;
		uConValue = (uConValue & ~(0xFF<<(uType*8))) |((uFltWidth<<(uType*8))|(eFltType<<(uType*8+7)));
		*pFLTx_Reg = uConValue;	
	}
	else if(uEINT_No > 3 && uEINT_No <= 6)
	{
		uType =uEINT_No-4;
		pFLTx_Reg =&(GPIO->rEINT17FLTCON1); 	    // EINT3FLTCON1
		uConValue = *pFLTx_Reg;
		uConValue = (uConValue & ~(0xFF<<(uType*8))) |((uFltWidth<<(uType*8))|(eFltType<<(uType*8+7)));
		*pFLTx_Reg = uConValue;	
	}
	
}



//////////
// Function Name : GPIO_EINT17ClrPend
// Function Desctiption : Clear Eint pending bit of the Eint17[6:0]
// Input : 			uEINT_No: EINT No.
// Output : NONE
//
// Version : v0.0
void GPIO_EINT17ClrPend(unsigned int uEINT_No )
{

	volatile unsigned int *pEINT17PEND;
	unsigned int uConValue;

	pEINT17PEND = &(GPIO->rEINT17PEND);

	uConValue = (1<<uEINT_No);
	*pEINT17PEND = uConValue;

}

//////////
// Function Name : GPIO_EINT17UnMask
// Function Desctiption : UnMask the Eint17[6:0]
// Input : 			uEINT_No: EINT No.
// Output : NONE
//
// Version : v0.0
void GPIO_EINT17UnMask(unsigned int uEINT_No )
{

	volatile unsigned int *pMASK_Reg;
	unsigned int uConValue;

	pMASK_Reg = &(GPIO->rEINT17MASK);
	
	uConValue = *pMASK_Reg;
       uConValue = (uConValue & ~(0x1<<(uEINT_No)));
	*pMASK_Reg = uConValue;

}

//////////
// Function Name : GPIO_EINT17Mask
// Function Desctiption : Mask the Eint17[6:0]
// Input : 			uEINT_No: EINT No.
// Output : NONE
//
// Version : v0.0
void GPIO_EINT17Mask(unsigned int uEINT_No )
{

	volatile unsigned int *pMASK_Reg;
	unsigned int uConValue;

	pMASK_Reg = &(GPIO->rEINT17MASK);
	
	uConValue = *pMASK_Reg;
       uConValue = (uConValue & ~(0x1<<(uEINT_No)))|(1<<uEINT_No);
	*pMASK_Reg = uConValue;

}




//////////
// Function Name : GPIO_SetEint18
// Function Desctiption : This function setup Eint18[7:0]=> GPJ0[7:0]
// Input : 			uEINT_No: EINT No.
//					uINTType: Select EINT Type. 
//								Low, High, Falling, Rising, Both
//					uFltType :  Select Filter Type
//								DisFLT(Disable Filter), DLYFLT(Delay Filter), DIGFLT(Digital Filter)
//					uFltWidth : Digital Filter Width ( 1~0x7F)
// Output : NONE
//
// Version : v0.0
void GPIO_SetEint18(unsigned int uEINT_No , unsigned int uIntType, FLT_eTYPE eFltType,  unsigned int uFltWidth)
{

	volatile unsigned int *pEINTx_Reg, *pFLTx_Reg;
	volatile unsigned int *pGPIO_EINT_Addr;
	unsigned int uGpioPort, uFunc, uType ;	
	unsigned int uConValue;

	GPIO_pBase = (void *)GPIO_BASE;

	pGPIO_EINT_Addr = &(GPIO->rEINT18CON);

	if (uEINT_No > 7)
	{
		printf("Error Eint No. \n");
		Assert(0);
	}
	
	// Check Filter Width 
	if(uFltWidth > 0x7f)
	{
		printf("Error Filter Width. \n");
		Assert(0);
	}

	//  Interrupt Type 
	if( uEINT_No <= 7)
	{
		uType =uEINT_No;
		pEINTx_Reg = pGPIO_EINT_Addr ; 	
		uConValue = *pEINTx_Reg;
		uConValue = (uConValue & ~(0xF<<(uType*4))) | (uIntType<<(uType*4));
		*pEINTx_Reg = uConValue;
	}


	// EINT Port
	if( uEINT_No <= 7)
	{
		uGpioPort = uEINT_No;
		uFunc = 0xf;  					// EINT Function 
		GPIO_SetFunctionEach(eGPIO_J0, (GPIO_eBitPos)uGpioPort, uFunc);		// ??
		// GPIO_SetPullUpDownEach(eGPIO_N,(GPIO_eBitPos) uGpioPort, 0x0);		// disable Pull-up/dn
	}
		

	// Fliter Type & Filter Width 
	if( uEINT_No <= 3)
	{
		uType =uEINT_No;
		pFLTx_Reg = &(GPIO->rEINT18FLTCON0) ; 	    // EINT18FLTCON0
		uConValue = *pFLTx_Reg;
		uConValue = (uConValue & ~(0xFF<<(uType*8))) |((uFltWidth<<(uType*8))|(eFltType<<(uType*8+7)));
		*pFLTx_Reg = uConValue;	
	}
	else if(uEINT_No > 3 && uEINT_No <= 7)
	{
		uType =uEINT_No-4;
		pFLTx_Reg =&(GPIO->rEINT18FLTCON1); 	    // EINT18FLTCON1
		uConValue = *pFLTx_Reg;
		uConValue = (uConValue & ~(0xFF<<(uType*8))) |((uFltWidth<<(uType*8))|(eFltType<<(uType*8+7)));
		*pFLTx_Reg = uConValue;	
	}
	
}



//////////
// Function Name : GPIO_EINT18ClrPend
// Function Desctiption : Clear Eint pending bit of the Eint18[7:0]
// Input : 			uEINT_No: EINT No.
// Output : NONE
//
// Version : v0.0
void GPIO_EINT18ClrPend(unsigned int uEINT_No )
{

	volatile unsigned int *pEINT18PEND;
	unsigned int uConValue;

	pEINT18PEND = &(GPIO->rEINT18PEND);

	uConValue = (1<<uEINT_No);
	*pEINT18PEND = uConValue;

}

//////////
// Function Name : GPIO_EINT18UnMask
// Function Desctiption : UnMask the Eint18[7:0]
// Input : 			uEINT_No: EINT No.
// Output : NONE
//
// Version : v0.0
void GPIO_EINT18UnMask(unsigned int uEINT_No )
{

	volatile unsigned int *pMASK_Reg;
	unsigned int uConValue;

	pMASK_Reg = &(GPIO->rEINT18MASK);
	
	uConValue = *pMASK_Reg;
       uConValue = (uConValue & ~(0x1<<(uEINT_No)));
	*pMASK_Reg = uConValue;

}

//////////
// Function Name : GPIO_EINT18Mask
// Function Desctiption : Mask the Eint18[7:0]
// Input : 			uEINT_No: EINT No.
// Output : NONE
//
// Version : v0.0
void GPIO_EINT18Mask(unsigned int uEINT_No )
{

	volatile unsigned int *pMASK_Reg;
	unsigned int uConValue;

	pMASK_Reg = &(GPIO->rEINT18MASK);
	
	uConValue = *pMASK_Reg;
       uConValue = (uConValue & ~(0x1<<(uEINT_No)))|(1<<uEINT_No);
	*pMASK_Reg = uConValue;

}


//////////
// Function Name : GPIO_SetEint19
// Function Desctiption : This function setup Eint19[5:0]=> GPJ1[5:0]
// Input : 			uEINT_No: EINT No.
//					uINTType: Select EINT Type. 
//								Low, High, Falling, Rising, Both
//					uFltType :  Select Filter Type
//								DisFLT(Disable Filter), DLYFLT(Delay Filter), DIGFLT(Digital Filter)
//					uFltWidth : Digital Filter Width ( 1~0x7F)
// Output : NONE
//
// Version : v0.0
void GPIO_SetEint19(unsigned int uEINT_No , unsigned int uIntType, FLT_eTYPE eFltType,  unsigned int uFltWidth)
{

	volatile unsigned int *pEINTx_Reg, *pFLTx_Reg;
	volatile unsigned int *pGPIO_EINT_Addr;
	unsigned int uGpioPort, uFunc, uType ;	
	unsigned int uConValue;

	GPIO_pBase = (void *)GPIO_BASE;

	pGPIO_EINT_Addr = &(GPIO->rEINT19CON);

	if (uEINT_No > 5)
	{
		printf("Error Eint No. \n");
		Assert(0);
	}
	
	// Check Filter Width 
	if(uFltWidth > 0x7f)
	{
		printf("Error Filter Width. \n");
		Assert(0);
	}

	//  Interrupt Type 
	if( uEINT_No <= 5)
	{
		uType =uEINT_No;
		pEINTx_Reg = pGPIO_EINT_Addr ; 	
		uConValue = *pEINTx_Reg;
		uConValue = (uConValue & ~(0xF<<(uType*4))) | (uIntType<<(uType*4));
		*pEINTx_Reg = uConValue;
	}


	// EINT Port
	if( uEINT_No <= 5)
	{
		uGpioPort = uEINT_No;
		uFunc = 0xf;  					// EINT Function 
		GPIO_SetFunctionEach(eGPIO_J1, (GPIO_eBitPos)uGpioPort, uFunc);		// ??
		// GPIO_SetPullUpDownEach(eGPIO_N,(GPIO_eBitPos) uGpioPort, 0x0);		// disable Pull-up/dn
	}
		

	// Fliter Type & Filter Width 
	if( uEINT_No <= 3)
	{
		uType =uEINT_No;
		pFLTx_Reg = &(GPIO->rEINT19FLTCON0) ; 	    // EINT3FLTCON0
		uConValue = *pFLTx_Reg;
		uConValue = (uConValue & ~(0xFF<<(uType*8))) |((uFltWidth<<(uType*8))|(eFltType<<(uType*8+7)));
		*pFLTx_Reg = uConValue;	
	}
	else if(uEINT_No > 3 && uEINT_No <= 5)
	{
		uType =uEINT_No-4;
		pFLTx_Reg =&(GPIO->rEINT19FLTCON1); 	    // EINT3FLTCON1
		uConValue = *pFLTx_Reg;
		uConValue = (uConValue & ~(0xFF<<(uType*8))) |((uFltWidth<<(uType*8))|(eFltType<<(uType*8+7)));
		*pFLTx_Reg = uConValue;	
	}
	
}



//////////
// Function Name : GPIO_EINT19ClrPend
// Function Desctiption : Clear Eint pending bit of the Eint19[5:0]
// Input : 			uEINT_No: EINT No.
// Output : NONE
//
// Version : v0.0
void GPIO_EINT19ClrPend(unsigned int uEINT_No )
{

	volatile unsigned int *pEINT19PEND;
	unsigned int uConValue;

	pEINT19PEND = &(GPIO->rEINT19PEND);

	uConValue = (1<<uEINT_No);
	*pEINT19PEND = uConValue;

}

//////////
// Function Name : GPIO_EINT19UnMask
// Function Desctiption : UnMask the Eint19[5:0]
// Input : 			uEINT_No: EINT No.
// Output : NONE
//
// Version : v0.0
void GPIO_EINT19UnMask(unsigned int uEINT_No )
{

	volatile unsigned int *pMASK_Reg;
	unsigned int uConValue;

	pMASK_Reg = &(GPIO->rEINT19MASK);
	
	uConValue = *pMASK_Reg;
       uConValue = (uConValue & ~(0x1<<(uEINT_No)));
	*pMASK_Reg = uConValue;

}

//////////
// Function Name : GPIO_EINT19Mask
// Function Desctiption : Mask the Eint19[5:0]
// Input : 			uEINT_No: EINT No.
// Output : NONE
//
// Version : v0.0
void GPIO_EINT19Mask(unsigned int uEINT_No )
{

	volatile unsigned int *pMASK_Reg;
	unsigned int uConValue;

	pMASK_Reg = &(GPIO->rEINT19MASK);
	
	uConValue = *pMASK_Reg;
       uConValue = (uConValue & ~(0x1<<(uEINT_No)))|(1<<uEINT_No);
	*pMASK_Reg = uConValue;

}



//////////
// Function Name : GPIO_SetEint20
// Function Desctiption : This function setup Eint20[7:0]=> GPJ2[7:0]
// Input : 			uEINT_No: EINT No.
//					uINTType: Select EINT Type. 
//								Low, High, Falling, Rising, Both
//					uFltType :  Select Filter Type
//								DisFLT(Disable Filter), DLYFLT(Delay Filter), DIGFLT(Digital Filter)
//					uFltWidth : Digital Filter Width ( 1~0x7F)
// Output : NONE
//
// Version : v0.0
void GPIO_SetEint20(unsigned int uEINT_No , unsigned int uIntType, FLT_eTYPE eFltType,  unsigned int uFltWidth)
{

	volatile unsigned int *pEINTx_Reg, *pFLTx_Reg;
	volatile unsigned int *pGPIO_EINT_Addr;
	unsigned int uGpioPort, uFunc, uType ;	
	unsigned int uConValue;

	GPIO_pBase = (void *)GPIO_BASE;

	pGPIO_EINT_Addr = &(GPIO->rEINT20CON);

	if (uEINT_No > 7)
	{
		printf("Error Eint No. \n");
		Assert(0);
	}
	
	// Check Filter Width 
	if(uFltWidth > 0x7f)
	{
		printf("Error Filter Width. \n");
		Assert(0);
	}

	//  Interrupt Type 
	if( uEINT_No <= 7)
	{
		uType =uEINT_No;
		pEINTx_Reg = pGPIO_EINT_Addr ; 	
		uConValue = *pEINTx_Reg;
		uConValue = (uConValue & ~(0xF<<(uType*4))) | (uIntType<<(uType*4));
		*pEINTx_Reg = uConValue;
	}


	// EINT Port
	if( uEINT_No <= 7)
	{
		uGpioPort = uEINT_No;
		uFunc = 0xf;  					// EINT Function 
		GPIO_SetFunctionEach(eGPIO_J2, (GPIO_eBitPos)uGpioPort, uFunc);		// ??
		// GPIO_SetPullUpDownEach(eGPIO_N,(GPIO_eBitPos) uGpioPort, 0x0);		// disable Pull-up/dn
	}
		

	// Fliter Type & Filter Width 
	if( uEINT_No <= 3)
	{
		uType =uEINT_No;
		pFLTx_Reg = &(GPIO->rEINT20FLTCON0) ; 	    // EINT20FLTCON0
		uConValue = *pFLTx_Reg;
		uConValue = (uConValue & ~(0xFF<<(uType*8))) |((uFltWidth<<(uType*8))|(eFltType<<(uType*8+7)));
		*pFLTx_Reg = uConValue;	
	}
	else if(uEINT_No >3 && uEINT_No <=7)
	{
		uType =uEINT_No-4;
		pFLTx_Reg =&(GPIO->rEINT20FLTCON1); 	    // EINT20FLTCON1
		uConValue = *pFLTx_Reg;
		uConValue = (uConValue & ~(0xFF<<(uType*8))) |((uFltWidth<<(uType*8))|(eFltType<<(uType*8+7)));
		*pFLTx_Reg = uConValue;	
	}
	
}



//////////
// Function Name : GPIO_EINT20ClrPend
// Function Desctiption : Clear Eint pending bit of the Eint20[7:0]
// Input : 			uEINT_No: EINT No.
// Output : NONE
//
// Version : v0.0
void GPIO_EINT20ClrPend(unsigned int uEINT_No )
{

	volatile unsigned int *pEINT20PEND;
	unsigned int uConValue;

	pEINT20PEND = &(GPIO->rEINT20PEND);

	uConValue = (1<<uEINT_No);
	*pEINT20PEND = uConValue;

}

//////////
// Function Name : GPIO_EINT20UnMask
// Function Desctiption : UnMask the Eint20[7:0]
// Input : 			uEINT_No: EINT No.
// Output : NONE
//
// Version : v0.0
void GPIO_EINT20UnMask(unsigned int uEINT_No )
{

	volatile unsigned int *pMASK_Reg;
	unsigned int uConValue;

	pMASK_Reg = &(GPIO->rEINT20MASK);
	
	uConValue = *pMASK_Reg;
       uConValue = (uConValue & ~(0x1<<(uEINT_No)));
	*pMASK_Reg = uConValue;

}

//////////
// Function Name : GPIO_EINT20Mask
// Function Desctiption : Mask the Eint20[7:0]
// Input : 			uEINT_No: EINT No.
// Output : NONE
//
// Version : v0.0
void GPIO_EINT20Mask(unsigned int uEINT_No )
{

	volatile unsigned int *pMASK_Reg;
	unsigned int uConValue;

	pMASK_Reg = &(GPIO->rEINT20MASK);
	
	uConValue = *pMASK_Reg;
       uConValue = (uConValue & ~(0x1<<(uEINT_No)))|(1<<uEINT_No);
	*pMASK_Reg = uConValue;

}



//////////
// Function Name : GPIO_SetEint21
// Function Desctiption : This function setup Eint21[7:0]=> GPJ3[7:0]
// Input : 			uEINT_No: EINT No.
//					uINTType: Select EINT Type. 
//								Low, High, Falling, Rising, Both
//					uFltType :  Select Filter Type
//								DisFLT(Disable Filter), DLYFLT(Delay Filter), DIGFLT(Digital Filter)
//					uFltWidth : Digital Filter Width ( 1~0x7F)
// Output : NONE
//
// Version : v0.0
void GPIO_SetEint21(unsigned int uEINT_No , unsigned int uIntType, FLT_eTYPE eFltType,  unsigned int uFltWidth)
{

	volatile unsigned int *pEINTx_Reg, *pFLTx_Reg;
	volatile unsigned int *pGPIO_EINT_Addr;
	unsigned int uGpioPort, uFunc, uType ;	
	unsigned int uConValue;

	GPIO_pBase = (void *)GPIO_BASE;

	pGPIO_EINT_Addr = &(GPIO->rEINT21CON);

	if (uEINT_No > 7)
	{
		printf("Error Eint No. \n");
		Assert(0);
	}
	
	// Check Filter Width 
	if(uFltWidth >0x7f)
	{
		printf("Error Filter Width. \n");
		Assert(0);
	}

	//  Interrupt Type 
	if( uEINT_No <= 7)
	{
		uType =uEINT_No;
		pEINTx_Reg = pGPIO_EINT_Addr ; 	
		uConValue = *pEINTx_Reg;
		uConValue = (uConValue & ~(0xF<<(uType*4))) | (uIntType<<(uType*4));
		*pEINTx_Reg = uConValue;
	}


	// EINT Port
	if( uEINT_No <= 7)
	{
		uGpioPort = uEINT_No;
		uFunc = 0xf;  					// EINT Function 
		GPIO_SetFunctionEach(eGPIO_J3, (GPIO_eBitPos)uGpioPort, uFunc);		// ??
		// GPIO_SetPullUpDownEach(eGPIO_N,(GPIO_eBitPos) uGpioPort, 0x0);		// disable Pull-up/dn
	}
		

	// Fliter Type & Filter Width 
	if( uEINT_No <= 3)
	{
		uType =uEINT_No;
		pFLTx_Reg = &(GPIO->rEINT21FLTCON0) ; 	    // EINT21FLTCON0
		uConValue = *pFLTx_Reg;
		uConValue = (uConValue & ~(0xFF<<(uType*8))) |((uFltWidth<<(uType*8))|(eFltType<<(uType*8+7)));
		*pFLTx_Reg = uConValue;	
	}
	else if(uEINT_No > 3 && uEINT_No <= 7)
	{
		uType =uEINT_No-4;
		pFLTx_Reg =&(GPIO->rEINT21FLTCON1); 	    // EINT21FLTCON1
		uConValue = *pFLTx_Reg;
		uConValue = (uConValue & ~(0xFF<<(uType*8))) |((uFltWidth<<(uType*8))|(eFltType<<(uType*8+7)));
		*pFLTx_Reg = uConValue;	
	}
	
}



//////////
// Function Name : GPIO_EINT21ClrPend
// Function Desctiption : Clear Eint pending bit of the Eint21[7:0]
// Input : 			uEINT_No: EINT No.
// Output : NONE
//
// Version : v0.0
void GPIO_EINT21ClrPend(unsigned int uEINT_No )
{

	volatile unsigned int *pEINT21PEND;
	unsigned int uConValue;

	pEINT21PEND = &(GPIO->rEINT21PEND);

	uConValue = (1<<uEINT_No);
	*pEINT21PEND = uConValue;

}

//////////
// Function Name : GPIO_EINT21UnMask
// Function Desctiption : UnMask the Eint21[7:0]
// Input : 			uEINT_No: EINT No.
// Output : NONE
//
// Version : v0.0
void GPIO_EINT21UnMask(unsigned int uEINT_No )
{

	volatile unsigned int *pMASK_Reg;
	unsigned int uConValue;

	pMASK_Reg = &(GPIO->rEINT21MASK);
	
	uConValue = *pMASK_Reg;
       uConValue = (uConValue & ~(0x1<<(uEINT_No)));
	*pMASK_Reg = uConValue;

}

//////////
// Function Name : GPIO_EINT21Mask
// Function Desctiption : Mask the Eint21[7:0]
// Input : 			uEINT_No: EINT No.
// Output : NONE
//
// Version : v0.0
void GPIO_EINT21Mask(unsigned int uEINT_No )
{

	volatile unsigned int *pMASK_Reg;
	unsigned int uConValue;

	pMASK_Reg = &(GPIO->rEINT21MASK);
	
	uConValue = *pMASK_Reg;
       uConValue = (uConValue & ~(0x1<<(uEINT_No)))|(1<<uEINT_No);
	*pMASK_Reg = uConValue;

}




//////////
// Function Name : GPIO_SetEint22
// Function Desctiption : This function setup Eint22[4:0]=> GPJ4[4:0]
// Input : 			uEINT_No: EINT No.
//					uINTType: Select EINT Type. 
//								Low, High, Falling, Rising, Both
//					uFltType :  Select Filter Type
//								DisFLT(Disable Filter), DLYFLT(Delay Filter), DIGFLT(Digital Filter)
//					uFltWidth : Digital Filter Width ( 1~0x7F)
// Output : NONE
//
// Version : v0.0
void GPIO_SetEint22(unsigned int uEINT_No , unsigned int uIntType, FLT_eTYPE eFltType,  unsigned int uFltWidth)
{

	volatile unsigned int *pEINTx_Reg, *pFLTx_Reg;
	volatile unsigned int *pGPIO_EINT_Addr;
	unsigned int uGpioPort, uFunc, uType ;	
	unsigned int uConValue;

	GPIO_pBase = (void *)GPIO_BASE;

	pGPIO_EINT_Addr = &(GPIO->rEINT22CON);

	
	if (uEINT_No >4)
	{
		printf("Error Eint No. \n");
		Assert(0);
	}
	

	// Check Filter Width 
	if(uFltWidth >0x7f)
	{
		printf("Error Filter Width. \n");
		Assert(0);
	}
	

	//  Interrupt Type 
	if( uEINT_No <= 4)
	{
		uType =uEINT_No;
		pEINTx_Reg = pGPIO_EINT_Addr ; 	
		uConValue = *pEINTx_Reg;
		uConValue = (uConValue & ~(0xF<<(uType*4))) | (uIntType<<(uType*4));
		*pEINTx_Reg = uConValue;
	}


	// EINT Port
	if( uEINT_No <= 4)
	{
		uGpioPort = uEINT_No;
		uFunc = 0xf;  					// EINT Function 
		GPIO_SetFunctionEach(eGPIO_J4, (GPIO_eBitPos)uGpioPort, uFunc);		// ??
		// GPIO_SetPullUpDownEach(eGPIO_N,(GPIO_eBitPos) uGpioPort, 0x0);		// disable Pull-up/dn
	}

	// Fliter Type & Filter Width 
	if( uEINT_No <= 3)
	{
		uType =uEINT_No;
		pFLTx_Reg = &(GPIO->rEINT22FLTCON0) ; 	    // EINT12FLTCON0
		uConValue = *pFLTx_Reg;
		uConValue = (uConValue & ~(0xFF<<(uType*8))) |((uFltWidth<<(uType*8))|(eFltType<<(uType*8+7)));
		*pFLTx_Reg = uConValue;	
	}
	else if(uEINT_No = 4)
	{
		uType =uEINT_No-4;
		pFLTx_Reg =&(GPIO->rEINT22FLTCON1); 	    // EINT12FLTCON1
		uConValue = *pFLTx_Reg;
		uConValue = (uConValue & ~(0xFF<<(uType*8))) |((uFltWidth<<(uType*8))|(eFltType<<(uType*8+7)));
		*pFLTx_Reg = uConValue;	
	}
	
	
}



//////////
// Function Name : GPIO_EINT22ClrPend
// Function Desctiption : Clear Eint pending bit of the Eint22[4:0]
// Input : 			uEINT_No: EINT No.
// Output : NONE
//
// Version : v0.0
void GPIO_EINT22ClrPend(unsigned int uEINT_No )
{

	volatile unsigned int *pEINT22PEND;
	unsigned int uConValue;

	pEINT22PEND = &(GPIO->rEINT22PEND);

	uConValue = (1<<uEINT_No);
	*pEINT22PEND = uConValue;

}

//////////
// Function Name : GPIO_EINT22UnMask
// Function Desctiption : UnMask the Eint22[4:0]
// Input : 			uEINT_No: EINT No.
// Output : NONE
//
// Version : v0.0
void GPIO_EINT22UnMask(unsigned int uEINT_No )
{

	volatile unsigned int *pMASK_Reg;
	unsigned int uConValue;

	pMASK_Reg = &(GPIO->rEINT22MASK);
	
	uConValue = *pMASK_Reg;
       uConValue = (uConValue & ~(0x1<<(uEINT_No)));
	*pMASK_Reg = uConValue;

}

//////////
// Function Name : GPIO_EINT22Mask
// Function Desctiption : Mask the Eint22[4:0]
// Input : 			uEINT_No: EINT No.
// Output : NONE
//
// Version : v0.0
void GPIO_EINT22Mask(unsigned int uEINT_No )
{

	volatile unsigned int *pMASK_Reg;
	unsigned int uConValue;

	pMASK_Reg = &(GPIO->rEINT22MASK);
	
	uConValue = *pMASK_Reg;
       uConValue = (uConValue & ~(0x1<<(uEINT_No)))|(1<<uEINT_No);
	*pMASK_Reg = uConValue;

}



//////////
// Function Name : GPIO_SetEint23
// Function Desctiption : This function setup Eint23[6:0]=> GPI[6:0]
// Input : 			uEINT_No: EINT No.
//					uINTType: Select EINT Type. 
//								Low, High, Falling, Rising, Both
//					uFltType :  Select Filter Type
//								DisFLT(Disable Filter), DLYFLT(Delay Filter), DIGFLT(Digital Filter)
//					uFltWidth : Digital Filter Width ( 1~0x7F)
// Output : NONE
//
// Version : v0.0
void GPIO_SetEint23(unsigned int uEINT_No , unsigned int uIntType, FLT_eTYPE eFltType,  unsigned int uFltWidth)
{

	volatile unsigned int *pEINTx_Reg, *pFLTx_Reg;
	volatile unsigned int *pGPIO_EINT_Addr;
	unsigned int uGpioPort, uFunc, uType ;	
	unsigned int uConValue;

	GPIO_pBase = (void *)GPIO_BASE;

	pGPIO_EINT_Addr = &(GPIO->rEINT23CON);

	if (uEINT_No > 6)
	{
		printf("Error Eint No. \n");
		Assert(0);
	}
	
	// Check Filter Width 
	if(uFltWidth >0x7f)
	{
		printf("Error Filter Width. \n");
		Assert(0);
	}

	//  Interrupt Type 
	if( uEINT_No <= 6)
	{
		uType =uEINT_No;
		pEINTx_Reg = pGPIO_EINT_Addr ; 	
		uConValue = *pEINTx_Reg;
		uConValue = (uConValue & ~(0xF<<(uType*4))) | (uIntType<<(uType*4));
		*pEINTx_Reg = uConValue;
	}


	// EINT Port
	if( uEINT_No <= 6)
	{
		uGpioPort = uEINT_No;
		uFunc = 0xf;  					// EINT Function 
		GPIO_SetFunctionEach(eGPIO_I, (GPIO_eBitPos)uGpioPort, uFunc);		// ??
		// GPIO_SetPullUpDownEach(eGPIO_N,(GPIO_eBitPos) uGpioPort, 0x0);		// disable Pull-up/dn
	}
		

	// Fliter Type & Filter Width 
	if( uEINT_No <= 3)
	{
		uType =uEINT_No;
		pFLTx_Reg = &(GPIO->rEINT23FLTCON0) ; 	    // EINT13FLTCON0
		uConValue = *pFLTx_Reg;
		uConValue = (uConValue & ~(0xFF<<(uType*8))) |((uFltWidth<<(uType*8))|(eFltType<<(uType*8+7)));
		*pFLTx_Reg = uConValue;	
	}
	else if(uEINT_No >3 && uEINT_No <= 6)
	{
		uType =uEINT_No-4;
		pFLTx_Reg =&(GPIO->rEINT23FLTCON1); 	    // EINT13FLTCON1
		uConValue = *pFLTx_Reg;
		uConValue = (uConValue & ~(0xFF<<(uType*8))) |((uFltWidth<<(uType*8))|(eFltType<<(uType*8+7)));
		*pFLTx_Reg = uConValue;	
	}
	
}



//////////
// Function Name : GPIO_EINT23ClrPend
// Function Desctiption : Clear Eint pending bit of the Eint23[6:0]
// Input : 			uEINT_No: EINT No.
// Output : NONE
//
// Version : v0.0
void GPIO_EINT23ClrPend(unsigned int uEINT_No )
{

	volatile unsigned int *pEINT23PEND;
	unsigned int uConValue;

	pEINT23PEND = &(GPIO->rEINT23PEND);

	uConValue = (1<<uEINT_No);
	*pEINT23PEND = uConValue;

}

//////////
// Function Name : GPIO_EINT23UnMask
// Function Desctiption : UnMask the Eint23[6:0]
// Input : 			uEINT_No: EINT No.
// Output : NONE
//
// Version : v0.0
void GPIO_EINT23UnMask(unsigned int uEINT_No )
{

	volatile unsigned int *pMASK_Reg;
	unsigned int uConValue;

	pMASK_Reg = &(GPIO->rEINT23MASK);
	
	uConValue = *pMASK_Reg;
       uConValue = (uConValue & ~(0x1<<(uEINT_No)));
	*pMASK_Reg = uConValue;

}

//////////
// Function Name : GPIO_EINT23Mask
// Function Desctiption : Mask the Eint23[6:0]
// Input : 			uEINT_No: EINT No.
// Output : NONE
//
// Version : v0.0
void GPIO_EINT23Mask(unsigned int uEINT_No )
{

	volatile unsigned int *pMASK_Reg;
	unsigned int uConValue;

	pMASK_Reg = &(GPIO->rEINT23MASK);
	
	uConValue = *pMASK_Reg;
       uConValue = (uConValue & ~(0x1<<(uEINT_No)))|(1<<uEINT_No);
	*pMASK_Reg = uConValue;

}






//////////
// Function Name : GPIO_SetEint30
// Function Desctiption : This function setup Eint30[7:0]=> GPH0[7:0]
// Input : 			uEINT_No: EINT No.
//					uINTType: Select EINT Type. 
//								Low, High, Falling, Rising, Both
//					uFltType :  Select Filter Type
//								DisFLT(Disable Filter), DLYFLT(Delay Filter), DIGFLT(Digital Filter)
//					uFltWidth : Digital Filter Width ( 1~0x7F)
// Output : NONE
//
// Version : v0.0
void GPIO_SetEint30(unsigned int uEINT_No , unsigned int uIntType, FLT_eTYPE eFltType,  unsigned int uFltWidth)
{

	volatile unsigned int *pEINTx_Reg, *pFLTx_Reg;
	volatile unsigned int *pGPIO_EINT_Addr;
	unsigned int uGpioPort, uFunc, uType ;	
	unsigned int uConValue;

	GPIO_pBase = (void *)GPIO_BASE;

	pGPIO_EINT_Addr = &(GPIO->rEINT30CON);

	if (uEINT_No > 7)
	{
		printf("Error Eint No. \n");
//jhlim		Assert(0);
	}
	
	// Check Filter Width 
	if(uFltWidth > 0x3f)
	{
		printf("Error Filter Width. \n");
//jhlim		Assert(0);
	}

	//  Interrupt Type 
	if( uEINT_No <= 7)
	{
		uType =uEINT_No;
		pEINTx_Reg = pGPIO_EINT_Addr ; 	
		uConValue = *pEINTx_Reg;
		uConValue = (uConValue & ~(0xF<<(uType*4))) | (uIntType<<(uType*4));
		*pEINTx_Reg = uConValue;
	}


	// EINT Port
	if( uEINT_No <= 7)
	{
		uGpioPort = uEINT_No;
		uFunc = 0xf;  					// EINT Function 
		GPIO_SetFunctionEach(eGPIO_H0, (GPIO_eBitPos)uGpioPort, uFunc);		// ??
		// GPIO_SetPullUpDownEach(eGPIO_N,(GPIO_eBitPos) uGpioPort, 0x0);		// disable Pull-up/dn
	}
		

	// Fliter Type & Filter Width 
	if( uEINT_No <= 3)
	{
		uType =uEINT_No;
		pFLTx_Reg = &(GPIO->rEINT30FLTCON0) ; 	    // EINT20FLTCON0
		uConValue = *pFLTx_Reg;
		uConValue = (uConValue & ~(0xFF<<(uType*8))) |((uFltWidth<<(uType*8))|(eFltType<<(uType*8+6)));
		*pFLTx_Reg = uConValue;	
	}
	else if(uEINT_No >3 && uEINT_No <=7)
	{
		uType =uEINT_No-4;
		pFLTx_Reg =&(GPIO->rEINT30FLTCON1); 	    // EINT20FLTCON1
		uConValue = *pFLTx_Reg;
		uConValue = (uConValue & ~(0xFF<<(uType*8))) |((uFltWidth<<(uType*8))|(eFltType<<(uType*8+6)));
		*pFLTx_Reg = uConValue;	
	}
	
}



//////////
// Function Name : GPIO_EINT30ClrPend
// Function Desctiption : Clear Eint pending bit of the Eint30[7:0]
// Input : 			uEINT_No: EINT No.
// Output : NONE
//
// Version : v0.0
void GPIO_EINT30ClrPend(unsigned int uEINT_No )
{

	volatile unsigned int *pEINT30PEND;
	unsigned int uConValue;

	pEINT30PEND = &(GPIO->rEINT30PEND);

	uConValue = (1<<uEINT_No);
	*pEINT30PEND = uConValue;

}

//////////
// Function Name : GPIO_EINT30UnMask
// Function Desctiption : UnMask the Eint30[7:0]
// Input : 			uEINT_No: EINT No.
// Output : NONE
//
// Version : v0.0
void GPIO_EINT30UnMask(unsigned int uEINT_No )
{

	volatile unsigned int *pMASK_Reg;
	unsigned int uConValue;

	pMASK_Reg = &(GPIO->rEINT30MASK);
	
	uConValue = *pMASK_Reg;
       uConValue = (uConValue & ~(0x1<<(uEINT_No)));
	*pMASK_Reg = uConValue;

}

//////////
// Function Name : GPIO_EINT30Mask
// Function Desctiption : Mask the Eint30[7:0]
// Input : 			uEINT_No: EINT No.
// Output : NONE
//
// Version : v0.0
void GPIO_EINT30Mask(unsigned int uEINT_No )
{

	volatile unsigned int *pMASK_Reg;
	unsigned int uConValue;

	pMASK_Reg = &(GPIO->rEINT30MASK);
	
	uConValue = *pMASK_Reg;
       uConValue = (uConValue & ~(0x1<<(uEINT_No)))|(1<<uEINT_No);
	*pMASK_Reg = uConValue;

}



//////////
// Function Name : GPIO_SetEint31
// Function Desctiption : This function setup Eint31[7:0]=> GPH1[7:0]
// Input : 			uEINT_No: EINT No.
//					uINTType: Select EINT Type. 
//								Low, High, Falling, Rising, Both
//					uFltType :  Select Filter Type
//								DisFLT(Disable Filter), DLYFLT(Delay Filter), DIGFLT(Digital Filter)
//					uFltWidth : Digital Filter Width ( 1~0x7F)
// Output : NONE
//
// Version : v0.0
void GPIO_SetEint31(unsigned int uEINT_No , unsigned int uIntType, FLT_eTYPE eFltType,  unsigned int uFltWidth)
{

	volatile unsigned int *pEINTx_Reg, *pFLTx_Reg;
	volatile unsigned int *pGPIO_EINT_Addr;
	unsigned int uGpioPort, uFunc, uType ;	
	unsigned int uConValue;

	GPIO_pBase = (void *)GPIO_BASE;

	pGPIO_EINT_Addr = &(GPIO->rEINT31CON);

	if (uEINT_No > 7)
	{
		printf("Error Eint No. \n");
//jhlim		Assert(0);
	}
	
	// Check Filter Width 
	if(uFltWidth >0x3f)
	{
		printf("Error Filter Width. \n");
//jhlim		Assert(0);
	}

	//  Interrupt Type 
	if( uEINT_No <= 7)
	{
		uType =uEINT_No;
		pEINTx_Reg = pGPIO_EINT_Addr ; 	
		uConValue = *pEINTx_Reg;
		uConValue = (uConValue & ~(0xF<<(uType*4))) | (uIntType<<(uType*4));
		*pEINTx_Reg = uConValue;
	}


	// EINT Port
	if( uEINT_No <= 7)
	{
		uGpioPort = uEINT_No;
		uFunc = 0xf;  					// EINT Function 
		GPIO_SetFunctionEach(eGPIO_H1, (GPIO_eBitPos)uGpioPort, uFunc);		// ??
		// GPIO_SetPullUpDownEach(eGPIO_N,(GPIO_eBitPos) uGpioPort, 0x0);		// disable Pull-up/dn
	}
		

	// Fliter Type & Filter Width 
	if( uEINT_No <= 3)
	{
		uType =uEINT_No;
		pFLTx_Reg = &(GPIO->rEINT31FLTCON0) ; 	    // EINT31FLTCON0
		uConValue = *pFLTx_Reg;
		uConValue = (uConValue & ~(0xFF<<(uType*8))) |((uFltWidth<<(uType*8))|(eFltType<<(uType*8+6)));
		*pFLTx_Reg = uConValue;	
	}
	else if(uEINT_No > 3 && uEINT_No <= 7)
	{
		uType =uEINT_No-4;
		pFLTx_Reg =&(GPIO->rEINT31FLTCON1); 	    // EINT31FLTCON1
		uConValue = *pFLTx_Reg;
		uConValue = (uConValue & ~(0xFF<<(uType*8))) |((uFltWidth<<(uType*8))|(eFltType<<(uType*8+6)));
		*pFLTx_Reg = uConValue;	
	}
	
}



//////////
// Function Name : GPIO_EINT31ClrPend
// Function Desctiption : Clear Eint pending bit of the Eint31[7:0]
// Input : 			uEINT_No: EINT No.
// Output : NONE
//
// Version : v0.0
void GPIO_EINT31ClrPend(unsigned int uEINT_No )
{

	volatile unsigned int *pEINT31PEND;
	unsigned int uConValue;

	pEINT31PEND = &(GPIO->rEINT31PEND);

	uConValue = (1<<uEINT_No);
	*pEINT31PEND = uConValue;

}

//////////
// Function Name : GPIO_EINT31UnMask
// Function Desctiption : UnMask the Eint31[7:0]
// Input : 			uEINT_No: EINT No.
// Output : NONE
//
// Version : v0.0
void GPIO_EINT31UnMask(unsigned int uEINT_No )
{

	volatile unsigned int *pMASK_Reg;
	unsigned int uConValue;

	pMASK_Reg = &(GPIO->rEINT31MASK);
	
	uConValue = *pMASK_Reg;
       uConValue = (uConValue & ~(0x1<<(uEINT_No)));
	*pMASK_Reg = uConValue;

}

//////////
// Function Name : GPIO_EINT31Mask
// Function Desctiption : Mask the Eint31[7:0]
// Input : 			uEINT_No: EINT No.
// Output : NONE
//
// Version : v0.0
void GPIO_EINT31Mask(unsigned int uEINT_No )
{

	volatile unsigned int *pMASK_Reg;
	unsigned int uConValue;

	pMASK_Reg = &(GPIO->rEINT31MASK);
	
	uConValue = *pMASK_Reg;
       uConValue = (uConValue & ~(0x1<<(uEINT_No)))|(1<<uEINT_No);
	*pMASK_Reg = uConValue;

}




//////////
// Function Name : GPIO_SetEint32
// Function Desctiption : This function setup Eint32[7:0]=> GPH2[7:0]
// Input : 			uEINT_No: EINT No.
//					uINTType: Select EINT Type. 
//								Low, High, Falling, Rising, Both
//					uFltType :  Select Filter Type
//								DisFLT(Disable Filter), DLYFLT(Delay Filter), DIGFLT(Digital Filter)
//					uFltWidth : Digital Filter Width ( 1~0x3F)
// Output : NONE
//
// Version : v0.0
void GPIO_SetEint32(unsigned int uEINT_No , unsigned int uIntType, FLT_eTYPE eFltType,  unsigned int uFltWidth)
{

	volatile unsigned int *pEINTx_Reg, *pFLTx_Reg;
	volatile unsigned int *pGPIO_EINT_Addr;
	unsigned int uGpioPort, uFunc, uType ;	
	unsigned int uConValue;

	GPIO_pBase = (void *)GPIO_BASE;

	pGPIO_EINT_Addr = &(GPIO->rEINT32CON);

	
	if (uEINT_No >7)
	{
		printf("Error Eint No. \n");
//jhlim		Assert(0);
	}
	

	// Check Filter Width 
	if(uFltWidth >0x3f)
	{
		printf("Error Filter Width. \n");
//jhlim		Assert(0);
	}
	

	//  Interrupt Type 
	if( uEINT_No <= 7)
	{
		uType =uEINT_No;
		pEINTx_Reg = pGPIO_EINT_Addr ; 	
		uConValue = *pEINTx_Reg;
		uConValue = (uConValue & ~(0xF<<(uType*4))) | (uIntType<<(uType*4));
		*pEINTx_Reg = uConValue;
	}


	// EINT Port
	if( uEINT_No <= 7)
	{
		uGpioPort = uEINT_No;
		uFunc = 0xf;  					// EINT Function 
		GPIO_SetFunctionEach(eGPIO_H2, (GPIO_eBitPos)uGpioPort, uFunc);		// ??
		// GPIO_SetPullUpDownEach(eGPIO_N,(GPIO_eBitPos) uGpioPort, 0x0);		// disable Pull-up/dn
	}

	// Fliter Type & Filter Width 
	if( uEINT_No <= 3)
	{
		uType =uEINT_No;
		pFLTx_Reg = &(GPIO->rEINT32FLTCON0) ; 	    // EINT12FLTCON0
		uConValue = *pFLTx_Reg;
		uConValue = (uConValue & ~(0xFF<<(uType*8))) |((uFltWidth<<(uType*8))|(eFltType<<(uType*8+6)));
		*pFLTx_Reg = uConValue;	
	}
	else if(uEINT_No > 3 && uEINT_No <= 7)
	{
		uType =uEINT_No-4;
		pFLTx_Reg =&(GPIO->rEINT32FLTCON1); 	    // EINT12FLTCON1
		uConValue = *pFLTx_Reg;
		uConValue = (uConValue & ~(0xFF<<(uType*8))) |((uFltWidth<<(uType*8))|(eFltType<<(uType*8+6)));
		*pFLTx_Reg = uConValue;	
	}
	
	
}



//////////
// Function Name : GPIO_EINT32ClrPend
// Function Desctiption : Clear Eint pending bit of the Eint32[7:0]
// Input : 			uEINT_No: EINT No.
// Output : NONE
//
// Version : v0.0
void GPIO_EINT32ClrPend(unsigned int uEINT_No )
{

	volatile unsigned int *pEINT32PEND;
	unsigned int uConValue;

	pEINT32PEND = &(GPIO->rEINT32PEND);

	uConValue = (1<<uEINT_No);
	*pEINT32PEND = uConValue;

}

//////////
// Function Name : GPIO_EINT32UnMask
// Function Desctiption : UnMask the Eint32[7:0]
// Input : 			uEINT_No: EINT No.
// Output : NONE
//
// Version : v0.0
void GPIO_EINT32UnMask(unsigned int uEINT_No )
{

	volatile unsigned int *pMASK_Reg;
	unsigned int uConValue;

	pMASK_Reg = &(GPIO->rEINT32MASK);
	
	uConValue = *pMASK_Reg;
       uConValue = (uConValue & ~(0x1<<(uEINT_No)));
	*pMASK_Reg = uConValue;

}

//////////
// Function Name : GPIO_EINT32Mask
// Function Desctiption : Mask the Eint32[7:0]
// Input : 			uEINT_No: EINT No.
// Output : NONE
//
// Version : v0.0
void GPIO_EINT32Mask(unsigned int uEINT_No )
{

	volatile unsigned int *pMASK_Reg;
	unsigned int uConValue;

	pMASK_Reg = &(GPIO->rEINT32MASK);
	
	uConValue = *pMASK_Reg;
       uConValue = (uConValue & ~(0x1<<(uEINT_No)))|(1<<uEINT_No);
	*pMASK_Reg = uConValue;

}



//////////
// Function Name : GPIO_SetEint33
// Function Desctiption : This function setup Eint33[7:0]=> GPH3[7:0]
// Input : 			uEINT_No: EINT No.
//					uINTType: Select EINT Type. 
//								Low, High, Falling, Rising, Both
//					uFltType :  Select Filter Type
//								DisFLT(Disable Filter), DLYFLT(Delay Filter), DIGFLT(Digital Filter)
//					uFltWidth : Digital Filter Width ( 1~0x7F)
// Output : NONE
//
// Version : v0.0
void GPIO_SetEint33(unsigned int uEINT_No , unsigned int uIntType, FLT_eTYPE eFltType,  unsigned int uFltWidth)
{

	volatile unsigned int *pEINTx_Reg, *pFLTx_Reg;
	volatile unsigned int *pGPIO_EINT_Addr;
	unsigned int uGpioPort, uFunc, uType ;	
	unsigned int uConValue;

	GPIO_pBase = (void *)GPIO_BASE;

	pGPIO_EINT_Addr = &(GPIO->rEINT33CON);

	if (uEINT_No > 7)
	{
		printf("Error Eint No. \n");
// jhlim		Assert(0);
	}
	
	// Check Filter Width 
	if(uFltWidth >0x3f)
	{
		printf("Error Filter Width. \n");
//jhlim		Assert(0);
	}

	//  Interrupt Type 
	if( uEINT_No <= 7)
	{
		uType =uEINT_No;
		pEINTx_Reg = pGPIO_EINT_Addr ; 	
		uConValue = *pEINTx_Reg;
		uConValue = (uConValue & ~(0xF<<(uType*4))) | (uIntType<<(uType*4));
		*pEINTx_Reg = uConValue;
	}


	// EINT Port
	if( uEINT_No <= 7)
	{
		uGpioPort = uEINT_No;
		uFunc = 0xf;  					// EINT Function 
		GPIO_SetFunctionEach(eGPIO_H3, (GPIO_eBitPos)uGpioPort, uFunc);		// ??
		// GPIO_SetPullUpDownEach(eGPIO_N,(GPIO_eBitPos) uGpioPort, 0x0);		// disable Pull-up/dn
	}
		

	// Fliter Type & Filter Width 
	if( uEINT_No <= 3)
	{
		uType =uEINT_No;
		pFLTx_Reg = &(GPIO->rEINT33FLTCON0) ; 	    // EINT13FLTCON0
		uConValue = *pFLTx_Reg;
		uConValue = (uConValue & ~(0xFF<<(uType*8))) |((uFltWidth<<(uType*8))|(eFltType<<(uType*8+6)));
		*pFLTx_Reg = uConValue;	
	}
	else if(uEINT_No >3 && uEINT_No <= 7)
	{
		uType =uEINT_No-4;
		pFLTx_Reg =&(GPIO->rEINT33FLTCON1); 	    // EINT13FLTCON1
		uConValue = *pFLTx_Reg;
		uConValue = (uConValue & ~(0xFF<<(uType*8))) |((uFltWidth<<(uType*8))|(eFltType<<(uType*8+6)));
		*pFLTx_Reg = uConValue;	
	}
	
}



//////////
// Function Name : GPIO_EINT33ClrPend
// Function Desctiption : Clear Eint pending bit of the Eint33[7:0]
// Input : 			uEINT_No: EINT No.
// Output : NONE
//
// Version : v0.0
void GPIO_EINT33ClrPend(unsigned int uEINT_No )
{

	volatile unsigned int *pEINT33PEND;
	unsigned int uConValue;

	pEINT33PEND = &(GPIO->rEINT33PEND);

	uConValue = (1<<uEINT_No);
	*pEINT33PEND = uConValue;

}

//////////
// Function Name : GPIO_EINT33UnMask
// Function Desctiption : UnMask the Eint33[7:0]
// Input : 			uEINT_No: EINT No.
// Output : NONE
//
// Version : v0.0
void GPIO_EINT33UnMask(unsigned int uEINT_No )
{

	volatile unsigned int *pMASK_Reg;
	unsigned int uConValue;

	pMASK_Reg = &(GPIO->rEINT33MASK);
	
	uConValue = *pMASK_Reg;
       uConValue = (uConValue & ~(0x1<<(uEINT_No)));
	*pMASK_Reg = uConValue;

}

//////////
// Function Name : GPIO_EINT33Mask
// Function Desctiption : Mask the Eint33[7:0]
// Input : 			uEINT_No: EINT No.
// Output : NONE
//
// Version : v0.0
void GPIO_EINT33Mask(unsigned int uEINT_No )
{

	volatile unsigned int *pMASK_Reg;
	unsigned int uConValue;

	pMASK_Reg = &(GPIO->rEINT33MASK);
	
	uConValue = *pMASK_Reg;
       uConValue = (uConValue & ~(0x1<<(uEINT_No)))|(1<<uEINT_No);
	*pMASK_Reg = uConValue;

}





//////////
// Function Name : EINT_GRPPRI
// Function Desctiption : EINT Group priority rotate enable/diable (Fixed)
// Input : 			uEINT_No: EINT No.
// Output : NONE
//
// Version : v0.0
void EINT_GRPPRI(PRI_eTYPE uPriType )
{

	volatile unsigned int *pEINT_GrpPri;
	unsigned int uConValue;

	pEINT_GrpPri = &(GPIO->rEINTGRPPRI);
	
	uConValue = *pEINT_GrpPri;
       uConValue = (uConValue & ~(0x1))|(uPriType);
	*pEINT_GrpPri = uConValue;

}


//////////
// Function Name : EINT_PRIORITY
// Function Desctiption : Every EINT group priority rotate enable/diable (Fixed)
// Input : 			uEINT_No: EINT No.
// Output : NONE
//
// Version : v0.0
void EINT_PRIORITY(unsigned int uEINT_No )
{

	volatile unsigned int *pEINT_Priority;
	unsigned int uConValue;

	pEINT_Priority = &(GPIO->rEINTPRIORITY);
	
	uConValue = *pEINT_Priority;
       uConValue = (uConValue & ~(0x1<<(uEINT_No-1)))|(1<<uEINT_No-1);
	*pEINT_Priority = uConValue;

}



//////////
// Function Name : Get_EINTSVC
// Function Desctiption : This function get EINT service group and interrupt number
// Input : 	None
// Output : 	Data register value
//
// Version : v0.0

unsigned int Get_EINTSVC(void)
{
	volatile unsigned int *pEINT_Service;
	volatile unsigned int *pGPIO_Base_Addr;

	pEINT_Service = &(GPIO->rEINTSERVICE);	
	return (*pEINT_Service);
}


//////////
// Function Name : Get_EINTSVCPND
// Function Desctiption : This function get pending EINT service group and interrupt number
// Input : 	None
// Output : 	Data register value
//
// Version : v0.0

unsigned int Get_EINTSVCPEND(void)
{
	volatile unsigned int *pEINT_ServicePnd;
	volatile unsigned int *pGPIO_Base_Addr;

	pEINT_ServicePnd = &(GPIO->rEINTSERVICEPEND);	
	return (*pEINT_ServicePnd);
}



//////////
// Function Name : EINT_GRPFIXPRI
// Function Desctiption : Group number of the highest priority when fixed group priority mode (EINT1 ~ 22)
// Input : 			uEINT_No: EINT No.
// Output : NONE
//
// Version : v0.0
void EINT_GRPFIXPRI(unsigned int uEINT_No )
{

	volatile unsigned int *pEINT_GrpFixPri;
	unsigned int uConValue;

	pEINT_GrpFixPri = &(GPIO->rEINTGRPFIXPRI);
	
	uConValue = *pEINT_GrpFixPri;
       uConValue = (uConValue & ~(0x1))|(uEINT_No);
	*pEINT_GrpFixPri = uConValue;

}


//////////
// Function Name : EINT_FIXPRI
// Function Desctiption : interrupt number of the highest priority when fixed group priority mode (0 ~ 7)
// Input :                uEINT_Grp_No			
//                           uEINT_No: EINT No.
// Output : NONE
//
// Version : v0.0
void EINT_FIXPRI(unsigned int uEINT_Grp_No, unsigned int uEINT_No  )
{

	volatile unsigned int *pEINT_Grpx_FixPri, *pEINT_Grp1_FixPri;
	unsigned int uConValue;

	pEINT_Grp1_FixPri = &(GPIO->rEINT1FIXPRI);
	pEINT_Grpx_FixPri = pEINT_Grp1_FixPri  + (uEINT_Grp_No-1);
	
	uConValue = *pEINT_Grpx_FixPri;
       uConValue = (uConValue & ~(0x1))|(uEINT_No);
	*pEINT_Grpx_FixPri = uConValue;

}


void GPIO_SetFunctionForLts222QLdi(void)
{
	GPIO_SetFunctionEach(eGPIO_B, eGPIO_5, 0x1); // CS
	GPIO_SetFunctionEach(eGPIO_B, eGPIO_4, 0x1);	// CLK
	GPIO_SetFunctionEach(eGPIO_B, eGPIO_7, 0x1);	// DATA 
}

void GPIO_SetDataForLts222QLdi(u8 cData)
{
	volatile int i, j;

	GPIO_SetDataEach(eGPIO_B, eGPIO_5, 0x1); // CS high
	GPIO_SetDataEach(eGPIO_B, eGPIO_4, 0x1); // CLK high

	for (j=0; j<4; j++);

	GPIO_SetDataEach(eGPIO_B, eGPIO_5, 0x0); // CS low

//	for (j=0; j<2; j++);
									
	for (i=7; i>=0; i--)
	{                                                               
		if (cData &  (1<<i))
		{      
			GPIO_SetDataEach(eGPIO_B, eGPIO_4, 0x0); // CLK low
			GPIO_SetDataEach(eGPIO_B, eGPIO_7, 0x1); // DATA high

			for (j=0; j<2; j++);

			GPIO_SetDataEach(eGPIO_B, eGPIO_4, 0x1); // CLK high
			//GPIO_SetDataEach(eGPIO_B, eGPIO_7, 0x1); // DATA high	

			for (j=0; j<2; j++);
		}           

		else
		{                                                       
			GPIO_SetDataEach(eGPIO_B, eGPIO_4, 0x0); // CLK low
			GPIO_SetDataEach(eGPIO_B, eGPIO_7, 0x0); // DATA low

			for (j=0; j<2; j++);

			GPIO_SetDataEach(eGPIO_B, eGPIO_4, 0x1); // CLK high
			//GPIO_SetDataEach(eGPIO_N, eGPIO_3, 0x0); // DATA low	

			for (j=0; j<2; j++);
		}
	}

	GPIO_SetDataEach(eGPIO_B, eGPIO_5, 0x0); // CS low

}
#endif

/* ktj make boot command */
/////////////////////////////////////////////////////////////////
#include <regs.h>

int do_gpio(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
    unsigned int gpio_id;
    unsigned int gpio_num;
    unsigned int gpio_level;

	if ( argc != 4 )
	{
		printf("Usage: gpio <group_id> <gpio_num> <gpio_level> \n");
		return 0;
	}

    gpio_num = atoi(argv[2]);
    gpio_level = atoi(argv[3]);

	if ( strcmp(argv[1], "A0") == 0 )       gpio_id = eGPIO_A0;
	else if ( strcmp(argv[1], "A1") == 0 )  gpio_id = eGPIO_A1;

	else if ( strcmp(argv[1], "B") == 0 )   gpio_id = eGPIO_B;

	else if ( strcmp(argv[1], "C0") == 0 )  gpio_id = eGPIO_C0;
	else if ( strcmp(argv[1], "C1") == 0 )  gpio_id = eGPIO_C1;

	else if ( strcmp(argv[1], "D0") == 0 )  gpio_id = eGPIO_D0;
	else if ( strcmp(argv[1], "D1") == 0 )  gpio_id = eGPIO_D1;

	else if ( strcmp(argv[1], "E0") == 0 )  gpio_id = eGPIO_E0;
	else if ( strcmp(argv[1], "E1") == 0 )  gpio_id = eGPIO_E1;

	else if ( strcmp(argv[1], "F0") == 0 )  gpio_id = eGPIO_F0;
	else if ( strcmp(argv[1], "F1") == 0 )  gpio_id = eGPIO_F1;
	else if ( strcmp(argv[1], "F2") == 0 )  gpio_id = eGPIO_F2;
	else if ( strcmp(argv[1], "F3") == 0 )  gpio_id = eGPIO_F3;

	else if ( strcmp(argv[1], "G0") == 0 )  gpio_id = eGPIO_G0;
	else if ( strcmp(argv[1], "G1") == 0 )  gpio_id = eGPIO_G1;
	else if ( strcmp(argv[1], "G2") == 0 )  gpio_id = eGPIO_G2;
	else if ( strcmp(argv[1], "G3") == 0 )  gpio_id = eGPIO_G3;

	else if ( strcmp(argv[1], "I") == 0 )   gpio_id = eGPIO_I;

	else if ( strcmp(argv[1], "J0") == 0 )  gpio_id = eGPIO_J0;
	else if ( strcmp(argv[1], "J1") == 0 )  gpio_id = eGPIO_J1;
	else if ( strcmp(argv[1], "J2") == 0 )  gpio_id = eGPIO_J2;
	else if ( strcmp(argv[1], "J3") == 0 )  gpio_id = eGPIO_J3;
	else if ( strcmp(argv[1], "J4") == 0 )  gpio_id = eGPIO_J4;

	else if ( strcmp(argv[1], "MP0_1") == 0 ) gpio_id = eGPIO_MP0_1;
	else if ( strcmp(argv[1], "MP0_2") == 0 ) gpio_id = eGPIO_MP0_2;
	else if ( strcmp(argv[1], "MP0_3") == 0 ) gpio_id = eGPIO_MP0_3;
	else if ( strcmp(argv[1], "MP0_4") == 0 ) gpio_id = eGPIO_MP0_4;
	else if ( strcmp(argv[1], "MP0_5") == 0 ) gpio_id = eGPIO_MP0_5;
	else if ( strcmp(argv[1], "MP0_6") == 0 ) gpio_id = eGPIO_MP0_6;
	else if ( strcmp(argv[1], "MP0_7") == 0 ) gpio_id = eGPIO_MP0_7;

	else if ( strcmp(argv[1], "MP1_0") == 0 ) gpio_id = eGPIO_MP1_0;
	else if ( strcmp(argv[1], "MP1_1") == 0 ) gpio_id = eGPIO_MP1_1;
	else if ( strcmp(argv[1], "MP1_2") == 0 ) gpio_id = eGPIO_MP1_2;
	else if ( strcmp(argv[1], "MP1_3") == 0 ) gpio_id = eGPIO_MP1_3;
	else if ( strcmp(argv[1], "MP1_4") == 0 ) gpio_id = eGPIO_MP1_4;
	else if ( strcmp(argv[1], "MP1_5") == 0 ) gpio_id = eGPIO_MP1_5;
	else if ( strcmp(argv[1], "MP1_6") == 0 ) gpio_id = eGPIO_MP1_6;
	else if ( strcmp(argv[1], "MP1_7") == 0 ) gpio_id = eGPIO_MP1_7;
	else if ( strcmp(argv[1], "MP1_8") == 0 ) gpio_id = eGPIO_MP1_8;

	else if ( strcmp(argv[1], "MP2_0") == 0 ) gpio_id = eGPIO_MP2_0;
	else if ( strcmp(argv[1], "MP2_1") == 0 ) gpio_id = eGPIO_MP2_1;
	else if ( strcmp(argv[1], "MP2_2") == 0 ) gpio_id = eGPIO_MP2_2;
	else if ( strcmp(argv[1], "MP2_3") == 0 ) gpio_id = eGPIO_MP2_3;
	else if ( strcmp(argv[1], "MP2_4") == 0 ) gpio_id = eGPIO_MP2_4;
	else if ( strcmp(argv[1], "MP2_5") == 0 ) gpio_id = eGPIO_MP2_5;
	else if ( strcmp(argv[1], "MP2_6") == 0 ) gpio_id = eGPIO_MP2_6;
	else if ( strcmp(argv[1], "MP2_7") == 0 ) gpio_id = eGPIO_MP2_7;
	else if ( strcmp(argv[1], "MP2_8") == 0 ) gpio_id = eGPIO_MP2_8;

	else if ( strcmp(argv[1], "H0") == 0 ) gpio_id = eGPIO_H0;
	else if ( strcmp(argv[1], "H1") == 0 ) gpio_id = eGPIO_H1;
	else if ( strcmp(argv[1], "H2") == 0 ) gpio_id = eGPIO_H2;
	else if ( strcmp(argv[1], "H3") == 0 ) gpio_id = eGPIO_H3;
	    
	else {
		printf("Usage: gpio <gpio_id> <gpio_num> <gpio_level> \n");
		return 0;
	}

    GPIO_SetFunctionEach(gpio_id, gpio_num, 1);	
    GPIO_SetDataEach(gpio_id, gpio_num, gpio_level);

	printf("[gpio_set] gpio_id = 0x%x gpio_num = %d gpio_level = %d \n\n", gpio_id, gpio_num, gpio_level);
	
	return 0;
}

U_BOOT_CMD(
        gpio, 4, 0, do_gpio,
        "fdisk\t- fdisk for sd/mmc.\n",
        "-c <device_num>\t- create partition.\n"
        "fdisk -p <device_num>\t- print partition information\n"
);


