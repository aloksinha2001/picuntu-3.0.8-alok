/* 
 * drivers/input/touchscreen/ft5x0x_ts.c
 *
 * FocalTech ft5x0x TouchScreen driver. 
 *
 * Copyright (c) 2010  Focal tech Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 *
 *	note: only support mulititouch	Wenfs 2010-10-01
 */
#include <linux/module.h>
#include <linux/delay.h>
#include <linux/hrtimer.h>
#include <linux/i2c.h>
#include <linux/input.h>
#include <linux/interrupt.h>
#include <linux/io.h>
#include <linux/platform_device.h>
#include <linux/async.h>
#include <linux/workqueue.h>
#include <linux/slab.h>
#include <mach/gpio.h>
#include <linux/irq.h>
#include <mach/board.h>
#include <linux/input/mt.h>

static int debug=0;
module_param(debug, int, S_IRUGO|S_IWUSR);

#define dprintk(level, fmt, arg...) do {			\
	if (debug >= level) 					\
	printk(KERN_WARNING"rk29xx_ft5x0x: " fmt , ## arg); } while (0)

#define RK29TP_DG(format, ...) dprintk(1, format, ## __VA_ARGS__)

#ifdef CONFIG_HAS_EARLYSUSPEND
#include <linux/earlysuspend.h>
#endif

//#define TOUCHKEY_ON_SCREEN
//#define TOUCH_KEY_LED  INVALID_GPIO
static unsigned char g_vid;

#define FT5X0X_TOUCH_SWAP_XY 0
#define CONFIG_FT5X0X_MULTITOUCH 1

/*
 * Added by yick @RockChip
 * Compatible with both types of firmware
 * default: point - only pressdown finger num
 * 			event - both down and up event
 */
#define USE_POINT 1
#if USE_POINT
uint16_t down_table	= 0;
uint16_t up_table	= ~0;
#endif

#define SCREEN_MAX_X    1024
#define SCREEN_MAX_Y    768
#define PRESS_MAX       255

#define FT5X0X_NAME	"ft5x0x_ts" 
#define MAX_CONTACTS 10
enum ft5x0x_ts_regs {
	FT5X0X_REG_PMODE	= 0xA5,	/* Power Consume Mode		*/	
};

//FT5X0X_REG_PMODE
#define PMODE_ACTIVE        0x00
#define PMODE_MONITOR       0x01
#define PMODE_STANDBY       0x02
#define PMODE_HIBERNATE     0x03

#ifndef ABS_MT_TOUCH_MAJOR
#define ABS_MT_TOUCH_MAJOR	0x30	/* touching ellipse */
#define ABS_MT_TOUCH_MINOR	0x31	/* (omit if circular) */
#define ABS_MT_WIDTH_MAJOR	0x32	/* approaching ellipse */
#define ABS_MT_WIDTH_MINOR	0x33	/* (omit if circular) */
#define ABS_MT_ORIENTATION	0x34	/* Ellipse orientation */
#define ABS_MT_POSITION_X	  0x35	/* Center X ellipse position */
#define ABS_MT_POSITION_Y	  0x36	/* Center Y ellipse position */
#define ABS_MT_TOOL_TYPE	  0x37	/* Type of touching device */
#define ABS_MT_BLOB_ID		  0x38	/* Group set of pkts as blob */
#endif /* ABS_MT_TOUCH_MAJOR */

static int debug1=0;
module_param(debug1, int, S_IRUGO|S_IWUSR);

static struct mutex ft_mutex;
struct point_data {
	u8 status;
	u8 id;
	u16 x;
	u16 y;
};

struct ts_event {
  u16  touch_point;
  struct point_data point[MAX_CONTACTS];
};

struct ft5x0x_ts_dev {
  struct i2c_client *client;	
	struct input_dev	*input_dev;
	int    irq;
	struct ts_event		event;
	struct work_struct 	pen_event_work;
	struct workqueue_struct *ts_workqueue;
	struct early_suspend	early_suspend;
};

static struct ft5x0x_ts_dev *g_dev;

//通过打开关闭该宏来开关下载配置.i文件
#ifndef CONFIG_TOUCHSCREEN_FT5X06_FIRMWARE_DOWNLOAD
//#define CONFIG_TOUCHSCREEN_FT5X06_FIRMWARE_DOWNLOAD
#endif


#if defined(CONFIG_TOUCHSCREEN_FT5X06_FIRMWARE_DOWNLOAD)

static struct i2c_client *this_client;

#define FT5406_IIC_SPEED          350*1000    //300*1000
#define FT5X0X_REG_THRES          0x80         /* Thresshold, the threshold be low, the sensitivy will be high */
#define FT5X0X_REG_REPORT_RATE    0x88         /* **************report rate, in unit of 10Hz **************/
#define FT5X0X_REG_PMODE          0xA5         /* Power Consume Mode 0 -- active, 1 -- monitor, 3 -- sleep */    
#define FT5X0X_REG_FIRMID         0xA6         /* ***************firmware version **********************/
#define FT5X0X_REG_NOISE_MODE     0xb2         /* to enable or disable power noise, 1 -- enable, 0 -- disable */

/***********************************************************************/

#define    FTS_PACKET_LENGTH        128


static u8 CTPM_FW[]=
{
    #include "ft_app.i"
};

typedef enum
{
    ERR_OK,
    ERR_MODE,
    ERR_READID,
    ERR_ERASE,
    ERR_STATUS,
    ERR_ECC,
    ERR_DL_ERASE_FAIL,
    ERR_DL_PROGRAM_FAIL,
    ERR_DL_VERIFY_FAIL
}E_UPGRADE_ERR_TYPE;

/***********************************************************************/

/***********************************************************************
    [function]: 
		           callback:                send data to ctpm by i2c interface;
    [parameters]:
			    txdata[in]:              data buffer which is used to send data;
			    length[in]:              the length of the data buffer;
    [return]:
			    FTS_TRUE:              success;
			    FTS_FALSE:             fail;
************************************************************************/
static int fts_i2c_txdata(u8 *txdata, int length)
{
	int ret;

	struct i2c_msg msg;

      msg.addr = this_client->addr;
      msg.flags = 0;
      msg.len = length;
      msg.buf = txdata;
	ret = i2c_transfer(this_client->adapter, &msg, 1);
	if (ret < 0)
		pr_err("%s i2c write error: %d\n", __func__, ret);

	return ret;
}

/***********************************************************************
    [function]: 
		           callback:               write data to ctpm by i2c interface;
    [parameters]:
			    buffer[in]:             data buffer;
			    length[in]:            the length of the data buffer;
    [return]:
			    FTS_TRUE:            success;
			    FTS_FALSE:           fail;
************************************************************************/
static bool  i2c_write_interface(u8* pbt_buf, int dw_lenth)
{
    int ret;
    ret=i2c_master_send(this_client, pbt_buf, dw_lenth);
    if(ret<=0)
    {
        printk("[TSP]i2c_write_interface error line = %d, ret = %d\n", __LINE__, ret);
        return false;
    }

    return true;
}

/***********************************************************************
    [function]: 
		           callback:                read register value ftom ctpm by i2c interface;
    [parameters]:
                        reg_name[in]:         the register which you want to write;
			    tx_buf[in]:              buffer which is contained of the writing value;
    [return]:
			    FTS_TRUE:              success;
			    FTS_FALSE:             fail;
************************************************************************/
static bool fts_register_write(u8 reg_name, u8* tx_buf)
{
	u8 write_cmd[2] = {0};

	write_cmd[0] = reg_name;
	write_cmd[1] = *tx_buf;

	/*call the write callback function*/
	return i2c_write_interface(write_cmd, 2);
}

/***********************************************************************
[function]: 
                      callback:         send a command to ctpm.
[parameters]:
			  btcmd[in]:       command code;
			  btPara1[in]:     parameter 1;    
			  btPara2[in]:     parameter 2;    
			  btPara3[in]:     parameter 3;    
                      num[in]:         the valid input parameter numbers, 
                                           if only command code needed and no 
                                           parameters followed,then the num is 1;    
[return]:
			  FTS_TRUE:      success;
			  FTS_FALSE:     io fail;
************************************************************************/
static bool cmd_write(u8 btcmd,u8 btPara1,u8 btPara2,u8 btPara3,u8 num)
{
    u8 write_cmd[4] = {0};

    write_cmd[0] = btcmd;
    write_cmd[1] = btPara1;
    write_cmd[2] = btPara2;
    write_cmd[3] = btPara3;
    return i2c_write_interface(write_cmd, num);
}

/***********************************************************************
    [function]: 
		           callback:              read data from ctpm by i2c interface;
    [parameters]:
			    buffer[in]:            data buffer;
			    length[in]:           the length of the data buffer;
    [return]:
			    FTS_TRUE:            success;
			    FTS_FALSE:           fail;
************************************************************************/
static bool i2c_read_interface(u8* pbt_buf, int dw_lenth)
{
    int ret;
    
    ret=i2c_master_recv(this_client, pbt_buf, dw_lenth);

    if(ret<=0)
    {
        printk("[TSP]i2c_read_interface error\n");
        return false;
    }
  
    return true;
}


/***********************************************************************
[function]: 
                      callback:         read a byte data  from ctpm;
[parameters]:
			  buffer[in]:       read buffer;
			  length[in]:      the size of read data;    
[return]:
			  FTS_TRUE:      success;
			  FTS_FALSE:     io fail;
************************************************************************/
static bool byte_read(u8* buffer, int length)
{
    return i2c_read_interface(buffer, length);
}

/***********************************************************************
[function]: 
                      callback:         write a byte data  to ctpm;
[parameters]:
			  buffer[in]:       write buffer;
			  length[in]:      the size of write data;    
[return]:
			  FTS_TRUE:      success;
			  FTS_FALSE:     io fail;
************************************************************************/
static bool byte_write(u8* buffer, int length)
{
    
    return i2c_write_interface(buffer, length);
}

/***********************************************************************
    [function]: 
		           callback:                 read register value ftom ctpm by i2c interface;
    [parameters]:
                        reg_name[in]:         the register which you want to read;
			    rx_buf[in]:              data buffer which is used to store register value;
			    rx_length[in]:          the length of the data buffer;
    [return]:
			    FTS_TRUE:              success;
			    FTS_FALSE:             fail;
************************************************************************/
static bool fts_register_read(u8 reg_name, u8* rx_buf, int rx_length)
{
	u8 read_cmd[2]= {0};
	u8 cmd_len 	= 0;

	read_cmd[0] = reg_name;
	cmd_len = 1;	

	/*send register addr*/
	if(!i2c_write_interface(&read_cmd[0], cmd_len))
	{
		return false;
	}

	/*call the read callback function to get the register value*/		
	if(!i2c_read_interface(rx_buf, rx_length))
	{
		return false;
	}
	return true;
}



/***********************************************************************
[function]: 
                        callback:          burn the FW to ctpm.
[parameters]:
			    pbt_buf[in]:     point to Head+FW ;
			    dw_lenth[in]:   the length of the FW + 6(the Head length);    
[return]:
			    ERR_OK:          no error;
			    ERR_MODE:      fail to switch to UPDATE mode;
			    ERR_READID:   read id fail;
			    ERR_ERASE:     erase chip fail;
			    ERR_STATUS:   status error;
			    ERR_ECC:        ecc error.
************************************************************************/
E_UPGRADE_ERR_TYPE  fts_ctpm_fw_upgrade(u8* pbt_buf, int dw_lenth)
{
    u8  cmd,reg_val[2] = {0};
	u8  buffer[2] = {0};
    u8  packet_buf[FTS_PACKET_LENGTH + 6];
    u8  auc_i2c_write_buf[10];
    u8  bt_ecc;
	
    int  j,temp,lenght,i_ret,packet_number, i = 0;
    int  i_is_new_protocol = 0;
	

    /******write 0xaa to register 0xfc******/
    cmd=0xaa;
    fts_register_write(0xfc,&cmd);
    mdelay(50);
	
     /******write 0x55 to register 0xfc******/
    cmd=0x55;
    fts_register_write(0xfc,&cmd);
    printk("[TSP] Step 1: Reset CTPM test\n");
   
    mdelay(10);   


    /*******Step 2:Enter upgrade mode ****/
    printk("\n[TSP] Step 2:enter new update mode\n");
    auc_i2c_write_buf[0] = 0x55;
    auc_i2c_write_buf[1] = 0xaa;
    do
    {
        i ++;
        i_ret = fts_i2c_txdata(auc_i2c_write_buf, 2);
        mdelay(5);
    }while(i_ret <= 0 && i < 10 );

    if (i > 1)
    {
        i_is_new_protocol = 1;
    }

    /********Step 3:check READ-ID********/        
    cmd_write(0x90,0x00,0x00,0x00,4);
    byte_read(reg_val,2);
    if (reg_val[0] == 0x79 && reg_val[1] == 0x3)
    {
        printk("[TSP] Step 3: CTPM ID,ID1 = 0x%x,ID2 = 0x%x\n",reg_val[0],reg_val[1]);
    }
    else
    {
        return ERR_READID;
        //i_is_new_protocol = 1;
    }
    

     /*********Step 4:erase app**********/
    if (i_is_new_protocol)
    {
        cmd_write(0x61,0x00,0x00,0x00,1);
    }
    else
    {
        cmd_write(0x60,0x00,0x00,0x00,1);
    }
    mdelay(1500);
    printk("[TSP] Step 4: erase. \n");



    /*Step 5:write firmware(FW) to ctpm flash*/
    bt_ecc = 0;
    printk("[TSP] Step 5: start upgrade. \n");
    dw_lenth = dw_lenth - 8;
    packet_number = (dw_lenth) / FTS_PACKET_LENGTH;
    packet_buf[0] = 0xbf;
    packet_buf[1] = 0x00;
    for (j=0;j<packet_number;j++)
    {
        temp = j * FTS_PACKET_LENGTH;
        packet_buf[2] = (u8)(temp>>8);
        packet_buf[3] = (u8)temp;
        lenght = FTS_PACKET_LENGTH;
        packet_buf[4] = (u8)(lenght>>8);
        packet_buf[5] = (u8)lenght;

        for (i=0;i<FTS_PACKET_LENGTH;i++)
        {
            packet_buf[6+i] = pbt_buf[j*FTS_PACKET_LENGTH + i]; 
            bt_ecc ^= packet_buf[6+i];
        }
        
        byte_write(&packet_buf[0],FTS_PACKET_LENGTH + 6);
        mdelay(FTS_PACKET_LENGTH/6 + 1);
        if ((j * FTS_PACKET_LENGTH % 1024) == 0)
        {
              printk("[TSP] upgrade the 0x%x th byte.\n", ((unsigned int)j) * FTS_PACKET_LENGTH);
        }
    }

    if ((dw_lenth) % FTS_PACKET_LENGTH > 0)
    {
        temp = packet_number * FTS_PACKET_LENGTH;
        packet_buf[2] = (u8)(temp>>8);
        packet_buf[3] = (u8)temp;

        temp = (dw_lenth) % FTS_PACKET_LENGTH;
        packet_buf[4] = (u8)(temp>>8);
        packet_buf[5] = (u8)temp;

        for (i=0;i<temp;i++)
        {
            packet_buf[6+i] = pbt_buf[ packet_number*FTS_PACKET_LENGTH + i]; 
            bt_ecc ^= packet_buf[6+i];
        }

        byte_write(&packet_buf[0],temp+6);    
        mdelay(20);
    }

    /***********send the last six byte**********/
    for (i = 0; i<6; i++)
    {
        temp = 0x6ffa + i;
        packet_buf[2] = (u8)(temp>>8);
        packet_buf[3] = (u8)temp;
        temp =1;
        packet_buf[4] = (u8)(temp>>8);
        packet_buf[5] = (u8)temp;
        packet_buf[6] = pbt_buf[ dw_lenth + i]; 
        bt_ecc ^= packet_buf[6];

        byte_write(&packet_buf[0],7);  
        mdelay(20);
    }

    /********send the opration head************/
    cmd_write(0xcc,0x00,0x00,0x00,1);
    byte_read(reg_val,1);
    printk("[TSP] Step 6:  ecc read 0x%x, new firmware 0x%x. \n", reg_val[0], bt_ecc);
    if(reg_val[0] != bt_ecc)
    {
        return ERR_ECC;
    }

    /*******Step 7: reset the new FW**********/
    cmd_write(0x07,0x00,0x00,0x00,1);
	mdelay(100);//100ms	
	fts_register_read(0xfc, buffer, 1);	
	if (buffer[0] == 1)
	{
	cmd=4;
	fts_register_write(0xfc, &cmd);
	mdelay(2500);//2500ms	
	 do	
	 {	
	 fts_register_read(0xfc, buffer, 1);	
	 mdelay(100);//100ms	
	 }while (buffer[0] != 1); 		   	
	}
    return ERR_OK;
}


/***********************************************************************/

int fts_ctpm_fw_upgrade_with_i_file(void)
{
   u8*     pbt_buf = 0;
   int i_ret;
    
   pbt_buf = CTPM_FW;
   i_ret =  fts_ctpm_fw_upgrade(pbt_buf,sizeof(CTPM_FW));
   
   return i_ret;
}

/***********************************************************************/

unsigned char fts_ctpm_get_upg_ver(void)
{
    unsigned int ui_sz;
	
    ui_sz = sizeof(CTPM_FW);
    if (ui_sz > 2)
    {
        return CTPM_FW[ui_sz - 2];
    }
    else
        return 0xff; 
 
}

#endif


#ifdef TOUCHKEY_ON_SCREEN

static void key_led_ctrl(int on)
{
	#ifdef TOUCH_KEY_LED
		gpio_set_value(TOUCH_KEY_LED, on);
	#endif
}

static int g_screen_key=0;

static unsigned char initkey_code[] =
{
    KEY_BACK,  KEY_HOMEPAGE, KEY_MENU
};

typedef struct {
	int x;
	int y;
	int keycode;
} rect;

static int get_screen_key(int x, int y)
{
	const int span = 10;
	int idx;
	rect rt[] = {
		{829,	79, 	KEY_BACK},   
		{829,	47,	KEY_HOMEPAGE},        /* home */ 
		{829,	0,	KEY_MENU},      	  /* menu */ 
		{0,0,0}, 
	}; 
	for(idx=0; rt[idx].keycode; idx++)
	{
		RK29TP_DG("***x1=%d, y1=%d\n", rt[idx].x, rt[idx].y);
		if(x >= rt[idx].x-span && x<= rt[idx].x+span)
			if(y >= rt[idx].y-span && y<= rt[idx].y+span)
				return rt[idx].keycode;
	}
	RK29TP_DG("***x=%d, y=%d\n", x, y);
	return 0;
}

struct input_dev *tp_key_input;
static int report_screen_key(int down_up)
{
	struct input_dev * keydev=(struct input_dev *)tp_key_input;	
	{
		input_event(keydev, EV_KEY, g_screen_key, down_up);
		input_sync(keydev);
	} 
	key_led_ctrl(down_up);
	if(!down_up) {
		g_screen_key=0;
	}
	
	return 0;
}
#endif

static int ft5x0x_i2c_rxdata(char *rxdata, int length)
{
	int ret;

	struct i2c_msg msgs[] = {
		{
			.addr	= g_dev->client->addr,
			.flags	= 0,
			.len	= 1,
			.buf	= rxdata,
			.scl_rate = 350 * 1000,
		},
		{
			.addr	= g_dev->client->addr,
			.flags	= I2C_M_RD,
			.len	= length,
			.buf	= rxdata,
			.scl_rate = 350 * 1000,
		},
	};

	ret = i2c_transfer(g_dev->client->adapter, msgs, 2);
	if (ret < 0)
		pr_err("msg %s i2c read error: %d\n", __func__, ret);
	
	return ret;
}

static int ft5x0x_i2c_txdata(char *txdata, int length)
{
	int ret;

	struct i2c_msg msg[] = {
		{
			.addr	= g_dev->client->addr,
			.flags	= 0,
			.len	= length,
			.buf	= txdata,
			.scl_rate = 350 * 1000,
		},
	};

	ret = i2c_transfer(g_dev->client->adapter, msg, 1);
	if (ret < 0)
		pr_err("%s i2c write error: %d\n", __func__, ret);

	return ret;
}

static int ft5x0x_set_reg(u8 addr, u8 para)
{
    u8 buf[3];
    int ret = -1;

    buf[0] = addr;
    buf[1] = para;
    ret = ft5x0x_i2c_txdata(buf, 2);
    if (ret < 0) {
        pr_err("write reg failed! %#x ret: %d", buf[0], ret);
        return -1;
    }
    
    return 0;
}

static int ft5x0x_read_data(void)
{
	struct ft5x0x_ts_dev *data = i2c_get_clientdata(g_dev->client);
	struct ts_event *event = &data->event;

	u8 buf[4+MAX_CONTACTS*6]= {0};//set send addr to 0x00 *important*
	int ret = -1;
    int i;


		ret = ft5x0x_i2c_rxdata(buf, 4+MAX_CONTACTS*6);

    	if (ret < 0) {
			printk("%s read_data i2c_rxdata failed: %d\n", __func__, ret);
			return ret;
		}
#if 0	// Just for debug
		u8 uc_ecc;
		int i;
		uc_ecc = buf[2];
		for (i=0; i<MAX_CONTACTS; i++)
		{
			uc_ecc ^= buf[3+6*i];
			uc_ecc ^= buf[4+6*i];
			uc_ecc ^= buf[5+6*i];
			uc_ecc ^= buf[6+6*i];
		}
//		if (uc_ecc == buf[1])  break;
//	}


	if (uc_ecc != buf[1])
	{
		printk("ecc check error uc_ecc=0x%x, buf[1]=0x%x.\n",uc_ecc, buf[1]);
		return 1;
	}
#endif


	memset(event, ~0x00, sizeof(struct ts_event));

#if USE_POINT
	event->touch_point = buf[2] & 0x0f;// 0000 1111
#else
	event->touch_point = buf[2] >>4;// 0000 1111
#endif

    RK29TP_DG("touch_point = %d\n", event->touch_point);
#ifdef CONFIG_FT5X0X_MULTITOUCH
#if 0
    switch (event->touch_point) {
		case 10:
			event->point[9].status = (buf[0x39] & 0xc0)>>6;
			event->point[9].id = (buf[0x3b] & 0xf0)>>4;
			event->point[9].x = (s16)(buf[0x39] & 0x07)<<8 | (s16)buf[0x3a];
			event->point[9].y = (s16)(buf[0x3b] & 0x07)<<8 | (s16)buf[0x3c];
		case 9:
			event->point[8].status = (buf[0x33] & 0xc0)>>6;
			event->point[8].id = (buf[0x35] & 0xf0)>>4;
			event->point[8].x = (s16)(buf[0x33] & 0x07)<<8 | (s16)buf[0x34];
			event->point[8].y = (s16)(buf[0x35] & 0x07)<<8 | (s16)buf[0x36];
		case 8:
			event->point[7].status = (buf[0x2d] & 0xc0)>>6;
			event->point[7].id = (buf[0x2f] & 0xf0)>>4;
			event->point[7].x = (s16)(buf[0x2d] & 0x07)<<8 | (s16)buf[0x2e];
			event->point[7].y = (s16)(buf[0x2f] & 0x07)<<8 | (s16)buf[0x30];
		case 7:
			event->point[6].status = (buf[0x27] & 0xc0)>>6;
			event->point[6].id = (buf[0x29] & 0xf0)>>4;
			event->point[6].x = (s16)(buf[0x27] & 0x07)<<8 | (s16)buf[0x28];
			event->point[6].y = (s16)(buf[0x29] & 0x07)<<8 | (s16)buf[0x2a];
		case 6:
			event->point[5].status = (buf[0x21] & 0xc0)>>6;
			event->point[5].id = (buf[0x23] & 0xf0)>>4;
			event->point[5].x = (s16)(buf[0x21] & 0x07)<<8 | (s16)buf[0x22];
			event->point[5].y = (s16)(buf[0x23] & 0x07)<<8 | (s16)buf[0x24];
		case 5:
			event->point[4].status = (buf[0x1b] & 0xc0)>>6;
			event->point[4].id = (buf[0x1d] & 0xf0)>>4;
			event->point[4].x = (s16)(buf[0x1b] & 0x07)<<8 | (s16)buf[0x1c];
			event->point[4].y = (s16)(buf[0x1d] & 0x07)<<8 | (s16)buf[0x1e];
		case 4:
			event->point[3].status = (buf[0x15] & 0xc0)>>6;
			event->point[3].id = (buf[0x17] & 0xf0)>>4;
			event->point[3].x = (s16)(buf[0x15] & 0x07)<<8 | (s16)buf[0x16];
			event->point[3].y = (s16)(buf[0x17] & 0x07)<<8 | (s16)buf[0x18];
		case 3:
			event->point[2].status = (buf[0x0f] & 0xc0)>>6;
			event->point[2].id = (buf[0x11] & 0xf0)>>4;
			event->point[2].x = (s16)(buf[0x0f] & 0x07)<<8 | (s16)buf[0x10];
			event->point[2].y = (s16)(buf[0x11] & 0x07)<<8 | (s16)buf[0x12];
		case 2:
			event->point[1].status = (buf[0x09] & 0xc0)>>6;
			event->point[1].id = (buf[0x0b] & 0xf0)>>4;
			event->point[1].x = (s16)(buf[0x09] & 0x07)<<8 | (s16)buf[0x0a];
			event->point[1].y = (s16)(buf[0x0b] & 0x07)<<8 | (s16)buf[0x0c];
		case 1:
			event->point[0].status = (buf[0x03] & 0xc0)>>6;
			event->point[0].id = (buf[0x05] & 0xf0)>>4;
			event->point[0].x = (s16)(buf[0x03] & 0x07)<<8 | (s16)buf[0x04];
			event->point[0].y = (s16)(buf[0x05] & 0x07)<<8 | (s16)buf[0x06];
        default:
		    return 0;
	}
#else
    for (i=0; i<event->touch_point; i++) {
    	event->point[i].status = (buf[3+i*6] & 0xc0)>>6;
    	event->point[i].id = (buf[5+i*6] & 0xf0)>>4;
    	event->point[i].x = (s16)(buf[3+i*6] & 0x07)<<8 | (s16)buf[4+i*6];
    	event->point[i].y = (s16)(buf[5+i*6] & 0x07)<<8 | (s16)buf[6+i*6];
    }

    return 0;
#endif
#endif
}

static void ft5x0x_report_value(void)
{
	struct ft5x0x_ts_dev *data = i2c_get_clientdata(g_dev->client);
	struct ts_event *event = &data->event;
	u8 i = 0;
	static int s_screen_key[MAX_CONTACTS] = {0}, sx, sy;


    static int been_to_screen[MAX_CONTACTS] = {0};
#if 0
	printk("point is %d x0 is %d y0 is %d\n",   
			//P1 status is %x ID1 is %x x1 is 0x%x y1 is 0x%x\n\n",
					event->touch_point,  
					//event->point[0].status,	//event->point[0].id,
					event->point[0].x, event->point[0].y);
					//event->point[1].status, event->point[1].id,
					//event->point[1].x, event->point[1].y);
#endif
#if USE_POINT
#if 0
    int being_to_screen[MAX_CONTACTS] = {0};

    for (i=0; i<event->touch_point; i++) {
        if ((event->point[i].id >= 0) && (event->point[i].id < MAX_CONTACTS)) {
            input_mt_slot(data->input_dev,event->point[i].id);
            input_report_abs(data->input_dev, ABS_MT_TRACKING_ID, event->point[i].id);
            input_report_abs(data->input_dev, ABS_MT_POSITION_X, event->point[i].x);
            input_report_abs(data->input_dev, ABS_MT_POSITION_Y, event->point[i].y);
            //printk("(%d) down %d: (%d, %d)\n", __LINE__, event->point[i].id, event->point[i].x, event->point[i].y);
            
            being_to_screen[event->point[i].id] = 1;
        }
    }
    for (i=0; i<MAX_CONTACTS; i++) {
	    if ((been_to_screen[i] == 1) && (being_to_screen[i] == 0)) {
			input_mt_slot(data->input_dev, i);
			input_report_abs(data->input_dev, ABS_MT_TRACKING_ID, -1);
			//printk("(%d) up %d\n", __LINE__, i);
	    }
	    been_to_screen[i] = being_to_screen[i];
    }
#else
	down_table = 0;
	for(i=0; i<event->touch_point; i++) {	
	    if (event->point[i].id >= MAX_CONTACTS)
	        continue;
		//RK29TP_DG("%d,  cc111111_touch_key(%d,%d)\n",event->point[i].id, event->point[i].x, event->point[i].y);
		down_table |= 1 << event->point[i].id;
		/*if (event->point[i].x >= 810)  
		{
			if(debug1)
			{
				if (s_screen_key[event->point[i].id] == 0) 
				{
				s_screen_key[event->point[i].id] = 1;
#ifdef TOUCHKEY_ON_SCREEN
				RK29TP_DG("%d,  cc_touch_key(%d,%d)\n",event->point[i].id, event->point[i].x, event->point[i].y);
				if (g_screen_key = get_screen_key(event->point[i].x, event->point[i].y)) 
				{
					RK29TP_DG("touch key = %x down\n", g_screen_key);
					report_screen_key(1);
				}
#endif
			}	
		}
		} else*/ 
		{
			sx = event->point[i].x;//(1792 - event->point[i].y) * 800 / 1792;
			sy = event->point[i].y;//(576 - (event->point[i].x - 288)) * 480 / 576;

			if(FT5X0X_TOUCH_SWAP_XY)
			{
				int tmp=sx;
				sx=sy;
				sy=tmp;
			}
			//RK29TP_DG("(++++++++++++++%d, %d)\n", sx, sy);
			input_mt_slot(data->input_dev,event->point[i].id);		
			input_report_abs(data->input_dev, ABS_MT_TRACKING_ID, event->point[i].id);			
			//input_report_abs(data->input_dev, ABS_MT_TOUCH_MAJOR, 200);
			input_report_abs(data->input_dev, ABS_MT_POSITION_X, sx);
			input_report_abs(data->input_dev, ABS_MT_POSITION_Y, sy);
			//input_report_abs(data->input_dev, ABS_MT_WIDTH_MAJOR, 100);
			//input_report_abs(data->input_dev, ABS_MT_PRESSURE,10);

			//input_mt_sync(data->input_dev);

			been_to_screen[event->point[i].id] = 1;
		}
	}	
	
	for(i=0; i<MAX_CONTACTS; i++) 
	{
		if( ( (~down_table) & 1<<i) && !(up_table & 1<<i) )
		{
            RK29TP_DG("been_to_screen[%d] = %d", i, been_to_screen[i]);
			if (s_screen_key[i] == 1&&debug1) 
				{
					s_screen_key[i] = 0;
#ifdef TOUCHKEY_ON_SCREEN
					RK29TP_DG("touch key up!\n");
					report_screen_key(0);
#endif          			
            			}
#if 0
                if (been_to_screen[i] == 1) 
		{
                    input_mt_slot(data->input_dev, i);
                    input_report_abs(data->input_dev, ABS_MT_TRACKING_ID, -1);
                }
		} else 
#endif
            {
			//RK29TP_DG("(-----------)\n");
				input_mt_slot(data->input_dev, i);
				input_report_abs(data->input_dev, ABS_MT_TRACKING_ID, -1);
	   }
            
            been_to_screen[i] = 0;
		}
	}
	up_table = ~down_table;
#endif
	
	input_sync(data->input_dev);

#else

	for(i=0; i<event->touch_point; i++)
	{
		if(event->point[i].status == 0 || event->point[i].status == 2 ) 
		{

			sx = event->point[i].x;
			sy = event->point[i].y;

			if(FT5X0X_TOUCH_SWAP_XY)
			{
				int tmp=sx;
				sx=sy;
				sy=tmp;
			}
			//RK29TP_DG("%0x %d,  cc0000000000+222222_touch_key(%d,%d)\n",data->input_dev,event->point[i].id, sx, sy);
			
			input_mt_slot(data->input_dev, event->point[i].id);
			input_report_abs(data->input_dev, ABS_MT_TRACKING_ID, event->point[i].id);
			input_report_abs(data->input_dev, ABS_MT_TOUCH_MAJOR, 200);
			input_report_abs(data->input_dev, ABS_MT_POSITION_X, sx);
			input_report_abs(data->input_dev, ABS_MT_POSITION_Y, sy);
			//input_report_abs(data->input_dev, ABS_MT_PRESSURE,10);
			input_report_abs(data->input_dev, ABS_MT_WIDTH_MAJOR, 100);
		}
		else if(event->point[i].status == 1) 
		{
		//RK29TP_DG("%0x %d,  cc0000000000+222222_touch_key(%d,%d)\n");

			input_mt_slot(data->input_dev, event->point[i].id);
			input_report_abs(data->input_dev, ABS_MT_TRACKING_ID, -1);
		}
		input_mt_sync(data->input_dev);
	}
	
	input_sync(data->input_dev);
#endif


}	/*end ft5x0x_report_value*/

