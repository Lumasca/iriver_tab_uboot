/**************************************************************************************
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
*	File Name : gpio.h
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
*  
**************************************************************************************/

#ifndef __GPIO_H__
#define __GPIO_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "smdkc110_def.h"


#define		Low_Level		0x0
#define		High_Level		0x1
#define		Falling_Edge	0x2
#define		Rising_Edge		0x3
#define		Both_Edge		0x4

//#define ENABLE_EINT

typedef enum GPIO_Id
{
	// Value & 0xFFF : Offset from GPIO base address(Value * 0x10)
	
	eGPIO_A0 = 0x000,    		// muxing bit : 4, 		offset : 0, 		
	eGPIO_A1 = 0x020,    		// muxing bit : 4, 		offset : 0x02*0x10 		
	eGPIO_B = 0x040,    		       // muxing bit : 4, 		offset : 0x04*0x10
	eGPIO_C0 = 0x060,    		// muxing bit : 4, 		offset : 0x06*0x10
	eGPIO_C1 = 0x080,    		// muxing bit : 4, 		offset : 0x08*0x10
	eGPIO_D0 = 0x0A0,    		// muxing bit : 4, 		offset : 0x0A*0x10
	eGPIO_D1 = 0x0C0,    		// muxing bit : 4, 		offset : 0x0C*0x10
	eGPIO_E0 = 0x0E0,    		// muxing bit : 4, 		offset : 0x0E*0x10
	eGPIO_E1 = 0x100,    		// muxing bit : 4, 		offset : 0x10*0x10	
	eGPIO_F0 = 0x120,    		// muxing bit : 4, 		offset : 0x12*0x10
	eGPIO_F1 = 0x140,    		// muxing bit : 4, 		offset : 0x14*0x10
	eGPIO_F2 = 0x160,    		// muxing bit : 4, 		offset : 0x16*0x10
	eGPIO_F3 = 0x180,    		// muxing bit : 4, 		offset : 0x18*0x10
	eGPIO_G0 = 0x1A0,    		// muxing bit : 4, 		offset : 0x1A*0x10
	eGPIO_G1 = 0x1C0,    		// muxing bit : 4, 		offset : 0x1C*0x10
	eGPIO_G2 = 0x1E0,    		// muxing bit : 4, 		offset : 0x1E*0x10
	eGPIO_G3 = 0x200,    		// muxing bit : 4, 		offset : 0x20*0x10

	eGPIO_I = 0x220,    		       // muxing bit : 4, 		offset : 0x22*0x10
	eGPIO_J0 = 0x240,    		// muxing bit : 4, 		offset : 0x24*0x10
	eGPIO_J1 = 0x260,    		// muxing bit : 4, 		offset : 0x26*0x10
	eGPIO_J2 = 0x280,    		// muxing bit : 4, 		offset : 0x28*0x10
	eGPIO_J3 = 0x2A0,    		// muxing bit : 4, 		offset : 0x2A*0x10
	eGPIO_J4 = 0x2C0,    		// muxing bit : 4, 		offset : 0x2C*0x10

	eGPIO_MP0_1 = 0x2E0,    		// muxing bit : 4, 		offset : 0x2E*0x10
	eGPIO_MP0_2 = 0x300,    		// muxing bit : 4, 		offset : 0x30*0x10
	eGPIO_MP0_3 = 0x320,    		// muxing bit : 4, 		offset : 0x32*0x10
	eGPIO_MP0_4 = 0x340,    		// muxing bit : 4, 		offset : 0x34*0x10
	eGPIO_MP0_5 = 0x360,    		// muxing bit : 4, 		offset : 0x36*0x10
	eGPIO_MP0_6 = 0x380,    		// muxing bit : 4, 		offset : 0x38*0x10
	eGPIO_MP0_7 = 0x3A0,    		// muxing bit : 4, 		offset : 0x3A*0x10

       eGPIO_MP1_0 = 0x3C0,    		// muxing bit : 4, 		offset : 0x3C*0x10
	eGPIO_MP1_1 = 0x3E0,    		// muxing bit : 4, 		offset : 0x3E*0x10
	eGPIO_MP1_2 = 0x400,    		// muxing bit : 4, 		offset : 0x40*0x10
	eGPIO_MP1_3 = 0x420,    		// muxing bit : 4, 		offset : 0x42*0x10
	eGPIO_MP1_4 = 0x440,    		// muxing bit : 4, 		offset : 0x44*0x10
	eGPIO_MP1_5 = 0x460,    		// muxing bit : 4, 		offset : 0x46*0x10
	eGPIO_MP1_6 = 0x480,    		// muxing bit : 4, 		offset : 0x48*0x10
	eGPIO_MP1_7 = 0x4A0,    		// muxing bit : 4, 		offset : 0x4A*0x10
	eGPIO_MP1_8 = 0x4C0,    		// muxing bit : 4, 		offset : 0x4C*0x10

	eGPIO_MP2_0 = 0x4E0,    		// muxing bit : 4, 		offset : 0x4E*0x10
	eGPIO_MP2_1 = 0x500,    		// muxing bit : 4, 		offset : 0x50*0x10
	eGPIO_MP2_2 = 0x520,    		// muxing bit : 4, 		offset : 0x52*0x10
	eGPIO_MP2_3 = 0x540,    		// muxing bit : 4, 		offset : 0x54*0x10
	eGPIO_MP2_4 = 0x560,    		// muxing bit : 4, 		offset : 0x56*0x10
	eGPIO_MP2_5 = 0x580,    		// muxing bit : 4, 	,	offset : 0x58*0x10
	eGPIO_MP2_6 = 0x5A0,    		// muxing bit : 4, 		offset : 0x5A*0x10
	eGPIO_MP2_7 = 0x5C0,    		// muxing bit : 4, 		offset : 0x5C*0x10
	eGPIO_MP2_8 = 0x5E0,    		// muxing bit : 4, 		offset : 0x5E*0x10

	eETC0 = 0x608,    		// offset(3bit) : 0x608
	eETC1 = 0x628,    		// offset(3bit) : 0x628
	eETC2 = 0x648,    		// offset(3bit) : 0x648
	eETC3 = 0x668,    		// offset(3bit) : 0x668
	eETC4 = 0x688,    		// offset(3bit) : 0x688

	eGPIO_H0 = 0xC00,    		// muxing bit : 4, 		offset : 0xC0*0x10
	eGPIO_H1 = 0xC20,    		// muxing bit : 4, 		offset : 0xC2*0x10
	eGPIO_H2 = 0xC40,    		// muxing bit : 4, 		offset : 0xC4*0x10
	eGPIO_H3 = 0xC60,    		// muxing bit : 4, 		offset : 0xC6*0x10
	
}
GPIO_eId;


