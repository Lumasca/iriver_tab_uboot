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
*	File Name : uart_test.c
*  
*	File Description : This file implements the API functons for Timer UART test.
*
*	Author : Woojin,Kim
*	Dept. : AP Development Team
*	Created Date : 2007/01/16
*	Version : 0.1 
* 
*	History
*	- Created (Woojin,Kim 2007/01/16)
*	- Modified (Jun,Kim 2009/04/15)  
**************************************************************************************/


#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <stdio.h>
#include "system.h"
#include "uart.h"
#include "intc.h"
#include "def.h"
#include "library.h"
#include "timer.h"
#include "sysc.h"

#include "option.h"


u8 Uart_AutoTest(void);


const char gUART_pInitString_PatternA[] = "123456789012345678901234567890ABCDEFGHIJKLMNOPQRSTUVWXYZ....abcdefghijklmnopqrstuvwxyz....";



typedef struct {
	s32 (*func)(UART_CtrlInfo_st *pCtrlInfo); 
	const char *desc;	
} UART_TestFuncMenu;


const UART_TestFuncMenu gUART_Test_TopMenu[] = 
{
	UART_SetDefaultInfo_Uart0,			"Set DefaultVar : UART0",
	UART_SetDefaultInfo_Uart1,			"Set DefaultVar : UART1",
	UART_SetDefaultInfo_Uart2,			"Set DefaultVar : UART2",
	UART_SetDefaultInfo_Uart3,			"Set DefaultVar : UART3\n",

	UART_PollingTxTest,					"Polling Tx Test",
	UART_InterruptTxTest,				"Interrupt Tx Test",
	UART_DMATxTest,					"DMA Tx Test\n",

	UART_PollingRxTest,					"Polling Rx Test",
	UART_InterruptRxTest,				"Interrupt Rx Test",
	UART_DMARxTest,					"DMA Rx Test\n",

	UART_IntLoopback_Polling_Test,		"Internal Loopback Polling Test",
	UART_IntLoopback_Interrupt_Test,	"Internal Loopback Interrupt Test",
	UART_IntLoopback_DMA_Test,		"Internal Loopback DMA Test\n",

	UART_ExtLoopback_Polling_Test,		"External Loopback Polling Test",
	UART_ExtLoopback_Interrupt_Test,	"External Loopback Interrupt Test",
	UART_ExtLoopback_DMA_Test,		"External Loopback DMA Test\n",

	UART_IntChLoopback_Polling_Test,	"InterChannel Loopback Polling Test",
	UART_IntChLoopback_Interrupt_Test,	"InterChannel Loopback Interrupt Test",
	UART_IntChLoopback_DMA_Test,		"InterChannel Loopback DMA Test\n",

	UART_PollingTxWithManualCTSTest,	"Polling Tx with manual CTS",
	UART_PollingRxWithManualRTSTest,	"Polling Rx with manual RTS\n",

	UART_ModemInterruptTest,			"Modem Interrupt Test\n",

	UART_SendBreakSignalTest,			"Send Break Signal Test\n",

	UART_IrDATxTest,						"IrDA Tx Test",
	UART_IrDARxTest,						"IrDA Rx Test\n",

	UART_SFR_test,						"SFR Test",
	UART_Console_AutoTest,				"Console Auto Test\n",

	UART_SetPLL,						"Set PLL(APLL/MPLL/EPLL/VPLL)\n",

	NULL,								"Exit"
};

void Test_UART(void)
{
	s32				MenuEntryCnt;
	s32				MenuIndx;
	s32				SelNum;
	s32				TestResult = 0;
	
	UART_SetDefaultSystemEnv();			/* Set Clock, TZPC */
	UART_InitDefaultVariable(&gUART_CtrlInfo);
	UART_Init(&gUART_CtrlInfo);

	UART_Printf("\n\n====================================================");
	UART_Printf("\n   Welcome to UART Test World!");
	UART_Printf("\n====================================================\n");

	MenuEntryCnt = sizeof(gUART_Test_TopMenu)/sizeof(UART_TestFuncMenu);
	while(1)
	{
		/* Display Top Menu */
		UART_Printf("\n====================================================");
		UART_Printf("\n   UART Top Menu");
		UART_Printf("\n====================================================\n");
		for(MenuIndx = 0; MenuIndx<MenuEntryCnt; MenuIndx++)
		{   
	   		UART_Printf("%2d.%s", MenuIndx, gUART_Test_TopMenu[MenuIndx].desc);
			UART_Printf("\n");
		}
		UART_Printf("\n====================================================");

		UART_Printf("\nSelect: ");
		SelNum = UART_GetIntNum();
		//if(SelNum <= 0 || SelNum >= MenuEntryCnt)
		if(SelNum == MenuEntryCnt - 1)
		{
			break;
		}

		if(SelNum >= 0 && SelNum < MenuEntryCnt - 1)
		{
			if(gUART_Test_TopMenu[SelNum].func != NULL)
			{
				TestResult = (gUART_Test_TopMenu[SelNum].func)(&gUART_CtrlInfo);
			}
		}
	}
}

typedef void (*UART_fnDisplayInfo_t)(UART_CON *pUartCon);
typedef void (*UART_fnSetInfo_t)(UART_CON *pUartCon);

typedef struct {
	UART_fnDisplayInfo_t	m_DisplayUartInfo;
	UART_fnSetInfo_t		m_SetUartInfo;
} UART_SetInfoEntry_st;

UART_SetInfoEntry_st gUART_SetInfoEntry[] = 
{
	{UART_DisplayVar_InitNCBuf, 				UART_SetVar_InitNCBuf},
	{UART_DisplayVar_ChangeDebugChannel, 		UART_SetVar_ChangeDebugChannel},
	{UART_DisplayVar_EnUart2CtsRts, 			UART_SetVar_EnUart2CtsRts},
	{UART_DisplayVar_EnInterruptPrint, 			UART_SetVar_EnInterruptPrint},	/* '\n' included */
	
	{UART_DisplayVar_LoadDefault, 				UART_SetVar_LoadDefault},
	{UART_DisplayVar_SWReset, 					UART_SetVar_SWReset},
	{UART_DisplayVar_UartOpen, 				UART_SetVar_UartOpen},
	//{UART_DisplayVar_ReadRegister, 				UART_SetVar_ReadRegister},
	{UART_DisplayVar_ReadWriteRegister, 		UART_SetVar_ReadWriteRegister},
	{UART_DisplayVar_Baudrate, 					UART_SetVar_Baudrate},
	{UART_DisplayVar_IrDAMode, 				UART_SetVar_IrDAMode},
	{UART_DisplayVar_InteranlLoopbackMode, 	UART_SetVar_InteranlLoopbackMode},
	{UART_DisplayVar_AFCMode, 					UART_SetVar_AFCMode},
	{UART_DisplayVar_FIFOMode, 				UART_SetVar_FIFOMode},
	{UART_DisplayVar_TxDMABurstSize, 			UART_SetVar_TxDMABurstSize},
	{UART_DisplayVar_RxDMABurstSize, 			UART_SetVar_RxDMABurstSize},
	{UART_DisplayVar_DataBitSize, 				UART_SetVar_DataBitSize},
	{UART_DisplayVar_ParityBitSize, 				UART_SetVar_ParityBitSize},
	{UART_DisplayVar_StopBitSize, 				UART_SetVar_StopBitSize},
	{UART_DisplayVar_TxMode, 					UART_SetVar_TxMode},
	{UART_DisplayVar_RxMode, 					UART_SetVar_RxMode},
	{UART_DisplayVar_TxTrigLevel, 				UART_SetVar_TxTrigLevel},
	{UART_DisplayVar_RxTrigLevel, 				UART_SetVar_RxTrigLevel},
	{UART_DisplayVar_RtsTrigLevel, 				UART_SetVar_RtsTrigLevel},
	{UART_DisplayVar_DrvStrength, 				UART_SetVar_DrvStrength},
	{UART_DisplayVar_EnModemIntMask, 			UART_SetVar_EnModemIntMask},
	{UART_DisplayVar_EnTxdIntMask, 			UART_SetVar_EnTxdIntMask},
	{UART_DisplayVar_EnErrorIntMask, 			UART_SetVar_EnErrorIntMask},
	{UART_DisplayVar_EnRxdIntMask, 			UART_SetVar_EnRxdIntMask},
	{UART_DisplayVar_RxTimeoutMode, 			UART_SetVar_RxTimeoutMode},
	{UART_DisplayVar_TxTransferSize, 			UART_SetVar_TxTransferSize},
	{UART_DisplayVar_RxTransferSize, 			UART_SetVar_RxTransferSize},
	//{UART_DisplayVar_DebugChannel, NULL},
	{UART_DisplayVar_ExtClkMode, 				UART_SetVar_ExtClkMode},
	{UART_DisplayVar_ExtClkSource, 				UART_SetVar_ExtClkSource},
	{UART_DisplayVar_ExtClkSourceDiv, 			UART_SetVar_ExtClkSourceDiv},
	{UART_DisplayVar_CurClkFrequency, 			NULL},
	{UART_DisplayVar_CheckTxEndByNull, 		UART_SetVar_CheckTxEndByNull},
	{UART_DisplayVar_CheckRxEndByCR, 			UART_SetVar_CheckRxEndByCR},
	{UART_DisplayVar_RxDMANum, 				NULL},
	{UART_DisplayVar_TxDMANum, 				NULL},
	{UART_DisplayVar_RxDMACh, 					UART_SetVar_RxDMACh},
	{UART_DisplayVar_TxDMACh, 					UART_SetVar_TxDMACh}
};


void UART_SetDefaultInfo(UART_CON *pUartCon)
{
	s32				MenuEntryCnt;
	s32				MenuIndx;
	s32				SelNum;
	UART_SetInfoEntry_st	*pSetInfoEntry;

	MenuEntryCnt = sizeof(gUART_SetInfoEntry)/sizeof(UART_SetInfoEntry_st);
	while(1)
	{
		UART_Printf("\n====================================================");
		UART_Printf("\n   UART%d Parameter Setting", pUartCon->m_UartChIdx);
		UART_Printf("\n====================================================\n");
		for(MenuIndx = 0; MenuIndx<MenuEntryCnt; MenuIndx++)
		{
			pSetInfoEntry = &gUART_SetInfoEntry[MenuIndx];

			UART_Printf("\n%2d. ", MenuIndx);
			if(pSetInfoEntry->m_DisplayUartInfo != NULL)
			{
				(*pSetInfoEntry->m_DisplayUartInfo)(pUartCon);
			}

			if(pSetInfoEntry->m_SetUartInfo == NULL)
			{
				UART_Printf("\t(Display Only)");
			}
		}
		UART_Printf("\n%2d. Exit", MenuIndx);
		UART_Printf("\n====================================================");

		UART_Printf("\nSelect: ");
		SelNum = UART_GetIntNum();
		//if(SelNum <= 0 || SelNum >= MenuEntryCnt)
		if(SelNum == MenuEntryCnt)	/* Out ouf entry range */
		{
			break;
		}

		if(SelNum >= 0 && SelNum < MenuEntryCnt)
		{
			pSetInfoEntry = &gUART_SetInfoEntry[SelNum];
			if(pSetInfoEntry->m_SetUartInfo != NULL)
			{
				(*pSetInfoEntry->m_SetUartInfo)(pUartCon);
			}
		}
	}
}


s32 UART_SetDefaultInfo_Uart0(UART_CtrlInfo_st *pCtrlInfo)
{
	UART_CON		*pUartCon;
	
	pUartCon = &pCtrlInfo->m_UartChTestInfo[UartCh_CH0];
	UART_SetDefaultInfo(pUartCon);
}

s32 UART_SetDefaultInfo_Uart1(UART_CtrlInfo_st *pCtrlInfo)
{
	UART_CON		*pUartCon;
	
	pUartCon = &pCtrlInfo->m_UartChTestInfo[UartCh_CH1];
	UART_SetDefaultInfo(pUartCon);
}

s32 UART_SetDefaultInfo_Uart2(UART_CtrlInfo_st *pCtrlInfo)
{
	UART_CON		*pUartCon;
	
	pUartCon = &pCtrlInfo->m_UartChTestInfo[UartCh_CH2];
	UART_SetDefaultInfo(pUartCon);
}

s32 UART_SetDefaultInfo_Uart3(UART_CtrlInfo_st *pCtrlInfo)
{
	UART_CON		*pUartCon;
	
	pUartCon = &pCtrlInfo->m_UartChTestInfo[UartCh_CH3];
	UART_SetDefaultInfo(pUartCon);
}




const UART_TestFuncMenu gUART_Test_PollingTxTest_Menu[] = 
{
	UART_SetDefaultInfo_Uart0,					"Set DefaultVar : UART0",
	UART_SetDefaultInfo_Uart1,					"Set DefaultVar : UART1",
	UART_SetDefaultInfo_Uart2,					"Set DefaultVar : UART2",
	UART_SetDefaultInfo_Uart3,					"Set DefaultVar : UART3\n",

	UART_Uart0PollingTxTest,					"Polling Tx Test : UART0",
	UART_Uart1PollingTxTest,					"Polling Tx Test : UART1",
	UART_Uart2PollingTxTest,					"Polling Tx Test : UART2",
	UART_Uart3PollingTxTest,					"Polling Tx Test : UART3\n",

	UART_SetPLL,								"Set PLL(APLL/MPLL/EPLL/VPLL)\n",

	NULL,										"Exit",
};

s32 UART_PollingTxTest(UART_CtrlInfo_st *pCtrlInfo)
{
	s32				MenuEntryCnt;
	s32				MenuIndx;
	s32				SelNum;
	s32				TestResult = 0;
	
	MenuEntryCnt = sizeof(gUART_Test_PollingTxTest_Menu)/sizeof(UART_TestFuncMenu);
	while(1)
	{
		/* Display Top Menu */
		UART_Printf("\n====================================================");
		UART_Printf("\n   UART Polling Tx Test");
		UART_Printf("\n====================================================\n");
		for(MenuIndx = 0; MenuIndx<MenuEntryCnt; MenuIndx++)
		{   
	   		UART_Printf("%2d.%s", MenuIndx, gUART_Test_PollingTxTest_Menu[MenuIndx].desc);
			UART_Printf("\n");
		}
		UART_Printf("\n====================================================");

		UART_Printf("\nSelect: ");
		SelNum = UART_GetIntNum();
		//if(SelNum <= 0 || SelNum >= MenuEntryCnt)
		if(SelNum == MenuEntryCnt - 1)
		{
			break;
		}

		if(SelNum >= 0 && SelNum < MenuEntryCnt - 1)
		{
			if(gUART_Test_PollingTxTest_Menu[SelNum].func != NULL)
			{
				TestResult = (gUART_Test_PollingTxTest_Menu[SelNum].func)(pCtrlInfo);
			}
		}
	}
}


s32 UART_UartAPollingTxTest(UART_CON *pUart)
{
	UART_BufAllocInfo_st	*pTxBufInfo;
	//UART_BufAllocInfo_st	*pRxBufInfo;
	//bool					TxDone = FALSE;
	//bool					RxDone = FALSE;

	if(pUart->m_UartChIdx == UART_GetCurDebugCh())
	{
		UART_Printf("\nError : UART%d is the debug channel.", pUart->m_UartChIdx);
		
		return UART_ERROR;
	}

	pUart->m_TxMode = UartMode_Interrupt;
	pUart->m_RxMode = UartMode_Interrupt;
	pTxBufInfo = &pUart->m_TxBufNode;

	UART_Open(pUart);

	pTxBufInfo->m_pBufPtr = UART_GetNCBuf(pTxBufInfo, pUart->m_TxTransferSize);
	if(pTxBufInfo->m_pBufPtr == NULL)
	{
		UART_ASSERT();
	}

	//UART_NCBufRandInit(pTxBufInfo, pUart->m_TxTransferSize);
	UART_NCBufPatternInit(pTxBufInfo, pUart->m_TxTransferSize, gUART_pInitString_PatternA, 10);
	UART_NCBufDisplayHex(pTxBufInfo, TRUE);

	UART_PollingTxString(pUart);

	UART_FreeNCBuf(pTxBufInfo);

	UART_Close(pUart);
}

s32 UART_Uart0PollingTxTest(UART_CtrlInfo_st *pCtrlInfo)
{
	UART_CON		*pUart;
	s32				Status;

	pUart = &pCtrlInfo->m_UartChTestInfo[UartCh_CH0];

	Status = UART_UartAPollingTxTest(pUart);

	return Status;
}

s32 UART_Uart1PollingTxTest(UART_CtrlInfo_st *pCtrlInfo)
{
	UART_CON		*pUart;
	s32				Status;

	pUart = &pCtrlInfo->m_UartChTestInfo[UartCh_CH1];

	Status = UART_UartAPollingTxTest(pUart);

	return Status;
}

s32 UART_Uart2PollingTxTest(UART_CtrlInfo_st *pCtrlInfo)
{
	UART_CON		*pUart;
	s32				Status;

	pUart = &pCtrlInfo->m_UartChTestInfo[UartCh_CH2];

	Status = UART_UartAPollingTxTest(pUart);

	return Status;
}

s32 UART_Uart3PollingTxTest(UART_CtrlInfo_st *pCtrlInfo)
{
	UART_CON		*pUart;
	s32				Status;

	pUart = &pCtrlInfo->m_UartChTestInfo[UartCh_CH3];

	Status = UART_UartAPollingTxTest(pUart);

	return Status;
}



const UART_TestFuncMenu gUART_Test_InterruptTxTest_Menu[] = 
{
	UART_SetDefaultInfo_Uart0,					"Set DefaultVar : UART0",
	UART_SetDefaultInfo_Uart1,					"Set DefaultVar : UART1",
	UART_SetDefaultInfo_Uart2,					"Set DefaultVar : UART2",
	UART_SetDefaultInfo_Uart3,					"Set DefaultVar : UART3\n",

	UART_Uart0InterruptTxTest,					"Interrupt Tx Test : UART0",
	UART_Uart1InterruptTxTest,					"Interrupt Tx Test : UART1",
	UART_Uart2InterruptTxTest,					"Interrupt Tx Test : UART2",
	UART_Uart3InterruptTxTest,					"Interrupt Tx Test : UART3\n",

	UART_SetPLL,								"Set PLL(APLL/MPLL/EPLL/VPLL)\n",

	NULL,										"Exit",
};

s32 UART_InterruptTxTest(UART_CtrlInfo_st *pCtrlInfo)
{
	s32				MenuEntryCnt;
	s32				MenuIndx;
	s32				SelNum;
	s32				TestResult = 0;
	
	MenuEntryCnt = sizeof(gUART_Test_InterruptTxTest_Menu)/sizeof(UART_TestFuncMenu);
	while(1)
	{
		/* Display Top Menu */
		UART_Printf("\n====================================================");
		UART_Printf("\n   UART Interrupt Tx Test");
		UART_Printf("\n====================================================\n");
		for(MenuIndx = 0; MenuIndx<MenuEntryCnt; MenuIndx++)
		{   
	   		UART_Printf("%2d.%s", MenuIndx, gUART_Test_InterruptTxTest_Menu[MenuIndx].desc);
			UART_Printf("\n");
		}
		UART_Printf("\n====================================================");

		UART_Printf("\nSelect: ");
		SelNum = UART_GetIntNum();
		//if(SelNum <= 0 || SelNum >= MenuEntryCnt)
		if(SelNum == MenuEntryCnt - 1)
		{
			break;
		}

		if(SelNum >= 0 && SelNum < MenuEntryCnt - 1)
		{
			if(gUART_Test_InterruptTxTest_Menu[SelNum].func != NULL)
			{
				TestResult = (gUART_Test_InterruptTxTest_Menu[SelNum].func)(pCtrlInfo);
			}
		}
	}
}


s32 UART_UartAInterruptTxTest(UART_CON *pUart)
{
	UART_BufAllocInfo_st	*pTxBufInfo;
	//UART_BufAllocInfo_st	*pRxBufInfo;
	bool					TxDone = FALSE;
	//bool					RxDone = FALSE;

	if(pUart->m_UartChIdx == UART_GetCurDebugCh())
	{
		UART_Printf("\nError : UART%d is the debug channel.", pUart->m_UartChIdx);
		
		return UART_ERROR;
	}

	pUart->m_TxMode = UartMode_Interrupt;
	pUart->m_RxMode = UartMode_Interrupt;
	pTxBufInfo = &pUart->m_TxBufNode;

	UART_Open(pUart);

	pTxBufInfo->m_pBufPtr = UART_GetNCBuf(pTxBufInfo, pUart->m_TxTransferSize);
	if(pTxBufInfo->m_pBufPtr == NULL)
	{
		UART_ASSERT();
	}

	//UART_NCBufRandInit(pTxBufInfo, pUart->m_TxTransferSize);
	UART_NCBufPatternInit(pTxBufInfo, pUart->m_TxTransferSize, gUART_pInitString_PatternA, 10);
	UART_NCBufDisplayHex(pTxBufInfo, TRUE);

	UART_InterruptTxString(pUart, UART_TxCompleteCallback, (u32)&TxDone);

	while(!UART_GetKey())
	{
		if(TxDone)
		{
			break;
		}
	}

	UART_FreeNCBuf(pTxBufInfo);

	UART_Close(pUart);
}

s32 UART_Uart0InterruptTxTest(UART_CtrlInfo_st *pCtrlInfo)
{
	UART_CON		*pUart;
	s32				Status;

	pUart = &pCtrlInfo->m_UartChTestInfo[UartCh_CH0];

	Status = UART_UartAInterruptTxTest(pUart);

	return Status;
}

s32 UART_Uart1InterruptTxTest(UART_CtrlInfo_st *pCtrlInfo)
{
	UART_CON		*pUart;
	s32				Status;

	pUart = &pCtrlInfo->m_UartChTestInfo[UartCh_CH1];

	Status = UART_UartAInterruptTxTest(pUart);

	return Status;
}

s32 UART_Uart2InterruptTxTest(UART_CtrlInfo_st *pCtrlInfo)
{
	UART_CON		*pUart;
	s32				Status;

	pUart = &pCtrlInfo->m_UartChTestInfo[UartCh_CH2];

	Status = UART_UartAInterruptTxTest(pUart);

	return Status;
}

s32 UART_Uart3InterruptTxTest(UART_CtrlInfo_st *pCtrlInfo)
{
	UART_CON		*pUart;
	s32				Status;

	pUart = &pCtrlInfo->m_UartChTestInfo[UartCh_CH3];

	Status = UART_UartAInterruptTxTest(pUart);

	return Status;
}



const UART_TestFuncMenu gUART_Test_DMATxTest_Menu[] = 
{
	UART_SetDefaultInfo_Uart0,					"Set DefaultVar : UART0",
	UART_SetDefaultInfo_Uart1,					"Set DefaultVar : UART1",
	UART_SetDefaultInfo_Uart2,					"Set DefaultVar : UART2",
	UART_SetDefaultInfo_Uart3,					"Set DefaultVar : UART3\n",

	UART_Uart0DMATxTest,						"DMA Tx Test : UART0",
	UART_Uart1DMATxTest,						"DMA Tx Test : UART1",
	UART_Uart2DMATxTest,						"DMA Tx Test : UART2",
	UART_Uart3DMATxTest,						"DMA Tx Test : UART3\n",

	UART_SetPLL,								"Set PLL(APLL/MPLL/EPLL/VPLL)\n",

	NULL,										"Exit",
};