static void ft5x0x_ts_pen_irq_work(struct work_struct *work)
{
	int ret = -1;
	mutex_lock(&ft_mutex);
	ret = ft5x0x_read_data();	
	if (ret == 0) {	
		ft5x0x_report_value();
	}
	mutex_unlock(&ft_mutex);
    // enable_irq(g_dev->irq);
}

static irqreturn_t ft5x0x_ts_interrupt(int irq, void *dev_id)
{
	struct ft5x0x_ts_dev *ft5x0x_ts = dev_id;
  	
	// disable_irq_nosync(g_dev->irq);		

	queue_work(ft5x0x_ts->ts_workqueue, &ft5x0x_ts->pen_event_work);

	return IRQ_HANDLED;
}

#ifdef CONFIG_HAS_EARLYSUSPEND
static void ft5x0x_ts_suspend(struct early_suspend *handler)
{
	int ret, i;
	struct ft5x0x_ts_dev *ts;
	ts =  container_of(handler, struct ft5x0x_ts_dev, early_suspend);
	
#ifdef TOUCHKEY_ON_SCREEN
	key_led_ctrl(0);
#endif

#if USE_POINT
	for(i=0; i<MAX_CONTACTS; i++) 
	{
		input_mt_slot(ts->input_dev, i);
		input_report_abs(ts->input_dev, ABS_MT_TRACKING_ID, -1);
	}
	
	input_sync(ts->input_dev);
#endif

	if(ts->irq)
		disable_irq_nosync(ts->irq);

	ret = cancel_work_sync(&ts->pen_event_work);
	if (ret && ts->irq) /* if work was pending disable-count is now 2 */
		enable_irq(ts->irq);
	// ==set mode ==, 
    ft5x0x_set_reg(FT5X0X_REG_PMODE, PMODE_HIBERNATE);
}

