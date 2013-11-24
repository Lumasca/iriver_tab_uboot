

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
*	File Name : uart.c
*  
*	File Description : This file implements the API functon for UART.
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

//#include <stdio.h>
//#include <string.h>
//#include <ctype.h>

#include "option.h"
#include "library.h"
#include "v210_sfr.h"
#include "system.h"
#include "uart.h"
#include "sysc.h"
#include "gpio.h"
#include "intc.h"
#include "dma.h"


UART_CtrlInfo_st		gUART_CtrlInfo;

static UART_CON	*gUART_pUartIsrCon[MAX_UART_CH];
static UART_CON	*gUART_pDMAIsrCon[MAX_UART_DMA_CHANNEL];

static UART_REGS *gUART_pDebugRegs;


//////////
// Function Name : UART_InitDebugCh
// Function Description : This function initializes a certain uart ch. for debugging console
// Input : NONE 
// Output : NONE
// Version : 
void UART_InitDebugCh(u8 ch, u32 uBaudRate)
{
	UART_CON			*pUartConDefault;
	UART_CON			*pUartConTest;
	u32					OpMode;

	OpMode = SYSC_GetOperatingMode();

	/* Currently not Used, but initialize it for test next time. */
	pUartConDefault = &gUART_CtrlInfo.m_UartChDefaultInfo[ch];
	UART_InitChVariable(ch, pUartConDefault);

	pUartConTest = &gUART_CtrlInfo.m_UartChTestInfo[ch];
	*pUartConTest = *pUartConDefault;
	
	//pUartConTest->m_IsDebugChannel = TRUE;
	//pUartConTest->m_EnLoopBackMode = FALSE;
	pUartConTest->m_EnAFC = FALSE;
	
	pUartConTest->m_Baudrate = uBaudRate;
	#if USE_DEBUGPORT_EXTCLK
	pUartConTest->m_IsUseExtClk = TRUE;
	#else
	pUartConTest->m_IsUseExtClk = FALSE;
	#endif /* USE_DEBUGPORT_EXTCLK */

	if(OpMode & 0x01)
	{
		pUartConTest->m_ExtClkSrc = UART_XUSBXTI;
	}
	else
	{
		pUartConTest->m_ExtClkSrc = UART_XXTI;
	}
	
	UART_Open_Debug(pUartConTest);

	/* 2009/07/07 : Park,HeeSang */
	gUART_CtrlInfo.m_IsDebugChActivated = TRUE;
	gUART_CtrlInfo.m_CurDebugCh = ch;
}

UartCh UART_GetCurDebugCh(void)
{
	if(gUART_CtrlInfo.m_IsDebugChActivated == TRUE)
	{
		return gUART_CtrlInfo.m_CurDebugCh;
	}
	else
	{
		//UART_ASSERT();
		
		return MAX_UART_CH;		/* for JATG Terminal. */
	}
}


//////////
// Function Name : UART_Putc
// Function Description : This function write character data to uart debugging ch
// Input : NONE 
// Output : NONE
// Version : 
void UART_Putc(char data)
{
	u32 temp;

	if(data=='\n')
	{
		while(1)
		{
			temp = Inp32(&gUART_pDebugRegs->rUtrStat);
			temp&=0x2;
			if(temp)
				break;
		}
//		Delay(10);
		Outp8(&gUART_pDebugRegs->rUtxh,'\r');
	}
	while(1)
	{
		temp = Inp32(&gUART_pDebugRegs->rUtrStat);
		temp&=0x02;
		if(temp)
		{
			break;
		}
	}
//	Delay(10);
	Outp8(&gUART_pDebugRegs->rUtxh,data);
	
	return;
}


//////////
// Function Name : UART_TxEmpty
// Function Description : This function Hold Uart Tx until FIFO empty
// Input : NONE 
// Output : NONE
// Version : 
void UART_TxEmpty(void)
{

	u32 temp32;

	while(1)
	{
		temp32 = Inp32(&gUART_pDebugRegs->rUtrStat);		
		temp32&=0x04;
		if(temp32)
		{
			break;
		}
	}
}



//////////
// Function Name : UART_Getc
// Function Description : This function read character data from uart debugging ch
// Input : NONE 
// Output : temp8, character data received through uart
// Version : 
s8 UART_Getc( void)
{
	u32 temp32;
	char temp8;
	
	while(1)
	{
		temp32 = Inp32(&gUART_pDebugRegs->rUtrStat);		
		temp32&=0x01;
		if(temp32)
			break;
	}
	temp8 = Inp8(&gUART_pDebugRegs->rUrxh);
	
	return temp8;
}


//////////
// Function Name : UART_GetKey
// Function Description : This function read character data from uart debugging ch if there is received data
// Input : NONE 
// Output : temp8 or 0, character data received through uart or 0
// Version : 
s8 UART_GetKey( void)
{
	u32 temp32;
	char temp8;

	temp32 = Inp32(&gUART_pDebugRegs->rUtrStat);
	if(temp32 & 0x1)
	{
		temp8 = Inp8(&gUART_pDebugRegs->rUrxh);
		
		return temp8;
	}
	else
	{
		return 0;
	}
}



//////////
// Function Name : UART_GetIntNum
// Function Description : Input a number(hex, dec) from UART
// Input : NONE 
// Version : 
// added by rb1004

s32 UART_GetIntNum(void)
{
    u8 str[32];
	
    UART_GetString((s8 *)str);
    
    return Str2Int((s8 *)str);
}

//Getintnum for Autotest : it use getsAutotest which uses fgetc
s32 UART_GetIntNumAutotest(void)
{
	u8 str[32];

	getsAutotest((s8 *)str);

	return Str2Int((s8 *)str);
}


//////////
// Function Name : UART_GetString
// Function Description : Input a string from UART
// Input :  
// Version : 
// added by rb1004
void UART_GetString(s8 *pStr)
{
    u8 *pStrOrg;
    u8 c;

    pStrOrg = (u8 *)pStr;
	
    while ((c= UART_Getc())!='\r')
    {
 		if (c=='\b')
 		{
 		    if ((int)pStrOrg < (int)pStr)
 		    {
 				 UART_PutString("\b \b");
 				pStr--;
 		    }
 		}
 		else
 		{
 		    *pStr++ = c;
 		     UART_Putc(c);
 		}
    }
	
    *pStr = '\0';
     UART_Putc('\n');
}


//////////
// Function Name : UART_PutString
// Function Description : Transmit a string through UART
// Input :  
// Version : 
// added by rb1004
void UART_PutString(const s8 *string)
{
	while(*string)
	{
		UART_Putc(*string++);
	}
}


//////////
// Function Name : Str2Int
// Function Description : convert string to digit.
// Input :  
// Version : 
// added by rb1004
static s32 Str2Int(s8 *string)
{
	s32 base     = 10;
	s32 minus    = 0;
	s32 result   = 0;
	s32 lastIndex;
	s32 i;

	if(string[0]=='-')
	{
		minus = 1;
		string++;
	}

	if(string[0]=='0' && (string[1]=='x' || string[1]=='X'))
	{
		base    = 16;
		string += 2;
	}

	lastIndex = strlen((const char *)string) - 1;

	if(lastIndex<0)
	{
		return -1;
	}

	if(string[lastIndex]=='h' || string[lastIndex]=='H' )
	{
		base = 16;
		string[lastIndex] = 0;
		lastIndex--;
	}

	if(base==10)
	{
		//result = atoi(string); // atoi conversion not working properly //trb 051011
		/* Alternate implementation for atoi */
		result = 0;
		for(i = 0; i<=lastIndex; i++)
		{
			result = result * 10 + string[i] - 0x30;
		}
		result = minus ? (-1*result):result;
	}
	else
	{
		for(i=0;i<=lastIndex;i++)
		{
			if(isalpha(string[i]))
			{
				if(isupper(string[i]))
				{
					result = (result<<4) + string[i] - 'A' + 10;
				}
				else
				{
					result = (result<<4) + string[i] - 'a' + 10;
				}
			}
			else
			{
				result = (result<<4) + string[i] - '0';
			}
		}
		result = minus ? (-1*result):result;
	}

	return result;
}



void InitBaseHwForUart(void)
{
	#if 0
	u32 uRegVal;

	// Uart external clock source when pclk is not used.
	uRegVal = Inp32(uart_rCLK_SRC4); 
	uRegVal &= ~(0xFFFF0000);
	uRegVal |= (UART_EXTCLK_SRC << 28)|
				(UART_EXTCLK_SRC << 24)|
				(UART_EXTCLK_SRC << 20)|
				(UART_EXTCLK_SRC << 16);
	Outp32(uart_rCLK_SRC4, uRegVal);
	//g_uOpClock = 27000000;
	#endif /* 0 */

	#if 0	
	SYSC_SetClkMuxState(eCLKMUX_UART0_XUSBXTI);
	SYSC_SetClkMuxState(eCLKMUX_UART1_XUSBXTI);
	SYSC_SetClkMuxState(eCLKMUX_UART2_XUSBXTI);
	SYSC_SetClkMuxState(eCLKMUX_UART3_XUSBXTI);
	#endif /* 0 */

	SYSC_SetClkGatebyIP(eCLKGATE_IP_UART0, TRUE);
	SYSC_SetClkGatebyIP(eCLKGATE_IP_UART1, TRUE);
	SYSC_SetClkGatebyIP(eCLKGATE_IP_UART2, TRUE);
	SYSC_SetClkGatebyIP(eCLKGATE_IP_UART3, TRUE);

	// Setting TZPC
	// For DMA mode
	Outp32(0xF1500804,0xff );
	Outp32(0xF1500810,0xff );
	Outp32(0xF150081C,0xff );
	Outp32(0xFAD00804,0xff );
	Outp32(0xFAD00810,0xff );
	Outp32(0xFAD0081C,0xff );
	Outp32(0xFAD00828,0xff );
	Outp32(0xE0600804,0xff );
	Outp32(0xE0600810,0xff );
	Outp32(0xE060081C,0xff );
	Outp32(0xE0600828,0xff );
	Outp32(0xE060081C,0x20 );	
}

void UART_SetDefaultSystemEnv(void)
{
	InitBaseHwForUart();

	INTC_SetVectAddr(NUM_UART0, Isr_Uart0);
	INTC_SetVectAddr(NUM_UART1, Isr_Uart1);
	INTC_SetVectAddr(NUM_UART2, Isr_Uart2);
	INTC_SetVectAddr(NUM_UART3, Isr_Uart3);

	INTC_SetVectAddr(NUM_PDMA0, Isr_Dma0);
	INTC_SetVectAddr(NUM_PDMA1, Isr_Dma1);

	INTC_Enable(NUM_PDMA0);	/* Always Enable DMA0 */
	INTC_Enable(NUM_PDMA1);	/* Always Enable DMA1 */
}

void UART_InitDefaultVariable(UART_CtrlInfo_st *pCtrlInfo)
{
	UartCh		LoopCnt;
	UartCh		CurDebugCh;
	int			IdxCnt;

	/* Create Non-Cachable Area Control. */
	UART_CreateNCBufCtrl(&pCtrlInfo->m_NCBufCtrlInfo, UART_BUF, UART_BUF_SIZE);
	UART_BufZeroInit((u8 *)UART_BUF, UART_BUF_SIZE);

	pCtrlInfo->m_EnUart2CtsRts = FALSE;
	pCtrlInfo->m_EnPrintInISR = TRUE;

	/* Normally Debug Channel is activated before UART Initialization is called. */
	CurDebugCh = UART_GetCurDebugCh();		/* return gUART_CtrlInfo.m_CurDebugCh */

	for(LoopCnt=0; LoopCnt<MAX_UART_CH; LoopCnt++)
	{
		if( (pCtrlInfo->m_IsDebugChActivated == TRUE) && (LoopCnt != CurDebugCh) )
		{
			/* Debug Channel was already initialized in UART_Open_Debug() in booting time. */
			
			UART_InitChVariable(LoopCnt, &pCtrlInfo->m_UartChDefaultInfo[LoopCnt]);
			pCtrlInfo->m_UartChTestInfo[LoopCnt] = pCtrlInfo->m_UartChDefaultInfo[LoopCnt];
		}
	}

	for(IdxCnt=0; IdxCnt<MAX_UART_CH; IdxCnt++)
	{
		gUART_pUartIsrCon[IdxCnt] = NULL;
	}

	for(IdxCnt=0; IdxCnt<MAX_UART_DMA_CHANNEL; IdxCnt++)
	{
		gUART_pDMAIsrCon[IdxCnt] = NULL;
	}
}

void UART_Init(UART_CtrlInfo_st *pCtrlInfo)
{
	//UART_SetDefaultSystemEnv();
}

void UART_InitChVariable(UartCh Ch, UART_CON *pUartCon)
{
	memset(pUartCon, 0x00, sizeof(UART_CON));
	
	pUartCon->m_pUartSFRBase = (UART_REGS *)(UART_BASE + (UART_OFFSET * Ch));
	pUartCon->m_UartChIdx = Ch;

	pUartCon->m_Baudrate = 115200;
	pUartCon->m_EnIRDAMode = FALSE;
	pUartCon->m_EnLoopBackMode = FALSE;
	pUartCon->m_EnAFC = FALSE;
	pUartCon->m_EnFifo = TRUE;
	pUartCon->m_TxDMABurstSize = UartDMA_1B;
	pUartCon->m_RxDMABurstSize = UartDMA_1B;
	pUartCon->m_IsUseExtClk = FALSE;
	pUartCon->m_DataBit = UartWL_8bit;
	pUartCon->m_ParityBit = UartParity_None;
	pUartCon->m_StopBit = UartStop_1bitStop;
	pUartCon->m_TxMode = UartMode_Interrupt;
	pUartCon->m_RxMode = UartMode_Interrupt;
	pUartCon->m_TxTrigLvl = UartFIFO_8B;
	pUartCon->m_RxTrigLvl = UartFIFO_8B;
	pUartCon->m_RtsTrigLvl = UartFIFO_255_63_15B;
	//pUartCon->m_EnSendBreak = FALSE;
	//pUartCon->m_EnRxTimeout = FALSE;
	pUartCon->m_EnRxTimeout = TRUE;
	pUartCon->m_ExtClkSrc = UART_SEPLL;
	pUartCon->m_ExtClkSrcDiv = 1;		/* 1 means DIV 0 because of SYSCON API function. */
	
	//pUartCon->m_OpClock = 0;		/* for debugging, Just for Read. */
	{
		u32			OpClock;
		u32			DivValue;
		u32			Slot;
		
		UART_GetOpClock(pUartCon->m_IsUseExtClk, pUartCon->m_Baudrate, pUartCon->m_ExtClkSrc, 
			pUartCon->m_ExtClkSrcDiv, &OpClock, &DivValue, &Slot);

		pUartCon->m_OpClock = OpClock;
		pUartCon->m_ClkDivValue = DivValue;
		pUartCon->m_ClkSlot = Slot;
	}

	pUartCon->m_TxTransferSize = UART_BUFFER_SIZE;
	pUartCon->m_RxTransferSize = UART_BUFFER_SIZE;
	
	pUartCon->m_CheckTxEndbyNull = FALSE;
	//pUartCon->m_CheckRxEndbyCR = TRUE;
	pUartCon->m_CheckRxEndbyCR = FALSE;

	pUartCon->m_UartRxDMANum = UART_DMA0;
	if(pUartCon->m_UartRxDMANum == UART_DMA0)
	{
		pUartCon->m_UartRxDmaCh = DMA_00 + 2 * Ch;
		pUartCon->m_UartRxDmaReqSrc = UART0_RX + 2 * Ch;
	}
	else if(pUartCon->m_UartRxDMANum == UART_DMA1)
	{
		pUartCon->m_UartRxDmaCh = DMA_10 + 2 * Ch;
		pUartCon->m_UartRxDmaReqSrc = UART0_RX + 2 * Ch;
	}

	pUartCon->m_UartTxDMANum = UART_DMA0;
	if(pUartCon->m_UartTxDMANum == UART_DMA0)
	{
		pUartCon->m_UartTxDmaCh = DMA_00 + 2 * Ch + 1;
		pUartCon->m_UartTxDmaReqSrc = UART0_TX + 2 * Ch;
	}
	else if(pUartCon->m_UartTxDMANum == UART_DMA1)
	{
		pUartCon->m_UartTxDmaCh = DMA_10 + 2 * Ch + 1;
		pUartCon->m_UartTxDmaReqSrc = UART0_TX + 2 * Ch;
	}

	switch(Ch)
	{
		case UartCh_CH0:
			pUartCon->m_IntNum = NUM_UART0;
			pUartCon->m_MaxFIFOSize = UART_FIFO_SIZE_CH0;
			break;
		
		case UartCh_CH1:
			pUartCon->m_IntNum = NUM_UART1;
			pUartCon->m_MaxFIFOSize = UART_FIFO_SIZE_CH1;
			break;

		case UartCh_CH2:
			pUartCon->m_IntNum = NUM_UART2;
			pUartCon->m_MaxFIFOSize = UART_FIFO_SIZE_CH2;
			break;

		case UartCh_CH3:
			pUartCon->m_IntNum = NUM_UART3;
			pUartCon->m_MaxFIFOSize = UART_FIFO_SIZE_CH3;
			break;

		default:
			UART_ASSERT();
			break;
	}
	pUartCon->m_DrvStrength = 0x3;

	pUartCon->m_EnModemIntMask = TRUE;
	pUartCon->m_EnTxdIntMask = TRUE;
	pUartCon->m_EnErrorIntMask = FALSE;
	pUartCon->m_EnRxdIntMask = TRUE;
}