s32 UART_DMATxTest(UART_CtrlInfo_st *pCtrlInfo)
{
	s32				MenuEntryCnt;
	s32				MenuIndx;
	s32				SelNum;
	s32				TestResult = 0;
	
	MenuEntryCnt = sizeof(gUART_Test_DMATxTest_Menu)/sizeof(UART_TestFuncMenu);
	while(1)
	{
		/* Display Top Menu */
		UART_Printf("\n====================================================");
		UART_Printf("\n   UART DMA Tx Test");
		UART_Printf("\n====================================================\n");
		for(MenuIndx = 0; MenuIndx<MenuEntryCnt; MenuIndx++)
		{   
	   		UART_Printf("%2d.%s", MenuIndx, gUART_Test_DMATxTest_Menu[MenuIndx].desc);
			UART_Printf("\n");
		}
		UART_Printf("\n====================================================");

		UART_Printf("\nSelect: ");
		SelNum = UART_GetIntNum();
		//if(SelNum <= 0 || SelNum >= MenuEntryCnt)
		if(SelNum == MenuEntryCnt - 1)
		{
			break;
		}

		if(SelNum >= 0 && SelNum < MenuEntryCnt - 1)
		{
			if(gUART_Test_DMATxTest_Menu[SelNum].func != NULL)
			{
				TestResult = (gUART_Test_DMATxTest_Menu[SelNum].func)(pCtrlInfo);
			}
		}
	}
}


s32 UART_UartADMATxTest(UART_CON *pUart)
{
	UART_BufAllocInfo_st	*pTxBufInfo;
	//UART_BufAllocInfo_st	*pRxBufInfo;
	bool					TxDone = FALSE;
	//bool					RxDone = FALSE;

	if(pUart->m_UartChIdx == UART_GetCurDebugCh())
	{
		UART_Printf("\nError : UART%d is the debug channel.", pUart->m_UartChIdx);
		
		return UART_ERROR;
	}

	pUart->m_TxMode = UartMode_DMA;
	pUart->m_RxMode = UartMode_DMA;
	pTxBufInfo = &pUart->m_TxBufNode;

	UART_Open(pUart);

	pTxBufInfo->m_pBufPtr = UART_GetNCBuf(pTxBufInfo, pUart->m_TxTransferSize);
	if(pTxBufInfo->m_pBufPtr == NULL)
	{
		UART_ASSERT();
	}

	//UART_NCBufRandInit(pTxBufInfo, pUart->m_TxTransferSize);
	UART_NCBufPatternInit(pTxBufInfo, pUart->m_TxTransferSize, gUART_pInitString_PatternA, 10);
	UART_NCBufDisplayHex(pTxBufInfo, TRUE);

	UART_DMATxString(pUart, UART_TxCompleteCallback, (u32)&TxDone);

	while(!UART_GetKey())
	{
		if(TxDone)
		{
			break;
		}
	}

	UART_FreeNCBuf(pTxBufInfo);

	UART_Close(pUart);
}

s32 UART_Uart0DMATxTest(UART_CtrlInfo_st *pCtrlInfo)
{
	UART_CON		*pUart;
	s32				Status;

	pUart = &pCtrlInfo->m_UartChTestInfo[UartCh_CH0];

	Status = UART_UartADMATxTest(pUart);

	return Status;
}

s32 UART_Uart1DMATxTest(UART_CtrlInfo_st *pCtrlInfo)
{
	UART_CON		*pUart;
	s32				Status;

	pUart = &pCtrlInfo->m_UartChTestInfo[UartCh_CH1];

	Status = UART_UartADMATxTest(pUart);

	return Status;
}

s32 UART_Uart2DMATxTest(UART_CtrlInfo_st *pCtrlInfo)
{
	UART_CON		*pUart;
	s32				Status;

	pUart = &pCtrlInfo->m_UartChTestInfo[UartCh_CH2];

	Status = UART_UartADMATxTest(pUart);

	return Status;
}

s32 UART_Uart3DMATxTest(UART_CtrlInfo_st *pCtrlInfo)
{
	UART_CON		*pUart;
	s32				Status;

	pUart = &pCtrlInfo->m_UartChTestInfo[UartCh_CH3];

	Status = UART_UartADMATxTest(pUart);

	return Status;
}


const UART_TestFuncMenu gUART_Test_PollingRxTest_Menu[] = 
{
	UART_SetDefaultInfo_Uart0,					"Set DefaultVar : UART0",
	UART_SetDefaultInfo_Uart1,					"Set DefaultVar : UART1",
	UART_SetDefaultInfo_Uart2,					"Set DefaultVar : UART2",
	UART_SetDefaultInfo_Uart3,					"Set DefaultVar : UART3\n",

	UART_Uart0PollingRxTest,					"Polling Rx Test : UART0",
	UART_Uart1PollingRxTest,					"Polling Rx Test : UART1",
	UART_Uart2PollingRxTest,					"Polling Rx Test : UART2",
	UART_Uart3PollingRxTest,					"Polling Rx Test : UART3",

	UART_SetPLL,								"Set PLL(APLL/MPLL/EPLL/VPLL)\n",

	NULL,										"Exit",
};

s32 UART_PollingRxTest(UART_CtrlInfo_st *pCtrlInfo)
{
	s32				MenuEntryCnt;
	s32				MenuIndx;
	s32				SelNum;
	s32				TestResult = 0;
	
	MenuEntryCnt = sizeof(gUART_Test_PollingRxTest_Menu)/sizeof(UART_TestFuncMenu);
	while(1)
	{
		/* Display Top Menu */
		UART_Printf("\n====================================================");
		UART_Printf("\n   UART Polling Rx Test");
		UART_Printf("\n====================================================\n");
		for(MenuIndx = 0; MenuIndx<MenuEntryCnt; MenuIndx++)
		{   
	   		UART_Printf("%2d.%s", MenuIndx, gUART_Test_PollingRxTest_Menu[MenuIndx].desc);
			UART_Printf("\n");
		}
		UART_Printf("\n====================================================");

		UART_Printf("\nSelect: ");
		SelNum = UART_GetIntNum();
		//if(SelNum <= 0 || SelNum >= MenuEntryCnt)
		if(SelNum == MenuEntryCnt - 1)
		{
			break;
		}

		if(SelNum >= 0 && SelNum < MenuEntryCnt - 1)
		{
			if(gUART_Test_PollingRxTest_Menu[SelNum].func != NULL)
			{
				TestResult = (gUART_Test_PollingRxTest_Menu[SelNum].func)(pCtrlInfo);
			}
		}
	}
}


s32 UART_UartAPollingRxTest(UART_CON *pUart)
{
	//UART_BufAllocInfo_st	*pTxBufInfo;
	UART_BufAllocInfo_st	*pRxBufInfo;
	//bool					TxDone = FALSE;
	//bool					RxDone = FALSE;

	if(pUart->m_UartChIdx == UART_GetCurDebugCh())
	{
		UART_Printf("\nError : UART%d is the debug channel.", pUart->m_UartChIdx);
		
		return UART_ERROR;
	}

	pUart->m_TxMode = UartMode_Interrupt;
	pUart->m_RxMode = UartMode_Interrupt;
	pRxBufInfo = &pUart->m_RxBufNode;

	UART_Open(pUart);

	pRxBufInfo->m_pBufPtr = UART_GetNCBuf(pRxBufInfo, pUart->m_RxTransferSize);
	if(pRxBufInfo->m_pBufPtr == NULL)
	{
		UART_ASSERT();
	}
	UART_NCBufZeroInit(pRxBufInfo);

	UART_PollingRxString(pUart);

	UART_NCBufDisplayHex(pRxBufInfo, TRUE);

	UART_FreeNCBuf(pRxBufInfo);

	UART_Close(pUart);
}

s32 UART_Uart0PollingRxTest(UART_CtrlInfo_st *pCtrlInfo)
{
	UART_CON		*pUart;
	s32				Status;

	pUart = &pCtrlInfo->m_UartChTestInfo[UartCh_CH0];

	Status = UART_UartAPollingRxTest(pUart);

	return Status;
}

s32 UART_Uart1PollingRxTest(UART_CtrlInfo_st *pCtrlInfo)
{
	UART_CON		*pUart;
	s32				Status;

	pUart = &pCtrlInfo->m_UartChTestInfo[UartCh_CH1];

	Status = UART_UartAPollingRxTest(pUart);

	return Status;
}

s32 UART_Uart2PollingRxTest(UART_CtrlInfo_st *pCtrlInfo)
{
	UART_CON		*pUart;
	s32				Status;

	pUart = &pCtrlInfo->m_UartChTestInfo[UartCh_CH2];

	Status = UART_UartAPollingRxTest(pUart);

	return Status;
}

s32 UART_Uart3PollingRxTest(UART_CtrlInfo_st *pCtrlInfo)
{
	UART_CON		*pUart;
	s32				Status;

	pUart = &pCtrlInfo->m_UartChTestInfo[UartCh_CH3];

	Status = UART_UartAPollingRxTest(pUart);

	return Status;
}



const UART_TestFuncMenu gUART_Test_InterruptRxTest_Menu[] = 
{
	UART_SetDefaultInfo_Uart0,					"Set DefaultVar : UART0",
	UART_SetDefaultInfo_Uart1,					"Set DefaultVar : UART1",
	UART_SetDefaultInfo_Uart2,					"Set DefaultVar : UART2",
	UART_SetDefaultInfo_Uart3,					"Set DefaultVar : UART3\n",

	UART_Uart0InterruptRxTest,					"Interrupt Rx Test : UART0",
	UART_Uart1InterruptRxTest,					"Interrupt Rx Test : UART1",
	UART_Uart2InterruptRxTest,					"Interrupt Rx Test : UART2",
	UART_Uart3InterruptRxTest,					"Interrupt Rx Test : UART3\n",

	UART_SetPLL,								"Set PLL(APLL/MPLL/EPLL/VPLL)\n",

	NULL,										"Exit",
};

s32 UART_InterruptRxTest(UART_CtrlInfo_st *pCtrlInfo)
{
	s32				MenuEntryCnt;
	s32				MenuIndx;
	s32				SelNum;
	s32				TestResult = 0;
	
	MenuEntryCnt = sizeof(gUART_Test_InterruptRxTest_Menu)/sizeof(UART_TestFuncMenu);
	while(1)
	{
		/* Display Top Menu */
		UART_Printf("\n====================================================");
		UART_Printf("\n   UART Interrupt Rx Test");
		UART_Printf("\n====================================================\n");
		for(MenuIndx = 0; MenuIndx<MenuEntryCnt; MenuIndx++)
		{   
	   		UART_Printf("%2d.%s", MenuIndx, gUART_Test_InterruptRxTest_Menu[MenuIndx].desc);
			UART_Printf("\n");
		}
		UART_Printf("\n====================================================");

		UART_Printf("\nSelect: ");
		SelNum = UART_GetIntNum();
		//if(SelNum <= 0 || SelNum >= MenuEntryCnt)
		if(SelNum == MenuEntryCnt - 1)
		{
			break;
		}

		if(SelNum >= 0 && SelNum < MenuEntryCnt - 1)
		{
			if(gUART_Test_InterruptRxTest_Menu[SelNum].func != NULL)
			{
				TestResult = (gUART_Test_InterruptRxTest_Menu[SelNum].func)(pCtrlInfo);
			}
		}
	}
}


s32 UART_UartAInterruptRxTest(UART_CON *pUart)
{
	//UART_BufAllocInfo_st	*pTxBufInfo;
	UART_BufAllocInfo_st	*pRxBufInfo;
	//bool					TxDone = FALSE;
	bool					RxDone = FALSE;

	if(pUart->m_UartChIdx == UART_GetCurDebugCh())
	{
		UART_Printf("\nError : UART%d is the debug channel.", pUart->m_UartChIdx);
		
		return UART_ERROR;
	}

	pUart->m_TxMode = UartMode_Interrupt;
	pUart->m_RxMode = UartMode_Interrupt;
	pRxBufInfo = &pUart->m_RxBufNode;

	UART_Open(pUart);

	pRxBufInfo->m_pBufPtr = UART_GetNCBuf(pRxBufInfo, pUart->m_RxTransferSize);
	if(pRxBufInfo->m_pBufPtr == NULL)
	{
		UART_ASSERT();
	}
	UART_NCBufZeroInit(pRxBufInfo);

	UART_InterruptRxString(pUart, UART_RxCompleteCallback, (u32)&RxDone);

	while(!UART_GetKey())
	{
		if(RxDone)
		{
			break;
		}
	}

	UART_NCBufDisplayHex(pRxBufInfo, TRUE);

	UART_FreeNCBuf(pRxBufInfo);

	UART_Close(pUart);
}

s32 UART_Uart0InterruptRxTest(UART_CtrlInfo_st *pCtrlInfo)
{
	UART_CON		*pUart;
	s32				Status;

	pUart = &pCtrlInfo->m_UartChTestInfo[UartCh_CH0];

	Status = UART_UartAInterruptRxTest(pUart);

	return Status;
}

s32 UART_Uart1InterruptRxTest(UART_CtrlInfo_st *pCtrlInfo)
{
	UART_CON		*pUart;
	s32				Status;

	pUart = &pCtrlInfo->m_UartChTestInfo[UartCh_CH1];

	Status = UART_UartAInterruptRxTest(pUart);

	return Status;
}

s32 UART_Uart2InterruptRxTest(UART_CtrlInfo_st *pCtrlInfo)
{
	UART_CON		*pUart;
	s32				Status;

	pUart = &pCtrlInfo->m_UartChTestInfo[UartCh_CH2];

	Status = UART_UartAInterruptRxTest(pUart);

	return Status;
}

s32 UART_Uart3InterruptRxTest(UART_CtrlInfo_st *pCtrlInfo)
{
	UART_CON		*pUart;
	s32				Status;

	pUart = &pCtrlInfo->m_UartChTestInfo[UartCh_CH3];

	Status = UART_UartAInterruptRxTest(pUart);

	return Status;
}



const UART_TestFuncMenu gUART_Test_DMARxTest_Menu[] = 
{
	UART_SetDefaultInfo_Uart0,					"Set DefaultVar : UART0",
	UART_SetDefaultInfo_Uart1,					"Set DefaultVar : UART1",
	UART_SetDefaultInfo_Uart2,					"Set DefaultVar : UART2",
	UART_SetDefaultInfo_Uart3,					"Set DefaultVar : UART3\n",

	UART_Uart0DMARxTest,						"DMA Rx Test : UART0",
	UART_Uart1DMARxTest,						"DMA Rx Test : UART1",
	UART_Uart2DMARxTest,						"DMA Rx Test : UART2",
	UART_Uart3DMARxTest,						"DMA Rx Test : UART3\n",

	UART_SetPLL,								"Set PLL(APLL/MPLL/EPLL/VPLL)\n",

	NULL,										"Exit",
};

s32 UART_DMARxTest(UART_CtrlInfo_st *pCtrlInfo)
{
	s32				MenuEntryCnt;
	s32				MenuIndx;
	s32				SelNum;
	s32				TestResult = 0;
	
	MenuEntryCnt = sizeof(gUART_Test_DMARxTest_Menu)/sizeof(UART_TestFuncMenu);
	while(1)
	{
		/* Display Top Menu */
		UART_Printf("\n====================================================");
		UART_Printf("\n   UART DMA Rx Test");
		UART_Printf("\n====================================================\n");
		for(MenuIndx = 0; MenuIndx<MenuEntryCnt; MenuIndx++)
		{   
	   		UART_Printf("%2d.%s", MenuIndx, gUART_Test_DMARxTest_Menu[MenuIndx].desc);
			UART_Printf("\n");
		}
		UART_Printf("\n====================================================");

		UART_Printf("\nSelect: ");
		SelNum = UART_GetIntNum();
		//if(SelNum <= 0 || SelNum >= MenuEntryCnt)
		if(SelNum == MenuEntryCnt - 1)
		{
			break;
		}

		if(SelNum >= 0 && SelNum < MenuEntryCnt - 1)
		{
			if(gUART_Test_DMARxTest_Menu[SelNum].func != NULL)
			{
				TestResult = (gUART_Test_DMARxTest_Menu[SelNum].func)(pCtrlInfo);
			}
		}
	}
}


s32 UART_UartADMARxTest(UART_CON *pUart)
{
	//UART_BufAllocInfo_st	*pTxBufInfo;
	UART_BufAllocInfo_st	*pRxBufInfo;
	//bool					TxDone = FALSE;
	bool					RxDone = FALSE;

	if(pUart->m_UartChIdx == UART_GetCurDebugCh())
	{
		UART_Printf("\nError : UART%d is the debug channel.", pUart->m_UartChIdx);
		
		return UART_ERROR;
	}

	pUart->m_TxMode = UartMode_DMA;
	pUart->m_RxMode = UartMode_DMA;
	pRxBufInfo = &pUart->m_RxBufNode;

	UART_Open(pUart);

	pRxBufInfo->m_pBufPtr = UART_GetNCBuf(pRxBufInfo, pUart->m_RxTransferSize);
	if(pRxBufInfo->m_pBufPtr == NULL)
	{
		UART_ASSERT();
	}
	UART_NCBufZeroInit(pRxBufInfo);

	UART_DMARxString(pUart, UART_RxCompleteCallback, (u32)&RxDone);

	while(!UART_GetKey())
	{
		if(RxDone)
		{
			break;
		}
	}

	UART_NCBufDisplayHex(pRxBufInfo, TRUE);

	UART_FreeNCBuf(pRxBufInfo);

	UART_Close(pUart);
}

s32 UART_Uart0DMARxTest(UART_CtrlInfo_st *pCtrlInfo)
{
	UART_CON		*pUart;
	s32				Status;

	pUart = &pCtrlInfo->m_UartChTestInfo[UartCh_CH0];

	Status = UART_UartADMARxTest(pUart);

	return Status;
}

s32 UART_Uart1DMARxTest(UART_CtrlInfo_st *pCtrlInfo)
{
	UART_CON		*pUart;
	s32				Status;

	pUart = &pCtrlInfo->m_UartChTestInfo[UartCh_CH1];

	Status = UART_UartADMARxTest(pUart);

	return Status;
}


s32 UART_Uart2DMARxTest(UART_CtrlInfo_st *pCtrlInfo)
{
	UART_CON		*pUart;
	s32				Status;

	pUart = &pCtrlInfo->m_UartChTestInfo[UartCh_CH2];

	Status = UART_UartADMARxTest(pUart);

	return Status;
}


s32 UART_Uart3DMARxTest(UART_CtrlInfo_st *pCtrlInfo)
{
	UART_CON		*pUart;
	s32				Status;

	pUart = &pCtrlInfo->m_UartChTestInfo[UartCh_CH3];

	Status = UART_UartADMARxTest(pUart);

	return Status;
}




const UART_TestFuncMenu gUART_Test_IntLoopback_PollingTest_Menu[] = 
{
	UART_SetDefaultInfo_Uart0,				"Set DefaultVar : UART0",
	UART_SetDefaultInfo_Uart1,				"Set DefaultVar : UART1",
	UART_SetDefaultInfo_Uart2,				"Set DefaultVar : UART2",
	UART_SetDefaultInfo_Uart3,				"Set DefaultVar : UART3\n",

	UART_IntLoopback_Uart0PollingTest,		"Internal Loopback Polling Test : UART0",
	UART_IntLoopback_Uart1PollingTest,		"Internal Loopback Polling Test : UART1",
	UART_IntLoopback_Uart2PollingTest,		"Internal Loopback Polling Test : UART2",
	UART_IntLoopback_Uart3PollingTest,		"Internal Loopback Polling Test : UART3\n",

	UART_SetPLL,							"Set PLL(APLL/MPLL/EPLL/VPLL)\n",

	NULL,									"Exit",
};

s32 UART_IntLoopback_Polling_Test(UART_CtrlInfo_st *pCtrlInfo)
{
	s32				MenuEntryCnt;
	s32				MenuIndx;
	s32				SelNum;
	s32				TestResult = 0;
	
	MenuEntryCnt = sizeof(gUART_Test_IntLoopback_PollingTest_Menu)/sizeof(UART_TestFuncMenu);
	while(1)
	{
		/* Display Top Menu */
		UART_Printf("\n====================================================");
		UART_Printf("\n   UART Internal Loopback Polling Test");
		UART_Printf("\n====================================================\n");
		for(MenuIndx = 0; MenuIndx<MenuEntryCnt; MenuIndx++)
		{   
	   		UART_Printf("%2d.%s", MenuIndx, gUART_Test_IntLoopback_PollingTest_Menu[MenuIndx].desc);
			UART_Printf("\n");
		}
		UART_Printf("\n====================================================");

		UART_Printf("\nSelect: ");
		SelNum = UART_GetIntNum();
		//if(SelNum <= 0 || SelNum >= MenuEntryCnt)
		if(SelNum == MenuEntryCnt - 1)
		{
			break;
		}

		if(SelNum >= 0 && SelNum < MenuEntryCnt - 1)
		{
			if(gUART_Test_IntLoopback_PollingTest_Menu[SelNum].func != NULL)
			{
				TestResult = (gUART_Test_IntLoopback_PollingTest_Menu[SelNum].func)(pCtrlInfo);
			}
		}
	}
}


s32 UART_IntLoopback_UartAPollingTest(UART_CON *pUart)
{
	UART_BufAllocInfo_st	*pTxBufInfo;
	UART_BufAllocInfo_st	*pRxBufInfo;
	bool					TxDone = FALSE;
	bool					RxDone = FALSE;
	int					ErrorCnt;

	if(pUart->m_UartChIdx == UART_GetCurDebugCh())
	{
		UART_Printf("\nError : UART%d is the debug channel.", pUart->m_UartChIdx);
		
		return UART_ERROR;
	}

	pUart->m_TxMode = UartMode_Interrupt;
	pUart->m_RxMode = UartMode_Interrupt;
	pUart->m_EnLoopBackMode = TRUE;		/* Enable Loopback Mode */
	pTxBufInfo = &pUart->m_TxBufNode;
	pRxBufInfo = &pUart->m_RxBufNode;

	UART_Open(pUart);

	pTxBufInfo->m_pBufPtr = UART_GetNCBuf(pTxBufInfo, pUart->m_TxTransferSize);
	if(pTxBufInfo->m_pBufPtr == NULL)
	{
		UART_ASSERT();
	}

	UART_NCBufRandInit(pTxBufInfo, pUart->m_TxTransferSize);
	UART_NCBufDisplayHex(pTxBufInfo, TRUE);

	pRxBufInfo->m_pBufPtr = UART_GetNCBuf(pRxBufInfo, pUart->m_RxTransferSize);
	if(pRxBufInfo->m_pBufPtr == NULL)
	{
		UART_ASSERT();
	}
	UART_NCBufZeroInit(pRxBufInfo);

	UART_LoopbackPollingTxRxString(pUart, pUart);

	ErrorCnt = UART_CompareData(pTxBufInfo->m_pBufPtr, pRxBufInfo->m_pBufPtr, pTxBufInfo->m_ReqDataSize);
	UART_NCBufDisplayHex(pRxBufInfo, TRUE);

	UART_FreeNCBuf(pTxBufInfo);
	UART_FreeNCBuf(pRxBufInfo);

	UART_Close(pUart);

	if(ErrorCnt == 0)
	{
		return UART_NO_ERROR;
	}
	else
	{
		return UART_ERROR;
	}
}

s32 UART_IntLoopback_Uart0PollingTest(UART_CtrlInfo_st *pCtrlInfo)
{
	UART_CON		*pUart;
	s32				Status;

	pUart = &pCtrlInfo->m_UartChTestInfo[UartCh_CH0];

	Status = UART_IntLoopback_UartAPollingTest(pUart);

	return Status;
}

s32 UART_IntLoopback_Uart1PollingTest(UART_CtrlInfo_st *pCtrlInfo)
{
	UART_CON		*pUart;
	s32				Status;

	pUart = &pCtrlInfo->m_UartChTestInfo[UartCh_CH1];

	Status = UART_IntLoopback_UartAPollingTest(pUart);

	return Status;
}

