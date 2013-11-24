#include "mx100_inc.h"
#include "mx100_bitmap.h"
#if defined(CONFIG_S5PC100)
 #include <s5pc100.h>
#elif defined(CONFIG_S5PC110)
 #include <s5pc110.h>
#else
#error "* CFG_ERROR : you have to select C100 or C110 for Android Fastboot"
#endif

//---------------------------------------------------------------------------//
#include <common.h>
#include <asm/io.h>
#include <asm/types.h>


#define S5P_PSHOLD_CONTROL 0xE010E81C



//
// Simple Memory Allocater
//

unsigned int g_mem_pool_start = MX100_MEMPOOL_BUFFER;
unsigned int g_mem_pool_cur_pos = 0;
unsigned int g_mem_pool_max_size = MX100_MEMPOOL_BUFFER_SIZE;

#if defined(CHARGE_ADAPTOR_1A) /* ktj */
extern int uboot_usbctl_init(void);
extern int fboot_usb_stop (void);
#endif


void* ir_malloc(int size)
{
	int new_size;
	int alloc_addr;

	alloc_addr = g_mem_pool_start+g_mem_pool_cur_pos;
	
	new_size = size << 2; // 4byte align;
	new_size++;
	new_size >>= 2;
	
	g_mem_pool_cur_pos += new_size;

//	printf("AllocBuffer : %x \n",alloc_addr);
	
	return (void*) (alloc_addr);
}

void ir_free(void* pmem)
{
//NA
}


/*
 * print sizes as "xxx kB", "xxx.y kB", "xxx MB", "xxx.y MB",
 * xxx GB, or xxx.y GB as needed; allow for optional trailing string
 * (like "\n")
 */
void sprint_size (char *buffer,unsigned long size)
{
	int offset = 0;
	
	ulong m = 0, n;
	phys_size_t d = 1 << 30;		/* 1 GB */
	char  c = 'G';

	if (size < d) {			/* try MB */
		c = 'M';
		d = 1 << 20;
		if (size < d) {		/* print in kB */
			c = 'k';
			d = 1 << 10;
		}
	}

	n = size / d;

	/* If there's a remainder, deal with it */
	if(size % d) {
		m = (10 * (size - (n * d)) + (d / 2) ) / d;

		if (m >= 10) {
			m -= 10;
			n += 1;
		}
	}

	offset += sprintf (buffer,"%2ld", n);
	if (m) {
		sprintf (&buffer[offset],".%ld", m);
	}
	offset += sprintf (&buffer[offset]," %cB", c);
}


//
// KEY routine
//

#if (MX100_HW_VER >= MX100_WS)
int MX100_Keycode[] = {
		KEY_POWER,
		KEY_VOLUMEUP,
		KEY_VOLUMEDOWN,		
};
#else
int MX100_Keycode[] = {
		KEY_BACK,		
		KEY_MENU,
		KEY_HOME,				
		KEY_VOLUMEUP,
//		KEY_POWER,
		KEY_VOLUMEDOWN,		
		KEY_CAMERA
};
#endif

int g_is_hw_reset_key_pressed = 0;

int is_pressed_hw_reset(void)
{
	return g_is_hw_reset_key_pressed;
}
extern int interrupt_init(void);

void	mx100_keypad_init(void)
{
int i,max_count;
//mx100evm1 keypad port.
//GPH2[5] : HOME
//GPH2[6] : MENU
//GPH2[7] :BACK
//GPH3[0] : VOL+
//GPH3[4] : VOL-
//

#if (MX100_HW_VER >= MX100_WS)

	GPIO_SetFunctionEach(U_GPIO_POWER_KEY_IN, eGPI);	
	GPIO_SetPullUpDownEach(U_GPIO_POWER_KEY_IN,eGPUen);

	GPIO_SetFunctionEach(U_GPIO_VOLPLUS_KEY_IN, eGPI);	
	GPIO_SetPullUpDownEach(U_GPIO_VOLPLUS_KEY_IN,eGPUen);

	GPIO_SetFunctionEach(U_GPIO_VOLMINUS_KEY_IN, eGPI);	
	GPIO_SetPullUpDownEach(U_GPIO_VOLMINUS_KEY_IN ,eGPUen);

#else
	GPIO_SetFunctionEach(U_GPIO_BACK_KEY_IN, eGPI);	
	GPIO_SetPullUpDownEach(U_GPIO_BACK_KEY_IN ,eGPUen);

	GPIO_SetFunctionEach(U_GPIO_MENU_KEY_IN, eGPI);	
	GPIO_SetPullUpDownEach(U_GPIO_MENU_KEY_IN,eGPUen);

	GPIO_SetFunctionEach(U_GPIO_HOME_KEY_IN, eGPI);	
	GPIO_SetPullUpDownEach(U_GPIO_HOME_KEY_IN ,eGPUen);

 	GPIO_SetFunctionEach(U_GPIO_VOLPLUS_KEY_IN, eGPI);	
	GPIO_SetPullUpDownEach(U_GPIO_VOLPLUS_KEY_IN,eGPUen);

	GPIO_SetFunctionEach(U_GPIO_VOLMINUS_KEY_IN, eGPI);	
	GPIO_SetPullUpDownEach(U_GPIO_VOLMINUS_KEY_IN ,eGPUen);

	GPIO_SetFunctionEach(U_GPIO_CAM_HALF_SHUTTOR_KEY_IN, eGPI);	
	GPIO_SetPullUpDownEach(U_GPIO_CAM_HALF_SHUTTOR_KEY_IN ,eGPUen);
#endif
	#if 1
	interrupt_init();
	udelay(1000);

	if(GPIO_GetDataEach(U_GPIO_VOLPLUS_KEY_IN) ==0) {
		g_is_hw_reset_key_pressed = 1;
	}
	printf("\nHW reset:%d\n",g_is_hw_reset_key_pressed);
	#endif
	
	#if 0

	for(i=0;i<10;i++) {
//		g_is_hw_reset_key_pressed =1;
		printf("%d %d\n",GPIO_GetDataEach(U_GPIO_POWER_KEY_IN),GPIO_GetDataEach(U_GPIO_VOLPLUS_KEY_IN));
		udelay(30 * 1000);
		
	}
	#endif	
}