void Isr_DMASub(UART_CON *pUartCon, DMA_CH DMACh)
{
	if(pUartCon != NULL)
	{
		if(DMACh == pUartCon->m_UartTxDmaCh)
		{
			pUartCon->m_TxBufNode.m_pCurPtr += pUartCon->m_TxBufNode.m_ReqDataSize;
			pUartCon->m_TxBufNode.m_RemainDataSize = 0;
			
			if(pUartCon->m_UartTxDMACallback !=NULL)
			{
				(*pUartCon->m_UartTxDMACallback)(pUartCon->m_UartTxDMACallback_Value);
			}

			/* Stop DMA */
			//DMA_StopDmac(&pUartCon->m_UartTxDma);
		}
		else
		{
			pUartCon->m_RxBufNode.m_pCurPtr += pUartCon->m_RxBufNode.m_ReqDataSize;
			pUartCon->m_RxBufNode.m_RemainDataSize = 0;

			if(pUartCon->m_UartRxDMACallback !=NULL)
			{
				(*pUartCon->m_UartRxDMACallback)(pUartCon->m_UartRxDMACallback_Value);
			}

			/* Stop DMA */
			//DMA_StopDmac(&pUartCon->m_UartRxDma);
		}
	}
	else
	{
		UART_ASSERT();
	}

	INTC_ClearVectAddr();
}

//////////
// Function Name : Isr_Dma0
// Function Description : This is Uart DMA request ISR which operates DMA request
// Input : NONE
// Output : NONE
// Version : v0.1
void __irq Isr_Dma0(void)
{	
	DMA_CH		DMACh;
	DMAC		DMAControl;

	if(UART_IsInterruptPrintValid())
	{
		Disp("D");
	}
	
	// This function is nessarry?
//	INTC_Disable(NUM_PDMA0);

	//DMAControl = g_oUARTInfor[0].m_oUARTTxDma;	/* Set dummy value for base address PDMA0_BASE */
	DMAControl.m_uBaseAddr = PDMA0_BASE;
	DMA_GetIntrSrc((u32 *)&DMACh, &DMAControl);
	DMA_ClearIntPending((u32)DMACh, &DMAControl);

	if(UART_IsInterruptPrintValid())
	{
		UART_Printf("\n[DMA0] CH(%d)\n", DMACh);
	}

	Isr_DMASub(gUART_pDMAIsrCon[DMACh], DMACh);

}


void __irq Isr_Dma1(void)
{
	DMA_CH		DMACh;
	DMAC		DMAControl;

	if(UART_IsInterruptPrintValid())
	{
		Disp("D");
	}
	
	DMAControl.m_uBaseAddr = PDMA1_BASE;
	DMA_GetIntrSrc((u32 *)&DMACh, &DMAControl);
	DMA_ClearIntPending((u32)DMACh, &DMAControl);

	if(UART_IsInterruptPrintValid())
	{
		UART_Printf("\n[DMA1] CH(%d)\n", DMACh);
	}

	Isr_DMASub(gUART_pDMAIsrCon[DMA_10 + DMACh], DMACh);
}

//////////
// Function Name : Isr_Uart0~3
// Function Description : These are Interrupt Service Routine of UART which are connected to Isr_UartSub
// Input : NONE
// Output : NONE
// Version : v0.1

void __irq Isr_Uart0(void)
{		
	
	Isr_UartSub(UartCh_CH0);	
	//INTC_ClearVectAddr();
}		

void __irq Isr_Uart1(void)
{	
	Isr_UartSub(UartCh_CH1);
	//INTC_ClearVectAddr();
}

void __irq Isr_Uart2(void)
{	
	Isr_UartSub(UartCh_CH2);
	//INTC_ClearVectAddr();
}

void __irq Isr_Uart3(void)
{	
	Isr_UartSub(UartCh_CH3);
	//INTC_ClearVectAddr();	
}	


void Isr_UartSub_Error(UART_CON *pUartCon, UART_REGS *pUartRegs)
{
	u32 ErrorStatus = 0;	
	u32 uTemp;

	UART_Printf("Error occured!!\n");

	ErrorStatus = Inp32(&pUartRegs->rUerStat);

	if(ErrorStatus & 0x01)
	{
		UART_Printf("%d ch : Overrun error!\n", pUartCon->m_UartChIdx);
		// Rx/Tx FIFO Reset
		uTemp = pUartRegs->rUfCon;
		uTemp |= ((1<<2)|(1<<1));
		Outp32(&pUartRegs->rUfCon, uTemp);
	}

	if(ErrorStatus & 0x02)
	{
		UART_Printf("%d ch : Parity error!\n", pUartCon->m_UartChIdx);
	}

	if(ErrorStatus & 0x04)
	{
		UART_Printf("%d ch : Frame error!\n", pUartCon->m_UartChIdx);
	}

	if(ErrorStatus & 0x08)
	{
		UART_Printf("%d ch : Breake detect\n", pUartCon->m_UartChIdx);
	}

	//uTemp = Inp32(&pUartRegs->rUintM);
	//uTemp |= BIT_UART_ERROR;
	//Outp32(&pUartRegs->rUintM , uTemp);
	//pUartRegs->rUintM |= BIT_UART_ERROR;
	Outp32(&pUartRegs->rUintSp , BIT_UART_ERROR);
	Outp32(&pUartRegs->rUintP , BIT_UART_ERROR);
}

void Isr_UartSub_FifoTx(UART_CON *pUartCon, UART_REGS *pUartRegs)
{
	u32		uTemp;
	u32		ValidTxSize;
	u32		CurTxFifoCnt;
	bool		IsTxEnd;

	CurTxFifoCnt = Inp32(&pUartRegs->rUfStat);
	//CurTxFifoCnt = UfStatReg;
	CurTxFifoCnt >>= 16;
	if(CurTxFifoCnt & 0x100)	/* Check FIFO FULL */
	{
		ValidTxSize = 0;
	}
	else
	{
		CurTxFifoCnt &= 0xFF;
		ValidTxSize = pUartCon->m_MaxFIFOSize - CurTxFifoCnt;
	}
	
	if(UART_IsInterruptPrintValid())
	{
		//UART_Printf("\nTxValid(%d)\n", ValidTxSize);
	}

	IsTxEnd = FALSE;

	while (ValidTxSize-- > 0)
	{
		if(pUartCon->m_TxBufNode.m_RemainDataSize == 0)
		{
			IsTxEnd = TRUE;
			break;
		}
		else
		{
			if(pUartCon->m_CheckTxEndbyNull == TRUE && *pUartCon->m_TxBufNode.m_pCurPtr == '\0')
			{
				IsTxEnd = TRUE;
				break;
			}
			else
			{
				if( ((u32)pUartCon->m_TxBufNode.m_pCurPtr >= (u32)pUartCon->m_TxBufNode.m_pBufPtr) &&
					((u32)pUartCon->m_TxBufNode.m_pCurPtr < (u32)pUartCon->m_TxBufNode.m_pBufPtr + 
						(u32)pUartCon->m_TxBufNode.m_BufSize) )
				{
					Outp32(&pUartRegs->rUtxh , *pUartCon->m_TxBufNode.m_pCurPtr++);
					pUartCon->m_TxBufNode.m_RemainDataSize--;
					if(pUartCon->m_TxBufNode.m_RemainDataSize == 0)
					{
						IsTxEnd = TRUE;
						break;
					}
				}
				else
				{
					/* Out of buffer range */
					UART_ASSERT();
				}
			}
		}
	}

	if( IsTxEnd == TRUE )
	{
		if(pUartCon->m_UartTxIntCallback != NULL)
		{
			(pUartCon->m_UartTxIntCallback)(pUartCon->m_UartTxIntCallback_Value);
		}

		/* Disalbe Tx Interrupt */
		uTemp = Inp32(&pUartRegs->rUintM);
		uTemp |= BIT_UART_TXD;
		Outp32(&pUartRegs->rUintM , uTemp);
	}
}


void Isr_UartSub_NonFifoTx(UART_CON *pUartCon, UART_REGS *pUartRegs)
{
	u32		uTemp;
	u32		ValidTxSize;

	if(Inp32(&pUartRegs->rUtrStat) & 0x4)
	{
		/* Tx Holding Register is not empty. */
		UART_ASSERT();
	}

	if( ((u32)pUartCon->m_TxBufNode.m_pCurPtr >= (u32)pUartCon->m_TxBufNode.m_pBufPtr) &&
		((u32)pUartCon->m_TxBufNode.m_pCurPtr < (u32)pUartCon->m_TxBufNode.m_pBufPtr + 
			pUartCon->m_TxBufNode.m_BufSize) )
	{
		if( pUartCon->m_TxBufNode.m_RemainDataSize == 0 || 
			(pUartCon->m_CheckTxEndbyNull == TRUE && *pUartCon->m_TxBufNode.m_pCurPtr == '\0') )
		{
			/* End of UART Tx */

			if(pUartCon->m_UartTxIntCallback != NULL)
			{
				(pUartCon->m_UartTxIntCallback)(pUartCon->m_UartTxIntCallback_Value);
			}

			/* Disalbe Tx Interrupt */
			uTemp = Inp32(&pUartRegs->rUintM);
			uTemp |= BIT_UART_TXD;
			Outp32(&pUartRegs->rUintM , uTemp);
		}
		else
		{
			Outp32(&pUartRegs->rUtxh , *pUartCon->m_TxBufNode.m_pCurPtr++);
			pUartCon->m_TxBufNode.m_RemainDataSize--;
		}
	}
	else
	{
		/* Out of buffer range */
		UART_ASSERT();
	}	
}


void Isr_UartSub_FifoRx(UART_CON *pUartCon, UART_REGS *pUartRegs)
{
	u32		ValidRxSize;
	u32		CurRxFifoCnt;
	bool		IsRxEnd;
	u8		tempChar;
	u32		uTemp;

	CurRxFifoCnt = Inp32(&pUartRegs->rUfStat);
	//CurRxFifoCnt = UfStatReg;
	if(CurRxFifoCnt & 0x100)	/* Check FIFO full */
	{
		ValidRxSize = pUartCon->m_MaxFIFOSize ;
	}
	else
	{
		ValidRxSize = CurRxFifoCnt & 0xFF;
	}
	
	if(UART_IsInterruptPrintValid())
	{
		//UART_Printf("\nRxValid(%d)\n", ValidRxSize);
	}

	IsRxEnd = FALSE;
	//while ((Inp32(&pUartRegs->rUfStat )& FIFO_Count_Mask))
	while (ValidRxSize-- > 0)
	{
		tempChar = (u8)(Inp32(&pUartRegs->rUrxh));

		if(pUartCon->m_RxBufNode.m_RemainDataSize == 0)
		{
			IsRxEnd = TRUE;
			break;
		}
		else
		{
			if(pUartCon->m_CheckRxEndbyCR == TRUE && tempChar == '\r')
			{
				*pUartCon->m_RxBufNode.m_pCurPtr = '\0';
				IsRxEnd = TRUE;
				break;
			}
			else
			{
				if( ((u32)pUartCon->m_RxBufNode.m_pCurPtr >= (u32)pUartCon->m_RxBufNode.m_pBufPtr) &&
					((u32)pUartCon->m_RxBufNode.m_pCurPtr < (u32)pUartCon->m_RxBufNode.m_pBufPtr + 
						pUartCon->m_RxBufNode.m_BufSize) )
				{
					*pUartCon->m_RxBufNode.m_pCurPtr = tempChar;
					pUartCon->m_RxBufNode.m_pCurPtr++;
					pUartCon->m_RxBufNode.m_RemainDataSize--;
					if(pUartCon->m_RxBufNode.m_RemainDataSize == 0)
					{
						IsRxEnd = TRUE;
						break;
					}
				}
				else
				{
					/* Out of buffer range */
					UART_ASSERT();
				}
			}
		}
	}

	if(IsRxEnd == TRUE)
	{
		if(pUartCon->m_UartRxIntCallback != NULL)
		{
			(pUartCon->m_UartRxIntCallback)(pUartCon->m_UartRxIntCallback_Value);
		}

		/* Disalbe Rx Interrupt */
		uTemp = Inp32(&pUartRegs->rUintM);
		uTemp |= BIT_UART_RXD;
		Outp32(&pUartRegs->rUintM , uTemp);
	}
}


void Isr_UartSub_NonFifoRx(UART_CON *pUartCon, UART_REGS *pUartRegs)
{
	u8		tempChar;
	u32		uTemp;

	if(Inp32(&pUartRegs->rUtrStat) & 0x1)
	{
		/* Receive buffer is NOT ready. */
		UART_ASSERT();
	}

	tempChar = (u8)(Inp32(&pUartRegs->rUrxh));

	if(pUartCon->m_RxBufNode.m_RemainDataSize == 0 ||
		(pUartCon->m_CheckRxEndbyCR == TRUE && tempChar == '\r') )
	{
			/* End of UART Rx */

			if(pUartCon->m_UartRxIntCallback != NULL)
			{
				(pUartCon->m_UartRxIntCallback)(pUartCon->m_UartRxIntCallback_Value);
			}

			/* Disalbe Rx Interrupt */
			uTemp = Inp32(&pUartRegs->rUintM);
			uTemp |= BIT_UART_RXD;
			Outp32(&pUartRegs->rUintM , uTemp);
	}
	else
	{
		if( ((u32)pUartCon->m_RxBufNode.m_pCurPtr >= (u32)pUartCon->m_RxBufNode.m_pBufPtr) &&
			((u32)pUartCon->m_RxBufNode.m_pCurPtr < (u32)pUartCon->m_RxBufNode.m_pBufPtr + 
				pUartCon->m_RxBufNode.m_BufSize) )
		{
			*pUartCon->m_RxBufNode.m_pCurPtr = (u8)(Inp32(&pUartRegs->rUrxh));
			pUartCon->m_RxBufNode.m_pCurPtr++;
			pUartCon->m_RxBufNode.m_RemainDataSize--;
		}
		else
		{
			/* Out of buffer range */
			UART_ASSERT();
		}
	}
}


//////////
// Function Name : Isr_UartSub
// Function Description : This is Uart SubISR which checks errors and operates Tx and Rx
// Input : NONE
// Output : NONE
// Version : v0.1

void Isr_UartSub(UartCh ch)
{
	UART_CON	*pUartCon;
	
	volatile UART_REGS *pUartRegs;			
	u32 uTemp;
	u32 UfStatReg;
	u32 UintPReg;

	pUartCon = gUART_pUartIsrCon[ch];
	if(pUartCon == NULL)
	{
		UART_ASSERT();
	}

	pUartRegs = pUartCon->m_pUartSFRBase;

	//UART_Printf("U");
	//UART_Printf("\nFSTAT : 0x%08x\n", Inp32(&pUartRegs->rUfStat));	/* FIFO Status Register */

	UfStatReg = Inp32(&pUartRegs->rUfStat);
	if(UART_IsInterruptPrintValid())
	{
		UART_Printf("\nFSTAT : 0x%08x\n", UfStatReg);	/* FIFO Status Register */
	}

	UintPReg = Inp32(&pUartRegs->rUintP);
	//UART_Printf("\nUINTP : 0x%08x\n", UintPReg);	/* Interrupt Status */
	
	// Check Errors 	
	if( UintPReg & BIT_UART_ERROR )
	{
		Isr_UartSub_Error(pUartCon, pUartRegs);
	}

	// Tx ISR	
	if( UintPReg & BIT_UART_TXD )
	{
		if (Inp32(&pUartRegs->rUfCon ) & FIFO_Enable) // 1 : fifo enable
		{
			Isr_UartSub_FifoTx(pUartCon, pUartRegs);
		}
		else // 0 : fifo disable
		{
			Isr_UartSub_NonFifoTx(pUartCon, pUartRegs);
		}

		//uTemp = Inp32(&pUartRegs->rUintM);
		//uTemp |= BIT_UART_TXD;
		//Outp32(&pUartRegs->rUintM , uTemp);
		//pUartRegs->rUintM |= BIT_UART_TXD;
		Outp32(&pUartRegs->rUintSp , BIT_UART_TXD);
		Outp32(&pUartRegs->rUintP , BIT_UART_TXD);
	}

	// Rx ISR	
	if(UintPReg & BIT_UART_RXD)
	{
		if (pUartRegs->rUfCon & FIFO_Enable) // 1 : fifo enable 
		{
			Isr_UartSub_FifoRx(pUartCon, pUartRegs);
		}
		else // 0 : fifo disable
		{
			Isr_UartSub_NonFifoRx(pUartCon, pUartRegs);
		}

		//uTemp = Inp32(&pUartRegs->rUintM);
		//uTemp |= BIT_UART_RXD;
		//Outp32(&pUartRegs->rUintM , uTemp);
		Outp32(&pUartRegs->rUintSp, BIT_UART_RXD);
		Outp32(&pUartRegs->rUintP, BIT_UART_RXD);
	}

	// Modem UART ISR	
	if( UintPReg & BIT_UART_MODEM )
	{
		// Implementation for modem uart
		if(UART_IsInterruptPrintValid())
		{
			UART_Printf("Modem Int Occured!\n");
		}

		//uTemp = Inp32(&pUartRegs->rUintM);
		//uTemp &= ~( BIT_UART_MODEM);
		//Outp32(&pUartRegs->rUintM , uTemp);

		//pUartRegs->rUintM &= ~(BIT_UART_MODEM);

		//uTemp = Inp32(&pUartRegs->rUintM);
		//uTemp |= BIT_UART_MODEM;
		//Outp32(&pUartRegs->rUintM , uTemp);
		Outp32(&pUartRegs->rUintSp , BIT_UART_MODEM);
		Outp32(&pUartRegs->rUintP , BIT_UART_MODEM);
	}

	// unknown UART interrupt 
	if( UintPReg & ~(BIT_UART_MODEM|BIT_UART_TXD|BIT_UART_ERROR|BIT_UART_RXD) )
	{
		if(UART_IsInterruptPrintValid())
		{
			UART_Printf("UARTx sub INT - unknown sub interrupt!!\n");
		}
	}

	INTC_ClearVectAddr();
}


