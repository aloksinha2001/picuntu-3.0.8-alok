/* drivers/input/touchscreen/zet6221_i2c.c
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * ZEITEC Semiconductor Co., Ltd
 * Tel: +886-3-579-0045
 * Fax: +886-3-579-9960
 * http://www.zeitecsemi.com
 */
/*
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/input.h>
#include <linux/platform_device.h>
#include <linux/jiffies.h>
#include <linux/io.h>
#include <linux/i2c.h>
#include <linux/gpio.h>   
#include <linux/delay.h>
#include <linux/earlysuspend.h>
*/
#include <linux/input.h>
#include <linux/slab.h>
#include <asm/io.h>
#include <asm/types.h>
#include <asm/uaccess.h>


#include <linux/timer.h>
#include <linux/module.h>
#include <linux/delay.h>
#include <linux/hrtimer.h>
#include <linux/input.h>
#include <linux/i2c.h>
#include <linux/interrupt.h>
#include <asm/uaccess.h>
#include <linux/workqueue.h>

#include <linux/io.h>
#include <linux/ioport.h>
#include <linux/input-polldev.h>
#include <linux/earlysuspend.h>

#include <linux/init.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/poll.h>

#include <linux/delay.h>
#include <linux/input/mt.h>
#include <mach/iomux.h>
#include <mach/gpio.h>
#include <mach/board.h>
#include <linux/wakelock.h>

//fw update.
#include "zet6221_fw.h"

                                         
/* -------------- global variable definition -----------*/
#define _MACH_MSM_TOUCH_H_

#define ZET_TS_ID_NAME "zet6221_ts"          

#define MJ5_TS_NAME "zet6221_touchscreen"
                       
                                
#define TPINFO	1
#define X_MAX	1024
#define Y_MAX	600
#define FINGER_NUMBER 5
#define KEY_NUMBER 0
#define P_MAX	1
#define DEBOUNCE_NUMBER	    5
#define D_POLLING_TIME	25000
#define U_POLLING_TIME	25000
#define S_POLLING_TIME  100
#define REPORT_POLLING_TIME  5

#define MAX_KEY_NUMBER      	8
#define MAX_FINGER_NUMBER	16
#define TRUE 		1
#define FALSE 		0

#define debug_mode 1
#define DPRINTK(fmt,args...)	do { if (debug_mode) printk(KERN_EMERG "[%s][%d] "fmt"\n", __FUNCTION__, __LINE__, ##args);} while(0)

//#define TRANSLATE_ENABLE 1
#define TOPRIGHT 	0
#define TOPLEFT  	1
#define BOTTOMRIGHT	2
#define BOTTOMLEFT	3
#define ORIGIN		BOTTOMRIGHT

struct msm_ts_platform_data {
	unsigned int x_max;
	unsigned int y_max;
	unsigned int pressure_max;
};

struct zet6221_tsdrv {
	struct i2c_client *i2c_ts;
	struct work_struct work1;
	struct input_dev *input;
	struct timer_list polling_timer;
	struct early_suspend early_suspend;
	unsigned int gpio; /* GPIO used for interrupt of TS1*/
	unsigned int irq;
	unsigned int x_max;
	unsigned int y_max;
	unsigned int pressure_max;
};

static u16 polling_time = S_POLLING_TIME;

static int __devinit zet6221_ts_probe(struct i2c_client *client, const struct i2c_device_id *id);
static int __devexit zet6221_ts_remove(struct i2c_client *dev);

extern int  zet6221_downloader(struct i2c_client * client);
extern u8 zet6221_ts_version();
extern u8 zet6221_ts_get_report_mode_t(struct i2c_client *client);

static int filterCount = 0; 
static u32 filterX[MAX_FINGER_NUMBER][2], filterY[MAX_FINGER_NUMBER][2]; 

static u8  key_menu_pressed = 0x1;
static u8  key_back_pressed = 0x1;
static u8  key_search_pressed = 0x1;

static u16 ResolutionX=X_MAX;
static u16 ResolutionY=Y_MAX;
static u16 FingerNum=0;
static u16 KeyNum=0;
static int bufLength=0;
static u8 inChargerMode=0;
static u8 xyExchange=0;
static int f_up_cnt=0;
int probe_finished =0;
static int resetCount = 0;
static struct i2c_client *this_client;

u8 pc[8];
// {IC Model, FW Version, FW version,Codebase Type=0x08, Customer ID, Project ID, Config Board No, Config Serial No}