static int	mx100_keypad_get_data(void)
{
	int		key_data = 0,count=0;

#if (MX100_HW_VER >= MX100_WS)
	key_data |= GPIO_GetDataEach(U_GPIO_POWER_KEY_IN)	 <<  count++;  // POWER
	key_data |= GPIO_GetDataEach(U_GPIO_VOLPLUS_KEY_IN) <<  count++;  // VOL+
	key_data |= GPIO_GetDataEach(U_GPIO_VOLMINUS_KEY_IN) <<  count++;  // VOL-
#else

	key_data |= GPIO_GetDataEach(U_GPIO_BACK_KEY_IN) << count++;   // BACK
	key_data |= GPIO_GetDataEach(U_GPIO_MENU_KEY_IN) <<  count++ ;  // MENU
	key_data |= GPIO_GetDataEach(U_GPIO_HOME_KEY_IN) <<  count++;  // HOME
	key_data |= GPIO_GetDataEach(U_GPIO_VOLPLUS_KEY_IN) <<  count++;  // VOL+
//	key_data |= GPIO_GetDataEach(U_GPIO_POWER_KEY_IN)	 <<  count++;  // POWER
	key_data |= GPIO_GetDataEach(U_GPIO_VOLMINUS_KEY_IN) <<  count++;  // VOL-
	key_data |= GPIO_GetDataEach(U_GPIO_CAM_HALF_SHUTTOR_KEY_IN)	 <<  count++;  // CAMERA
#endif

	key_data = ~key_data;

	return	key_data;
}




//[*]--------------------------------------------------------------------------------------------------[*]
static int	generate_keycode(unsigned short prev_key, unsigned short cur_key, int *key_table)
{
	unsigned short 	press_key, release_key, i;
	
	press_key	= (cur_key ^ prev_key) & cur_key;
	release_key	= (cur_key ^ prev_key) & prev_key;
	
	i = 0;
	while(press_key)	{
		if(press_key & 0x01)	{

			#if 0
			if(mx100_keypad.hold_state)	{
				
				if(key_table[i] == KEY_POWER)	{
				//		input_report_key(mx100_keypad.driver, key_table[i], KEY_PRESS);					
				}
				else	{	
				//	input_report_switch(mx100_keypad.driver, SW_KEY_HOLD, ON);
				}
				
				
			}
			else	
			#endif
			
			{
				#ifdef ENABLE_CROSSKEY_ON_CAMERA_KEY
				if(key_table[i] == KEY_CAMERA) {
					
					if(g_cross_key_mode==1) g_cross_key_mode = 0;
					else g_cross_key_mode = 	1;
					
					if(g_cross_key_mode == 1) {
						printk("\nCross Key Mode\n");
					} else {
						printk("\nNormal Key Mode\n");
					}
					
				} else {
					if(g_cross_key_mode  == 1) {
						input_report_key(mx100_keypad.driver, MX100_Keycode_Shift[i], KEY_PRESS);
						#if defined(DEBUG_MSG)
						printk("PRESS KEY :%d %d %s",i,MX100_Keycode_Shift[i], (char *)MX100_KeyMapStr_Shift[i ]);
						#endif

					} else {
						input_report_key(mx100_keypad.driver, key_table[i], KEY_PRESS);
						#if defined(DEBUG_MSG)
						printk("PRESS KEY :%d %d %s",i, key_table[i], (char *)MX100_KeyMapStr[i ]);							
						#endif
					}
				}
				#else
				
				//input_report_key(mx100_keypad.driver, key_table[i], KEY_PRESS);
				
				return key_table[i];

				#if defined(DEBUG_MSG)
				printk("PRESS KEY :%d %d %s",i, key_table[i], (char *)MX100_KeyMapStr[i ]);							
				#endif
				
				#endif
			
			}
			
			// POWER OFF PRESS
		//	if(key_table[i] == KEY_POWER)	mx100_poweroff_timer_run();
			
		}
		i++;	press_key >>= 1;
	}

	return KEY_NONE;
	
	#if 0
	i = 0;
	while(release_key)	{
		if(release_key & 0x01)	{

			#if 0
			if(mx100_keypad.hold_state)	{
//				if(key_table[i] == KEY_POWER)	input_report_key(mx100_keypad.driver, key_table[i], KEY_RELEASE);
	//			else							input_report_switch(mx100_keypad.driver, SW_KEY_HOLD, OFF);
			}
			else	
			#endif
			{
				#ifdef ENABLE_CROSSKEY_ON_CAMERA_KEY
				if(g_cross_key_mode==1) {
					input_report_key(mx100_keypad.driver, MX100_Keycode_Shift[i], KEY_RELEASE);
					#if defined(DEBUG_MSG)
					printk("RELEASE KEY :%d %d %s",i,MX100_Keycode_Shift[i], (char *)MX100_KeyMapStr_Shift[i ]);
					#endif
					
				} else {
					input_report_key(mx100_keypad.driver, key_table[i], KEY_RELEASE);
					#if defined(DEBUG_MSG)
					printk("RELEASE KEY :%d %d %s",i, key_table[i], (char *)MX100_KeyMapStr[i ]);
					#endif

				}

				#else
				//input_report_key(mx100_keypad.driver, key_table[i], KEY_RELEASE);
				#if defined(DEBUG_MSG)
				printk("RELEASE KEY :%d %d %s",i, key_table[i], (char *)MX100_KeyMapStr[i ]);
				#endif

				#endif
			}

			// POWER OFF (LONG PRESS)	
			//if(key_table[i] == KEY_POWER)		del_timer_sync(&mx100_keypad.poweroff_timer);
			
		}
		i++;	release_key >>= 1;
	}
	#endif
}

int mx100_keypad_scan(void)
{
	static	unsigned short	prev_keypad_data = 0, cur_keypad_data = 0;
	int keycode;
	
	// key data process
	cur_keypad_data = mx100_keypad_get_data();
	if(prev_keypad_data ==0 ) prev_keypad_data = cur_keypad_data;

	if(prev_keypad_data != cur_keypad_data)	{


		keycode =  generate_keycode(prev_keypad_data, cur_keypad_data, &MX100_Keycode[0]);

		prev_keypad_data = cur_keypad_data;

		return keycode;
	}

	return KEY_NONE;
}

#define SCAN_INTERVAL (1000 *30)

int mx100_get_key(void)
{
	int key;
	while(1) {
		key = mx100_keypad_scan();
		if(key != KEY_NONE) return key;
		udelay(SCAN_INTERVAL);
	}
}

int mx100_get_key_async(void)
{
	int key;
	key = mx100_keypad_scan();
	udelay(SCAN_INTERVAL);

	return key;
}



int mx100_detect_alert_key(int detect_time,int key1,int key2)
{
	int key,i,max_count;

	max_count = (detect_time * 1000) /SCAN_INTERVAL ;
	i = 0;

	//printf("if you enter mx100 system mode, press VOL - KEY\n");

	//display_text(0,COLOR_WHITE,"SYSTEM MODE :VOL -");

	for(i=0;i<max_count;i++) {
		key = mx100_keypad_scan();
		if(key != KEY_NONE) {
			if(key1 == key) return 1;
		}
		udelay(SCAN_INTERVAL);
	}

	return 0;
}

//
//  usb, adapter check function.
//