//////////
// Function Name : UART_Open_Debug
// Function Description : This function set up H/W(GPIO) and initialize SFR of UART for Debugging
// Input : ch [0~4 UART channel]
// Output : g_pUartRxStr[ch] [charater type received string]
// Version : v0.1
u8 UART_Open_Debug(UART_CON *pUartCon)
{
	UART_SetGPIO(pUartCon);
	UART_SetExtClkSrc(pUartCon);


	#if USE_DEBUGPORT_EXTCLK
	if(pUartCon->m_IsUseExtClk == TRUE)
	{
		Delay(1000);	/* wait 1000 us for external clock stable */
	}
	#endif /* USE_DEBUGPORT_EXTCLK */

	UART_InitializeREG(pUartCon);

	gUART_pDebugRegs = pUartCon->m_pUartSFRBase;

	return TRUE;
}


void UART_SetGPIO(UART_CON *pUartCon)
{
	switch(pUartCon->m_UartChIdx)
	{
		case UartCh_CH0 :
			GPIO_SetFunctionEach(eGPIO_A0, eGPIO_0, eGFunc_0); //Uart0 RXD
			GPIO_SetFunctionEach(eGPIO_A0, eGPIO_1, eGFunc_0); //Uart0 TXD			
			GPIO_SetFunctionEach(eGPIO_A0, eGPIO_2, eGFunc_0); //Uart0 CTSn			
			GPIO_SetFunctionEach(eGPIO_A0, eGPIO_3, eGFunc_0); //Uart0 RTSn			

			GPIO_SetPullUpDownEach( eGPIO_A0, eGPIO_0, 0 );	
			GPIO_SetPullUpDownEach( eGPIO_A0, eGPIO_1, 0 );	
			GPIO_SetPullUpDownEach( eGPIO_A0, eGPIO_2, 0 );	
			GPIO_SetPullUpDownEach( eGPIO_A0, eGPIO_3, 0 );	

			GPIO_SetDSEach(eGPIO_A0, eGPIO_0, pUartCon->m_DrvStrength);
			GPIO_SetDSEach(eGPIO_A0, eGPIO_1, pUartCon->m_DrvStrength);
			GPIO_SetDSEach(eGPIO_A0, eGPIO_2, pUartCon->m_DrvStrength);
			GPIO_SetDSEach(eGPIO_A0, eGPIO_3, pUartCon->m_DrvStrength);

			break;

		case UartCh_CH1 :
			GPIO_SetFunctionEach(eGPIO_A0, eGPIO_4, eGFunc_0); //Uart1 RXD
			GPIO_SetFunctionEach(eGPIO_A0, eGPIO_5, eGFunc_0); //Uart1 TXD			
			GPIO_SetFunctionEach(eGPIO_A0, eGPIO_6, eGFunc_0); //Uart1 CTSn			
			GPIO_SetFunctionEach(eGPIO_A0, eGPIO_7, eGFunc_0); //Uart1 RTSn		

			GPIO_SetPullUpDownEach( eGPIO_A0, eGPIO_4, 0 );	
			GPIO_SetPullUpDownEach( eGPIO_A0, eGPIO_5, 0 );	
			GPIO_SetPullUpDownEach( eGPIO_A0, eGPIO_6, 0 );	
			GPIO_SetPullUpDownEach( eGPIO_A0, eGPIO_7, 0 );	

			GPIO_SetDSEach(eGPIO_A0, eGPIO_4, pUartCon->m_DrvStrength);
			GPIO_SetDSEach(eGPIO_A0, eGPIO_5, pUartCon->m_DrvStrength);
			GPIO_SetDSEach(eGPIO_A0, eGPIO_6, pUartCon->m_DrvStrength);
			GPIO_SetDSEach(eGPIO_A0, eGPIO_7, pUartCon->m_DrvStrength);

			break;

		case UartCh_CH2 :
			GPIO_SetFunctionEach(eGPIO_A1, eGPIO_0, eGFunc_0); //Uart2 RXD
			GPIO_SetFunctionEach(eGPIO_A1, eGPIO_1, eGFunc_0); //Uart2 TXD			

			GPIO_SetPullUpDownEach( eGPIO_A1, eGPIO_0, 0 );	
			GPIO_SetPullUpDownEach( eGPIO_A1, eGPIO_1, 0 );	

			GPIO_SetDSEach(eGPIO_A1, eGPIO_0, pUartCon->m_DrvStrength);
			GPIO_SetDSEach(eGPIO_A1, eGPIO_1, pUartCon->m_DrvStrength);

			if(gUART_CtrlInfo.m_EnUart2CtsRts == TRUE)
			{
				GPIO_SetFunctionEach(eGPIO_A1, eGPIO_2, eGFunc_1); //Uart2 CTSn
				GPIO_SetFunctionEach(eGPIO_A1, eGPIO_3, eGFunc_1); //Uart2 RTSn

				GPIO_SetPullUpDownEach( eGPIO_A1, eGPIO_2, 0 );	
				GPIO_SetPullUpDownEach( eGPIO_A1, eGPIO_3, 0 );	

				GPIO_SetDSEach(eGPIO_A1, eGPIO_2, pUartCon->m_DrvStrength);
				GPIO_SetDSEach(eGPIO_A1, eGPIO_3, pUartCon->m_DrvStrength);
			}

			break;

		case UartCh_CH3 :
			GPIO_SetFunctionEach(eGPIO_A1, eGPIO_2, eGFunc_0); //Uart2 RXD
			GPIO_SetFunctionEach(eGPIO_A1, eGPIO_3, eGFunc_0); //Uart2 TXD			

			GPIO_SetPullUpDownEach( eGPIO_A1, eGPIO_2, 0 );	
			GPIO_SetPullUpDownEach( eGPIO_A1, eGPIO_3, 0 );	

			GPIO_SetDSEach(eGPIO_A1, eGPIO_2, pUartCon->m_DrvStrength);
			GPIO_SetDSEach(eGPIO_A1, eGPIO_3, pUartCon->m_DrvStrength);

			break;

		default :
			UART_ASSERT();
			break;
	}


}


void UART_SetExtClkSrc(UART_CON *pUartCon)
{
	switch(pUartCon->m_UartChIdx)
	{
		case UartCh_CH0 :
			SYSC_SetClkMuxState(eCLKMUX_UART0_XXTI + pUartCon->m_ExtClkSrc);
			SYSC_SetClkDiv(eCLKDIV_UART0, pUartCon->m_ExtClkSrcDiv);	/* Do NOT Divide */
			break;

		case UartCh_CH1 :
			SYSC_SetClkMuxState(eCLKMUX_UART1_XXTI + pUartCon->m_ExtClkSrc);
			SYSC_SetClkDiv(eCLKDIV_UART1, pUartCon->m_ExtClkSrcDiv);	/* Do NOT Divide */
			break;

		case UartCh_CH2 :
			SYSC_SetClkMuxState(eCLKMUX_UART2_XXTI + pUartCon->m_ExtClkSrc);
			SYSC_SetClkDiv(eCLKDIV_UART2, pUartCon->m_ExtClkSrcDiv);	/* Do NOT Divide */
			break;

		case UartCh_CH3 :
			SYSC_SetClkMuxState(eCLKMUX_UART3_XXTI + pUartCon->m_ExtClkSrc);
			SYSC_SetClkDiv(eCLKDIV_UART3, pUartCon->m_ExtClkSrcDiv);	/* Do NOT Divide */
			break;

		default :
			UART_ASSERT();
			break;
	}
}

//////////
// Function Name : UART_Open
// Function Description : This function set up H/W(GPIO) and initialize SFR of UART for Testing
// Input : ch [0~4 UART channel]
// Output : g_pUartRxStr[ch] [charater type received string]
// Version : v0.1

u8 UART_Open(UART_CON *pUartCon)
{
	#if 0
	INTC_SetVectAddr(NUM_PDMA0,Isr_Dma0);
	INTC_SetVectAddr(NUM_PDMA1,Isr_Dma1);

	INTC_SetVectAddr(NUM_UART0,Isr_Uart0);
	INTC_SetVectAddr(NUM_UART1,Isr_Uart1);
	INTC_SetVectAddr(NUM_UART2,Isr_Uart2);
	INTC_SetVectAddr(NUM_UART3,Isr_Uart3);
	#endif /* 0 */
	
	UART_SetGPIO(pUartCon);
	UART_SetExtClkSrc(pUartCon);

	if(UART_InitializeREG(pUartCon)  == SENDBREAK)
	{
		return SENDBREAK; // Initialize register set for Uart 0
	}

	/* Initialize the variable to refer to it in ISR. */
	gUART_pUartIsrCon[pUartCon->m_UartChIdx] = pUartCon;
	gUART_pDMAIsrCon[pUartCon->m_UartTxDmaCh] = pUartCon;
	gUART_pDMAIsrCon[pUartCon->m_UartRxDmaCh] = pUartCon;

	#if 0
	switch(pUartCon->m_UartChIdx)
	{
		case UartCh_CH0 :
			INTC_Enable(NUM_UART0);
			break;

		case UartCh_CH1 :
			INTC_Enable(NUM_UART1);
			break;

		case UartCh_CH2 :
			INTC_Enable(NUM_UART2);
			break;

		case UartCh_CH3 :
			INTC_Enable(NUM_UART3);
			break;

		default :
			UART_ASSERT();
			break;
	}
	#endif /* 0 */
	
	return TRUE;
}


//////////
// Function Name : UART_Close
// Function Description : This function closed H/W(GPIO) and disable UART exception
// Input : ch [0~4 UART channel]
// Output : 1: success 2:fail
// Version : v0.1

u8 UART_Close(UART_CON *pUartCon)
{
	#if 0
	volatile UART_REGS *pUartRegs;

	if (ch > 3) return FALSE;
	pUartRegs = (volatile UART_REGS *)(UART_BASE+UART_OFFSET*ch);
	#endif /* 0 */

	#if 0
	INTC_Disable(NUM_PDMA0);
	INTC_Disable(NUM_PDMA1);
	#endif /* 0 */

	switch(pUartCon->m_UartChIdx)
	{
		case UartCh_CH0:
			INTC_Disable(NUM_UART0);
			break;

		case UartCh_CH1:
			INTC_Disable(NUM_UART1);
			break;

		case UartCh_CH2:
			INTC_Disable(NUM_UART2);
			break;

		case UartCh_CH3:
			INTC_Disable(NUM_UART3);
			break;

		default:
			UART_ASSERT();
			break;
	}
	
	#if 0
	pUartRegs->rUintM |= (BIT_UART_MODEM|BIT_UART_TXD|BIT_UART_ERROR|BIT_UART_RXD);
	
	switch(ch)
	{
		case 0 :
			INTC_Disable(NUM_UART0);
			#if 0
			GPIO_SetFunctionEach(eGPIO_A0, eGPIO_4, eGPI); //Uart1 RXD
			GPIO_SetFunctionEach(eGPIO_A0, eGPIO_5, eGPI); //Uart1 TXD			
			GPIO_SetFunctionEach(eGPIO_A0, eGPIO_6, eGPI); //Uart1 CTSn			
			GPIO_SetFunctionEach(eGPIO_A0, eGPIO_7, eGPI); //Uart1 RTSn		
			#else
			GPIO_SetFunctionEach(eGPIO_A0, eGPIO_0, eGPI); //Uart1 RXD
			GPIO_SetFunctionEach(eGPIO_A0, eGPIO_1, eGPI); //Uart1 TXD			
			GPIO_SetFunctionEach(eGPIO_A0, eGPIO_2, eGPI); //Uart1 CTSn			
			GPIO_SetFunctionEach(eGPIO_A0, eGPIO_3, eGPI); //Uart1 RTSn		
			#endif /* 0 */
	
			break;
		case 1 :
			INTC_Disable(NUM_UART1);
			GPIO_SetFunctionEach(eGPIO_A0, eGPIO_4, eGPI); //Uart1 RXD
			GPIO_SetFunctionEach(eGPIO_A0, eGPIO_5, eGPI); //Uart1 TXD			
			GPIO_SetFunctionEach(eGPIO_A0, eGPIO_6, eGPI); //Uart1 CTSn			
			GPIO_SetFunctionEach(eGPIO_A0, eGPIO_7, eGPI); //Uart1 RTSn		
			break;
		case 2 :
			INTC_Disable(NUM_UART2);
			GPIO_SetFunctionEach(eGPIO_A1, eGPIO_0, eGPI); //Uart2 RXD
			GPIO_SetFunctionEach(eGPIO_A1, eGPIO_1, eGPI); //Uart2 TXD	
			break;
		case 3 :
			INTC_Disable(NUM_UART3);
			GPIO_SetFunctionEach(eGPIO_A1, eGPIO_2, eGPI); //Uart2 RXD
			GPIO_SetFunctionEach(eGPIO_A1, eGPIO_3, eGPI); //Uart2 TXD		
			break;
		default :
			UART_Printf("Can't close UARTx!! Select 0,1, or 2!!");
			return FALSE;
	}
	#endif /* 0 */
	
	return TRUE;
}


//////////
// Function Name : UART_InitializeREG
// Function Description : This function set up SFR by pre-defined value 
// Input : *pUartRegs [SFR value base address]
//		 *pUartCon [Pre-defined value's base address, UART_Config]
// Output : SENDBREAK [when sendbreak signal set up]
//		    0 [nomal mode (sendbreak signal disable)]
// Version : v0.1

u32 UART_InitializeREG(UART_CON *pUartCon) // Initialize register set with current control set
{
	UART_REGS	*pUartRegs;

	pUartRegs = pUartCon->m_pUartSFRBase;
	
	UART_CalcBaudrate(pUartCon->m_IsUseExtClk, pUartCon->m_Baudrate,
		pUartCon->m_ExtClkSrc, pUartCon->m_ExtClkSrcDiv, &pUartCon->m_OpClock, 
		&pUartCon->m_ClkDivValue, &pUartCon->m_ClkSlot);
	UART_SelectExtClkSrc(pUartCon);

	Delay(1);	/* it needs some delay after changing a baudrate. */

	Outp32(&pUartRegs->rUfCon , (pUartCon->m_TxTrigLvl<<8)|(pUartCon->m_RxTrigLvl<<4)|
//			TX_FIFO_RESET|RX_FIFO_RESET	|(pUartCon->m_EnFifo));
			(pUartCon->m_EnFifo));

	Outp32(&pUartRegs->rUlCon , (pUartCon->m_EnIRDAMode<<6)|(pUartCon->m_ParityBit<<3)|
			(pUartCon->m_StopBit<<2) |(pUartCon->m_DataBit));
	if(pUartCon->m_EnRxTimeout)
	{
		Outp32(&pUartRegs->rUCon , (pUartCon->m_TxDMABurstSize<<20)|(pUartCon->m_RxDMABurstSize<<16)|
			(pUartCon->m_IsUseExtClk<<10)|TX_INT_TYPE|RX_INT_TYPE|RX_ERR_INT_EN|RX_TIMEOUT_EN
			//|(pUartCon->m_EnLoopBackMode<<5)| (pUartCon->m_EnSendBreak<<4) |(pUartCon->m_TxMode<<2)|
			|(pUartCon->m_EnLoopBackMode<<5)| (pUartCon->m_TxMode<<2)|
			(pUartCon->m_RxMode));
	}
	else
	{
		Outp32(&pUartRegs->rUCon , (pUartCon->m_TxDMABurstSize<<20)|(pUartCon->m_RxDMABurstSize<<16)|
		(pUartCon->m_IsUseExtClk<<10)|TX_INT_TYPE|RX_INT_TYPE|RX_ERR_INT_EN	|(pUartCon->m_EnLoopBackMode<<5)|
		//(pUartCon->m_EnSendBreak<<4) |(pUartCon->m_TxMode<<2)|(pUartCon->m_RxMode));
		(pUartCon->m_TxMode<<2)|(pUartCon->m_RxMode));
	}

	#if 0
	if(pUartCon->m_EnSendBreak)
	{
		return SENDBREAK;
	}
	#endif /* 0 */

	Outp32(&pUartRegs->rUfCon , (pUartCon->m_TxTrigLvl<<8)|(pUartCon->m_RxTrigLvl<<4)|
		TX_FIFO_RESET|RX_FIFO_RESET	|(pUartCon->m_EnFifo));

	Outp32(&pUartRegs->rUmCon , (pUartCon->m_RtsTrigLvl<<5)|(pUartCon->m_EnAFC<<4)|RTS_ACTIVE);	
	//Outp32(&pUartRegs->rUintM , 0xf); // mask
	Outp32(&pUartRegs->rUintM , (pUartCon->m_EnModemIntMask << 3) | (pUartCon->m_EnTxdIntMask << 2) |
		(pUartCon->m_EnErrorIntMask << 1) | (pUartCon->m_EnRxdIntMask << 0) );
	Outp32(&pUartRegs->rUintSp , 0xf); // source pending clear
	Outp32(&pUartRegs->rUintP , 0xf); // pending clear

	return 0;
}