//Touch Screen
static const struct i2c_device_id zet6221_ts_idtable[] = {
       { ZET_TS_ID_NAME, 0 },
       { }
};

static struct i2c_driver zet6221_ts_driver = {
	.driver = {
		.owner = THIS_MODULE,
		.name  = ZET_TS_ID_NAME,
	},
	.probe	  = zet6221_ts_probe,
	.remove		= __devexit_p(zet6221_ts_remove),
	.id_table = zet6221_ts_idtable,
};


/***********************************************************************
    [function]: 
		        callback: Timer Function if there is no interrupt fuction;
    [parameters]:
			    arg[in]:  arguments;
    [return]:
			    NULL;
************************************************************************/

static void polling_timer_func(unsigned long arg)
{
	struct zet6221_tsdrv *ts = (struct zet6221_tsdrv *)arg;
	schedule_work(&ts->work1);
	mod_timer(&ts->polling_timer,jiffies + msecs_to_jiffies(polling_time));
}

/***********************************************************************
    [function]: 
		        callback: read data by i2c interface;
    [parameters]:
			    client[in]:  struct i2c_client â€?represent an I2C slave device;
			    data [out]:  data buffer to read;
			    length[in]:  data length to read;
    [return]:
			    Returns negative errno, else the number of messages executed;
************************************************************************/
s32 zet6221_i2c_read_tsdata(struct i2c_client *client, u8 *data, u8 length)
{
	struct i2c_msg msg;
	msg.addr = client->addr;
	msg.flags = I2C_M_RD;
	msg.len = length;
	msg.buf = data;
	msg.scl_rate= 300*1000;
	return i2c_transfer(client->adapter,&msg, 1);
}

/***********************************************************************
    [function]: 
		        callback: write data by i2c interface;
    [parameters]:
			    client[in]:  struct i2c_client â€?represent an I2C slave device;
			    data [out]:  data buffer to write;
			    length[in]:  data length to write;
    [return]:
			    Returns negative errno, else the number of messages executed;
************************************************************************/
s32 zet6221_i2c_write_tsdata(struct i2c_client *client, u8 *data, u8 length)
{
	struct i2c_msg msg;
	msg.addr = client->addr;
	msg.flags = 0;
	msg.len = length;
	msg.buf = data;
	msg.scl_rate= 300*1000;
	return i2c_transfer(client->adapter,&msg, 1);
}

/***********************************************************************
    [function]: 
		        callback: coordinate traslating;
    [parameters]:
			    px[out]:  value of X axis;
			    py[out]:  value of Y axis;
				p [in]:   pressed of released status of fingers;
    [return]:
			    NULL;
************************************************************************/
void touch_coordinate_traslating(u32 *px, u32 *py, u8 p)
{
	int i;
	u8 pressure;

	#if ORIGIN == TOPRIGHT
	for(i=0;i<MAX_FINGER_NUMBER;i++){
		pressure = (p >> (MAX_FINGER_NUMBER-i-1)) & 0x1;
		if(pressure)
		{
			px[i] = X_MAX - px[i];
		}
	}
	#elif ORIGIN == BOTTOMRIGHT
	for(i=0;i<MAX_FINGER_NUMBER;i++){
		pressure = (p >> (MAX_FINGER_NUMBER-i-1)) & 0x1;
		if(pressure)
		{
			px[i] = X_MAX - px[i];
			py[i] = Y_MAX - py[i];
		}
	}
	#elif ORIGIN == BOTTOMLEFT
	for(i=0;i<MAX_FINGER_NUMBER;i++){
		pressure = (p >> (MAX_FINGER_NUMBER-i-1)) & 0x1;
		if(pressure)
		{
			py[i] = Y_MAX - py[i];
		}
	}
	#endif
}

/***********************************************************************
    [function]: 
		        callback: reset function;
    [parameters]:
			    void;
    [return]:
			    void;
************************************************************************/
void ctp_reset(void)
{
#if defined(TS_RST_GPIO)
	//reset mcu
    gpio_direction_output(TS_RST_GPIO, 1);
	msleep(1);
    gpio_direction_output(TS_RST_GPIO, 0);
	msleep(10);
	gpio_direction_output(TS_RST_GPIO, 1);
	msleep(20);
#else
	u8 ts_reset_cmd[1] = {0xb0};
	zet6221_i2c_write_tsdata(this_client, ts_reset_cmd, 1);
#endif
}

