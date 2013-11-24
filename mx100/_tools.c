/********************************************************************
 *		mx100 system tools
 *		created by JH.LIM		
 *		first revision .2010.10
 *		
 *********************************************************************/
 
#include "mx100_inc.h"

char *intToBinary(int width,int i) ;

struct i2c_slave_ctl g_i2c_ch2_ctl;

struct gpio_i2c_slave_ctl g_gpio_i2c_slave_ctl;


/////////////////////////////////////////////////////////////////////////////
//
//WM8993 codec
//
//
/////////////////////////////////////////////////////////////////////////////



int IRIVER_WM8993_Test(void)
{
	#define WM8993_I2C_CH (2)
	#define WM8993_ID_WR		0x34	
	int iRet;
	
	display_text(0,COLOR_WHITE,"Audio Codec(WM8993) Test ====> ");

	IRIVER_IIC_SlaveInit(&g_i2c_ch2_ctl,WM8993_I2C_CH,WM8993_ID_WR >>1,0x0);

	iRet = IRIVER_IIC_WriteByte(&g_i2c_ch2_ctl, 0x00, 0x3E);

	if(iRet==1) {		
		display_text(1,COLOR_BLUE,"OK\n");
		return 1;
	} else {
		display_text(1,COLOR_RED,"NG\n");
		return 0;
	}
}

/////////////////////////////////////////////////////////////////////////////
//
// cypress touch.
//
//
/////////////////////////////////////////////////////////////////////////////

#define MF_DEVICE_STATUS (0)
#define MF_OPERATING_MODE (0x1)
#define MF_SENSITIVITY_CONTROL (0x2)
#define MF_FILTER_LEVEL (0x3)
#define MF_XY_SIZE_UPPER (0xA)
#define MF_X_SIZE_LOWER (0xB)
#define MF_Y_SIZE_LOWER (0xc)
#define MF_INPUT_INFO (0x10)
#define MF_TS_POSITION_ENABLE (0x11)

#define MF_XY_POS0_UPPER (0x12)
#define MF_X_POS0_LOWER (0x13)
#define MF_Y_POS0_LOWER (0x14)

#define MF_XY_POS1_UPPER (0x15)
#define MF_X_POS1_LOWER (0x16)
#define MF_Y_POS1_LOWER (0x17)

#define MF_XY_POS2_UPPER (0x18)
#define MF_X_POS2_LOWER (0x19)
#define MF_Y_POS2_LOWER (0x1A)

#define MF_XY_POS3_UPPER (0x1B)
#define MF_X_POS3_LOWER (0x1C)
#define MF_Y_POS3_LOWER (0x1D)

#define MF_XY_POS4_UPPER (0x1E)
#define MF_X_POS4_LOWER (0x1F)
#define MF_Y_POS4_LOWER (0x20)


#define MF_Z_POS0_LOWER (0x21)
#define MF_Z_POS1_LOWER (0x22)
#define MF_Z_POS2_LOWER (0x23)
#define MF_Z_POS3_LOWER (0x24)
#define MF_Z_POS4_LOWER (0x25)

#define MF_TSP_REV	(0x40)
#define MF_HW_REV	 (0x41)
#define MF_COMP_GROUP (0x42)
#define MF_FW_VER (0x43)
#if 0
static int g_melfas_reg[] =
{
 MF_DEVICE_STATUS,
 MF_OPERATING_MODE ,
 MF_SENSITIVITY_CONTROL ,
 MF_FILTER_LEVEL ,
 MF_XY_SIZE_UPPER ,
 MF_X_SIZE_LOWER ,
 MF_Y_SIZE_LOWER ,
 MF_INPUT_INFO ,
 MF_TS_POSITION_ENABLE ,

 MF_XY_POS0_UPPER ,
 MF_X_POS0_LOWER ,
 MF_Y_POS0_LOWER ,

 MF_XY_POS1_UPPER,
 MF_X_POS1_LOWER ,
 MF_Y_POS1_LOWER ,

 MF_XY_POS2_UPPER,
 MF_X_POS2_LOWER ,
 MF_Y_POS2_LOWER ,

 MF_XY_POS3_UPPER,
 MF_X_POS3_LOWER ,
 MF_Y_POS3_LOWER ,

 MF_XY_POS4_UPPER,
 MF_X_POS4_LOWER ,
 MF_Y_POS4_LOWER ,


 MF_Z_POS0_LOWER ,
 MF_Z_POS1_LOWER ,
 MF_Z_POS2_LOWER ,
 MF_Z_POS3_LOWER ,
 MF_Z_POS4_LOWER ,

 MF_TSP_REV,
 MF_HW_REV,
 MF_COMP_GROUP,
 MF_FW_VER 
};
#endif