int is_usb_connected(void)
{
	int is_usb_connected = !GPIO_GetDataEach(U_GPIO_USB_INSERT_IN );

//	printf("usb status : %d\n",is_usb_connected);

	return is_usb_connected;
}

int is_adaptor_connected(void)
{
	int is_adaptor_connected = !GPIO_GetDataEach(U_GPIO_DC_APT_DETECT_IN);
//	printf("adaptor status : %d\n",is_adaptor_connected);
	
	return is_adaptor_connected;
}


//
// battery check.
//
#define MAX17040_VCELL_MSB	0x02
#define MAX17040_VCELL_LSB	0x03
#define MAX17040_SOC_MSB	0x04
#define MAX17040_SOC_LSB	0x05
#define MAX17040_MODE_MSB	0x06
#define MAX17040_MODE_LSB	0x07
#define MAX17040_VER_MSB	0x08
#define MAX17040_VER_LSB	0x09
#define MAX17040_RCOMP_MSB	0x0C
#define MAX17040_RCOMP_LSB	0x0D
#define MAX17040_CMD_MSB	0xFE
#define MAX17040_CMD_LSB	0xFF

#ifdef USE_CUSTOM_MODEL /* ktj */
int init_fuel_gauge(struct gpio_i2c_slave_ctl *pi2c_ch_ctlint);
#endif

void max17040_reset(struct gpio_i2c_slave_ctl *pi2c_ch_ctlint)
{
	IRIVER_GPIO_IIC_WriteByte(pi2c_ch_ctlint, MAX17040_CMD_MSB, 0x54);
	IRIVER_GPIO_IIC_WriteByte(pi2c_ch_ctlint, MAX17040_CMD_LSB, 0x00);
}

int max17040_get_vcell(struct gpio_i2c_slave_ctl *pi2c_ch_ctlint)
{
	unsigned char msb;
	unsigned char lsb;
	int vcell;
	
	msb = IRIVER_GPIO_IIC_ReadByte(pi2c_ch_ctlint, MAX17040_VCELL_MSB);
	lsb = IRIVER_GPIO_IIC_ReadByte(pi2c_ch_ctlint, MAX17040_VCELL_LSB);

	vcell = (msb << 4) + (lsb >> 4);
	vcell = (vcell * 125) * 10;	
	return vcell;
}

static int g_cur_soc = 1;
int get_cur_soc(void)
{
	return g_cur_soc;
}
int max17040_get_soc(struct gpio_i2c_slave_ctl *pi2c_ch_ctlint)
{
	int soc;
	int vcell;
	unsigned char SOC1;
	unsigned char SOC2;
	
	SOC1 = IRIVER_GPIO_IIC_ReadByte(pi2c_ch_ctlint, MAX17040_SOC_MSB);
	SOC2 = IRIVER_GPIO_IIC_ReadByte(pi2c_ch_ctlint, MAX17040_SOC_LSB);

#if 0//#ifdef USE_CUSTOM_MODEL /* ktj */
/*  soc = ((SOC1 * 256) + SOC2) * 1953125 / 1000000000; */

    soc = ((SOC1 * 256) + SOC2) * 19531 / 10000000;
    
    if (soc > 100)
        soc = 100;
    else if (soc < 0)
        soc = 0; 
#else
    soc = SOC1;
#endif

	vcell = max17040_get_vcell(pi2c_ch_ctlint);

#ifdef USE_LEVEL_VCELL /* ktj */

    if (vcell <= LOW_VCELL)
        soc = 0;
    else if (vcell >= MAX_VCELL)
        soc = 100;
    else
    {
        soc = (vcell - LOW_VCELL) * 100 / (MAX_VCELL - LOW_VCELL);
        if (soc == 0 && vcell > LOW_VCELL)
            soc = 1;    
    }
#endif	

    printf("[MAX17040] vcell = %d soc = %d SOC1 = %d SOC2 = %d \n\n", vcell, soc, SOC1, SOC2); /* ktj */
	g_cur_soc = soc;
	return soc;
}

void max17040_get_soc1_soc2(struct gpio_i2c_slave_ctl *pi2c_ch_ctlint, unsigned char *soc1, unsigned char *soc2)
{
	*soc1 = IRIVER_GPIO_IIC_ReadByte(pi2c_ch_ctlint, MAX17040_SOC_MSB);
	*soc2 = IRIVER_GPIO_IIC_ReadByte(pi2c_ch_ctlint, MAX17040_SOC_LSB);
}

 int max17040_get_version(struct gpio_i2c_slave_ctl *pi2c_ch_ctlint)
{
	unsigned char msb;
	unsigned char lsb;

	msb = IRIVER_GPIO_IIC_ReadByte(pi2c_ch_ctlint, MAX17040_VER_MSB);
	lsb = IRIVER_GPIO_IIC_ReadByte(pi2c_ch_ctlint, MAX17040_VER_LSB);

	return (msb<<8) | lsb;
	
}


#define SCAN_INTERVAL (1000 *30)

#ifdef ENABLE_CHARGE_ROUTINE /* ktj, fix build error on evm */


int g_charge_power_down = 0;

int is_lcd_on(void);

//
//insert power save code below function.
//
void charge_power_down(void)
{
	LCD_turnoff();
	g_charge_power_down = 1;
}

//
//
//
void charge_power_up(void)
{
	LCD_turnon();
	g_charge_power_down = 0;
}

//
//
//
int is_charge_power_down(void)
{
	return g_charge_power_down;
}

//-------------------------------------------------------------------------------//

#ifndef mdelay
#define mdelay(x)       udelay(1000*x)
#endif

static struct gpio_i2c_slave_ctl g_gpio_i2c_slave_ctl;

void power_off(void);

void display_lowbatt_poweroff_popup( void)
{
    gfx_draw_bitmap(g_surface,
              567, //int iLeft,
             109, //int iTop,
             (unsigned char*) pImg_TURN_OFF_02_KOR,
              0, //int iSourceLeft,
              0, //int iSourceTop,
               0,//int iSourceCopydWidth,
              0,// int iSourceCopydHeight,
              GFX_ROP_COPY //int iRop
               );	

}
void clear_lowbatt_poweroff_popup( void)
{
	gfx_draw_fill_rect(g_surface,567,109,GFX_BM_WIDTH(pImg_TURN_OFF_02_KOR),GFX_BM_HEIGHT(pImg_TURN_OFF_02_KOR),GFX_RGB32(0x0,0x0,0x0));
}

void display_unpluged_poweroff_popup( void)
{
    gfx_draw_bitmap(g_surface,
              567, //int iLeft,
             109, //int iTop,
             (unsigned char*) pImg_TURN_OFF_03_KOR,
              0, //int iSourceLeft,
              0, //int iSourceTop,
               0,//int iSourceCopydWidth,
              0,// int iSourceCopydHeight,
              GFX_ROP_COPY //int iRop
               );	

}