/***********************************************************************
    [function]: 
		        callback: read finger information from TP;
    [parameters]:
    			client[in]:  struct i2c_client â€?represent an I2C slave device;
			    x[out]:  values of X axis;
			    y[out]:  values of Y axis;
			    z[out]:  values of Z axis;
				pr[out]:  pressed of released status of fingers;
				ky[out]:  pressed of released status of keys;
    [return]:
			    Packet ID;
************************************************************************/
u8 zet6221_ts_get_xy_from_panel(struct i2c_client *client, u32 *x, u32 *y, u32 *z, u32 *pr, u32 *ky)
{
	u8  ts_data[70];
	int ret;
	int i;
	
	memset(ts_data,0,70);

	ret=zet6221_i2c_read_tsdata(client, ts_data, bufLength);
	
	*pr = ts_data[1];
	*pr = (*pr << 8) | ts_data[2];
		
	for(i=0;i<FingerNum;i++)
	{
		x[i]=(u8)((ts_data[3+4*i])>>4)*256 + (u8)ts_data[(3+4*i)+1];
		y[i]=(u8)((ts_data[3+4*i]) & 0x0f)*256 + (u8)ts_data[(3+4*i)+2];
		z[i]=(u8)((ts_data[(3+4*i)+3]) & 0x0f);
	}
		
	//if key enable
	if(KeyNum > 0)
		*ky = ts_data[3+4*FingerNum];

	return ts_data[0];
}

/***********************************************************************
    [function]: 
		        callback: get dynamic report information;
    [parameters]:
    			client[in]:  struct i2c_client â€?represent an I2C slave device;

    [return]:
			    1;
************************************************************************/
u8 zet6221_ts_get_report_mode(struct i2c_client *client)
{
	u8 ts_report_cmd[1] = {0xb2};
	u8 ts_reset_cmd[1] = {0xb0};
	u8 ts_in_data[17] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	
	int ret;
	int i;
	int count=0;

	ret=zet6221_i2c_write_tsdata(client, ts_report_cmd, 1);

	if (ret > 0)
	{
		while(1)
		{
			msleep(1);

			if (gpio_get_value(TS_INT_GPIO) == 0)
			{
				DPRINTK( "int low\n");
				ret=zet6221_i2c_read_tsdata(client, ts_in_data, 17);
				
				if(ret > 0)
				{
				
					for(i=0;i<8;i++)
					{
						pc[i]=ts_in_data[i] & 0xff;
					}				
					
					if(pc[3] != 0x08)
					{
						printk ("=============== zet6221_ts_get_report_mode report error ===============\n");
						return 0;
					}

					xyExchange = (ts_in_data[16] & 0x8) >> 3;
					if(xyExchange == 1)
					{
						ResolutionY= ts_in_data[9] & 0xff;
						ResolutionY= (ResolutionY << 8)|(ts_in_data[8] & 0xff);
						ResolutionX= ts_in_data[11] & 0xff;
						ResolutionX= (ResolutionX << 8) | (ts_in_data[10] & 0xff);
					}
					else
					{
						ResolutionX = ts_in_data[9] & 0xff;
						ResolutionX = (ResolutionX << 8)|(ts_in_data[8] & 0xff);
						ResolutionY = ts_in_data[11] & 0xff;
						ResolutionY = (ResolutionY << 8) | (ts_in_data[10] & 0xff);
					}
					
					FingerNum = (ts_in_data[15] & 0x7f);
					KeyNum = (ts_in_data[15] & 0x80);

					if(KeyNum==0)
						bufLength  = 3+4*FingerNum;
					else
						bufLength  = 3+4*FingerNum+1;

					//DPRINTK( "bufLength=%d\n",bufLength);
				
					break;
				
				}else
				{
					printk ("=============== zet6221_ts_get_report_mode read error ===============\n");
					return 0;
				}
				
			}else
			{
				//DPRINTK( "int high\n");
				if(count++ > 2000)
				{
					printk ("=============== zet6221_ts_get_report_mode time out ===============\n");
					return 0;
				}
				
			}
		}

	}else
	{
		printk ("=============== zet6221_ts_get_report_mode WRITE ERROR ===============\n");
		return 0;
	}
	return 1;
}