enum {MELFAS_TS_NG,MELFAS_TS_7000,MELFAS_TS_8000 };

int IRIVER_Touch_Test(void)
{
	int melfas_touch_type;
	
	#define MELFAS_I2C_CH (0)
	#define MELFAS_8000_ID_WR		(0x48)
	#define MELFAS_7000_ID_WR		(0x20)
//	#define MELFAS_ID_WR		(0x47)
	
	int iRet = 0;
	
	display_text(0,COLOR_WHITE,"Touch Screen(Melfas TS) Test ====> ");

	#if (MX100_HW_VER >= MX100_WS)
	GPIO_SetFunctionEach(U_GPIO_TOUCH_ENABLE_OUT, eGPO);	
	GPIO_SetPullUpDownEach(U_GPIO_TOUCH_ENABLE_OUT ,eGPUen);
	GPIO_SetDataEach(U_GPIO_TOUCH_ENABLE_OUT,1);
	udelay(300 * 1000); //
	#endif

	melfas_touch_type = MELFAS_TS_NG;

	IRIVER_IIC_SlaveInit(&g_i2c_ch2_ctl,MELFAS_I2C_CH,MELFAS_7000_ID_WR ,MELFAS_7000_ID_WR);

	iRet = IRIVER_IIC_ReadByte(&g_i2c_ch2_ctl, 0x40);

	if(iRet >= 0) {		
		melfas_touch_type = MELFAS_TS_7000;
		display_text(0,COLOR_WHITE,"melfas ts 7000 detected... ");
	} 
	else { 

		IRIVER_IIC_SlaveInit(&g_i2c_ch2_ctl,MELFAS_I2C_CH,MELFAS_8000_ID_WR ,MELFAS_8000_ID_WR);

		iRet = IRIVER_IIC_ReadByte(&g_i2c_ch2_ctl, 0x40);

		if(iRet >= 0) {		
			melfas_touch_type = MELFAS_TS_8000;		
			display_text(1,COLOR_WHITE,"melfas ts 8000 detected... ");
		} 
	}

	if(melfas_touch_type == MELFAS_TS_NG) {		
		display_text(1,COLOR_RED,"NG\n",iRet);
		return 0;
	} else {
		display_text(1,COLOR_BLUE,"OK:%d\n",iRet);
		return 1;
	}

}

int get_melfas_ts_version(void)
{
	unsigned char touch_data[32];
	int i,j;
	
	#define MELFAS_I2C_CH (0)
	#define MELFAS_ID_WR		(0x20)
	
	int iRet = 0;
	
	#if (MX100_HW_VER >= MX100_WS)
	GPIO_SetFunctionEach(U_GPIO_TOUCH_ENABLE_OUT, eGPO);	
	GPIO_SetPullUpDownEach(U_GPIO_TOUCH_ENABLE_OUT ,eGPUen);
	GPIO_SetDataEach(U_GPIO_TOUCH_ENABLE_OUT,0);
	GPIO_SetDataEach(U_GPIO_TOUCH_ENABLE_OUT,1);
	udelay(300 * 1000); //
	#endif
	

//	IRIVER_IIC_SlaveInit(&g_i2c_ch2_ctl,MELFAS_I2C_CH,MELFAS_ID_WR ,MELFAS_ID_WR);


	IRIVER_GPIO_IIC_SlaveInit(&g_gpio_i2c_slave_ctl,
						U_GPIO_TOUCH_SDA_IO,
						U_GPIO_TOUCH_SCL_IO,
						MELFAS_ID_WR,
						MELFAS_ID_WR);

	mx100_display_log(LOG_NOR,"0x40:%d",iRet);
	

	return iRet;
}




/////////////////////////////////////////////////////////////////////////////
//
// MAX17040 Fuel Gage Module
//
//
/////////////////////////////////////////////////////////////////////////////