//////////
// Function Name : UART_CalcBaudrate
// Function Description : This function defines UbrDiv and UdivSlot value depends on OpClock src
// Input : *pUartRegs [SFR value base address]
//		 *pUartCon [Pre-defined value's base address, UART_Config]
// Output : NONE
// Version : v0.1
const u32 aSlotTable[16] = {0x0000, 0x0080, 0x0808, 0x0888, 0x2222, 0x4924, 0x4a52, 0x54aa,
			     		0x5555, 0xd555, 0xd5d5, 0xddd5, 0xdddd, 0xdfdd, 0xdfdf, 0xffdf};



s32 UART_GetOpClock(bool IsUseExtClk, u32 Baudrate, UART_ClkSrc_et ExtClkSrc, 
	u32 ExtClkSrcDiv, u32 *pOpClock, u32 *pDivValue, u32 *pSlot)
{
	u32			nSlotIdx;
	float			tempDiv;

	if(IsUseExtClk != TRUE)
	{
		*pOpClock = g_uPclkPsys;
	}
	else
	{
		switch(ExtClkSrc)
		{
			case UART_XXTI:
			case UART_XUSBXTI:
				*pOpClock = FIN;			/* 24 MHz */
				break;

			case UART_SHDMI27M:
				*pOpClock = 27000000;		/* 27 MHz */
				break;

			case UART_SUSBPHY0:
				*pOpClock = 48000000;		/* 48 MHz */
				break;

			case UART_SUSBPHY1:
				*pOpClock = 48000000;		/* 48 MHz */
				break;

			case UART_SHDMIPHY:
				*pOpClock = 27000000;		/* 27 MHz */
				break;

			case UART_SMPLL:
				*pOpClock = g_uMPLL;		/* 27 MHz */
				break;

			case UART_SEPLL:
				*pOpClock = g_uEPLL;		/* 27 MHz */
				break;

			case UART_SVPLL:
				*pOpClock = g_uVPLL;		/* 27 MHz */
				break;

			default:
				UART_ASSERT();
				break;
		}

		if(ExtClkSrcDiv == 0)
		{
			UART_ASSERT();
		}
		*pOpClock /= ExtClkSrcDiv;
	}

	tempDiv = ( (float)*pOpClock  / ((float)16.0 * (float)Baudrate) ) - 1;
	if((int)tempDiv == 0 && IsUseExtClk != TRUE) {
		/* DIV_VAL should be greater than 0 in PCLK, but may be equal 0 in SCLK. */
		
		UART_Printf("\nError : DIV_VAL(%d) in PCLK", (u32)tempDiv);
		UART_Printf("\n\nPress any key to continue....\n\n");

		while(!UART_GetKey())
		{
		}

		return UART_ERROR;
	}
	nSlotIdx = (u32)((tempDiv - (int)tempDiv) * 16);

	*pDivValue = (u32)tempDiv;
	*pSlot = aSlotTable[nSlotIdx];

	return UART_NO_ERROR;
}



s32 UART_CalcBaudrate(bool IsUseExtClk, u32 Baudrate,
	UART_ClkSrc_et ExtClkSrc, u32 ExtClkSrcDiv, u32 *pOpClock, u32 *pClockDivValue, u32 *pClockSlot)
{
	UART_REGS	*pUartRegs;
	u32			OpClock;
	u32			DivValue;
	u32			Slot;
	s32			Status;

	Status = UART_GetOpClock(IsUseExtClk, Baudrate, ExtClkSrc, 
		ExtClkSrcDiv, &OpClock, &DivValue, &Slot);
	if(Status != UART_NO_ERROR)
	{
		return Status;
	}

	*pOpClock = OpClock;		/* Save the current Clock Frequence for Debugging. */
	*pClockDivValue = DivValue;
	*pClockSlot = Slot;

	return UART_NO_ERROR;
}



s32 UART_SelectExtClkSrc(UART_CON *pUartCon)
{
	UART_REGS	*pUartRegs;
	s32 Status;

	switch(pUartCon->m_UartChIdx)
	{
		case UartCh_CH0:
			SYSC_SetClkMuxState(eCLKMUX_UART0_XXTI + pUartCon->m_ExtClkSrc);
			SYSC_SetClkDiv(eCLKDIV_UART0, pUartCon->m_ExtClkSrcDiv);
			break;
		case UartCh_CH1:
			SYSC_SetClkMuxState(eCLKMUX_UART1_XXTI + pUartCon->m_ExtClkSrc);
			SYSC_SetClkDiv(eCLKDIV_UART1, pUartCon->m_ExtClkSrcDiv);
			break;
		case UartCh_CH2:
			SYSC_SetClkMuxState(eCLKMUX_UART2_XXTI + pUartCon->m_ExtClkSrc);
			SYSC_SetClkDiv(eCLKDIV_UART2, pUartCon->m_ExtClkSrcDiv);
			break;
		case UartCh_CH3:
			SYSC_SetClkMuxState(eCLKMUX_UART3_XXTI + pUartCon->m_ExtClkSrc);
			SYSC_SetClkDiv(eCLKDIV_UART3, pUartCon->m_ExtClkSrcDiv);
			break;

		default:
			UART_ASSERT();
			break;
	}

	pUartRegs = pUartCon->m_pUartSFRBase;
	
	Outp32(&pUartRegs->rUbrDiv , pUartCon->m_ClkDivValue);
	Outp32(&pUartRegs->rUdivSlot , pUartCon->m_ClkSlot);	
}



void UART_SetIsrFunc(UART_CON *pUartCon, UART_IntSrc_et UartIntSrc, void (*fnIsrFunc)(u32), u32 IsrVar)
{
	switch(UartIntSrc)
	{
		case UART_INT_TX:
			pUartCon->m_UartTxIntCallback = fnIsrFunc;
			pUartCon->m_UartTxIntCallback_Value = IsrVar;
			break;

		case UART_INT_RX:
			pUartCon->m_UartRxIntCallback = fnIsrFunc;
			pUartCon->m_UartRxIntCallback_Value = IsrVar;
			break;
			
		case UART_INT_DMA_TX:
			pUartCon->m_UartTxDMACallback = fnIsrFunc;
			pUartCon->m_UartTxDMACallback_Value = IsrVar;
			break;

		case UART_INT_DMA_RX:
			pUartCon->m_UartRxDMACallback = fnIsrFunc;
			pUartCon->m_UartRxDMACallback_Value = IsrVar;
			break;

		default:
			UART_ASSERT();
			break;
	}
}


void UART_PollingTxString(UART_CON *pUart)
{
	volatile UART_REGS *pUartRegs;
	
	pUartRegs = pUart->m_pUartSFRBase;


	while(pUart->m_TxBufNode.m_RemainDataSize > 0 )
	{
		if(Inp32(&pUartRegs->rUtrStat)&0x4) 
		{
			Outp8(&pUartRegs->rUtxh, *pUart->m_TxBufNode.m_pCurPtr++);
			pUart->m_TxBufNode.m_RemainDataSize--;
			if(pUart->m_TxBufNode.m_RemainDataSize == 0)
			{
				break;
			}
		}

		if(UART_GetKey())
		{
			break;
		}
	}
}



void UART_PollingRxString(UART_CON *pUart)
{
	volatile UART_REGS *pUartRegs;
	
	pUartRegs = pUart->m_pUartSFRBase;

	while( pUart->m_RxBufNode.m_RemainDataSize > 0 )
	{
		if(Inp32(&pUartRegs->rUtrStat)&0x1) 
		{
			*pUart->m_RxBufNode.m_pCurPtr++ = Inp8(&pUartRegs->rUrxh);
			pUart->m_RxBufNode.m_RemainDataSize--;

			if(pUart->m_RxBufNode.m_RemainDataSize == 0)
			{
				break;
			}
		}

		if(UART_GetKey())
		{
			break;
		}
	}
}


void UART_PollingTxWithManualCTSString(UART_CON *pUart)
{
	volatile UART_REGS *pUartRegs;
	
	pUartRegs = pUart->m_pUartSFRBase;


	while(pUart->m_TxBufNode.m_RemainDataSize > 0 )
	{
		if(Inp32(&pUartRegs->rUmStat)&0x1)		/* Check CTS signal */
		{
			if(Inp32(&pUartRegs->rUtrStat)&0x4) 
			{
				Outp8(&pUartRegs->rUtxh, *pUart->m_TxBufNode.m_pCurPtr++);
				pUart->m_TxBufNode.m_RemainDataSize--;
				if(pUart->m_TxBufNode.m_RemainDataSize == 0)
				{
					break;
				}
			}
		}

		if(UART_GetKey())
		{
			break;
		}
	}
}

void UART_SendBreakSignal(UART_CON *pUartCon)
{
	
	u32 uTemp;
	volatile UART_REGS *pUartRegs;

	pUartRegs = pUartCon->m_pUartSFRBase;

	uTemp = Inp32(&pUartRegs->rUCon);
	uTemp |= (1<<4);						// Send Break signal
	Outp32(&pUartRegs->rUCon,uTemp);
		
}


void UART_WrRTS(UART_CON *pUartCon, u32 en)
{
	volatile UART_REGS *pUartRegs;

	pUartRegs = pUartCon->m_pUartSFRBase;

	Outp32(&pUartRegs->rUmCon,en);
}

bool UART_IsInterruptPrintValid(void)
{
	return gUART_CtrlInfo.m_EnPrintInISR;
}


void UART_PollingRxWithManualRTSString(UART_CON *pUart)
{
	volatile UART_REGS *pUartRegs;
	
	pUartRegs = pUart->m_pUartSFRBase;

	while( pUart->m_RxBufNode.m_RemainDataSize > 0 )
	{
		UART_WrRTS(pUart, RTS_Act);			/* Enbale RTS */
		//UART_WrRTS(pUart, RTS_inAct);		/* Disable RTS */
		if(Inp32(&pUartRegs->rUtrStat)&0x1) 
		{
			*pUart->m_RxBufNode.m_pCurPtr++ = Inp8(&pUartRegs->rUrxh);
			pUart->m_RxBufNode.m_RemainDataSize--;

			if(pUart->m_RxBufNode.m_RemainDataSize == 0)
			{
				break;
			}
		}

		//UART_WrRTS(pUart, RTS_Act);			/* Enbale RTS */
		UART_WrRTS(pUart, RTS_inAct);		/* Disable RTS */

		if(UART_GetKey())
		{
			break;
		}
	}
}


void UART_LoopbackPollingTxRxString(UART_CON *pUartTx, UART_CON *pUartRx)
{
	volatile UART_REGS *pUartTxRegs;
	volatile UART_REGS *pUartRxRegs;
	bool TxDone;
	bool RxDone;
	
	pUartTxRegs = pUartTx->m_pUartSFRBase;
	pUartRxRegs = pUartRx->m_pUartSFRBase;

	TxDone = FALSE;
	RxDone = FALSE;

	while( !(TxDone == TRUE && RxDone == TRUE) )
	{
		if(TxDone != TRUE)
		{
			if(Inp32(&pUartTxRegs->rUtrStat)&0x4) 
			{
				Outp8(&pUartTxRegs->rUtxh, *pUartTx->m_TxBufNode.m_pCurPtr++);
				pUartTx->m_TxBufNode.m_RemainDataSize--;
				if(pUartTx->m_TxBufNode.m_RemainDataSize == 0)
				{
					TxDone = TRUE;
				}
			}
		}

		if(RxDone != TRUE)
		{

			if(Inp32(&pUartRxRegs->rUtrStat)&0x1) 
			{
				*pUartRx->m_RxBufNode.m_pCurPtr++ = Inp8(&pUartRxRegs->rUrxh);
				pUartRx->m_RxBufNode.m_RemainDataSize--;

				if(pUartRx->m_RxBufNode.m_RemainDataSize == 0)
				{
					RxDone = TRUE;
				}
			}
		}

		if(UART_GetKey())
		{
			break;
		}
	}
}


void UART_LoopbackInterruptTxRxString(UART_CON *pUartCon, 
	void (*fnTxComplete)(u32), u32 IsrTxVar, void (*fnRxComplete)(u32), u32 IsrRxVar)
{
	volatile UART_REGS *pUartRegs;
	u8 cTemp;
	u32 uTemp2;
	u32 uDstAddr;
	u32	IntMaskBit;

	pUartRegs = pUartCon->m_pUartSFRBase;

	/* Set UART Interrupt */
	pUartCon->m_EnTxdIntMask = FALSE;
	pUartCon->m_EnErrorIntMask = FALSE;
	pUartCon->m_EnRxdIntMask = FALSE;
	
	IntMaskBit = (pUartCon->m_EnModemIntMask << 3) | (pUartCon->m_EnTxdIntMask << 2) | 
		(pUartCon->m_EnErrorIntMask << 1) |(pUartCon->m_EnRxdIntMask << 0);

	uTemp2 = Inp32(&pUartRegs->rUintM);
	uTemp2 &= ~0xF;
	uTemp2 |= IntMaskBit;
	Outp32(&pUartRegs->rUintM, uTemp2);

	UART_SetIsrFunc(pUartCon, UART_INT_TX, fnTxComplete, IsrTxVar);
	UART_SetIsrFunc(pUartCon, UART_INT_RX, fnRxComplete, IsrRxVar);

	INTC_Enable(pUartCon->m_IntNum);
}



void UART_InterruptTxString(UART_CON *pUartCon, void (*fnTxComplete)(u32), u32 IsrVar)
{
	volatile UART_REGS *pUartRegs;
	u8 cTemp;
	u32 uTemp2;
	u32 uDstAddr;
	u32	IntMaskBit;

	pUartRegs = pUartCon->m_pUartSFRBase;

	/* Set UART Interrupt */
	pUartCon->m_EnTxdIntMask = FALSE;
	pUartCon->m_EnErrorIntMask = FALSE;
	
	IntMaskBit = (pUartCon->m_EnModemIntMask << 3) | (pUartCon->m_EnTxdIntMask << 2) | 
		(pUartCon->m_EnErrorIntMask << 1) |(pUartCon->m_EnRxdIntMask << 0);

	uTemp2 = Inp32(&pUartRegs->rUintM);
	uTemp2 &= ~0xF;
	uTemp2 |= IntMaskBit;
	Outp32(&pUartRegs->rUintM, uTemp2);

	UART_SetIsrFunc(pUartCon, UART_INT_TX, fnTxComplete, IsrVar);

	INTC_Enable(pUartCon->m_IntNum);
}



u8* UART_InterruptRxString(UART_CON *pUartCon, void (*fnRxComplete)(u32), u32 IsrVar)
{
	volatile UART_REGS *pUartRegs;
	u8 cTemp;
	u32 uTemp2;
	u32 uSrcAddr;
	u32	IntMaskBit;

	pUartRegs = pUartCon->m_pUartSFRBase;

	/* Set UART Interrupt */
	pUartCon->m_EnErrorIntMask = FALSE;
	pUartCon->m_EnRxdIntMask = FALSE;
	
	IntMaskBit = (pUartCon->m_EnModemIntMask << 3) | (pUartCon->m_EnTxdIntMask << 2) | 
		(pUartCon->m_EnErrorIntMask << 1) |(pUartCon->m_EnRxdIntMask << 0);

	uTemp2 = Inp32(&pUartRegs->rUintM);
	uTemp2 &= ~0xF;
	uTemp2 |= IntMaskBit;
	Outp32(&pUartRegs->rUintM,uTemp2);

	UART_SetIsrFunc(pUartCon, UART_INT_RX, fnRxComplete, IsrVar);

	INTC_Enable(pUartCon->m_IntNum);
}