s32 UART_IntLoopback_Uart2PollingTest(UART_CtrlInfo_st *pCtrlInfo)
{
	UART_CON		*pUart;
	s32				Status;

	pUart = &pCtrlInfo->m_UartChTestInfo[UartCh_CH2];

	Status = UART_IntLoopback_UartAPollingTest(pUart);

	return Status;
}


s32 UART_IntLoopback_Uart3PollingTest(UART_CtrlInfo_st *pCtrlInfo)
{
	UART_CON		*pUart;
	s32				Status;

	pUart = &pCtrlInfo->m_UartChTestInfo[UartCh_CH3];

	Status = UART_IntLoopback_UartAPollingTest(pUart);

	return Status;
}





const UART_TestFuncMenu gUART_Test_IntLoopback_InterruptTest_Menu[] = 
{
	UART_SetDefaultInfo_Uart0,				"Set DefaultVar : UART0",
	UART_SetDefaultInfo_Uart1,				"Set DefaultVar : UART1",
	UART_SetDefaultInfo_Uart2,				"Set DefaultVar : UART2",
	UART_SetDefaultInfo_Uart3,				"Set DefaultVar : UART3\n",

	UART_IntLoopback_Uart0InterruptTest,	"Internal Loopback Interrupt Test : UART0",
	UART_IntLoopback_Uart1InterruptTest,	"Internal Loopback Interrupt Test : UART1",
	UART_IntLoopback_Uart2InterruptTest,	"Internal Loopback Interrupt Test : UART2",
	UART_IntLoopback_Uart3InterruptTest,	"Internal Loopback Interrupt Test : UART3\n",

	UART_SetPLL,							"Set PLL(APLL/MPLL/EPLL/VPLL)\n",

	NULL,									"Exit",
};

s32 UART_IntLoopback_Interrupt_Test(UART_CtrlInfo_st *pCtrlInfo)
{
	s32				MenuEntryCnt;
	s32				MenuIndx;
	s32				SelNum;
	s32				TestResult = 0;
	
	MenuEntryCnt = sizeof(gUART_Test_IntLoopback_InterruptTest_Menu)/sizeof(UART_TestFuncMenu);
	while(1)
	{
		/* Display Top Menu */
		UART_Printf("\n====================================================");
		UART_Printf("\n   UART Internal Loopback Interrupt Test");
		UART_Printf("\n====================================================\n");
		for(MenuIndx = 0; MenuIndx<MenuEntryCnt; MenuIndx++)
		{   
	   		UART_Printf("%2d.%s", MenuIndx, gUART_Test_IntLoopback_InterruptTest_Menu[MenuIndx].desc);
			UART_Printf("\n");
		}
		UART_Printf("\n====================================================");

		UART_Printf("\nSelect: ");
		SelNum = UART_GetIntNum();
		//if(SelNum <= 0 || SelNum >= MenuEntryCnt)
		if(SelNum == MenuEntryCnt - 1)
		{
			break;
		}

		if(SelNum >= 0 && SelNum < MenuEntryCnt - 1)
		{
			if(gUART_Test_IntLoopback_InterruptTest_Menu[SelNum].func != NULL)
			{
				TestResult = (gUART_Test_IntLoopback_InterruptTest_Menu[SelNum].func)(pCtrlInfo);
			}
		}
	}
}


s32 UART_IntLoopback_UartAInterruptTest(UART_CON *pUart)
{
	UART_BufAllocInfo_st	*pTxBufInfo;
	UART_BufAllocInfo_st	*pRxBufInfo;
	bool					TxDone = FALSE;
	bool					RxDone = FALSE;
	int					ErrorCnt ;

	if(pUart->m_UartChIdx == UART_GetCurDebugCh())
	{
		UART_Printf("\nError : UART%d is the debug channel.", pUart->m_UartChIdx);
		
		return UART_ERROR;
	}

	pUart->m_TxMode = UartMode_Interrupt;
	pUart->m_RxMode = UartMode_Interrupt;
	pUart->m_EnLoopBackMode = TRUE;		/* Enable Loopback Mode */
	pTxBufInfo = &pUart->m_TxBufNode;
	pRxBufInfo = &pUart->m_RxBufNode;

	UART_Open(pUart);

	pTxBufInfo->m_pBufPtr = UART_GetNCBuf(pTxBufInfo, pUart->m_TxTransferSize);
	if(pTxBufInfo->m_pBufPtr == NULL)
	{
		UART_ASSERT();
	}

	UART_NCBufRandInit(pTxBufInfo, pUart->m_TxTransferSize);
	UART_NCBufDisplayHex(pTxBufInfo, TRUE);

	pRxBufInfo->m_pBufPtr = UART_GetNCBuf(pRxBufInfo, pUart->m_RxTransferSize);
	if(pRxBufInfo->m_pBufPtr == NULL)
	{
		UART_ASSERT();
	}
	UART_NCBufZeroInit(pRxBufInfo);

	//UART_InterruptRxString(pUart, UART_RxCompleteCallback, (u32)&RxDone);
	//UART_InterruptTxString(pUart, UART_TxCompleteCallback, (u32)&TxDone);
	UART_LoopbackInterruptTxRxString(pUart, 
		UART_RxCompleteCallback, (u32)&RxDone, UART_TxCompleteCallback, (u32)&TxDone);

	while(!UART_GetKey())
	{
		if(RxDone)
		{
			break;
		}
	}

	ErrorCnt = UART_CompareData(pTxBufInfo->m_pBufPtr, pRxBufInfo->m_pBufPtr, pTxBufInfo->m_ReqDataSize);
	UART_NCBufDisplayHex(pRxBufInfo, TRUE);

	UART_FreeNCBuf(pTxBufInfo);
	UART_FreeNCBuf(pRxBufInfo);

	UART_Close(pUart);

	if(ErrorCnt == 0)
	{
		return UART_NO_ERROR;
	}
	else
	{
		return UART_ERROR;
	}
}

s32 UART_IntLoopback_Uart0InterruptTest(UART_CtrlInfo_st *pCtrlInfo)
{
	UART_CON		*pUart;
	s32				Status;

	pUart = &pCtrlInfo->m_UartChTestInfo[UartCh_CH0];

	Status = UART_IntLoopback_UartAInterruptTest(pUart);

	return Status;
}

s32 UART_IntLoopback_Uart1InterruptTest(UART_CtrlInfo_st *pCtrlInfo)
{
	UART_CON		*pUart;
	s32				Status;

	pUart = &pCtrlInfo->m_UartChTestInfo[UartCh_CH1];

	Status = UART_IntLoopback_UartAInterruptTest(pUart);

	return Status;
}

s32 UART_IntLoopback_Uart2InterruptTest(UART_CtrlInfo_st *pCtrlInfo)
{
	UART_CON		*pUart;
	s32				Status;

	pUart = &pCtrlInfo->m_UartChTestInfo[UartCh_CH2];

	Status = UART_IntLoopback_UartAInterruptTest(pUart);

	return Status;
}

s32 UART_IntLoopback_Uart3InterruptTest(UART_CtrlInfo_st *pCtrlInfo)
{
	UART_CON		*pUart;
	s32				Status;

	pUart = &pCtrlInfo->m_UartChTestInfo[UartCh_CH3];

	Status = UART_IntLoopback_UartAInterruptTest(pUart);

	return Status;
}



const UART_TestFuncMenu gUART_Test_IntLoopback_DMATest_Menu[] = 
{
	UART_SetDefaultInfo_Uart0,				"Set DefaultVar : UART0",
	UART_SetDefaultInfo_Uart1,				"Set DefaultVar : UART1",
	UART_SetDefaultInfo_Uart2,				"Set DefaultVar : UART2",
	UART_SetDefaultInfo_Uart3,				"Set DefaultVar : UART3\n",

	UART_IntLoopback_Uart0DMATest,		"Internal Loopback DMA Test : UART0",
	UART_IntLoopback_Uart1DMATest,		"Internal Loopback DMA Test : UART1",
	UART_IntLoopback_Uart2DMATest,		"Internal Loopback DMA Test : UART2",
	UART_IntLoopback_Uart3DMATest,		"Internal Loopback DMA Test : UART3\n",

	UART_SetPLL,							"Set PLL(APLL/MPLL/EPLL/VPLL)\n",

	NULL,									"Exit",
};

s32 UART_IntLoopback_DMA_Test(UART_CtrlInfo_st *pCtrlInfo)
{
	s32				MenuEntryCnt;
	s32				MenuIndx;
	s32				SelNum;
	s32				TestResult = 0;
	
	MenuEntryCnt = sizeof(gUART_Test_IntLoopback_DMATest_Menu)/sizeof(UART_TestFuncMenu);
	while(1)
	{
		/* Display Top Menu */
		UART_Printf("\n====================================================");
		UART_Printf("\n   UART Internal Loopback DMA Test");
		UART_Printf("\n====================================================\n");
		for(MenuIndx = 0; MenuIndx<MenuEntryCnt; MenuIndx++)
		{   
	   		UART_Printf("%2d.%s", MenuIndx, gUART_Test_IntLoopback_DMATest_Menu[MenuIndx].desc);
			UART_Printf("\n");
		}
		UART_Printf("\n====================================================");

		UART_Printf("\nSelect: ");
		SelNum = UART_GetIntNum();
		//if(SelNum <= 0 || SelNum >= MenuEntryCnt)
		if(SelNum == MenuEntryCnt - 1)
		{
			break;
		}

		if(SelNum >= 0 && SelNum < MenuEntryCnt - 1)
		{
			if(gUART_Test_IntLoopback_DMATest_Menu[SelNum].func != NULL)
			{
				TestResult = (gUART_Test_IntLoopback_DMATest_Menu[SelNum].func)(pCtrlInfo);
			}
		}
	}
}


s32 UART_IntLoopback_UartADMATest(UART_CON *pUart)
{
	UART_BufAllocInfo_st	*pTxBufInfo;
	UART_BufAllocInfo_st	*pRxBufInfo;
	bool					TxDone = FALSE;
	bool					RxDone = FALSE;
	int					ErrorCnt ;

	if(pUart->m_UartChIdx == UART_GetCurDebugCh())
	{
		UART_Printf("\nError : UART%d is the debug channel.", pUart->m_UartChIdx);
		
		return UART_ERROR;
	}

	pUart->m_TxMode = UartMode_DMA;
	pUart->m_RxMode = UartMode_DMA;
	pUart->m_EnLoopBackMode = TRUE;		/* Enable Loopback Mode */
	pTxBufInfo = &pUart->m_TxBufNode;
	pRxBufInfo = &pUart->m_RxBufNode;

	UART_Open(pUart);

	pTxBufInfo->m_pBufPtr = UART_GetNCBuf(pTxBufInfo, pUart->m_TxTransferSize);
	if(pTxBufInfo->m_pBufPtr == NULL)
	{
		UART_ASSERT();
	}

	UART_NCBufRandInit(pTxBufInfo, pUart->m_TxTransferSize);
	UART_NCBufDisplayHex(pTxBufInfo, TRUE);

	pRxBufInfo->m_pBufPtr = UART_GetNCBuf(pRxBufInfo, pUart->m_RxTransferSize);
	if(pRxBufInfo->m_pBufPtr == NULL)
	{
		UART_ASSERT();
	}
	UART_NCBufZeroInit(pRxBufInfo);

	//UART_DMARxString(pUart, UART_RxCompleteCallback, (u32)&RxDone);
	//UART_DMATxString(pUart, UART_TxCompleteCallback, (u32)&TxDone);
	UART_LoopbackDMATxRxString(pUart, 
		UART_RxCompleteCallback, (u32)&RxDone, UART_TxCompleteCallback, (u32)&TxDone);

	while(!UART_GetKey())
	{
		if(RxDone)
		{
			break;
		}
	}

	ErrorCnt = UART_CompareData(pTxBufInfo->m_pBufPtr, pRxBufInfo->m_pBufPtr, pTxBufInfo->m_ReqDataSize);
	UART_NCBufDisplayHex(pRxBufInfo, TRUE);

	UART_FreeNCBuf(pTxBufInfo);
	UART_FreeNCBuf(pRxBufInfo);

	UART_Close(pUart);

	if(ErrorCnt == 0)
	{
		return UART_NO_ERROR;
	}
	else
	{
		return UART_ERROR;
	}
}

s32 UART_IntLoopback_Uart0DMATest(UART_CtrlInfo_st *pCtrlInfo)
{
	UART_CON		*pUart;
	s32				Status;

	pUart = &pCtrlInfo->m_UartChTestInfo[UartCh_CH0];

	Status = UART_IntLoopback_UartADMATest(pUart);

	return Status;
}

s32 UART_IntLoopback_Uart1DMATest(UART_CtrlInfo_st *pCtrlInfo)
{
	UART_CON		*pUart;
	s32				Status;

	pUart = &pCtrlInfo->m_UartChTestInfo[UartCh_CH1];

	Status = UART_IntLoopback_UartADMATest(pUart);

	return Status;
}

s32 UART_IntLoopback_Uart2DMATest(UART_CtrlInfo_st *pCtrlInfo)
{
	UART_CON		*pUart;
	s32				Status;

	pUart = &pCtrlInfo->m_UartChTestInfo[UartCh_CH2];

	Status = UART_IntLoopback_UartADMATest(pUart);

	return Status;
}

s32 UART_IntLoopback_Uart3DMATest(UART_CtrlInfo_st *pCtrlInfo)
{
	UART_CON		*pUart;
	s32				Status;

	pUart = &pCtrlInfo->m_UartChTestInfo[UartCh_CH3];

	Status = UART_IntLoopback_UartADMATest(pUart);

	return Status;
}



const UART_TestFuncMenu gUART_Test_ExtLoopback_PollingTest_Menu[] = 
{
	UART_SetDefaultInfo_Uart0,				"Set DefaultVar : UART0",
	UART_SetDefaultInfo_Uart1,				"Set DefaultVar : UART1",
	UART_SetDefaultInfo_Uart2,				"Set DefaultVar : UART2",
	UART_SetDefaultInfo_Uart3,				"Set DefaultVar : UART3\n",

	UART_ExtLoopback_Uart0PollingTest,		"External Loopback Polling Test : UART0",
	UART_ExtLoopback_Uart1PollingTest,		"External Loopback Polling Test : UART1",
	UART_ExtLoopback_Uart2PollingTest,		"External Loopback Polling Test : UART2",
	UART_ExtLoopback_Uart3PollingTest,		"External Loopback Polling Test : UART3\n",

	UART_SetPLL,							"Set PLL(APLL/MPLL/EPLL/VPLL)\n",

	NULL,									"Exit",
};

s32 UART_ExtLoopback_Polling_Test(UART_CtrlInfo_st *pCtrlInfo)
{
	s32				MenuEntryCnt;
	s32				MenuIndx;
	s32				SelNum;
	s32				TestResult = 0;
	
	MenuEntryCnt = sizeof(gUART_Test_ExtLoopback_PollingTest_Menu)/sizeof(UART_TestFuncMenu);
	while(1)
	{
		/* Display Top Menu */
		UART_Printf("\n====================================================");
		UART_Printf("\n   UART External Loopback Polling Test");
		UART_Printf("\n====================================================\n");
		for(MenuIndx = 0; MenuIndx<MenuEntryCnt; MenuIndx++)
		{   
	   		UART_Printf("%2d.%s", MenuIndx, gUART_Test_ExtLoopback_PollingTest_Menu[MenuIndx].desc);
			UART_Printf("\n");
		}
		UART_Printf("\n====================================================");

		UART_Printf("\nSelect: ");
		SelNum = UART_GetIntNum();
		//if(SelNum <= 0 || SelNum >= MenuEntryCnt)
		if(SelNum == MenuEntryCnt - 1)
		{
			break;
		}

		if(SelNum >= 0 && SelNum < MenuEntryCnt - 1)
		{
			if(gUART_Test_ExtLoopback_PollingTest_Menu[SelNum].func != NULL)
			{
				TestResult = (gUART_Test_ExtLoopback_PollingTest_Menu[SelNum].func)(pCtrlInfo);
			}
		}
	}
}


s32 UART_ExtLoopback_UartAPollingTest(UART_CON *pUart)
{
	UART_BufAllocInfo_st	*pTxBufInfo;
	UART_BufAllocInfo_st	*pRxBufInfo;
	bool					TxDone = FALSE;
	bool					RxDone = FALSE;
	int					ErrorCnt ;

	if(pUart->m_UartChIdx == UART_GetCurDebugCh())
	{
		UART_Printf("\nError : UART%d is the debug channel.", pUart->m_UartChIdx);
		
		return UART_ERROR;
	}

	pUart->m_TxMode = UartMode_Interrupt;
	pUart->m_RxMode = UartMode_Interrupt;
	pUart->m_EnLoopBackMode = FALSE;		/* Disable Loopback Mode */
	pTxBufInfo = &pUart->m_TxBufNode;
	pRxBufInfo = &pUart->m_RxBufNode;

	UART_Open(pUart);

	pTxBufInfo->m_pBufPtr = UART_GetNCBuf(pTxBufInfo, pUart->m_TxTransferSize);
	if(pTxBufInfo->m_pBufPtr == NULL)
	{
		UART_ASSERT();
	}

	UART_NCBufRandInit(pTxBufInfo, pUart->m_TxTransferSize);
	UART_NCBufDisplayHex(pTxBufInfo, TRUE);

	pRxBufInfo->m_pBufPtr = UART_GetNCBuf(pRxBufInfo, pUart->m_RxTransferSize);
	if(pRxBufInfo->m_pBufPtr == NULL)
	{
		UART_ASSERT();
	}
	UART_NCBufZeroInit(pRxBufInfo);

	UART_LoopbackPollingTxRxString(pUart, pUart);

	ErrorCnt = UART_CompareData(pTxBufInfo->m_pBufPtr, pRxBufInfo->m_pBufPtr, pTxBufInfo->m_ReqDataSize);
	UART_NCBufDisplayHex(pRxBufInfo, TRUE);

	UART_FreeNCBuf(pTxBufInfo);
	UART_FreeNCBuf(pRxBufInfo);

	UART_Close(pUart);

	if(ErrorCnt == 0)
	{
		return UART_NO_ERROR;
	}
	else
	{
		return UART_ERROR;
	}
}

s32 UART_ExtLoopback_Uart0PollingTest(UART_CtrlInfo_st *pCtrlInfo)
{
	UART_CON		*pUart;
	s32				Status;

	pUart = &pCtrlInfo->m_UartChTestInfo[UartCh_CH0];

	Status = UART_ExtLoopback_UartAPollingTest(pUart);

	return Status;
}

s32 UART_ExtLoopback_Uart1PollingTest(UART_CtrlInfo_st *pCtrlInfo)
{
	UART_CON		*pUart;
	s32				Status;

	pUart = &pCtrlInfo->m_UartChTestInfo[UartCh_CH1];

	Status = UART_ExtLoopback_UartAPollingTest(pUart);

	return Status;
}

s32 UART_ExtLoopback_Uart2PollingTest(UART_CtrlInfo_st *pCtrlInfo)
{
	UART_CON		*pUart;
	s32				Status;

	pUart = &pCtrlInfo->m_UartChTestInfo[UartCh_CH2];

	Status = UART_ExtLoopback_UartAPollingTest(pUart);

	return Status;
}

s32 UART_ExtLoopback_Uart3PollingTest(UART_CtrlInfo_st *pCtrlInfo)
{
	UART_CON		*pUart;
	s32				Status;

	pUart = &pCtrlInfo->m_UartChTestInfo[UartCh_CH3];

	Status = UART_ExtLoopback_UartAPollingTest(pUart);

	return Status;
}


const UART_TestFuncMenu gUART_Test_ExtLoopback_InterruptTest_Menu[] = 
{
	UART_SetDefaultInfo_Uart0,				"Set DefaultVar : UART0",
	UART_SetDefaultInfo_Uart1,				"Set DefaultVar : UART1",
	UART_SetDefaultInfo_Uart2,				"Set DefaultVar : UART2",
	UART_SetDefaultInfo_Uart3,				"Set DefaultVar : UART3\n",

	UART_ExtLoopback_Uart0InterruptTest,	"External Loopback Interrupt Test : UART0",
	UART_ExtLoopback_Uart1InterruptTest,	"External Loopback Interrupt Test : UART1",
	UART_ExtLoopback_Uart2InterruptTest,	"External Loopback Interrupt Test : UART2",
	UART_ExtLoopback_Uart3InterruptTest,	"External Loopback Interrupt Test : UART3",

	UART_SetPLL,							"Set PLL(APLL/MPLL/EPLL/VPLL)\n",

	NULL,									"Exit",
};

s32 UART_ExtLoopback_Interrupt_Test(UART_CtrlInfo_st *pCtrlInfo)
{
	s32				MenuEntryCnt;
	s32				MenuIndx;
	s32				SelNum;
	s32				TestResult = 0;
	
	MenuEntryCnt = sizeof(gUART_Test_ExtLoopback_InterruptTest_Menu)/sizeof(UART_TestFuncMenu);
	while(1)
	{
		/* Display Top Menu */
		UART_Printf("\n====================================================");
		UART_Printf("\n   UART External Loopback Interrupt Test");
		UART_Printf("\n====================================================\n");
		for(MenuIndx = 0; MenuIndx<MenuEntryCnt; MenuIndx++)
		{   
	   		UART_Printf("%2d.%s", MenuIndx, gUART_Test_ExtLoopback_InterruptTest_Menu[MenuIndx].desc);
			UART_Printf("\n");
		}
		UART_Printf("\n====================================================");

		UART_Printf("\nSelect: ");
		SelNum = UART_GetIntNum();
		//if(SelNum <= 0 || SelNum >= MenuEntryCnt)
		if(SelNum == MenuEntryCnt - 1)
		{
			break;
		}

		if(SelNum >= 0 && SelNum < MenuEntryCnt - 1)
		{
			if(gUART_Test_ExtLoopback_InterruptTest_Menu[SelNum].func != NULL)
			{
				TestResult = (gUART_Test_ExtLoopback_InterruptTest_Menu[SelNum].func)(pCtrlInfo);
			}
		}
	}
}


s32 UART_ExtLoopback_UartAInterruptTest(UART_CON *pUart)
{
	UART_BufAllocInfo_st	*pTxBufInfo;
	UART_BufAllocInfo_st	*pRxBufInfo;
	bool					TxDone = FALSE;
	bool					RxDone = FALSE;
	int					ErrorCnt ;

	if(pUart->m_UartChIdx == UART_GetCurDebugCh())
	{
		UART_Printf("\nError : UART%d is the debug channel.", pUart->m_UartChIdx);
		
		return UART_ERROR;
	}

	pUart->m_TxMode = UartMode_Interrupt;
	pUart->m_RxMode = UartMode_Interrupt;
	pUart->m_EnLoopBackMode = FALSE;		/* Disable Loopback Mode */
	pTxBufInfo = &pUart->m_TxBufNode;
	pRxBufInfo = &pUart->m_RxBufNode;

	UART_Open(pUart);

	pTxBufInfo->m_pBufPtr = UART_GetNCBuf(pTxBufInfo, pUart->m_TxTransferSize);
	if(pTxBufInfo->m_pBufPtr == NULL)
	{
		UART_ASSERT();
	}

	UART_NCBufRandInit(pTxBufInfo, pUart->m_TxTransferSize);
	UART_NCBufDisplayHex(pTxBufInfo, TRUE);

	pRxBufInfo->m_pBufPtr = UART_GetNCBuf(pRxBufInfo, pUart->m_RxTransferSize);
	if(pRxBufInfo->m_pBufPtr == NULL)
	{
		UART_ASSERT();
	}
	UART_NCBufZeroInit(pRxBufInfo);

	//UART_InterruptRxString(pUart, UART_RxCompleteCallback, (u32)&RxDone);
	//UART_InterruptTxString(pUart, UART_TxCompleteCallback, (u32)&TxDone);
	UART_LoopbackInterruptTxRxString(pUart, 
		UART_RxCompleteCallback, (u32)&RxDone, UART_TxCompleteCallback, (u32)&TxDone);

	while(!UART_GetKey())
	{
		if(RxDone)
		{
			break;
		}
	}

	ErrorCnt = UART_CompareData(pTxBufInfo->m_pBufPtr, pRxBufInfo->m_pBufPtr, pTxBufInfo->m_ReqDataSize);
	UART_NCBufDisplayHex(pRxBufInfo, TRUE);

	UART_FreeNCBuf(pTxBufInfo);
	UART_FreeNCBuf(pRxBufInfo);

	UART_Close(pUart);

	if(ErrorCnt == 0)
	{
		return UART_NO_ERROR;
	}
	else
	{
		return UART_ERROR;
	}
}

