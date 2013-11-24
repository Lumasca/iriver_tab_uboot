/**************************************************************************************
* 
*	Project Name : S5PV210 Validation
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
*	File Name : uart.h
*  
*	File Description : This file declares prototypes of UART API funcions.
*
*	Author : Woojin,Kim
*	Dept. : AP Development Team
*	Created Date : 2007/01/16
*	Version : 0.1 
* 
*	History
*	- Creat debug function (InitDebug,Putc,Getc,Getkey)(Haksoo,Kim 2006/11/08) 
*	- Creat Full function (Woojin,Kim 2007/01/16)
*  	- Modify function (Jun,Kim 2009/04/16)
**************************************************************************************/

#ifndef __UART_H__
#define __UART_H__


#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

#include "def.h"
#include "dma.h"

#define	UART_NO_ERROR		0
#define	UART_ERROR			(-1)

#define	UART_DEBUG		1

#if UART_DEBUG
#define	UART_ASSERT()	{						\
							int LoopCnt = 0;		\
							while(LoopCnt <10)	\
							{					\
								LoopCnt = 0;		\
							}					\
						}
#else
#define	UART_ASSERT()
#endif /* UART_DEBUG */

#define MAX_UART_CH				4
#define MAX_UART_DMA_CHANNEL		16
#define USE_DEBUGPORT_EXTCLK		0

#define	UART_BUFFER_SIZE			8		// for debugging

#define UART_OFFSET			0x400

#if 0
#define UART0_BASE			(UART_REGS *)(UART_BASE)
#define UART1_BASE			(UART_REGS *)(UART_BASE+UART_OFFSET)
#define UART2_BASE			(UART_REGS *)(UART_BASE+UART_OFFSET*2)
#define UART3_BASE			(UART_REGS *)(UART_BASE+UART_OFFSET*3)
#else
#define UART0_BASE			(UART_BASE)
#define UART1_BASE			(UART_BASE+UART_OFFSET)
#define UART2_BASE			(UART_BASE+UART_OFFSET*2)
#define UART3_BASE			(UART_BASE+UART_OFFSET*3)
#endif /* 0 */
//#define FIFO_DEBUG_BUF		(0x41202000)


#define SENDBREAK			(0x99)

#ifdef FPGA_MODE
#define UART_BUF			(0x21000000)
#else
#define UART_BUF			(0x41000000)
#endif /* FPGA_MODE */

#define UART_BUF_SIZE		(0x100000)		/* 1MB */

#define TX_FIFO_RESET		(1<<2)
#define RX_FIFO_RESET		(1<<1)
#define TX_INT_TYPE			(1<<9)			// 0:pulse 1:level(6400 should be level)
#define RX_INT_TYPE			(1<<8)			// 0:pulse 1:level(6400 should be level)

#define RX_TIMEOUT_EN		(1<<7)			// 0:disable 1:enable, disable for FIFO test
#define RX_ERR_INT_EN		(1<<6)			// 0:disable 1:enable
#define RTS_ACTIVE			(1)				// In normal mode, nRTS signal  0:low, 1:High

#define BIT_UART_MODEM		(1<<3)
#define BIT_UART_TXD		(1<<2)
#define BIT_UART_ERROR		(1<<1)
#define BIT_UART_RXD		(1)

#define UART_MAX(a,b)	( (a) > (b) ? (a) : (b) )
#define UART_MIN(a,b)	( (a) > (b) ? (b) : (a) )

typedef enum RTS
{
	RTS_inAct = 0,
	RTS_Act = 1
}RTS;

typedef enum
{
	UartCh_CH0 = 0,
	UartCh_CH1,
	UartCh_CH2,
	UartCh_CH3
}UartCh;


typedef enum
{
	UartParity_None = 0,
	UartParity_Odd = 4,
	UartParity_Even = 5,
	UartParity_Force1 = 6,
	UartParity_Force0 = 7
}UartParity;

typedef enum
{
	UartStop_1bitStop = 0,
	UartStop_2bitStop
}UartStop;

// Uart word length
typedef enum
{
	UartWL_5bit = 0,
	UartWL_6bit,
	UartWL_7bit,
	UartWL_8bit
}UartWL;

typedef enum
{
	UartClk_PCLK=0,
	UartClk_SCLK_UART = 1
}UartClk;


typedef enum
{
	UartMode_Disable = 0,
	UartMode_Interrupt,
	UartMode_DMA,
	UartMode_DMA_FlowCon	/* Spec out in S5PV210 */
}UartMode;

typedef enum
{
	UartFIFO_1B = 0,
	UartFIFO_4B,
	UartFIFO_8B,
	UartFIFO_16B,
	UartFIFO_32B,
	UartFIFO_64B,
	UartFIFO_128B,
	UartFIFO_256B,
}UartFIFO;

typedef enum
{
	UartFIFO_255_63_15B = 0,
	UartFIFO_224_56_14B,
	UartFIFO_192_48_12B,
	UartFIFO_160_40_10B,
	UartFIFO_128_32_8B,
	UartFIFO_96_24_6B,
	UartFIFO_64_16_4B,
	UartFIFO_32_8_2B,
}UartRTSFIFO;