void UART_LoopbackDMATxRxString(UART_CON *pUartCon, 
	void (*fnTxComplete)(u32), u32 IsrTxVar, void (*fnRxComplete)(u32), u32 IsrRxVar)
{
	volatile UART_REGS *pUartRegs;
	u8 cTemp;
	u32 uTemp2;
	u32 uSrcAddr;
	u32 uDstAddr;
	u32	IntMaskBit;

	pUartRegs = pUartCon->m_pUartSFRBase;

	/* Set UART Interrupt */
	pUartCon->m_EnErrorIntMask = FALSE;
	pUartCon->m_EnRxdIntMask = FALSE;
	
	IntMaskBit = (pUartCon->m_EnModemIntMask << 3) | (pUartCon->m_EnTxdIntMask << 2) | 
		(pUartCon->m_EnErrorIntMask << 1) |(pUartCon->m_EnRxdIntMask << 0);

	uTemp2 = Inp32(&pUartRegs->rUintM);
	uTemp2 &= ~0xF;
	uTemp2 |= IntMaskBit;
	Outp32(&pUartRegs->rUintM, uTemp2);

	UART_SetIsrFunc(pUartCon, UART_INT_RX, fnRxComplete, IsrRxVar);	/* for treating Trailing Bytes. */

	INTC_Enable(pUartCon->m_IntNum);


	/* Set DMA Rx */
	
	uSrcAddr = (u32)(&pUartRegs->rUrxh);
	
	DMA_SetCh(pUartCon->m_UartRxDmaCh, &pUartCon->m_UartRxDma);
	DMA_InitCh(BYTE, pUartCon->m_UartRxDmaReqSrc, DMA_P2M, pUartCon->m_RxDMABurstSize,
		&pUartCon->m_UartRxDma);

	UART_SetIsrFunc(pUartCon, UART_INT_DMA_RX, fnRxComplete, IsrRxVar);

	DMA_StartCh(uSrcAddr, (u32)pUartCon->m_RxBufNode.m_pCurPtr, pUartCon->m_RxBufNode.m_ReqDataSize, 
		&pUartCon->m_UartRxDma);
		
	/* Set DMA Tx */
	uDstAddr = (u32)(&pUartRegs->rUtxh);
	
	DMA_SetCh(pUartCon->m_UartTxDmaCh, &pUartCon->m_UartTxDma);
	DMA_InitCh(BYTE, pUartCon->m_UartTxDmaReqSrc, DMA_M2P, pUartCon->m_TxDMABurstSize,
		&pUartCon->m_UartTxDma);

	UART_SetIsrFunc(pUartCon, UART_INT_DMA_TX, fnTxComplete, IsrTxVar);

	DMA_StartCh((u32)pUartCon->m_TxBufNode.m_pCurPtr, uDstAddr, pUartCon->m_TxBufNode.m_ReqDataSize, 
		&pUartCon->m_UartTxDma);
}



void UART_DMATxString(UART_CON *pUartCon, void (*fnTxComplete)(u32), u32 IsrVar)
{
	volatile UART_REGS *pUartRegs;
	u8 cTemp;
	u32 uTemp2;
	u32 uDstAddr;
	u32	IntMaskBit;

	pUartRegs = pUartCon->m_pUartSFRBase;

	/* Set UART Interrupt */
	pUartCon->m_EnErrorIntMask = FALSE;
	
	IntMaskBit = (pUartCon->m_EnModemIntMask << 3) | (pUartCon->m_EnTxdIntMask << 2) | 
		(pUartCon->m_EnErrorIntMask << 1) |(pUartCon->m_EnRxdIntMask << 0);

	uTemp2 = Inp32(&pUartRegs->rUintM);
	uTemp2 &= ~0xF;
	uTemp2 |= IntMaskBit;
	Outp32(&pUartRegs->rUintM, uTemp2);

	INTC_Enable(pUartCon->m_IntNum);


	/* Set DMA */
	
	uDstAddr = (u32)(&pUartRegs->rUtxh);
	
	DMA_SetCh(pUartCon->m_UartTxDmaCh, &pUartCon->m_UartTxDma);
	DMA_InitCh(BYTE, pUartCon->m_UartTxDmaReqSrc, DMA_M2P, pUartCon->m_TxDMABurstSize,
		&pUartCon->m_UartTxDma);

	UART_SetIsrFunc(pUartCon, UART_INT_DMA_TX, fnTxComplete, IsrVar);

	DMA_StartCh((u32)pUartCon->m_TxBufNode.m_pCurPtr, uDstAddr, pUartCon->m_TxBufNode.m_ReqDataSize, 
		&pUartCon->m_UartTxDma);
}


u8* UART_DMARxString(UART_CON *pUartCon, void (*fnRxComplete)(u32), u32 IsrVar)
{
	volatile UART_REGS *pUartRegs;
	u8 cTemp;
	u32 uTemp2;
	u32 uSrcAddr;
	u32	IntMaskBit;

	pUartRegs = pUartCon->m_pUartSFRBase;

	/* Set UART Interrupt */
	pUartCon->m_EnErrorIntMask = FALSE;
	pUartCon->m_EnRxdIntMask = FALSE;
	
	IntMaskBit = (pUartCon->m_EnModemIntMask << 3) | (pUartCon->m_EnTxdIntMask << 2) | 
		(pUartCon->m_EnErrorIntMask << 1) |(pUartCon->m_EnRxdIntMask << 0);

	uTemp2 = Inp32(&pUartRegs->rUintM);
	uTemp2 &= ~0xF;
	uTemp2 |= IntMaskBit;
	Outp32(&pUartRegs->rUintM,uTemp2);

	UART_SetIsrFunc(pUartCon, UART_INT_RX, fnRxComplete, IsrVar);	/* for treating Trailing Bytes. */

	INTC_Enable(pUartCon->m_IntNum);


	/* Set DMA */
	
	uSrcAddr = (u32)(&pUartRegs->rUrxh);
	
	DMA_SetCh(pUartCon->m_UartRxDmaCh, &pUartCon->m_UartRxDma);
	DMA_InitCh(BYTE, pUartCon->m_UartRxDmaReqSrc, DMA_P2M, pUartCon->m_RxDMABurstSize,
		&pUartCon->m_UartRxDma);

	UART_SetIsrFunc(pUartCon, UART_INT_DMA_RX, fnRxComplete, IsrVar);

	DMA_StartCh(uSrcAddr, (u32)pUartCon->m_RxBufNode.m_pCurPtr, pUartCon->m_RxBufNode.m_ReqDataSize, 
		&pUartCon->m_UartRxDma);
}


void UART_DisplayVar_InitNCBuf(UART_CON *pUartCon)
{
	UART_Printf("Init Non-Cache Buffer : Start(0x%08x), Cur(0x%08x), Size(%.2f MB)",
		gUART_CtrlInfo.m_NCBufCtrlInfo.m_StartMemAddr, gUART_CtrlInfo.m_NCBufCtrlInfo.m_CurMemAddr,
		(float)gUART_CtrlInfo.m_NCBufCtrlInfo.m_TotalSize/(1024*1024));
}

void UART_DisplayVar_ChangeDebugChannel(UART_CON *pUartCon)
{
	UART_Printf("Change Debug Channel : UART%d (%d bps)", UART_GetCurDebugCh(), pUartCon->m_Baudrate);
}

void UART_DisplayVar_EnUart2CtsRts(UART_CON *pUartCon)
{
	UART_Printf("Enable UART2 CTS/RTS : %s\n", gUART_CtrlInfo.m_EnUart2CtsRts ? "ON" : "OFF");
}

void UART_DisplayVar_EnInterruptPrint(UART_CON *pUartCon)
{
	UART_Printf("Enable Interrupt Print : %s\n", gUART_CtrlInfo.m_EnPrintInISR ? "ON" : "OFF");
}


void UART_DisplayVar_LoadDefault(UART_CON *pUartCon)
{
	UART_Printf("Load Default Setting...");
}

void UART_DisplayVar_SWReset(UART_CON *pUartCon)
{
	UART_Printf("FIFO Reset");
}

void UART_DisplayVar_UartOpen(UART_CON *pUartCon)
{
	UART_Printf("UART%d Open", pUartCon->m_UartChIdx);
}

#if 0
void UART_DisplayVar_ReadRegister(UART_CON *pUartCon)
{
	UART_Printf("Read Register");
}
#endif /* 0 */

void UART_DisplayVar_ReadWriteRegister(UART_CON *pUartCon)
{
	UART_Printf("Read/Write Register");
}

void UART_DisplayVar_Baudrate(UART_CON *pUartCon)
{
	UART_Printf("BaudRate : %d bps", pUartCon->m_Baudrate);
}

void UART_DisplayVar_IrDAMode(UART_CON *pUartCon)
{
	if(pUartCon->m_EnIRDAMode == TRUE)
	{
		UART_Printf("IrDA : ON");
	}
	else
	{
		UART_Printf("IrDA : OFF");
	}
}

void UART_DisplayVar_InteranlLoopbackMode(UART_CON *pUartCon)
{
	if(pUartCon->m_EnLoopBackMode == TRUE)
	{
		UART_Printf("Internal Loopback : ON");
	}
	else
	{
		UART_Printf("Internal Loopback : OFF");
	}
}

void UART_DisplayVar_AFCMode(UART_CON *pUartCon)
{
	if(pUartCon->m_EnAFC == TRUE)
	{
		UART_Printf("AFC(Auto Flow Control) : ON");
	}
	else
	{
		UART_Printf("AFC(Auto Flow Control) : OFF");
	}
}

void UART_DisplayVar_FIFOMode(UART_CON *pUartCon)
{
	if(pUartCon->m_EnFifo == TRUE)
	{
		UART_Printf("FIFO Mode : ON");
	}
	else
	{
		UART_Printf("FIFO Mode : OFF");
	}
}


void UART_DisplayVar_TxDMABurstSize(UART_CON *pUartCon)
{
	switch(pUartCon->m_TxDMABurstSize)
	{
		case UartDMA_1B:
			UART_Printf("TxDMABurst : DMA_1B");
			break;
		case UartDMA_4B:
			UART_Printf("TxDMABurst : DMA_4B");
			break;
		case UartDMA_8B:
			UART_Printf("TxDMABurst : DMA_8B");
			break;
		case UartDMA_16B:
			UART_Printf("TxDMABurst : DMA_16B");
			break;
		case UartDMA_32B:
			UART_Printf("TxDMABurst : DMA_32B");
			break;
		case UartDMA_64B:
			UART_Printf("TxDMABurst : DMA_64B");
			break;
		case UartDMA_128B:
			UART_Printf("TxDMABurst : DMA_128B");
			break;
		case UartDMA_256B:
			UART_Printf("TxDMABurst : DMA_256B");
			break;
		default:
			UART_ASSERT();
			break;
	}
}

void UART_DisplayVar_RxDMABurstSize(UART_CON *pUartCon)
{
	switch(pUartCon->m_RxDMABurstSize)
	{
		case UartDMA_1B:
			UART_Printf("RxDMABurst : DMA_1B");
			break;
		case UartDMA_4B:
			UART_Printf("RxDMABurst : DMA_4B");
			break;
		case UartDMA_8B:
			UART_Printf("RxDMABurst : DMA_8B");
			break;
		case UartDMA_16B:
			UART_Printf("RxDMABurst : DMA_16B");
			break;
		case UartDMA_32B:
			UART_Printf("RxDMABurst : DMA_32B");
			break;
		case UartDMA_64B:
			UART_Printf("RxDMABurst : DMA_64B");
			break;
		case UartDMA_128B:
			UART_Printf("RxDMABurst : DMA_128B");
			break;
		case UartDMA_256B:
			UART_Printf("RxDMABurst : DMA_256B");
			break;
		default:
			UART_ASSERT();
			break;
	}
}


void UART_DisplayVar_DataBitSize(UART_CON *pUartCon)
{
	switch(pUartCon->m_DataBit)
	{
		case UartWL_5bit:
			UART_Printf("Data bit : 5bit");
			break;
		case UartWL_6bit:
			UART_Printf("Data bit : 6bit");
			break;
		case UartWL_7bit:
			UART_Printf("Data bit : 7bit");
			break;
		case UartWL_8bit:
			UART_Printf("Data bit : 8bit");
			break;
		default:
			UART_ASSERT();
			break;
	}
}

void UART_DisplayVar_ParityBitSize(UART_CON *pUartCon)
{
	switch(pUartCon->m_ParityBit)
	{
		case UartParity_None:
			UART_Printf("Parity bit : NONE");
			break;
		case UartParity_Odd:
			UART_Printf("Parity bit : Odd");
			break;
		case UartParity_Even:
			UART_Printf("Parity bit : Even");
			break;
		case UartParity_Force1:
			UART_Printf("Parity bit : Force1");
			break;
		case UartParity_Force0:
			UART_Printf("Parity bit : Force0");
			break;
		default:
			UART_ASSERT();
			break;
	}
}


void UART_DisplayVar_StopBitSize(UART_CON *pUartCon)
{
	switch(pUartCon->m_StopBit)
	{
		case UartStop_1bitStop:
			UART_Printf("Stop bit : 1 Stop");
			break;
		case UartStop_2bitStop:
			UART_Printf("Stop bit : 2 Stop");
			break;
		default:
			UART_ASSERT();
			break;
	}
}

void UART_DisplayVar_TxMode(UART_CON *pUartCon)
{
	switch(pUartCon->m_TxMode)
	{
		case UartMode_Disable:
			UART_Printf("Tx Mode : Disable");
			break;
		case UartMode_Interrupt:
			UART_Printf("Tx Mode : Interrupt Mode");
			break;
		case UartMode_DMA:
			UART_Printf("Tx Mode : DMA Mode");
			break;
		default:
			UART_ASSERT();
			break;
	}
}

void UART_DisplayVar_RxMode(UART_CON *pUartCon)
{
	switch(pUartCon->m_RxMode)
	{
		case UartMode_Disable:
			UART_Printf("Rx Mode : Disable");
			break;
		case UartMode_Interrupt:
			UART_Printf("Rx Mode : Interrupt Mode");
			break;
		case UartMode_DMA:
			UART_Printf("Rx Mode : DMA Mode");
			break;
		default:
			UART_ASSERT();
			break;
	}
}

void UART_DisplayVar_TxTrigLevel(UART_CON *pUartCon)
{
	switch(pUartCon->m_TxTrigLvl)
	{
		case UartFIFO_1B:
			UART_Printf("Tx Trigger Level : FIFO_1B");
			break;
		case UartFIFO_4B:
			UART_Printf("Tx Trigger Level : FIFO_4B");
			break;
		case UartFIFO_8B:
			UART_Printf("Tx Trigger Level : FIFO_8B");
			break;
		case UartFIFO_16B:
			UART_Printf("Tx Trigger Level : FIFO_16B");
			break;
		case UartFIFO_32B:
			UART_Printf("Tx Trigger Level : FIFO_32B");
			break;
		case UartFIFO_64B:
			UART_Printf("Tx Trigger Level : FIFO_64B");
			break;
		case UartFIFO_128B:
			UART_Printf("Tx Trigger Level : FIFO_128B");
			break;
		case UartFIFO_256B:
			UART_Printf("Tx Trigger Level : FIFO_256B");
			break;
		default:
			UART_ASSERT();
			break;
	}
}

void UART_DisplayVar_RxTrigLevel(UART_CON *pUartCon)
{
	switch(pUartCon->m_RxTrigLvl)
	{
		case UartFIFO_1B:
			UART_Printf("Rx Trigger Level : FIFO_1B");
			break;
		case UartFIFO_4B:
			UART_Printf("Rx Trigger Level : FIFO_4B");
			break;
		case UartFIFO_8B:
			UART_Printf("Rx Trigger Level : FIFO_8B");
			break;
		case UartFIFO_16B:
			UART_Printf("Rx Trigger Level : FIFO_16B");
			break;
		case UartFIFO_32B:
			UART_Printf("Rx Trigger Level : FIFO_32B");
			break;
		case UartFIFO_64B:
			UART_Printf("Rx Trigger Level : FIFO_64B");
			break;
		case UartFIFO_128B:
			UART_Printf("Rx Trigger Level : FIFO_128B");
			break;
		case UartFIFO_256B:
			UART_Printf("Rx Trigger Level : FIFO_256B");
			break;
		default:
			UART_ASSERT();
			break;
	}
}

void UART_DisplayVar_RtsTrigLevel(UART_CON *pUartCon)
{
	switch(pUartCon->m_RtsTrigLvl)
	{
		case UartFIFO_255_63_15B:
			UART_Printf("RTS Triger Level : 255/63/15B");
			break;
		case UartFIFO_224_56_14B:
			UART_Printf("RTS Triger Level : 224/56/14B");
			break;
		case UartFIFO_192_48_12B:
			UART_Printf("RTS Triger Level : 192/48/12B");
			break;
		case UartFIFO_160_40_10B:
			UART_Printf("RTS Triger Level : 160/40/10B");
			break;
		case UartFIFO_128_32_8B:
			UART_Printf("RTS Triger Level : 128/32/8B");
			break;
		case UartFIFO_96_24_6B:
			UART_Printf("RTS Triger Level : 96/24/6B");
			break;
		case UartFIFO_64_16_4B:
			UART_Printf("RTS Triger Level : 64/16/4B");
			break;
		case UartFIFO_32_8_2B:
			UART_Printf("RTS Triger Level : 32/8/2B");
			break;
		default:
			UART_ASSERT();
			break;
	}
}

void UART_DisplayVar_DrvStrength(UART_CON *pUartCon)
{
	UART_Printf("Drv Strength : %d", pUartCon->m_DrvStrength);
}

void UART_DisplayVar_EnModemIntMask(UART_CON *pUartCon)
{
	if(pUartCon->m_EnModemIntMask == TRUE)
	{
		UART_Printf("Modem Interrupt Mask : SET");
	}
	else
	{
		UART_Printf("Modem Interrupt Mask : CLEAR");
	}
}

void UART_DisplayVar_EnTxdIntMask(UART_CON *pUartCon)
{
	if(pUartCon->m_EnTxdIntMask == TRUE)
	{
		UART_Printf("Tx Interrupt Mask : SET");
	}
	else
	{
		UART_Printf("Tx Interrupt Mask : CLEAR");
	}
}

