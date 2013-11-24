#include "_gpio_i2c.h"

#ifndef __MX100_SYS_H__
#define __MX100_SYS_H__


//
// Simple Memory Allocater
//


void* ir_malloc(int size);
void ir_free(void* pmem);

void sprint_size (char *buffer,unsigned long size);


#if (MX100_HW_VER >= MX100_WS)
enum _KEY{
		KEY_NONE = 0,
		KEY_VOLUMEUP,
		KEY_POWER,
		KEY_VOLUMEDOWN,				
};
#else
enum _KEY{
		KEY_NONE = 0,
		KEY_BACK,		
		KEY_MENU,
		KEY_HOME,				
		KEY_VOLUMEUP,
		KEY_POWER,
		KEY_VOLUMEDOWN,		
		KEY_CAMERA
		
};
#endif

void	mx100_keypad_init(void);
int is_pressed_hw_reset(void);

int mx100_get_key(void);
int mx100_get_key_async(void);

int mx100_detect_alert_key(int detect_time,int key1,int key2);

int mx100_keypad_scan(void);


//
//  usb, adapter check function.
//

int is_usb_connected(void);
int is_adaptor_connected(void);

//
// max17040 .
// 

//#define BATT_GAUGE_TEST  //JHL: use only test !!!!
//#define LCD_BRIGHT_TUNE  /* 2011.05.13  JHLIM  use only test */

// referenced from the kernel max17040 driver.

#define USE_CUSTOM_MODEL        /* ktj, use max17040 customized model */
#undef  USE_LEVEL_VCELL         /* ktj, only for test, calculate level by vcell, not define level by soc value */ 

#ifdef BATT_GAUGE_TEST
#define USE_LEVEL_VCELL
#endif


#ifdef USE_LEVEL_VCELL
#define MAX_VCELL   4180000     /* can be reached at 4184000 */  
#define LOW_VCELL   3450000
#endif

#ifdef USE_LEVEL_VCELL /* ktj fixed, min value is too high */
    #define MIN_BOOT_SOC  (2)
#else
//  #define MIN_BOOT_SOC  (1)
    #define MIN_BOOT_SOC  (2)
#endif

int max17040_get_vcell(struct gpio_i2c_slave_ctl *pi2c_ch_ctlint);
int max17040_get_soc(struct gpio_i2c_slave_ctl *pi2c_ch_ctlint);
 int max17040_get_version(struct gpio_i2c_slave_ctl *pi2c_ch_ctlint);

int charge_routine(void);

int mx100_boot_from(void);


enum _MX100_HW_TYPE{
		MX100_UNKNOWN =0,
		MX100_ES1,
		MX100_ES2,		
		MX100_TP1,
		MX100_LPP,		/*JHLIM 2011.04.20  */		
};

int get_mx100_hw_type(void);

int	ADCTS_Init(void);

void mx100_mdelay(unsigned long msec);
int get_cur_soc(void);



#endif 