/***********************************************************************
    [function]: 
		        callback: get dynamic report information with timer delay;
    [parameters]:
    			client[in]:  struct i2c_client represent an I2C slave device;

    [return]:
			    1;
************************************************************************/

u8 zet6221_ts_get_report_mode_t(struct i2c_client *client)
{
	u8 ts_report_cmd[1] = {0xb2};
	u8 ts_in_data[17] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	
	int ret;
	int i;
	
	ret=zet6221_i2c_write_tsdata(client, ts_report_cmd, 1);

	if (ret > 0)
	{
			//udelay(10);
			msleep(10);
			printk ("=============== zet6221_ts_get_report_mode_t ===============\n");
			ret=zet6221_i2c_read_tsdata(client, ts_in_data, 17);
			
			if(ret > 0)
			{
				
				for(i=0;i<8;i++)
				{
					pc[i]=ts_in_data[i] & 0xff;
				}
				
				if(pc[3] != 0x08)
				{
					printk ("=============== zet6221_ts_get_report_mode_t report error ===============\n");
					return 0;
				}

				xyExchange = (ts_in_data[16] & 0x8) >> 3;
				if(xyExchange == 1)
				{
					ResolutionY= ts_in_data[9] & 0xff;
					ResolutionY= (ResolutionY << 8)|(ts_in_data[8] & 0xff);
					ResolutionX= ts_in_data[11] & 0xff;
					ResolutionX= (ResolutionX << 8) | (ts_in_data[10] & 0xff);
				}
				else
				{
					ResolutionX = ts_in_data[9] & 0xff;
					ResolutionX = (ResolutionX << 8)|(ts_in_data[8] & 0xff);
					ResolutionY = ts_in_data[11] & 0xff;
					ResolutionY = (ResolutionY << 8) | (ts_in_data[10] & 0xff);
				}
					
				FingerNum = (ts_in_data[15] & 0x7f);
				KeyNum = (ts_in_data[15] & 0x80);
				inChargerMode = (ts_in_data[16] & 0x2) >> 1;

				if(KeyNum==0)
					bufLength  = 3+4*FingerNum;
				else
					bufLength  = 3+4*FingerNum+1;
				
			}
			else
			{
				printk ("=============== zet6221_ts_get_report_mode_t READ ERROR ===============\n");
				return 0;
			}
							
	}
	else
	{
		printk ("=============== zet6221_ts_get_report_mode_t WRITE ERROR ===============\n");
		return 0;
	}
	return 1;
}

/***********************************************************************
    [function]: 
		        callback: interrupt function;
    [parameters]:
    			irq[in]:  irq value;
    			dev_id[in]: dev_id;

    [return]:
			    NULL;
************************************************************************/
static irqreturn_t zet6221_ts_interrupt(int irq, void *dev_id)
{
	//polling_time	= D_POLLING_TIME;

	//printk("%s-------------\n",__func__);
	/*if (finger_mode > 0)
	{*/
		if (gpio_get_value(TS_INT_GPIO) == 0)
		{
			/* IRQ is triggered by FALLING code here */
			struct zet6221_tsdrv *ts_drv = dev_id;
			//disable_irq_nosync(ts_drv->irq);
			schedule_work(&ts_drv->work1);
			//DPRINTK("TS1_INT_GPIO falling\n");
		}else
		{
			//DPRINTK("TS1_INT_GPIO raising\n");
		}
	//}

	return IRQ_HANDLED;
}

