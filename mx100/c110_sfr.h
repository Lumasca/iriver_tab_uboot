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
*	File Name : c110_sfr.h
*  
*	File Description : This file defines SFR base addresses.
*
*	Author : GOM
*	Dept. : AP Development Team
*	Created Date : 2009/02/05
*	Version : 0.0 
* 
*	History
*	- Created(GOM 2009/02/05)
*  
**************************************************************************************/


#ifndef __C110_SFR_H__
#define __C110_SFR_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "smdkc110_def.h"


//// System
#define CHIPID_BASE				(0xE0000000)
#define SYSCON_BASE			(0xE0100000)
#define GPIO_BASE				(0xE0200000)
#define IEM_IEC_BASE			(0xE0800000)
#define IEM_APC_BASE			(0xE0700000)


//// Bus Architecture

#define ASYNC_AXI_APBIF_MFC_VSYS0_BASE			(0xF1800000)
#define ASYNC_AXI_APBIF_MFC_VSYS1_BASE			(0xF1900000)
#define ASYNC_AXI_APBIF_DSYS_MSYS0_BASE			(0xF1A00000)
#define ASYNC_AXI_APBIF_DSYS_MSYS1_BASE			(0xF1B00000)
#define ASYNC_AXI_APBIF_MSFR_DSFR_BASE			(0xF1C00000)
#define ASYNC_AXI_APBIF_MSFR_PSFR_BASE			(0xF1D00000)
#define ASYNC_AXI_APBIF_MSYS_DMC0_BASE			(0xF1E00000)
#define ASYNC_AXI_APBIF_MSFR_MPERI_BASE			(0xF1F00000)

#define TZPC0_BASE				(0xF1500000)
#define TZPC1_BASE				(0xFAD00000)
#define TZPC2_BASE				(0xE0600000)
#define TZPC3_BASE				(0xE1C00000)

/*
#define APB_D1_BASE				(0xE2000000)
#define APB_B1_BASE				(0xE2100000)
#define APB_C1_BASE				(0xE2200000)
#define APB_LCD_BASE			(0xE2300000)
#define APB_TV_BASE				(0xE2400000)

#define APB_ARM_BASE			(0xE2600000)

#define APB_D0_BASE				(0xE3000000)
#define APB_B0_BASE				(0xE3100000)
#define APB_C0_BASE				(0xE3200000)
#define APB_MEM_BASE			(0xE3300000)
#define APB_ASYNCBR_BASE		(0xE3400000)
*/

//// Interrupt
#define VIC0_BASE				(0xF2000000)
#define VIC1_BASE				(0xF2100000)
#define VIC2_BASE				(0xF2200000)
#define VIC3_BASE				(0xF2300000)

#define TZIC0_BASE				(0xF2800000)
#define TZIC1_BASE				(0xF2900000)
#define TZIC2_BASE				(0xF2A00000)
#define TZIC3_BASE				(0xF2B00000)

//// Memory
#define DMC0_BASE				(0xF0000000)
#define DMC1_BASE				(0xF1400000)
#define SROM_BASE				(0xE8000000)
#define OND_BASE  				(0xB0000000)
#define ONDXL_BASE				(0xB0000000)
#define NFCON_BASE				(0xB0E00000)
#define CFCON_BASE				(0xE8200000)

//// DMA
#define MDMA_BASE				(0xFA300000)
#define SMDMA_BASE				(0xFA200000)
#define PDMA0_BASE				(0xE0900000)
#define PDMA1_BASE				(0xE0A00000)
#define SPDMA0_BASE			(0xE0B00000)
#define SPDMA1_BASE			(0xE0C00000)

//// Timer
#define PWMTIMER_BASE			(0xE2500000)
#define SYSTIMER_BASE			(0xE2600000)
#define WDT_BASE				(0xE2700000)
#define RTC_BASE				(0xE2800000)


