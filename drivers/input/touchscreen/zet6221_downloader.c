#include <linux/kernel.h>
#include <linux/i2c.h>
#include <linux/delay.h>
#include <linux/gpio.h>
#include "zet6221_fw.h"
                                                   
#define ZET6221_DOWNLOADER_NAME "zet6221_downloader"

#define RSTPIN_ENABLE       1                    
                       
#define GPIO_LOW 	0
#define GPIO_HIGH 	1
                                                
static u8 fw_version0;
static u8 fw_version1;
                                                                       
#define debug_mode 1
#define DPRINTK(fmt,args...)	do { if (debug_mode) printk(KERN_EMERG "[%s][%d] "fmt"\n", __FUNCTION__, __LINE__, ##args);} while(0)

static unsigned char zeitec_zet6221_page[130] __initdata;
static unsigned char zeitec_zet6221_page_in[130] __initdata;
//static u16 fb[8] = {0x3EEA,0x3EED,0x3EF0,0x3EF3,0x3EF6,0x3EF9,0x3EFC,0x3EFF};
static u16 fb[8] = {0x3DF1,0x3DF4,0x3DF7,0x3DFA,0x3EF6,0x3EF9,0x3EFC,0x3EFF};

extern s32 zet6221_i2c_write_tsdata(struct i2c_client *client, u8 *data, u8 length);
extern s32 zet6221_i2c_read_tsdata(struct i2c_client *client, u8 *data, u8 length);
extern u8 pc[];

//#define I2C_CTPM_ADDRESS        (0x76)

/***********************************************************************
[function]: 
    callback: write data to ctpm by i2c interface,implemented by special user;
[parameters]:
 	client[in]			:i2c client structure;
    bt_ctpm_addr[in]    :the address of the ctpm;
    pbt_buf[in]         :data buffer;
    dw_lenth[in]        :the length of the data buffer;
[return]:
    1    :success;
    0    :fail;
************************************************************************/
int i2c_write_interface(struct i2c_client *client, u8 bt_ctpm_addr, u8* pbt_buf, u16 dw_lenth)
{
	struct i2c_msg msg;
	msg.addr = bt_ctpm_addr;
	msg.flags = 0;
	msg.len = dw_lenth;
	msg.buf = pbt_buf;
	msg.scl_rate = 300*1000;
	return i2c_transfer(client->adapter,&msg, 1);
}

/***********************************************************************
[function]: 
    callback: read data from ctpm by i2c interface,implemented by special user;
[parameters]:
 	client[in]			:i2c client structure;
    bt_ctpm_addr[in]    :the address of the ctpm;
    pbt_buf[out]        :data buffer;
    dw_lenth[in]        :the length of the data buffer;
[return]:
    1    :success;
    0    :fail;
************************************************************************/
int i2c_read_interface(struct i2c_client *client, u8 bt_ctpm_addr, u8* pbt_buf, u16 dw_lenth)
{
	struct i2c_msg msg;
	msg.addr = bt_ctpm_addr;
	msg.flags = I2C_M_RD;
	msg.len = dw_lenth;
	msg.buf = pbt_buf;
	msg.scl_rate = 300*1000;
	return i2c_transfer(client->adapter,&msg, 1);
}

/***********************************************************************
    [function]: 
		        callback: check version;
    [parameters]:
    			void

    [return]:
			    0: different 1: same;
************************************************************************/
u8 zet6221_ts_version()
{	
	int i;
	
#if 1
	printk("pc: ");
	for(i=0;i<8;i++)
		printk("%02x ",pc[i]);
	printk("\n");
	
	printk("src: ");
	for(i=0;i<8;i++)
		printk("%02x ",zeitec_zet6221_firmware[fb[i]]);
	printk("\n");
#endif
	
	for(i=0;i<8;i++)
		if(pc[i]!=zeitec_zet6221_firmware[fb[i]])
			return 0;
			
	return 1;
}