/***********************************************************************
    [function]: 
		        callback: touch information handler;
    [parameters]:
    			_work[in]:  struct work_struct;

    [return]:
			    NULL;
************************************************************************/
static void zet6221_ts_work(struct work_struct *_work)
{
	u32 x[MAX_FINGER_NUMBER], y[MAX_FINGER_NUMBER], z[MAX_FINGER_NUMBER], pr, ky, points;
	u32 px,py,pz;
	u8 ret;
	u8 pressure;
	int i;

	if (bufLength == 0)
	{
		return;
	}

	if(resetCount == 1)
		{
			resetCount = 0;
			return;
		}

	if (gpio_get_value(TS_INT_GPIO) != 0)
	{
		/* do not read when IRQ is triggered by RASING*/
		//DPRINTK("INT HIGH\n");
		return;
	}

	struct zet6221_tsdrv *ts =
		container_of(_work, struct zet6221_tsdrv, work1);

	struct i2c_client *tsclient1 = ts->i2c_ts;

	ret = zet6221_ts_get_xy_from_panel(tsclient1, x, y, z, &pr, &ky);

	if(ret == 0x3C)
	{

		//DPRINTK( "[KY] = %02X\n", ky);
		
		points = pr;
		
		#if defined(TRANSLATE_ENABLE)
		touch_coordinate_traslating(x, y, points);
		#endif

		if(points == 0)
		{
			f_up_cnt++;
			if( f_up_cnt >= DEBOUNCE_NUMBER )
			{
				f_up_cnt = 0;
				input_report_abs(ts->input, ABS_MT_TOUCH_MAJOR, 0);
				input_report_key(ts->input, BTN_TOUCH , 0); 
				input_mt_sync(ts->input);
			}
			//printk("no_finger \n");
			goto no_finger;
		}

		f_up_cnt = 0;

		for(i=0;i<FingerNum;i++){
			pressure = (points >> (MAX_FINGER_NUMBER-i-1)) & 0x1;
			//DPRINTK( "valid=%04X pressure[%d]= %d x= %d y= %d\n",points , i, pressure,x[i],y[i]);
			//if (pressure) printk( "valid=%04X pressure[%d]= %d x= %d y= %d\n",points , i, pressure,x[i],y[i]);


	    	
			if(pressure)
			{		
				px = x[i];
				py = y[i];
				pz = z[i];
				input_report_key(ts->input, BTN_TOUCH , 1); 
				input_report_abs(ts->input, ABS_MT_TRACKING_ID, i);
	    		input_report_abs(ts->input, ABS_MT_TOUCH_MAJOR, 100);
				input_report_abs(ts->input, ABS_MT_PRESSURE, 100);
			//	input_report_abs( input, ABS_MT_TOUCH_MAJOR, 0);
	    		//input_report_abs(ts->input, ABS_MT_POSITION_X, x[i]);
	    		//input_report_abs(ts->input, ABS_MT_POSITION_Y, y[i]);
	    		//printk("x = %d  y = %d  MAX = %d\n",px,py,i);
	    		input_report_abs(ts->input, ABS_MT_POSITION_X, ResolutionX-px);
	    		input_report_abs(ts->input, ABS_MT_POSITION_Y, py);				   
	    		input_mt_sync(ts->input);
				

			}

			/*else if(px < 800 && py < 480)  
			{  		     				   
			    input_report_key(ts->input, BTN_TOUCH , 0); 
		        input_report_abs(ts->input, ABS_MT_PRESSURE, 0);
                input_report_abs(ts->input, ABS_MT_TOUCH_MAJOR, 0);
                input_report_abs(ts->input, ABS_MT_WIDTH_MAJOR, 0);
				//input_report_abs(ts->input, ABS_MT_TRACKING_ID, i);
				printk("release x = %d  y = %d  MAX = %d\n",px,py,i-1);
	     		input_report_abs(ts->input, ABS_MT_POSITION_X, px);
	    		input_report_abs(ts->input, ABS_MT_POSITION_Y, py);
				input_mt_sync(ts->input);				
				input_sync(ts->input);
			     px = x[i];
				py = y[i];
				pz = z[i];
			}*/
		}
		//input_sync(ts->input);

no_finger:
		if(KeyNum > 0)
		{
			//for(i=0;i<MAX_KEY_NUMBER;i++)
			for(i=0;i<4;i++)
			{			
				pressure = ky & ( 0x01 << i );
				switch(i)
				{
					case 0:
						if(pressure)
						{
							if(key_back_pressed)
							{
								input_report_key(ts->input, KEY_BACK, 1);
								input_report_key(ts->input, KEY_BACK, 0);
								key_back_pressed = 0x0;
							}
						}else
						{
							key_back_pressed = 0x1;
						}
						break;
					case 1:
						if(pressure)
						{
							input_report_key(ts->input, KEY_HOME, 1);
							input_report_key(ts->input, KEY_HOME, 0);
						}
						break;
					case 2:
						if(pressure)
						{
							if(key_menu_pressed)
							{
								input_report_key(ts->input, KEY_MENU, 1);
								input_report_key(ts->input, KEY_MENU, 0);
								key_menu_pressed = 0x0;
							}
						}else
						{
							key_menu_pressed = 0x1;
						}
						break;
					case 3:
						if(pressure)
						{
							if(key_search_pressed)
							{
								input_report_key(ts->input, KEY_SEARCH, 1);
								input_report_key(ts->input, KEY_SEARCH, 0);
								key_search_pressed = 0x0;
							}
						}else
						{
							key_search_pressed = 0x1;
						}
						break;
					/*case 4:
						break;
					case 5:
						break;
					case 6:
						break;
					case 7:
						break;*/
				}

			}
		}

		input_sync(ts->input);		
		
		msleep(1);
		//enable_irq(ts->irq);
	}

}