int IRIVER_FuelGage_Test(void)
{
	#define MAX17040_I2C_CH (2)
	#define MAX17040_ID_W (0x6c)
	#define MAX17040_ID_R (0x6d)

	int iRet;
	int version;
	int vcell;
	int soc;
	
	printf("FuelGage Test");

	display_text(0,COLOR_WHITE,"Battery FuelGauge (max17040) Test ====> ");

#define U_GPIO_FUEL_GAUGE_SDA_IO 		eGPIO_J4, eGPIO_0
#define U_GPIO_FUEL_GAUGE_SCL_IO		eGPIO_J4, eGPIO_1


	IRIVER_GPIO_IIC_SlaveInit(&g_gpio_i2c_slave_ctl,
						U_GPIO_FUEL_GAUGE_SDA_IO,
						U_GPIO_FUEL_GAUGE_SCL_IO,
						MAX17040_ID_W >>1,
						MAX17040_ID_R>>1);

	iRet = IRIVER_GPIO_IIC_WriteByte(&g_gpio_i2c_slave_ctl, 0x00, 0x3E);

//	max17040_reset(&gpio_i2c_slave_ctl);

	version=max17040_get_version(&g_gpio_i2c_slave_ctl);

	vcell=max17040_get_vcell(&g_gpio_i2c_slave_ctl);

	soc=max17040_get_soc(&g_gpio_i2c_slave_ctl);

	display_text(0,COLOR_WHITE,"max17040 version : %x : vcell : %d soc:%d ===>",version,vcell,soc);
	#if 0
	for(i=0;i<100;i++) {

		vcell=max17040_get_vcell(&gpio_i2c_slave_ctl);
		printf("max17040 vcell : %d \r",vcell);
		udelay(500 * 1000);

	}
	#endif
	
//	Data = IRIVER_IIC_ReadByte(&gpio_i2c_slave_ctl, 0x00);

	if(iRet==1) {		
		display_text(1,COLOR_BLUE,"OK\n");
		return 1;
	} else {
		display_text(1,COLOR_RED,"NG\n");
		return 0;
	}
}


/*
 * BMA150 accelerometer driver Test.
  *
*/

int IRIVER_BMA150_Test(void)
{
	#define BMA150_ID_W (0x70 >> 1)
	#define BMA150_ID_R (0x70 >> 1)

	#define BMA150_CHIP_ID_REG              0x00

	#define BMA150_CHIP_ID                  0x02

	int iRet;
	
	display_text(0,COLOR_WHITE,"Accelerometer (bma150) Test ====> ");
	 #if (MX100_HW_VER >= MX100_WS)

	IRIVER_GPIO_IIC_SlaveInit(&g_gpio_i2c_slave_ctl,
						U_GPIO_BMA150_I2C_SDA_IO,
						U_GPIO_BMA150_I2C_SCL_IO,
						BMA150_ID_W,
						BMA150_ID_R);

	iRet = IRIVER_GPIO_IIC_ReadByte(&g_gpio_i2c_slave_ctl, BMA150_CHIP_ID_REG);

	if (iRet == BMA150_CHIP_ID) {
		display_text(1,COLOR_BLUE,"OK\n");	
		return 1;
	} else {
		display_text(1,COLOR_RED,"NG\n");
		return 0;
	}
	#else
		return 1;
	#endif
	
}

/*
 * BMA150 accelerometer driver Test.
  *
*/

int IRIVER_SI4709_Test(void)
{
	#define SI4709_ID_W (0x20 >> 1)
	#define SI4709_ID_R (0x20 >> 1)

	#define BOOTCONFIG              9       /* Boot Configuration */

	int iRet;

	display_text(0,COLOR_WHITE,"FM Radio (si4709) Test ====> ");

	#if (MX100_HW_VER >= MX100_WS)
	GPIO_SetFunctionEach(U_GPIO_FM_RESET_OUT, eGPO);	
	GPIO_SetPullUpDownEach(U_GPIO_FM_RESET_OUT ,eGPUen);
	GPIO_SetDataEach(U_GPIO_FM_RESET_OUT,0);
	udelay(10 * 1000); //
	GPIO_SetDataEach(U_GPIO_FM_RESET_OUT,1);
	udelay(20 * 1000); //
	

	IRIVER_GPIO_IIC_SlaveInit(&g_gpio_i2c_slave_ctl,
						U_GPIO_FM_I2C_SDA_IO,
						U_GPIO_FM_I2C_SCL_IO,
						SI4709_ID_W,
						SI4709_ID_R);

	iRet = IRIVER_GPIO_IIC_ReadByte(&g_gpio_i2c_slave_ctl, BOOTCONFIG);

	if (iRet >=0) {
		display_text(1,COLOR_BLUE,"OK\n");	
		return 1;
	} else {
		display_text(1,COLOR_RED,"NG\n");
		return 0;
	}
	#else

	return 0;
	#endif
	
}