void UART_DisplayVar_EnErrorIntMask(UART_CON *pUartCon)
{
	if(pUartCon->m_EnErrorIntMask == TRUE)
	{
		UART_Printf("Error Interrupt Mask : SET");
	}
	else
	{
		UART_Printf("Error Interrupt Mask : CLEAR");
	}
}

void UART_DisplayVar_EnRxdIntMask(UART_CON *pUartCon)
{
	if(pUartCon->m_EnRxdIntMask == TRUE)
	{
		UART_Printf("Rx Interrupt Mask : SET");
	}
	else
	{
		UART_Printf("Rx Interrupt Mask : CLEAR");
	}
}

void UART_DisplayVar_RxTimeoutMode(UART_CON *pUartCon)
{
	if(pUartCon->m_EnRxTimeout == TRUE)
	{
		UART_Printf("Rx Timeout : ON");
	}
	else
	{
		UART_Printf("Rx Timeout : OFF");
	}
}

void UART_DisplayVar_TxTransferSize(UART_CON *pUartCon)
{
	UART_Printf("Tx Data Transfer Size : %d byte(s)", pUartCon->m_TxTransferSize);
}


void UART_DisplayVar_RxTransferSize(UART_CON *pUartCon)
{
	UART_Printf("Rx Data Transfer Size : %d byte(s)", pUartCon->m_RxTransferSize);
}

#if 0
void UART_DisplayVar_DebugChannel(UART_CON *pUartCon)
{
	if(pUartCon->m_IsDebugChannel == TRUE)
	{
		UART_Printf("UART Usage : Debug");
	}
	else
	{
		UART_Printf("UART Usage : Normal(Not for Debug)");
	}
}
#endif /* 0 */

void UART_DisplayVar_ExtClkMode(UART_CON *pUartCon)
{
	if(pUartCon->m_IsUseExtClk == TRUE)
	{
		UART_Printf("Clock Selection : SCLK_UART");
	}
	else
	{
		UART_Printf("Clock Selection : PCLK");
	}
}

void UART_DisplayVar_ExtClkSource(UART_CON *pUartCon)
{
	switch(pUartCon->m_ExtClkSrc)
	{
		case UART_XXTI:
			UART_Printf("EXT_CLK_SRC : XTI");
			break;
		case UART_XUSBXTI:
			UART_Printf("EXT_CLK_SRC : USBXTI");
			break;
		case UART_SHDMI27M:
			UART_Printf("EXT_CLK_SRC : HDMI27M");
			break;
		case UART_SUSBPHY0:
			UART_Printf("EXT_CLK_SRC : PHY0");
			break;
		case UART_SUSBPHY1:
			UART_Printf("EXT_CLK_SRC : PHY1");
			break;
		case UART_SHDMIPHY:
			UART_Printf("EXT_CLK_SRC : HDMIPHY");
			break;
		case UART_SMPLL:
			UART_Printf("EXT_CLK_SRC : MPLL(%.2fMHz)", (float)g_uMPLL/1.0e6);
			break;
		case UART_SEPLL:
			UART_Printf("EXT_CLK_SRC : EPLL(%.2fMHz)", (float)g_uEPLL/1.0e6);
			break;
		case UART_SVPLL:
			UART_Printf("EXT_CLK_SRC : VPLL(%.2fMHz)", (float)g_uVPLL/1.0e6);
			break;
		default:
			UART_ASSERT();
			break;
	}
}


void UART_DisplayVar_ExtClkSourceDiv(UART_CON *pUartCon)
{
	UART_Printf("EXT_CLK_SRC DIV (from 1) : %d", pUartCon->m_ExtClkSrcDiv);
}


void UART_DisplayVar_CurClkFrequency(UART_CON *pUartCon)
{
	u32			OpClock;
	u32			DivValue;
	u32			Slot;
	
	UART_GetOpClock(pUartCon->m_IsUseExtClk, pUartCon->m_Baudrate, pUartCon->m_ExtClkSrc, 
		pUartCon->m_ExtClkSrcDiv, &OpClock, &DivValue, &Slot);
	UART_Printf("OpClock(%.2fMHz), DIV_VAL(%d), Slot(0x%x)",
		(float)OpClock/1.0e6, DivValue, Slot);
}


void UART_DisplayVar_CheckTxEndByNull(UART_CON *pUartCon)
{
	if(pUartCon->m_CheckTxEndbyNull == TRUE)
	{
		UART_Printf("Check Tx End by NULL : ON");
	}
	else
	{
		UART_Printf("Check Tx End by NULL : OFF");
	}
}


void UART_DisplayVar_CheckRxEndByCR(UART_CON *pUartCon)
{
	if(pUartCon->m_CheckRxEndbyCR == TRUE)
	{
		UART_Printf("Check Rx End by CR : ON");
	}
	else
	{
		UART_Printf("Check Rx End by CR : OFF");
	}
}


void UART_DisplayVar_TxDMANum(UART_CON *pUartCon)
{
	switch(pUartCon->m_UartTxDMANum)
	{
		case UART_DMA0:
			UART_Printf("Tx DMA : PDMA_0");
			break;
		case UART_DMA1:
			UART_Printf("Tx DMA : PDMA_1");
			break;
		default:
			UART_ASSERT();
			break;
	}
}


void UART_DisplayVar_RxDMANum(UART_CON *pUartCon)
{
	switch(pUartCon->m_UartRxDMANum)
	{
		case UART_DMA0:
			UART_Printf("Rx DMA : PDMA_0");
			break;
		case UART_DMA1:
			UART_Printf("Rx DMA : PDMA_1");
			break;
		default:
			UART_ASSERT();
			break;
	}
}



void UART_DisplayVar_TxDMACh(UART_CON *pUartCon)
{
	switch(pUartCon->m_UartTxDmaCh)
	{
		case DMA_00:
			UART_Printf("Tx DMA_CH : DMA_00");
			break;
		case DMA_01:
			UART_Printf("Tx DMA_CH : DMA_01");
			break;
		case DMA_02:
			UART_Printf("Tx DMA_CH : DMA_02");
			break;
		case DMA_03:
			UART_Printf("Tx DMA_CH : DMA_03");
			break;
		case DMA_04:
			UART_Printf("Tx DMA_CH : DMA_04");
			break;
		case DMA_05:
			UART_Printf("Tx DMA_CH : DMA_05");
			break;
		case DMA_06:
			UART_Printf("Tx DMA_CH : DMA_06");
			break;
		case DMA_07:
			UART_Printf("Tx DMA_CH : DMA_07");
			break;
		case DMA_10:
			UART_Printf("Tx DMA_CH : DMA_10");
			break;
		case DMA_11:
			UART_Printf("Tx DMA_CH : DMA_11");
			break;
		case DMA_12:
			UART_Printf("Tx DMA_CH : DMA_12");
			break;
		case DMA_13:
			UART_Printf("Tx DMA_CH : DMA_13");
			break;
		case DMA_14:
			UART_Printf("Tx DMA_CH : DMA_14");
			break;
		case DMA_15:
			UART_Printf("Tx DMA_CH : DMA_15");
			break;
		case DMA_16:
			UART_Printf("Tx DMA_CH : DMA_16");
			break;
		case DMA_17:
			UART_Printf("Tx DMA_CH : DMA_17");
			break;
		default:
			UART_ASSERT();
			break;
	}
}


void UART_DisplayVar_RxDMACh(UART_CON *pUartCon)
{
	switch(pUartCon->m_UartRxDmaCh)
	{
		case DMA_00:
			UART_Printf("Rx DMA_CH : DMA_00");
			break;
		case DMA_01:
			UART_Printf("Rx DMA_CH : DMA_01");
			break;
		case DMA_02:
			UART_Printf("Rx DMA_CH : DMA_02");
			break;
		case DMA_03:
			UART_Printf("Rx DMA_CH : DMA_03");
			break;
		case DMA_04:
			UART_Printf("Rx DMA_CH : DMA_04");
			break;
		case DMA_05:
			UART_Printf("Rx DMA_CH : DMA_05");
			break;
		case DMA_06:
			UART_Printf("Rx DMA_CH : DMA_06");
			break;
		case DMA_07:
			UART_Printf("Rx DMA_CH : DMA_07");
			break;
		case DMA_10:
			UART_Printf("Rx DMA_CH : DMA_10");
			break;
		case DMA_11:
			UART_Printf("Rx DMA_CH : DMA_11");
			break;
		case DMA_12:
			UART_Printf("Rx DMA_CH : DMA_12");
			break;
		case DMA_13:
			UART_Printf("Rx DMA_CH : DMA_13");
			break;
		case DMA_14:
			UART_Printf("Rx DMA_CH : DMA_14");
			break;
		case DMA_15:
			UART_Printf("Rx DMA_CH : DMA_15");
			break;
		case DMA_16:
			UART_Printf("Rx DMA_CH : DMA_16");
			break;
		case DMA_17:
			UART_Printf("Rx DMA_CH : DMA_17");
			break;
		default:
			UART_ASSERT();
			break;
	}
}



void UART_SetVar_InitNCBuf(UART_CON *pUartCon)
{
	UART_ResetNCBufCtrl(&gUART_CtrlInfo.m_NCBufCtrlInfo);
}

void UART_SetVar_ChangeDebugChannel(UART_CON *pUartCon)
{
	s32		SelNum;
	s32		Status;
	u32		BaudRate;
	u8		ch;

	UART_Printf("\nSelect Debug Channel : Cur(UART%d)", UART_GetCurDebugCh());
	UART_Printf("\n");

	UART_Printf("\n0.UART0 ");
	UART_Printf("\n1.UART1 ");
	UART_Printf("\n2.UART2 ");
	UART_Printf("\n3.UART3 ");
	UART_Printf("\n");

	UART_Printf("\nChoose : ");

	SelNum = UART_GetIntNum();
	if(SelNum < 0 || SelNum > 3)
	{
		return;
	}

	ch = SelNum;

	UART_Printf("\nBaudRate (ex 9600, 115200[D], 843750) : ");

	SelNum = UART_GetIntNum();
	if(SelNum < 0)
	{
		SelNum = 115200;
	}

	BaudRate = SelNum;

	UART_InitDebugCh(ch, BaudRate);
}

void UART_SetVar_EnUart2CtsRts(UART_CON *pUartCon)
{
	s32				SelNum;

	UART_Printf("\nEnable UART2 CTS/RTS : ");
	UART_Printf("\n");

	UART_Printf("\n0.OFF(D)");
	UART_Printf("\n1.ON");
	UART_Printf("\n");

	UART_Printf("\nChoose : ");

	SelNum = UART_GetIntNum();
	if(SelNum < 0 || SelNum > 1)
	{
		SelNum = 0;
	}

	gUART_CtrlInfo.m_EnUart2CtsRts = SelNum;
}


void UART_SetVar_EnInterruptPrint(UART_CON *pUartCon)
{
	s32				SelNum;

	UART_Printf("\nEnable Interrupt Print : ");
	UART_Printf("\n");

	UART_Printf("\n0.OFF");
	UART_Printf("\n1.ON(D)");
	UART_Printf("\n");

	UART_Printf("\nChoose : ");

	SelNum = UART_GetIntNum();
	if(SelNum < 0 || SelNum > 1)
	{
		SelNum = 1;
	}

	gUART_CtrlInfo.m_EnPrintInISR = SelNum;
}


void UART_SetVar_LoadDefault(UART_CON *pUartCon)
{
	s32				SelNum;

	UART_Printf("\nAre you sure? : ");
	UART_Printf("\n");

	UART_Printf("\n0.NO(D)");
	UART_Printf("\n1.YES");
	UART_Printf("\n");

	UART_Printf("\nChoose : ");

	SelNum = UART_GetIntNum();
	if(SelNum < 0 || SelNum > 1)
	{
		SelNum = 0;
	}

	if(SelNum == 0)
	{
		/* Do nothing */
		
		return;
	}

	switch(pUartCon->m_UartChIdx)
	{
		case UartCh_CH0:
			*pUartCon = gUART_CtrlInfo.m_UartChDefaultInfo[UartCh_CH0];
			break;
			
		case UartCh_CH1:
			*pUartCon = gUART_CtrlInfo.m_UartChDefaultInfo[UartCh_CH1];
			break;

		case UartCh_CH2:
			*pUartCon = gUART_CtrlInfo.m_UartChDefaultInfo[UartCh_CH2];
			break;

		case UartCh_CH3:
			*pUartCon = gUART_CtrlInfo.m_UartChDefaultInfo[UartCh_CH3];
			break;

		default:
			UART_ASSERT();
			break;
	}
}


void UART_SetVar_SWReset(UART_CON *pUartCon)
{
	UART_REGS	*pUartRegs;
	u32			TempReg;

	pUartRegs = pUartCon->m_pUartSFRBase;

	TempReg = pUartRegs->rUfCon;
	TempReg |= TX_FIFO_RESET|RX_FIFO_RESET;
	
	/* FIFO Reset */
	Outp32(&pUartRegs->rUfCon , TempReg);

	//UART_SWReset(pUartCon);
}

void UART_SetVar_UartOpen(UART_CON *pUartCon)
{
	s32				SelNum;

	UART_Printf("\nAre you sure? : ");
	UART_Printf("\n");

	UART_Printf("\n0.NO(D)");
	UART_Printf("\n1.YES");
	UART_Printf("\n");

	UART_Printf("\nChoose : ");

	SelNum = UART_GetIntNum();
	if(SelNum < 0 || SelNum > 1)
	{
		SelNum = 0;
	}

	if(SelNum == 0)
	{
		/* Do nothing */
		
		return;
	}

	UART_Open(pUartCon);
}

#if 0
void UART_SetVar_ReadRegister(UART_CON *pUartCon)
{
	s32				SelNum;
	s32				Status;

	while(1)
	{
		UART_Printf("\nRead Register : ");
		UART_Printf("\n");
		
		UART_Printf("\n0.ULCON         (%08X)",  (u32)pUartCon->m_pUartSFRBase + 0x00);
		UART_Printf("\n1.UCON          (%08X)",  (u32)pUartCon->m_pUartSFRBase + 0x04);
		UART_Printf("\n2.UFCON         (%08X)",  (u32)pUartCon->m_pUartSFRBase + 0x08);
		UART_Printf("\n3.UMCON         (%08X)",  (u32)pUartCon->m_pUartSFRBase + 0x0C);
		UART_Printf("\n4.UTRSTAT       (%08X)",  (u32)pUartCon->m_pUartSFRBase + 0x10);
		UART_Printf("\n5.UERSTAT       (%08X)",  (u32)pUartCon->m_pUartSFRBase + 0x14);
		UART_Printf("\n6.UFSTAT        (%08X)",  (u32)pUartCon->m_pUartSFRBase + 0x18);
		UART_Printf("\n7.UMSTAT        (%08X)",  (u32)pUartCon->m_pUartSFRBase + 0x1C);
		UART_Printf("\n8.UTXH          (%08X)",  (u32)pUartCon->m_pUartSFRBase + 0x20);
		UART_Printf("\n9.URXH          (%08X)",  (u32)pUartCon->m_pUartSFRBase + 0x24);
		UART_Printf("\n10.UBRDIV       (%08X)",  (u32)pUartCon->m_pUartSFRBase + 0x28);
		UART_Printf("\n11.UDIVSLOT     (%08X)",  (u32)pUartCon->m_pUartSFRBase + 0x2C);
		UART_Printf("\n12.INTP         (%08X)",  (u32)pUartCon->m_pUartSFRBase + 0x30);
		UART_Printf("\n13.UINTSP       (%08X)",  (u32)pUartCon->m_pUartSFRBase + 0x34);
		UART_Printf("\n14.UINTM        (%08X)",  (u32)pUartCon->m_pUartSFRBase + 0x38);
		UART_Printf("\n15.ALL\n");
		UART_Printf("\n16.Exit");
		UART_Printf("\n");
		
		UART_Printf("\nChoose : ");

		SelNum = UART_GetIntNum();
		if(SelNum == 16)
		{
			/* Do nothing */

			return;
		}

		if(SelNum < 0 || SelNum > 15)
		{
			break;
		}

		if(SelNum >=0 && SelNum <= 14)
		{
			UART_Printf("\n%08X : %08X",  (u32)pUartCon->m_pUartSFRBase + SelNum * 4, 
				*(volatile u32 *)((u32)pUartCon->m_pUartSFRBase + SelNum * 4));
		}
		else
		{
			int LoopCnt;

			for(LoopCnt=0; LoopCnt < 15; LoopCnt++)
			{
				UART_Printf("\n%08X : %08X",  (u32)pUartCon->m_pUartSFRBase + LoopCnt * 4, 
					*(volatile u32 *)((u32)pUartCon->m_pUartSFRBase + LoopCnt * 4));
			}
		}

		UART_Printf("\n\nPress any key to continue....\n\n");

		while(!UART_GetKey())
		{
		}

	}
}
#endif /* 0 */