typedef enum
{
	UartDMA_1B=0,
	UartDMA_4B,
	UartDMA_8B,
	UartDMA_16B,	/* Tx NOT Support, Rx Support because of PDMA in S5PV210. */
	UartDMA_32B,	/* NOT Support in S5PV210. */
	UartDMA_64B,	/* NOT Support in S5PV210. */
	UartDMA_128B,	/* NOT Support in S5PV210. */
	UartDMA_256B	/* NOT Support in S5PV210. */
}UartDMABurst;

typedef enum
{
	UartAFC_Disable=1,
	UartAFC_Enable
}UartAFC;

typedef enum
{
	UartRTSL_63B=1,
	UartRTSL_56B,
	UartRTSL_48B,
	UartRTSL_40B,
	UartRTSL_32B,
	UartRTSL_24B,
	UartRTSL_16B,
	UartRTSL_8B,
}UartRTSL;

typedef enum
{
	UART_XXTI = 0,
	UART_XUSBXTI,
	UART_SHDMI27M,
	UART_SUSBPHY0,
	UART_SUSBPHY1,
	UART_SHDMIPHY,
	UART_SMPLL,
	UART_SEPLL,
	UART_SVPLL
} UART_ClkSrc_et;

typedef enum
{
	UART_DMA0 = 0,
	UART_DMA1
} UART_DMA_et;

typedef enum
{
	UART_INT_TX,
	UART_INT_RX,
	UART_INT_DMA_TX,
	UART_INT_DMA_RX
} UART_IntSrc_et;

#define UART_FIFO_SIZE_CH0			256
#define UART_FIFO_SIZE_CH1			64
#define UART_FIFO_SIZE_CH2			16
#define UART_FIFO_SIZE_CH3			16

#define NormalMode					1
#define IrDaMode						2
#define NoParity						0
#define OddParity					2
#define EvenParity					3
#define ParityCheck1					4
#define ParityCheck0					5
#define OneStopBit					1
#define TwoStopBit					2
#define DataLength5bit				0
#define DataLength6bit				1
#define DataLength7bit				2
#define DataLength8bit				3
#define SendBreakSiganl				0x1
#define LoopTest						0x2
#define UARTPCLK					0x1
#define UARTEXTCLK					0x2
#define UARTEPLL					0x3
#define UARTPolORIRQMode			0x1
#define UARTDMAMode				0x2

#define UART_1st_DEBUG_CH			0
#define UART_2nd_DEBUG_CH			3

// rUFCONn
#define FIFO_Enable					(1 << 0)

// rUTRSTATn
#define Transmitter_empty			(1<<2)
#define Transmit_buffer_empty		(1<<1)
#define Receive_buffer_data_ready	(1<<0)

// rUFSTATn
#define Tx_FIFO_Full					(0x1 << 24)
#define Rx_FIFO_Full					(0x1 << 8)
#define FIFO_Count_Mask				0xFF

#define uart_rCLK_SRC4				0xE0100210
#define UART_EXTCLK_SRC			0x1



typedef struct tag_UART_REGS
{
	u32 rUlCon;
	u32 rUCon;
	u32 rUfCon;
	u32 rUmCon;
	u32 rUtrStat;
	u32 rUerStat;
	u32 rUfStat;
	u32 rUmStat;
	u32 rUtxh;
	u32 rUrxh;
	u32 rUbrDiv;
	u32 rUdivSlot;
	u32 rUintP;
	u32 rUintSp;
	u32 rUintM;
} UART_REGS;


typedef struct {
	u32		m_StartMemAddr;
	u32		m_TotalSize;
	u32		m_CurMemAddr;		/* I'll use the Non Cacheable Memory Area as Ring-Buffer. */
} UART_NCBufCtrlInfo_st;

typedef struct {
	u8 				*m_pBufPtr;			/* Start of buffer pointer */
	u8 				*m_pCurPtr;			/* Current buffer pointer */
	u32				m_BufSize;			/* Buffer size */	
	u32				m_ReqDataSize;		/* Requested data size */
	u32				m_RemainDataSize;	/* not treated, Remain data size */
} UART_BufAllocInfo_st;


typedef void (*UART_IntHandler_t)(void) __irq;
typedef void (*UART_Callback_t)(u32 Value2);