s32 UART_ExtLoopback_Uart0InterruptTest(UART_CtrlInfo_st *pCtrlInfo)
{
	UART_CON		*pUart;
	s32				Status;

	pUart = &pCtrlInfo->m_UartChTestInfo[UartCh_CH0];

	Status = UART_ExtLoopback_UartAInterruptTest(pUart);

	return Status;
}

s32 UART_ExtLoopback_Uart1InterruptTest(UART_CtrlInfo_st *pCtrlInfo)
{
	UART_CON		*pUart;
	s32				Status;

	pUart = &pCtrlInfo->m_UartChTestInfo[UartCh_CH1];

	Status = UART_ExtLoopback_UartAInterruptTest(pUart);

	return Status;
}


s32 UART_ExtLoopback_Uart2InterruptTest(UART_CtrlInfo_st *pCtrlInfo)
{
	UART_CON		*pUart;
	s32				Status;

	pUart = &pCtrlInfo->m_UartChTestInfo[UartCh_CH2];

	Status = UART_ExtLoopback_UartAInterruptTest(pUart);

	return Status;
}


s32 UART_ExtLoopback_Uart3InterruptTest(UART_CtrlInfo_st *pCtrlInfo)
{
	UART_CON		*pUart;
	s32				Status;

	pUart = &pCtrlInfo->m_UartChTestInfo[UartCh_CH3];

	Status = UART_ExtLoopback_UartAInterruptTest(pUart);

	return Status;
}




const UART_TestFuncMenu gUART_Test_ExtLoopback_DMATest_Menu[] = 
{
	UART_SetDefaultInfo_Uart0,				"Set DefaultVar : UART0",
	UART_SetDefaultInfo_Uart1,				"Set DefaultVar : UART1",
	UART_SetDefaultInfo_Uart2,				"Set DefaultVar : UART2",
	UART_SetDefaultInfo_Uart3,				"Set DefaultVar : UART3\n",

	UART_ExtLoopback_Uart0DMATest,		"External Loopback DMA Test : UART0",
	UART_ExtLoopback_Uart1DMATest,		"External Loopback DMA Test : UART1",
	UART_ExtLoopback_Uart2DMATest,		"External Loopback DMA Test : UART2",
	UART_ExtLoopback_Uart3DMATest,		"External Loopback DMA Test : UART3\n",

	UART_SetPLL,							"Set PLL(APLL/MPLL/EPLL/VPLL)\n",

	NULL,									"Exit",
};

s32 UART_ExtLoopback_DMA_Test(UART_CtrlInfo_st *pCtrlInfo)
{
	s32				MenuEntryCnt;
	s32				MenuIndx;
	s32				SelNum;
	s32				TestResult = 0;
	
	MenuEntryCnt = sizeof(gUART_Test_ExtLoopback_DMATest_Menu)/sizeof(UART_TestFuncMenu);
	while(1)
	{
		/* Display Top Menu */
		UART_Printf("\n====================================================");
		UART_Printf("\n   UART External Loopback DMA Test");
		UART_Printf("\n====================================================\n");
		for(MenuIndx = 0; MenuIndx<MenuEntryCnt; MenuIndx++)
		{   
	   		UART_Printf("%2d.%s", MenuIndx, gUART_Test_ExtLoopback_DMATest_Menu[MenuIndx].desc);
			UART_Printf("\n");
		}
		UART_Printf("\n====================================================");

		UART_Printf("\nSelect: ");
		SelNum = UART_GetIntNum();
		//if(SelNum <= 0 || SelNum >= MenuEntryCnt)
		if(SelNum == MenuEntryCnt - 1)
		{
			break;
		}

		if(SelNum >= 0 && SelNum < MenuEntryCnt - 1)
		{
			if(gUART_Test_ExtLoopback_DMATest_Menu[SelNum].func != NULL)
			{
				TestResult = (gUART_Test_ExtLoopback_DMATest_Menu[SelNum].func)(pCtrlInfo);
			}
		}
	}
}


s32 UART_ExtLoopback_UartADMATest(UART_CON *pUart)
{
	UART_BufAllocInfo_st	*pTxBufInfo;
	UART_BufAllocInfo_st	*pRxBufInfo;
	bool					TxDone = FALSE;
	bool					RxDone = FALSE;
	int					ErrorCnt ;

	if(pUart->m_UartChIdx == UART_GetCurDebugCh())
	{
		UART_Printf("\nError : UART%d is the debug channel.", pUart->m_UartChIdx);
		
		return UART_ERROR;
	}

	pUart->m_TxMode = UartMode_DMA;
	pUart->m_RxMode = UartMode_DMA;
	pUart->m_EnLoopBackMode = TRUE;		/* Enable Loopback Mode */
	pTxBufInfo = &pUart->m_TxBufNode;
	pRxBufInfo = &pUart->m_RxBufNode;

	UART_Open(pUart);

	pTxBufInfo->m_pBufPtr = UART_GetNCBuf(pTxBufInfo, pUart->m_TxTransferSize);
	if(pTxBufInfo->m_pBufPtr == NULL)
	{
		UART_ASSERT();
	}

	UART_NCBufRandInit(pTxBufInfo, pUart->m_TxTransferSize);
	UART_NCBufDisplayHex(pTxBufInfo, TRUE);

	pRxBufInfo->m_pBufPtr = UART_GetNCBuf(pRxBufInfo, pUart->m_RxTransferSize);
	if(pRxBufInfo->m_pBufPtr == NULL)
	{
		UART_ASSERT();
	}
	UART_NCBufZeroInit(pRxBufInfo);

	//UART_DMARxString(pUart, UART_RxCompleteCallback, (u32)&RxDone);
	//UART_DMATxString(pUart, UART_TxCompleteCallback, (u32)&TxDone);
	UART_LoopbackDMATxRxString(pUart, 
		UART_RxCompleteCallback, (u32)&RxDone, UART_TxCompleteCallback, (u32)&TxDone);

	while(!UART_GetKey())
	{
		if(RxDone)
		{
			break;
		}
	}

	ErrorCnt = UART_CompareData(pTxBufInfo->m_pBufPtr, pRxBufInfo->m_pBufPtr, pTxBufInfo->m_ReqDataSize);
	UART_NCBufDisplayHex(pRxBufInfo, TRUE);

	UART_FreeNCBuf(pTxBufInfo);
	UART_FreeNCBuf(pRxBufInfo);

	UART_Close(pUart);

	if(ErrorCnt == 0)
	{
		return UART_NO_ERROR;
	}
	else
	{
		return UART_ERROR;
	}
}

s32 UART_ExtLoopback_Uart0DMATest(UART_CtrlInfo_st *pCtrlInfo)
{
	UART_CON		*pUart;
	s32				Status;

	pUart = &pCtrlInfo->m_UartChTestInfo[UartCh_CH0];

	Status = UART_ExtLoopback_UartADMATest(pUart);

	return Status;
}

s32 UART_ExtLoopback_Uart1DMATest(UART_CtrlInfo_st *pCtrlInfo)
{
	UART_CON		*pUart;
	s32				Status;

	pUart = &pCtrlInfo->m_UartChTestInfo[UartCh_CH1];

	Status = UART_ExtLoopback_UartADMATest(pUart);

	return Status;
}

s32 UART_ExtLoopback_Uart2DMATest(UART_CtrlInfo_st *pCtrlInfo)
{
	UART_CON		*pUart;
	s32				Status;

	pUart = &pCtrlInfo->m_UartChTestInfo[UartCh_CH2];

	Status = UART_ExtLoopback_UartADMATest(pUart);

	return Status;
}

s32 UART_ExtLoopback_Uart3DMATest(UART_CtrlInfo_st *pCtrlInfo)
{
	UART_CON		*pUart;
	s32				Status;

	pUart = &pCtrlInfo->m_UartChTestInfo[UartCh_CH3];

	Status = UART_ExtLoopback_UartADMATest(pUart);

	return Status;
}




const UART_TestFuncMenu gUART_Test_IntChLoopback_PollingTest_Menu[] = 
{
	UART_SetDefaultInfo_Uart0,					"Set DefaultVar : UART0",
	UART_SetDefaultInfo_Uart1,					"Set DefaultVar : UART1",
	UART_SetDefaultInfo_Uart2,					"Set DefaultVar : UART2",
	UART_SetDefaultInfo_Uart3,					"Set DefaultVar : UART3\n",

	UART_IntChLoopback_Uart0Uart1PollingTest,	"Inter-Channel Loopback Polling Test : UART0 - UART1",
	UART_IntChLoopback_Uart0Uart2PollingTest,	"Inter-Channel Loopback Polling Test : UART0 - UART2",
	UART_IntChLoopback_Uart0Uart3PollingTest,	"Inter-Channel Loopback Polling Test : UART0 - UART3",
	UART_IntChLoopback_Uart1Uart0PollingTest,	"Inter-Channel Loopback Polling Test : UART1 - UART0",
	UART_IntChLoopback_Uart1Uart2PollingTest,	"Inter-Channel Loopback Polling Test : UART1 - UART2",
	UART_IntChLoopback_Uart1Uart3PollingTest,	"Inter-Channel Loopback Polling Test : UART1 - UART3",
	UART_IntChLoopback_Uart2Uart0PollingTest,	"Inter-Channel Loopback Polling Test : UART2 - UART0",
	UART_IntChLoopback_Uart2Uart1PollingTest,	"Inter-Channel Loopback Polling Test : UART2 - UART1",
	UART_IntChLoopback_Uart2Uart3PollingTest,	"Inter-Channel Loopback Polling Test : UART2 - UART3",
	UART_IntChLoopback_Uart3Uart0PollingTest,	"Inter-Channel Loopback Polling Test : UART3 - UART0",
	UART_IntChLoopback_Uart3Uart1PollingTest,	"Inter-Channel Loopback Polling Test : UART3 - UART1",
	UART_IntChLoopback_Uart3Uart2PollingTest,	"Inter-Channel Loopback Polling Test : UART3 - UART2\n",

	UART_SetPLL,								"Set PLL(APLL/MPLL/EPLL/VPLL)\n",

	NULL,										"Exit",
};

s32 UART_IntChLoopback_Polling_Test(UART_CtrlInfo_st *pCtrlInfo)
{
	s32				MenuEntryCnt;
	s32				MenuIndx;
	s32				SelNum;
	s32				TestResult = 0;
	
	MenuEntryCnt = sizeof(gUART_Test_IntChLoopback_PollingTest_Menu)/sizeof(UART_TestFuncMenu);
	while(1)
	{
		/* Display Top Menu */
		UART_Printf("\n====================================================");
		UART_Printf("\n   UART Inter Channel Loopback Polling Test");
		UART_Printf("\n====================================================\n");
		for(MenuIndx = 0; MenuIndx<MenuEntryCnt; MenuIndx++)
		{   
	   		UART_Printf("%2d.%s", MenuIndx, gUART_Test_IntChLoopback_PollingTest_Menu[MenuIndx].desc);
			UART_Printf("\n");
		}
		UART_Printf("\n====================================================");

		UART_Printf("\nSelect: ");
		SelNum = UART_GetIntNum();
		//if(SelNum <= 0 || SelNum >= MenuEntryCnt)
		if(SelNum == MenuEntryCnt - 1)
		{
			break;
		}

		if(SelNum >= 0 && SelNum < MenuEntryCnt - 1)
		{
			if(gUART_Test_IntChLoopback_PollingTest_Menu[SelNum].func != NULL)
			{
				TestResult = (gUART_Test_IntChLoopback_PollingTest_Menu[SelNum].func)(pCtrlInfo);
			}
		}
	}
}


s32 UART_IntChLoopback_UartAUartBPollingTest(UART_CON *pUartTx, UART_CON *pUartRx)
{
	UART_BufAllocInfo_st	*pTxBufInfo;
	UART_BufAllocInfo_st	*pRxBufInfo;
	bool					TxDone = FALSE;
	bool					RxDone = FALSE;
	int					ErrorCnt ;

	if( pUartTx->m_UartChIdx == UART_GetCurDebugCh() || pUartRx->m_UartChIdx == UART_GetCurDebugCh() )
	{
		UART_Printf("\nError : UART%d is the debug channel.", UART_GetCurDebugCh());
		
		return UART_ERROR;
	}

	pUartTx->m_TxMode = UartMode_Interrupt;
	pUartTx->m_RxMode = UartMode_Interrupt;
	pUartRx->m_TxMode = UartMode_Interrupt;
	pUartRx->m_RxMode = UartMode_Interrupt;
	pTxBufInfo = &pUartTx->m_TxBufNode;
	pRxBufInfo = &pUartRx->m_RxBufNode;

	UART_Open(pUartTx);
	UART_Open(pUartRx);

	pTxBufInfo->m_pBufPtr = UART_GetNCBuf(pTxBufInfo, pUartTx->m_TxTransferSize);
	if(pTxBufInfo->m_pBufPtr == NULL)
	{
		UART_ASSERT();
	}

	UART_NCBufRandInit(pTxBufInfo, pUartTx->m_TxTransferSize);
	UART_NCBufDisplayHex(pTxBufInfo, TRUE);

	pRxBufInfo->m_pBufPtr = UART_GetNCBuf(pRxBufInfo, pUartRx->m_RxTransferSize);
	if(pRxBufInfo->m_pBufPtr == NULL)
	{
		UART_ASSERT();
	}
	UART_NCBufZeroInit(pRxBufInfo);

	UART_LoopbackPollingTxRxString(pUartTx, pUartRx);

	ErrorCnt = UART_CompareData(pTxBufInfo->m_pBufPtr, pRxBufInfo->m_pBufPtr, pTxBufInfo->m_ReqDataSize);
	UART_NCBufDisplayHex(pRxBufInfo, TRUE);

	UART_FreeNCBuf(pTxBufInfo);
	UART_FreeNCBuf(pRxBufInfo);

	UART_Close(pUartTx);
	UART_Close(pUartRx);

	if(ErrorCnt == 0)
	{
		return UART_NO_ERROR;
	}
	else
	{
		return UART_ERROR;
	}
}

s32 UART_IntChLoopback_Uart0Uart1PollingTest(UART_CtrlInfo_st *pCtrlInfo)
{
	UART_CON		*pUartTx;
	UART_CON		*pUartRx;
	s32				Status;

	pUartTx = &pCtrlInfo->m_UartChTestInfo[UartCh_CH0];
	pUartRx = &pCtrlInfo->m_UartChTestInfo[UartCh_CH1];

	Status = UART_IntChLoopback_UartAUartBPollingTest(pUartTx, pUartRx);

	return Status;
}

s32 UART_IntChLoopback_Uart0Uart2PollingTest(UART_CtrlInfo_st *pCtrlInfo)
{
	UART_CON		*pUartTx;
	UART_CON		*pUartRx;
	s32				Status;

	pUartTx = &pCtrlInfo->m_UartChTestInfo[UartCh_CH0];
	pUartRx = &pCtrlInfo->m_UartChTestInfo[UartCh_CH2];

	Status = UART_IntChLoopback_UartAUartBPollingTest(pUartTx, pUartRx);

	return Status;
}
s32 UART_IntChLoopback_Uart0Uart3PollingTest(UART_CtrlInfo_st *pCtrlInfo)
{
	UART_CON		*pUartTx;
	UART_CON		*pUartRx;
	s32				Status;

	pUartTx = &pCtrlInfo->m_UartChTestInfo[UartCh_CH0];
	pUartRx = &pCtrlInfo->m_UartChTestInfo[UartCh_CH3];

	Status = UART_IntChLoopback_UartAUartBPollingTest(pUartTx, pUartRx);

	return Status;
}

s32 UART_IntChLoopback_Uart1Uart0PollingTest(UART_CtrlInfo_st *pCtrlInfo)
{
	UART_CON		*pUartTx;
	UART_CON		*pUartRx;
	s32				Status;

	pUartTx = &pCtrlInfo->m_UartChTestInfo[UartCh_CH1];
	pUartRx = &pCtrlInfo->m_UartChTestInfo[UartCh_CH0];

	Status = UART_IntChLoopback_UartAUartBPollingTest(pUartTx, pUartRx);

	return Status;
}

s32 UART_IntChLoopback_Uart1Uart2PollingTest(UART_CtrlInfo_st *pCtrlInfo)
{
	UART_CON		*pUartTx;
	UART_CON		*pUartRx;
	s32				Status;

	pUartTx = &pCtrlInfo->m_UartChTestInfo[UartCh_CH1];
	pUartRx = &pCtrlInfo->m_UartChTestInfo[UartCh_CH2];

	Status = UART_IntChLoopback_UartAUartBPollingTest(pUartTx, pUartRx);

	return Status;
}

s32 UART_IntChLoopback_Uart1Uart3PollingTest(UART_CtrlInfo_st *pCtrlInfo)
{
	UART_CON		*pUartTx;
	UART_CON		*pUartRx;
	s32				Status;

	pUartTx = &pCtrlInfo->m_UartChTestInfo[UartCh_CH1];
	pUartRx = &pCtrlInfo->m_UartChTestInfo[UartCh_CH3];

	Status = UART_IntChLoopback_UartAUartBPollingTest(pUartTx, pUartRx);

	return Status;
}

s32 UART_IntChLoopback_Uart2Uart0PollingTest(UART_CtrlInfo_st *pCtrlInfo)
{
	UART_CON		*pUartTx;
	UART_CON		*pUartRx;
	s32				Status;

	pUartTx = &pCtrlInfo->m_UartChTestInfo[UartCh_CH2];
	pUartRx = &pCtrlInfo->m_UartChTestInfo[UartCh_CH0];

	Status = UART_IntChLoopback_UartAUartBPollingTest(pUartTx, pUartRx);

	return Status;
}

s32 UART_IntChLoopback_Uart2Uart1PollingTest(UART_CtrlInfo_st *pCtrlInfo)
{
	UART_CON		*pUartTx;
	UART_CON		*pUartRx;
	s32				Status;

	pUartTx = &pCtrlInfo->m_UartChTestInfo[UartCh_CH2];
	pUartRx = &pCtrlInfo->m_UartChTestInfo[UartCh_CH1];

	Status = UART_IntChLoopback_UartAUartBPollingTest(pUartTx, pUartRx);

	return Status;
}

s32 UART_IntChLoopback_Uart2Uart3PollingTest(UART_CtrlInfo_st *pCtrlInfo)
{
	UART_CON		*pUartTx;
	UART_CON		*pUartRx;
	s32				Status;

	pUartTx = &pCtrlInfo->m_UartChTestInfo[UartCh_CH2];
	pUartRx = &pCtrlInfo->m_UartChTestInfo[UartCh_CH3];

	Status = UART_IntChLoopback_UartAUartBPollingTest(pUartTx, pUartRx);

	return Status;
}

s32 UART_IntChLoopback_Uart3Uart0PollingTest(UART_CtrlInfo_st *pCtrlInfo)
{
	UART_CON		*pUartTx;
	UART_CON		*pUartRx;
	s32				Status;

	pUartTx = &pCtrlInfo->m_UartChTestInfo[UartCh_CH3];
	pUartRx = &pCtrlInfo->m_UartChTestInfo[UartCh_CH0];

	Status = UART_IntChLoopback_UartAUartBPollingTest(pUartTx, pUartRx);

	return Status;
}

s32 UART_IntChLoopback_Uart3Uart1PollingTest(UART_CtrlInfo_st *pCtrlInfo)
{
	UART_CON		*pUartTx;
	UART_CON		*pUartRx;
	s32				Status;

	pUartTx = &pCtrlInfo->m_UartChTestInfo[UartCh_CH3];
	pUartRx = &pCtrlInfo->m_UartChTestInfo[UartCh_CH1];

	Status = UART_IntChLoopback_UartAUartBPollingTest(pUartTx, pUartRx);

	return Status;
}

s32 UART_IntChLoopback_Uart3Uart2PollingTest(UART_CtrlInfo_st *pCtrlInfo)
{
	UART_CON		*pUartTx;
	UART_CON		*pUartRx;
	s32				Status;

	pUartTx = &pCtrlInfo->m_UartChTestInfo[UartCh_CH3];
	pUartRx = &pCtrlInfo->m_UartChTestInfo[UartCh_CH2];

	Status = UART_IntChLoopback_UartAUartBPollingTest(pUartTx, pUartRx);

	return Status;
}


const UART_TestFuncMenu gUART_Test_IntChLoopback_InterruptTest_Menu[] = 
{
	UART_SetDefaultInfo_Uart0,						"Set DefaultVar : UART0",
	UART_SetDefaultInfo_Uart1,						"Set DefaultVar : UART1",
	UART_SetDefaultInfo_Uart2,						"Set DefaultVar : UART2",
	UART_SetDefaultInfo_Uart3,						"Set DefaultVar : UART3\n",

	UART_IntChLoopback_Uart0Uart1InterruptTest,	"Inter-Channel Loopback Interrupt Test : UART0 - UART1",
	UART_IntChLoopback_Uart0Uart2InterruptTest,	"Inter-Channel Loopback Interrupt Test : UART0 - UART2",
	UART_IntChLoopback_Uart0Uart3InterruptTest,	"Inter-Channel Loopback Interrupt Test : UART0 - UART3",
	UART_IntChLoopback_Uart1Uart0InterruptTest,	"Inter-Channel Loopback Interrupt Test : UART1 - UART0",
	UART_IntChLoopback_Uart1Uart2InterruptTest,	"Inter-Channel Loopback Interrupt Test : UART1 - UART2",
	UART_IntChLoopback_Uart1Uart3InterruptTest,	"Inter-Channel Loopback Interrupt Test : UART1 - UART3",
	UART_IntChLoopback_Uart2Uart0InterruptTest,	"Inter-Channel Loopback Interrupt Test : UART2 - UART0",
	UART_IntChLoopback_Uart2Uart1InterruptTest,	"Inter-Channel Loopback Interrupt Test : UART2 - UART1",
	UART_IntChLoopback_Uart2Uart3InterruptTest,	"Inter-Channel Loopback Interrupt Test : UART2 - UART3",
	UART_IntChLoopback_Uart3Uart0InterruptTest,	"Inter-Channel Loopback Interrupt Test : UART3 - UART0",
	UART_IntChLoopback_Uart3Uart1InterruptTest,	"Inter-Channel Loopback Interrupt Test : UART3 - UART1",
	UART_IntChLoopback_Uart3Uart2InterruptTest,	"Inter-Channel Loopback Interrupt Test : UART3 - UART2\n",

	UART_SetPLL,									"Set PLL(APLL/MPLL/EPLL/VPLL)\n",

	NULL,											"Exit",
};