static void ft5x0x_ts_resume(struct early_suspend *handler)
{
    int i;
	struct ft5x0x_ts_dev *ts;
	ts =  container_of(handler, struct ft5x0x_ts_dev, early_suspend);
	// wake the mode
	
#ifdef TOUCHKEY_ON_SCREEN
	key_led_ctrl(0);
#endif
	gpio_direction_output(RK30_PIN4_PD0, 0);
	// gpio_set_value(RK29_PIN6_PC3,GPIO_LOW);
	msleep(5);
	gpio_set_value(RK30_PIN4_PD0,GPIO_HIGH);
	msleep(200);
#if USE_POINT
	for(i=0; i<MAX_CONTACTS; i++) 
	{
		input_mt_slot(ts->input_dev, i);
		input_report_abs(ts->input_dev, ABS_MT_TRACKING_ID, -1);
	}

	down_table	= 0;
	up_table	= ~0;
#endif

	if(ts->irq)
		enable_irq(ts->irq);
}
#endif  //CONFIG_HAS_EARLYSUSPEND

static int 
ft5x0x_ts_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
	struct ft5x0x_ts_dev *ft5x0x_ts;
	struct ft5x0x_platform_data *pdata = pdata = client->dev.platform_data;
	struct input_dev *input_dev;
	int err = 0;
	uint8_t version[4] = {0xA1, 0x00, 0x00, 0x00};
	int i;
    s8 phys[32];
	
  mutex_init(&ft_mutex);
	if (pdata == NULL) {
		dev_err(&client->dev, "%s: platform data is null\n", __func__);
		goto exit_platform_data_null;
	}	
	
	if (!i2c_check_functionality(client->adapter, I2C_FUNC_I2C)) {
		err = -ENODEV;
		goto exit_check_functionality_failed;
	}

	ft5x0x_ts = (struct ft5x0x_ts_dev *)kzalloc(sizeof(*ft5x0x_ts), GFP_KERNEL);
	if (!ft5x0x_ts)	{
		err = -ENOMEM;
		goto exit_alloc_data_failed;
	}

	input_dev = input_allocate_device();
	if (!input_dev) {
		err = -ENOMEM;
		dev_err(&client->dev, "failed to allocate input device\n");
		goto exit_input_dev_alloc_failed;
	}
	
	
	ft5x0x_ts->input_dev = input_dev;
	ft5x0x_ts->client = client;
	ft5x0x_ts->irq = client->irq;