//
//akm8975 compass driver test.
// 

int IRIVER_AKM8975_Test(void)
{
	#define AKM8975_ID_W (0x0c)
	#define AMK8975_ID_R (0x0c)

	#define AK8975_REG_WIA          0x00
	
	int iRet;

	display_text(0,COLOR_WHITE,"Compass (akm8975) Test ====> ");

    	#if (MX100_HW_VER >= MX100_WS)

	IRIVER_GPIO_IIC_SlaveInit(&g_gpio_i2c_slave_ctl,
						U_GPIO_COMPAS_I2C_SDA_IO,
						U_GPIO_COMPAS_I2C_SCL_IO,
						AKM8975_ID_W,
						AMK8975_ID_R);

	iRet = IRIVER_GPIO_IIC_ReadByte(&g_gpio_i2c_slave_ctl, AK8975_REG_WIA);

	if (iRet !=0x48) {
		display_text(1,COLOR_RED,"NG\n");
		return 0;
	} else {
		display_text(1,COLOR_BLUE,"OK\n");	
		return 1;
	}
	#else
	return 0;
	#endif
}

//
// Ambient light sensorr driver for ISL29023 Test.
//

#if 0
static int ld_isl29023_init_registers(struct isl29023_data *data)
{
	u8 low_lsb,low_msb, high_lsb, high_msb;
    uint8_t command1, command2;

    low_msb  = 0x4E; low_lsb  = 0x20;   // 20000 
    high_msb = 0x9C; high_lsb = 0x40;   // 40000
    
    command1 = ISL29023_OPERATION_MODE_ALS_CONTINUE | ISL29023_INT_PERSIST_4;
    command2 = ISL29023_RESOLUTION_16BIT | ISL29023_LUX_RANGE4;

	isl29023_write_reg(data, ISL29023_COMMAND_II, command2);
	isl29023_write_reg(data, ISL29023_ALSIR_LT_LSB,	low_lsb);
	isl29023_write_reg(data, ISL29023_ALSIR_LT_MSB,	low_msb);
	isl29023_write_reg(data, ISL29023_ALSIR_HT_LSB,	high_lsb);
	isl29023_write_reg(data, ISL29023_ALSIR_HT_MSB,	high_msb);

	isl29023_write_reg(data, ISL29023_COMMAND_I, command1);     // start


#if 0 /* ktj test */
    int i;
	u8 reg_val;

    for (i=0; i<8; i++)
    {
	    isl29023_read_reg(data, i, &reg_val);
	    rdebug("addr 0x%x = 0x%x ", i, reg_val);
	}
#endif	
	
	
	return 0;
}
#endif

int IRIVER_ISL29023_ALS_IR_Test(void)
{
	#define ISL29023_ID_W (0x88 >> 1)
	#define ISL29023_ID_R (0x88 >> 1)

#define ISL29023_COMMAND_II         0x01
#define ISL29023_RESOLUTION_16BIT           0x00
#define ISL29023_LUX_RANGE4         0x03

	unsigned char command2;
	
	int iRet;

	display_text(0,COLOR_WHITE,"Light Sensor (ais29023) Test ====> ");

    	#if (MX100_HW_VER >= MX100_WS)

	IRIVER_GPIO_IIC_SlaveInit(&g_gpio_i2c_slave_ctl,
						U_GPIO_LIGHT_SENSOR_SDA_IO,
						U_GPIO_LIGHT_SENSOR_SCL_IO,
						ISL29023_ID_W,
						ISL29023_ID_R);


    command2 = ISL29023_RESOLUTION_16BIT | ISL29023_LUX_RANGE4;

	iRet = IRIVER_GPIO_IIC_WriteByte(&g_gpio_i2c_slave_ctl, ISL29023_COMMAND_II, command2);

	if(iRet==1) {		
		display_text(1,COLOR_BLUE,"OK\n");
		return 1;
	} else {
		display_text(1,COLOR_RED,"NG\n");
		return 0;
	}
	#else
	return 0;
	#endif
}

//
//* Haptic driver for ISA1200 Test
//

