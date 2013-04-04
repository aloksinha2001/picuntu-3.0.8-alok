/* drivers/input/touchscreen/gt800_801.h
 * 
 * 2010 - 2012 Goodix Technology.
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be a reference 
 * to you, when you are integrating the GOODiX's CTP IC into your system, 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
 * General Public License for more details.
 * 
 * Version:1.2
 *          V1.0:2012/05/01,create file.
 *          V1.2:2012/06/08,add some macro define.
 */

#ifndef _LINUX_GOODIX_TOUCH_H
#define	_LINUX_GOODIX_TOUCH_H

#include <linux/kernel.h>
#include <linux/hrtimer.h>
#include <linux/i2c.h>
#include <linux/input.h>
#include <linux/module.h>
#include <linux/delay.h>
#include <linux/i2c.h>
#include <linux/proc_fs.h>
#include <linux/string.h>
#include <asm/uaccess.h>
#include <linux/vmalloc.h>
#include <linux/interrupt.h>
#include <linux/io.h>
#include <mach/gpio.h>
#include <linux/earlysuspend.h>


extern s32 gtp_update_proc(void*);
extern struct i2c_client * i2c_connect_client;

extern u16 show_len;
extern u16 total_len;
extern s32 gup_update_proc(void* data);

struct goodix_ts_data {
  spinlock_t irq_lock;
  struct i2c_client *client;
  struct input_dev  *input_dev;
  struct hrtimer timer;
  struct work_struct  work;
  struct early_suspend early_suspend;
  s32 irq_is_disable;
  s32 use_irq;
  u16 abs_x_max;
  u16 abs_y_max;
  u8  max_touch_num;
  u8  int_trigger_type;
  u8  chip_type;
  u8  enter_update;
  u8  version[40];
};

//***************************PART1:ON/OFF define*******************************
#define GTP_DEBUG_ON            0
#define GTP_DEBUG_ARRAY_ON      0
#define GTP_DEBUG_FUNC_ON       0
#define GTP_CUSTOM_CFG          0
#define GTP_DRIVER_SEND_CFG     0
#define GTP_HAVE_TOUCH_KEY      0
#define GTP_CHANGE_X2Y          1
#define GTP_POWER_CTRL_SLEEP    0
#define GTP_ICS_SLOT_REPORT     1
#define GTP_CREATE_WR_NODE      0

//***************************PART2:TODO define**********************************
//STEP_1(REQUIRED):Change config table.
/*TODO: puts the config info corresponded to your TP here, the following is just 
a sample config, send this config should cause the chip cannot work normally*/
#define CTP_CFG_GROUP1 {\
    GTP_REG_CONFIG_DATA,\
    0x0F,0x05,0x06,0x28,0x02,0x14,0x14,0x10,0x28,0xB2,0x01,0xe0,0x03,0x20,0xED,0xCB,\
    0xA9,0x87,0x65,0x43,0x21,0x00,0x00,0x00,0x00,0x00,0x4D,0xC1,0x20,0x01,0x01,0x41,\
    0x64,0x3C,0x1E,0x28,0x0E,0x00,0x00,0x00,0x00,0x50,0x3C,0x32,0x71,0x00,0x00,0x00,\
    0x00,0x00,0x00,0x00,0x01\
    }
//TODO puts your group2 config info here,if need.
#define CTP_CFG_GROUP2 {\
    }
//TODO puts your group3 config info here,if need.
#define CTP_CFG_GROUP3 {\
    }

//STEP_2(REQUIRED):Change I/O define & I/O operation mode.
#define GTP_RST_PORT    RK30_PIN1_PC0
#define GTP_PWR_PORT    INVALID_GPIO
#define GTP_INT_PORT    RK30_PIN4_PC2
#define GTP_INT_IRQ     gpio_to_irq(GTP_INT_PORT)
//#define GTP_INT_CFG     S3C_GPIO_SFN(0xF)

