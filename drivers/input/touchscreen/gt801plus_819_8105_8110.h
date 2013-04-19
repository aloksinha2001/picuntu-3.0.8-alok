/* drivers/input/touchscreen/gt801plus_819_8105_8110.h
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
 *        V1.0:2012/05/01,Create file.
 *        V1.2:2012/06/08,add some macro.
 *
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

//extern u16 show_len;
//extern u16 total_len;
//extern s32 gup_update_proc(void*);
//extern struct i2c_client * i2c_connect_client;

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
  u8  green_wake_mode;
  u8  chip_type;
  u8  enter_update;
  u8  version[17];
};

//extern s32 gtp_read_version(struct goodix_ts_data *ts);
//extern void gtp_wakeup_green(struct goodix_ts_data *ts,u8 disable);
//extern void gtp_irq_enable(struct goodix_ts_data *ts);
//void gtp_irq_disable(struct goodix_ts_data *ts);

//***************************PART1:ON/OFF define*******************************
#define GTP_DEBUG_ON          0
#define GTP_DEBUG_ARRAY_ON    0
#define GTP_DEBUG_FUNC_ON     0
#define GTP_CUSTOM_CFG        0
#define GTP_DRIVER_SEND_CFG   0
#define GTP_HAVE_TOUCH_KEY    0
#define GTP_CHANGE_X2Y        0
#define GTP_ICS_SLOT_REPORT   1
#define GTP_CREATE_WR_NODE    0

//***************************PART2:TODO define**********************************
//STEP_1(REQUIRED):Change config table.
/*TODO: puts the config info corresponded to your TP here, the following is just 
a sample config, send this config should cause the chip cannot work normally*/
#define CTP_CFG_GROUP1 {\
        0x65,0x03,0x04,0x00,0x03,0x00,0x0A,0x22,0x1E,0xE7,0x32,0x05,0x08,0x10,0x4C,0x41,\
        0x41,0x20,0x00,0x09,0x60,0x60,0x32,0x46,0x0E,0x0D,0x0C,0x0B,0x0A,0x09,0x08,0x07,\
        0x06,0x05,0x04,0x03,0x02,0x01,0x00,0x1D,0x1C,0x1B,0x1A,0x19,0x18,0x17,0x16,0x15,\
        0x14,0x13,0x12,0x11,0x10,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
        0x00,0x00,0x00,0x00,0x00,0x00\
  }
//TODO puts your group2 config info here,if need.
#define CTP_CFG_GROUP2 {\
  }
//TODO puts your group3 config info here,if need.
#define CTP_CFG_GROUP3 {\
  }
//TODO puts your group4 config info here,if need.
#define CTP_CFG_GROUP4 {\
  }

//STEP_2(REQUIRED):Change I/O define & I/O operation mode#define GTP_RST_PORT  S5PV210_GPH1(3)//S3C64XX_GPL(10)
#define GTP_RST_PORT  RK30_PIN4_PD0//S5PV210_GPH1(3)
#define GTP_INT_PORT  RK30_PIN4_PC2
#define GTP_INT_IRQ   gpio_to_irq(GTP_INT_PORT)
#define GTP_INT_CFG   //S3C_GPIO_SFN(2)

#define GTP_GPIO_AS_INPUT(pin)          do{\
                                            gpio_direction_input(pin);\
                                            gpio_pull_updown(pin, GPIO_LOW);\
                                        }while(0)
#define GTP_GPIO_AS_INT(pin)            do{\
                                            gpio_direction_input(pin);\
                                            gpio_pull_updown(pin, GPIO_LOW);\
                                        }while(0)
#define GTP_GPIO_GET_VALUE(pin)         gpio_get_value(pin)
#define GTP_GPIO_OUTPUT(pin,level)      gpio_direction_output(pin,level)
#define GTP_GPIO_REQUEST(pin, label)    gpio_request(pin, label)
#define GTP_GPIO_FREE(pin)              gpio_free(pin)
#define GTP_GPIO_READ(pin)              gpio_get_value(pin)
#define GTP_IRQ_TAB                     {IRQ_TYPE_EDGE_RISING,IRQ_TYPE_EDGE_FALLING,\
                                        IRQ_TYPE_LEVEL_LOW,IRQ_TYPE_LEVEL_HIGH}