static int g_ani_count = 0;

unsigned char* g_ani_images[] ={
	pImg_LOOP_01,
	pImg_LOOP_02,
	pImg_LOOP_03,
	pImg_LOOP_04,
	pImg_LOOP_05		
};
void display_charge_ani( void)
{
    unsigned char* ani_image = 0;

    if(g_ani_count < 0) g_ani_count = 0;
	
    if(g_ani_count > 4) g_ani_count = 0;
  
   ani_image = g_ani_images[g_ani_count];
   	
    if(ani_image)  {
	    gfx_draw_bitmap(g_surface,
	              616, //int iLeft,
	             267, //int iTop,
	             (unsigned char*) ani_image,
	              0, //int iSourceLeft,
	              0, //int iSourceTop,
	               0,//int iSourceCopydWidth,
	              0,// int iSourceCopydHeight,
	              GFX_ROP_COPY //int iRop
	               );	
    }

   g_ani_count ++;

}


#define LOW_BAT_SOC_OFF           0     /* SOC1 */
#define LOW_BAT_SOC2_POPUP      128     /* SOC2 */

void check_low_battery(void)
{
	int vcell, i;
    unsigned char soc1, soc2;


    for (i=0; i<5; i++)
    {
	    vcell = max17040_get_vcell(&g_gpio_i2c_slave_ctl);

	    max17040_get_soc1_soc2(&g_gpio_i2c_slave_ctl, &soc1, &soc2);
	    g_cur_soc = soc1;

		printf("CHK_LOWBAT : vcell = %d soc1 = %d soc2 = %d \n", vcell, soc1, soc2);

        mdelay(1);
    }

    LCD_turnon(); 

    if (is_adaptor_connected())
    {
        /* charging */        
    }
    else
    {
#if 1
//      if (soc1 <= 0 || (soc1 == 1 && soc2 < 128))
        if (soc1 < 2)
        {
            printf("=== Power-off by low battery ===\n");
            
            if (soc1 < 0 ||  (soc1 == 0 && soc2 < 128))
            {
            	display_lowbatt_poweroff_popup();

             	LCD_turnon();
            
		 mx100_mdelay(2000);  /* 2s delay */      
            	power_off();
            }
            else
            {
            	display_lowbatt_poweroff_popup();
            
               	LCD_turnon();
            
		 mx100_mdelay(2000);  /* 2s delay */
            
	            power_off();
            }
        }
#else
        if (soc1 <= LOW_BAT_SOC_OFF)
        {
        	printf("=== Power-off by low battery ===\n");

            if (soc2 >= LOW_BAT_SOC2_POPUP)
            {
        		display_lowbatt_poweroff_popup();

               	LCD_turnon();

		        mx100_mdelay(2000);  /* 2s delay */

	    	    power_off();
            }
            else
            {
            	power_off();
            }
        }
#endif
    }
}


#define MAX17040_I2C_CH (2)
#define MAX17040_ID_W (0x6c)
#define MAX17040_ID_R (0x6d)

int check_routine(void)
{
	IRIVER_GPIO_IIC_SlaveInit(&g_gpio_i2c_slave_ctl,
						U_GPIO_FUEL_GAUGE_SDA_IO,
						U_GPIO_FUEL_GAUGE_SCL_IO,
						MAX17040_ID_W >>1,
						MAX17040_ID_R>>1);

#ifdef USE_CUSTOM_MODEL
    init_fuel_gauge(&g_gpio_i2c_slave_ctl);
#endif

    check_low_battery();
}

static int gCharging = 0;

#if defined(CHANGE_SYS_CLK) /* ktj */
extern void setclock_1st(void);
extern void setclock_2nd(void);
extern void PMIC_dvsarm(void);

void clock_change(void)
{
//  printf("\nsetclock_1st \n");

    PMIC_dvsarm();
    mdelay(10);

    setclock_1st();
    mdelay(10);

    setclock_2nd();
    mdelay(10);

//  printf("\nsetclock_2nd \n");
}
#endif

#define CHARGE_TIME_MS(X)  (( (X )* 1000)/ (SCAN_INTERVAL) )

#ifdef BATT_GAUGE_TEST
#define BATTERY_DISPLAY_TIME  (CHARGE_TIME_MS(60000))   // 6000ms
#else
#define BATTERY_DISPLAY_TIME  (CHARGE_TIME_MS(6000))   // 8000ms  2011.05.23 : req from cto.
#endif


#define BATTERY_DISPLAY_INTERVAL  (CHARGE_TIME_MS(1500))   // 2000ms