/***********************************************************************
    [function]: 
		        callback: charger mode enable;
    [parameters]:
    			void

    [return]:
			    void
************************************************************************/
void zet6221_ts_charger_mode()
{	
	printk("zet6221_ts_charger_mode\n");
	struct zet6221_tsdrv *zet6221_ts;
	u8 ts_write_charge_cmd[1] = {0xb5}; 
	int ret=0;
	ret=zet6221_i2c_write_tsdata(this_client, ts_write_charge_cmd, 1);
}
EXPORT_SYMBOL_GPL(zet6221_ts_charger_mode);

/***********************************************************************
    [function]: 
		        callback: charger mode disable;
    [parameters]:
    			void

    [return]:
			    void
************************************************************************/
void zet6221_ts_charger_mode_disable()
{
	printk("zet6221_ts_charger_mode_disable\n");
	struct zet6221_tsdrv *zet6221_ts;
	u8 ts_write_cmd[1] = {0xb6}; 
	int ret=0;
	ret=zet6221_i2c_write_tsdata(this_client, ts_write_cmd, 1);
}
EXPORT_SYMBOL_GPL(zet6221_ts_charger_mode_disable);


static void ts_early_suspend(struct early_suspend *handler)
{
	//Sleep Mode
	u8 ts_sleep_cmd[1] = {0xb1}; 
	int ret=0;
	ret=zet6221_i2c_write_tsdata(this_client, ts_sleep_cmd, 1);
        return;

}

static void ts_late_resume(struct early_suspend *handler)
{

#if 1
        gpio_direction_output(TS_RST_GPIO, 0);
		msleep(10);
		gpio_direction_output(TS_RST_GPIO, 1);
		msleep(20);
#else
		//Wake Up Mode
		u8 ts_wakeup_cmd[1] = {0xb4};
		zet6221_i2c_write_tsdata(this_client, ts_wakeup_cmd, 1);
#endif

resetCount = 1;
      
return;

}

static int __devinit zet6221_ts_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
	int result;
	struct input_dev *input_dev;
	struct zet6221_tsdrv *zet6221_ts;
	
	int count=0;

	printk( "[TS] zet6221_ts_probe \n");
	gpio_direction_output(TS_PWR_GPIO, GPIO_HIGH);
	zet6221_ts = kzalloc(sizeof(*zet6221_ts), GFP_KERNEL);
	if (zet6221_ts == NULL) {
                result = -ENOMEM;
                goto fail_alloc_mem;
        }
	zet6221_ts->i2c_ts = client;
	zet6221_ts->gpio = TS_INT_GPIO; /*s3c6410*/
	//zet6221_ts->gpio = TS1_INT_GPIO;
	
	this_client = client;
	
	i2c_set_clientdata(client, zet6221_ts);

	client->driver = &zet6221_ts_driver;

	INIT_WORK(&zet6221_ts->work1, zet6221_ts_work);

	input_dev = input_allocate_device();
	if (!input_dev || !zet6221_ts) {
		result = -ENOMEM;
		goto fail_alloc_mem;
	}
	
	i2c_set_clientdata(client, zet6221_ts);

	input_dev->name = MJ5_TS_NAME;
	input_dev->phys = "zet6221_touch/input0";
	input_dev->id.bustype = BUS_HOST;
	input_dev->id.vendor = 0x0001;
	input_dev->id.product = 0x0002;
	input_dev->id.version = 0x0100;
   
//bootloader
#if 1
zet_download:
	printk( "zet6221_downloader \n");
	if(zet6221_downloader(client)<=0)
	{
		printk("end_download\n");
		//goto request_irq_fail;
	}
#endif
                                                                          
