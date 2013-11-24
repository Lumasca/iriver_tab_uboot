//--------------------------------------------------------
//
//
//	Melfas MCS7000 Series Download base v1.0 2010.04.05
//
//    modifyed by JHLIM iriver.
//--------------------------------------------------------
#include "mx100_inc.h"

//struct gpio_i2c_slave_ctl g_gpio_i2c_slave_ctl_melfas_ts;

#include "melfas_download.h"

//============================================================
//
//	Porting order
//
//============================================================
/*

1. melfas_download_porting.h
- Check typedef	[melfas_download_porting.h]

- Check download options	[melfas_download_porting.h]

- Add Port control code  ( CE, RESETB, I2C,... )	[melfas_download_porting.h]

- Apply your delay function ( inside mcsdl_delaly() )	[melfas_download.c]
Modify delay parameter constant ( ex. MCSDL_DELAY_5MS ) to make it fit to your delay function.

- Rename 'uart_printf()' to your console print function for debugging. [melfas_download_porting.h]
or, define uart_printf() as different function properly.

- Check Watchdog timer, Interrupt factor

- Including Melfas binary .c file

- Basenad dealy function
fill up mcsdl_delay()

- Implement processing external Melfas binary .bin file.

*/

//============================================================
//
//	Type define
//
//============================================================

typedef char				INT8;
typedef unsigned char		UINT8;
//typedef unsigned char		uint8_t;
typedef short				INT16;
typedef unsigned short		UINT16;
//typedef unsigned short		uint16_t;
typedef int					INT32;
typedef unsigned int		UINT32;
typedef unsigned char		BOOLEAN;


#ifndef TRUE
#define TRUE 				(1==1)
#endif

#ifndef FALSE
#define FALSE 				(1==0)
#endif

#ifndef NULL
#define NULL 				0
#endif

#define uart_printf printf

//============================================================
//
//	Porting Download Options
//
//============================================================

// For printing debug information. ( Please check 'printing function' )
#define MELFAS_ENABLE_DBG_PRINT											1
#define MELFAS_ENABLE_DBG_PROGRESS_PRINT								1

/* JHLIM */
#define ENABLE_PROGRESS_UI												1

// For delay function test. ( Disable after Porting is finished )
#define MELFAS_ENABLE_DELAY_TEST										1


//============================================================
//
//	IO Control poting.
//
//	Fill 'Using signal' up only.
// 	See MCSDL_USE_VDD_CONTROL,
//		MCSDL_USE_CE_CONTROL,
//
//============================================================

//----------------
// VDD
//----------------
#if MCSDL_USE_VDD_CONTROL

#define MCSDL_VDD_SET_HIGH()         	
#define MCSDL_VDD_SET_LOW()         		

#else

#define MCSDL_VDD_SET_HIGH()             			// Nothing
#define MCSDL_VDD_SET_LOW()              			// Nothing

#endif


//----------------
// CE
//----------------
#if MCSDL_USE_CE_CONTROL
#define MCSDL_CE_SET_HIGH()   	         	GPIO_SetDataEach(U_GPIO_TOUCH_ENABLE_OUT,1)
#define MCSDL_CE_SET_LOW()      		GPIO_SetDataEach(U_GPIO_TOUCH_ENABLE_OUT,0)
#define MCSDL_CE_SET_OUTPUT()   	       	GPIO_SetFunctionEach(U_GPIO_TOUCH_ENABLE_OUT, eGPO)	
//____HERE!_____
#else
#define MCSDL_CE_SET_HIGH()							// Nothing
#define MCSDL_CE_SET_LOW()							// Nothing
#define MCSDL_CE_SET_OUTPUT()						// Nothing
#endif


//----------------
// RESETB
//----------------

#define MCSDL_RESETB_SET_HIGH()            	GPIO_SetDataEach(U_GPIO_TOUCH_INT_IN,1)
#define MCSDL_RESETB_SET_LOW()             	GPIO_SetDataEach(U_GPIO_TOUCH_INT_IN,0)
#define MCSDL_RESETB_SET_OUTPUT()       	GPIO_SetFunctionEach(U_GPIO_TOUCH_INT_IN, eGPO)
#define MCSDL_RESETB_SET_INPUT()          	GPIO_SetFunctionEach(U_GPIO_TOUCH_INT_IN, eGPI)

//------------------
// I2C SCL & SDA
//------------------

#define MCSDL_GPIO_SCL_SET_HIGH()		GPIO_SetDataEach(U_GPIO_TOUCH_SCL_IO,1)
#define MCSDL_GPIO_SCL_SET_LOW()		GPIO_SetDataEach(U_GPIO_TOUCH_SCL_IO,0)

#define MCSDL_GPIO_SDA_SET_HIGH()		GPIO_SetDataEach(U_GPIO_TOUCH_SDA_IO,1)
#define MCSDL_GPIO_SDA_SET_LOW()		GPIO_SetDataEach(U_GPIO_TOUCH_SDA_IO,0)

#define MCSDL_GPIO_SCL_SET_OUTPUT()		GPIO_SetFunctionEach(U_GPIO_TOUCH_SCL_IO, eGPO)
#define MCSDL_GPIO_SCL_SET_INPUT()		GPIO_SetFunctionEach(U_GPIO_TOUCH_SCL_IO, eGPI)