void draw_string1(char *str)
{
	unsigned int ColorSet[3] = { GFX_RGB32(0xff,0xff,0xff),GFX_RGB32(0xdd,0xdd,0xdd),GFX_RGB32(0x0,0x0,0x0) };

	gfx_draw_string(
			g_surface,
			(void*)str,
	              10,
	              10,
	              TF_SHADE | TF_TRANSPARENT,
	              0,
	              ColorSet	);
}
int charge_routine(void)
{
	
//	static struct gpio_i2c_slave_ctl g_gpio_i2c_slave_ctl;

	int vcell;
	int soc;
	int i;
	int key;
	int pgs_init = 0;
	int lcd_turnon = 1;
	unsigned int tmp = 0;
	int usbconnected = 0;
	struct ts_gfx_progressbar l_pgsbar;

	#ifdef ENABLE_IMAGE_CHARGE_UI
	gfx_init_progressbar(&l_pgsbar,
						WIN_X1,360,
						WIN_Y1,150,
						WIN_WIDTH,GFX_BM_WIDTH(pImg_BATTERY_BG),
						WIN_HEIGHT,GFX_BM_HEIGHT(pImg_BATTERY_BG),
						PGS_STYLE,PGS_CHARGE,
						PGS_START,0,
						PGS_END,100,
						PGS_STEP,5,
						WIN_TITLE,"test");


	#else	
	gfx_init_progressbar(&l_pgsbar,
						WIN_X1,340,
						WIN_Y1,300,
						WIN_WIDTH,300,
						WIN_HEIGHT,50,
						PGS_STYLE,PGS_NORMAL,						
						PGS_START,0,
						PGS_END,100,
						PGS_STEP,1,
						WIN_TITLE,"test");
	#endif
	
	vcell=max17040_get_vcell(&g_gpio_i2c_slave_ctl);
	soc=max17040_get_soc(&g_gpio_i2c_slave_ctl);

	charge_power_up();

#if defined(CHARGE_ADAPTOR_1A) /* ktj */
    if(is_usb_connected() || is_adaptor_connected()) 
	{
        uboot_usbctl_init();
	}
#endif


	i = 0;
	//min 2436
	// max	3874
	while(1) 
	{
		key = mx100_keypad_scan();

		udelay(SCAN_INTERVAL);

		if(is_usb_connected() || is_adaptor_connected() ) 
		{  
			gCharging = 1;
			if(usbconnected==0) {
				pgs_init = 0;
				i=0;
				usbconnected = 1;
			}

			if(key == KEY_POWER ) 
			{
				i = 0;
				
				if(soc < MIN_BOOT_SOC) 
				{
					charge_power_up();
					display_lowbatt_poweroff_popup();
					mx100_mdelay(2000);
					pgs_init = 0;
					printf("soc is too low , so boot is not enable.\n");
				} 
				else 
				{
					if(is_charge_power_down()) 
					{
						charge_power_up();
					} 
					else  
					{
						goto boot;
					}
				}
			} 
			else 
			{
				#ifdef LCD_BRIGHT_TUNE
				int pwm_duty = get_PWM_duty();
				char temp[32];
				if(key == KEY_VOLUMEUP)  {
				    if(pwm_duty > 0) {
				    	PWM_UserSelect(--pwm_duty); 
					
					sprintf(temp,"pwm_duty : %2d\n",pwm_duty);
					draw_string1(temp);
				    }
				} 
				else if(key == KEY_VOLUMEDOWN) {
				    if(pwm_duty < 100) {
				    	PWM_UserSelect(++pwm_duty); 
					sprintf(temp,"pwm_duty : %2d\n",pwm_duty);
					draw_string1(temp);

				    }
				}

				#else
			
				if(i == BATTERY_DISPLAY_TIME) 
				{
					charge_power_down();
					i=0;
				}
				#endif
			}
		} 
		else 
		{
			if(usbconnected==1) {
				pgs_init = 0;
				i = 0;
				usbconnected = 0;
			}
		
			if(soc < MIN_BOOT_SOC) 
			{
				printf("_battery level is low\n");
				display_lowbatt_poweroff_popup();
				mx100_mdelay(2000);  /* 2s delay */
				power_off(); 
			}  
			else if (gCharging == 1)
			{
			    /* cable disconnected */
				printf("_cable is disconnected\n");
				display_unpluged_poweroff_popup();
				mx100_mdelay(2000);  /* 2s delay */
			    	power_off();	        
		    	}
			else 
			{   
				goto boot;
			}
		}
			

		if(i++% BATTERY_DISPLAY_INTERVAL==0) 
		{
			vcell=max17040_get_vcell(&g_gpio_i2c_slave_ctl);
			soc=max17040_get_soc(&g_gpio_i2c_slave_ctl);

			if(pgs_init ==0) {
				gfx_draw_progressbar(&l_pgsbar,-1);
				gfx_draw_progressbar(&l_pgsbar,0);
				clear_lowbatt_poweroff_popup();
				pgs_init = 1;
			}
			
			if(is_lcd_on()) 
			{			
				if(i < BATTERY_DISPLAY_INTERVAL) {

				} else {
					gfx_draw_progressbar(&l_pgsbar,soc);
				}
				display_charge_ani();
			}
		}
	}

	boot:

	#ifdef ENABLE_IMAGE_CHARGE_UI
	gfx_draw_fill_rect(g_surface,360,150,GFX_BM_WIDTH(pImg_BATTERY_BG)+100,GFX_BM_HEIGHT(pImg_BATTERY_BG),GFX_RGB32(0x0,0x0,0x0));
	#else		
	gfx_draw_fill_rect(g_surface,340,300,300,50,GFX_RGB32(0x0,0x0,0x0));
	#endif
#if defined(CHARGE_ADAPTOR_1A) /* ktj */
    if(is_usb_connected() || is_adaptor_connected()) 
        fboot_usb_stop();   
#endif

#if defined(CHANGE_SYS_CLK) /* ktj */
    clock_change(); 
#endif

	return 0;
}
#endif


#ifdef USE_CUSTOM_MODEL /* ktj */

#define MDOEL_SIZE  17  /* bytes */

unsigned char model0[] = {
0x40,
0xA6,
0xC0,
0xAF,
0x60,
0xB1,
0xC0,
0xB3,
0x10,
0xB4,
0x70,
0xB5,
0x40,
0xB5,
0x90,
0xB5,
0xD0    
};

unsigned char model1[] = {
0x50,
0xB6,
0x60,
0xB6,
0x90,
0xB9,
0x60,
0xBA,
0x90,
0xBD,
0xF0,
0xC3,
0x40,
0xC8,
0x40,
0xD1,
0x20
};

unsigned char model2[] = {
0x60,
0x01,
0xF0,
0x0C,
0xF0,
0x01,
0xF0,
0x15,
0xE0,
0x15,
0xA0,
0x4A,
0xE0,
0x69,
0x30,
0x06,
0x90
};

unsigned char model3[] = {
0x70,
0x47,
0xC0,
0x06,
0x10,
0x20,
0x10,
0x0B,
0xB0,
0x08,
0xF0,
0x07,
0xF0,
0x06,
0x00,
0x06,
0x00,
};

/* zsani2: fuel gauage table : voltage, gradient, intercept */
struct _with_charging_soc_table 
{
	int vol;
	int a;
	int b;
};

// with charger
struct _with_charging_soc_table tbl[8] =
{
	{4193000, 14500, 2748700},
	{4140000, 11000, 3086100},
	{3940000,  8500, 3278700},
	{3780000,  4100, 3535100},
	{3750000,  7100, 3381600},
	{3690000,  2400, 3586300},
	{3625000,  5000, 3544100},
	{3605000, 11900, 3460300},
};

// without charger
struct _with_charging_soc_table tbl_a[10] =
{
	{4100000, 21500, 2012100},
	{4050000, 12200, 2916900},
	{3840000,  9900, 3130600},
	{3690000,  7500, 3299800},
	{3640000,  4700, 3448400},
	{3620000,  4000, 3476000},
	{3560000,  2400, 3534300},
	{3545000,  8000, 3473100},
	{3435000, 15100, 3409700},
	{3400000, 54100, 3344500},

};