//// Connectivity & Storage
#define UART_BASE				(0xE2900000)
#define I2C0_BASE				(0xE1800000) // general IIC
#define I2C2_BASE				(0xE1A00000) // PMIC IIC
#define SPI0_BASE				(0xE1300000)
#define SPI1_BASE				(0xE1400000)
#define SPI2_BASE				(0xE1500000)

#define I2C_HDMI_PHY_BASE		(0xFA900000)
#define I2C_HDMI_DDC_BASE		(0xFAB00000)

#define MIPI_DSI_BASE			(0xFA500000)
#define MIPI_CSI_BASE			(0xFA600000)
#define USBOTG_LINK_BASE		(0xEC000000)
#define USBOTG_PHY_BASE		(0xEC100000)
#define USBHOST_EHCI_BASE		(0xEC200000)
#define USBHOST_OHCI_BASE		(0xEC300000)
#define MODEMIF_BASE			(0xED000000)

#define HSMMC0_BASE			(0xEB000000)
#define HSMMC1_BASE			(0xEB100000)
#define HSMMC2_BASE			(0xEB200000)
#define HSMMC3_BASE			(0xEB300000)

//// Multimedia
#define LCDC_BASE				(0xF8000000)
#define MIE_BASE				(0xF8100000)
#define FIMC_A_BASE				(0xFB200000)
#define FIMC_B_BASE				(0xFB300000)
#define FIMC_C_BASE				(0xFB400000)
#define JPEG_BASE				(0xFB600000)
#define ROTATOR_BASE			(0xFB100000)
#define G3D_BASE				(0xF3000000)
#define TVENC_BASE				(0xF9000000)
#define VP_BASE					(0xF9100000)
#define MIXER_BASE				(0xF9200000)
#define HDMI_BASE				(0xFA100000)
#define MFC_BASE				(0xF1700000)
//#define MDNIE_AHB_BASE			(0xF8200000)
//#define MDNIE_APB_BASE			(0xFAE00000)
#define IELCDC_BASE				(0xF8200000)
#define MDNIE_BASE				(0xFAE00000)
#define IPC_BASE					(0xFB700000)// Interleave progressive converter

//// Audio & ETC
#define I2S0_BASE				(0xEEE30000)  //for v5.1 ch
#define I2S1_BASE				(0xE2100000)
#define I2S2_BASE				(0xE2A00000)
#define PCM0_BASE				(0xE2300000)
#define PCM1_BASE				(0xE1200000)
#define PCM2_BASE				(0xE2B00000)
#define AC97_BASE				(0xE2200000)
#define SPDIF_BASE				(0xE1100000)
#define ADCTS_BASE				(0xE1700000)
#define KEYPADIF_BASE			(0xE1600000)

#define	SSS_REG_BASE			(0xEA000000)

//// Security
/*
#define RX_SFR_BASE				(0xF4000000)	// AHB_RX
#define TX_SFR_BASE				(0xF4100000)	// AHB_TX
#define DnI_BASE					(0xF4000000)
#define AES_RX_BASE				(0xF4040000)
#define AES_TX_BASE				(0xF4140000)
#define DES_RX_BASE				(0xF4050000)
#define DES_TX_BASE				(0xF4150000)
#define HASH_RX_BASE			(0xF4060000)
#define HASH_TX_BASE			(0xF4160000)
#define RX_FIFO_BASE				(0xF4200000)	// DMA_RX
#define TX_FIFO_BASE				(0xF4300000)	// DMA_TX
#define PKA_BASE					(0xF4071000)
#define SECJTAG_BASE				(0xF4080000)

*/
#define SDM_BASE				(0xF1600000)	// Security domain manager
#define SECKEY_BASE				(0xE0E00000)	// security key

//// Debug
#define CORESIGHT_BASE			(0xE0D00000)

#ifdef __cplusplus
}
#endif

#endif /*__C110_SFR_H__*/