/*
	input_mt_init_slots(input_dev, 255);
	__set_bit(EV_ABS, input_dev->evbit);
	__set_bit(INPUT_PROP_DIRECT, input_dev->propbit);
	set_bit(ABS_MT_POSITION_X, input_dev->absbit);
	set_bit(ABS_MT_POSITION_Y, input_dev->absbit);
	set_bit(ABS_MT_TOUCH_MAJOR, input_dev->absbit);
	set_bit(ABS_MT_WIDTH_MAJOR, input_dev->absbit);

	input_mt_init_slots(input_dev, MAX_CONTACTS);

	input_set_abs_params(input_dev,ABS_MT_POSITION_X, 0, SCREEN_MAX_X, 0, 0);
	input_set_abs_params(input_dev,ABS_MT_POSITION_Y, 0, SCREEN_MAX_Y, 0, 0);
	input_set_abs_params(input_dev,ABS_MT_TOUCH_MAJOR, 0, PRESS_MAX, 0, 0);
	input_set_abs_params(input_dev,ABS_MT_WIDTH_MAJOR, 0, 200, 0, 0);*/


/////////////////////////////////////////////////////////////////////////////
	input_dev->name		= FT5X0X_NAME;		//dev_name(&client->dev)
	input_dev->dev.parent = &client->dev;
	
    	sprintf(phys, "input/ts");
    	input_dev->phys = phys;
	input_dev->id.vendor = 0xDEAD;
	input_dev->id.product = 0xBEEF;
	input_dev->id.version = 10427;	//screen firmware version