int check_quick_start()
{
	int i = 0, cal_soc, soc, vcell;
    unsigned char fdata[5] = {0x00,};
    int soc_large = 0;
    int soc_diff;

    printf("[MAX17040] Check Quick Start \n");

	vcell = max17040_get_vcell(&g_gpio_i2c_slave_ctl);
	soc = max17040_get_soc(&g_gpio_i2c_slave_ctl);

	printf("[MAX17040] VCELL = %d V, soc = %d\n", vcell, soc);

    if (is_adaptor_connected())
    {
	    printf("[MAX17040] With charger \n");
	    
	    if(vcell >= tbl[0].vol) i=0;
	    else if(vcell < tbl[0].vol && vcell >= tbl[1].vol) i=1;
	    else if(vcell < tbl[1].vol && vcell >= tbl[2].vol) i=2;
	    else if(vcell < tbl[2].vol && vcell >= tbl[3].vol) i=3;
	    else if(vcell < tbl[3].vol && vcell >= tbl[4].vol) i=4;
	    else if(vcell < tbl[4].vol && vcell >= tbl[5].vol) i=5;
	    else if(vcell < tbl[5].vol && vcell >= tbl[6].vol) i=6;
	    else if(vcell < tbl[6].vol) i=7;

	    cal_soc = (vcell - tbl[i].b) / tbl[i].a;
    }
    else
    {
	    printf("[MAX17040] Without charger \n");

	    if(vcell >= tbl[0].vol) i=0;
	    else if(vcell < tbl_a[0].vol && vcell >= tbl_a[1].vol) i=1;
	    else if(vcell < tbl_a[1].vol && vcell >= tbl_a[2].vol) i=2;
	    else if(vcell < tbl_a[2].vol && vcell >= tbl_a[3].vol) i=3;
	    else if(vcell < tbl_a[3].vol && vcell >= tbl_a[4].vol) i=4;
	    else if(vcell < tbl_a[4].vol && vcell >= tbl_a[5].vol) i=5;
	    else if(vcell < tbl_a[5].vol && vcell >= tbl_a[6].vol) i=6;
	    else if(vcell < tbl_a[6].vol && vcell >= tbl_a[7].vol) i=7;
	    else if(vcell < tbl_a[7].vol && vcell >= tbl_a[8].vol) i=8;
	    else if(vcell < tbl_a[8].vol) i=9;

	    cal_soc = (vcell - tbl_a[i].b) / tbl_a[i].a;
    }

	/* (calcurated SOC (vcell - b) / a) - SOC > 20 ------> go Quick Start */
	printf("[MAX17040] INDEX[%d] calcurated SOC[%d]%% - SOC[%d]%% = %d%%\n", i, cal_soc, soc, cal_soc - soc);

	//return cal_soc - soc > 20 || cal_soc - soc < -20 ? 1 : 0;
	
//	if(cal_soc - soc > 10 || cal_soc - soc < -10)

    if (soc >= cal_soc)
        soc_diff = soc - cal_soc;
    else
        soc_diff = cal_soc - soc ;
        
    printf("soc_diff = %d \n", soc_diff);
    
    if (soc_diff > 15)  // before 10    	
	{
		// quick_start mode
		printf("[MAX17040] === Quick Start Mode === \n");

		fdata[0] = 0x06;
		fdata[1] = 0x40;
		fdata[2] = 0x00;
		IRIVER_GPIO_IIC_WriteData(&g_gpio_i2c_slave_ctl, fdata, 3);
		
		mdelay(500);
	
		fdata[0] = 0x06;
		fdata[1] = 0x00;
		fdata[2] = 0x00;
		IRIVER_GPIO_IIC_WriteData(&g_gpio_i2c_slave_ctl, fdata, 3);
	}
	else
	{
		// normal mode
		printf("[MAX17040] === Normal Mode === \n");
	}
}

int init_fuel_gauge(struct gpio_i2c_slave_ctl *pi2c_ch_ctlint)
{
    int fdata_size;
    unsigned char fdata[5];
    unsigned char read_reg;
    unsigned char SOC1, SOC2;
    unsigned char OriginalRCOMP1, OriginalRCOMP2;
    unsigned char OriginalOCV1, OriginalOCV2;
    int StartingRCOMP = 200; /* iriver¡¯s RCOMP value at 20¨¬C = 200 */
	unsigned int tmp = 0;


	printf("[MAX17040] -----------------------> USB_SW_OE HIGH \n");
	tmp = readl(GPH3DAT);
	tmp |= 0x40;    
	writel(tmp,GPH3DAT);
	mdelay(5);


    // Unlock Model Access
    printf("[MAX17040] Unlock Model Access \n");
    fdata[0] = 0x3E;
    fdata[1] = 0x4A;
    fdata[2] = 0x57;
    fdata_size = 3;
    IRIVER_GPIO_IIC_WriteData(pi2c_ch_ctlint, fdata, fdata_size);
    
   
    // Read Original RCOMP and OCV Register
    printf("[MAX17040] Read Original RCOMP and OCV Register \n");
    read_reg = 0x0C;
    fdata_size = 4;
    IRIVER_GPIO_IIC_ReadData(pi2c_ch_ctlint, read_reg, fdata, 4);      
    OriginalRCOMP1 = fdata[0];
    OriginalRCOMP2 = fdata[1];
    OriginalOCV1 = fdata[2];
    OriginalOCV2 = fdata[3];
    printf("[MAX17040] OriginalRCOMP1 = %d OriginalRCOMP2 = %d OriginalOCV1 = %d OriginalOCV2 = %d \n", OriginalRCOMP1, OriginalRCOMP2, OriginalOCV1, OriginalOCV2);


    // Compare OriginalRCOMP with StartingRCOMP
    if (OriginalRCOMP1 == StartingRCOMP)
    {
        printf("[MAX17040] Already initialized \n");    
        
        // Lock Model Access
        printf("[MAX17040] Lock Model Access \n");
        fdata[0] = 0x3E;
        fdata[1] = 0x00;
        fdata[2] = 0x00;
        fdata_size = 3;
        IRIVER_GPIO_IIC_WriteData(pi2c_ch_ctlint, fdata, fdata_size);

        goto init_done;
    }    


    // Write OCV
    printf("[MAX17040] Write OCV \n");
    fdata[0] = 0x0E;
    fdata[1] = 0xDB;
    fdata[2] = 0x20;
    fdata_size = 3;
    IRIVER_GPIO_IIC_WriteData(pi2c_ch_ctlint, fdata, fdata_size);


    // Write RCOMP
    printf("[MAX17040] Write RCOMP \n");
    fdata[0] = 0x0C;
    fdata[1] = 0xFF;
    fdata[2] = 0x00;
    fdata_size = 3;
    IRIVER_GPIO_IIC_WriteData(pi2c_ch_ctlint, fdata, fdata_size);
    

    // Write Model
    printf("[MAX17040] Write Model \n");
    IRIVER_GPIO_IIC_WriteData(pi2c_ch_ctlint, model0, MDOEL_SIZE);
    IRIVER_GPIO_IIC_WriteData(pi2c_ch_ctlint, model1, MDOEL_SIZE);
    IRIVER_GPIO_IIC_WriteData(pi2c_ch_ctlint, model2, MDOEL_SIZE);
    IRIVER_GPIO_IIC_WriteData(pi2c_ch_ctlint, model3, MDOEL_SIZE);


    // Delay at least 150ms
    mdelay(200);
    

    // Write OCV
    printf("[MAX17040] Write OCV \n");
    fdata[0] = 0x0E;
    fdata[1] = 0xDB;
    fdata[2] = 0x20;
    fdata_size = 3;
    IRIVER_GPIO_IIC_WriteData(pi2c_ch_ctlint, fdata, fdata_size);


    // Delay between 150ms and 600ms
    mdelay(300);
    

    // Read SOC Register and Compare to expected result
    printf("[MAX17040] Read SOC Register and Compare to expected result \n");
    read_reg = 0x04;
    fdata_size = 2;
    IRIVER_GPIO_IIC_ReadData(pi2c_ch_ctlint, read_reg, fdata, fdata_size);      
    SOC1 = fdata[0];
    SOC2 = fdata[1];
    printf("[MAX17040] SOC1 = %d SOC2 = %d \n", SOC1, SOC2);
    if (SOC1 >= 0x71 && SOC1 < 0x73)
        printf("[MAX17040] Model was loaded successful\n");
    else
        printf("[MAX17040] Model was not loaded successful\n");


    // Restore RCOMP and OCV
    printf("[MAX17040] Restore RCOMP and OCV \n");
    fdata[0] = 0x0C;
    fdata[1] = OriginalRCOMP1;
    fdata[2] = OriginalRCOMP2;
    fdata[3] = OriginalOCV1;
    fdata[4] = OriginalOCV2;
    fdata_size = 5;
    IRIVER_GPIO_IIC_WriteData(pi2c_ch_ctlint, fdata, fdata_size);

    
    // Lock Model Access
    printf("[MAX17040] Lock Model Access \n");
    fdata[0] = 0x3E;
    fdata[1] = 0x00;
    fdata[2] = 0x00;
    fdata_size = 3;
    IRIVER_GPIO_IIC_WriteData(pi2c_ch_ctlint, fdata, fdata_size);


    // Update RCOMP              
    printf("[MAX17040] Update RCOMP  \n");
    fdata[0] = 0x0C;
    fdata[1] = StartingRCOMP;
    fdata[2] = 0x00;
    fdata_size = 3;
    IRIVER_GPIO_IIC_WriteData(pi2c_ch_ctlint, fdata, fdata_size);


    check_quick_start();

	
	mdelay(100);


init_done:
	printf("[MAX17040] -----------------------> USB_SW_OE LOW \n");
	tmp = readl(GPH3DAT);
	tmp &= 0xFFFFFFBF;    
	writel(tmp,GPH3DAT);
}
#endif // USE_CUSTOM_MODEL