typedef struct tag_UART_CON
{
	UART_REGS			*m_pUartSFRBase;
	UartCh				m_UartChIdx;			/* UART0, UART1, UART2, UART3 */
	
	u32					m_Baudrate;
	bool					m_EnIRDAMode;			/* 0:Uart mode, 1:IRDA Mode */
	bool					m_EnLoopBackMode;		/* UCON[5]  (0 : normal, 1 : LoopBack) */
	bool					m_EnAFC;
	bool					m_EnFifo;
	UartDMABurst			m_TxDMABurstSize;
	UartDMABurst			m_RxDMABurstSize;
	UartWL				m_DataBit;				/* 0:5bit, 1:6bit, 2:7bit, 3:8bit */
	UartParity			m_ParityBit;				/* 0:no parity, 4:odd, 5:even, 6:forced 1, 7:forced 0 */
	UartStop				m_StopBit;				/* 0:one stopbit, 1:two stopbit */
	UartMode				m_TxMode;				/* 0:disable, 1:interrupt or polling, 2:DMA0, 3:DMA1 */
	UartMode				m_RxMode;				/* 0:disable, 1:interrupt or polling, 2:DMA, 3:DMA with AFC */
	UartFIFO				m_TxTrigLvl;			/* 0:1, 1:4, 2:8, 3:16, 4:32, 5:64, 6:128, 7:256 (byte) */
	UartFIFO				m_RxTrigLvl;			/* 0:1, 1:4, 2:8, 3:16, 4:32, 5:64, 6:128, 7:256 (byte) */
	UartRTSFIFO			m_RtsTrigLvl;
	//bool				m_EnSendBreak;		/* ULCON[4]  (0 : normal, 1 : Send Break Signal) */
	bool					m_EnRxTimeout;

	//bool				m_IsDebugChannel;
	bool					m_IsUseExtClk;			/* 0:PCLK, 1:EXTCLK */
	UART_ClkSrc_et		m_ExtClkSrc;			/* XXTI, XUSBXTI, ... */
	u32					m_ExtClkSrcDiv;			/* from 1 */
	u32 					m_OpClock;			/* for debugging */
	u32					m_ClkDivValue;			/* for debugging */
	u32					m_ClkSlot;				/* for debugging */

	u32					m_TxTransferSize;
	u32					m_RxTransferSize;

	UART_BufAllocInfo_st	m_RxBufNode;
	UART_BufAllocInfo_st	m_TxBufNode;

	bool					m_CheckTxEndbyNull;	/* If the end of tx string is NULL, stop Tx in interrupt. */
	bool					m_CheckRxEndbyCR;		/* If the end of rx string is CR, stop Rx in interrupt. */

	u32					m_MaxFIFOSize;
	u32					m_DrvStrength;
	
	u32					m_IntNum;
	bool					m_EnModemIntMask;
	bool					m_EnTxdIntMask;
	bool					m_EnErrorIntMask;
	bool					m_EnRxdIntMask;

	UART_Callback_t		m_UartRxIntCallback;
	u32					m_UartRxIntCallback_Value;

	UART_Callback_t		m_UartTxIntCallback;
	u32					m_UartTxIntCallback_Value;

	UART_DMA_et			m_UartRxDMANum;		/* UART_DMA0, UART_DMA1 */
	UART_DMA_et			m_UartTxDMANum;		/* UART_DMA0, UART_DMA1 */
	
	DMA_CH				m_UartRxDmaCh;
	DMA_CH				m_UartTxDmaCh;
	DREQ_SRC			m_UartRxDmaReqSrc;
	DREQ_SRC			m_UartTxDmaReqSrc;
	DMAC				m_UartRxDma;			/* RX DMA Request Descriptor */	
	DMAC				m_UartTxDma;			/* TX DMA Request Descriptor */

	UART_Callback_t		m_UartRxDMACallback;
	u32					m_UartRxDMACallback_Value;

	UART_Callback_t		m_UartTxDMACallback;
	u32					m_UartTxDMACallback_Value;
} UART_CON;


typedef struct 
{
	UART_CON			m_UartChDefaultInfo[MAX_UART_CH];	/* Default Value */
	UART_CON			m_UartChTestInfo[MAX_UART_CH];		/* Test Value */
	bool					m_IsDebugChActivated;
	UartCh				m_CurDebugCh;						/* Current Debug Channel */
	UART_NCBufCtrlInfo_st	m_NCBufCtrlInfo;					/* To control the Non-Cacheable memory Area */
	bool					m_EnUart2CtsRts;
	bool					m_EnPrintInISR;
} UART_CtrlInfo_st;


extern UART_CtrlInfo_st gUART_CtrlInfo;


void UART_InitDebugCh(u8 ch, u32 uBaudRate);
void UART_Putc(char data);
void UART_TxEmpty(void);
s8 UART_Getc(void);
s8 UART_GetKey(void);
//u8 UART_Config(void);
void UART_TxString(UART_CON *pUartCon, u8 *str, u32 TxDataSize);
u8* UART_RxString(UART_CON *pUartCon, u8 *pDstAddr, u32 RxDataSize);
void UART_OpenModem(UART_CON *pUartCon);
void UART_WrUtxh(UART_CON *pUartCon,u8 cTx);
u8 UART_RdUrxh(UART_CON *pUartCon);
void UART_RxEmpty(UART_CON *pUartCon);
u32 UART_RdUTRSTAT(UART_CON *pUartCon);
u32 UART_RdUFSTAT(UART_CON *pUartCon);
u32 UART_RdUMSTAT(UART_CON *pUartCon);
void UART_WrRTS(UART_CON *pUartCon, u32 en);
void UART_SendBreakSignal(UART_CON *pUartCon);
void UART_EnModemINT(UART_CON *pUartCon, u32 uEn);
void UART_SetGPIO(UART_CON *pUartCon);
void UART_SetExtClkSrc(UART_CON *pUartCon);
u8 UART_Open_Debug(UART_CON *pUartCon);
u8 UART_Open(UART_CON *pUartCon);
u8 UART_Close(UART_CON *pUartCon);
u32 UART_InitializeREG(UART_CON *pUartCon);
s32 UART_CalcBaudrate(bool IsUseExtClk, u32 Baudrate, UART_ClkSrc_et ExtClkSrc, 
	u32 ExtClkSrcDiv, u32 *pOpClock, u32 *pClockDivValue, u32 *pClockSlot);