int IRIVER_ISA1200_HapticTest(void)
{
	#define ISA1200_ID_W (0x90 >> 1)
	#define ISA1200_ID_R (0x90 >> 1)

	int iRet;

	display_text(0,COLOR_WHITE,"Haptic Vibrator(isa1200) Test ====> ");

	#if (MX100_HW_VER >= MX100_WS)

	IRIVER_GPIO_IIC_SlaveInit(&g_gpio_i2c_slave_ctl,
						U_GPIO_MOTOR_SDA_IO,
						U_GPIO_MOTOR_SCL_IO,
						ISA1200_ID_W,
						ISA1200_ID_R);

	GPIO_SetFunctionEach(U_GPIO_MOTOR_PWRCTL_OUT, eGPO);	
	GPIO_SetPullUpDownEach(U_GPIO_MOTOR_PWRCTL_OUT ,eGPUen);
	GPIO_SetDataEach(U_GPIO_MOTOR_PWRCTL_OUT,1);

	GPIO_SetFunctionEach(U_GPIO_MOTOR_HAPTIC_OUT, eGPO);	
	GPIO_SetPullUpDownEach(U_GPIO_MOTOR_HAPTIC_OUT ,eGPUen);
	GPIO_SetDataEach(U_GPIO_MOTOR_HAPTIC_OUT,1);

	udelay(5 * 1000);  // for charge on

	GPIO_SetDataEach(U_GPIO_MOTOR_HAPTIC_OUT,0);
	udelay(5 * 1000);  // for charge on

//---------------------------------------------------------------------------//

	iRet = IRIVER_GPIO_IIC_WriteByte(&g_gpio_i2c_slave_ctl, 0x31, 0x40);

	if(iRet==1) {		
		display_text(1,COLOR_BLUE,"OK\n");
		return 1;
	} else {
		display_text(1,COLOR_RED,"NG\n");
		return 0;
	}
	#else
	return 0;
	#endif
}

//
//* aat1271a flash driver Test
//

#define OFF 	0
#define ON		1

#define MOVIE_MODE_CURRENT			17
#define FLASH_SAFETY_TIMER			18
#define MOVIE_MODE_CONFIG			19
#define FLASH_TO_MOVIE_RATIO		20

#define MOVIE_MODE_CURRENT_71		4
#define MOVIE_MODE_CURRENT_63		5
#define MOVIE_MODE_CURRENT_56		6

static void aat1271a_flash_write(int addr, int data)
{
	int i;

	for (i = 0; i < addr; i++) {
		GPIO_SetDataEach(U_GPIO_CAM_FLASH_SET_OUT, GPIO_LEVEL_LOW);
		udelay(10);	
		GPIO_SetDataEach(U_GPIO_CAM_FLASH_SET_OUT, GPIO_LEVEL_HIGH);
		udelay(50);
	}

	udelay(500);	

	for (i = 0; i < data; i++) {
		GPIO_SetDataEach(U_GPIO_CAM_FLASH_SET_OUT, GPIO_LEVEL_LOW);
		udelay(10);	
		GPIO_SetDataEach(U_GPIO_CAM_FLASH_SET_OUT, GPIO_LEVEL_HIGH);
		udelay(50);
	}
	
	udelay(500);	
}
int g_movie_movie_current;
void aat1271a_falsh_movie_control(int ctrl)
{
	if (ctrl) {
		/* Falsh Mode Off */
		GPIO_SetDataEach(U_GPIO_CAM_FLASH_EN_OUT, GPIO_LEVEL_LOW);
		/* Movie Mode Current Setting & On */	
		aat1271a_flash_write(MOVIE_MODE_CURRENT, g_movie_movie_current);
	}
	else {
		/* Falsh Mode Off */
		GPIO_SetDataEach(U_GPIO_CAM_FLASH_EN_OUT, GPIO_LEVEL_LOW);
		/* Movie Mode Off */
		GPIO_SetDataEach(U_GPIO_CAM_FLASH_SET_OUT, GPIO_LEVEL_LOW);
	}
}

void mx100_aat1271a_movie_control(int onoff,int value)
{
		if(value >0) {
			g_movie_movie_current = value;
		}
		
		aat1271a_falsh_movie_control(onoff);
}

int IRIVER_AAT1271a_flash_Test(void)
{
	int i;
	display_text(0,COLOR_WHITE,"Flash LED (AAT1271a) Test ====> ");
	display_text(1,COLOR_BLUE,"see Flash LED light.\n");

	GPIO_SetFunctionEach(U_GPIO_CAM_FLASH_SET_OUT, eGPO);		
	GPIO_SetDataEach(U_GPIO_CAM_FLASH_SET_OUT, GPIO_LEVEL_LOW);
	GPIO_SetPullUpDownEach(U_GPIO_CAM_FLASH_SET_OUT ,eGPUDdis);

	GPIO_SetFunctionEach(U_GPIO_CAM_FLASH_EN_OUT, eGPO);		
	GPIO_SetDataEach(U_GPIO_CAM_FLASH_EN_OUT, GPIO_LEVEL_LOW);
	GPIO_SetPullUpDownEach(U_GPIO_CAM_FLASH_EN_OUT ,eGPUDdis);


	for(i=0;i<3;i++)   {
		mx100_aat1271a_movie_control(1,10);
		udelay(1000 * 500);
		mx100_aat1271a_movie_control(0,10);		
		udelay(1000 * 500);
	}


	return 1;
}