s32 UART_IntChLoopback_Interrupt_Test(UART_CtrlInfo_st *pCtrlInfo)
{
	s32				MenuEntryCnt;
	s32				MenuIndx;
	s32				SelNum;
	s32				TestResult = 0;
	
	MenuEntryCnt = sizeof(gUART_Test_IntChLoopback_InterruptTest_Menu)/sizeof(UART_TestFuncMenu);
	while(1)
	{
		/* Display Top Menu */
		UART_Printf("\n====================================================");
		UART_Printf("\n   UART Inter Channel Loopback Interrupt Test");
		UART_Printf("\n====================================================\n");
		for(MenuIndx = 0; MenuIndx<MenuEntryCnt; MenuIndx++)
		{   
	   		UART_Printf("%2d.%s", MenuIndx, gUART_Test_IntChLoopback_InterruptTest_Menu[MenuIndx].desc);
			UART_Printf("\n");
		}
		UART_Printf("\n====================================================");

		UART_Printf("\nSelect: ");
		SelNum = UART_GetIntNum();
		//if(SelNum <= 0 || SelNum >= MenuEntryCnt)
		if(SelNum == MenuEntryCnt - 1)
		{
			break;
		}

		if(SelNum >= 0 && SelNum < MenuEntryCnt - 1)
		{
			if(gUART_Test_IntChLoopback_InterruptTest_Menu[SelNum].func != NULL)
			{
				TestResult = (gUART_Test_IntChLoopback_InterruptTest_Menu[SelNum].func)(pCtrlInfo);
			}
		}
	}
}


s32 UART_IntChLoopback_UartAUartBInterruptTest(UART_CON *pUartTx, UART_CON *pUartRx)
{
	UART_BufAllocInfo_st	*pTxBufInfo;
	UART_BufAllocInfo_st	*pRxBufInfo;
	bool					TxDone = FALSE;
	bool					RxDone = FALSE;
	int					ErrorCnt ;

	if( pUartTx->m_UartChIdx == UART_GetCurDebugCh() || pUartRx->m_UartChIdx == UART_GetCurDebugCh() )
	{
		UART_Printf("\nError : UART%d is the debug channel.", UART_GetCurDebugCh());
		
		return UART_ERROR;
	}

	pUartTx->m_TxMode = UartMode_Interrupt;
	pUartTx->m_RxMode = UartMode_Interrupt;
	pUartRx->m_TxMode = UartMode_Interrupt;
	pUartRx->m_RxMode = UartMode_Interrupt;
	pTxBufInfo = &pUartTx->m_TxBufNode;
	pRxBufInfo = &pUartRx->m_RxBufNode;

	UART_Open(pUartTx);
	UART_Open(pUartRx);

	pTxBufInfo->m_pBufPtr = UART_GetNCBuf(pTxBufInfo, pUartTx->m_TxTransferSize);
	if(pTxBufInfo->m_pBufPtr == NULL)
	{
		UART_ASSERT();
	}

	UART_NCBufRandInit(pTxBufInfo, pUartTx->m_TxTransferSize);
	UART_NCBufDisplayHex(pTxBufInfo, TRUE);

	pRxBufInfo->m_pBufPtr = UART_GetNCBuf(pRxBufInfo, pUartRx->m_RxTransferSize);
	if(pRxBufInfo->m_pBufPtr == NULL)
	{
		UART_ASSERT();
	}
	UART_NCBufZeroInit(pRxBufInfo);

	UART_InterruptRxString(pUartRx, UART_RxCompleteCallback, (u32)&RxDone);
	UART_InterruptTxString(pUartTx, UART_TxCompleteCallback, (u32)&TxDone);

	while(!UART_GetKey())
	{
		if(TxDone && RxDone)
		{
			break;
		}
	}

	ErrorCnt = UART_CompareData(pTxBufInfo->m_pBufPtr, pRxBufInfo->m_pBufPtr, pTxBufInfo->m_ReqDataSize);
	UART_NCBufDisplayHex(pRxBufInfo, TRUE);

	UART_FreeNCBuf(pTxBufInfo);
	UART_FreeNCBuf(pRxBufInfo);

	UART_Close(pUartTx);
	UART_Close(pUartRx);

	if(ErrorCnt == 0)
	{
		return UART_NO_ERROR;
	}
	else
	{
		return UART_ERROR;
	}
}

s32 UART_IntChLoopback_Uart0Uart1InterruptTest(UART_CtrlInfo_st *pCtrlInfo)
{
	UART_CON		*pUartTx;
	UART_CON		*pUartRx;
	s32				Status;

	pUartTx = &pCtrlInfo->m_UartChTestInfo[UartCh_CH0];
	pUartRx = &pCtrlInfo->m_UartChTestInfo[UartCh_CH1];

	Status = UART_IntChLoopback_UartAUartBInterruptTest(pUartTx, pUartRx);

	return Status;
}

s32 UART_IntChLoopback_Uart0Uart2InterruptTest(UART_CtrlInfo_st *pCtrlInfo)
{
	UART_CON		*pUartTx;
	UART_CON		*pUartRx;
	s32				Status;

	pUartTx = &pCtrlInfo->m_UartChTestInfo[UartCh_CH0];
	pUartRx = &pCtrlInfo->m_UartChTestInfo[UartCh_CH2];

	Status = UART_IntChLoopback_UartAUartBInterruptTest(pUartTx, pUartRx);

	return Status;
}


s32 UART_IntChLoopback_Uart0Uart3InterruptTest(UART_CtrlInfo_st *pCtrlInfo)
{
	UART_CON		*pUartTx;
	UART_CON		*pUartRx;
	s32				Status;

	pUartTx = &pCtrlInfo->m_UartChTestInfo[UartCh_CH0];
	pUartRx = &pCtrlInfo->m_UartChTestInfo[UartCh_CH3];

	Status = UART_IntChLoopback_UartAUartBInterruptTest(pUartTx, pUartRx);

	return Status;
}


s32 UART_IntChLoopback_Uart1Uart0InterruptTest(UART_CtrlInfo_st *pCtrlInfo)
{
	UART_CON		*pUartTx;
	UART_CON		*pUartRx;
	s32				Status;

	pUartTx = &pCtrlInfo->m_UartChTestInfo[UartCh_CH1];
	pUartRx = &pCtrlInfo->m_UartChTestInfo[UartCh_CH0];

	Status = UART_IntChLoopback_UartAUartBInterruptTest(pUartTx, pUartRx);

	return Status;
}


s32 UART_IntChLoopback_Uart1Uart2InterruptTest(UART_CtrlInfo_st *pCtrlInfo)
{
	UART_CON		*pUartTx;
	UART_CON		*pUartRx;
	s32				Status;

	pUartTx = &pCtrlInfo->m_UartChTestInfo[UartCh_CH1];
	pUartRx = &pCtrlInfo->m_UartChTestInfo[UartCh_CH2];

	Status = UART_IntChLoopback_UartAUartBInterruptTest(pUartTx, pUartRx);

	return Status;
}


s32 UART_IntChLoopback_Uart1Uart3InterruptTest(UART_CtrlInfo_st *pCtrlInfo)
{
	UART_CON		*pUartTx;
	UART_CON		*pUartRx;
	s32				Status;

	pUartTx = &pCtrlInfo->m_UartChTestInfo[UartCh_CH1];
	pUartRx = &pCtrlInfo->m_UartChTestInfo[UartCh_CH3];

	Status = UART_IntChLoopback_UartAUartBInterruptTest(pUartTx, pUartRx);

	return Status;
}


s32 UART_IntChLoopback_Uart2Uart0InterruptTest(UART_CtrlInfo_st *pCtrlInfo)
{
	UART_CON		*pUartTx;
	UART_CON		*pUartRx;
	s32				Status;

	pUartTx = &pCtrlInfo->m_UartChTestInfo[UartCh_CH2];
	pUartRx = &pCtrlInfo->m_UartChTestInfo[UartCh_CH0];

	Status = UART_IntChLoopback_UartAUartBInterruptTest(pUartTx, pUartRx);

	return Status;
}


s32 UART_IntChLoopback_Uart2Uart1InterruptTest(UART_CtrlInfo_st *pCtrlInfo)
{
	UART_CON		*pUartTx;
	UART_CON		*pUartRx;
	s32				Status;

	pUartTx = &pCtrlInfo->m_UartChTestInfo[UartCh_CH2];
	pUartRx = &pCtrlInfo->m_UartChTestInfo[UartCh_CH1];

	Status = UART_IntChLoopback_UartAUartBInterruptTest(pUartTx, pUartRx);

	return Status;
}


s32 UART_IntChLoopback_Uart2Uart3InterruptTest(UART_CtrlInfo_st *pCtrlInfo)
{
	UART_CON		*pUartTx;
	UART_CON		*pUartRx;
	s32				Status;

	pUartTx = &pCtrlInfo->m_UartChTestInfo[UartCh_CH2];
	pUartRx = &pCtrlInfo->m_UartChTestInfo[UartCh_CH3];

	Status = UART_IntChLoopback_UartAUartBInterruptTest(pUartTx, pUartRx);

	return Status;
}


s32 UART_IntChLoopback_Uart3Uart0InterruptTest(UART_CtrlInfo_st *pCtrlInfo)
{
	UART_CON		*pUartTx;
	UART_CON		*pUartRx;
	s32				Status;

	pUartTx = &pCtrlInfo->m_UartChTestInfo[UartCh_CH3];
	pUartRx = &pCtrlInfo->m_UartChTestInfo[UartCh_CH0];

	Status = UART_IntChLoopback_UartAUartBInterruptTest(pUartTx, pUartRx);

	return Status;
}


s32 UART_IntChLoopback_Uart3Uart1InterruptTest(UART_CtrlInfo_st *pCtrlInfo)
{
	UART_CON		*pUartTx;
	UART_CON		*pUartRx;
	s32				Status;

	pUartTx = &pCtrlInfo->m_UartChTestInfo[UartCh_CH3];
	pUartRx = &pCtrlInfo->m_UartChTestInfo[UartCh_CH1];

	Status = UART_IntChLoopback_UartAUartBInterruptTest(pUartTx, pUartRx);

	return Status;
}


s32 UART_IntChLoopback_Uart3Uart2InterruptTest(UART_CtrlInfo_st *pCtrlInfo)
{
	UART_CON		*pUartTx;
	UART_CON		*pUartRx;
	s32				Status;

	pUartTx = &pCtrlInfo->m_UartChTestInfo[UartCh_CH3];
	pUartRx = &pCtrlInfo->m_UartChTestInfo[UartCh_CH2];

	Status = UART_IntChLoopback_UartAUartBInterruptTest(pUartTx, pUartRx);

	return Status;
}



const UART_TestFuncMenu gUART_Test_IntChLoopback_DMATest_Menu[] = 
{
	UART_SetDefaultInfo_Uart0,					"Set DefaultVar : UART0",
	UART_SetDefaultInfo_Uart1,					"Set DefaultVar : UART1",
	UART_SetDefaultInfo_Uart2,					"Set DefaultVar : UART2",
	UART_SetDefaultInfo_Uart3,					"Set DefaultVar : UART3\n",

	UART_IntChLoopback_Uart0Uart1DMATest,		"Inter-Channel Loopback DMA Test : UART0 - UART1",
	UART_IntChLoopback_Uart0Uart2DMATest,		"Inter-Channel Loopback DMA Test : UART0 - UART2",
	UART_IntChLoopback_Uart0Uart3DMATest,		"Inter-Channel Loopback DMA Test : UART0 - UART3",
	UART_IntChLoopback_Uart1Uart0DMATest,		"Inter-Channel Loopback DMA Test : UART1 - UART0",
	UART_IntChLoopback_Uart1Uart2DMATest,		"Inter-Channel Loopback DMA Test : UART1 - UART2",
	UART_IntChLoopback_Uart1Uart3DMATest,		"Inter-Channel Loopback DMA Test : UART1 - UART3",
	UART_IntChLoopback_Uart2Uart0DMATest,		"Inter-Channel Loopback DMA Test : UART2 - UART0",
	UART_IntChLoopback_Uart2Uart1DMATest,		"Inter-Channel Loopback DMA Test : UART2 - UART1",
	UART_IntChLoopback_Uart2Uart3DMATest,		"Inter-Channel Loopback DMA Test : UART2 - UART3",
	UART_IntChLoopback_Uart3Uart0DMATest,		"Inter-Channel Loopback DMA Test : UART3 - UART0",
	UART_IntChLoopback_Uart3Uart1DMATest,		"Inter-Channel Loopback DMA Test : UART3 - UART1",
	UART_IntChLoopback_Uart3Uart2DMATest,		"Inter-Channel Loopback DMA Test : UART3 - UART2\n",

	UART_SetPLL,								"Set PLL(APLL/MPLL/EPLL/VPLL)\n",

	NULL,										"Exit",
};

s32 UART_IntChLoopback_DMA_Test(UART_CtrlInfo_st *pCtrlInfo)
{
	s32				MenuEntryCnt;
	s32				MenuIndx;
	s32				SelNum;
	s32				TestResult = 0;
	
	MenuEntryCnt = sizeof(gUART_Test_IntChLoopback_DMATest_Menu)/sizeof(UART_TestFuncMenu);
	while(1)
	{
		/* Display Top Menu */
		UART_Printf("\n====================================================");
		UART_Printf("\n   UART Inter Channel Loopback DMA Test");
		UART_Printf("\n====================================================\n");
		for(MenuIndx = 0; MenuIndx<MenuEntryCnt; MenuIndx++)
		{   
	   		UART_Printf("%2d.%s", MenuIndx, gUART_Test_IntChLoopback_DMATest_Menu[MenuIndx].desc);
			UART_Printf("\n");
		}
		UART_Printf("\n====================================================");

		UART_Printf("\nSelect: ");
		SelNum = UART_GetIntNum();
		//if(SelNum <= 0 || SelNum >= MenuEntryCnt)
		if(SelNum == MenuEntryCnt - 1)
		{
			break;
		}

		if(SelNum >= 0 && SelNum < MenuEntryCnt - 1)
		{
			if(gUART_Test_IntChLoopback_DMATest_Menu[SelNum].func != NULL)
			{
				TestResult = (gUART_Test_IntChLoopback_DMATest_Menu[SelNum].func)(pCtrlInfo);
			}
		}
	}
}



void UART_TxCompleteCallback(u32 var0)
{
	bool *pComplete = (bool *)var0;

	(*pComplete) = TRUE;

	if(UART_IsInterruptPrintValid())
	{
		UART_Printf("TxComplete\n");
	}
}

void UART_RxCompleteCallback(u32 var0)
{
	bool *pComplete = (bool *)var0;

	(*pComplete) = TRUE;

	if(UART_IsInterruptPrintValid())
	{
		UART_Printf("RxComplete\n");
	}
}


s32 UART_IntChLoopback_UartAUartBDMATest(UART_CON *pUartTx, UART_CON *pUartRx)
{
	UART_BufAllocInfo_st	*pTxBufInfo;
	UART_BufAllocInfo_st	*pRxBufInfo;
	bool					TxDone = FALSE;
	bool					RxDone = FALSE;
	int					ErrorCnt ;

	if( pUartTx->m_UartChIdx == UART_GetCurDebugCh() || pUartRx->m_UartChIdx == UART_GetCurDebugCh() )
	{
		UART_Printf("\nError : UART%d is the debug channel.", UART_GetCurDebugCh());
		
		return UART_ERROR;
	}

	pUartTx->m_TxMode = UartMode_DMA;
	pUartRx->m_RxMode = UartMode_DMA;
	pTxBufInfo = &pUartTx->m_TxBufNode;
	pRxBufInfo = &pUartRx->m_RxBufNode;

	UART_Open(pUartTx);
	UART_Open(pUartRx);

	pTxBufInfo->m_pBufPtr = UART_GetNCBuf(pTxBufInfo, pUartTx->m_TxTransferSize);
	if(pTxBufInfo->m_pBufPtr == NULL)
	{
		UART_ASSERT();
	}

	UART_NCBufRandInit(pTxBufInfo, pUartTx->m_TxTransferSize);
	UART_NCBufDisplayHex(pTxBufInfo, TRUE);

	pRxBufInfo->m_pBufPtr = UART_GetNCBuf(pRxBufInfo, pUartRx->m_RxTransferSize);
	if(pRxBufInfo->m_pBufPtr == NULL)
	{
		UART_ASSERT();
	}
	UART_NCBufZeroInit(pRxBufInfo);

	UART_DMARxString(pUartRx, UART_RxCompleteCallback, (u32)&RxDone);
	UART_DMATxString(pUartTx, UART_TxCompleteCallback, (u32)&TxDone);

	while(!UART_GetKey())
	{
		if(TxDone && RxDone)
		{
			break;
		}
	}

	ErrorCnt = UART_CompareData(pTxBufInfo->m_pBufPtr, pRxBufInfo->m_pBufPtr, pTxBufInfo->m_ReqDataSize);
	UART_NCBufDisplayHex(pRxBufInfo, TRUE);

	UART_FreeNCBuf(pTxBufInfo);
	UART_FreeNCBuf(pRxBufInfo);

	DMA_StopDmac(&pUartTx->m_UartTxDma);
	DMA_StopDmac(&pUartRx->m_UartRxDma);

	UART_Close(pUartTx);
	UART_Close(pUartRx);

	if(ErrorCnt == 0)
	{
		return UART_NO_ERROR;
	}
	else
	{
		return UART_ERROR;
	}
}

s32 UART_IntChLoopback_Uart0Uart1DMATest(UART_CtrlInfo_st *pCtrlInfo)
{
	UART_CON		*pUartTx;
	UART_CON		*pUartRx;
	s32				Status;

	pUartTx = &pCtrlInfo->m_UartChTestInfo[UartCh_CH0];
	pUartRx = &pCtrlInfo->m_UartChTestInfo[UartCh_CH1];

	Status = UART_IntChLoopback_UartAUartBDMATest(pUartTx, pUartRx);

	return Status;
}

s32 UART_IntChLoopback_Uart0Uart2DMATest(UART_CtrlInfo_st *pCtrlInfo)
{
	UART_CON		*pUartTx;
	UART_CON		*pUartRx;
	s32				Status;

	pUartTx = &pCtrlInfo->m_UartChTestInfo[UartCh_CH0];
	pUartRx = &pCtrlInfo->m_UartChTestInfo[UartCh_CH2];

	Status = UART_IntChLoopback_UartAUartBDMATest(pUartTx, pUartRx);

	return Status;
}

s32 UART_IntChLoopback_Uart0Uart3DMATest(UART_CtrlInfo_st *pCtrlInfo)
{
	UART_CON		*pUartTx;
	UART_CON		*pUartRx;
	s32				Status;

	pUartTx = &pCtrlInfo->m_UartChTestInfo[UartCh_CH0];
	pUartRx = &pCtrlInfo->m_UartChTestInfo[UartCh_CH3];

	Status = UART_IntChLoopback_UartAUartBDMATest(pUartTx, pUartRx);

	return Status;
}

s32 UART_IntChLoopback_Uart1Uart0DMATest(UART_CtrlInfo_st *pCtrlInfo)
{
	UART_CON		*pUartTx;
	UART_CON		*pUartRx;
	s32				Status;

	pUartTx = &pCtrlInfo->m_UartChTestInfo[UartCh_CH1];
	pUartRx = &pCtrlInfo->m_UartChTestInfo[UartCh_CH0];

	Status = UART_IntChLoopback_UartAUartBDMATest(pUartTx, pUartRx);

	return Status;
}

s32 UART_IntChLoopback_Uart1Uart2DMATest(UART_CtrlInfo_st *pCtrlInfo)
{
	UART_CON		*pUartTx;
	UART_CON		*pUartRx;
	s32				Status;

	pUartTx = &pCtrlInfo->m_UartChTestInfo[UartCh_CH1];
	pUartRx = &pCtrlInfo->m_UartChTestInfo[UartCh_CH2];

	Status = UART_IntChLoopback_UartAUartBDMATest(pUartTx, pUartRx);

	return Status;
}

s32 UART_IntChLoopback_Uart1Uart3DMATest(UART_CtrlInfo_st *pCtrlInfo)
{
	UART_CON		*pUartTx;
	UART_CON		*pUartRx;
	s32				Status;

	pUartTx = &pCtrlInfo->m_UartChTestInfo[UartCh_CH1];
	pUartRx = &pCtrlInfo->m_UartChTestInfo[UartCh_CH3];

	Status = UART_IntChLoopback_UartAUartBDMATest(pUartTx, pUartRx);

	return Status;
}

s32 UART_IntChLoopback_Uart2Uart0DMATest(UART_CtrlInfo_st *pCtrlInfo)
{
	UART_CON		*pUartTx;
	UART_CON		*pUartRx;
	s32				Status;

	pUartTx = &pCtrlInfo->m_UartChTestInfo[UartCh_CH2];
	pUartRx = &pCtrlInfo->m_UartChTestInfo[UartCh_CH0];

	Status = UART_IntChLoopback_UartAUartBDMATest(pUartTx, pUartRx);

	return Status;
}

s32 UART_IntChLoopback_Uart2Uart1DMATest(UART_CtrlInfo_st *pCtrlInfo)
{
	UART_CON		*pUartTx;
	UART_CON		*pUartRx;
	s32				Status;

	pUartTx = &pCtrlInfo->m_UartChTestInfo[UartCh_CH2];
	pUartRx = &pCtrlInfo->m_UartChTestInfo[UartCh_CH1];

	Status = UART_IntChLoopback_UartAUartBDMATest(pUartTx, pUartRx);

	return Status;
}

s32 UART_IntChLoopback_Uart2Uart3DMATest(UART_CtrlInfo_st *pCtrlInfo)
{
	UART_CON		*pUartTx;
	UART_CON		*pUartRx;
	s32				Status;

	pUartTx = &pCtrlInfo->m_UartChTestInfo[UartCh_CH2];
	pUartRx = &pCtrlInfo->m_UartChTestInfo[UartCh_CH3];

	Status = UART_IntChLoopback_UartAUartBDMATest(pUartTx, pUartRx);

	return Status;
}

s32 UART_IntChLoopback_Uart3Uart0DMATest(UART_CtrlInfo_st *pCtrlInfo)
{
	UART_CON		*pUartTx;
	UART_CON		*pUartRx;
	s32				Status;

	pUartTx = &pCtrlInfo->m_UartChTestInfo[UartCh_CH3];
	pUartRx = &pCtrlInfo->m_UartChTestInfo[UartCh_CH0];

	Status = UART_IntChLoopback_UartAUartBDMATest(pUartTx, pUartRx);

	return Status;
}

s32 UART_IntChLoopback_Uart3Uart1DMATest(UART_CtrlInfo_st *pCtrlInfo)
{
	UART_CON		*pUartTx;
	UART_CON		*pUartRx;
	s32				Status;

	pUartTx = &pCtrlInfo->m_UartChTestInfo[UartCh_CH3];
	pUartRx = &pCtrlInfo->m_UartChTestInfo[UartCh_CH1];

	Status = UART_IntChLoopback_UartAUartBDMATest(pUartTx, pUartRx);

	return Status;
}

s32 UART_IntChLoopback_Uart3Uart2DMATest(UART_CtrlInfo_st *pCtrlInfo)
{
	UART_CON		*pUartTx;
	UART_CON		*pUartRx;
	s32				Status;

	pUartTx = &pCtrlInfo->m_UartChTestInfo[UartCh_CH3];
	pUartRx = &pCtrlInfo->m_UartChTestInfo[UartCh_CH2];

	Status = UART_IntChLoopback_UartAUartBDMATest(pUartTx, pUartRx);

	return Status;
}