#if defined(TPINFO)
	
	count=0;             
	do{
		    
		ctp_reset();
			printk( "ctp_reset \n");

		//if(zet6221_ts_get_report_mode(client)==0)    //get IC info by waiting for INT=low
		if(zet6221_ts_get_report_mode_t(client)==0)  //get IC info by delay 
			{
				ResolutionX = X_MAX;
				ResolutionY = Y_MAX;
				FingerNum = FINGER_NUMBER;
				KeyNum = KEY_NUMBER;
				if(KeyNum==0)
					bufLength  = 3+4*FingerNum;
				else
					bufLength  = 3+4*FingerNum+1;
			}
		else
			{
				//bootloader
				if(zet6221_ts_version()!=0)
					break;
			}
		
		count++;
	}while(count<REPORT_POLLING_TIME);
	
	//bootloader
	if(count==REPORT_POLLING_TIME)
		goto zet_download;
	
#else
	ResolutionX = X_MAX;
	ResolutionY = Y_MAX;
	FingerNum = FINGER_NUMBER;
	KeyNum = KEY_NUMBER;
	if(KeyNum==0)
		bufLength  = 3+4*FingerNum;
	else
		bufLength  = 3+4*FingerNum+1;
#endif

	DPRINTK( "ResolutionX=%d ResolutionY=%d FingerNum=%d KeyNum=%d\n",ResolutionX,ResolutionY,FingerNum,KeyNum);
	//printk( "ResolutionX=%d ResolutionY=%d FingerNum=%d KeyNum=%d\n",ResolutionX,ResolutionY,FingerNum,KeyNum);

#if 0
	
    set_bit(ABS_MT_TOUCH_MAJOR, input_dev->absbit); 
    set_bit(ABS_MT_POSITION_X, input_dev->absbit); 
    set_bit(ABS_MT_POSITION_Y, input_dev->absbit); 
    set_bit(ABS_MT_WIDTH_MAJOR, input_dev->absbit); 
	input_set_abs_params(input_dev, ABS_MT_TOUCH_MAJOR, 0, P_MAX, 0, 0);
	input_set_abs_params(input_dev, ABS_MT_WIDTH_MAJOR, 0, 200, 0, 0);
	//if(ResolutionX==0 && ResolutionY==0)
	//{		
		input_set_abs_params(input_dev, ABS_MT_POSITION_X, 0, ResolutionX, 0, 0);
		input_set_abs_params(input_dev, ABS_MT_POSITION_Y, 0, ResolutionY, 0, 0);
	//}else
	//{
	//	input_set_abs_params(input_dev, ABS_MT_POSITION_X, 0, ResolutionX, 0, 0);
	//	input_set_abs_params(input_dev, ABS_MT_POSITION_Y, 0, ResolutionY, 0, 0);
	//}	
	//input_set_abs_params(input_dev, ABS_MT_WIDTH_MAJOR, 0, 10, 0, 0);
#else	

{


	//    input_dev->evbit[0] = BIT_MASK (EV_SYN)|BIT_MASK (EV_ABS) ;//mult touch

	    set_bit(EV_ABS, input_dev->evbit);
		set_bit(ABS_MT_POSITION_X, input_dev->absbit);
		set_bit(ABS_MT_POSITION_Y, input_dev->absbit);
		set_bit(ABS_MT_WIDTH_MAJOR,input_dev->absbit);
		set_bit(ABS_MT_TRACKING_ID,input_dev->absbit);
	//	  set_bit(BTN_TOUCH, input->keybit);
	
	
	//	input_set_abs_params(input_dev, ABS_MT_POSITION_X, 0, 800, 0, 0);
	//	input_set_abs_params(input_dev, ABS_MT_POSITION_Y, 0, 480, 0, 0);
		input_set_abs_params(input_dev, ABS_MT_POSITION_X, 0, ResolutionX, 0, 0);
		input_set_abs_params(input_dev, ABS_MT_POSITION_Y, 0, ResolutionY, 0, 0);
	
		//input_set_abs_params(input_dev, ABS_MT_PRESSURE, 0, (800 * 480) >> 1, 0, 0);
		input_set_abs_params(input_dev, ABS_MT_TRACKING_ID, 0, 10, 0, 0);	
		input_set_abs_params(input_dev, ABS_MT_WIDTH_MAJOR, 0, 16, 0, 0);
	    input_set_abs_params(input_dev, ABS_MT_TRACKING_ID, 0, 16, 0, 0);
	    input_set_abs_params(input_dev, ABS_MT_TOUCH_MAJOR,0, 1, 0, 0);
	    input_set_abs_params(input_dev, ABS_MT_PRESSURE,0, 1, 0, 0);

}