//
//* GPS uart port Test
//

#include <s5pc110.h>

int serial_tstc_gps(void)
{
	S5PC11X_UART *const uart = S5PC11X_GetBase_UART(S5PC11X_UART2);

	return uart->UTRSTAT & 0x1;
}
int serial_getc_gps(void)
{
	S5PC11X_UART *const uart = S5PC11X_GetBase_UART(S5PC11X_UART2);

	/* wait for character to arrive */
	while (!(uart->UTRSTAT & 0x1));

	return uart->URXH & 0xff;
}

void GPS_LDO_control(int onoff)
{
	#define MAX8698_I2C_CH (2)
	#define MAX8698_ID_WR	(0x66)	
	int iRet,RegValue0,RegValue1;

	IRIVER_IIC_SlaveInit(&g_i2c_ch2_ctl,MAX8698_I2C_CH,MAX8698_ID_WR,MAX8698_ID_WR);

//	RegValue0 = IRIVER_IIC_ReadByte(&g_i2c_ch2_ctl, 0x0);
//	printf("PMIC REG0 : %s\n",intToBinary(8,RegValue0));

	RegValue1 = IRIVER_IIC_ReadByte(&g_i2c_ch2_ctl, 0x1);

	if(onoff==1) {
	//	RegValue0 |= 0b00100000;  // LDO5 vddcam_28
		RegValue1 |= 0b00010000;  // LDO7 vddcam
	} else {
	//	RegValue0 &= ~0b00100000;  // LDO5 vddcam_28
		RegValue1 &= ~0b00010000;  // LDO7 vddcam
	}
	printf("PMIC REG1 : %s\n",intToBinary(8,RegValue1));   

//	IRIVER_IIC_WriteByte(&g_i2c_ch2_ctl, 0x0,RegValue0);
	IRIVER_IIC_WriteByte(&g_i2c_ch2_ctl, 0x1,RegValue1);
}

int IRIVER_GPS_Port_Test(void)
{
	int i;
	int data;
	int iRet;
	int datacount = 0;
	DECLARE_GLOBAL_DATA_PTR;	

	display_text(0,COLOR_WHITE,"GPS port Test ====> ");
	udelay(50000);
	gd->baudrate = 9600;
	serial_setbrg ();
	udelay(50000);

	GPS_LDO_control(1);

	mx100_mdelay(100);

	GPIO_SetFunctionEach(U_GPIO_GPS_DEBUG_SELECT_OUT, eGPO);		

	GPIO_SetDataEach(U_GPIO_GPS_DEBUG_SELECT_OUT, GPIO_LEVEL_HIGH);

	GPIO_SetFunctionEach(U_GPIO_GPS_PWR_ENABLE_OUT, eGPO);		
	GPIO_SetPullUpDownEach(U_GPIO_GPS_PWR_ENABLE_OUT ,eGPUDdis);
	GPIO_SetDataEach(U_GPIO_GPS_PWR_ENABLE_OUT, GPIO_LEVEL_HIGH);
	mx100_mdelay(2000);

	iRet = 1;
	
	for(i=0;i<1000;i++) {
		data = -1;
		if (serial_tstc_gps()) {
			data = serial_getc_gps();	
	//		display_text(1,COLOR_BLUE,"%d",data);
		//	printf("%d",data);
			datacount++;
		}

		if(data==-1) {
			iRet = 0;
		//	break;
		} else {
		//	printf("%d",data);
		}
	}
	
	GPIO_SetDataEach(U_GPIO_GPS_PWR_ENABLE_OUT, GPIO_LEVEL_LOW);

	GPIO_SetDataEach(U_GPIO_GPS_DEBUG_SELECT_OUT, GPIO_LEVEL_LOW);

	//GPS_LDO_control(0);

//	if(iRet==1) {		
		display_text(1,COLOR_BLUE,"OK %d\n",datacount);
		return 1;
//	} else {
	//	display_text(1,COLOR_RED,"NG\n");
//		return 0;
//	}
//	return 1;
}