const UART_TestFuncMenu gUART_Test_PollingRxWithManualRTSTest_Menu[] = 
{
	UART_SetDefaultInfo_Uart0,					"Set DefaultVar : UART0",
	UART_SetDefaultInfo_Uart1,					"Set DefaultVar : UART1",
	UART_SetDefaultInfo_Uart2,					"Set DefaultVar : UART2",
	UART_SetDefaultInfo_Uart3,					"Set DefaultVar : UART3\n",

	UART_Uart0PollingRxWithManualRTSTest,		"Polling Rx with manual RTS Test : UART0",
	UART_Uart1PollingRxWithManualRTSTest,		"Polling Rx with manual RTS Test : UART1",
	UART_Uart2PollingRxWithManualRTSTest,		"Polling Rx with manual RTS Test : UART2",
	UART_Uart3PollingRxWithManualRTSTest,		"Polling Rx with manual RTS Test : UART3\n",

	UART_SetPLL,								"Set PLL(APLL/MPLL/EPLL/VPLL)\n",

	NULL,										"Exit",
};

s32 UART_PollingRxWithManualRTSTest(UART_CtrlInfo_st *pCtrlInfo)
{
	s32				MenuEntryCnt;
	s32				MenuIndx;
	s32				SelNum;
	s32				TestResult = 0;
	
	MenuEntryCnt = sizeof(gUART_Test_PollingRxWithManualRTSTest_Menu)/sizeof(UART_TestFuncMenu);
	while(1)
	{
		/* Display Top Menu */
		UART_Printf("\n====================================================");
		UART_Printf("\n   UART Polling Rx with manual RTS Test");
		UART_Printf("\n====================================================\n");
		for(MenuIndx = 0; MenuIndx<MenuEntryCnt; MenuIndx++)
		{   
	   		UART_Printf("%2d.%s", MenuIndx, gUART_Test_PollingRxWithManualRTSTest_Menu[MenuIndx].desc);
			UART_Printf("\n");
		}
		UART_Printf("\n====================================================");

		UART_Printf("\nSelect: ");
		SelNum = UART_GetIntNum();
		//if(SelNum <= 0 || SelNum >= MenuEntryCnt)
		if(SelNum == MenuEntryCnt - 1)
		{
			break;
		}

		if(SelNum >= 0 && SelNum < MenuEntryCnt - 1)
		{
			if(gUART_Test_PollingRxWithManualRTSTest_Menu[SelNum].func != NULL)
			{
				TestResult = (gUART_Test_PollingRxWithManualRTSTest_Menu[SelNum].func)(pCtrlInfo);
			}
		}
	}
}


s32 UART_UartAPollingRxWithManualRTSTest(UART_CON *pUart)
{
	//UART_BufAllocInfo_st	*pTxBufInfo;
	UART_BufAllocInfo_st	*pRxBufInfo;
	//bool					TxDone = FALSE;
	//bool					RxDone = FALSE;

	if(pUart->m_UartChIdx == UART_GetCurDebugCh())
	{
		UART_Printf("\nError : UART%d is the debug channel.", pUart->m_UartChIdx);
		
		return UART_ERROR;
	}

	pUart->m_TxMode = UartMode_Interrupt;
	pUart->m_RxMode = UartMode_Interrupt;
	pRxBufInfo = &pUart->m_RxBufNode;

	UART_Open(pUart);

	pRxBufInfo->m_pBufPtr = UART_GetNCBuf(pRxBufInfo, pUart->m_RxTransferSize);
	if(pRxBufInfo->m_pBufPtr == NULL)
	{
		UART_ASSERT();
	}
	UART_NCBufZeroInit(pRxBufInfo);

	UART_WrRTS(pUart, RTS_inAct);
	UART_Printf("Start up Tx(CTS) on the other B'd and then press any key, it will start Rx(RTS)\n");
	UART_Getc();

	UART_PollingRxWithManualRTSString(pUart);

	UART_WrRTS(pUart, RTS_Act);

	UART_NCBufDisplayHex(pRxBufInfo, TRUE);

	UART_FreeNCBuf(pRxBufInfo);

	UART_Close(pUart);
}

s32 UART_Uart0PollingRxWithManualRTSTest(UART_CtrlInfo_st *pCtrlInfo)
{
	UART_CON		*pUart;
	s32				Status;

	pUart = &pCtrlInfo->m_UartChTestInfo[UartCh_CH0];

	Status = UART_UartAPollingRxWithManualRTSTest(pUart);

	return Status;
}


s32 UART_Uart1PollingRxWithManualRTSTest(UART_CtrlInfo_st *pCtrlInfo)
{
	UART_CON		*pUart;
	s32				Status;

	pUart = &pCtrlInfo->m_UartChTestInfo[UartCh_CH1];

	Status = UART_UartAPollingRxWithManualRTSTest(pUart);

	return Status;
}

s32 UART_Uart2PollingRxWithManualRTSTest(UART_CtrlInfo_st *pCtrlInfo)
{
	UART_CON		*pUart;
	s32				Status;

	pUart = &pCtrlInfo->m_UartChTestInfo[UartCh_CH2];

	Status = UART_UartAPollingRxWithManualRTSTest(pUart);

	return Status;
}

s32 UART_Uart3PollingRxWithManualRTSTest(UART_CtrlInfo_st *pCtrlInfo)
{
	UART_CON		*pUart;
	s32				Status;

	pUart = &pCtrlInfo->m_UartChTestInfo[UartCh_CH3];

	Status = UART_UartAPollingRxWithManualRTSTest(pUart);

	return Status;
}




const UART_TestFuncMenu gUART_Test_ModemInterrupt[] = 
{
	UART_SetDefaultInfo_Uart0,					"Set DefaultVar : UART0",
	UART_SetDefaultInfo_Uart1,					"Set DefaultVar : UART1",
	UART_SetDefaultInfo_Uart2,					"Set DefaultVar : UART2",
	UART_SetDefaultInfo_Uart3,					"Set DefaultVar : UART3\n",

	UART_Uart0ModemInterruptTest,				"Modem Interrupt Test : UART0",
	UART_Uart1ModemInterruptTest,				"Modem Interrupt Test : UART1",
	UART_Uart2ModemInterruptTest,				"Modem Interrupt Test : UART2",
	UART_Uart3ModemInterruptTest,				"Modem Interrupt Test : UART3\n",

	UART_SetPLL,								"Set PLL(APLL/MPLL/EPLL/VPLL)\n",

	NULL,										"Exit",
};

s32 UART_ModemInterruptTest(UART_CtrlInfo_st *pCtrlInfo)
{
	s32				MenuEntryCnt;
	s32				MenuIndx;
	s32				SelNum;
	s32				TestResult = 0;
	
	MenuEntryCnt = sizeof(gUART_Test_ModemInterrupt)/sizeof(UART_TestFuncMenu);
	while(1)
	{
		/* Display Top Menu */
		UART_Printf("\n====================================================");
		UART_Printf("\n   UART Modem Interrupt Test");
		UART_Printf("\n====================================================\n");
		for(MenuIndx = 0; MenuIndx<MenuEntryCnt; MenuIndx++)
		{   
	   		UART_Printf("%2d.%s", MenuIndx, gUART_Test_ModemInterrupt[MenuIndx].desc);
			UART_Printf("\n");
		}
		UART_Printf("\n====================================================");

		UART_Printf("\nSelect: ");
		SelNum = UART_GetIntNum();
		//if(SelNum <= 0 || SelNum >= MenuEntryCnt)
		if(SelNum == MenuEntryCnt - 1)
		{
			break;
		}

		if(SelNum >= 0 && SelNum < MenuEntryCnt - 1)
		{
			if(gUART_Test_ModemInterrupt[SelNum].func != NULL)
			{
				TestResult = (gUART_Test_ModemInterrupt[SelNum].func)(pCtrlInfo);
			}
		}
	}
}


s32 UART_UartAModemInterruptTest(UART_CON *pUart)
{
	UART_BufAllocInfo_st	*pRxBufInfo;

	if(pUart->m_UartChIdx == UART_GetCurDebugCh())
	{
		UART_Printf("\nError : UART%d is the debug channel.", pUart->m_UartChIdx);
		
		return UART_ERROR;
	}

	pUart->m_TxMode = UartMode_Interrupt;
	pUart->m_RxMode = UartMode_Interrupt;

	pUart->m_EnModemIntMask = FALSE;		/* Enable Modem Interrupt. */

	UART_Open(pUart);

	INTC_Enable(pUart->m_IntNum);

	UART_Printf("\n UART Modem Interrupt is enabled.");
	UART_Printf("\n Make the CTS signal toggled.");

	UART_Printf("\n");
	UART_Printf("\n");
}


s32 UART_Uart0ModemInterruptTest(UART_CtrlInfo_st *pCtrlInfo)
{
	UART_CON		*pUart;
	s32				Status;

	pUart = &pCtrlInfo->m_UartChTestInfo[UartCh_CH0];

	Status = UART_UartAModemInterruptTest(pUart);

	return Status;
}

s32 UART_Uart1ModemInterruptTest(UART_CtrlInfo_st *pCtrlInfo)
{
	UART_CON		*pUart;
	s32				Status;

	pUart = &pCtrlInfo->m_UartChTestInfo[UartCh_CH1];

	Status = UART_UartAModemInterruptTest(pUart);

	return Status;
}

s32 UART_Uart2ModemInterruptTest(UART_CtrlInfo_st *pCtrlInfo)
{
	UART_CON		*pUart;
	s32				Status;

	pUart = &pCtrlInfo->m_UartChTestInfo[UartCh_CH2];

	Status = UART_UartAModemInterruptTest(pUart);

	return Status;
}

s32 UART_Uart3ModemInterruptTest(UART_CtrlInfo_st *pCtrlInfo)
{
	UART_CON		*pUart;
	s32				Status;

	pUart = &pCtrlInfo->m_UartChTestInfo[UartCh_CH3];

	Status = UART_UartAModemInterruptTest(pUart);

	return Status;
}


const UART_TestFuncMenu gUART_Test_SendBreakSignalTest_Menu[] = 
{
	UART_SetDefaultInfo_Uart0,					"Set DefaultVar : UART0",
	UART_SetDefaultInfo_Uart1,					"Set DefaultVar : UART1",
	UART_SetDefaultInfo_Uart2,					"Set DefaultVar : UART2",
	UART_SetDefaultInfo_Uart3,					"Set DefaultVar : UART3\n",

	UART_Uart0SendBreakSignalTest,				"Send Break Signal Test : UART0",
	UART_Uart1SendBreakSignalTest,				"Send Break Signal Test : UART1",
	UART_Uart2SendBreakSignalTest,				"Send Break Signal Test : UART2",
	UART_Uart3SendBreakSignalTest,				"Send Break Signal Test : UART3\n",

	UART_SetPLL,								"Set PLL(APLL/MPLL/EPLL/VPLL)\n",

	NULL,										"Exit",
};

s32 UART_SendBreakSignalTest(UART_CtrlInfo_st *pCtrlInfo)
{
	s32				MenuEntryCnt;
	s32				MenuIndx;
	s32				SelNum;
	s32				TestResult = 0;
	
	MenuEntryCnt = sizeof(gUART_Test_SendBreakSignalTest_Menu)/sizeof(UART_TestFuncMenu);
	while(1)
	{
		/* Display Top Menu */
		UART_Printf("\n====================================================");
		UART_Printf("\n   UART Send Break Signal Test");
		UART_Printf("\n====================================================\n");
		for(MenuIndx = 0; MenuIndx<MenuEntryCnt; MenuIndx++)
		{   
	   		UART_Printf("%2d.%s", MenuIndx, gUART_Test_SendBreakSignalTest_Menu[MenuIndx].desc);
			UART_Printf("\n");
		}
		UART_Printf("\n====================================================");

		UART_Printf("\nSelect: ");
		SelNum = UART_GetIntNum();
		//if(SelNum <= 0 || SelNum >= MenuEntryCnt)
		if(SelNum == MenuEntryCnt - 1)
		{
			break;
		}

		if(SelNum >= 0 && SelNum < MenuEntryCnt - 1)
		{
			if(gUART_Test_SendBreakSignalTest_Menu[SelNum].func != NULL)
			{
				TestResult = (gUART_Test_SendBreakSignalTest_Menu[SelNum].func)(pCtrlInfo);
			}
		}
	}
}


s32 UART_UartASendBreakSignalTest(UART_CON *pUart)
{
	UART_BufAllocInfo_st	*pTxBufInfo;
	u8 cKey;

	if(pUart->m_UartChIdx == UART_GetCurDebugCh())
	{
		UART_Printf("\nError : UART%d is the debug channel.", pUart->m_UartChIdx);
		
		return UART_ERROR;
	}

	pUart->m_TxMode = UartMode_Interrupt;
	pUart->m_RxMode = UartMode_Interrupt;

	UART_Open(pUart);

	UART_Printf("Press any key then you can have a SendBreak signal through the channel %d, check it out!\n", 
		pUart->m_UartChIdx);	
	UART_Printf("If You enter 'x' , Test will be finished!\n");

	while(1)
	{
		cKey = UART_Getc();

		if ( cKey == 'x')
		{
			break;
		}

		UART_SendBreakSignal(pUart);
	}

	UART_Close(pUart);
}

s32 UART_Uart0SendBreakSignalTest(UART_CtrlInfo_st *pCtrlInfo)
{
	UART_CON		*pUart;
	s32				Status;

	pUart = &pCtrlInfo->m_UartChTestInfo[UartCh_CH0];

	Status = UART_UartASendBreakSignalTest(pUart);

	return Status;
}

s32 UART_Uart1SendBreakSignalTest(UART_CtrlInfo_st *pCtrlInfo)
{
	UART_CON		*pUart;
	s32				Status;

	pUart = &pCtrlInfo->m_UartChTestInfo[UartCh_CH1];

	Status = UART_UartASendBreakSignalTest(pUart);

	return Status;
}

s32 UART_Uart2SendBreakSignalTest(UART_CtrlInfo_st *pCtrlInfo)
{
	UART_CON		*pUart;
	s32				Status;

	pUart = &pCtrlInfo->m_UartChTestInfo[UartCh_CH2];

	Status = UART_UartASendBreakSignalTest(pUart);

	return Status;
}

s32 UART_Uart3SendBreakSignalTest(UART_CtrlInfo_st *pCtrlInfo)
{
	UART_CON		*pUart;
	s32				Status;

	pUart = &pCtrlInfo->m_UartChTestInfo[UartCh_CH3];

	Status = UART_UartASendBreakSignalTest(pUart);

	return Status;
}



const UART_TestFuncMenu gUART_Test_IrDATxTest_Menu[] = 
{
	UART_SetDefaultInfo_Uart0,					"Set DefaultVar : UART0",
	UART_SetDefaultInfo_Uart1,					"Set DefaultVar : UART1",
	UART_SetDefaultInfo_Uart2,					"Set DefaultVar : UART2",
	UART_SetDefaultInfo_Uart3,					"Set DefaultVar : UART3\n",

	UART_Uart0IrDATxTest,						"IrDA Tx Test : UART0",
	UART_Uart1IrDATxTest,						"IrDA Tx Test : UART1",
	UART_Uart2IrDATxTest,						"IrDA Tx Test : UART2",
	UART_Uart3IrDATxTest,						"IrDA Tx Test : UART3\n",

	UART_SetPLL,								"Set PLL(APLL/MPLL/EPLL/VPLL)\n",

	NULL,										"Exit",
};

s32 UART_IrDATxTest(UART_CtrlInfo_st *pCtrlInfo)
{
	s32				MenuEntryCnt;
	s32				MenuIndx;
	s32				SelNum;
	s32				TestResult = 0;
	
	MenuEntryCnt = sizeof(gUART_Test_IrDATxTest_Menu)/sizeof(UART_TestFuncMenu);
	while(1)
	{
		/* Display Top Menu */
		UART_Printf("\n====================================================");
		UART_Printf("\n   UART IrDA Tx Test");
		UART_Printf("\n====================================================\n");
		for(MenuIndx = 0; MenuIndx<MenuEntryCnt; MenuIndx++)
		{   
	   		UART_Printf("%2d.%s", MenuIndx, gUART_Test_IrDATxTest_Menu[MenuIndx].desc);
			UART_Printf("\n");
		}
		UART_Printf("\n====================================================");

		UART_Printf("\nSelect: ");
		SelNum = UART_GetIntNum();
		//if(SelNum <= 0 || SelNum >= MenuEntryCnt)
		if(SelNum == MenuEntryCnt - 1)
		{
			break;
		}

		if(SelNum >= 0 && SelNum < MenuEntryCnt - 1)
		{
			if(gUART_Test_IrDATxTest_Menu[SelNum].func != NULL)
			{
				TestResult = (gUART_Test_IrDATxTest_Menu[SelNum].func)(pCtrlInfo);
			}
		}
	}
}


s32 UART_UartAIrDATxTest(UART_CON *pUart)
{
	UART_BufAllocInfo_st	*pTxBufInfo;
	//UART_BufAllocInfo_st	*pRxBufInfo;
	//bool					TxDone = FALSE;
	//bool					RxDone = FALSE;

	if(pUart->m_UartChIdx == UART_GetCurDebugCh())
	{
		UART_Printf("\nError : UART%d is the debug channel.", pUart->m_UartChIdx);
		
		return UART_ERROR;
	}

	pUart->m_TxMode = UartMode_Interrupt;
	pUart->m_RxMode = UartMode_Interrupt;
	pUart->m_EnIRDAMode = TRUE;
	pTxBufInfo = &pUart->m_TxBufNode;

	UART_Open(pUart);

	pTxBufInfo->m_pBufPtr = UART_GetNCBuf(pTxBufInfo, pUart->m_TxTransferSize);
	if(pTxBufInfo->m_pBufPtr == NULL)
	{
		UART_ASSERT();
	}

	//UART_NCBufRandInit(pTxBufInfo, pUart->m_TxTransferSize);
	UART_NCBufPatternInit(pTxBufInfo, pUart->m_TxTransferSize, gUART_pInitString_PatternA, 10);
	UART_NCBufDisplayHex(pTxBufInfo, TRUE);

	UART_PollingTxString(pUart);

	UART_FreeNCBuf(pTxBufInfo);

	UART_Close(pUart);
}

s32 UART_Uart0IrDATxTest(UART_CtrlInfo_st *pCtrlInfo)
{
	UART_CON		*pUart;
	s32				Status;

	pUart = &pCtrlInfo->m_UartChTestInfo[UartCh_CH0];

	Status = UART_UartAIrDATxTest(pUart);

	return Status;
}

s32 UART_Uart1IrDATxTest(UART_CtrlInfo_st *pCtrlInfo)
{
	UART_CON		*pUart;
	s32				Status;

	pUart = &pCtrlInfo->m_UartChTestInfo[UartCh_CH1];

	Status = UART_UartAIrDATxTest(pUart);

	return Status;
}

s32 UART_Uart2IrDATxTest(UART_CtrlInfo_st *pCtrlInfo)
{
	UART_CON		*pUart;
	s32				Status;

	pUart = &pCtrlInfo->m_UartChTestInfo[UartCh_CH2];

	Status = UART_UartAIrDATxTest(pUart);

	return Status;
}

s32 UART_Uart3IrDATxTest(UART_CtrlInfo_st *pCtrlInfo)
{
	UART_CON		*pUart;
	s32				Status;

	pUart = &pCtrlInfo->m_UartChTestInfo[UartCh_CH3];

	Status = UART_UartAIrDATxTest(pUart);

	return Status;
}




const UART_TestFuncMenu gUART_Test_IrDARxTest_Menu[] = 
{
	UART_SetDefaultInfo_Uart0,					"Set DefaultVar : UART0",
	UART_SetDefaultInfo_Uart1,					"Set DefaultVar : UART1",
	UART_SetDefaultInfo_Uart2,					"Set DefaultVar : UART2",
	UART_SetDefaultInfo_Uart3,					"Set DefaultVar : UART3\n",

	UART_Uart0IrDARxTest,					"IrDA Rx Test : UART0",
	UART_Uart1IrDARxTest,					"IrDA Rx Test : UART1",
	UART_Uart2IrDARxTest,					"IrDA Rx Test : UART2",
	UART_Uart3IrDARxTest,					"IrDA Rx Test : UART3\n",

	UART_SetPLL,								"Set PLL(APLL/MPLL/EPLL/VPLL)\n",

	NULL,										"Exit",
};

s32 UART_IrDARxTest(UART_CtrlInfo_st *pCtrlInfo)
{
	s32				MenuEntryCnt;
	s32				MenuIndx;
	s32				SelNum;
	s32				TestResult = 0;
	
	MenuEntryCnt = sizeof(gUART_Test_IrDARxTest_Menu)/sizeof(UART_TestFuncMenu);
	while(1)
	{
		/* Display Top Menu */
		UART_Printf("\n====================================================");
		UART_Printf("\n   UART IrDA Rx Test");
		UART_Printf("\n====================================================\n");
		for(MenuIndx = 0; MenuIndx<MenuEntryCnt; MenuIndx++)
		{   
	   		UART_Printf("%2d.%s", MenuIndx, gUART_Test_IrDARxTest_Menu[MenuIndx].desc);
			UART_Printf("\n");
		}
		UART_Printf("\n====================================================");

		UART_Printf("\nSelect: ");
		SelNum = UART_GetIntNum();
		//if(SelNum <= 0 || SelNum >= MenuEntryCnt)
		if(SelNum == MenuEntryCnt - 1)
		{
			break;
		}

		if(SelNum >= 0 && SelNum < MenuEntryCnt - 1)
		{
			if(gUART_Test_IrDARxTest_Menu[SelNum].func != NULL)
			{
				TestResult = (gUART_Test_IrDARxTest_Menu[SelNum].func)(pCtrlInfo);
			}
		}
	}
}


s32 UART_UartAIrDARxTest(UART_CON *pUart)
{
	//UART_BufAllocInfo_st	*pTxBufInfo;
	UART_BufAllocInfo_st	*pRxBufInfo;
	//bool					TxDone = FALSE;
	//bool					RxDone = FALSE;

	if(pUart->m_UartChIdx == UART_GetCurDebugCh())
	{
		UART_Printf("\nError : UART%d is the debug channel.", pUart->m_UartChIdx);
		
		return UART_ERROR;
	}

	pUart->m_TxMode = UartMode_Interrupt;
	pUart->m_RxMode = UartMode_Interrupt;
	pUart->m_EnIRDAMode = TRUE;
	pRxBufInfo = &pUart->m_RxBufNode;

	UART_Open(pUart);

	pRxBufInfo->m_pBufPtr = UART_GetNCBuf(pRxBufInfo, pUart->m_RxTransferSize);
	if(pRxBufInfo->m_pBufPtr == NULL)
	{
		UART_ASSERT();
	}
	UART_NCBufZeroInit(pRxBufInfo);

	UART_PollingRxString(pUart);

	UART_NCBufDisplayHex(pRxBufInfo, TRUE);

	UART_FreeNCBuf(pRxBufInfo);

	UART_Close(pUart);
}

s32 UART_Uart0IrDARxTest(UART_CtrlInfo_st *pCtrlInfo)
{
	UART_CON		*pUart;
	s32				Status;

	pUart = &pCtrlInfo->m_UartChTestInfo[UartCh_CH0];

	Status = UART_UartAIrDARxTest(pUart);

	return Status;
}

s32 UART_Uart1IrDARxTest(UART_CtrlInfo_st *pCtrlInfo)
{
	UART_CON		*pUart;
	s32				Status;

	pUart = &pCtrlInfo->m_UartChTestInfo[UartCh_CH1];

	Status = UART_UartAIrDARxTest(pUart);

	return Status;
}

s32 UART_Uart2IrDARxTest(UART_CtrlInfo_st *pCtrlInfo)
{
	UART_CON		*pUart;
	s32				Status;

	pUart = &pCtrlInfo->m_UartChTestInfo[UartCh_CH2];

	Status = UART_UartAIrDARxTest(pUart);

	return Status;
}

s32 UART_Uart3IrDARxTest(UART_CtrlInfo_st *pCtrlInfo)
{
	UART_CON		*pUart;
	s32				Status;

	pUart = &pCtrlInfo->m_UartChTestInfo[UartCh_CH3];

	Status = UART_UartAIrDARxTest(pUart);

	return Status;
}