typedef enum GPIO_BitPos
{
	eGPIO_0	= (unsigned int)(0),
	eGPIO_1	= (unsigned int)(1),
	eGPIO_2	= (unsigned int)(2),
	eGPIO_3	= (unsigned int)(3),
	eGPIO_4	= (unsigned int)(4),
	eGPIO_5	= (unsigned int)(5),
	eGPIO_6	= (unsigned int)(6),
	eGPIO_7	= (unsigned int)(7),
	eGPIO_8	= (unsigned int)(8),
	eGPIO_9	= (unsigned int)(9),
	eGPIO_10	= (unsigned int)(10),
	eGPIO_11	= (unsigned int)(11),
	eGPIO_12	= (unsigned int)(12),
	eGPIO_13	= (unsigned int)(13),
	eGPIO_14	= (unsigned int)(14),
	eGPIO_15	= (unsigned int)(15)
}GPIO_eBitPos;


// added by junon
typedef enum GPIO_Func
{
	eGPI		= (unsigned int)(0),	// GPIO input 
	eGPO		= (unsigned int)(1),	// GPIO output
	eGFunc_0	= (unsigned int)(2),	// Function 0 select
	eGFunc_1	= (unsigned int)(3),	// Function 1 select
	eGFunc_2	= (unsigned int)(4),	// Function 2 select
	eGFunc_3	= (unsigned int)(5),	// Function 3 select
	eGFunc_4	= (unsigned int)(6),	// Function 4 select
	eGFunc_5	= (unsigned int)(7),
	eGFunc_6	= (unsigned int)(8),
	eGFunc_7	= (unsigned int)(9),
	eGFunc_8	= (unsigned int)(10),
	eGFunc_9	= (unsigned int)(11),
	eGFunc_10	= (unsigned int)(12),
	eGFunc_11	= (unsigned int)(13),
	eGFunc_12	= (unsigned int)(14),
	eGINT		= (unsigned int)(15),	// General external interrupt (no wake up)
} GPIO_eFunc;

typedef enum GPIO_Data
{
	eLow	= (unsigned int)(0),	// pull-up/down disable
	eHigh		= (unsigned int)(1),	// pull-down enable
} GPIO_eData;