#if 0//def TOUCHKEY_ON_SCREEN
	#ifdef TOUCH_KEY_LED
		err = gpio_request(TOUCH_KEY_LED, "key led");
		if (err < 0) {
			printk(KERN_ERR
			       "ft5x0x_probe: Unable to request gpio: %d\n",
			       TOUCH_KEY_LED);
			//goto exit_input_register_device_failed;
		} else {
			gpio_direction_output(TOUCH_KEY_LED, GPIO_LOW);
			gpio_set_value(TOUCH_KEY_LED, GPIO_LOW);
		}
	#endif
	
	tp_key_input = ft5x0x_ts->input_dev;
	for (i = 0; i < ARRAY_SIZE(initkey_code); i++) {
		// __set_bit(initkey_code[i], ft5x0x_ts->input_dev->keybit);
		input_set_capability(ft5x0x_ts->input_dev, EV_KEY, initkey_code[i]);
	}
#endif
	input_dev->evbit[0] = BIT_MASK(EV_SYN) | BIT_MASK(EV_KEY) | BIT_MASK(EV_ABS) ;
	input_dev->absbit[0] = BIT(ABS_X) | BIT(ABS_Y) ;

	__set_bit(INPUT_PROP_DIRECT, input_dev->propbit);//property bit set as input directly
	input_mt_init_slots(input_dev, 255);


	__set_bit(EV_ABS, input_dev->evbit);//set event bit

	set_bit(ABS_MT_POSITION_X, input_dev->absbit);
	set_bit(ABS_MT_POSITION_Y, input_dev->absbit);
	//set_bit(ABS_MT_TOUCH_MAJOR, input_dev->absbit);
	//set_bit(ABS_MT_WIDTH_MAJOR, input_dev->absbit);

	input_mt_init_slots(input_dev, MAX_CONTACTS);

	input_set_abs_params(input_dev,ABS_MT_POSITION_X, 0, SCREEN_MAX_X, 0, 0);
	input_set_abs_params(input_dev,ABS_MT_POSITION_Y, 0, SCREEN_MAX_Y, 0, 0);
	//input_set_abs_params(input_dev,ABS_MT_TOUCH_MAJOR, 0, 255, 0, 0);
	//input_set_abs_params(input_dev,ABS_MT_WIDTH_MAJOR, 0, 255, 0, 0);
	//input_set_abs_params(input_dev,ABS_MT_PRESSURE, 0, 255, 0, 0);

	err = input_register_device(input_dev);
	if (err) {
		dev_err(&client->dev,
		"ft5x0x_ts_probe: failed to register input device: %s\n",
		dev_name(&client->dev));
		goto exit_input_register_device_failed;
	}
	
	g_dev = ft5x0x_ts;
	
	i2c_set_clientdata(client, ft5x0x_ts);
	INIT_WORK(&ft5x0x_ts->pen_event_work, ft5x0x_ts_pen_irq_work);
	ft5x0x_ts->ts_workqueue = create_workqueue(FT5X0X_NAME);
	if (!ft5x0x_ts->ts_workqueue) {
		err = -ESRCH;
		goto exit_create_singlethread;
	}

  if(pdata->init_platform_hw)
      pdata->init_platform_hw();

  err = ft5x0x_i2c_rxdata(version, 4);
  if (err < 0) {
		dev_err(&client->dev, "ft5x0x_probe : fail to read TP lib version and vendor id\n");

        if(pdata->exit_platform_hw)
            pdata->exit_platform_hw();

		goto exit_platform_data_null;
  }
  dev_info(&client->dev, "ft5x0x_probe : TP lib version 0x%x vendor ID 0x%x\n", 
		  ((unsigned int)version[1]<<8 | (unsigned int)version[2]), (unsigned int)version[3]);
  version[0] = 0xA6;
  err = ft5x0x_i2c_rxdata(version, 2);
  if (err < 0) {
		dev_err(&client->dev, "ft5x0x_probe : fail to read TP fw version\n");
		goto exit_platform_data_null;
  }
  dev_info(&client->dev, "ft5x0x_probe : TP firmware version 0x%x\n", (unsigned int)version[1]);
  //ft5x0x_set_reg(0x80,0x64);

  if(!ft5x0x_ts->irq)
  {
    dev_dbg(&ft5x0x_ts->client->dev, "no IRQ?\n");
    return -ENODEV;
  }
  else
  {
    ft5x0x_ts->irq = gpio_to_irq(ft5x0x_ts->irq);
  }