s32 UART_SelectExtClkSrc(UART_CON *pUartCon);
void __irq Isr_Uart0(void);
void __irq Isr_Uart1(void);
void __irq Isr_Uart2(void);
void __irq Isr_Uart3(void);
void Isr_UartSub(u8 ch);
void __irq Isr_Dma0(void);
void __irq Isr_Dma1(void);
s32 UART_GetIntNum(void);
static s32 Str2Int(s8 *string);
void UART_PutString(const s8 *string);
void UART_GetString(s8 *pStr);
u32 CalcEPLL(u32 uMdiv,u32 uPdiv,u32 uSdiv,u32 uKdiv);

UartCh UART_GetCurDebugCh(void);

void UART_SetDefaultSystemEnv(void);
void UART_InitDefaultVariable(UART_CtrlInfo_st *pCtrlInfo);
void UART_Init(UART_CtrlInfo_st *pCtrlInfo);
void UART_InitChVariable(UartCh Ch, UART_CON *pUartCon);

void UART_SetIsrFunc(UART_CON *pUartCon, UART_IntSrc_et UartIntSrc, void (*fnIsrFunc)(u32), u32 IsrVar);
s32 UART_GetOpClock(bool IsUseExtClk, u32 Baudrate, UART_ClkSrc_et ExtClkSrc, 
	u32 ExtClkSrcDiv, u32 *pOpClock, u32 *pDivValue, u32 *pSlot);

s32 UART_SendBreakSignalTest(UART_CtrlInfo_st *pCtrlInfo);
bool UART_IsInterruptPrintValid(void);

	
void UART_CreateNCBufCtrl(UART_NCBufCtrlInfo_st *pNCBufCtrlInfo, u32 StartAddr, u32 TotalSize);
void UART_DeleteNCBufCtrl(UART_NCBufCtrlInfo_st *pNCBufCtrlInfo);
void UART_ResetNCBufCtrl(UART_NCBufCtrlInfo_st *pNCBufCtrlInfo);

void UART_PollingTxString(UART_CON *pUart);
void UART_PollingRxString(UART_CON *pUart);
void UART_LoopbackPollingTxRxString(UART_CON *pUartTx, UART_CON *pUartRx);
void UART_LoopbackInterruptTxRxString(UART_CON *pUartCon, 
	void (*fnTxComplete)(u32), u32 IsrTxVar, void (*fnRxComplete)(u32), u32 IsrRxVar);
void UART_LoopbackDMATxRxString(UART_CON *pUartCon, 
	void (*fnTxComplete)(u32), u32 IsrTxVar, void (*fnRxComplete)(u32), u32 IsrRxVar);
void UART_InterruptTxString(UART_CON *pUartCon, void (*fnTxComplete)(u32), u32 IsrVar);
u8* UART_InterruptRxString(UART_CON *pUartCon, void (*fnRxComplete)(u32), u32 IsrVar);
void UART_DMATxString(UART_CON *pUartCon, void (*fnTxComplete)(u32), u32 IsrVar);
u8* UART_DMARxString(UART_CON *pUartCon, void (*fnRxComplete)(u32), u32 IsrVar);

void UART_PollingTxWithManualCTSString(UART_CON *pUart);
void UART_PollingRxWithManualRTSString(UART_CON *pUart);


u8 *UART_GetNCBuf(UART_BufAllocInfo_st *pBufAllocInfo, u32 BufSize);
int UART_FreeNCBuf(UART_BufAllocInfo_st *pBufAllocInfo);
int UART_IncNCValidDataSize(UART_BufAllocInfo_st *pBufAllocInfo, u32 Size);
int UART_NCBufPatternInit(UART_BufAllocInfo_st *pBufAllocInfo, u32 BufSize, const char *pPattern, int PatternSize);
int UART_NCBufRandInit(UART_BufAllocInfo_st *pBufAllocInfo, u32 BufSize);
int UART_NCBufZeroInit(UART_BufAllocInfo_st *pBufAllocInfo);
void UART_NCBufDisplay(UART_BufAllocInfo_st *pBufAllocInfo, bool EnDisplay);
void UART_NCBufDisplayHex(UART_BufAllocInfo_st *pBufAllocInfo, bool EnDisplay);