const UART_TestFuncMenu gUART_Test_PollingTxWithManualCTSTest_Menu[] = 
{
	UART_SetDefaultInfo_Uart0,					"Set DefaultVar : UART0",
	UART_SetDefaultInfo_Uart1,					"Set DefaultVar : UART1",
	UART_SetDefaultInfo_Uart2,					"Set DefaultVar : UART2",
	UART_SetDefaultInfo_Uart3,					"Set DefaultVar : UART3\n",

	UART_Uart0PollingTxWithManualCTSTest,		"Polling Tx with manual CTS Test : UART0",
	UART_Uart1PollingTxWithManualCTSTest,		"Polling Tx with manual CTS Test : UART1",
	UART_Uart2PollingTxWithManualCTSTest,		"Polling Tx with manual CTS Test : UART2",
	UART_Uart3PollingTxWithManualCTSTest,		"Polling Tx with manual CTS Test : UART3\n",

	UART_SetPLL,								"Set PLL(APLL/MPLL/EPLL/VPLL)\n",

	NULL,										"Exit",
};

s32 UART_PollingTxWithManualCTSTest(UART_CtrlInfo_st *pCtrlInfo)
{
	s32				MenuEntryCnt;
	s32				MenuIndx;
	s32				SelNum;
	s32				TestResult = 0;
	
	MenuEntryCnt = sizeof(gUART_Test_PollingTxWithManualCTSTest_Menu)/sizeof(UART_TestFuncMenu);
	while(1)
	{
		/* Display Top Menu */
		UART_Printf("\n====================================================");
		UART_Printf("\n   UART Polling Tx with manual CTS Test");
		UART_Printf("\n====================================================\n");
		for(MenuIndx = 0; MenuIndx<MenuEntryCnt; MenuIndx++)
		{   
	   		UART_Printf("%2d.%s", MenuIndx, gUART_Test_PollingTxWithManualCTSTest_Menu[MenuIndx].desc);
			UART_Printf("\n");
		}
		UART_Printf("\n====================================================");

		UART_Printf("\nSelect: ");
		SelNum = UART_GetIntNum();
		//if(SelNum <= 0 || SelNum >= MenuEntryCnt)
		if(SelNum == MenuEntryCnt - 1)
		{
			break;
		}

		if(SelNum >= 0 && SelNum < MenuEntryCnt - 1)
		{
			if(gUART_Test_PollingTxWithManualCTSTest_Menu[SelNum].func != NULL)
			{
				TestResult = (gUART_Test_PollingTxWithManualCTSTest_Menu[SelNum].func)(pCtrlInfo);
			}
		}
	}
}


s32 UART_UartAPollingTxWithManualCTSTest(UART_CON *pUart)
{
	UART_BufAllocInfo_st	*pTxBufInfo;
	//UART_BufAllocInfo_st	*pRxBufInfo;
	//bool					TxDone = FALSE;
	//bool					RxDone = FALSE;

	if(pUart->m_UartChIdx == UART_GetCurDebugCh())
	{
		UART_Printf("\nError : UART%d is the debug channel.", pUart->m_UartChIdx);
		
		return UART_ERROR;
	}

	pUart->m_TxMode = UartMode_Interrupt;
	pUart->m_RxMode = UartMode_Interrupt;
	pTxBufInfo = &pUart->m_TxBufNode;

	UART_Open(pUart);

	pTxBufInfo->m_pBufPtr = UART_GetNCBuf(pTxBufInfo, pUart->m_TxTransferSize);
	if(pTxBufInfo->m_pBufPtr == NULL)
	{
		UART_ASSERT();
	}

	//UART_NCBufRandInit(pTxBufInfo, pUart->m_TxTransferSize);
	UART_NCBufPatternInit(pTxBufInfo, pUart->m_TxTransferSize, gUART_pInitString_PatternA, 10);
	UART_NCBufDisplayHex(pTxBufInfo, TRUE);

	UART_PollingTxWithManualCTSString(pUart);

	UART_FreeNCBuf(pTxBufInfo);

	UART_Close(pUart);
}

s32 UART_Uart0PollingTxWithManualCTSTest(UART_CtrlInfo_st *pCtrlInfo)
{
	UART_CON		*pUart;
	s32				Status;

	pUart = &pCtrlInfo->m_UartChTestInfo[UartCh_CH0];

	Status = UART_UartAPollingTxWithManualCTSTest(pUart);

	return Status;
}

s32 UART_Uart1PollingTxWithManualCTSTest(UART_CtrlInfo_st *pCtrlInfo)
{
	UART_CON		*pUart;
	s32				Status;

	pUart = &pCtrlInfo->m_UartChTestInfo[UartCh_CH1];

	Status = UART_UartAPollingTxWithManualCTSTest(pUart);

	return Status;
}

s32 UART_Uart2PollingTxWithManualCTSTest(UART_CtrlInfo_st *pCtrlInfo)
{
	UART_CON		*pUart;
	s32				Status;

	pUart = &pCtrlInfo->m_UartChTestInfo[UartCh_CH2];

	Status = UART_UartAPollingTxWithManualCTSTest(pUart);

	return Status;
}

s32 UART_Uart3PollingTxWithManualCTSTest(UART_CtrlInfo_st *pCtrlInfo)
{
	UART_CON		*pUart;
	s32				Status;

	pUart = &pCtrlInfo->m_UartChTestInfo[UartCh_CH3];

	Status = UART_UartAPollingTxWithManualCTSTest(pUart);

	return Status;
}




#include "SfrReadWriten.h"

REGINFOn		sRegInfoUART0[] = 
{ 
    //01234567890123                 
	{"rULCON0 ",			UART0_BASE+0x0, 	7-1, 	RW, 0x0 , DPPB, 0,0},		
	{"rUCON0 ",			UART0_BASE+0x4,		23-1, 	RW, 0x0  , DPDB, 0,0},	
	{"rUFCON0 ",			UART0_BASE+0x8, 	11-1,	RW, 0x0  , DPPB, 0,0},		
	{"rUMCON0 ",			UART0_BASE+0xc,		8-1, 	RW, 0x0  , DPDB, 0,0},	
	{"rUTRSTAT0 ",		UART0_BASE+0x10,	3-1, 	RO, 0x6  , DPDB, 0,0},	
	{"rUERSTAT0 ",		UART0_BASE+0x14,	3-1,		RO, 0x0  , DPDB, 0,0},	
	{"rUFSTAT0 ",			UART0_BASE+0x18,	25-1, 	RO, 0x0  , DPDB, 0,0},	
	{"rUMSTAT0 ",		UART0_BASE+0x1c,	5-1, 	RO, 0x0  , DPDB, 0,0},		
	{"rUTXH0 ",			UART0_BASE+0x20,	8-1, 	WO, 0x0  , DPDB, 0,0},		
	{"rURXH0 ",			UART0_BASE+0x24,	8-1, 	RO, 0x0  , DPDB, 0,0},			
	{"rUBRDIV0 ",			UART0_BASE+0x28,	16-1, 	RW, 0x0  , DPDB, 0,0},			
	{"rUDIVSLOT0 ",		UART0_BASE+0x2c,	16-1, 	RW, 0x0  , DPDB, 0,0},			
	{"rUINTP0 ",			UART0_BASE+0x30,	4-1, 	WO, 0x0  , DPDB, 0,0},		
	{"rUINTSP0 ",			UART0_BASE+0x34,	4-1, 	WO, 0x0  , DPDB, 0,0},			
	{"rUINTM0 ",			UART0_BASE+0x38,	4-1, 	RW, 0x0  , DPDB, 0,0}			
};

REGINFOn		sRegInfoUART1[] = 
{ 
    //01234567890123                 
	{"rULCON1 ",			UART1_BASE+0x0, 	7-1, 	RW, 0x0 , DPPB, 0,0},		
	{"rUCON1 ",			UART1_BASE+0x4,		23-1, 	RW, 0x0  , DPDB, 0,0},	
	{"rUFCON1 ",			UART1_BASE+0x8, 	11-1,	RW, 0x0  , DPPB, 0,0},		
	{"rUMCON1 ",			UART1_BASE+0xc,		8-1, 	RW, 0x0  , DPDB, 0,0},	
	{"rUTRSTAT1 ",		UART1_BASE+0x10,	3-1, 	RO, 0x6  , DPDB, 0,0},	
	{"rUERSTAT1 ",		UART1_BASE+0x14,	3-1,		RO, 0x0  , DPDB, 0,0},	
	{"rUFSTAT1 ",			UART1_BASE+0x18,	25-1, 	RO, 0x0  , DPDB, 0,0},	
	{"rUMSTAT1 ",		UART1_BASE+0x1c,	5-1, 	RO, 0x0  , DPDB, 0,0},		
	{"rUTXH1 ",			UART1_BASE+0x20,	8-1, 	WO, 0x0  , DPDB, 0,0},		
	{"rURXH1 ",			UART1_BASE+0x24,	8-1, 	RO, 0x0  , DPDB, 0,0},			
	{"rUBRDIV1 ",			UART1_BASE+0x28,	16-1, 	RW, 0x0  , DPDB, 0,0},			
	{"rUDIVSLOT1 ",		UART1_BASE+0x2c,	16-1, 	RW, 0x0  , DPDB, 0,0},			
	{"rUINTP1 ",			UART1_BASE+0x30,	4-1, 	WO, 0x0  , DPDB, 0,0},			
	{"rUINTSP1 ",			UART1_BASE+0x34,	4-1, 	WO, 0x0  , DPDB, 0,0},			
	{"rUINTM1 ",			UART1_BASE+0x38,	4-1, 	RW, 0x0  , DPDB, 0,0}			
};

REGINFOn		sRegInfoUART2[] = 
{ 
    //01234567890123                 
	{"rULCON2 ",			UART2_BASE+0x0, 	7-1, 	RW, 0x0 , DPPB, 0,0},		
	{"rUCON2 ",			UART2_BASE+0x4,		23-1, 	RW, 0x0  , DPDB, 0,0},	
	{"rUFCON2 ",			UART2_BASE+0x8, 	11-1,	RW, 0x0  , DPPB, 0,0},		
	{"rUMCON2 ",			UART2_BASE+0xc,		8-1, 	RW, 0x0  , DPDB, 0,0},	
	{"rUTRSTAT2 ",		UART2_BASE+0x10,	3-1, 	RO, 0x6  , DPDB, 0,0},	
	{"rUERSTAT2 ",		UART2_BASE+0x14,	3-1,		RO, 0x0  , DPDB, 0,0},	
	{"rUFSTAT2 ",			UART2_BASE+0x18,	25-1, 	RO, 0x0  , DPDB, 0,0},	
	{"rUMSTAT2 ",		UART2_BASE+0x1c,	5-1, 	RO, 0x0  , DPDB, 0,0},		
	{"rUTXH2 ",			UART2_BASE+0x20,	8-1, 	WO, 0x0  , DPDB, 0,0},		
	{"rURXH2 ",			UART2_BASE+0x24,	8-1, 	RO, 0x0  , DPDB, 0,0},			
	{"rUBRDIV2 ",			UART2_BASE+0x28,	16-1, 	RW, 0x0  , DPDB, 0,0},			
	{"rUDIVSLOT2 ",		UART2_BASE+0x2c,	16-1, 	RW, 0x0  , DPDB, 0,0},			
	{"rUINTP2 ",			UART2_BASE+0x30,	4-1, 	WO, 0x0  , DPDB, 0,0},			
	{"rUINTSP2 ",			UART2_BASE+0x34,	4-1, 	WO, 0x0  , DPDB, 0,0},			
	{"rUINTM2 ",			UART2_BASE+0x38,	4-1, 	RW, 0x0  , DPDB, 0,0}			
};


REGINFOn		sRegInfoUART3[] = 
{ 
    //01234567890123                 
	{"rULCON3 ",			UART3_BASE+0x0, 	7-1, 	RW, 0x0 , DPPB, 0,0},		
	{"rUCON3 ",			UART3_BASE+0x4,		23-1, 	RW, 0x0  , DPDB, 0,0},	
	{"rUFCON3 ",			UART3_BASE+0x8, 	11-1,	RW, 0x0  , DPPB, 0,0},		
	{"rUMCON3 ",			UART3_BASE+0xc,		8-1, 	RW, 0x0  , DPDB, 0,0},	
	{"rUTRSTAT3 ",		UART3_BASE+0x10,	3-1, 	RO, 0x6  , DPDB, 0,0},	
	{"rUERSTAT3 ",		UART3_BASE+0x14,	3-1,		RO, 0x0  , DPDB, 0,0},	
	{"rUFSTAT3 ",			UART3_BASE+0x18,	25-1, 	RO, 0x0  , DPDB, 0,0},	
	{"rUMSTAT3 ",		UART3_BASE+0x1c,	5-1, 	RO, 0x0  , DPDB, 0,0},		
	{"rUTXH3 ",			UART3_BASE+0x20,	8-1, 	WO, 0x0  , DPDB, 0,0},		
	{"rURXH3 ",			UART3_BASE+0x24,	8-1, 	RO, 0x0  , DPDB, 0,0},			
	{"rUBRDIV3 ",			UART3_BASE+0x28,	16-1, 	RW, 0x0  , DPDB, 0,0},			
	{"rUDIVSLOT3 ",		UART3_BASE+0x2c,	16-1, 	RW, 0x0  , DPDB, 0,0},			
	{"rUINTP3 ",			UART3_BASE+0x30,	4-1, 	WO, 0x0  , DPDB, 0,0},			
	{"rUINTSP3 ",			UART3_BASE+0x34,	4-1, 	WO, 0x0  , DPDB, 0,0},			
	{"rUINTM3 ",			UART3_BASE+0x38,	4-1, 	RW, 0x0  , DPDB, 0,0}			
};




void UART_SFR_test(UART_CtrlInfo_st *pCtrlInfo)
{
	UartCh		CurDebugCh;
	UartCh		Ch;
	s32			sCh;

	CurDebugCh = UART_GetCurDebugCh();

	while(1){
		UART_Printf("Which channel do you want to check?\n");
		UART_Printf("[0 ~ 3] :");
		sCh = UART_GetIntNum();
		if (sCh == -1)
		{
			return;
		}

		if(sCh < 0 || sCh > 3)
		{
			return;
		}

		switch(sCh)
		{
			case UartCh_CH0:
				TestSFRn(sRegInfoUART0, sizeof(sRegInfoUART0)/sizeof(REGINFOn));
				break;

			case UartCh_CH1:
				TestSFRn(sRegInfoUART1, sizeof(sRegInfoUART1)/sizeof(REGINFOn));
				break;

			case UartCh_CH2:
				TestSFRn(sRegInfoUART2, sizeof(sRegInfoUART2)/sizeof(REGINFOn));
				break;

			case UartCh_CH3:
				TestSFRn(sRegInfoUART3, sizeof(sRegInfoUART3)/sizeof(REGINFOn));
				break;
				
			default:
				UART_ASSERT();
				break;
		}

		UART_Printf("\nPress any key to re-test(Quit : 'q')\n");
		if(UART_Getc() == 'q')
		{
			break;
		}
	}	
}


void UART_Console_AutoTest(UART_CtrlInfo_st *pCtrlInfo)
{
	Uart_AutoTest();
}


typedef struct PLL_PMS_Entry_struct {
	float		m_Fout;
	u32		m_PDIV;
	u32		m_MDIV;
	u32		m_SDIV;
	bool		m_IsVSELHigh;
} PLL_PMSEntry_st;



#define	PLL_MAX_VALID_ENTRY			20
#define	PLL_MAX_ACCURATE_ENTRY		5

typedef struct PLL_control_info_struct{
	PLL_PMSEntry_st		m_ValidPMSEntry[PLL_MAX_VALID_ENTRY];
	u32					m_ValidPMSEntryLimit;
	u32					m_PDIVMin;
	u32					m_PDIVMax;
	u32					m_MDIVMin;
	u32					m_MDIVMax;
	u32					m_SDIVMin;
	u32					m_SDIVMax;
	float					m_AccurateRate;		/* 1%, 0.1%, 0.01%, etc */
	float					m_AccurateEntry[PLL_MAX_ACCURATE_ENTRY];
	u32					m_AccurateEntryLimit;
	u32					m_ValidPMSEntryCnt;
	bool					m_EnSameTargetClk;	/* Only check the PMS for the exactly same Target clock */
	bool					m_EnVSELHigh;
	//float					m_FrefMin;
	//float					m_FrefMax;
	//float					m_FVCOLowMin;
	//float					m_FVCOLowMax;
	//float					m_FVCOHighMin;
	//float					m_FVCOHighMax;
	//float					m_FoutMin;
	//float					m_FoutMax;
} PLL_CtrlInfo_st;

typedef s32 (*PLL_GetCalcFreq_t)(PLL_CtrlInfo_st *pPLLCtrlInfo, float *pFreq, u32 PDIV, u32 MDIV, u32 SDIV, bool *pIsVSELHigh);

s32 PLL_CalculateFreq_APLL(PLL_CtrlInfo_st *pPLLCtrlInfo, float *pFreq, u32 PDIV, u32 MDIV, u32 SDIV, bool *pIsVSELHigh)
{
	float		FINClk;
	float		FRef;
	float		FVCO;
	float		Fout;
	u32		SDivExpo;

	FINClk = (float)FIN;

	if(PDIV == 0)
	{
		UART_ASSERT();
	}

	FRef = FINClk/PDIV;
	if(FRef < 1.0e6 || FRef > 1.0e7)	/* FRef should be 3MHz ~ 6MHz in S5PV210. */
	{
		return UART_ERROR;
	}

	//FVCO = MDIV * FINClk / PDIV;
	FVCO = 2 * MDIV * FINClk / PDIV;

	if(FVCO >= 9.6e8 && FVCO <= 2.06e9)	/* FVCO should be 1GHz ~ 1.4GHz in S5PV210 low voltage. */
	{
		*pIsVSELHigh = FALSE;
	}
	#if 0
	else if(FVCO >= 1.4e9 && FVCO <= 2.0e9)	/* FVCO should be 1GHz ~ 1.4GHz in S5PV210 high voltage. */
	{
		if(pPLLCtrlInfo->m_EnVSELHigh == TRUE)
		{
			*pIsVSELHigh = TRUE;
		}
		else
		{
			return UART_ERROR;
		}
	}
	#endif /* 0 */
	else
	{
		return UART_ERROR;
	}
	
	//Fout = MDIV * FINClk / (PDIV * (2^(SDIV-1)));
	SDivExpo = 1 << (SDIV-1);
	Fout = MDIV * FINClk / (PDIV * SDivExpo);
	#if 0
	if(Fout > 3.2e7 || Fout > 2.0e9)			/* FOUT sould be 32Mhz ~ 2GHz */
	{
		return UART_ERROR;
	}
	#endif /* 0 */
	
	*pFreq = Fout;

	return UART_NO_ERROR;
}

s32 PLL_CalculateFreq_MPLL(PLL_CtrlInfo_st *pPLLCtrlInfo, float *pFreq, u32 PDIV, u32 MDIV, u32 SDIV, bool *pIsVSELHigh)
{
	float		FINClk;
	float		FRef;
	float		FVCO;
	float		Fout;
	u32		SDivExpo;

	FINClk = (float)FIN;

	if(PDIV == 0)
	{
		UART_ASSERT();
	}

	FRef = FINClk/PDIV;
	if(FRef < 3.0e6 || FRef > 6.0e6)	/* FRef should be 3MHz ~ 6MHz in S5PV210. */
	{
		return UART_ERROR;
	}

	FVCO = MDIV * FINClk / PDIV;

	if(FVCO >= 1.0e9 && FVCO <= 1.4e9)	/* FVCO should be 1GHz ~ 1.4GHz in S5PV210 low voltage. */
	{
		*pIsVSELHigh = FALSE;
	}
	else if(FVCO >= 1.4e9 && FVCO <= 2.0e9)	/* FVCO should be 1GHz ~ 1.4GHz in S5PV210 high voltage. */
	{
		if(pPLLCtrlInfo->m_EnVSELHigh == TRUE)
		{
			*pIsVSELHigh = TRUE;
		}
		else
		{
			return UART_ERROR;
		}
	}
	else
	{
		return UART_ERROR;
	}
	
	//Fout = MDIV * FINClk / (PDIV * (2^SDIV));
	SDivExpo = 1 << SDIV;
	Fout = MDIV * FINClk / (PDIV * SDivExpo);
	if(Fout < 3.2e7 || Fout > 2.0e9)			/* FOUT sould be 32Mhz ~ 2GHz */
	{
		return UART_ERROR;
	}
	
	*pFreq = Fout;

	return UART_NO_ERROR;
}

s32 PLL_CalculateFreq_EPLL(PLL_CtrlInfo_st *pPLLCtrlInfo, float *pFreq, u32 PDIV, u32 MDIV, u32 SDIV, bool *pIsVSELHigh)
{
	float		FINClk;
	float		FRef;
	float		FVCO;
	float		Fout;
	u32		SDivExpo;

	FINClk = (float)FIN;

	if(PDIV == 0)
	{
		UART_ASSERT();
	}

	FRef = FINClk/PDIV;
	if(FRef < 2.0e6 || FRef > 6.0e6)	/* FRef should be 3MHz ~ 6MHz in S5PV210. */
	{
		return UART_ERROR;
	}

	FVCO = MDIV * FINClk / PDIV;

	if(FVCO >= 3.3e8 && FVCO <= 4.6e8)	/* FVCO should be 1GHz ~ 1.4GHz in S5PV210 low voltage. */
	{
		*pIsVSELHigh = FALSE;
	}
	else if(FVCO >= 4.6e8 && FVCO <= 6.6e8)	/* FVCO should be 1GHz ~ 1.4GHz in S5PV210 high voltage. */
	{
		if(pPLLCtrlInfo->m_EnVSELHigh == TRUE)
		{
			*pIsVSELHigh = TRUE;
		}
		else
		{
			return UART_ERROR;
		}
	}
	else
	{
		return UART_ERROR;
	}

	//Fout = MDIV * FINClk / (PDIV * (2^SDIV));
	SDivExpo = 1 << SDIV;
	Fout = MDIV * FINClk / (PDIV * SDivExpo);
	if(Fout < 1.0e7 || Fout > 6.0e8)			/* FOUT sould be 32Mhz ~ 2GHz */
	{
		return UART_ERROR;
	}
	
	*pFreq = Fout;

	return UART_NO_ERROR;
}


s32 PLL_CalculateFreq_VPLL(PLL_CtrlInfo_st *pPLLCtrlInfo, float *pFreq, u32 PDIV, u32 MDIV, u32 SDIV, bool *pIsVSELHigh)
{
	return PLL_CalculateFreq_EPLL(pPLLCtrlInfo, pFreq, PDIV, MDIV, SDIV, pIsVSELHigh);	/* same to EPLL in S5PV210 */
}