typedef enum GPIO_PUD
{
	eGPUDdis	= (unsigned int)(0),	// pull-up/down disable
	eGPDen		= (unsigned int)(1),	// pull-down enable
	eGPUen		= (unsigned int)(2),	// pull-up enable
	eGPRes		= (unsigned int)(3),	// reserved
} GPIO_ePUD;




typedef enum FLT_TYPE
{
	eDisFLT	= (unsigned int) 0x0,		// Digital Fiter Off (EINT Group 1~23 )
	eEnFLT	= (unsigned int) 0x1, 		// Digital Fiter On (EINT Group 1~23)
	eDLYFLT	= (unsigned int) 0x2,		// EINT Group 30~33, 2'b10 => Filter On & Delay Filter
	eDIGFLT	= (unsigned int) 0x3		// EINT Group 30~33, 2'b11 => Fiter On & Digital Filter
} FLT_eTYPE;

typedef enum PRI_TYPE
{
	eFixed	= (unsigned int) 0x0,		// Digital Fiter Off (EINT Group 1~23 )
	eRotate	= (unsigned int) 0x1, 		// Digital Fiter On (EINT Group 1~23)
} PRI_eTYPE;


/*
typedef enum LCD_TYPE
{
	eHOSTIF	= 	0,
	eRGBIF		=	1,
	eYCbCr		=	2,
	eHidden		=	3
} LCD_eTYPE;
*/

void GPIO_Init(void);
void GPIO_SetFunctionEach(GPIO_eId Id, GPIO_eBitPos eBitPos, unsigned int uFunction);
void GPIO_SetFunctionAll(GPIO_eId Id, unsigned int uValue0);
void GPIO_SetDataEach(GPIO_eId Id, GPIO_eBitPos eBitPos, unsigned int uValue);
void GPIO_SetDataAll(GPIO_eId Id, unsigned int uValue);
unsigned int GPIO_GetDataEach(GPIO_eId Id,GPIO_eBitPos eBitPos);  //added by jhlim

unsigned int GPIO_GetDataAll(GPIO_eId Id);
void GPIO_SetPullUpDownEach(GPIO_eId Id, GPIO_eBitPos eBitPos, unsigned int uValue);
void GPIO_SetPullUpDownAll(GPIO_eId Id, unsigned int uValue);
void ETC_SetPullUpDownEach(GPIO_eId Id, GPIO_eBitPos eBitPos, unsigned int uValue);
void ETC_SetPullUpDownAll(GPIO_eId Id, unsigned int uValue);
void GPIO_SetDSEach(GPIO_eId Id, GPIO_eBitPos eBitPos, unsigned int uValue);

void GPIO_SetDSAll(GPIO_eId Id, unsigned int uValue);
void GPIO_SetSREach(GPIO_eId Id, GPIO_eBitPos eBitPos, unsigned int uValue);
void GPIO_SetSRAll(GPIO_eId Id, unsigned int uValue);
void GPIO_SetConRegPDNAll(GPIO_eId Id, unsigned int uValue);
void GPIO_SetPullUDPDNAll(GPIO_eId Id, unsigned int uValue);

#ifdef ENABLE_EINT
void GPIO_SetEint1(unsigned int uEINT_No , unsigned int uIntType, FLT_eTYPE eFltType,  unsigned int uFltWidth);
void GPIO_EINT1ClrPend(unsigned int uEINT_No );
void GPIO_EINT1UnMask(unsigned int uEINT_No );
void GPIO_EINT1Mask(unsigned int uEINT_No );


void GPIO_SetEint2(unsigned int uEINT_No , unsigned int uIntType, FLT_eTYPE eFltType,  unsigned int uFltWidth);
void GPIO_EINT2ClrPend(unsigned int uEINT_No );
void GPIO_EINT2UnMask(unsigned int uEINT_No );
void GPIO_EINT2Mask(unsigned int uEINT_No );


void GPIO_SetEint3(unsigned int uEINT_No , unsigned int uIntType, FLT_eTYPE eFltType,  unsigned int uFltWidth);
void GPIO_EINT3ClrPend(unsigned int uEINT_No );
void GPIO_EINT3UnMask(unsigned int uEINT_No );
void GPIO_EINT3Mask(unsigned int uEINT_No );


void GPIO_SetEint4(unsigned int uEINT_No , unsigned int uIntType, FLT_eTYPE eFltType,  unsigned int uFltWidth);
void GPIO_EINT4ClrPend(unsigned int uEINT_No );
void GPIO_EINT4UnMask(unsigned int uEINT_No );
void GPIO_EINT4Mask(unsigned int uEINT_No );