void UART_SetVar_ReadWriteRegister(UART_CON *pUartCon)
{
	s32				SelNum;
	s32				YesNo;
	u32				InputValue;
	s32				Status;
	u32				TempReg;

	while(1)
	{
		UART_Printf("\nRead/Write Register : ");
		UART_Printf("\n");
		
		UART_Printf("\n0.ULCON         (%08X)",  (u32)pUartCon->m_pUartSFRBase + 0x00);
		UART_Printf("\n1.UCON          (%08X)",  (u32)pUartCon->m_pUartSFRBase + 0x04);
		UART_Printf("\n2.UFCON         (%08X)",  (u32)pUartCon->m_pUartSFRBase + 0x08);
		UART_Printf("\n3.UMCON         (%08X)",  (u32)pUartCon->m_pUartSFRBase + 0x0C);
		UART_Printf("\n4.UTRSTAT       (%08X)",  (u32)pUartCon->m_pUartSFRBase + 0x10);
		UART_Printf("\n5.UERSTAT       (%08X)",  (u32)pUartCon->m_pUartSFRBase + 0x14);
		UART_Printf("\n6.UFSTAT        (%08X)",  (u32)pUartCon->m_pUartSFRBase + 0x18);
		UART_Printf("\n7.UMSTAT        (%08X)",  (u32)pUartCon->m_pUartSFRBase + 0x1C);
		UART_Printf("\n8.UTXH          (%08X)",  (u32)pUartCon->m_pUartSFRBase + 0x20);
		UART_Printf("\n9.URXH          (%08X)",  (u32)pUartCon->m_pUartSFRBase + 0x24);
		UART_Printf("\n10.UBRDIV       (%08X)",  (u32)pUartCon->m_pUartSFRBase + 0x28);
		UART_Printf("\n11.UDIVSLOT     (%08X)",  (u32)pUartCon->m_pUartSFRBase + 0x2C);
		UART_Printf("\n12.INTP         (%08X)",  (u32)pUartCon->m_pUartSFRBase + 0x30);
		UART_Printf("\n13.UINTSP       (%08X)",  (u32)pUartCon->m_pUartSFRBase + 0x34);
		UART_Printf("\n14.UINTM        (%08X)",  (u32)pUartCon->m_pUartSFRBase + 0x38);
		UART_Printf("\n15.ALL\n");
		UART_Printf("\n16.Exit");
		UART_Printf("\n");
		
		UART_Printf("\nChoose : ");

		SelNum = UART_GetIntNum();
		if(SelNum == 16)
		{
			/* Do nothing */

			return;
		}

		if(SelNum < 0 || SelNum > 15)
		{
			break;
		}

		if(SelNum >=0 && SelNum <= 14)
		{
			TempReg = *(volatile u32 *)((u32)pUartCon->m_pUartSFRBase + SelNum * 4);
			
			UART_Printf("\n%08X : %08X (%d)",  (u32)pUartCon->m_pUartSFRBase + SelNum * 4, 
				TempReg, TempReg);
			UART_Printf("\n");

			UART_Printf("\nDo you want to write? : ");
			UART_Printf("\n");

			UART_Printf("\n0.NO(D)");
			UART_Printf("\n1.YES");
			UART_Printf("\n");

			UART_Printf("\nChoose : ");

			YesNo = UART_GetIntNum();
			if(YesNo == 1)
			{
				UART_Printf("\nInput Value (ex: 1234, 0x12345678, 12345678h) : ");

				InputValue = (u32)UART_GetIntNum();

				*(u32 *)((u32)pUartCon->m_pUartSFRBase + SelNum * 4) = InputValue;

				TempReg = *(volatile u32 *)((u32)pUartCon->m_pUartSFRBase + SelNum * 4);

				UART_Printf("\n%08X : %08X (%d)",  (u32)pUartCon->m_pUartSFRBase + SelNum * 4, 
					TempReg, TempReg);

				UART_Printf("\n\nPress any key to continue....\n\n");

				while(!UART_GetKey())
				{
				}
			}
		}
		else
		{
			int LoopCnt;

			for(LoopCnt=0; LoopCnt < 15; LoopCnt++)
			{
				TempReg = *(volatile u32 *)((u32)pUartCon->m_pUartSFRBase + LoopCnt * 4);

				UART_Printf("\n%08X : %08X (%d)",  (u32)pUartCon->m_pUartSFRBase + LoopCnt * 4, 
					TempReg, TempReg);
			}

			UART_Printf("\n\nPress any key to continue....\n\n");

			while(!UART_GetKey())
			{
			}
		}
	}
}


void UART_SetVar_Baudrate(UART_CON *pUartCon)
{
	s32				SelNum;
	s32				Status;

	UART_Printf("\nBaudRate (ex 9600, 115200[D], 843750) : ");

	SelNum = UART_GetIntNum();
	if(SelNum < 0)
	{
		SelNum = 115200;
	}

	Status = UART_CalcBaudrate(pUartCon->m_IsUseExtClk, SelNum,
		pUartCon->m_ExtClkSrc, pUartCon->m_ExtClkSrcDiv, &pUartCon->m_OpClock, 
		&pUartCon->m_ClkDivValue, &pUartCon->m_ClkSlot);
	if(Status == UART_NO_ERROR)
	{
		pUartCon->m_Baudrate = (u32)SelNum;
		UART_SelectExtClkSrc(pUartCon);
	}
}



void UART_SetVar_IrDAMode(UART_CON *pUartCon)
{
	s32				SelNum;

	UART_Printf("\nIrDA : ");
	UART_Printf("\n");

	UART_Printf("\n0.OFF(D)");
	UART_Printf("\n1.ON");
	UART_Printf("\n");

	UART_Printf("\nChoose : ");

	SelNum = UART_GetIntNum();
	if(SelNum < 0 || SelNum > 1)
	{
		SelNum = 0;
	}
	pUartCon->m_EnIRDAMode = (u32)SelNum;
}



void UART_SetVar_InteranlLoopbackMode(UART_CON *pUartCon)
{
	s32				SelNum;

	UART_Printf("\nInternal LoopBack : ");
	UART_Printf("\n");

	UART_Printf("\n0.OFF(D)");
	UART_Printf("\n1.ON");
	UART_Printf("\n");
	
	UART_Printf("\nChoose : ");

	SelNum = UART_GetIntNum();
	if(SelNum < 0 || SelNum > 1)
	{
		SelNum = 0;
	}
	pUartCon->m_EnLoopBackMode = (u32)SelNum;
}



void UART_SetVar_AFCMode(UART_CON *pUartCon)
{
	s32				SelNum;

	UART_Printf("\nAFC(Auto Flow Control) : ");
	UART_Printf("\n");

	UART_Printf("\n0.OFF(D)");
	UART_Printf("\n1.ON");
	UART_Printf("\n");

	UART_Printf("\nChoose : ");

	SelNum = UART_GetIntNum();
	if(SelNum < 0 || SelNum > 1)
	{
		SelNum = 0;
	}
	pUartCon->m_EnAFC = (u32)SelNum;
}


void UART_SetVar_FIFOMode(UART_CON *pUartCon)
{
	s32				SelNum;

	UART_Printf("\nFIFO Mode : ");
	UART_Printf("\n");

	UART_Printf("\n0.OFF");
	UART_Printf("\n1.ON(D)");
	UART_Printf("\n");

	UART_Printf("\nChoose : ");

	SelNum = UART_GetIntNum();
	if(SelNum < 0 || SelNum > 1)
	{
		SelNum = 1;
	}
	pUartCon->m_EnFifo = (u32)SelNum;
}


void UART_SetVar_TxDMABurstSize(UART_CON *pUartCon)
{
	s32				SelNum;

	UART_Printf("\nTx DMA Burst : ");
	UART_Printf("\n");

	UART_Printf("\n0.DMA_1B(D)");
	UART_Printf("\n1.DMA_4B");
	UART_Printf("\n2.DMA_8B");
	UART_Printf("\n");

	UART_Printf("\nChoose : ");

	SelNum = UART_GetIntNum();
	if(SelNum < 0 || SelNum > 2)
	{
		SelNum = 0;
	}
	pUartCon->m_TxDMABurstSize = (u32)SelNum;
}



void UART_SetVar_RxDMABurstSize(UART_CON *pUartCon)
{
	s32				SelNum;

	UART_Printf("\nRx DMA Burst :");
	UART_Printf("\n");

	UART_Printf("\n0.DMA_1B(D)");
	UART_Printf("\n1.DMA_4B");
	UART_Printf("\n2.DMA_8B");
	UART_Printf("\n3.DMA_16B");
	UART_Printf("\n");

	UART_Printf("\nChoose : ");

	SelNum = UART_GetIntNum();
	if(SelNum < 0 || SelNum > 3)
	{
		SelNum = 0;
	}
	pUartCon->m_RxDMABurstSize = (u32)SelNum;
}




void UART_SetVar_DataBitSize(UART_CON *pUartCon)
{
	s32				SelNum;

	UART_Printf("\nData bit :");
	UART_Printf("\n");

	UART_Printf("\n0.WL_5bit");
	UART_Printf("\n1.WL_6bit");
	UART_Printf("\n2.WL_7bit");
	UART_Printf("\n3.WL_8bit(D)");
	UART_Printf("\n");

	UART_Printf("\nChoose : ");

	SelNum = UART_GetIntNum();
	if(SelNum < 0 || SelNum > 3)
	{
		SelNum = 0;
	}
	pUartCon->m_DataBit = (u32)SelNum;
}



void UART_SetVar_ParityBitSize(UART_CON *pUartCon)
{
	s32				SelNum;

	UART_Printf("\nParity bit :");
	UART_Printf("\n");

	UART_Printf("\n0.None(D)");
	UART_Printf("\n1.Odd");
	UART_Printf("\n2.Even");
	UART_Printf("\n3.Force1");
	UART_Printf("\n4.Force0");
	UART_Printf("\n");

	UART_Printf("\nChoose : ");

	SelNum = UART_GetIntNum();
	switch(SelNum)
	{
		case 0:
			break;
		case 1:
		case 2:
		case 3:
		case 4:
			SelNum += 3;		/* 1=>4, 2=>5, 3=>6, 4=>7 */
			break;
		default :
			SelNum = 0;
			break;
	}
	
	pUartCon->m_ParityBit = (u32)SelNum;
}



void UART_SetVar_StopBitSize(UART_CON *pUartCon)
{
	s32				SelNum;

	UART_Printf("\nStop bit :");
	UART_Printf("\n");

	UART_Printf("\n0.StopBit_1(D)");
	UART_Printf("\n1.StopBit_2");
	UART_Printf("\n");

	UART_Printf("\nChoose : ");

	SelNum = UART_GetIntNum();
	if(SelNum < 0 || SelNum > 1)
	{
		SelNum = 0;
	}
	
	pUartCon->m_StopBit = (u32)SelNum;
}



void UART_SetVar_TxMode(UART_CON *pUartCon)
{
	s32				SelNum;

	UART_Printf("\nTx Mode :");
	UART_Printf("\n");

	UART_Printf("\n0.Disable");
	UART_Printf("\n1.Interrupt(D)");
	UART_Printf("\n2.DMA");
	UART_Printf("\n");

	UART_Printf("\nChoose : ");

	SelNum = UART_GetIntNum();
	if(SelNum < 0 || SelNum > 2)
	{
		SelNum = 0;
	}
	
	pUartCon->m_TxMode = (u32)SelNum;
}




void UART_SetVar_RxMode(UART_CON *pUartCon)
{
	s32				SelNum;

	UART_Printf("\nRx Mode : ");
	UART_Printf("\n");

	UART_Printf("\n0.Disable");
	UART_Printf("\n1.Interrupt(D)");
	UART_Printf("\n2.DMA");
	UART_Printf("\n");

	UART_Printf("\nChoose : ");

	SelNum = UART_GetIntNum();
	if(SelNum < 0 || SelNum > 2)
	{
		SelNum = 0;
	}
	
	pUartCon->m_RxMode = (u32)SelNum;
}




void UART_SetVar_TxTrigLevel(UART_CON *pUartCon)
{
	s32				SelNum;

	UART_Printf("\nTx Trigger Level : ");
	UART_Printf("\n");

	UART_Printf("\n0.FIFO_1B[D]");
	UART_Printf("\n1.FIFO_4B");
	UART_Printf("\n2.FIFO_8B");
	UART_Printf("\n3.FIFO_16B");
	UART_Printf("\n4.FIFO_32B");
	UART_Printf("\n5.FIFO_64B");
	UART_Printf("\n6.FIFO_128B");
	UART_Printf("\n7.FIFO_256B");
	UART_Printf("\n");

	UART_Printf("\nChoose : ");

	SelNum = UART_GetIntNum();
	if(SelNum < 0 || SelNum > 7)
	{
		SelNum = 0;
	}
	
	pUartCon->m_TxTrigLvl = (u32)SelNum;
}



void UART_SetVar_RxTrigLevel(UART_CON *pUartCon)
{
	s32				SelNum;

	UART_Printf("\nRx Trigger Level : ");
	UART_Printf("\n");

	UART_Printf("\n0.FIFO_1B[D]");
	UART_Printf("\n1.FIFO_4B");
	UART_Printf("\n2.FIFO_8B");
	UART_Printf("\n3.FIFO_16B");
	UART_Printf("\n4.FIFO_32B");
	UART_Printf("\n5.FIFO_64B");
	UART_Printf("\n6.FIFO_128B");
	UART_Printf("\n7.FIFO_256B");
	UART_Printf("\n");

	UART_Printf("\nChoose : ");

	SelNum = UART_GetIntNum();
	if(SelNum < 0 || SelNum > 7)
	{
		SelNum = 0;
	}
	
	pUartCon->m_RxTrigLvl = (u32)SelNum;
}



void UART_SetVar_RtsTrigLevel(UART_CON *pUartCon)
{
	s32				SelNum;

	UART_Printf("\nRTS Trigger Level : ");
	UART_Printf("\n");

	UART_Printf("\n0.FIFO_255/63/15B[D]");
	UART_Printf("\n1.FIFO_224/56/14B");
	UART_Printf("\n2.FIFO_192/48/12B");
	UART_Printf("\n3.FIFO_160/40/10B");
	UART_Printf("\n4.FIFO_128/32/8B");
	UART_Printf("\n5.FIFO_96/24/6B");
	UART_Printf("\n6.FIFO_64/16/4B");
	UART_Printf("\n7.FIFO_32/8/2B");
	UART_Printf("\n");

	UART_Printf("\nChoose : ");

	SelNum = UART_GetIntNum();
	if(SelNum < 0 || SelNum > 7)
	{
		SelNum = 0;
	}
	
	pUartCon->m_RtsTrigLvl = (u32)SelNum;
}

void UART_SetVar_DrvStrength(UART_CON *pUartCon)
{
	s32				SelNum;

	UART_Printf("\nDrive Strength (0 ~ 3[D]) : ");

	SelNum = UART_GetIntNum();
	if(SelNum < 0 || SelNum > 3)
	{
		SelNum = 3;
	}

	pUartCon->m_DrvStrength = (u32)SelNum;
}

void UART_SetVar_EnModemIntMask(UART_CON *pUartCon)
{
	s32				SelNum;

	UART_Printf("\nModem Interrupt Mask : ");
	UART_Printf("\n");

	UART_Printf("\n0.CLEAR");
	UART_Printf("\n1.SET(D)");
	UART_Printf("\n");

	UART_Printf("\nChoose : ");

	SelNum = UART_GetIntNum();
	if(SelNum < 0 || SelNum > 1)
	{
		SelNum = 1;
	}
	pUartCon->m_EnModemIntMask = (u32)SelNum;
}


void UART_SetVar_EnTxdIntMask(UART_CON *pUartCon)
{
	s32				SelNum;

	UART_Printf("\nTXD Interrupt Mask : ");
	UART_Printf("\n");

	UART_Printf("\n0.CLEAR");
	UART_Printf("\n1.SET(D)");
	UART_Printf("\n");

	UART_Printf("\nChoose : ");

	SelNum = UART_GetIntNum();
	if(SelNum < 0 || SelNum > 1)
	{
		SelNum = 1;
	}
	pUartCon->m_EnTxdIntMask = (u32)SelNum;
}


void UART_SetVar_EnErrorIntMask(UART_CON *pUartCon)
{
	s32				SelNum;

	UART_Printf("\nERROR Interrupt Mask : ");
	UART_Printf("\n");

	UART_Printf("\n0.CLEAR");
	UART_Printf("\n1.SET(D)");
	UART_Printf("\n");

	UART_Printf("\nChoose : ");

	SelNum = UART_GetIntNum();
	if(SelNum < 0 || SelNum > 1)
	{
		SelNum = 1;
	}
	pUartCon->m_EnErrorIntMask = (u32)SelNum;
}


void UART_SetVar_EnRxdIntMask(UART_CON *pUartCon)
{
	s32				SelNum;

	UART_Printf("\nRXD Interrupt Mask : ");
	UART_Printf("\n");

	UART_Printf("\n0.CLEAR");
	UART_Printf("\n1.SET(D)");
	UART_Printf("\n");

	UART_Printf("\nChoose : ");

	SelNum = UART_GetIntNum();
	if(SelNum < 0 || SelNum > 1)
	{
		SelNum = 1;
	}
	pUartCon->m_EnRxdIntMask = (u32)SelNum;
}


void UART_SetVar_RxTimeoutMode(UART_CON *pUartCon)
{
	s32				SelNum;

	UART_Printf("\nRx Timeout : ");
	UART_Printf("\n");

	UART_Printf("\n0.OFF");
	UART_Printf("\n1.ON(D)");
	UART_Printf("\n");

	UART_Printf("\nChoose : ");

	SelNum = UART_GetIntNum();
	if(SelNum < 0 || SelNum > 1)
	{
		SelNum = 1;
	}
	pUartCon->m_EnRxTimeout = (u32)SelNum;
}