#if defined(CONFIG_TOUCHSCREEN_FT5X06_FIRMWARE_DOWNLOAD)
{
    unsigned char reg_value;
    unsigned char reg_version;

         this_client=client;
  	 fts_register_read(FT5X0X_REG_FIRMID, &reg_version,1);
	 printk("[TSP] firmware version = 0x%2x\n", reg_version);
	 fts_register_read(FT5X0X_REG_REPORT_RATE, &reg_value,1);
	 printk("[TSP]firmware report rate = %dHz\n", reg_value*10);
	 fts_register_read(FT5X0X_REG_THRES, &reg_value,1);
	 printk("[TSP]firmware threshold = %d\n", reg_value * 4);
	 fts_register_read(FT5X0X_REG_NOISE_MODE, &reg_value,1);
	 printk("[TSP]nosie mode = 0x%2x\n", reg_value);

	  if (fts_ctpm_get_upg_ver() != reg_version)  
	  {
		  printk("[TSP] start upgrade new verison 0x%2x\n", fts_ctpm_get_upg_ver());
		  msleep(200);
		  err =  fts_ctpm_fw_upgrade_with_i_file();
		  if (err == 0)
		  {
			  printk("[TSP] ugrade successfuly.\n");
			  msleep(300);
			  fts_register_read(FT5X0X_REG_FIRMID, &reg_value,1);
			  printk("FTS_DBG from old version 0x%2x to new version = 0x%2x\n", reg_version, reg_value);
		  }
		  else
		  {
			  printk("[TSP]  ugrade fail err=%d, line = %d.\n",err, __LINE__);
		  }
		  msleep(4000);
	  }
}
#endif

  err = request_irq(ft5x0x_ts->irq, ft5x0x_ts_interrupt, IRQF_TRIGGER_FALLING/*IRQF_DISABLED*/, "ft5x0x_ts", ft5x0x_ts);
	if (err < 0) {
		dev_err(&client->dev, "ft5x0x_probe: request irq failed\n");
		goto exit_irq_request_failed;
	}

	disable_irq_nosync(g_dev->irq);