void GPIO_SetEint5(unsigned int uEINT_No , unsigned int uIntType, FLT_eTYPE eFltType,  unsigned int uFltWidth);
void GPIO_EINT5ClrPend(unsigned int uEINT_No );
void GPIO_EINT5UnMask(unsigned int uEINT_No );
void GPIO_EINT5Mask(unsigned int uEINT_No );


void GPIO_SetEint6(unsigned int uEINT_No , unsigned int uIntType, FLT_eTYPE eFltType,  unsigned int uFltWidth);
void GPIO_EINT6ClrPend(unsigned int uEINT_No );
void GPIO_EINT6UnMask(unsigned int uEINT_No );
void GPIO_EINT6Mask(unsigned int uEINT_No );


void GPIO_SetEint7(unsigned int uEINT_No , unsigned int uIntType, FLT_eTYPE eFltType,  unsigned int uFltWidth);
void GPIO_EINT7ClrPend(unsigned int uEINT_No );
void GPIO_EINT7UnMask(unsigned int uEINT_No );
void GPIO_EINT7Mask(unsigned int uEINT_No );



void GPIO_SetEint8(unsigned int uEINT_No , unsigned int uIntType, FLT_eTYPE eFltType,  unsigned int uFltWidth);
void GPIO_EINT8ClrPend(unsigned int uEINT_No );
void GPIO_EINT8UnMask(unsigned int uEINT_No );
void GPIO_EINT8Mask(unsigned int uEINT_No );


void GPIO_SetEint9(unsigned int uEINT_No , unsigned int uIntType, FLT_eTYPE eFltType,  unsigned int uFltWidth);
void GPIO_EINT9ClrPend(unsigned int uEINT_No );
void GPIO_EINT9UnMask(unsigned int uEINT_No );
void GPIO_EINT9Mask(unsigned int uEINT_No );



void GPIO_SetEint10(unsigned int uEINT_No , unsigned int uIntType, FLT_eTYPE eFltType,  unsigned int uFltWidth);
void GPIO_EINT10ClrPend(unsigned int uEINT_No );
void GPIO_EINT10UnMask(unsigned int uEINT_No );
void GPIO_EINT10Mask(unsigned int uEINT_No );


void GPIO_SetEint11(unsigned int uEINT_No , unsigned int uIntType, FLT_eTYPE eFltType,  unsigned int uFltWidth);
void GPIO_EINT11ClrPend(unsigned int uEINT_No );
void GPIO_EINT11UnMask(unsigned int uEINT_No );
void GPIO_EINT11Mask(unsigned int uEINT_No );


void GPIO_SetEint12(unsigned int uEINT_No , unsigned int uIntType, FLT_eTYPE eFltType,  unsigned int uFltWidth);
void GPIO_EINT12ClrPend(unsigned int uEINT_No );
void GPIO_EINT12UnMask(unsigned int uEINT_No );
void GPIO_EINT12Mask(unsigned int uEINT_No );


void GPIO_SetEint13(unsigned int uEINT_No , unsigned int uIntType, FLT_eTYPE eFltType,  unsigned int uFltWidth);
void GPIO_EINT13ClrPend(unsigned int uEINT_No );
void GPIO_EINT13UnMask(unsigned int uEINT_No );
void GPIO_EINT13Mask(unsigned int uEINT_No );


void GPIO_SetEint14(unsigned int uEINT_No , unsigned int uIntType, FLT_eTYPE eFltType,  unsigned int uFltWidth);
void GPIO_EINT14ClrPend(unsigned int uEINT_No );
void GPIO_EINT14UnMask(unsigned int uEINT_No );
void GPIO_EINT14Mask(unsigned int uEINT_No );


void GPIO_SetEint15(unsigned int uEINT_No , unsigned int uIntType, FLT_eTYPE eFltType,  unsigned int uFltWidth);
void GPIO_EINT15ClrPend(unsigned int uEINT_No );
void GPIO_EINT15UnMask(unsigned int uEINT_No );
void GPIO_EINT15Mask(unsigned int uEINT_No );


void GPIO_SetEint16(unsigned int uEINT_No , unsigned int uIntType, FLT_eTYPE eFltType,  unsigned int uFltWidth);
void GPIO_EINT16ClrPend(unsigned int uEINT_No );
void GPIO_EINT16UnMask(unsigned int uEINT_No );
void GPIO_EINT16Mask(unsigned int uEINT_No );