s32 PLL_GetPMSValue(char *pPLLName, PLL_CtrlInfo_st *pPLLCtrlInfo, 
	PLL_GetCalcFreq_t GetCalcFreqFunction, float TargetClk, u32 *pPDIV, u32 *pMDIV, u32 *pSDIV)
{
	PLL_PMSEntry_st	*pPMSEntry;
	float		AccuracyMin;
	float		AccuracyMax;
	s32		MaxPmsEntry;
	s32		PmsEntryIdx;
	s32		MaxAccuracy;
	s32		AccuracyIdx;
	float		CalculatedClk;
	u32		Epll_PDIV;
	u32		EPll_MDIV;
	u32		EPll_SDIV;
	bool		IsHighVSel;
	s32		Status;
	s32		SelNum;
	u32		LocalCnt;

	MaxPmsEntry = pPLLCtrlInfo->m_ValidPMSEntryLimit;
	MaxAccuracy = pPLLCtrlInfo->m_AccurateEntryLimit;
	for(LocalCnt = 0; LocalCnt < MaxAccuracy; LocalCnt++)
	{
		pPLLCtrlInfo->m_AccurateEntry[LocalCnt] = LocalCnt * pPLLCtrlInfo->m_AccurateRate;
	}
	
	memset(pPLLCtrlInfo->m_ValidPMSEntry, 0x00, sizeof(pPLLCtrlInfo->m_ValidPMSEntry));

	PmsEntryIdx = 0;
	pPLLCtrlInfo->m_ValidPMSEntryCnt = 0;

	UART_Printf("\nWait a moment (a few seconds)....");

	/* same to the TargetClk */
	for(Epll_PDIV = pPLLCtrlInfo->m_PDIVMin; Epll_PDIV <= pPLLCtrlInfo->m_PDIVMax; Epll_PDIV++)
	{
		for(EPll_MDIV = pPLLCtrlInfo->m_MDIVMin; EPll_MDIV <= pPLLCtrlInfo->m_MDIVMax; EPll_MDIV++)
		{
			for(EPll_SDIV = pPLLCtrlInfo->m_SDIVMin; EPll_SDIV <= pPLLCtrlInfo->m_SDIVMax; EPll_SDIV++)
			{
				Status = (*GetCalcFreqFunction)(pPLLCtrlInfo, &CalculatedClk, Epll_PDIV, EPll_MDIV, EPll_SDIV, &IsHighVSel);
				if(Status == UART_NO_ERROR)
				{
					if(CalculatedClk == TargetClk)
					{
						if(PmsEntryIdx < MaxPmsEntry)
						{
							pPMSEntry = &pPLLCtrlInfo->m_ValidPMSEntry[PmsEntryIdx];
							pPMSEntry->m_Fout = CalculatedClk;
							pPMSEntry->m_PDIV = Epll_PDIV;
							pPMSEntry->m_MDIV = EPll_MDIV;
							pPMSEntry->m_SDIV = EPll_SDIV;
							pPMSEntry->m_IsVSELHigh = IsHighVSel;
							PmsEntryIdx++;
						}
					}
				}
			}
		}
	}

	/* not same to the TargetClk but sometimes useful. */
	if(pPLLCtrlInfo->m_EnSameTargetClk != TRUE || pPLLCtrlInfo->m_ValidPMSEntryCnt == 0)	/* a exactly same to the TargetClk is not found. */
	{
		for(AccuracyIdx = 1; AccuracyIdx < MaxAccuracy; AccuracyIdx++)
		{
			AccuracyMin = TargetClk * pPLLCtrlInfo->m_AccurateEntry[AccuracyIdx-1];
			AccuracyMax = TargetClk * pPLLCtrlInfo->m_AccurateEntry[AccuracyIdx];

			for(Epll_PDIV = pPLLCtrlInfo->m_PDIVMin; Epll_PDIV <= pPLLCtrlInfo->m_PDIVMax; Epll_PDIV++)
			{
				for(EPll_MDIV = pPLLCtrlInfo->m_MDIVMin; EPll_MDIV <= pPLLCtrlInfo->m_MDIVMax; EPll_MDIV++)
				{
					for(EPll_SDIV = pPLLCtrlInfo->m_SDIVMin; EPll_SDIV <= pPLLCtrlInfo->m_SDIVMax; EPll_SDIV++)
					{
						Status = (*GetCalcFreqFunction)(pPLLCtrlInfo, &CalculatedClk, Epll_PDIV, EPll_MDIV, EPll_SDIV, &IsHighVSel);
						if(Status == UART_NO_ERROR)
						{
							if( (CalculatedClk >= TargetClk - AccuracyMax && CalculatedClk < TargetClk - AccuracyMin) ||
								(CalculatedClk > TargetClk + AccuracyMin && CalculatedClk <= TargetClk + AccuracyMax) )
							{
								if(PmsEntryIdx < MaxPmsEntry)
								{
									pPMSEntry = &pPLLCtrlInfo->m_ValidPMSEntry[PmsEntryIdx];
									pPMSEntry->m_Fout = CalculatedClk;
									pPMSEntry->m_PDIV = Epll_PDIV;
									pPMSEntry->m_MDIV = EPll_MDIV;
									pPMSEntry->m_SDIV = EPll_SDIV;
									pPMSEntry->m_IsVSELHigh = IsHighVSel;
									PmsEntryIdx++;
								}
								else
								{
									goto PMS_Found_Done;
								}
							}
						}
					}
				}
			}
		}
	}

PMS_Found_Done:

	UART_Printf("\n====================================================");
	UART_Printf("\n%s Valid PMS Value", pPLLName);
	UART_Printf("\n====================================================\n");

	pPLLCtrlInfo->m_ValidPMSEntryCnt = PmsEntryIdx;
	for(PmsEntryIdx = 0; PmsEntryIdx < pPLLCtrlInfo->m_ValidPMSEntryCnt; PmsEntryIdx++)
	{
		pPMSEntry = &pPLLCtrlInfo->m_ValidPMSEntry[PmsEntryIdx];
		
		UART_Printf("\n%2d. PDIV(%2d) MDIV(%4d) SDIV(%1d) %s : %9.4f MHz", PmsEntryIdx,
			pPMSEntry->m_PDIV, pPMSEntry->m_MDIV, pPMSEntry->m_SDIV, 
			pPMSEntry->m_IsVSELHigh ? "HIGH" : "LOW ", pPMSEntry->m_Fout/1.0e6);
	}

	UART_Printf("\n%2d. Exit", PmsEntryIdx);
	UART_Printf("\n====================================================");

	UART_Printf("\nSelect: ");
	SelNum = UART_GetIntNum();
	if(SelNum < 0 || SelNum >= pPLLCtrlInfo->m_ValidPMSEntryCnt)	/* Out ouf entry range */
	{
		return UART_ERROR;
	}

	pPMSEntry = &pPLLCtrlInfo->m_ValidPMSEntry[SelNum];
	*pPDIV = pPMSEntry->m_PDIV;
	*pMDIV = pPMSEntry->m_MDIV;
	*pSDIV = pPMSEntry->m_SDIV;

	return UART_NO_ERROR;
}

void UART_DisplayPMS_APLL(void)
{
	UART_Printf("APLL(%.2fMHz)", (float)g_uAPLL/1.0e6);
}

void UART_DisplayPMS_MPLL(void)
{
	UART_Printf("MPLL(%.2fMHz)", (float)g_uMPLL/1.0e6);
}

void UART_DisplayPMS_EPLL(void)
{
	UART_Printf("EPLL(%.2fMHz)", (float)g_uEPLL/1.0e6);
}

void UART_DisplayPMS_VPLL(void)
{
	UART_Printf("VPLL(%.2fMHz)", (float)g_uVPLL/1.0e6);
}


void UART_SetPMS_APLL(void)
{
	PLL_CtrlInfo_st		PLLCtrlInfo;
	float		TargetClock;
	u32		PDIV;
	u32		MDIV;
	u32		SDIV;
	s32		Status;
	s32		SelNum;
	float		ClockUnit;

	UART_Printf("\nInput Target Clock (960M ~ 2060M) : ");
	UART_Printf("\nInput Target Clock unit");
	UART_Printf("\n");

	UART_Printf("\n0.Hz");
	UART_Printf("\n1.KHz");
	UART_Printf("\n2.MHz(D)");
	UART_Printf("\n");

	UART_Printf("\nChoose : ");
	SelNum = UART_GetIntNum();
	switch(SelNum)
	{
		case 0:
			ClockUnit = 1.0;
			break;
		case 1:
			ClockUnit = 1.0e3;
			break;
		case 2:
		default:
			ClockUnit = 1.0e6;
			break;
	}

	UART_Printf("\nInput Target Clock : ");
	SelNum = UART_GetIntNum();
	if(SelNum <= 0)
	{
		return;
	}

	TargetClock = (float)SelNum;
	TargetClock *= ClockUnit;
	if(TargetClock < 9.6e7 || TargetClock > 2.06e9)
	{
		UART_Printf("\nInValid Clock Frequency : %9.4f MHz", TargetClock / 1.0e6);
		
		return;
	}

	memset(&PLLCtrlInfo, 0x00, sizeof(PLL_CtrlInfo_st));
	
	PLLCtrlInfo.m_ValidPMSEntryLimit = PLL_MAX_VALID_ENTRY;
	PLLCtrlInfo.m_AccurateEntryLimit = PLL_MAX_ACCURATE_ENTRY;
	PLLCtrlInfo.m_AccurateRate = 0.01;		/* 1 % degree */
	PLLCtrlInfo.m_PDIVMin = 1;
	PLLCtrlInfo.m_PDIVMax = 63;
	PLLCtrlInfo.m_MDIVMin = 64;
	PLLCtrlInfo.m_MDIVMax = 1023;
	PLLCtrlInfo.m_SDIVMin = 1;
	PLLCtrlInfo.m_SDIVMax = 5;
	PLLCtrlInfo.m_EnSameTargetClk = FALSE;
	PLLCtrlInfo.m_EnVSELHigh = FALSE;

	Status = PLL_GetPMSValue("APLL", &PLLCtrlInfo, PLL_CalculateFreq_APLL, TargetClock, &PDIV, &MDIV, &SDIV);
	if(Status == UART_NO_ERROR)
	{
		UART_Printf("\nPDIV(%d), MDIV(%d), SDIV(%d) but NOT applied to the system.", PDIV, MDIV, SDIV);

		#if 0
		SYSC_StartPLLbyMPS(eAPLL, MDIV, PDIV, SDIV);
		while(!SYSC_IsLockDetected(eAPLL))
		{
			/* Wait until PLL is locked. */
		}
		
		SYSC_UpdateClkInform();
		#endif /* 0 */
	}
}

void UART_SetPMS_MPLL(void)
{
	PLL_CtrlInfo_st		PLLCtrlInfo;
	float		TargetClock;
	u32		PDIV;
	u32		MDIV;
	u32		SDIV;
	s32		Status;
	s32		SelNum;
	float		ClockUnit;

	UART_Printf("\nInput Target Clock (32M ~ 2G) : ");
	UART_Printf("\nInput Target Clock unit");
	UART_Printf("\n");

	UART_Printf("\n0.Hz");
	UART_Printf("\n1.KHz");
	UART_Printf("\n2.MHz(D)");
	UART_Printf("\n");

	UART_Printf("\nChoose : ");
	SelNum = UART_GetIntNum();
	switch(SelNum)
	{
		case 0:
			ClockUnit = 1.0;
			break;
		case 1:
			ClockUnit = 1.0e3;
			break;
		case 2:
		default:
			ClockUnit = 1.0e6;
			break;
	}

	UART_Printf("\nInput Target Clock : ");
	SelNum = UART_GetIntNum();
	if(SelNum <= 0)
	{
		return;
	}

	TargetClock = (float)SelNum;
	TargetClock *= ClockUnit;
	if(TargetClock < 3.2e7 || TargetClock > 2.0e9)
	{
		UART_Printf("\nInValid Clock Frequency : %9.4f MHz", TargetClock / 1.0e6);
		
		return;
	}

	memset(&PLLCtrlInfo, 0x00, sizeof(PLL_CtrlInfo_st));
	
	PLLCtrlInfo.m_ValidPMSEntryLimit = PLL_MAX_VALID_ENTRY;
	PLLCtrlInfo.m_AccurateEntryLimit = PLL_MAX_ACCURATE_ENTRY;
	PLLCtrlInfo.m_AccurateRate = 0.01;		/* 1 % degree */
	PLLCtrlInfo.m_PDIVMin = 1;
	PLLCtrlInfo.m_PDIVMax = 63;
	PLLCtrlInfo.m_MDIVMin = 64;
	PLLCtrlInfo.m_MDIVMax = 1023;
	PLLCtrlInfo.m_SDIVMin = 0;
	PLLCtrlInfo.m_SDIVMax = 4;
	PLLCtrlInfo.m_EnSameTargetClk = FALSE;
	PLLCtrlInfo.m_EnVSELHigh = FALSE;

	Status = PLL_GetPMSValue("MPLL", &PLLCtrlInfo, PLL_CalculateFreq_MPLL, TargetClock, &PDIV, &MDIV, &SDIV);
	if(Status == UART_NO_ERROR)
	{
		UART_Printf("\nPDIV(%d), MDIV(%d), SDIV(%d)", PDIV, MDIV, SDIV);
		
		SYSC_StartPLLbyMPS(eMPLL, MDIV, PDIV, SDIV);
		while(!SYSC_IsLockDetected(eMPLL))
		{
			/* Wait until PLL is locked. */
		}
		
		SYSC_UpdateClkInform();
	}
}


void UART_SetPMS_EPLL(void)
{
	PLL_CtrlInfo_st		PLLCtrlInfo;
	float		TargetClock;
	u32		PDIV;
	u32		MDIV;
	u32		SDIV;
	s32		Status;
	s32		SelNum;
	float		ClockUnit;

	UART_Printf("\nValid Clock Range(10M ~ 600M)");
	UART_Printf("\nInput Target Clock unit");
	UART_Printf("\n");

	UART_Printf("\n0.Hz");
	UART_Printf("\n1.KHz");
	UART_Printf("\n2.MHz(D)");
	UART_Printf("\n");

	UART_Printf("\nChoose : ");
	SelNum = UART_GetIntNum();
	switch(SelNum)
	{
		case 0:
			ClockUnit = 1.0;
			break;
		case 1:
			ClockUnit = 1.0e3;
			break;
		case 2:
		default:
			ClockUnit = 1.0e6;
			break;
	}

	UART_Printf("\nInput Target Clock : ");
	SelNum = UART_GetIntNum();
	if(SelNum <= 0)
	{
		return;
	}

	TargetClock = (float)SelNum;
	TargetClock *= ClockUnit;
	if(TargetClock < 1.0e7 || TargetClock > 6.0e8)
	{
		UART_Printf("\nInValid Clock Frequency : %9.4f MHz", TargetClock / 1.0e6);
		
		return;
	}

	memset(&PLLCtrlInfo, 0x00, sizeof(PLL_CtrlInfo_st));
	
	PLLCtrlInfo.m_ValidPMSEntryLimit = PLL_MAX_VALID_ENTRY;
	PLLCtrlInfo.m_AccurateEntryLimit = PLL_MAX_ACCURATE_ENTRY;
	PLLCtrlInfo.m_AccurateRate = 0.01;		/* 1 % degree */
	PLLCtrlInfo.m_PDIVMin = 1;
	PLLCtrlInfo.m_PDIVMax = 63;
	PLLCtrlInfo.m_MDIVMin = 16;
	PLLCtrlInfo.m_MDIVMax = 255;
	PLLCtrlInfo.m_SDIVMin = 0;
	PLLCtrlInfo.m_SDIVMax = 4;
	PLLCtrlInfo.m_EnSameTargetClk = FALSE;
	PLLCtrlInfo.m_EnVSELHigh = FALSE;

	Status = PLL_GetPMSValue("EPLL", &PLLCtrlInfo, PLL_CalculateFreq_EPLL, TargetClock, &PDIV, &MDIV, &SDIV);
	if(Status == UART_NO_ERROR)
	{
		UART_Printf("\nPDIV(%d), MDIV(%d), SDIV(%d)", PDIV, MDIV, SDIV);
		
		SYSC_StartPLLbyMPS(eEPLL, MDIV, PDIV, SDIV);
		while(!SYSC_IsLockDetected(eEPLL))
		{
			/* Wait until PLL is locked. */
		}
		
		SYSC_UpdateClkInform();
	}
}

void UART_SetPMS_VPLL(void)
{
	PLL_CtrlInfo_st		PLLCtrlInfo;
	float		TargetClock;
	u32		PDIV;
	u32		MDIV;
	u32		SDIV;
	s32		Status;
	s32		SelNum;
	float		ClockUnit;

	UART_Printf("\nValid Clock Range(10M ~ 600M)");
	UART_Printf("\nInput Target Clock unit");
	UART_Printf("\n");

	UART_Printf("\n0.Hz");
	UART_Printf("\n1.KHz");
	UART_Printf("\n2.MHz(D)");
	UART_Printf("\n");

	UART_Printf("\nChoose : ");
	SelNum = UART_GetIntNum();
	switch(SelNum)
	{
		case 0:
			ClockUnit = 1.0;
			break;
		case 1:
			ClockUnit = 1.0e3;
			break;
		case 2:
		default:
			ClockUnit = 1.0e6;
			break;
	}

	UART_Printf("\nInput Target Clock : ");
	SelNum = UART_GetIntNum();
	if(SelNum <= 0)
	{
		return;
	}

	TargetClock = (float)SelNum;
	TargetClock *= ClockUnit;
	if(TargetClock < 1.0e7 || TargetClock > 6.0e8)
	{
		UART_Printf("\nInValid Clock Frequency : %9.4f MHz", TargetClock / 1.0e6);
		
		return;
	}

	memset(&PLLCtrlInfo, 0x00, sizeof(PLL_CtrlInfo_st));
	
	PLLCtrlInfo.m_ValidPMSEntryLimit = PLL_MAX_VALID_ENTRY;
	PLLCtrlInfo.m_AccurateEntryLimit = PLL_MAX_ACCURATE_ENTRY;
	PLLCtrlInfo.m_AccurateRate = 0.01;		/* 1 % degree */
	PLLCtrlInfo.m_PDIVMin = 1;
	PLLCtrlInfo.m_PDIVMax = 63;
	PLLCtrlInfo.m_MDIVMin = 16;
	PLLCtrlInfo.m_MDIVMax = 255;
	PLLCtrlInfo.m_SDIVMin = 0;
	PLLCtrlInfo.m_SDIVMax = 4;
	PLLCtrlInfo.m_EnSameTargetClk = FALSE;
	PLLCtrlInfo.m_EnVSELHigh = FALSE;

	Status = PLL_GetPMSValue("VPLL", &PLLCtrlInfo, PLL_CalculateFreq_VPLL, TargetClock, &PDIV, &MDIV, &SDIV);
	if(Status == UART_NO_ERROR)
	{
		UART_Printf("\nPDIV(%d), MDIV(%d), SDIV(%d)", PDIV, MDIV, SDIV);
		
		SYSC_StartPLLbyMPS(eVPLL, MDIV, PDIV, SDIV);
		while(!SYSC_IsLockDetected(eVPLL))
		{
			/* Wait until PLL is locked. */
		}
		
		SYSC_UpdateClkInform();
	}
}


typedef void (*UART_fnDisplayPLL_t)(void);
typedef void (*UART_fnSetPLL_t)(void);

typedef struct {
	UART_fnDisplayPLL_t	m_DisplayPLL;
	UART_fnSetPLL_t		m_SetPLL;
} UART_SetPLLEntry_st;

UART_SetPLLEntry_st gUART_SetPLLEntry[] = 
{
	{UART_DisplayPMS_APLL, UART_SetPMS_APLL},
	{UART_DisplayPMS_MPLL, UART_SetPMS_MPLL},
	{UART_DisplayPMS_EPLL, UART_SetPMS_EPLL},
	{UART_DisplayPMS_VPLL, UART_SetPMS_VPLL}
};


void UART_SetPLL(UART_CtrlInfo_st *pCtrlInfo)
{
	s32				MenuEntryCnt;
	s32				MenuIndx;
	s32				SelNum;
	UART_SetPLLEntry_st	*pSetPLLEntry;

	MenuEntryCnt = sizeof(gUART_SetPLLEntry)/sizeof(UART_SetInfoEntry_st);
	while(1)
	{
		UART_Printf("\n====================================================");
		UART_Printf("\n   SET APLL/MPLL/EPLL/VPLL");
		UART_Printf("\n====================================================\n");
		for(MenuIndx = 0; MenuIndx<MenuEntryCnt; MenuIndx++)
		{
			pSetPLLEntry = &gUART_SetPLLEntry[MenuIndx];

			UART_Printf("\n%2d. ", MenuIndx);
			if(pSetPLLEntry->m_DisplayPLL != NULL)
			{
				(*pSetPLLEntry->m_DisplayPLL)();
			}

			if(pSetPLLEntry->m_SetPLL == NULL)
			{
				UART_Printf("\t(Display Only)");
			}
		}
		UART_Printf("\n%2d. Exit", MenuIndx);
		UART_Printf("\n====================================================");

		UART_Printf("\nSelect: ");
		SelNum = UART_GetIntNum();
		//if(SelNum <= 0 || SelNum >= MenuEntryCnt)
		if(SelNum == MenuEntryCnt)	/* Out ouf entry range */
		{
			break;
		}

		if(SelNum >= 0 && SelNum < MenuEntryCnt)
		{
			pSetPLLEntry = &gUART_SetPLLEntry[SelNum];
			if(pSetPLLEntry->m_SetPLL != NULL)
			{
				(*pSetPLLEntry->m_SetPLL)();
			}
		}
	}
}



// File Name : Uart_AutoTest(LoopBack mode)
// File Description : Check Uart LoopBack mode 
// Input : NONE
// Output : NONE
// Version : 0.1
u8 Uart_AutoTest(void)
{
	UartCh		CurDebugCh;
	UartCh		Ch;
	UART_CON	*pUart;
	s32			Status;

	/* Initialize the Default variable and settings. */
	UART_SetDefaultSystemEnv();			/* Set Clock, TZPC */
	UART_InitDefaultVariable(&gUART_CtrlInfo);
	UART_Init(&gUART_CtrlInfo);

	UART_Printf("\n\n====================================================");
	UART_Printf("\n   Welcome to UART Test World!");
	UART_Printf("\n====================================================\n");
	
	CurDebugCh = UART_GetCurDebugCh();

	#if 1
	for(Ch = UartCh_CH0; Ch < MAX_UART_CH; Ch++)
	{
		if(Ch != CurDebugCh)
		{
			pUart = &gUART_CtrlInfo.m_UartChTestInfo[Ch];
			#if 1
			Status = UART_IntLoopback_UartAPollingTest(pUart);
			#else
			Status = UART_ExtLoopback_UartAPollingTest(pUart);
			#endif /* 0 */
			if(Status != UART_NO_ERROR)
			{
				UART_Printf("\nUART%d Loopback Test Error.", Ch);
				
				return 0;
			}
			else
			{
				UART_Printf("\nUART%d Loopback Test Success.", Ch);
			}
		}
		else
		{
			UART_Printf("\nSkip UART%d.", Ch);
		}
	}
	#else
	{
		UART_CON	*pUartA;
		UART_CON	*pUartB;

		#if 1
		pUartA = &gUART_CtrlInfo.m_UartChTestInfo[UartCh_CH0];
		pUartB = &gUART_CtrlInfo.m_UartChTestInfo[UartCh_CH1];
		#else
		pUartA = &gUART_CtrlInfo.m_UartChTestInfo[UartCh_CH1];
		pUartB = &gUART_CtrlInfo.m_UartChTestInfo[UartCh_CH2];
		#endif /* 0 */

		Status = UART_IntChLoopback_UartAUartBPollingTest(pUartA, pUartB);
		if(Status != UART_NO_ERROR)
		{
			UART_Printf("\nUART%d Loopback Test Error.", pUartA->m_UartChIdx);
			
			return 0;
		}
		else
		{
			UART_Printf("\nUART%d Loopback Test Success.", pUartA->m_UartChIdx);
		}

		Status = UART_IntChLoopback_UartAUartBPollingTest(pUartB, pUartA);
		if(Status != UART_NO_ERROR)
		{
			UART_Printf("\nUART%d Loopback Test Error.", pUartA->m_UartChIdx);
			
			return 0;
		}
		else
		{
			UART_Printf("\nUART%d Loopback Test Success.", pUartA->m_UartChIdx);
		}

	}
	#endif /* 0 */

	return 1;
}