#ifdef CONFIG_HAS_EARLYSUSPEND
	ft5x0x_ts->early_suspend.level = EARLY_SUSPEND_LEVEL_BLANK_SCREEN + 1;
	ft5x0x_ts->early_suspend.suspend = ft5x0x_ts_suspend;
	ft5x0x_ts->early_suspend.resume	= ft5x0x_ts_resume;
	register_early_suspend(&ft5x0x_ts->early_suspend);
#endif
//wake the CTPM
//	__gpio_as_output(GPIO_FT5X0X_WAKE);		
//	__gpio_clear_pin(GPIO_FT5X0X_WAKE);		//set wake = 0,base on system
//	 msleep(100);
//	__gpio_set_pin(GPIO_FT5X0X_WAKE);			//set wake = 1,base on system
//	msleep(100);
//	ft5x0x_set_reg(0x88, 0x05); //5, 6,7,8
//	ft5x0x_set_reg(0x80, 30);
//	msleep(50);
    enable_irq(g_dev->irq);

    return 0;

exit_input_register_device_failed:
	input_free_device(input_dev);
exit_input_dev_alloc_failed:
	free_irq(client->irq, ft5x0x_ts);
	//free_irq(IRQ_EINT(6), ft5x0x_ts);
exit_irq_request_failed:
exit_platform_data_null:
	cancel_work_sync(&ft5x0x_ts->pen_event_work);
	destroy_workqueue(ft5x0x_ts->ts_workqueue);