#define MCSDL_GPIO_SDA_SET_OUTPUT()		GPIO_SetFunctionEach(U_GPIO_TOUCH_SDA_IO, eGPO)
#define MCSDL_GPIO_SDA_SET_INPUT()		GPIO_SetFunctionEach(U_GPIO_TOUCH_SDA_IO, eGPI)

#define MCSDL_GPIO_SDA_IS_HIGH()			 GPIO_GetDataEach(U_GPIO_TOUCH_SDA_IO)


#if MCSDL_USING_HW_I2C
#define MCSDL_SET_GPIO_I2C()						____HERE!_____
#define MCSDL_SET_HW_I2C()							____HERE!_____
#else
#define MCSDL_SET_GPIO_I2C()						// Nothing
#define MCSDL_SET_HW_I2C()							// Nothing
#endif




//============================================================
//
//	Delay parameter setting
//
//	These are used on 'mcsdl_delay()'
//
//============================================================
#define MCSDL_DELAY_1US                                                      1
#define MCSDL_DELAY_2US								    2
#define MCSDL_DELAY_3US								    3
#define MCSDL_DELAY_5US								    5
#define MCSDL_DELAY_7US								    7

#define MCSDL_DELAY_10US							   10
#define MCSDL_DELAY_15US							   15
#define MCSDL_DELAY_20US                   						   20
#define MCSDL_DELAY_40US                   						   40
#define MCSDL_DELAY_100US							  100
#define MCSDL_DELAY_150US							  150
#define MCSDL_DELAY_300US							  300
#define MCSDL_DELAY_500US             				  500

#define MCSDL_DELAY_1MS								 1000
#define MCSDL_DELAY_5MS								 5000
#define MCSDL_DELAY_10MS							10000
#define MCSDL_DELAY_25MS							25000
#define MCSDL_DELAY_30MS							30000
#define MCSDL_DELAY_40MS							40000
#define MCSDL_DELAY_45MS							45000


//============================================================
//
//	Defence External Effect
//
//============================================================
#if 0

#define MELFAS_DISABLE_BASEBAND_ISR()				____HERE!_____
#define MELFAS_DISABLE_WATCHDOG_TIMER_RESET()		____HERE!_____

#define MELFAS_ROLLBACK_BASEBAND_ISR()				____HERE!_____
#define MELFAS_ROLLBACK_WATCHDOG_TIMER_RESET()		____HERE!_____

#else

#define MELFAS_DISABLE_BASEBAND_ISR()				// Nothing
#define MELFAS_DISABLE_WATCHDOG_TIMER_RESET()		// Nothing

#define MELFAS_ROLLBACK_BASEBAND_ISR()				// Nothing
#define MELFAS_ROLLBACK_WATCHDOG_TIMER_RESET()		// Nothing

#endif



//============================================================
//
//	Include MELFAS Binary code File ( ex> MELFAS_FIRM_bin.c)
//
//	Warning!!!!
//		Please, don't add binary.c file into project
//		Just #include here !!
//
//============================================================

//#include "MELFAS_FIRMWARE_R01_V00_bin.c"
//#include "MELFAS_FIRMWARE_R10_V01_bin.c"
//#include "MTH_MX100_RB10_VA04_bin.c"

UINT8  ucVerifyBuffer[MELFAS_TRANSFER_LENGTH];		//	You may melloc *ucVerifyBuffer instead of this


//---------------------------------
//	Downloading functions
//---------------------------------
static int  mcsdl_download(const UINT8 *pData, const UINT16 nLength);

static void mcsdl_set_ready(void);
static void mcsdl_reboot_mcs(void);

static int  mcsdl_erase_flash(void);
static int  mcsdl_program_flash( UINT8 *pDataOriginal, UINT16 unLength );
static void mcsdl_program_flash_part( UINT8 *pData, UINT8 ucLength );

static int  mcsdl_verify_flash( UINT8 *pData, UINT16 nLength );

static void mcsdl_read_flash( UINT8 *pBuffer);

static int  mcsdl_read_flash_from( UINT8 *pBuffer, UINT16 unStart_addr, UINT16 unLength);

static void mcsdl_select_isp_mode(UINT8 ucMode);
static void mcsdl_unselect_isp_mode(void);

static void mcsdl_read_32bits( UINT8 *pData );
static void mcsdl_write_bits(UINT32 wordData, int nBits);
static void mcsdl_scl_toggle_twice(void);

void mcsdl_delay_test(INT32 nCount);

//---------------------------------
//	Delay functions
//---------------------------------
//static void mcsdl_delay(UINT32 nCount);


static void mcsdl_delay(UINT32 nCount)
{
	udelay(nCount * 2);
#if 0
    switch (nCount)
	{
	case MCSDL_DELAY_1US:
		
        break;
	case MCSDL_DELAY_2US:
		
        break;
	case MCSDL_DELAY_3US:
		
        break;    
	case MCSDL_DELAY_5US:
		
        break;        
	case MCSDL_DELAY_15US:
		
        break;    
	case MCSDL_DELAY_100US:
		
        break;    
	case MCSDL_DELAY_150US:
		
        break;    
	case MCSDL_DELAY_500US:
		
        break;        
	case MCSDL_DELAY_1MS:
		
        break;    
	case MCSDL_DELAY_5MS:
		
        break;    
	case MCSDL_DELAY_10MS:
		
        break;    
	case MCSDL_DELAY_25MS:
		
        break;    
	case MCSDL_DELAY_30MS:
		
        break;    
	case MCSDL_DELAY_45MS:
		
        break;
	default:
		
        break;
		
	}
#endif
}