void UART_DisplayVar_InitNCBuf(UART_CON *pUartCon);
void UART_DisplayVar_ChangeDebugChannel(UART_CON *pUartCon);
void UART_DisplayVar_EnUart2CtsRts(UART_CON *pUartCon);
void UART_DisplayVar_EnInterruptPrint(UART_CON *pUartCon);
void UART_DisplayVar_LoadDefault(UART_CON *pUartCon);
void UART_DisplayVar_SWReset(UART_CON *pUartCon);
void UART_DisplayVar_UartOpen(UART_CON *pUartCon);
//void UART_DisplayVar_ReadRegister(UART_CON *pUartCon);
void UART_DisplayVar_ReadWriteRegister(UART_CON *pUartCon);
void UART_DisplayVar_Baudrate(UART_CON *pUartCon);
void UART_DisplayVar_IrDAMode(UART_CON *pUartCon);
void UART_DisplayVar_InteranlLoopbackMode(UART_CON *pUartCon);
void UART_DisplayVar_AFCMode(UART_CON *pUartCon);
void UART_DisplayVar_FIFOMode(UART_CON *pUartCon);
void UART_DisplayVar_TxDMABurstSize(UART_CON *pUartCon);
void UART_DisplayVar_RxDMABurstSize(UART_CON *pUartCon);
void UART_DisplayVar_DataBitSize(UART_CON *pUartCon);
void UART_DisplayVar_ParityBitSize(UART_CON *pUartCon);
void UART_DisplayVar_StopBitSize(UART_CON *pUartCon);
void UART_DisplayVar_TxMode(UART_CON *pUartCon);
void UART_DisplayVar_RxMode(UART_CON *pUartCon);
void UART_DisplayVar_TxTrigLevel(UART_CON *pUartCon);
void UART_DisplayVar_RxTrigLevel(UART_CON *pUartCon);
void UART_DisplayVar_DrvStrength(UART_CON *pUartCon);
void UART_DisplayVar_EnModemIntMask(UART_CON *pUartCon);
void UART_DisplayVar_EnTxdIntMask(UART_CON *pUartCon);
void UART_DisplayVar_EnErrorIntMask(UART_CON *pUartCon);
void UART_DisplayVar_EnRxdIntMask(UART_CON *pUartCon);
void UART_DisplayVar_RtsTrigLevel(UART_CON *pUartCon);
void UART_DisplayVar_RxTimeoutMode(UART_CON *pUartCon);
void UART_DisplayVar_TxTransferSize(UART_CON *pUartCon);
void UART_DisplayVar_RxTransferSize(UART_CON *pUartCon);
void UART_DisplayVar_DebugChannel(UART_CON *pUartCon);
void UART_DisplayVar_ExtClkMode(UART_CON *pUartCon);
void UART_DisplayVar_ExtClkSource(UART_CON *pUartCon);
void UART_DisplayVar_ExtClkSourceDiv(UART_CON *pUartCon);
void UART_DisplayVar_CurClkFrequency(UART_CON *pUartCon);
void UART_DisplayVar_CheckTxEndByNull(UART_CON *pUartCon);
void UART_DisplayVar_CheckRxEndByCR(UART_CON *pUartCon);
void UART_DisplayVar_TxDMANum(UART_CON *pUartCon);
void UART_DisplayVar_RxDMANum(UART_CON *pUartCon);
void UART_DisplayVar_TxDMACh(UART_CON *pUartCon);
void UART_DisplayVar_RxDMACh(UART_CON *pUartCon);

void UART_SetVar_InitNCBuf(UART_CON *pUartCon);
void UART_SetVar_ChangeDebugChannel(UART_CON *pUartCon);
void UART_SetVar_EnUart2CtsRts(UART_CON *pUartCon);
void UART_SetVar_EnInterruptPrint(UART_CON *pUartCon);		/* '\n' included */
void UART_SetVar_LoadDefault(UART_CON *pUartCon);
void UART_SetVar_SWReset(UART_CON *pUartCon);
void UART_SetVar_UartOpen(UART_CON *pUartCon);
//void UART_SetVar_ReadRegister(UART_CON *pUartCon);
void UART_SetVar_ReadWriteRegister(UART_CON *pUartCon);
void UART_SetVar_Baudrate(UART_CON *pUartCon);
void UART_SetVar_IrDAMode(UART_CON *pUartCon);
void UART_SetVar_InteranlLoopbackMode(UART_CON *pUartCon);
void UART_SetVar_AFCMode(UART_CON *pUartCon);
void UART_SetVar_FIFOMode(UART_CON *pUartCon);
void UART_SetVar_TxDMABurstSize(UART_CON *pUartCon);
void UART_SetVar_RxDMABurstSize(UART_CON *pUartCon);
void UART_SetVar_DataBitSize(UART_CON *pUartCon);
void UART_SetVar_ParityBitSize(UART_CON *pUartCon);
void UART_SetVar_StopBitSize(UART_CON *pUartCon);
void UART_SetVar_TxMode(UART_CON *pUartCon);
void UART_SetVar_RxMode(UART_CON *pUartCon);
void UART_SetVar_TxTrigLevel(UART_CON *pUartCon);
void UART_SetVar_RxTrigLevel(UART_CON *pUartCon);
void UART_SetVar_RtsTrigLevel(UART_CON *pUartCon);
void UART_SetVar_DrvStrength(UART_CON *pUartCon);
void UART_SetVar_EnModemIntMask(UART_CON *pUartCon);
void UART_SetVar_EnTxdIntMask(UART_CON *pUartCon);
void UART_SetVar_EnErrorIntMask(UART_CON *pUartCon);
void UART_SetVar_EnRxdIntMask(UART_CON *pUartCon);
void UART_SetVar_RxTimeoutMode(UART_CON *pUartCon);
void UART_SetVar_TxTransferSize(UART_CON *pUartCon);
void UART_SetVar_RxTransferSize(UART_CON *pUartCon);
void UART_SetVar_ExtClkMode(UART_CON *pUartCon);
void UART_SetVar_ExtClkSource(UART_CON *pUartCon);
void UART_SetVar_ExtClkSourceDiv(UART_CON *pUartCon);
void UART_SetVar_CheckTxEndByNull(UART_CON *pUartCon);
void UART_SetVar_CheckRxEndByCR(UART_CON *pUartCon);
void UART_SetVar_RxDMACh(UART_CON *pUartCon);
void UART_SetVar_TxDMACh(UART_CON *pUartCon);