void power_off(void)
{
	int retrycount =0 ;

#if 0 // kernel reffrence
	/* PS_HOLD --> Output Low */
	printk(KERN_EMERG "%s : setting GPIO_PDA_PS_HOLD low.\n", __func__);
	/* PS_HOLD output High --> Low  PS_HOLD_CONTROL, R/W, 0xE010_E81C */
	writel(readl(S5P_PSHOLD_CONTROL) & 0xFFFFFEFF, S5P_PSHOLD_CONTROL);

	while(1);

	printk(KERN_EMERG "%s : should not reach here!\n", __func__);
#endif

    if(is_usb_connected() || is_adaptor_connected()) {
   	    printf("power off usb\n");
	    return;	
    }

    printf("power-off\n");

	retry:

    Outp32(0xE010A000,0x8);  

    /* PS_HOLD --> Output Low */

    Outp32(S5P_PSHOLD_CONTROL, Inp32(S5P_PSHOLD_CONTROL) & 0xFFFFFEFF);
    mx100_mdelay(100);

    if(is_usb_connected() || is_adaptor_connected()) {
	    printf("power off usb\n");
	    return;	
    }

    if(retrycount++ < 3) goto retry;  /* JHLIM 2011.05.04  */		

    printf("power off error!!!!!!!!!\n");
	
    while(1);
}

#include <s5pc110.h>

#include <movi.h>
#include <asm/io.h>
#include <regs.h>
#include <mmc.h>

int mx100_boot_from(void)
{
	int reg = (*(volatile u32 *)(INF_REG_BASE + INF_REG3_OFFSET));
	return reg;
}

void mx100_mdelay(unsigned long msec)
{
	int i;

	for(i=0;i<msec;i++) {
		udelay(1000);
	}
}



///////////////////////////////////////////////////
//
// added JHLIM 2011.04.01
// detect mx100 hw type.
//
///////////////////////////////////////////////////

#define	ENABLE					1
#define	DISABLE					0
#define	SELECT					1
#define	DESELECT				0

#define	STYLUS_DOWN			0
#define	STYLUS_UP				1

#define	ADCTS_TOUCH_INIT		2
#define	ADCTS_TOUCH_DOWN	0
#define	ADCTS_TOUCH_UP		1

typedef enum ADCTS_Id
{
	eADCTS_ADCCON = 0,
	eADCTS_ADCTSC = 1,
	eADCTS_ADCDLY = 2,
	eADCTS_ADCDAT0 = 3,
	eADCTS_ADCDAT1 = 4,
	eADCTS_ADCUPDN = 5,
	eADCTS_ADCCLRINT = 6,
	eADCTS_ADCMUX = 7,
	eADCTS_ADCCLRINTPNDNUP = 8,
}
eADCTS_Id;


typedef enum ADCTS_MeasureModeId
{
	eADCTS_MEASURE_NOP = 0,
	eADCTS_MEASURE_MANUAL_X = 1,
	eADCTS_MEASURE_MANUAL_Y = 2,
	eADCTS_MEASURE_AUTO_SEQ = 3,
	eADCTS_MEASURE_FOR_STYLUS = 4,
}
eADCTS_MeasureModeId;

typedef struct ADCTS_Inform
{	
	u32	uADCSampleData;	
	u8	ucADCflag;
	u8	ucTouchStatus;
}
ADCTS_oInform;



#define 	ADCTS			( (volatile oADCTS_REGS *) (g_pADCTSBase))
#define	ADCTS_PRSCVL		10

typedef struct tag_ADCTS_REGS
{
	u32 rADCCON;
	u32 rADCTSC;
	u32 rADCDLY;
	u32 rADCDAT0;
	u32 rADCDAT1;
	u32 rADCUPDN;
	u32 rADCCLRINT;
} 
oADCTS_REGS;

// define global variables
static void *	g_pADCTSBase;
volatile	ADCTS_oInform	g_oADCTSInform;

int	ADCTS_Init(void);
u32 ADCTS_GetXPData(void);
u32 ADCTS_GetRegValue(u8 ucADCTSRegId);
void ADCTS_SetRegValue(u8 ucADCTSRegId, u32 uValue);