//---------------------------------
//	For debugging display
//---------------------------------
#if MELFAS_ENABLE_DBG_PRINT
static void mcsdl_print_result(int nRet);
#endif


//----------------------------------
// Download enable command
//----------------------------------
#if MELFAS_USE_PROTOCOL_COMMAND_FOR_DOWNLOAD

void melfas_send_download_enable_command(void)
{
	// TO DO : Fill this up
	printf(" reset touch...\n");
	
	
	GPIO_SetFunctionEach(U_GPIO_TOUCH_ENABLE_OUT, eGPO);	
	GPIO_SetPullUpDownEach(U_GPIO_TOUCH_ENABLE_OUT ,eGPUen);
	GPIO_SetDataEach(U_GPIO_TOUCH_ENABLE_OUT,0);
	mcsdl_delay(MCSDL_DELAY_100US);
	GPIO_SetDataEach(U_GPIO_TOUCH_ENABLE_OUT,1);
}

#endif

int IRIVER_Touch_Fw_Up_Test(void)
{
	mcsdl_delay_test(1000000);
}

int IRIVER_Touch_Fw_Update(void)
{
#define MELFAS_ID_WR		(0x20)
#define MELFAS_ID_R 		(0x20)
	int i;
	
	char data[32];
	
	int iRet;
	
	//	display_text(0,COLOR_WHITE,"Melfas Touch Fw update:");
	
	GPIO_SetPullUpDownEach(U_GPIO_TOUCH_ENABLE_OUT ,eGPUDdis);
	GPIO_SetPullUpDownEach(U_GPIO_TOUCH_INT_IN ,eGPUDdis);
	GPIO_SetPullUpDownEach(U_GPIO_TOUCH_SCL_IO ,eGPUDdis);
	GPIO_SetPullUpDownEach(U_GPIO_TOUCH_SDA_IO ,eGPUDdis);
	
	GPIO_SetFunctionEach(U_GPIO_TOUCH_ENABLE_OUT, eGPO);	
	
	mcsdl_download_binary_data();
	
	
}

//============================================================
//
//	Main Download furnction
//
//============================================================

int mcsdl_download_binary_data(void)
{
	int nRet;
	
#if MELFAS_USE_PROTOCOL_COMMAND_FOR_DOWNLOAD
	melfas_send_download_enable_command();
	mcsdl_delay(MCSDL_DELAY_100US);
#endif
	
	MELFAS_DISABLE_BASEBAND_ISR();					// Disable Baseband touch interrupt ISR.
	MELFAS_DISABLE_WATCHDOG_TIMER_RESET();			// Disable Baseband watchdog timer
	
	//------------------------
	// Run Download
	//------------------------
	{
		extern int read_from_sd (char *file,char **readed_buffer, int *file_size);
		char *melfas_binary;
		int file_size;
		
		if(read_from_sd("melfas_ts.bin",&melfas_binary,&file_size) == 0) {  // not exist sdfuse/melfas_ts.bin 
			mx100_display_log(LOG_FAIL,"melfas_ts.bin file not exist !!!\n check t-flash");
			return 0;
			//	nRet = mcsdl_download( (const UINT8*) MELFAS_binary, (const UINT16)MELFAS_binary_nLength );
		} else {
			mx100_display_log(LOG_NOR,"update from /sdfuse/melfas_ts.bin");
			nRet = mcsdl_download( (const UINT8*) melfas_binary, (const UINT16)file_size );
		}
	}
	MELFAS_ROLLBACK_BASEBAND_ISR();					// Roll-back Baseband touch interrupt ISR.
	MELFAS_ROLLBACK_WATCHDOG_TIMER_RESET();			// Roll-back Baseband watchdog timer
	
	return ( nRet == MCSDL_RET_SUCCESS );
}



int mcsdl_download_binary_file(void)
{
	int nRet;
	
	UINT8  *pBinary = NULL;
	UINT16 nBinary_length =0;
	
	//==================================================
	//
	//	1. Read '.bin file'
	//  2. *pBinary       : Binary data
	//	   nBinary_length : Firmware size
	//	3. Run mcsdl_download( pBinary, nBinary_length);
	//
	//==================================================
	
	
#if MELFAS_USE_PROTOCOL_COMMAND_FOR_DOWNLOAD
    melfas_send_download_enable_command();
    mcsdl_delay(MCSDL_DELAY_100US);
#endif
    
    MELFAS_DISABLE_BASEBAND_ISR();                  // Disable Baseband touch interrupt ISR.
    MELFAS_DISABLE_WATCHDOG_TIMER_RESET();          // Disable Baseband watchdog timer
	
	if( pBinary != NULL && nBinary_length > 0 && nBinary_length < 32*1024 ){
		
		//------------------------
		// Run Download
		//------------------------
		nRet = mcsdl_download( (const UINT8 *)pBinary, (const UINT16)nBinary_length );
	}else{
		
		nRet = MCSDL_RET_WRONG_BINARY;
	}
	
    MELFAS_ROLLBACK_BASEBAND_ISR();                 // Roll-back Baseband touch interrupt ISR.
    MELFAS_ROLLBACK_WATCHDOG_TIMER_RESET();         // Roll-back Baseband watchdog timer
    
	
#if MELFAS_ENABLE_DBG_PRINT
	mcsdl_print_result( nRet );
#endif
	
#if 0
	if( pData != NULL )										// free memory alloced.
		free(pData);
#endif
	
	return ( nRet == MCSDL_RET_SUCCESS );
	
}

