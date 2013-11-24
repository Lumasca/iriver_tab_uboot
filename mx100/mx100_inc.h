/********************************************************************
* mx100 uboot header.
*
 *********************************************************************/
#ifndef __MX100_INC_H__
#define __MX100_INC_H__

#include <config.h>
#include <common.h>

#include "_gfx.h"
#include "_i2c.h"
#include "_sys.h"
#include "_gpio.h"
#include "gpio-mx100.h"
#include "_main.h"
#include "_gpio_i2c.h"
#include "_tools.h"
#include "_timer.h"

#include <common.h>
#include <asm/io.h>
#include <asm/types.h>

#define Inp32(_addr)		readl(_addr)
#define Outp32(addr, data)	(*(volatile u32 *)(addr) = (data))
#define Delay(_a)		udelay(_a*1000)

#if defined(CONFIG_S5PC100)
 #include <s5pc100.h>
#elif defined(CONFIG_S5PC110)
 #include <s5pc110.h>
#else
#error "* CFG_ERROR : you have to select C100 or C110 for Android Fastboot"
#endif
//#define ENABLE_DEBUG

#ifdef ENABLE_DEBUG
#define MX100_DBG(args...) do {  printf("[%s:%s:%d]:\n",__FILE__, __FUNCTION__,__LINE__);   printf(args);} while (0)
#else
#define MX100_DBG(args...) 
#endif

#endif