int get_mx100_hw_type(void)
{
#define MAX_MEASURE_NUM (10)
#define HW_DETECT_PORT (1)

//
// ES1 3.0v  4095
//ES2 2.8v
// TP1 2.6v   3400

int count,total_adc_value=0,avr_adc =0;
int mx100_hw_type;

	ADC_InitADCCON();

	for(count=0;count <MAX_MEASURE_NUM;count++)
	{
		ADC_SelectADCCh(HW_DETECT_PORT);
		ADC_EnableStart(ENABLE);

		while (! ADC_IsAfterStartup() ) ;

		while (! (ADC_IsEOCFlag()));		

		total_adc_value += ADCTS_GetXPData();

		mdelay(1);
	}

	avr_adc = total_adc_value / MAX_MEASURE_NUM;
	
	if(avr_adc >= 2900 && avr_adc <3200) {
		mx100_hw_type = MX100_LPP;		
	} 
	else if(avr_adc >= 3300 && avr_adc <3500) {
		mx100_hw_type = MX100_TP1;		
	} 
	else if(avr_adc >=3500 && avr_adc < 3700) {
		mx100_hw_type = MX100_ES1;
	}
	else if(avr_adc >=3700 && avr_adc < 3850) {
		mx100_hw_type = MX100_ES2;
	} 
	else if(avr_adc >=4000 && avr_adc < 5000) {
		mx100_hw_type = MX100_ES2;
	} 
	else {
		mx100_hw_type = MX100_UNKNOWN;
	}
	
	printf("adcvalue: %d \n",avr_adc);
	#ifdef MX100_BOOTLOADER_4_MP	
	if(mx100_hw_type != MX100_LPP) {
		printf("mx100 hw detect error: %d \n",avr_adc);
	}
	mx100_hw_type = MX100_LPP;
	#endif
		
	return mx100_hw_type;
}

int	ADCTS_Init(void)
{
	u32 uBaseAddress=0;

	uBaseAddress = ADCTS_BASE;
	
	g_pADCTSBase = (void *)uBaseAddress;

	ADCTS_SetADCClearInt();
	ADCTS_SetADCClearWKInt();
	
	return TRUE;
}


void ADC_InitADCCON(void)
{
	u32			uADCCONValue=0;

	uADCCONValue			= ADCTS_GetRegValue(eADCTS_ADCCON);
	uADCCONValue			= (1<<16)|(1<<14)|(ADCTS_PRSCVL<<6)|(0<<2)|(0<<1);;
	ADCTS_SetRegValue(eADCTS_ADCCON, uADCCONValue);	
}

int ADC_IsEOCFlag(void)
{
	u32			uADCCONValue=0;
	u8			ucEOCFlag=0;

	uADCCONValue			= ADCTS_GetRegValue(eADCTS_ADCCON);
	ucEOCFlag 				= (uADCCONValue & 0x8000) >> 15;

	if ( ucEOCFlag == 1 )
	{
		return true;
	}
	else 
	{
		return false;
	}
}

void ADC_SelectADCCh(u8 ucSel)
{
	u32			uADCMUXValue=0;

	uADCMUXValue			= ADCTS_GetRegValue(eADCTS_ADCMUX);
	uADCMUXValue			= (uADCMUXValue & ~(0xf<<0))|(ucSel<<0);
	ADCTS_SetRegValue(eADCTS_ADCMUX, uADCMUXValue);
}

void ADC_SelectStandbyMode(u8 ucSel)
{
	u32			uADCCONValue=0;

	uADCCONValue			= ADCTS_GetRegValue(eADCTS_ADCCON);
	uADCCONValue			= (uADCCONValue & ~(1<<2))|(ucSel<<2);
	ADCTS_SetRegValue(eADCTS_ADCCON, uADCCONValue);
}

void ADC_EnableReadStart(u8 ucSel)
{
	u32			uADCCONValue=0;

	uADCCONValue			= ADCTS_GetRegValue(eADCTS_ADCCON);
	uADCCONValue			= (uADCCONValue & ~(1<<1))|(ucSel<<1);
	ADCTS_SetRegValue(eADCTS_ADCCON, uADCCONValue);
}

void ADC_EnableStart(u8 ucSel)
{
	u32			uADCCONValue=0;

	uADCCONValue			= ADCTS_GetRegValue(eADCTS_ADCCON);
	uADCCONValue			= (uADCCONValue & ~(1<<0))|(ucSel<<0);
	ADCTS_SetRegValue(eADCTS_ADCCON, uADCCONValue);
}

int ADC_IsAfterStartup(void)
{
	u32			uADCCONValue=0;
	u8			ucEnableStartFlag=0;

	uADCCONValue			= ADCTS_GetRegValue(eADCTS_ADCCON);
	ucEnableStartFlag			= (uADCCONValue & 0x1) ;

	if ( ucEnableStartFlag == 0 )
	{
		return true;
	}
	else 
	{
		return false;
	}
}


u32 ADCTS_GetXPData(void)
{
	u32			uADCDAT0Value=0;

	uADCDAT0Value	= ADCTS_GetRegValue(eADCTS_ADCDAT0);

	return (uADCDAT0Value & 0xfff);
}

u32 ADCTS_GetYPData(void)
{
	u32			uADCDAT1Value=0;

	uADCDAT1Value	= ADCTS_GetRegValue(eADCTS_ADCDAT1);

	return (uADCDAT1Value & 0xfff);
}

void ADCTS_SetADCClearInt(void)
{
	u32			uADCCLRINTValue=0;

	uADCCLRINTValue	= ADCTS_GetRegValue(eADCTS_ADCCLRINT);
	uADCCLRINTValue	|= 0;
	ADCTS_SetRegValue(eADCTS_ADCCLRINT, uADCCLRINTValue);	
}

void ADCTS_SetADCClearWKInt(void)
{
	u32			uADCCLRINTPNDNUPValue=0;

	uADCCLRINTPNDNUPValue	= ADCTS_GetRegValue(eADCTS_ADCCLRINTPNDNUP);
	uADCCLRINTPNDNUPValue	|= 0;
	ADCTS_SetRegValue(eADCTS_ADCCLRINTPNDNUP, uADCCLRINTPNDNUPValue);	
}

u32 ADCTS_GetRegValue(u8 ucADCTSRegId)
{
	volatile u32 *	puADCTSBaseAddr;
	volatile u32 *	puADCTSReg;	

	puADCTSBaseAddr			= &(ADCTS->rADCCON);
	puADCTSReg			= puADCTSBaseAddr + ucADCTSRegId;

	return *puADCTSReg;	
}

void ADCTS_SetRegValue(u8 ucADCTSRegId, u32 uValue)
{
	volatile u32 *	puADCTSBaseAddr;
	volatile u32 *	puADCTSReg;	

	puADCTSBaseAddr		= &(ADCTS->rADCCON);
	puADCTSReg			= puADCTSBaseAddr + ucADCTSRegId;

	*puADCTSReg		= uValue;
}



/* end of file */