//------------------------------------------------------------------
//
//	Download function
//
//------------------------------------------------------------------

static int mcsdl_download(const UINT8 *pBianry, const UINT16 unLength )
{
	int nRet;
	
	//---------------------------------
	// Check Binary Size
	//---------------------------------
	if( unLength >= MELFAS_FIRMWARE_MAX_SIZE )
	{
		
		nRet = MCSDL_RET_PROGRAM_SIZE_IS_WRONG;
		goto MCSDL_DOWNLOAD_FINISH;
	}
	
	
#if MELFAS_ENABLE_DBG_PROGRESS_PRINT
	uart_printf(" - Starting download...\n");
#endif
	
	
	//---------------------------------
	// Make it ready
	//---------------------------------
#if MELFAS_ENABLE_DBG_PROGRESS_PRINT
	uart_printf(" > Ready\n");
#endif
	
	mcsdl_set_ready();
	
	//---------------------------------
	// Erase Flash
	//---------------------------------
	
#if MELFAS_ENABLE_DBG_PROGRESS_PRINT
	uart_printf(" > Erase\n");
#endif
	
	mx100_display_log(LOG_NOR,"erase flash...");
	mx100_set_progress_total(20);
	
	nRet = mcsdl_erase_flash();
	
	if( nRet != MCSDL_RET_SUCCESS ) 
	{
		uart_printf(" Erase Error\n");
		
		//	goto MCSDL_DOWNLOAD_FINISH;
	}
	mx100_display_log(LOG_NOR,"erase flash end...");
	
	//---------------------------------
	// Program Flash
	//---------------------------------
#if MELFAS_ENABLE_DBG_PROGRESS_PRINT
	uart_printf(" > Program   ");
#endif
	mx100_display_log(LOG_NOR,"start  flash program...");
	
	mx100_set_progress_total(40);
	
	nRet = mcsdl_program_flash( (UINT8*)pBianry, (UINT16)unLength );
	if( nRet != MCSDL_RET_SUCCESS ) 
	{
		uart_printf(" program Error\n");
		
		goto MCSDL_DOWNLOAD_FINISH;
	}
	//---------------------------------
	// Verify flash
	//---------------------------------
	
#if MELFAS_ENABLE_DBG_PROGRESS_PRINT
	uart_printf(" > Verify    ");
#endif
	
	mx100_display_log(LOG_NOR,"verify flash program...");
	mx100_set_progress_total(60);
	
	nRet = mcsdl_verify_flash( (UINT8*)pBianry, (UINT16)unLength );
	
	if( nRet != MCSDL_RET_SUCCESS )
		goto MCSDL_DOWNLOAD_FINISH;
	
	mx100_set_progress_total(90);
	
	nRet = MCSDL_RET_SUCCESS;
	
	
MCSDL_DOWNLOAD_FINISH :
	
#if MELFAS_ENABLE_DBG_PRINT
	mcsdl_print_result( nRet );								// Show result
#endif
	
#if MELFAS_ENABLE_DBG_PROGRESS_PRINT
	uart_printf(" > Rebooting\n");
	uart_printf(" - Fin.\n\n");
#endif
	
	mcsdl_reboot_mcs();
	
	return nRet;
}



//------------------------------------------------------------------
//
//	Sub functions
//
//------------------------------------------------------------------

static int mcsdl_erase_flash(void)
{
	int	  i;
	UINT8 readBuffer[32] = {0,};
	
	//----------------------------------------
	//	Do erase
	//----------------------------------------
	mcsdl_select_isp_mode(ISP_MODE_ERASE_FLASH);
	mcsdl_unselect_isp_mode();
	
	//----------------------------------------
	//	Check 'erased well'
	//----------------------------------------
	
	
	mcsdl_read_flash_from(  readBuffer	  , 0x0000, 16 );
	mcsdl_read_flash_from( &readBuffer[16], 0x7FF0, 16 );
	
	// Compare with '0xFF'
	for(i=0; i<32; i++){
		if( readBuffer[i] != 0xFF )
			return MCSDL_RET_ERASE_FLASH_VERIFY_FAILED;
	}
	
	return MCSDL_RET_SUCCESS;
}