void Test_UART(void);

s32 UART_SetDefaultInfo_Uart0(UART_CtrlInfo_st *pCtrlInfo);
s32 UART_SetDefaultInfo_Uart1(UART_CtrlInfo_st *pCtrlInfo);
s32 UART_SetDefaultInfo_Uart2(UART_CtrlInfo_st *pCtrlInfo);
s32 UART_SetDefaultInfo_Uart3(UART_CtrlInfo_st *pCtrlInfo);

s32 UART_PollingTxTest(UART_CtrlInfo_st *pCtrlInfo);
s32 UART_InterruptTxTest(UART_CtrlInfo_st *pCtrlInfo);
s32 UART_DMATxTest(UART_CtrlInfo_st *pCtrlInfo);
s32 UART_PollingRxTest(UART_CtrlInfo_st *pCtrlInfo);
s32 UART_InterruptRxTest(UART_CtrlInfo_st *pCtrlInfo);
s32 UART_DMARxTest(UART_CtrlInfo_st *pCtrlInfo);
s32 UART_IntLoopback_Polling_Test(UART_CtrlInfo_st *pCtrlInfo);
s32 UART_IntLoopback_Interrupt_Test(UART_CtrlInfo_st *pCtrlInfo);
s32 UART_IntLoopback_DMA_Test(UART_CtrlInfo_st *pCtrlInfo);
s32 UART_ExtLoopback_Polling_Test(UART_CtrlInfo_st *pCtrlInfo);
s32 UART_ExtLoopback_Interrupt_Test(UART_CtrlInfo_st *pCtrlInfo);
s32 UART_ExtLoopback_DMA_Test(UART_CtrlInfo_st *pCtrlInfo);
s32 UART_IntChLoopback_Polling_Test(UART_CtrlInfo_st *pCtrlInfo);
s32 UART_IntChLoopback_Interrupt_Test(UART_CtrlInfo_st *pCtrlInfo);
s32 UART_IntChLoopback_DMA_Test(UART_CtrlInfo_st *pCtrlInfo);
s32 UART_PollingTxWithManualCTSTest(UART_CtrlInfo_st *pCtrlInfo);
s32 UART_PollingRxWithManualRTSTest(UART_CtrlInfo_st *pCtrlInfo);
s32 UART_ModemInterruptTest(UART_CtrlInfo_st *pCtrlInfo);
s32 UART_IrDATxTest(UART_CtrlInfo_st *pCtrlInfo);
s32 UART_IrDARxTest(UART_CtrlInfo_st *pCtrlInfo);

void UART_TxCompleteCallback(u32 var0);
void UART_RxCompleteCallback(u32 var0);


s32 UART_Uart0PollingTxTest(UART_CtrlInfo_st *pCtrlInfo);
s32 UART_Uart1PollingTxTest(UART_CtrlInfo_st *pCtrlInfo);
s32 UART_Uart2PollingTxTest(UART_CtrlInfo_st *pCtrlInfo);
s32 UART_Uart3PollingTxTest(UART_CtrlInfo_st *pCtrlInfo);
s32 UART_Uart0InterruptTxTest(UART_CtrlInfo_st *pCtrlInfo);
s32 UART_Uart1InterruptTxTest(UART_CtrlInfo_st *pCtrlInfo);
s32 UART_Uart2InterruptTxTest(UART_CtrlInfo_st *pCtrlInfo);
s32 UART_Uart3InterruptTxTest(UART_CtrlInfo_st *pCtrlInfo);
s32 UART_Uart0DMATxTest(UART_CtrlInfo_st *pCtrlInfo);
s32 UART_Uart1DMATxTest(UART_CtrlInfo_st *pCtrlInfo);
s32 UART_Uart2DMATxTest(UART_CtrlInfo_st *pCtrlInfo);
s32 UART_Uart3DMATxTest(UART_CtrlInfo_st *pCtrlInfo);

s32 UART_Uart0PollingRxTest(UART_CtrlInfo_st *pCtrlInfo);
s32 UART_Uart1PollingRxTest(UART_CtrlInfo_st *pCtrlInfo);
s32 UART_Uart2PollingRxTest(UART_CtrlInfo_st *pCtrlInfo);
s32 UART_Uart3PollingRxTest(UART_CtrlInfo_st *pCtrlInfo);
s32 UART_Uart0InterruptRxTest(UART_CtrlInfo_st *pCtrlInfo);
s32 UART_Uart1InterruptRxTest(UART_CtrlInfo_st *pCtrlInfo);
s32 UART_Uart2InterruptRxTest(UART_CtrlInfo_st *pCtrlInfo);
s32 UART_Uart3InterruptRxTest(UART_CtrlInfo_st *pCtrlInfo);
s32 UART_Uart0DMARxTest(UART_CtrlInfo_st *pCtrlInfo);
s32 UART_Uart1DMARxTest(UART_CtrlInfo_st *pCtrlInfo);
s32 UART_Uart2DMARxTest(UART_CtrlInfo_st *pCtrlInfo);
s32 UART_Uart3DMARxTest(UART_CtrlInfo_st *pCtrlInfo);