int IRIVER_SmartReset_Test(void)
{
	int i;
	int data;
	int iRet;
	
	display_text(0,COLOR_WHITE,"SmartReset Test ====> ");

#if (MX100_HW_VER >= MX100_WS)
	GPIO_SetFunctionEach(U_GPIO_POWER_KEY_IN, eGPO);	
	GPIO_SetFunctionEach(U_GPIO_VOLPLUS_KEY_IN, eGPO);	
	GPIO_SetPullUpDownEach(U_GPIO_POWER_KEY_IN,eGPUDdis);
	GPIO_SetPullUpDownEach(U_GPIO_VOLPLUS_KEY_IN,eGPUDdis);

	GPIO_SetDataEach(U_GPIO_POWER_KEY_IN,1);
	GPIO_SetDataEach(U_GPIO_VOLPLUS_KEY_IN,0);

	mx100_mdelay(1000 * 12);

//	GPIO_SetFunctionEach(U_GPIO_POWER_KEY_IN, eGPI);	
//	GPIO_SetFunctionEach(U_GPIO_VOLPLUS_KEY_IN, eGPI);	


#endif

	if(iRet==1) {		
		display_text(1,COLOR_BLUE,"OK\n");
		return 1;
	} else {
		display_text(1,COLOR_RED,"NG\n");
		return 0;
	}
	return 1;
}

int IRIVER_Wifi_Test(void)
{
	int i;
	int data;
	int iRet;
	
	display_text(0,COLOR_WHITE,"Wifi Test ====> ");


	if(iRet==1) {		
		display_text(1,COLOR_BLUE,"OK\n");
		return 1;
	} else {
		display_text(1,COLOR_RED,"NG\n");
		return 0;
	}
	return 1;
}

int IRIVER_BT_Test(void)
{
	int i;
	int data;
	int iRet;
	
	display_text(0,COLOR_WHITE,"BT Test ====> ");


	if(iRet==1) {		
		display_text(1,COLOR_BLUE,"OK\n");
		return 1;
	} else {
		display_text(1,COLOR_RED,"NG\n");
		return 0;
	}
	return 1;
}

int IRIVER_DMB_Test(void)
{
	int i;
	int data;
	int iRet;
	
	display_text(0,COLOR_WHITE,"DMB Test ====> ");


	if(iRet==1) {		
		display_text(1,COLOR_BLUE,"OK\n");
		return 1;
	} else {
		display_text(1,COLOR_RED,"NG\n");
		return 0;
	}
	return 1;
}


char *intToBinary(int width,int i) 
{
  static char s1[64 + 1] = { '0', };
  int count = width;

  do { s1[--count] = '0' + (char) (i & 1);
       i = i >> 1;
  } while (count);

  return s1;
}	

void Camera_LDO_control(int onoff)
{
	#define MAX8698_I2C_CH (2)
	#define MAX8698_ID_WR	(0x66)	
	int iRet,RegValue0,RegValue1;

	IRIVER_IIC_SlaveInit(&g_i2c_ch2_ctl,MAX8698_I2C_CH,MAX8698_ID_WR,MAX8698_ID_WR);

	RegValue0 = IRIVER_IIC_ReadByte(&g_i2c_ch2_ctl, 0x0);
	printf("PMIC REG0 : %s\n",intToBinary(8,RegValue0));

	RegValue1 = IRIVER_IIC_ReadByte(&g_i2c_ch2_ctl, 0x1);
	printf("PMIC REG1 : %s\n",intToBinary(8,RegValue1));   

	if(onoff==1) {
		RegValue0 |= 0b00100000;  // LDO5 vddcam_28
		RegValue1 |= 0b01000000;  // LDO7 vddcam
	} else {
		RegValue0 &= ~0b00100000;  // LDO5 vddcam_28
		RegValue1 &= ~0b01000000;  // LDO7 vddcam
	}
	IRIVER_IIC_WriteByte(&g_i2c_ch2_ctl, 0x0,RegValue0);
	IRIVER_IIC_WriteByte(&g_i2c_ch2_ctl, 0x1,RegValue1);
}