static int mcsdl_program_flash( UINT8 *pDataOriginal, UINT16 unLength )
{
	int i;
	int display_count=0;
	int write_len = 0;
	
	
	UINT8   *pData;
	UINT8   ucLength;
	
	UINT16  addr;
	UINT32  header;
	
	addr   = 0;
	pData  = pDataOriginal;
	
    ucLength = MELFAS_TRANSFER_LENGTH;
	
#if MELFAS_ENABLE_DBG_PROGRESS_PRINT
	mx100_set_progress(0);
	
#endif
	
	//kang
	
	while( (addr*4) < (int)unLength)	{
		
        if( ( unLength - (addr*4) ) < MELFAS_TRANSFER_LENGTH ){
            ucLength  = (UINT8)(unLength - (addr*4) );
        }
		
		//--------------------------------------
		//	Select ISP Mode
		//--------------------------------------
		
		// start ADD DELAY        
        mcsdl_delay(MCSDL_DELAY_40US);
		//end ADD DELAY        
		mcsdl_select_isp_mode( ISP_MODE_SERIAL_WRITE );
		
		//---------------------------------------------
		//  Header
		//  Address[13ibts] <<1
		//---------------------------------------------
		header = ((addr&0x1FFF) << 1) | 0x0 ;
		header = header << 14;
		// Write 18bits
		mcsdl_write_bits( header, 18 );
		
		//---------------------------------
		//	Writing
		//---------------------------------
		addr +=1;
		
#if MELFAS_ENABLE_DBG_PROGRESS_PRINT
		if(display_count++%64==0) {		
			uart_printf("#");
			mx100_set_progress((write_len * 100)  / unLength);
		}
		
#endif
		
		mcsdl_program_flash_part(pData, 0);
		
		pData  += ucLength;
		
		write_len += ucLength;
		
		//---------------------------------------------
		//	Tail
		//---------------------------------------------
        MCSDL_GPIO_SDA_SET_HIGH();
		//kang
		mcsdl_delay(MCSDL_DELAY_40US);
        for(i=0; i<6; i++){
			
			if( i==2 ) mcsdl_delay(MCSDL_DELAY_20US);
            else if( i==3 ) mcsdl_delay(MCSDL_DELAY_40US);
			
            MCSDL_GPIO_SCL_SET_HIGH();  mcsdl_delay(MCSDL_DELAY_10US);
            MCSDL_GPIO_SCL_SET_LOW();   mcsdl_delay(MCSDL_DELAY_10US);
        }
		
		mcsdl_unselect_isp_mode();
		//start ADD DELAY      
        mcsdl_delay(MCSDL_DELAY_300US);
		//end ADD DELAY 
	}
	
#if MELFAS_ENABLE_DBG_PROGRESS_PRINT
	mx100_set_progress(100);
#endif
	
#if MELFAS_ENABLE_DBG_PROGRESS_PRINT
	uart_printf("\n");
#endif
	
	
	
	return MCSDL_RET_SUCCESS;
}

static void mcsdl_program_flash_part( UINT8 *pData, UINT8 ucLength )
{
	int     i;
	UINT32	data;
	
	//---------------------------------
	//	Body
	//---------------------------------
	
	data  = (UINT32)pData[0] <<  0;
	data |= (UINT32)pData[1] <<  8;
	data |= (UINT32)pData[2] << 16;
	data |= (UINT32)pData[3] << 24;
	mcsdl_write_bits(data, 32);
	
	
	
}

static int mcsdl_verify_flash( UINT8 *pDataOriginal, UINT16 unLength )
{
	int	  i, j;
	int	  nRet;
	int 	display_count = 0;
	int    read_len = 0;
	
	UINT8 *pData;
	UINT8 ucLength;
	
	UINT16 addr;
	UINT32 wordData;
	
	addr  = 0;
	pData = (UINT8 *) pDataOriginal;
	
	ucLength  = MELFAS_TRANSFER_LENGTH;
	
#if MELFAS_ENABLE_DBG_PROGRESS_PRINT
	mx100_set_progress(0);
	
#endif
	
    while(  (addr*4) < (int)unLength){
		
        if( ( unLength -  (addr*4) ) < MELFAS_TRANSFER_LENGTH ){
            ucLength = (UINT8)(unLength -  (addr*4) );
        }
		// start ADD DELAY        
        mcsdl_delay(MCSDL_DELAY_40US);
		//--------------------------------------
		//	Select ISP Mode
		//--------------------------------------
		mcsdl_select_isp_mode(ISP_MODE_SERIAL_READ);
		
		
		//---------------------------------------------
		//	Header
		//	Address[13ibts] <<1
		//---------------------------------------------
		
		wordData   = ( (addr&0x1FFF) << 1 ) | 0x0;
		wordData <<= 14;
		
		mcsdl_write_bits( wordData, 18 );
        addr+=1;
		
#if MELFAS_ENABLE_DBG_PROGRESS_PRINT
		if(display_count++%64==0) {		
			uart_printf("#");
			mx100_set_progress((read_len * 100)  / unLength);
			
		}
#endif
		
		//--------------------
		// Read flash
		//--------------------
		mcsdl_read_flash( ucVerifyBuffer);
		
		//kang
#if 1  
		MCSDL_GPIO_SDA_SET_HIGH();
		
		//            mcsdl_delay(MCSDL_DELAY_1MS);
		for(i=0; i<6; i++){
			
			if( i==2 ) mcsdl_delay(MCSDL_DELAY_3US);
			else if( i==3 ) mcsdl_delay(MCSDL_DELAY_40US);//(MCSDL_DELAY_1MS);
            
			MCSDL_GPIO_SCL_SET_HIGH();  mcsdl_delay(MCSDL_DELAY_10US);
			MCSDL_GPIO_SCL_SET_LOW();   mcsdl_delay(MCSDL_DELAY_10US);
		}
		//kang
#endif
		//--------------------
		// Comparing
		//--------------------
		
		for(j=0; j<(int)ucLength; j++){
			
#if MELFAS_ENABLE_DBG_PROGRESS_PRINT
			//	uart_printf(" %02X", ucVerifyBuffer[j] );
#endif
			
			
			if( ucVerifyBuffer[j] != pData[j] ){
#if MELFAS_ENABLE_DBG_PROGRESS_PRINT
				uart_printf("\nAddress : 0x%04X : 0x%02X - 0x%02X\n", addr, pData[j], ucVerifyBuffer[j] );
#endif
				nRet = MCSDL_RET_PROGRAM_VERIFY_FAILED;
				goto MCSDL_VERIFY_FLASH_FINISH;
				
			}
		}
		
		pData += ucLength;
		read_len += ucLength;
		
		mcsdl_unselect_isp_mode();
		
	}
	
	
	
	nRet = MCSDL_RET_SUCCESS;
	
#if MELFAS_ENABLE_DBG_PROGRESS_PRINT
	mx100_set_progress(100);
#endif
	
MCSDL_VERIFY_FLASH_FINISH:
	
#if MELFAS_ENABLE_DBG_PROGRESS_PRINT
	uart_printf("\n");
#endif
	
	mcsdl_unselect_isp_mode();
	
	return nRet;
}