s32 UART_IntLoopback_Uart0PollingTest(UART_CtrlInfo_st *pCtrlInfo);
s32 UART_IntLoopback_Uart1PollingTest(UART_CtrlInfo_st *pCtrlInfo);
s32 UART_IntLoopback_Uart2PollingTest(UART_CtrlInfo_st *pCtrlInfo);
s32 UART_IntLoopback_Uart3PollingTest(UART_CtrlInfo_st *pCtrlInfo);
s32 UART_IntLoopback_Uart0InterruptTest(UART_CtrlInfo_st *pCtrlInfo);
s32 UART_IntLoopback_Uart1InterruptTest(UART_CtrlInfo_st *pCtrlInfo);
s32 UART_IntLoopback_Uart2InterruptTest(UART_CtrlInfo_st *pCtrlInfo);
s32 UART_IntLoopback_Uart3InterruptTest(UART_CtrlInfo_st *pCtrlInfo);
s32 UART_IntLoopback_Uart0DMATest(UART_CtrlInfo_st *pCtrlInfo);
s32 UART_IntLoopback_Uart1DMATest(UART_CtrlInfo_st *pCtrlInfo);
s32 UART_IntLoopback_Uart2DMATest(UART_CtrlInfo_st *pCtrlInfo);
s32 UART_IntLoopback_Uart3DMATest(UART_CtrlInfo_st *pCtrlInfo);

s32 UART_ExtLoopback_Uart0PollingTest(UART_CtrlInfo_st *pCtrlInfo);
s32 UART_ExtLoopback_Uart1PollingTest(UART_CtrlInfo_st *pCtrlInfo);
s32 UART_ExtLoopback_Uart2PollingTest(UART_CtrlInfo_st *pCtrlInfo);
s32 UART_ExtLoopback_Uart3PollingTest(UART_CtrlInfo_st *pCtrlInfo);
s32 UART_ExtLoopback_Uart0InterruptTest(UART_CtrlInfo_st *pCtrlInfo);
s32 UART_ExtLoopback_Uart1InterruptTest(UART_CtrlInfo_st *pCtrlInfo);
s32 UART_ExtLoopback_Uart2InterruptTest(UART_CtrlInfo_st *pCtrlInfo);
s32 UART_ExtLoopback_Uart3InterruptTest(UART_CtrlInfo_st *pCtrlInfo);
s32 UART_ExtLoopback_Uart0DMATest(UART_CtrlInfo_st *pCtrlInfo);
s32 UART_ExtLoopback_Uart1DMATest(UART_CtrlInfo_st *pCtrlInfo);
s32 UART_ExtLoopback_Uart2DMATest(UART_CtrlInfo_st *pCtrlInfo);
s32 UART_ExtLoopback_Uart3DMATest(UART_CtrlInfo_st *pCtrlInfo);

s32 UART_IntChLoopback_Uart0Uart1PollingTest(UART_CtrlInfo_st *pCtrlInfo);
s32 UART_IntChLoopback_Uart0Uart2PollingTest(UART_CtrlInfo_st *pCtrlInfo);
s32 UART_IntChLoopback_Uart0Uart3PollingTest(UART_CtrlInfo_st *pCtrlInfo);
s32 UART_IntChLoopback_Uart1Uart0PollingTest(UART_CtrlInfo_st *pCtrlInfo);
s32 UART_IntChLoopback_Uart1Uart2PollingTest(UART_CtrlInfo_st *pCtrlInfo);
s32 UART_IntChLoopback_Uart1Uart3PollingTest(UART_CtrlInfo_st *pCtrlInfo);
s32 UART_IntChLoopback_Uart2Uart0PollingTest(UART_CtrlInfo_st *pCtrlInfo);
s32 UART_IntChLoopback_Uart2Uart1PollingTest(UART_CtrlInfo_st *pCtrlInfo);
s32 UART_IntChLoopback_Uart2Uart3PollingTest(UART_CtrlInfo_st *pCtrlInfo);
s32 UART_IntChLoopback_Uart3Uart0PollingTest(UART_CtrlInfo_st *pCtrlInfo);
s32 UART_IntChLoopback_Uart3Uart1PollingTest(UART_CtrlInfo_st *pCtrlInfo);
s32 UART_IntChLoopback_Uart3Uart2PollingTest(UART_CtrlInfo_st *pCtrlInfo);

s32 UART_IntChLoopback_Uart0Uart1InterruptTest(UART_CtrlInfo_st *pCtrlInfo);
s32 UART_IntChLoopback_Uart0Uart2InterruptTest(UART_CtrlInfo_st *pCtrlInfo);
s32 UART_IntChLoopback_Uart0Uart3InterruptTest(UART_CtrlInfo_st *pCtrlInfo);
s32 UART_IntChLoopback_Uart1Uart0InterruptTest(UART_CtrlInfo_st *pCtrlInfo);
s32 UART_IntChLoopback_Uart1Uart2InterruptTest(UART_CtrlInfo_st *pCtrlInfo);
s32 UART_IntChLoopback_Uart1Uart3InterruptTest(UART_CtrlInfo_st *pCtrlInfo);
s32 UART_IntChLoopback_Uart2Uart0InterruptTest(UART_CtrlInfo_st *pCtrlInfo);
s32 UART_IntChLoopback_Uart2Uart1InterruptTest(UART_CtrlInfo_st *pCtrlInfo);
s32 UART_IntChLoopback_Uart2Uart3InterruptTest(UART_CtrlInfo_st *pCtrlInfo);
s32 UART_IntChLoopback_Uart3Uart0InterruptTest(UART_CtrlInfo_st *pCtrlInfo);
s32 UART_IntChLoopback_Uart3Uart1InterruptTest(UART_CtrlInfo_st *pCtrlInfo);
s32 UART_IntChLoopback_Uart3Uart2InterruptTest(UART_CtrlInfo_st *pCtrlInfo);

