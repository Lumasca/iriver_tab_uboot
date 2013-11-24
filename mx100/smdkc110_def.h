/**************************************************************************************
* 
*	Project Name : S5PC100 Validation
*
*	Copyright 2006 by Samsung Electronics, Inc.
*	All rights reserved.
*
*	Project Description :
*		This software is only for validating functions of the S5PC100.
*		Anybody can use this software without our permission.
*  
*--------------------------------------------------------------------------------------
* 
*	File Name : def.h
*  
*	File Description : This file defines some types used commonly.
*
*	Author : Haksoo,Kim
*	Dept. : AP Development Team
*	Created Date : 2006/11/08
*	Version : 0.1 
* 
*	History
*	- Created(Haksoo,Kim 2006/11/08)
*  
**************************************************************************************/

#ifndef __DEF_H__
#define __DEF_H__

#ifdef __cplusplus
extern "C" {
#endif
#define S5PC110
// Type defines 
#if 0
typedef unsigned long		u32;
typedef unsigned short		u16;
typedef unsigned char		u8;

typedef signed long			s32;
typedef signed short		s16;
typedef signed char			s8;

typedef unsigned char		bool;
#endif

//added by rb1004
#define OK                  				0
#define ERROR               			(-1)
#define TIMEOUT             			(-2)
#define REPEATER_NOT_READY  	(-3)
#define ILLEGAL_DEVICE      		(-4)

#define FALSE			(0)
#define TRUE				(1)
#define false				(0)
#define true				(1)

#define FPGA_16MHz			(1)
#define FPGA_16_9344MHz		(0)
#define FPGA_20MHz			(0)
#define FPGA_27MHz			(0)

#define Getc				UART_Getc
#define Putc				UART_Putc
#define GetKey			UART_GetKey
#define Disp				UART_Printf
#define Dbg        		 	UART_Printf
#define GetString			UART_GetString

typedef enum Function_Test
{
	eTest_Manual, eTest_Auto
}eFunction_Test;

typedef struct FunctionT_AutoVar
{
	unsigned int Test_Parameter[10];
}oFunctionT_AutoVar;

typedef struct testFuncMenu {
	void (*func)(void); 
	const char *desc;
} testFuncMenu;

typedef struct AutotestFuncMenu {
	void (*func)(eFunction_Test eTest, oFunctionT_AutoVar oParameter); 
	const char *desc;
} AutotestFuncMenu;

typedef enum
{
	NTSC_M,
	PAL_M,
	PAL_BGHID,
	PAL_N,
	PAL_NC,
	PAL_60,
	NTSC_443,
	NTSC_J
} TV_STANDARDS;

typedef enum
{
	FLIP_NO, FLIP_X, FLIP_Y, FLIP_XY
} FLIP_DIR;

typedef enum
{
	ROT_0, ROT_90, ROT_180, ROT_270
} ROT_DEG;

typedef enum
{
	BYTE, HWORD, WORD, DWORD
} DATA_SIZE;

typedef enum
{
	BT601, BT656
}ITU_R_STANDARD;

typedef enum
{
	RISING, FALLING
} POL;

typedef enum
{
	TX, RX
} DIR;

typedef enum 
{
	SINGLE, BURST4, BURST8, BURST16, BURST16X2, BURST32, BURST64, BURST128, BURST256
}BURST_MODE;

typedef enum
{
	HCLK_SRC, PCLK_SRC, ECLK0_SRC, ECLK1_SRC
} CLK_SRC;


typedef enum
{
	QCIF,			// 176X144 
	CIF, 			// 352x288 	// Video Conferencing
	QQVGA,
	QVGA,			// 320X240
	QVGA_PORT,		// 240X320	// QVGA Portrait
	VGA,			// 640X480
	WVGA,			// 800X480	// 6410 
	WVGA_PORT,		// 480X800	// WVGA Portrait
	SVGA,			// 800X600
	WSVGA,			// 1024X600
	XGA,			// 1024X768	// C100, C110
	SXGA,			// 1280X1024
	UXGA,			// 1600X1200
	WUXGA,			// 1920X1200
	QXGA,			// 2048X1536
	SD480,			// 720X480	//NTSC
	SD576,			// 720X576	// PAL
	HD720,			// 1280X720	// 720P
	HD1080,			// 1920X1080	// 1080P, 1080i
	CAM_8M			// 3264x2448	// 8M camera
}IMG_RESOLUTION;

// {IMG_RESOLUTION}_HSIZE,  {IMG_RESOLUTION}_VSIZE
#define	QCIF_HSIZE		176
#define	QCIF_VSIZE		144
#define	CIF_HSIZE		352
#define	CIF_VSIZE		288
#define	QVGA_HSIZE		320
#define	QVGA_VSIZE		240
#define	QVGA_PORT_HSIZE	240
#define	QVGA_PORT_VSIZE	320
#define	VGA_HSIZE		640
#define	VGA_VSIZE		480
#define	WVGA_HSIZE	800
#define	WVGA_VSIZE	480
#define	WVGA_PORT_HSIZE	480
#define	WVGA_PORT_VSIZE	800
#define	SVGA_HSIZE		800
#define	SVGA_VSIZE		600
#define	WSVGA_HSIZE	1024
#define	WSVGA_VSIZE	600
#define	XGA_HSIZE		1024
#define	XGA_VSIZE		768
#define	SXGA_HSZIE		1280
#define	SXGA_VSIZE		1024
#define	UXGA_HSIZE		1600
#define	UXGA_VSIZE		1200
#define	WUXGA_HSIZE	1920
#define	WUXGA_VSIZE	1200
#define	QXGA_HSIZE		2048
#define	QXGA_VSIZE		1536
#define	SD480_HSIZE	720
#define	SD480_VSIZE	480
#define	SD576_HSIZE	720
#define	SD576_VSIZE	576
#define	HD720_HSIZE	1280
#define	HD720_VSIZE	720
#define	HD1080_HSIZE	1920
#define	HD1080_VSIZE	1080


typedef enum
{	
	PROGRESSIVE,
	INTERLACED
} SCAN_MODE;


typedef enum
{
	//FIMD
	RGB565,
	RGB666,
	RGB888,
	ARGB8888,					// 32BPP Display
	PRE_ARGB8888,
	ARGB1888,					// 25BPP Display
	ARGB1887,					// 24BPP Display
	ARGB1666,					// 19BPP Display
	ARGB1555,					// 16BPP Display
	ARGB4444,






	RGB1555,					// 16BPP Display
	ARGB1444,					// 13BPP Display
	ARGB1232,					// 8BPP Display
	PAL8,						// Palette 8BPP
	PAL4,						// Palette 4BPP
	PAL2,						// Palette 2BPP
	PAL1,

	//FIMC 
	YCBCR420_2P_CRCB_LE,		// 2Plane, CRCB Little Endian  (Cr3Cb3Cr2Cb2Cr1Cb1Cr0Cb0)
	YCBCR420_2P_CBCR_LE,		// 2Plane, CBCR Little Endian (Cb3Cr3Cb2Cr2Cb1Cr1Cb0Cr0)
	YCBCR420_2P_CRCB_BE,		// 2Plane, CRCB Big Endian  (Cr0Cb0Cr1Cb1Cb2Cr2Cb3Cr3)
	YCBCR420_2P_CBCR_BE,		// 2Plane, CBCR Big Endian (Cb0Cr0Cb1Cr1Cb2Cr2Cb3Cr3)
	YCBCR420_3P,				// 3Plane

	YCBCR422_1P_YCBYCR,		// 1Plane,  Data Stored Order (Y3Cb1Y2Cr1Y1Cb0Y0Cr0)
	YCBCR422_1P_CBYCRY,		// 1Plane,  Data Stored Order (Cb1Y3Cr1Y2Cb0Y1Cr0Y0)
	YCBCR422_1P_YCRYCB,		// 1Plane,  Data Stored Order (Y3Cr1Y2Cb1Y1Cr0Y0Cb0)
	YCBCR422_1P_CRYCBY,		// 1Plane,  Data Stored Order (Cr1Y3Cb1Y2Cr0Y1Cb0Y0)
	YCBCR422_2P_CRCB_LE,		// 2Plane, CRCB Little Endian  (Cr3Cb3Cr2Cb2Cr1Cb1Cr0Cb0)	
	YCBCR422_2P_CBCR_LE,		// 2Plane, CBCR Little Endian (Cb3Cr3Cb2Cr2Cb1Cr1Cb0Cr0)
	YCBCR422_2P_CRCB_BE,		// 2Plane, CRCB Big Endian  (Cr0Cb0Cr1Cb1Cb2Cr2Cb3Cr3)
	YCBCR422_2P_CBCR_BE,		// 2Plane, CBCR Big Endian (Cb0Cr0Cb1Cr1Cb2Cr2Cb3Cr3)
	YCBCR422_3P,				// 3Plane

	YCBCR444_2P_CRCB_LE,		// 2Plane, CRCB Little Endian  (Cr3Cb3Cr2Cb2Cr1Cb1Cr0Cb0)	
	YCBCR444_2P_CBCR_LE,		// 2Plane, CBCR Little Endian (Cb3Cr3Cb2Cr2Cb1Cr1Cb0Cr0)
	YCBCR444_2P_CRCB_BE,		// 2Plane, CRCB Big Endian  (Cr0Cb0Cr1Cb1Cb2Cr2Cb3Cr3)
	YCBCR444_2P_CBCR_BE,		// 2Plane, CBCR Big Endian (Cb0Cr0Cb1Cr1Cb2Cr2Cb3Cr3)
	YCBCR444_3P,				// 3Plane

	EXT_RGB565,				// Extension RGB565 by FIMC(Input DMA Path) R&B[2:0] <= R&B[7:5], G[1:0] <= G[7:6]
	EXT_RGB666,				// Extension RGB666 by FIMC(Input DMA Path) R&G&B[1:0] <= R&G&B[7:6]
	
	BayerRGB8,					// Mipi CSI Raw RGB Data
	BayerRGB10,					// Mipi CSI Raw RGB Data
	BayerRGB12,					// Mipi CSI Raw RGB Data

	// Definition of the Tile Image & Interlaced Image
	YCBCR420_2P_CRCB_LE_IB,		// Interlaced Bob Image ( for VP Little Endian Type )
	YCBCR420_2P_CRCB_LE_IW,		// Interlaced Weave ( for VP  Little Endian Type)
	TILE_YCBCR420_2P_CRCB_LE,		// 64byte X 32line Tile Image (for VP Little Endian Type , MFC Decoder )
	TILE_YCBCR420_2P_CRCB_LE_IB,	// 64byte X 32line Tile Image & Interlaced Bob Image (for VP Little Endian Type)
	TILE_YCBCR420_2P_CRCB_LE_IW,	// 64byte X 32line Tile Image & Interlaced Weave Image (for VP Little Endian Type)	
	TILE16_YCBCR420_2P_CRCB_LE_IB,	// 16byte X 16line Tile Image ( for MFC Encoder )

	//The Definition of the previous devices	

	NV12_VP_BE,				// Only Used for VP,  (Y0Y1Y2Y3Y4Y5Y6Y7)(Cb0Cr0Cb1Cr1Cb2Cr2Cb3Cr3)
	NV12_IB_VP_BE,				// 
	NV12_IW_VP_BE,				//
	TILE_NV12_VP_BE,
	TILE_NV12_IB_VP_BE,
	TILE_NV12_IW_VP_BE,

	RGB8,
	ARGB8 = ARGB1232,

	RGB16_I,
	RGB24_I,	
	RGB16_IW,
	RGB24_IW,
	NV12_I = YCBCR420_2P_CRCB_LE_IB,

	RGB16 = RGB565,
	RGB24 = RGB888,	
	ARGB16 = ARGB1555,
	ARGB24 = ARGB1887,
	ARGB25 = ARGB1888,

	CRYCBY	=	YCBCR422_1P_CRYCBY,
	CBYCRY	=	YCBCR422_1P_CBYCRY,
	YCRYCB	=	YCBCR422_1P_YCRYCB,
	YCBYCR	=	YCBCR422_1P_YCBYCR,
	NV12	=	YCBCR420_2P_CRCB_LE,	// VP Little Endian Type
	NV16	=	YCBCR422_2P_CRCB_LE,
	NV24	=	YCBCR444_2P_CRCB_LE,
	YC420	=	YCBCR420_3P,
	YC422	=	YCBCR422_3P,
	YC444	=	YCBCR444_3P,

	NV12_IB		=	YCBCR420_2P_CRCB_LE_IB,	// Interlaced Bob Image
	NV12_IW	=	YCBCR420_2P_CRCB_LE_IW,	// Interlaced Weave Image
	TILE_NV12	=	TILE_YCBCR420_2P_CRCB_LE,
	TILE_NV12_IB	=	TILE_YCBCR420_2P_CRCB_LE_IB,
	TILE_NV12_IW	=	TILE_YCBCR420_2P_CRCB_LE_IW,
	TILE16_NV12	=	TILE16_YCBCR420_2P_CRCB_LE_IB
} IMG_FMT;

// Color Space Conversion Range
typedef enum
{
	ITU601_NARROW,
	ITU601_WIDE,
	ITU709_NARROW,
	ITU709_WIDE,

	NARROW = ITU601_NARROW, // Y:16~235, Cb/Cr:16~240 
	WIDE = ITU601_WIDE // Y/Cb/Cr:0~255 (default)	
}CSC_RANGE;

typedef enum
{
	LOCAL_IF_RGB24,
	LOCAL_IF_YCBCR444
} LOCAL_IF_TYPE;

typedef enum {
	OV7620=1, S5X433, AU70H, S5X3A1, S5K3AAE, S5K4AAF, S5K3C1F, S5K3BAF, S5K4CAGX, ADV7180, M5MO
} CIS_MODEL;


#ifdef __cplusplus
}
#endif

#endif