static void mcsdl_read_flash( UINT8 *pBuffer)
{
	int i;
    
    MCSDL_GPIO_SDA_SET_LOW();
	
	mcsdl_delay(MCSDL_DELAY_40US);
	
    for (i=0; i< 5; i++){
        MCSDL_GPIO_SCL_SET_HIGH();  mcsdl_delay(MCSDL_DELAY_10US);
        MCSDL_GPIO_SCL_SET_LOW();  mcsdl_delay(MCSDL_DELAY_10US);
	}
	
	mcsdl_read_32bits( pBuffer );
}


static int mcsdl_read_flash_from( UINT8 *pBuffer, UINT16 unStart_addr, UINT16 unLength)
{
	int	  i;
	int j;
	
	UINT8  ucLength;
	
	UINT16 addr;
	UINT32 wordData;
	
	if( unLength >= MELFAS_FIRMWARE_MAX_SIZE ){
		return MCSDL_RET_PROGRAM_SIZE_IS_WRONG;
	}
	
	addr  = 0;
	
	ucLength  = MELFAS_TRANSFER_LENGTH;
	
#if MELFAS_ENABLE_DBG_PROGRESS_PRINT
	//uart_printf(" %04X : ", unStart_addr );
#endif
	
	for( i = 0; i < (int)unLength; i+=(int)ucLength ){
		addr = (UINT16)i;
        
        mcsdl_select_isp_mode(ISP_MODE_SERIAL_READ);
        wordData   = ( ((unStart_addr + addr)&0x1FFF) << 1 ) | 0x0;
        wordData <<= 14;
        
        mcsdl_write_bits( wordData, 18 );
		
		if( ( unLength - addr ) < MELFAS_TRANSFER_LENGTH ){
			ucLength = (UINT8)(unLength - addr);
		}
		
		//--------------------
		// Read flash
		//--------------------
		mcsdl_read_flash( &pBuffer[addr]);
		
#if MELFAS_ENABLE_DBG_PROGRESS_PRINT
		for(j=0; j<(int)ucLength; j++){
			printf("%02X ", pBuffer[j] );
		}
#endif
        
        mcsdl_unselect_isp_mode();
		
	}
	
	
	return MCSDL_RET_SUCCESS;
	
}


static void mcsdl_set_ready(void)
{
	//--------------------------------------------
	// Tkey module reset
	//--------------------------------------------
	
	MCSDL_VDD_SET_LOW();
	
	MCSDL_CE_SET_LOW();
	MCSDL_CE_SET_OUTPUT();
	
	MCSDL_SET_GPIO_I2C();
	
	MCSDL_GPIO_SDA_SET_LOW();
	MCSDL_GPIO_SDA_SET_OUTPUT();
	
	MCSDL_GPIO_SCL_SET_LOW();
	MCSDL_GPIO_SCL_SET_OUTPUT();
	
	MCSDL_RESETB_SET_LOW();
	MCSDL_RESETB_SET_OUTPUT();
	
	mcsdl_delay(MCSDL_DELAY_25MS);						// Delay for Stable VDD
	
	MCSDL_VDD_SET_HIGH();
	MCSDL_CE_SET_HIGH();
	
	MCSDL_GPIO_SDA_SET_HIGH();
	
	mcsdl_delay(MCSDL_DELAY_40MS); 						// Delay '30 msec'
	
}


static void mcsdl_reboot_mcs(void)
{
	//--------------------------------------------
	// Tkey module reset
	//--------------------------------------------
	
	MCSDL_VDD_SET_LOW();
	
	MCSDL_CE_SET_LOW();
	MCSDL_CE_SET_OUTPUT();
	
	MCSDL_GPIO_SDA_SET_HIGH();
	MCSDL_GPIO_SDA_SET_OUTPUT();
	
	MCSDL_GPIO_SCL_SET_HIGH();
	MCSDL_GPIO_SCL_SET_OUTPUT();
	
	MCSDL_SET_HW_I2C();
	
	MCSDL_RESETB_SET_LOW();
	MCSDL_RESETB_SET_OUTPUT();
	
	mcsdl_delay(MCSDL_DELAY_25MS);						// Delay for Stable VDD
	
	MCSDL_RESETB_SET_INPUT();
	MCSDL_VDD_SET_HIGH();
	MCSDL_CE_SET_HIGH();
	
	mcsdl_delay(MCSDL_DELAY_30MS); 						// Delay '25 msec'
	
}