void UART_SetVar_TxTransferSize(UART_CON *pUartCon)
{
	s32				SelNum;

	UART_Printf("\nTx Data Transfer Size (1..8[D]..65536  byte(s)) : ");

	SelNum = UART_GetIntNum();
	if(SelNum < 0 || SelNum > 65536)
	{
		SelNum = 8;
	}

	pUartCon->m_TxTransferSize = (u32)SelNum;
}


void UART_SetVar_RxTransferSize(UART_CON *pUartCon)
{
	s32				SelNum;

	UART_Printf("\nRx Data Transfer Size (1..8[D]..65536  byte(s)) : ");

	SelNum = UART_GetIntNum();
	if(SelNum < 0 || SelNum > 65536)
	{
		SelNum = 8;
	}

	pUartCon->m_RxTransferSize = (u32)SelNum;
}


void UART_SetVar_ExtClkMode(UART_CON *pUartCon)
{
	s32				SelNum;
	s32				Status;

	UART_Printf("\nClock Selection : ");
	UART_Printf("\n");

	UART_Printf("\n0.PCLK(D)");
	UART_Printf("\n1.SCLK_UART");
	UART_Printf("\n");

	UART_Printf("\nChoose : ");

	SelNum = UART_GetIntNum();
	if(SelNum < 0 || SelNum > 1)
	{
		SelNum = 0;
	}

	Status = UART_CalcBaudrate(SelNum, pUartCon->m_Baudrate,
		pUartCon->m_ExtClkSrc, pUartCon->m_ExtClkSrcDiv, &pUartCon->m_OpClock, 
		&pUartCon->m_ClkDivValue, &pUartCon->m_ClkSlot);
	if(Status == UART_NO_ERROR)
	{
		pUartCon->m_IsUseExtClk = (u32)SelNum;
		UART_SelectExtClkSrc(pUartCon);
	}
}



void UART_SetVar_ExtClkSource(UART_CON *pUartCon)
{
	s32				SelNum;
	s32				Status;

	UART_Printf("\nEXT_CLK_SRC : ");
	UART_Printf("\n");
	
	UART_Printf("\n0.XXTI");
	UART_Printf("\n1.USBXTI[D]");
	UART_Printf("\n2.HDMI27M");
	UART_Printf("\n4.USBPHY1");
	UART_Printf("\n5.HDMYPHY");
	UART_Printf("\n6.MPLL(%.2fMHz)", (float)g_uMPLL/1.0e6);
	UART_Printf("\n7.EPLL(%.2fMHz)", (float)g_uEPLL/1.0e6);
	UART_Printf("\n8.VPLL(%.2fMHz)", (float)g_uVPLL/1.0e6);
	UART_Printf("\n");
	
	UART_Printf("\nChoose : ");

	SelNum = UART_GetIntNum();
	if(SelNum < 0 || SelNum > 8)
	{
		SelNum = 1;
	}

	Status = UART_CalcBaudrate(pUartCon->m_IsUseExtClk, pUartCon->m_Baudrate,
		SelNum, pUartCon->m_ExtClkSrcDiv, &pUartCon->m_OpClock, 
		&pUartCon->m_ClkDivValue, &pUartCon->m_ClkSlot);
	if(Status == UART_NO_ERROR)
	{
		pUartCon->m_ExtClkSrc = (u32)SelNum;
		UART_SelectExtClkSrc(pUartCon);
	}
}




void UART_SetVar_ExtClkSourceDiv(UART_CON *pUartCon)
{
	s32				SelNum;
	s32				Status;

	UART_Printf("\nBaudRate (1[D] ~ 8) : ");

	SelNum = UART_GetIntNum();
	if(SelNum < 0)
	{
		SelNum = 1;
	}
	pUartCon->m_ExtClkSrcDiv = (u32)SelNum;

	Status = UART_CalcBaudrate(pUartCon->m_IsUseExtClk, pUartCon->m_Baudrate,
		pUartCon->m_ExtClkSrc, SelNum, &pUartCon->m_OpClock, 
		&pUartCon->m_ClkDivValue, &pUartCon->m_ClkSlot);
	if(Status == UART_NO_ERROR)
	{
		pUartCon->m_ExtClkSrcDiv = (u32)SelNum;
		UART_SelectExtClkSrc(pUartCon);
	}
}


void UART_SetVar_CheckTxEndByNull(UART_CON *pUartCon)
{
	s32				SelNum;

	UART_Printf("\nCheck Tx End By NULL : ");
	UART_Printf("\n");

	UART_Printf("\n0.OFF");
	UART_Printf("\n1.ON(D)");
	UART_Printf("\n");

	UART_Printf("\nChoose : ");

	SelNum = UART_GetIntNum();
	if(SelNum < 0 || SelNum > 1)
	{
		SelNum = 1;
	}
	pUartCon->m_CheckTxEndbyNull = (u32)SelNum;
}


void UART_SetVar_CheckRxEndByCR(UART_CON *pUartCon)
{
	s32				SelNum;

	UART_Printf("\nCheck Rx End By CR : ");
	UART_Printf("\n");

	UART_Printf("\n0.OFF");
	UART_Printf("\n1.ON(D)");
	UART_Printf("\n");

	UART_Printf("\nChoose : ");

	SelNum = UART_GetIntNum();
	if(SelNum < 0 || SelNum > 1)
	{
		SelNum = 1;
	}
	pUartCon->m_CheckRxEndbyCR = (u32)SelNum;
}



void UART_SetVar_RxDMACh(UART_CON *pUartCon)
{
	s32				SelNum;

	UART_Printf("\nRX DMA_CH : ");
	UART_Printf("\n");
	
	UART_Printf("\n0.DMA_00[D]");
	UART_Printf("\n1.DMA_01");
	UART_Printf("\n2.DMA_02");
	UART_Printf("\n3.DMA_03");
	UART_Printf("\n4.DMA_04");
	UART_Printf("\n5.DMA_05");
	UART_Printf("\n6.DMA_06");
	UART_Printf("\n7.DMA_07");
	UART_Printf("\n");

	UART_Printf("\n8.DMA_10");
	UART_Printf("\n9.DMA_11");
	UART_Printf("\n10.DMA_12");
	UART_Printf("\n11.DMA_13");
	UART_Printf("\n12.DMA_14");
	UART_Printf("\n13.DMA_15");
	UART_Printf("\n14.DMA_16");
	UART_Printf("\n15.DMA_17");
	UART_Printf("\n");

	UART_Printf("\nChoose : ");

	SelNum = UART_GetIntNum();
	if(SelNum < 0 || SelNum > 15)
	{
		SelNum = 0;
	}
	
	pUartCon->m_UartRxDmaCh = (u32)SelNum;
	if(SelNum < 8)
	{
		pUartCon->m_UartRxDMANum = UART_DMA0;
	}
	else
	{
		pUartCon->m_UartRxDMANum = UART_DMA1;
	}
}



void UART_SetVar_TxDMACh(UART_CON *pUartCon)
{
	s32				SelNum;

	UART_Printf("\nRX DMA_CH : ");
	UART_Printf("\n");
	
	UART_Printf("\n0.DMA_00");
	UART_Printf("\n1.DMA_01[D]");
	UART_Printf("\n2.DMA_02");
	UART_Printf("\n3.DMA_03");
	UART_Printf("\n4.DMA_04");
	UART_Printf("\n5.DMA_05");
	UART_Printf("\n6.DMA_06");
	UART_Printf("\n7.DMA_07");
	UART_Printf("\n");

	UART_Printf("\n8.DMA_10");
	UART_Printf("\n9.DMA_11");
	UART_Printf("\n10.DMA_12");
	UART_Printf("\n11.DMA_13");
	UART_Printf("\n12.DMA_14");
	UART_Printf("\n13.DMA_15");
	UART_Printf("\n14.DMA_16");
	UART_Printf("\n15.DMA_17");
	UART_Printf("\n");

	UART_Printf("\nChoose : ");

	SelNum = UART_GetIntNum();
	if(SelNum < 0 || SelNum > 15)
	{
		SelNum = 1;
	}
	
	pUartCon->m_UartTxDmaCh = (u32)SelNum;
	if(SelNum < 8)
	{
		pUartCon->m_UartTxDMANum = UART_DMA0;
	}
	else
	{
		pUartCon->m_UartTxDMANum = UART_DMA1;
	}
}


void UART_CreateNCBufCtrl(UART_NCBufCtrlInfo_st *pNCBufCtrlInfo, u32 StartAddr, u32 TotalSize)
{
	pNCBufCtrlInfo->m_StartMemAddr = StartAddr;
	pNCBufCtrlInfo->m_CurMemAddr = StartAddr;
	pNCBufCtrlInfo->m_TotalSize = TotalSize;

	UART_BufZeroInit(pNCBufCtrlInfo->m_StartMemAddr, pNCBufCtrlInfo->m_TotalSize);
}



void UART_DeleteNCBufCtrl(UART_NCBufCtrlInfo_st *pNCBufCtrlInfo)
{
	/* Do nothing */
}


void UART_ResetNCBufCtrl(UART_NCBufCtrlInfo_st *pNCBufCtrlInfo)
{
	pNCBufCtrlInfo->m_CurMemAddr = pNCBufCtrlInfo->m_StartMemAddr;

	UART_BufZeroInit(pNCBufCtrlInfo->m_StartMemAddr, pNCBufCtrlInfo->m_TotalSize);
}




u8 *UART_GetNCBuf(UART_BufAllocInfo_st *pBufAllocInfo, u32 BufSize)
{
	UART_NCBufCtrlInfo_st *pNCBufCtrlInfo;

	pNCBufCtrlInfo = &gUART_CtrlInfo.m_NCBufCtrlInfo;
	if(BufSize > pNCBufCtrlInfo->m_TotalSize)
	{
		UART_ASSERT();
		
		return NULL;
	}
	
	if(pNCBufCtrlInfo->m_CurMemAddr + BufSize < pNCBufCtrlInfo->m_StartMemAddr + pNCBufCtrlInfo->m_TotalSize)
	{
		pBufAllocInfo->m_pBufPtr = (u8 *)pNCBufCtrlInfo->m_CurMemAddr;
		pBufAllocInfo->m_pCurPtr = (u8 *)pNCBufCtrlInfo->m_CurMemAddr;
		pNCBufCtrlInfo->m_CurMemAddr  += BufSize;
	}
	else
	{
		/* because ring buffer */
		pBufAllocInfo->m_pBufPtr = (u8 *)pNCBufCtrlInfo->m_StartMemAddr;
		pBufAllocInfo->m_pCurPtr = (u8 *)pNCBufCtrlInfo->m_StartMemAddr;
		pNCBufCtrlInfo->m_CurMemAddr = pNCBufCtrlInfo->m_StartMemAddr + BufSize;
	}
	pBufAllocInfo->m_BufSize = BufSize;

	return (u8 *)pBufAllocInfo->m_pBufPtr;
}




int UART_FreeNCBuf(UART_BufAllocInfo_st *pBufAllocInfo)
{
	/* NOT implemented because I'll just use the non cachable memory as Ring-Buffer. */
	
	return UART_NO_ERROR;
}


int UART_NCBufPatternInit(UART_BufAllocInfo_st *pBufAllocInfo, u32 BufSize, const char *pPattern, int PatternSize)
{
	void		*pBuf;
	u32		LocalLoop;
	char		*pDst;

	pBuf = pBufAllocInfo->m_pBufPtr;
	if(BufSize == 0 || PatternSize == 0)
	{
		UART_ASSERT();
		
		return UART_ERROR;
	}

	pDst = pBuf;

	pBufAllocInfo->m_ReqDataSize = BufSize;
	pBufAllocInfo->m_RemainDataSize = BufSize;
	if(PatternSize > BufSize)
	{
		PatternSize = BufSize;
	}
	while(BufSize>0)
	{
		LocalLoop = 0;
		while(LocalLoop < PatternSize)
		{
			*pDst = *(pPattern + LocalLoop);
			pDst++;
			LocalLoop++;
			BufSize--;
		}
	}

	return UART_NO_ERROR;
}



int UART_NCBufRandInit(UART_BufAllocInfo_st *pBufAllocInfo, u32 BufSize)
{
 	u8 *pBufPtr;

	pBufPtr = pBufAllocInfo->m_pBufPtr;
 	if(BufSize == 0)
	{
		UART_ASSERT();
		
		return UART_ERROR;
	}

	pBufAllocInfo->m_ReqDataSize = BufSize;
	pBufAllocInfo->m_RemainDataSize = BufSize;
	while(BufSize>0)
	{
		*pBufPtr = (u8)(rand() & 0xFF);
		pBufPtr++;
		BufSize--;
	}

	return UART_NO_ERROR;
}




int UART_NCBufZeroInit(UART_BufAllocInfo_st *pBufAllocInfo)
{
	void *pBuf;
	u32 BufSize;

	pBuf = pBufAllocInfo->m_pBufPtr;
	BufSize = pBufAllocInfo->m_BufSize;
	if(BufSize == 0)
	{
		UART_ASSERT();
		
		return UART_ERROR;
	}

	memset(pBuf, 0x00, BufSize);
	pBufAllocInfo->m_ReqDataSize = BufSize;
	pBufAllocInfo->m_RemainDataSize = BufSize;
	
	return UART_NO_ERROR;
}

void UART_NCBufDisplay(UART_BufAllocInfo_st *pBufAllocInfo, bool EnDisplay)
{
	u8 *pBuf;
	u32 BufSize;
	u32	LoopCnt;

	if(EnDisplay != TRUE)
	{
		return;
	}

	pBuf = pBufAllocInfo->m_pBufPtr;
	BufSize = pBufAllocInfo->m_ReqDataSize;
	for(LoopCnt=0; LoopCnt<BufSize; LoopCnt++)
	{
		UART_Printf("%c", *pBuf++);
	}
}

void UART_NCBufDisplayHex(UART_BufAllocInfo_st *pBufAllocInfo, bool EnDisplay)
{
	u8 *pBuf;
	u32 BufSize;
	u32	LoopCnt;

	if(EnDisplay != TRUE)
	{
		return;
	}

	UART_Printf("\n");
	pBuf = pBufAllocInfo->m_pBufPtr;
	BufSize = pBufAllocInfo->m_ReqDataSize;
	for(LoopCnt=0; LoopCnt<BufSize; LoopCnt++)
	{
		UART_Printf("%02x ", *pBuf++);
	}
	UART_Printf("\n");
}


int UART_BufPatternInit(void *pBuf, u32 BufSize, char *pPattern, int PatternSize)
{
	u32		LocalLoop;
	char		*pDst;

	if(BufSize == 0 || PatternSize == 0)
	{
		UART_ASSERT();
		
		return UART_ERROR;
	}

	pDst = pBuf;

	if(PatternSize > BufSize)
	{
		PatternSize = BufSize;
	}

	while(BufSize>0)
	{
		LocalLoop = 0;
		while(LocalLoop < PatternSize)
		{
			*pDst = *(pPattern + LocalLoop);
			pDst++;
			LocalLoop++;
			BufSize--;
		}
	}

	return UART_NO_ERROR;
}



int UART_BufRandInit(void *pBuf, u32 BufSize)
{
	u8 *pBufPtr;

	pBufPtr = pBuf;
	if(BufSize == 0)
	{
		UART_ASSERT();
		
		return UART_ERROR;
	}

	while(BufSize>0)
	{
		*pBufPtr = (u8)(rand() & 0xFF);
		pBufPtr++;
		BufSize--;
	}

	return UART_NO_ERROR;
}




int UART_BufZeroInit(void *pBuf, u32 BufSize)
{
	if(BufSize == 0)
	{
		UART_ASSERT();
		
		return UART_ERROR;
	}

	memset(pBuf, 0x00, BufSize);

	return UART_NO_ERROR;
}

void UART_BufDisplay(u8 *pBuf, u32 BufSize, bool EnDisplay)
{
	u32	LoopCnt;

	if(EnDisplay != TRUE)
	{
		return;
	}

	UART_Printf("\n");
	for(LoopCnt=0; LoopCnt<BufSize; LoopCnt++)
	{
		UART_Printf("%c", *pBuf++);
	}
}


void UART_BufDisplayHex(u8 *pBuf, u32 BufSize, bool EnDisplay)
{
	u32	LoopCnt;

	if(EnDisplay != TRUE)
	{
		return;
	}

	UART_Printf("\n");
	for(LoopCnt=0; LoopCnt<BufSize; LoopCnt++)
	{
		UART_Printf("%02x ", *pBuf++);
	}
}

int UART_CompareData(u8* data1, u8* data2, u32 bytes)
{
	u32 i;
	int errorCnt=0;
	
	for (i=0; i<bytes; i++)
	{
		if(*data1 != *data2 )
		{
			if (errorCnt==0)
			{
				UART_Printf("\n%08x=%02x <-> %08x=%02x\n", data1, *data1, data2, *data2);
			}
			errorCnt++;
		}
		data1++;
		data2++;
	}

	if(errorCnt==0)
	{
		UART_Printf("\nData Compare Ok");
	}
	else
	{
		UART_Printf( "\nError Count: %d", errorCnt );
	}

	return errorCnt;
}