#define GTP_GPIO_AS_INPUT(pin)          gpio_direction_input(pin)
#define GTP_GPIO_AS_INT(pin)            do{\
                                            gpio_direction_input(pin);\
                                        }while(0)
#define GTP_GPIO_GET_VALUE(pin)         gpio_get_value(pin)
#define GTP_GPIO_OUTPUT(pin,level)      gpio_direction_output(pin,level)
#define GTP_GPIO_REQUEST(pin, label)    gpio_request(pin, label)
#define GTP_GPIO_FREE(pin)              gpio_free(pin)
#define GTP_IRQ_TAB                     {IRQ_TYPE_EDGE_RISING,IRQ_TYPE_EDGE_FALLING}

//STEP_3(optional):Custom set some config by themself,if need.
#if GTP_CUSTOM_CFG
    #define GTP_MAX_HEIGHT   800
    #define GTP_MAX_WIDTH    480
    #define GTP_MAX_TOUCH    5
    #define GTP_INT_TRIGGER  1
#else
    #define GTP_MAX_HEIGHT   4096
    #define GTP_MAX_WIDTH    4096			
    #define GTP_MAX_TOUCH    5
    #define GTP_INT_TRIGGER  1
#endif

//STEP_4(optional):If this project have touch key,Set touch key config.                                    
#if GTP_HAVE_TOUCH_KEY
#define GTP_KEY_TAB {KEY_MENU, KEY_HOME, KEY_SEND}
#endif

//***************************PART3:OTHER define*********************************
#define GTP_DRIVER_VERSION    "V1.2<2012/06/08>"
#define GTP_I2C_NAME          "Goodix-TS"
#define GTP_POLL_TIME		      10
#define GTP_ADDR_LENGTH       1
#define GTP_CONFIG_LENGTH     58
#define FAIL                  0
#define SUCCESS               1
//Register define
#define GTP_REG_SENSOR_ID     0x00
#define GTP_REG_MOUDLE_SWITCH 0x39
#define GTP_REG_SLEEP         0x50
#define GTP_REG_VERSION       0x6A
#define GTP_REG_CONFIG_DATA   0x30
//Log define
#define GTP_INFO(fmt,arg...)           printk("<<-GTP-INFO->>[%d]"fmt"\n", __LINE__, ##arg)
#define GTP_ERROR(fmt,arg...)          printk("<<-GTP-ERROR->>[%d]"fmt"\n", __LINE__, ##arg)
#define GTP_DEBUG(fmt,arg...)          do{\
                                         if(GTP_DEBUG_ON)\
                                         printk("<<-GTP-DEBUG->>[%d]"fmt"\n",__LINE__, ##arg);\
                                       }while(0)
#define GTP_DEBUG_ARRAY(array, num)    do{\
                                         s32 i;\
                                         u8* a = array;\
                                         if(GTP_DEBUG_ARRAY_ON)\
                                         printk("<<-GTP-DEBUG-ARRAY->>\n");\
                                         for (i = 0; i < (num); i++)\
                                         {\
                                           if(GTP_DEBUG_ARRAY_ON)\
                                           printk("%02x   ", (a)[i]);\
                                           if ((i + 1 ) %10 == 0)\
                                           {\
                                             if(GTP_DEBUG_ARRAY_ON)\
                                             printk("\n");\
                                           }\
                                         }\
                                         if(GTP_DEBUG_ARRAY_ON)\
                                         printk("\n");\
                                       }while(0)
#define GTP_DEBUG_FUNC()               do{\
                                         if(GTP_DEBUG_FUNC_ON)\
                                         printk("<<-GTP-FUNC->>[%d]Func:%s\n", __LINE__, __func__);\
                                       }while(0)
#define GTP_SWAP(x, y)                 do{\
                                         typeof(x) z = x;\
                                         x = y;\
                                         y = z;\
                                       }while (0)

//*****************************End of Part III********************************


#endif /* _LINUX_GOODIX_TOUCH_H */