//--------------------------------------------
//
//   Write ISP Mode entering signal
//
//--------------------------------------------

static void mcsdl_select_isp_mode(UINT8 ucMode)
{
	int    i;
	
	UINT8 enteringCodeMassErase[16]   = { 0,1,0,1,1,0,0,1,1,1,1,1,0,0,1,1 };
	UINT8 enteringCodeSerialWrite[16] = { 0,1,1,0,0,0,1,0,1,1,0,0,1,1,0,1 };
	UINT8 enteringCodeSerialRead[16]  = { 0,1,1,0,1,0,1,0,1,1,0,0,1,0,0,1 };
	
	
	UINT8 *pCode;
	
	
	//------------------------------------
	// Entering ISP mode : Part 1
	//------------------------------------
	if( ucMode == ISP_MODE_ERASE_FLASH       ) pCode = enteringCodeMassErase;
	else if( ucMode == ISP_MODE_SERIAL_WRITE      ) pCode = enteringCodeSerialWrite;
	else if( ucMode == ISP_MODE_SERIAL_READ       ) pCode = enteringCodeSerialRead;
	
	for(i=0; i<16; i++){
		
		if( pCode[i] == 1 )	
            MCSDL_RESETB_SET_HIGH();
		else			
            MCSDL_RESETB_SET_LOW();
		
		//start add delay for INT 
        mcsdl_delay(MCSDL_DELAY_3US);        
		//end delay for INT 
		
		MCSDL_GPIO_SCL_SET_HIGH();	mcsdl_delay(MCSDL_DELAY_3US);
		MCSDL_GPIO_SCL_SET_LOW();	mcsdl_delay(MCSDL_DELAY_3US);
	}
	
	MCSDL_RESETB_SET_HIGH();			// High
	
	//---------------------------------------------------
	// Entering ISP mode : Part 2	- Only Mass Erase
	//---------------------------------------------------
	
	if( ucMode == ISP_MODE_ERASE_FLASH   ){
        mcsdl_delay(MCSDL_DELAY_7US);
		for(i=0; i<4; i++){
			
			if( i==2 ) mcsdl_delay(MCSDL_DELAY_25MS);
			else if( i==3 ) mcsdl_delay(MCSDL_DELAY_150US);
			
			MCSDL_GPIO_SCL_SET_HIGH();	mcsdl_delay(MCSDL_DELAY_3US);
			MCSDL_GPIO_SCL_SET_LOW();	mcsdl_delay(MCSDL_DELAY_7US);
		}
	}
	
}

static void mcsdl_unselect_isp_mode(void)
{
	int i;
	
	// MCSDL_GPIO_SDA_SET_HIGH();
	// MCSDL_GPIO_SDA_SET_OUTPUT();
	
	MCSDL_RESETB_SET_LOW();		
	mcsdl_delay(MCSDL_DELAY_3US);
	
	for(i=0; i<10; i++){
		
		MCSDL_GPIO_SCL_SET_HIGH();	mcsdl_delay(MCSDL_DELAY_3US);
		MCSDL_GPIO_SCL_SET_LOW();	mcsdl_delay(MCSDL_DELAY_3US);
	}
	
	
}


static void mcsdl_read_32bits( UINT8 *pData )
{
	int i, j;
	
	MCSDL_GPIO_SDA_SET_INPUT();
#if 1
	for (i=3; i >= 0; i--){
		
		pData[i] = 0;
		
		for (j=0; j<8; j++){
			
			pData[i] <<= 1;
			
			MCSDL_GPIO_SCL_SET_HIGH();  mcsdl_delay(MCSDL_DELAY_1US);       
			MCSDL_GPIO_SCL_SET_LOW();       mcsdl_delay(MCSDL_DELAY_1US);
			
			if ( MCSDL_GPIO_SDA_IS_HIGH() )
				pData[i] |= 0x01;
			
			
		}
	}
	
#else
	for (i=3; i<=0; i--){
		
		pData[i] = 0;
		
		for (j=0; j<8; j++){
			
			pData[i] <<= 1;
			
			MCSDL_GPIO_SCL_SET_HIGH();  mcsdl_delay(MCSDL_DELAY_1US);       
			MCSDL_GPIO_SCL_SET_LOW();       mcsdl_delay(MCSDL_DELAY_1US);
			
			if ( MCSDL_GPIO_SDA_IS_HIGH() )
				pData[i] |= 0x01;
			
			
		}
	}
#endif
	
}



static void mcsdl_write_bits(UINT32 wordData, int nBits)
{
	int i;
	
	MCSDL_GPIO_SDA_SET_LOW();
	MCSDL_GPIO_SDA_SET_OUTPUT();
	
	for (i=0; i<nBits; i++){
		
		if ( wordData & 0x80000000 ) {	MCSDL_GPIO_SDA_SET_HIGH();	}
		else						 {	MCSDL_GPIO_SDA_SET_LOW();	}
		
		mcsdl_delay(MCSDL_DELAY_3US);
		
		MCSDL_GPIO_SCL_SET_HIGH();		mcsdl_delay(MCSDL_DELAY_3US);
		MCSDL_GPIO_SCL_SET_LOW();		mcsdl_delay(MCSDL_DELAY_3US);
		
		wordData <<= 1;
	}
}