#endif
	

	set_bit(KEY_BACK, input_dev->keybit);
	set_bit(KEY_HOME, input_dev->keybit);
	set_bit(KEY_MENU, input_dev->keybit);
	//set_bit(KEY_SEARCH, input_dev->keybit);

	input_dev->evbit[0] = BIT(EV_SYN) | BIT_MASK(EV_KEY) | BIT_MASK(EV_ABS);
	input_dev->keybit[BIT_WORD(BTN_TOUCH)] = BIT_MASK(BTN_TOUCH);

	result = input_register_device(input_dev);
	if (result)
		goto fail_ip_reg;

	zet6221_ts->input = input_dev;

	input_set_drvdata(zet6221_ts->input, zet6221_ts);

	//setup_timer(&zet6221_ts->polling_timer, polling_timer_func, (unsigned long)zet6221_ts);
	//mod_timer(&zet6221_ts->polling_timer,jiffies + msecs_to_jiffies(200));
	
	
	//s3c6410
	//result = gpio_request(zet6221_ts->gpio, "GPN"); 
	result = gpio_request(zet6221_ts->gpio, "GPN"); 
	if (result)
		goto gpio_request_fail;

	zet6221_ts->irq = gpio_to_irq(zet6221_ts->gpio);
	DPRINTK( "[TS] zet6221_ts_probe.gpid_to_irq [zet6221_ts->irq=%d]\n",zet6221_ts->irq);

	result = request_irq(zet6221_ts->irq, zet6221_ts_interrupt,IRQF_TRIGGER_FALLING, ZET_TS_ID_NAME, zet6221_ts);
	if (result)
		goto request_irq_fail;

	//AT91SAM9G45
/*	if (at91_set_gpio_input(TS1_INT_GPIO, 0)) {
		printk(KERN_DEBUG "Cannot set pin %i for GPIO input.\n", TS1_INT_GPIO);
	}
	
	if (at91_set_deglitch(TS1_INT_GPIO, 1)) {
		printk(KERN_DEBUG "Cannot set pin %i for GPIO deglitch.\n", TS1_INT_GPIO);
	}
	
	result = request_irq (TS1_INT_GPIO, (void *)zet6221_ts_interrupt, IRQF_TRIGGER_FALLING | IRQF_TRIGGER_RISING, ZET_TS_ID_NAME, zet6221_ts);
	if (result < 0)
		goto request_irq_fail;
*/



	zet6221_ts->early_suspend.suspend = ts_early_suspend,
	zet6221_ts->early_suspend.resume = ts_late_resume,
	zet6221_ts->early_suspend.level = EARLY_SUSPEND_LEVEL_DISABLE_FB + 2,
	register_early_suspend(&zet6221_ts->early_suspend);
	//disable_irq(zet6221_ts->irq);

	probe_finished = 1;
	return 0;

request_irq_fail:
	gpio_free(zet6221_ts->gpio);
gpio_request_fail:
	free_irq(zet6221_ts->irq, zet6221_ts);
	input_unregister_device(input_dev);
	input_dev = NULL;
fail_ip_reg:
fail_alloc_mem:
	input_free_device(input_dev);
	kfree(zet6221_ts);
	return result;
}
      
static int __devexit zet6221_ts_remove(struct i2c_client *dev)
{
	struct zet6221_tsdrv *zet6221_ts = i2c_get_clientdata(dev);

	free_irq(zet6221_ts->irq, zet6221_ts);
	gpio_free(zet6221_ts->gpio);
	del_timer_sync(&zet6221_ts->polling_timer);
	input_unregister_device(zet6221_ts->input);
	kfree(zet6221_ts);

	return 0;
}
static int __init zet6221_ts_init(void)
{

	i2c_add_driver(&zet6221_ts_driver);
	return 0;
}
module_init(zet6221_ts_init);

static void __exit zet6221_ts_exit(void)
{
    i2c_del_driver(&zet6221_ts_driver);
}
module_exit(zet6221_ts_exit);

void zet6221_set_ts_mode(u8 mode)
{
	DPRINTK( "[Touch Screen]ts mode = %d \n", mode);
}
EXPORT_SYMBOL_GPL(zet6221_set_ts_mode);
EXPORT_SYMBOL_GPL(probe_finished);


MODULE_DESCRIPTION("ZET6221 I2C Touch Screen driver");
MODULE_LICENSE("GPL v2");