//STEP_3(optional):Custom set some config by themself,if need.
#if GTP_CUSTOM_CFG
  #define GTP_MAX_WIDTH     800			
  #define GTP_MAX_HEIGHT    480
  #define GTP_MAX_TOUCH     5
  #define GTP_INT_TRIGGER   1
#else
  #define GTP_MAX_WIDTH     4096			
  #define GTP_MAX_HEIGHT    4096
  #define GTP_MAX_TOUCH     10
  #define GTP_INT_TRIGGER   1
#endif

//STEP_4(optional):if this project have touch key,Set touch key config.                                    
#if GTP_HAVE_TOUCH_KEY
    #define GTP_READ_COOR_ADDR 0x00
    #define GTP_KEY_TAB {KEY_MENU, KEY_HOME, KEY_SEND}
#else
    #define GTP_READ_COOR_ADDR 0x01
#endif

//***************************PART3:OTHER define*********************************
#define GTP_DRIVER_VERSION    "V1.2<2012/06/08>"
#define GTP_I2C_NAME          "GT801PLUS-TS"
#define GTP_I2C_CLK           400*1000
#define GTP_POLL_TIME	      10
#define GTP_ADDR_LENGTH       1
#define FAIL                  0
#define SUCCESS               1
#define GT801PLUS             1
#define GT81XPLUS             0
//Register define
#define GTP_REG_SLEEP         0x50
#define GTP_REG_SENSOR_ID     0x52
#define GTP_REG_XMAX_H        0x66
#define GTP_REG_I2CDELAY      0x6E
#define GTP_REG_VERSION       0xF0
//Log define
#define GTP_INFO(fmt,arg...)           printk("<<-GTP-INFO->>[%d]"fmt"\n", __LINE__, ##arg)
#define GTP_ERROR(fmt,arg...)          printk("<<-GTP-ERROR->>[%d]"fmt"\n", __LINE__, ##arg)
#define GTP_DEBUG(fmt,arg...)          do{\
                                         if(GTP_DEBUG_ON)\
                                         printk("<<-GTP-DEBUG->> [%d]"fmt"\n",__LINE__, ##arg);\
                                       }while(0)
#define GTP_DEBUG_ARRAY(array, num)    do{\
                                         s32 i;\
                                         u8* a = array;\
                                         if(GTP_DEBUG_ARRAY_ON)\
                                         {\
                                            printk("<<-GTP-DEBUG-ARRAY->>\n");\
                                            for (i = 0; i < (num); i++)\
                                            {\
                                                printk("%02x   ", (a)[i]);\
                                                if ((i + 1 ) %10 == 0)\
                                                {\
                                                    printk("\n");\
                                                }\
                                            }\
                                            printk("\n");\
                                        }\
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

//****************************PART4:UPDATE define*******************************
#define PACK_SIZE               64
#define MAX_TIMEOUT             60000
#define MAX_I2C_RETRIES         7000

//I2C buf address
#define ADDR_CMD                80
#define ADDR_STA                81
#define ADDR_DAT                0
#define ADDR_DAT_OLD            82

//Moudle state
#define NEW_UPDATE_START        0x01
#define UPDATE_START            0x02
#define SLAVE_READY             0x08
#define UNKNOWN_ERROR           0x00
#define FRAME_ERROR             0x10
#define CHECKSUM_ERROR          0x20
#define TRANSLATE_ERROR         0x40
#define FLASH_ERROR             0x80

//Error no
#define ERROR_NO_FILE           2   //ENOENT
#define ERROR_FILE_READ         23  //ENFILE
#define ERROR_FILE_TYPE         21  //EISDIR
#define ERROR_GPIO_REQUEST      4   //EINTR
#define ERROR_I2C_TRANSFER      5   //EIO
#define ERROR_NO_RESPONSE       16  //EBUSY
#define ERROR_TIMEOUT           110 //ETIMEDOUT

//*****************************End of Part III********************************


#endif /* _LINUX_GOODIX_TOUCH_H */