s32 UART_IntChLoopback_Uart0Uart1DMATest(UART_CtrlInfo_st *pCtrlInfo);
s32 UART_IntChLoopback_Uart0Uart2DMATest(UART_CtrlInfo_st *pCtrlInfo);
s32 UART_IntChLoopback_Uart0Uart3DMATest(UART_CtrlInfo_st *pCtrlInfo);
s32 UART_IntChLoopback_Uart1Uart0DMATest(UART_CtrlInfo_st *pCtrlInfo);
s32 UART_IntChLoopback_Uart1Uart2DMATest(UART_CtrlInfo_st *pCtrlInfo);
s32 UART_IntChLoopback_Uart1Uart3DMATest(UART_CtrlInfo_st *pCtrlInfo);
s32 UART_IntChLoopback_Uart2Uart0DMATest(UART_CtrlInfo_st *pCtrlInfo);
s32 UART_IntChLoopback_Uart2Uart1DMATest(UART_CtrlInfo_st *pCtrlInfo);
s32 UART_IntChLoopback_Uart2Uart3DMATest(UART_CtrlInfo_st *pCtrlInfo);
s32 UART_IntChLoopback_Uart3Uart0DMATest(UART_CtrlInfo_st *pCtrlInfo);
s32 UART_IntChLoopback_Uart3Uart1DMATest(UART_CtrlInfo_st *pCtrlInfo);
s32 UART_IntChLoopback_Uart3Uart2DMATest(UART_CtrlInfo_st *pCtrlInfo);

s32 UART_Uart0PollingTxWithManualCTSTest(UART_CtrlInfo_st *pCtrlInfo);
s32 UART_Uart1PollingTxWithManualCTSTest(UART_CtrlInfo_st *pCtrlInfo);
s32 UART_Uart2PollingTxWithManualCTSTest(UART_CtrlInfo_st *pCtrlInfo);
s32 UART_Uart3PollingTxWithManualCTSTest(UART_CtrlInfo_st *pCtrlInfo);

s32 UART_Uart0PollingRxWithManualRTSTest(UART_CtrlInfo_st *pCtrlInfo);
s32 UART_Uart1PollingRxWithManualRTSTest(UART_CtrlInfo_st *pCtrlInfo);
s32 UART_Uart2PollingRxWithManualRTSTest(UART_CtrlInfo_st *pCtrlInfo);
s32 UART_Uart3PollingRxWithManualRTSTest(UART_CtrlInfo_st *pCtrlInfo);

s32 UART_Uart0ModemInterruptTest(UART_CtrlInfo_st *pCtrlInfo);
s32 UART_Uart1ModemInterruptTest(UART_CtrlInfo_st *pCtrlInfo);
s32 UART_Uart2ModemInterruptTest(UART_CtrlInfo_st *pCtrlInfo);
s32 UART_Uart3ModemInterruptTest(UART_CtrlInfo_st *pCtrlInfo);

s32 UART_Uart0SendBreakSignalTest(UART_CtrlInfo_st *pCtrlInfo);
s32 UART_Uart1SendBreakSignalTest(UART_CtrlInfo_st *pCtrlInfo);
s32 UART_Uart2SendBreakSignalTest(UART_CtrlInfo_st *pCtrlInfo);
s32 UART_Uart3SendBreakSignalTest(UART_CtrlInfo_st *pCtrlInfo);

s32 UART_Uart0IrDATxTest(UART_CtrlInfo_st *pCtrlInfo);
s32 UART_Uart1IrDATxTest(UART_CtrlInfo_st *pCtrlInfo);
s32 UART_Uart2IrDATxTest(UART_CtrlInfo_st *pCtrlInfo);
s32 UART_Uart3IrDATxTest(UART_CtrlInfo_st *pCtrlInfo);

s32 UART_Uart0IrDARxTest(UART_CtrlInfo_st *pCtrlInfo);
s32 UART_Uart1IrDARxTest(UART_CtrlInfo_st *pCtrlInfo);
s32 UART_Uart2IrDARxTest(UART_CtrlInfo_st *pCtrlInfo);
s32 UART_Uart3IrDARxTest(UART_CtrlInfo_st *pCtrlInfo);

void UART_SFR_test(UART_CtrlInfo_st *pCtrlInfo);
void UART_Console_AutoTest(UART_CtrlInfo_st *pCtrlInfo);
void UART_SetPLL(UART_CtrlInfo_st *pCtrlInfo);



#ifdef __cplusplus
}
#endif

#endif //__UART_H__