static void mcsdl_scl_toggle_twice(void)
{
	
	MCSDL_GPIO_SDA_SET_HIGH();
	MCSDL_GPIO_SDA_SET_OUTPUT();
	
	MCSDL_GPIO_SCL_SET_HIGH();	mcsdl_delay(MCSDL_DELAY_20US);
	MCSDL_GPIO_SCL_SET_LOW();	mcsdl_delay(MCSDL_DELAY_20US);
	
	MCSDL_GPIO_SCL_SET_HIGH();	mcsdl_delay(MCSDL_DELAY_20US);
	MCSDL_GPIO_SCL_SET_LOW();	mcsdl_delay(MCSDL_DELAY_20US);
}




//============================================================
//
//	Debugging print functions.
//
//============================================================

#ifdef MELFAS_ENABLE_DBG_PRINT

static void mcsdl_print_result(int nRet)
{
	if( nRet == MCSDL_RET_SUCCESS ){
		
		uart_printf(" > MELFAS Firmware downloading SUCCESS.\n");
		mx100_display_log(LOG_OK,"MELFAS Firmware downloading SUCCESS.\n");
		
	}else{
		
		uart_printf(" > MELFAS Firmware downloading FAILED  :  ");
		mx100_display_log(LOG_FAIL,"MELFAS Firmware downloading FAILED  :  ");
		
		switch( nRet ){
			
		case MCSDL_RET_SUCCESS                  		:   		uart_printf("MCSDL_RET_SUCCESS\n" );                 		break;
		case MCSDL_RET_ERASE_FLASH_VERIFY_FAILED		:   uart_printf("MCSDL_RET_ERASE_FLASH_VERIFY_FAILED\n" );		break;
		case MCSDL_RET_PROGRAM_VERIFY_FAILED			:   uart_printf("MCSDL_RET_PROGRAM_VERIFY_FAILED\n" );      	break;
			
		case MCSDL_RET_PROGRAM_SIZE_IS_WRONG			:   uart_printf("MCSDL_RET_PROGRAM_SIZE_IS_WRONG\n" );    		break;
		case MCSDL_RET_VERIFY_SIZE_IS_WRONG				:   uart_printf("MCSDL_RET_VERIFY_SIZE_IS_WRONG\n" );      		break;
		case MCSDL_RET_WRONG_BINARY						:   uart_printf("MCSDL_RET_WRONG_BINARY\n" );      				break;
			
		case MCSDL_RET_READING_HEXFILE_FAILED       	:   uart_printf("MCSDL_RET_READING_HEXFILE_FAILED\n" );			break;
		case MCSDL_RET_FILE_ACCESS_FAILED       		:   uart_printf("MCSDL_RET_FILE_ACCESS_FAILED\n" );				break;
		case MCSDL_RET_MELLOC_FAILED     		  		:   uart_printf("MCSDL_RET_MELLOC_FAILED\n" );      			break;
			
		case MCSDL_RET_WRONG_MODULE_REVISION     		:   uart_printf("MCSDL_RET_WRONG_MODULE_REVISION\n" );      	break;
			
		default                             			:	uart_printf("UNKNOWN ERROR. [0x%02X].\n", nRet );      		break;
		}
		
		uart_printf("\n");
	}
	
}
#endif


#if MELFAS_ENABLE_DELAY_TEST

//============================================================
//
//	For initial testing of delay and gpio control
//
//	You can confirm GPIO control and delay time by calling this function.
//
//============================================================

void mcsdl_delay_test(INT32 nCount)
{
	INT16 i;
	
	MELFAS_DISABLE_BASEBAND_ISR();					// Disable Baseband touch interrupt ISR.
	MELFAS_DISABLE_WATCHDOG_TIMER_RESET();			// Disable Baseband watchdog timer
	
	//--------------------------------
	//	Repeating 'nCount' times
	//--------------------------------
	
	MCSDL_SET_GPIO_I2C();
	MCSDL_GPIO_SCL_SET_OUTPUT();
	MCSDL_GPIO_SDA_SET_OUTPUT();
	MCSDL_RESETB_SET_OUTPUT();
	
	MCSDL_GPIO_SCL_SET_HIGH();
	
	for( i=0; i<nCount; i++ ){
		
#if 1
		
		MCSDL_GPIO_SCL_SET_LOW();
		
		mcsdl_delay(MCSDL_DELAY_20US);
		
		MCSDL_GPIO_SCL_SET_HIGH();
		
		mcsdl_delay(MCSDL_DELAY_100US);
		
#elif 0
		
		MCSDL_GPIO_SCL_SET_LOW();
		
		mcsdl_delay(MCSDL_DELAY_500US);
		
		MCSDL_GPIO_SCL_SET_HIGH();
		
		mcsdl_delay(MCSDL_DELAY_1MS);
		
#else
		
		MCSDL_GPIO_SCL_SET_LOW();
		
		mcsdl_delay(MCSDL_DELAY_25MS);
		
		TKEY_INTR_SET_LOW();
		
		mcsdl_delay(MCSDL_DELAY_45MS);
		
		TKEY_INTR_SET_HIGH();
		
#endif
	}
	
	MCSDL_GPIO_SCL_SET_HIGH();
	
	MELFAS_ROLLBACK_BASEBAND_ISR();					// Roll-back Baseband touch interrupt ISR.
	MELFAS_ROLLBACK_WATCHDOG_TIMER_RESET();			// Roll-back Baseband watchdog timer
}
#endif