exit_create_singlethread:
	printk("==singlethread error =\n");
	i2c_set_clientdata(client, NULL);
	kfree(ft5x0x_ts);
exit_alloc_data_failed:
exit_check_functionality_failed:
	return err;
}

static int __devexit ft5x0x_ts_remove(struct i2c_client *client)
{
	struct ft5x0x_ts_dev *ft5x0x_ts = i2c_get_clientdata(client);
	unregister_early_suspend(&ft5x0x_ts->early_suspend);
	free_irq(client->irq, ft5x0x_ts);
	input_unregister_device(ft5x0x_ts->input_dev);
	kfree(ft5x0x_ts);
	cancel_work_sync(&ft5x0x_ts->pen_event_work);
	destroy_workqueue(ft5x0x_ts->ts_workqueue);
	i2c_set_clientdata(client, NULL);
	return 0;
}

static const struct i2c_device_id ft5x0x_ts_id[] = {
	{ FT5X0X_NAME, 0 },{ }
};
MODULE_DEVICE_TABLE(i2c, ft5x0x_ts_id);

static struct i2c_driver ft5x0x_ts_driver = {
	.probe		= ft5x0x_ts_probe,
	.remove		= __devexit_p(ft5x0x_ts_remove),
	.id_table	= ft5x0x_ts_id,
	.driver	= {
		.name	= FT5X0X_NAME,
		.owner	= THIS_MODULE,
	},
};

static int __init ft5x0x_ts_init(void)
{
	return i2c_add_driver(&ft5x0x_ts_driver);
}

static void __exit ft5x0x_ts_exit(void)
{
	i2c_del_driver(&ft5x0x_ts_driver);
}

module_init(ft5x0x_ts_init);
module_exit(ft5x0x_ts_exit);

MODULE_AUTHOR("<wenfs@Focaltech-systems.com>");
MODULE_DESCRIPTION("FocalTech ft5x0x TouchScreen driver");
MODULE_LICENSE("GPL");