int mt9p111_sensor_power(int on)
{
    static int mt9p111_power_status = false;

	#define		GPIO_CAM_STANDBY        	eGPIO_H3, eGPIO_3
	#define		GPIO_CAM_RESET        		eGPIO_H0, eGPIO_2


	int err;

	/* Camera A */

	GPIO_SetPullUpDownEach(GPIO_CAM_STANDBY,eGPUDdis);

	/* Camera A */
	GPIO_SetPullUpDownEach(GPIO_CAM_RESET,eGPUDdis);

	GPIO_SetFunctionEach(GPIO_CAM_RESET, eGPO);	
	GPIO_SetFunctionEach(GPIO_CAM_STANDBY, eGPO);	

	if (on)
	{
		GPIO_SetDataEach(GPIO_CAM_RESET,0);
	
		Camera_LDO_control(on);

//		clk_enable(clk);
		mx100_mdelay(20);
		
		GPIO_SetDataEach(GPIO_CAM_STANDBY,0);
		mx100_mdelay(10);

		GPIO_SetDataEach(GPIO_CAM_RESET,1);

		mx100_mdelay(10);
	}
	else
	{
		GPIO_SetDataEach(GPIO_CAM_RESET,0);

//		gpio_direction_output(reset, 0);
		mx100_mdelay(10);
		GPIO_SetDataEach(GPIO_CAM_STANDBY,1);
		
//		gpio_direction_output(stanby, 1);
		mx100_mdelay(5);

		Camera_LDO_control(on);

	//	clk_disable(clk);
		//mx100_mdelay(5);
	}

	if(!on)
	{
		mx100_mdelay(50);
	}
	
	return 0;
}

int IRIVER_Camera_Test(void)
{
	int i;
	int data;
	int iRet;
	
	display_text(0,COLOR_WHITE,"Camera Test ====> ");
	#if 1
	#define MT9P111_ID_W  (0x7a>>1)

	#define		GPIO_CAM_SDA        	eGPIO_J3, eGPIO_4
	#define		GPIO_CAM_SCL	    	eGPIO_J3, eGPIO_5

	IRIVER_GPIO_IIC_SlaveInit(&g_gpio_i2c_slave_ctl,
						GPIO_CAM_SDA,
						GPIO_CAM_SCL,
						MT9P111_ID_W,
						MT9P111_ID_W);


	#define MT9M113_ID_W  (0x78>>1)

	IRIVER_GPIO_IIC_SlaveInit(&g_gpio_i2c_slave_ctl,
						GPIO_CAM_SDA,
						GPIO_CAM_SCL,
						MT9M113_ID_W,
						MT9M113_ID_W);

	mt9p111_sensor_power(1);

	if(iRet==1) {		
		display_text(1,COLOR_BLUE,"OK\n");
		return 1;
	} else {
		display_text(1,COLOR_RED,"NG\n");
		return 0;
	}
	#endif
	
	return 1;
}

#define ERROR_DELAY (1000 *1000 * 5)
#define TEST_DELAY (1000 * 500)

#define MAX_PERI_DEVICE (8)

void mx100_peri_test(void)
{
	int iRet= 0,iTotal=0;

	set_text_display_pos(10,50);

	display_text(1,COLOR_WHITE,"starting Peri Test...");

	iRet += IRIVER_WM8993_Test();
	iTotal++;
	
	udelay(TEST_DELAY);

	iRet += IRIVER_FuelGage_Test();
	iTotal++;

	udelay(TEST_DELAY);

	iRet += IRIVER_Touch_Test();	
	iTotal++;
	udelay(TEST_DELAY);

	iRet += IRIVER_BMA150_Test();
	iTotal++;

	udelay(TEST_DELAY);

	iRet += IRIVER_SI4709_Test();
	iTotal++;

	udelay(TEST_DELAY);

	iRet += IRIVER_AKM8975_Test();
	iTotal++;

	udelay(TEST_DELAY);

	iRet += IRIVER_ISL29023_ALS_IR_Test();
	iTotal++;

	udelay(TEST_DELAY);

	iRet +=IRIVER_ISA1200_HapticTest();
	iTotal++;

//	iRet +=IRIVER_GPS_Port_Test();  // under working.
//	iTotal++;

//	iRet +=IRIVER_Camera_Test();  // under working.
//	iTotal++;

	iRet +=IRIVER_AAT1271a_flash_Test();
	iTotal++;
	
	display_text(1,COLOR_WHITE," ");
	

	if(iRet==iTotal) {
		display_text(0,COLOR_BLUE,"mx100 Peri Test Result OK (%d)/Total(%d) ",iRet,iTotal);
	} else {
		display_text(0,COLOR_RED,"mx100 Peri Test Fail OK(%d)/Total(%d) ",iRet,iTotal);

		udelay(ERROR_DELAY);
	}

}