void GPIO_SetEint17(unsigned int uEINT_No , unsigned int uIntType, FLT_eTYPE eFltType,  unsigned int uFltWidth);
void GPIO_EINT17ClrPend(unsigned int uEINT_No );
void GPIO_EINT17UnMask(unsigned int uEINT_No );
void GPIO_EINT17Mask(unsigned int uEINT_No );



void GPIO_SetEint18(unsigned int uEINT_No , unsigned int uIntType, FLT_eTYPE eFltType,  unsigned int uFltWidth);
void GPIO_EINT18ClrPend(unsigned int uEINT_No );
void GPIO_EINT18UnMask(unsigned int uEINT_No );
void GPIO_EINT18Mask(unsigned int uEINT_No );


void GPIO_SetEint19(unsigned int uEINT_No , unsigned int uIntType, FLT_eTYPE eFltType,  unsigned int uFltWidth);
void GPIO_EINT19ClrPend(unsigned int uEINT_No );
void GPIO_EINT19UnMask(unsigned int uEINT_No );
void GPIO_EINT19Mask(unsigned int uEINT_No );



void GPIO_SetEint20(unsigned int uEINT_No , unsigned int uIntType, FLT_eTYPE eFltType,  unsigned int uFltWidth);
void GPIO_EINT20ClrPend(unsigned int uEINT_No );
void GPIO_EINT20UnMask(unsigned int uEINT_No );
void GPIO_EINT20Mask(unsigned int uEINT_No );


void GPIO_SetEint21(unsigned int uEINT_No , unsigned int uIntType, FLT_eTYPE eFltType,  unsigned int uFltWidth);
void GPIO_EINT21ClrPend(unsigned int uEINT_No );
void GPIO_EINT21UnMask(unsigned int uEINT_No );
void GPIO_EINT21Mask(unsigned int uEINT_No );


void GPIO_SetEint22(unsigned int uEINT_No , unsigned int uIntType, FLT_eTYPE eFltType,  unsigned int uFltWidth);
void GPIO_EINT22ClrPend(unsigned int uEINT_No );
void GPIO_EINT22UnMask(unsigned int uEINT_No );
void GPIO_EINT22Mask(unsigned int uEINT_No );


void GPIO_SetEint23(unsigned int uEINT_No , unsigned int uIntType, FLT_eTYPE eFltType,  unsigned int uFltWidth);
void GPIO_EINT23ClrPend(unsigned int uEINT_No );
void GPIO_EINT23UnMask(unsigned int uEINT_No );
void GPIO_EINT23Mask(unsigned int uEINT_No );


void GPIO_SetEint30(unsigned int uEINT_No , unsigned int uIntType, FLT_eTYPE eFltType,  unsigned int uFltWidth);
void GPIO_EINT30ClrPend(unsigned int uEINT_No );
void GPIO_EINT30UnMask(unsigned int uEINT_No );
void GPIO_EINT30Mask(unsigned int uEINT_No );


void GPIO_SetEint31(unsigned int uEINT_No , unsigned int uIntType, FLT_eTYPE eFltType,  unsigned int uFltWidth);
void GPIO_EINT31ClrPend(unsigned int uEINT_No );
void GPIO_EINT31UnMask(unsigned int uEINT_No );
void GPIO_EINT31Mask(unsigned int uEINT_No );


void GPIO_SetEint32(unsigned int uEINT_No , unsigned int uIntType, FLT_eTYPE eFltType,  unsigned int uFltWidth);
void GPIO_EINT32ClrPend(unsigned int uEINT_No );
void GPIO_EINT32UnMask(unsigned int uEINT_No );
void GPIO_EINT32Mask(unsigned int uEINT_No );


void GPIO_SetEint33(unsigned int uEINT_No , unsigned int uIntType, FLT_eTYPE eFltType,  unsigned int uFltWidth);
void GPIO_EINT33ClrPend(unsigned int uEINT_No );
void GPIO_EINT33UnMask(unsigned int uEINT_No );
void GPIO_EINT33Mask(unsigned int uEINT_No );

#endif

void EINT_GRPPRI(PRI_eTYPE uPriType );

unsigned int Get_EINTSVC(void);
unsigned int Get_EINTSVCPEND(void);
void EINT_GRPFIXPRI(unsigned int uEINT_No );
void EINT_FIXPRI(unsigned int uEINT_Grp_No, unsigned int uEINT_No  );


#ifdef __cplusplus
}
#endif

#endif //__GPIO_H__