/***********************************************************************
    [function]: 
		        callback: send password;
    [parameters]:
    			client[in]:  struct i2c_client â€?represent an I2C slave device;

    [return]:
			    1;
************************************************************************/
u8 zet6221_ts_sndpwd(struct i2c_client *client)
{
	u8 ts_sndpwd_cmd[3] = {0x20,0xC5,0x9D};
	
	int ret;

#if defined(I2C_CTPM_ADDRESS)
	ret=i2c_write_interface(client, I2C_CTPM_ADDRESS, ts_sndpwd_cmd, 3);
#else
	ret=zet6221_i2c_write_tsdata(client, ts_sndpwd_cmd, 3);
#endif
	
	return ret;
}

/***********************************************************************
    [function]: 
		        callback: set/check sfr information;
    [parameters]:
    			client[in]:  struct i2c_client â€?represent an I2C slave device;

    [return]:
			    1;
************************************************************************/
u8 zet6221_ts_sfr(struct i2c_client *client)
{
	u8 ts_cmd[1] = {0x2C};
	u8 ts_in_data[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	u8 ts_cmd17[17] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	u8 ts_sfr_data[16] = {0x18,0x76,0x27,0x27,0xFF,0x03,0x8E,0x14,0x00,0x38,0x82,0xEC,0x00,0x00,0x7d,0x03};
	int ret;
	int i;
	
	printk("\nsfr cmd : "); 
#if defined(I2C_CTPM_ADDRESS)
	ret=i2c_write_interface(client, I2C_CTPM_ADDRESS, ts_cmd, 1);
#else
	ret=zet6221_i2c_write_tsdata(client, ts_cmd, 1);
#endif
	msleep(1);

	printk("%02x \nsfr rcv : ",ts_cmd[0]); 
	
#if defined(I2C_CTPM_ADDRESS)
	ret=i2c_read_interface(client, I2C_CTPM_ADDRESS, ts_in_data, 16);
#else
	ret=zet6221_i2c_read_tsdata(client, ts_in_data, 16);
#endif
	msleep(1);

	for(i=0;i<16;i++)
	{
		ts_cmd17[i+1]=ts_in_data[i];
		printk("%02x ",ts_in_data[i]); 
		
#if 1
		if(i>1 && i<8)
		{
			if(ts_in_data[i]!=ts_sfr_data[i])
				return 0;
		}
#endif

	}
	printk("\n"); 

	if(ts_in_data[14]!=0x3D)
	{
		ts_cmd17[15]=0x3D;
		
		ts_cmd17[0]=0x2B;	
		
#if defined(I2C_CTPM_ADDRESS)
		ret=i2c_write_interface(client, I2C_CTPM_ADDRESS, ts_cmd17, 17);
#else
		ret=zet6221_i2c_write_tsdata(client, ts_cmd17, 17);
#endif

		if(ret<0)
		{
			printk("enable sfr(0x3D) failed!\n"); 
			return 0;
		}

	}
	
	return 1;
}

/***********************************************************************
    [function]: 
		        callback: mass erase flash;
    [parameters]:
    			client[in]:  struct i2c_client â€?represent an I2C slave device;

    [return]:
			    1;
************************************************************************/
u8 zet6221_ts_masserase(struct i2c_client *client)
{
	u8 ts_cmd[1] = {0x24};
	
	int ret;

#if defined(I2C_CTPM_ADDRESS)
	ret=i2c_write_interface(client, I2C_CTPM_ADDRESS, ts_cmd, 1);
#else
	ret=zet6221_i2c_write_tsdata(client, ts_cmd, 1);
#endif
	
	return 1;
}

/***********************************************************************
    [function]: 
		        callback: erase flash by page;
    [parameters]:
    			client[in]:  struct i2c_client â€?represent an I2C slave device;

    [return]:
			    1;
************************************************************************/
u8 zet6221_ts_pageerase(struct i2c_client *client,int npage)
{
	u8 ts_cmd[2] = {0x23,0x00};
	
	int ret;

	ts_cmd[1]=npage;
#if defined(I2C_CTPM_ADDRESS)
	ret=i2c_write_interface(client, I2C_CTPM_ADDRESS, ts_cmd, 2);
#else
	ret=zet6221_i2c_write_tsdata(client, ts_cmd, 2);
#endif
	
	return 1;
}

/***********************************************************************
    [function]: 
		        callback: reset mcu;
    [parameters]:
    			client[in]:  struct i2c_client â€?represent an I2C slave device;

    [return]:
			    1;
************************************************************************/
u8 zet6221_ts_resetmcu(struct i2c_client *client)
{
	u8 ts_cmd[1] = {0x29};
	
	int ret;

#if defined(I2C_CTPM_ADDRESS)
	ret=i2c_write_interface(client, I2C_CTPM_ADDRESS, ts_cmd, 1);
#else
	ret=zet6221_i2c_write_tsdata(client, ts_cmd, 1);
#endif
	
	return 1;
}

/***********************************************************************
    [function]: 
		        callback: start HW function;
    [parameters]:
    			client[in]:  struct i2c_client â€?represent an I2C slave device;

    [return]:
			    1;
************************************************************************/
u8 zet6221_ts_hwcmd(struct i2c_client *client)
{
	u8 ts_cmd[1] = {0xB9};
	
	int ret;

#if defined(I2C_CTPM_ADDRESS)
	ret=i2c_write_interface(client, I2C_CTPM_ADDRESS, ts_cmd, 1);
#else
	ret=zet6221_i2c_write_tsdata(client, ts_cmd, 1);
#endif
	
	return 1;
}

/***********************************************************************
update FW
************************************************************************/
int __init zet6221_downloader( struct i2c_client *client )
{
	int BufLen=0;
	int BufPage=0;
	int BufIndex=0;
	int ret;
	int i;
	
	int nowBufLen=0;
	int nowBufPage=0;
	int nowBufIndex=0;
	int retryCount=0;
	
begin_download:
	
#if defined(RSTPIN_ENABLE)
	//reset mcu
	gpio_direction_output(TS_RST_GPIO, GPIO_LOW);
	msleep(5);
#else
	zet6221_ts_hwcmd(client);
	msleep(200);
#endif
	//send password
	//send password
	ret=zet6221_ts_sndpwd(client);
	msleep(100);
	
	if(ret<=0)
		return ret;
	
/*****compare version*******/

	//0~3
	memset(zeitec_zet6221_page_in,0x00,130);
	zeitec_zet6221_page_in[0]=0x25;
	zeitec_zet6221_page_in[1]=(fb[0] >> 7);      //(fb[0]/128);
#if defined(I2C_CTPM_ADDRESS)
		ret=i2c_write_interface(client, I2C_CTPM_ADDRESS, zeitec_zet6221_page_in, 2);
#else
		ret=zet6221_i2c_write_tsdata(client, zeitec_zet6221_page_in, 2);
#endif

	if(ret<=0)
		return ret;	
	
	zeitec_zet6221_page_in[0]=0x0;
	zeitec_zet6221_page_in[1]=0x0;
#if defined(I2C_CTPM_ADDRESS)
	ret=i2c_read_interface(client, I2C_CTPM_ADDRESS, zeitec_zet6221_page_in, 128);
#else
	ret=zet6221_i2c_read_tsdata(client, zeitec_zet6221_page_in, 128);
#endif

	if(ret<=0)
		return ret;
		
	//printk("page=%d ",(fb[0] >> 7));             //(fb[0]/128));
	for(i=0;i<4;i++)
	{
		pc[i]=zeitec_zet6221_page_in[(fb[i] & 0x7f)];     //[(fb[i]%128)];
		//printk("offset[%d]=%d ",i,(fb[i] & 0x7f));        //(fb[i]%128));
	}
	//printk("\n");
	
	
	//4~7
	memset(zeitec_zet6221_page_in,0x00,130);
	zeitec_zet6221_page_in[0]=0x25;
	zeitec_zet6221_page_in[1]=(fb[4] >> 7);			//(fb[4]/128);
#if defined(I2C_CTPM_ADDRESS)
	ret=i2c_write_interface(client, I2C_CTPM_ADDRESS, zeitec_zet6221_page_in, 2);
#else
	ret=zet6221_i2c_write_tsdata(client, zeitec_zet6221_page_in, 2);
#endif

	if(ret<=0)
		return ret;
	
	zeitec_zet6221_page_in[0]=0x0;
	zeitec_zet6221_page_in[1]=0x0;
#if defined(I2C_CTPM_ADDRESS)
		ret=i2c_read_interface(client, I2C_CTPM_ADDRESS, zeitec_zet6221_page_in, 128);
#else
		ret=zet6221_i2c_read_tsdata(client, zeitec_zet6221_page_in, 128);
#endif

	if(ret<=0)
		return ret;

	//printk("page=%d ",(fb[4] >> 7)); //(fb[4]/128));
	for(i=4;i<8;i++)
	{
		pc[i]=zeitec_zet6221_page_in[(fb[i] & 0x7f)]; 		//[(fb[i]%128)];
		//printk("offset[%d]=%d ",i,(fb[i] & 0x7f));  		//(fb[i]%128));
	}
	//printk("\n");
	
	//page 127
	memset(zeitec_zet6221_page_in,0x00,130);
	zeitec_zet6221_page_in[0]=0x25;
	zeitec_zet6221_page_in[1]=127;
#if defined(I2C_CTPM_ADDRESS)
	ret=i2c_write_interface(client, I2C_CTPM_ADDRESS, zeitec_zet6221_page_in, 2);
#else
	ret=zet6221_i2c_write_tsdata(client, zeitec_zet6221_page_in, 2);
#endif

	if(ret<=0)
		return ret;	
	
	zeitec_zet6221_page_in[0]=0x0;
	zeitec_zet6221_page_in[1]=0x0;
#if defined(I2C_CTPM_ADDRESS)
		ret=i2c_read_interface(client, I2C_CTPM_ADDRESS, zeitec_zet6221_page_in, 128);
#else
		ret=zet6221_i2c_read_tsdata(client, zeitec_zet6221_page_in, 128);
#endif

	if(ret<=0)
		return ret;

	for(i=0;i<128;i++)
	{
		//printk( "=======================begin download========================\n");
		if(0x3F80+i < sizeof(zeitec_zet6221_firmware)/sizeof(char))
		{
			if(zeitec_zet6221_page_in[i]!=zeitec_zet6221_firmware[0x3F80+i])
			{
				printk("page 127 [%d] doesn't match! continue to download!\n",i);
				goto proc_sfr;
			}
		}
	}
	printk( "zet6221 version \n");
	if(zet6221_ts_version()!=0)
		goto exit_download;
		
/*****compare version*******/

proc_sfr:
	//sfr
	if(zet6221_ts_sfr(client)==0)
	{
		printk( "=======================proc_sfr========================\n");
		//comment out to disable sfr checking loop
		//return 0;

#if 1

#if defined(RSTPIN_ENABLE)
	
		gpio_direction_output(TS_RST_GPIO, GPIO_HIGH);
		msleep(20);
		
		gpio_direction_output(TS_RST_GPIO, GPIO_LOW);
		msleep(20);
		
		gpio_direction_output(TS_RST_GPIO, GPIO_HIGH);
#else
		zet6221_ts_resetmcu(client);
#endif	
		msleep(20);
		printk( "=======================begin download========================\n");
		goto begin_download;
		
#endif

	}
	msleep(20);
	
	//comment out to enable download procedure
	//return 1;	

	//erase
	if(BufLen==0)
	{
		//mass erase
		//DPRINTK( "mass erase\n");
		zet6221_ts_masserase(client);
		msleep(200);

		BufLen=sizeof(zeitec_zet6221_firmware)/sizeof(char);
	}else
	{
		zet6221_ts_pageerase(client,BufPage);
		msleep(200);
	}

	
	while(BufLen>0)
	{
download_page:

		memset(zeitec_zet6221_page,0x00,130);
		
		DPRINTK( "Start: write page%d\n",BufPage);
		nowBufIndex=BufIndex;
		nowBufLen=BufLen;
		nowBufPage=BufPage;
		
		if(BufLen>128)
		{
			for(i=0;i<128;i++)
			{
				zeitec_zet6221_page[i+2]=zeitec_zet6221_firmware[BufIndex];
				BufIndex+=1;
			}
			zeitec_zet6221_page[0]=0x22;
			zeitec_zet6221_page[1]=BufPage;
			BufLen-=128;
		}
		else
		{
			for(i=0;i<BufLen;i++)
			{
				zeitec_zet6221_page[i+2]=zeitec_zet6221_firmware[BufIndex];
				BufIndex+=1;
			}
			zeitec_zet6221_page[0]=0x22;
			zeitec_zet6221_page[1]=BufPage;
			BufLen=0;
		}
		
#if defined(I2C_CTPM_ADDRESS)
		ret=i2c_write_interface(client, I2C_CTPM_ADDRESS, zeitec_zet6221_page, 130);
#else
		ret=zet6221_i2c_write_tsdata(client, zeitec_zet6221_page, 130);
#endif
		msleep(200);
		
#if 1

		memset(zeitec_zet6221_page_in,0x00,130);
		zeitec_zet6221_page_in[0]=0x25;
		zeitec_zet6221_page_in[1]=BufPage;
#if defined(I2C_CTPM_ADDRESS)
		ret=i2c_write_interface(client, I2C_CTPM_ADDRESS, zeitec_zet6221_page_in, 2);
#else
		ret=zet6221_i2c_write_tsdata(client, zeitec_zet6221_page_in, 2);
#endif
	
		zeitec_zet6221_page_in[0]=0x0;
		zeitec_zet6221_page_in[1]=0x0;
#if defined(I2C_CTPM_ADDRESS)
		ret=i2c_read_interface(client, I2C_CTPM_ADDRESS, zeitec_zet6221_page_in, 128);
#else
		ret=zet6221_i2c_read_tsdata(client, zeitec_zet6221_page_in, 128);
#endif       
		                          
		for(i=0;i<128;i++)
		{
			if(i < nowBufLen)
			{
				if(zeitec_zet6221_page[i+2]!=zeitec_zet6221_page_in[i])
				{
					BufIndex=nowBufIndex;
					BufLen=nowBufLen;
					BufPage=nowBufPage;
				
					if(retryCount < 5)
					{
						retryCount++;
						goto download_page;
					}else
					{
						//BufIndex=0;
						//BufLen=0;
						//BufPage=0;
						retryCount=0;
						
#if defined(RSTPIN_ENABLE)
						gpio_direction_output(TS_RST_GPIO, GPIO_HIGH);
						msleep(20);
		
						gpio_direction_output(TS_RST_GPIO, GPIO_LOW);
						msleep(20);
		
						gpio_direction_output(TS_RST_GPIO, GPIO_HIGH);
#else
						zet6221_ts_resetmcu(client);
#endif	
						msleep(20);
						goto begin_download;
					}

				}
			}
		}
		
#endif
		retryCount=0;
		BufPage+=1;
	}
	
exit_download:

	

#if defined(RSTPIN_ENABLE)
	printk("exit_download\n");
	gpio_direction_output(TS_RST_GPIO, GPIO_HIGH);
	printk("exit_download1\n");
	msleep(100);
	printk("exit_download2\n");
#endif

	zet6221_ts_resetmcu(client);
	msleep(100);
	
}
