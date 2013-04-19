/* drivers/input/touchscreen/gt801plus_819_8105_8110_update.c
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
 * Version:1.0
 *        V1.0:2012/05/01,create file.
 *
 */
#include <linux/kthread.h>
#include "gt801plus_819_8105_8110.h"

static s8* gt81x_fw;
static s8* gt81x_config_info;
static s8* fw_path = NULL;
static s8* cfg_path = NULL;
u16 show_len;
u16 total_len;
u32 oldcrc32 = 0xFFFFFFFF;
u32 crc32_table[256];
u32 ulPolynomial = 0x04c11db7;

static s32 gtp_update_fw(void* arg);

/*******************************************************	
Function:
	The entrance of GTP update function.

Input:
	data:the data to update.
	
Output:
	Executive outcomes.1---succeed.
*********************************************************/
s32 gup_enter_update_mode(struct i2c_client * client)
{
    return 0;
}
void gup_leave_update_mode(void)
{
}
s32 gup_update_proc(void* data)
{
    struct goodix_ts_data *ts;
    struct task_struct *thread = NULL;	
    s8 *cmd;
    u8 fw_path_len = 0;
    u8 cfg_path_len = 0;
     
    GTP_DEBUG_FUNC();

    cmd = (s8*)data;
    ts = i2c_get_clientdata(i2c_connect_client);
    if (ts->enter_update)
    {
        GTP_ERROR("Updating,Exit Update.");
        return 0;
    }
  
    if (ts==NULL)
    {
        return 0;
    }
    
    if(data == NULL)
    {
        GTP_ERROR("Null path,Exit Update.");
        return 0;
    }	
    GTP_INFO("Begin update......");
    
    fw_path_len = strlen(data);
    fw_path = (s8*)vmalloc(fw_path_len);
    memcpy(fw_path, data, fw_path_len+1);
    
    cfg_path_len = strlen(data+fw_path_len+1);
    if (cfg_path_len)
    {
        cfg_path = (s8*)vmalloc(cfg_path_len);
        memcpy(cfg_path, data+fw_path_len+1, cfg_path_len);
    }
    
    GTP_DEBUG("fw_path:%s, fw_len:%d", fw_path, fw_path_len);
    GTP_DEBUG("cfg_path:%s, cfg_len:%d", cfg_path, cfg_path_len);
    
    total_len = 0;
    show_len = 0;
    thread = kthread_run(gtp_update_fw, NULL, "guitar_update");
    return 1;
}

/*******************************************************	
Function:
	Open file function.

Input:
	path:the file path.
	old_fs_p:old fs point.
	
Output:
	File point.
*********************************************************/
static struct file * gtp_file_open(u8 * path, mm_segment_t * old_fs_p)
{
    s32 errno = -1;
    struct file *filp = NULL;
		
    filp = filp_open(path, O_RDONLY, 0644);
    if (!filp || IS_ERR(filp))
    {
        if (!filp)
        {
            errno = -ENOENT;
        }
        else
        { 
            errno = PTR_ERR(filp);
        }					
        GTP_ERROR("Open update file error.");
        return NULL;
    }
    *old_fs_p = get_fs();
    set_fs(get_ds());

    filp->f_op->llseek(filp,0,0);
    return filp ;
}

/*******************************************************	
Function:
	Close file function.

Input:
	filp:the file point.
	old_fs_p:old fs point.
	
Output:
	None.
*********************************************************/
static void gtp_file_close(struct file * filp, mm_segment_t old_fs)
{
    set_fs(old_fs);
    if (filp)
    {
        filp_close(filp, NULL);
    }
}

/*******************************************************	
Function:
	Get file length function.

Input:
	path:the file path.
	
Output:
	File length.
*********************************************************/
static int gtp_get_file_length(char * path)
{
    struct file * file_ck = NULL;
    mm_segment_t old_fs;
    s32 length ;
	
    file_ck = gtp_file_open(path, &old_fs);
    if (file_ck == NULL)
    {
        return 0;
    }

    length = file_ck->f_op->llseek(file_ck, 0, SEEK_END);
    if (length < 0)
    {
        length = 0;
    }
    gtp_file_close(file_ck, old_fs);
    return length;	
}

/*******************************************************	
Function:
	Get file data function.

Input:
	path:the file path.
	
Output:
	Executive outcomes.1---succeed.
*********************************************************/
static s32 gtp_get_file_data(s8 *path)
{
    s32 fw_len;
    s32 ret;
    struct file * file_ck = NULL;
    mm_segment_t old_fs;
	
    fw_len = gtp_get_file_length(path);
    gt81x_fw = (char*)vmalloc(fw_len);
    file_ck = gtp_file_open(path, &old_fs);
	
    if (path != NULL)
    {
        GTP_DEBUG("Update file path:%s", path);
    }
	
    if (file_ck == NULL)
    {
        return -ERROR_NO_FILE;
    }
		
    if (fw_len <= 0 || (fw_len%4) != 0)
    {
        gtp_file_close(file_ck, old_fs);
        return -ERROR_FILE_TYPE;
    }
	
    //set file point to the begining of the file
    file_ck->f_op->llseek(file_ck, 0, SEEK_SET);
    ret = file_ck->f_op->read(file_ck, gt81x_fw, fw_len, &file_ck->f_pos);
    if (ret<0)
    {
        gtp_file_close(file_ck, old_fs);
        return -ERROR_FILE_READ;
    }
	
    gtp_file_close(file_ck, old_fs);
    return 1;    
}

/*******************************************************	
Function:
	GTP i2c read regs.

Input:
	client:i2c client.
	reg:GTP register.
	buf:buffer to store read data
	len:read length.
	
Output:
	Executive outcomes.
*********************************************************/
static s32 gtp_read_regs(struct i2c_client *client, u8 reg, u8 buf[], unsigned len)
{
    struct i2c_msg msgs[2];
    s32 ret=-1;
    s32 retries = 0;
	
    msgs[0].flags=!I2C_M_RD;
    msgs[0].addr=client->addr;
    msgs[0].len=1;
    msgs[0].buf=&reg;

    msgs[1].flags=I2C_M_RD;
    msgs[1].addr=client->addr;
    msgs[1].len=len;
    msgs[1].buf=buf;

    while(retries<5)
    {
        ret=i2c_transfer(client->adapter,msgs, 2);
        if (ret == 2)
        {
            return ret;
        }
        retries++;
    }
    return -1;
}
/*******************************************************	
Function:
	GTP i2c set regs.

Input:
	client:i2c client.
	reg:GTP register.
	buf:data buffer to read.
	len:read length.
	
Output:
	Executive outcomes.
*********************************************************/
static s32 gtp_set_regs(struct i2c_client *client, u8 reg, u8 const buf[],u32 len)
{
    struct i2c_msg msg;
    s32 ret=-1;
    s32 retries = 0;
    u8 *data;
    data = (u8*)vmalloc(len+1);
	
    data[0] = reg;
    for (retries=0;retries<len;retries++)
    {
        data[1+retries] = buf[retries];
    }

    msg.flags=!I2C_M_RD;
    msg.addr=client->addr;
    msg.len=len+1;
    msg.buf=data;		

    retries = 0;	
    while(retries<5)
    {
        ret=i2c_transfer(client->adapter,&msg, 1);
        if (ret == 1)
        {
            return ret;
        }
        retries++;
    }
    vfree(data);
    return -1;
}

/*******************************************************	
Function:
	Wait GTP ready function.

Input:
	client:i2c client.
	status:which status to wait.
	
Output:
	Executive outcomes.
*********************************************************/
static s32 gtp_wait_for_slave(struct i2c_client *client, u8 status)
{
    u8 i2c_state_buf[2];
    s32 ret = -1;
    s32 i = 0;
    while(i++ < MAX_I2C_RETRIES)
    {
        ret = gtp_read_regs(client,ADDR_STA, i2c_state_buf, 1);
        GTP_DEBUG("i2c read state byte:0x%x",i2c_state_buf[0]);
        if (ret < 0)
        {
            return ERROR_I2C_TRANSFER;
        }
        if (i2c_state_buf[0]==0xff)
        {
            continue;
        }
        if (i2c_state_buf[0] & status)
        {
            return i2c_state_buf[0];
        }
        msleep(10);
    }
    return -ERROR_TIMEOUT;
}

/*******************************************************	
Function:
	Hex text transfer to int.

Input:
	c:source.
	
Output:
	Transfer result.
*********************************************************/
static s32 hexToInt(s8 c) 
{
    s32 x = (s32)c;
    if (x >= 'a' && x <= 'f')
    {
        x -= 'a'-10;
    }
    else if (x >= 'A' && x <= 'F')
    {
        x -= 'A'-10;
    }
    else if (x >= '0' && x <= '9')
    {
        x -= '0';
    }
    else
    {
        x = -1;
    }
    return x;
}

/*******************************************************	
Function:
	Write config after update.

Input:
	client:i2c client.
	cfg_path:config file path.
	
Output:
	Executive outcomes.0！！success.
*********************************************************/
int gtp_update_write_config(struct i2c_client *client, char* cfg_path)
{
    s32 i;
    s32 ret = -1;
    s32 len = 0;
    u8 cfg_rd_buf[200];
    u8 cfg_cmd_buf = 0x03;
    u8 retries = 0;
    u8 *config_info = NULL;
    s32 high,low;
     struct file * file_ck = NULL;
    mm_segment_t old_fs;
	
    file_ck = gtp_file_open(cfg_path, &old_fs);
    if (file_ck == NULL)
    {
        return 0;
    }
    len = file_ck->f_op->llseek(file_ck, 0, SEEK_END);
    gt81x_config_info = (char*)vmalloc(len);
    config_info = (char*)vmalloc(len/5);
	
    file_ck->f_op->llseek(file_ck, 0, SEEK_SET);
    ret = file_ck->f_op->read(file_ck, gt81x_config_info, len, &file_ck->f_pos);
    if (ret<0)
    {
        gtp_file_close(file_ck, old_fs);
        return -ERROR_FILE_READ;
    }
	
    gtp_file_close(file_ck, old_fs);
	
    for (i=0; i<len; i+=5)
    {
        high = hexToInt(gt81x_config_info[i+2]);
        low = hexToInt(gt81x_config_info[i+3]);
        config_info[i/5] = (char)(high*16+low);
    }	
reconfig:
    len=len/5;
    GTP_DEBUG("Config length:%d", len);
    GTP_DEBUG_ARRAY(config_info, len);	
    ret = gtp_set_regs(client, 101, &config_info[1], len-1);
    if (ret < 0)
    {
        GTP_ERROR("send config info failed!");
        return ret;
    }
	
    ret = gtp_read_regs(client, 101, cfg_rd_buf, len-1);
    if (ret < 0)
    {
        return ret;
    }
    GTP_DEBUG_ARRAY(cfg_rd_buf, len);
    if (memcmp(cfg_rd_buf, &config_info[1], len-1))
    {	
        GTP_ERROR("config info check error!");
        if (retries < 5)
        {
            retries++;
            ret = gtp_set_regs(client, ADDR_CMD, &cfg_cmd_buf, 1);
            if (ret < 0)
            {
                return ret;
            }
            goto reconfig;
        }
        return -1;
    }
    cfg_cmd_buf = 0x04;
    ret = gtp_set_regs(client, ADDR_CMD, &cfg_cmd_buf, 1);
    if (ret < 0)
    {
        return ret;
    }
	
    vfree(config_info);
    vfree(gt81x_config_info);
    return 0;
}

//////////////////////////////////////////////////////////////////////////////
/**
@brief CRC cal proc,include : Reflect,init_crc32_table,GenerateCRC32
@param global var oldcrc32
@return states
*/
static u32 Reflect(u64 ref, s8 ch)
{
	u32 value=0;
	s32 i;
	for(i = 1; i < (ch + 1); i++)
	{
		if(ref & 1)
			value |= 1 << (ch - i);
		ref >>= 1;
	}
	return value;
}
/*---------------------------------------------------------------------------------------------------------*/
/*  CRC Check Program INIT								                                           		   */
/*---------------------------------------------------------------------------------------------------------*/
static void init_crc32_table(void)
{
	u32 temp;
	u32 t1,t2;
	u32 flag;
	s32 i,j;
	for(i = 0; i <= 0xFF; i++)
	{
		temp=Reflect(i, 8);
		crc32_table[i]= temp<< 24;
		for (j = 0; j < 8; j++)
		{

			flag=crc32_table[i]&0x80000000;
			t1=(crc32_table[i] << 1);
			if(flag==0)
				t2=0;
			else
				t2=ulPolynomial;
			crc32_table[i] =t1^t2 ;

		}
		crc32_table[i] = Reflect(crc32_table[i], 32);
	}
}
/*---------------------------------------------------------------------------------------------------------*/
/*  CRC main Program									                                           		   */
/*---------------------------------------------------------------------------------------------------------*/
static void GenerateCRC32(u8 * buf, u32 len)
{
	u32 i;
	u32 t;

	for (i = 0; i != len; ++i)
	{
		t = (oldcrc32 ^ buf[i]) & 0xFF;
		oldcrc32 = ((oldcrc32 >> 8) & 0xFFFFFF) ^ crc32_table[t];
	}
}

static s32 gtp_file_crc(s8 * path)
{
	u8 buffer[64] = { 0 } ;
	struct file * file_ck = NULL;
	mm_segment_t old_fs;
	s32 count, ret, length ;
	
	file_ck = gtp_file_open(path, &old_fs);
	
	if(path != NULL)
		printk("File Path:%s\n", path);
	
	if(file_ck == NULL)
		return -ERROR_NO_FILE;

	length = file_ck->f_op->llseek(file_ck, 0, SEEK_END);
	if(length <= 0 || (length%4) != 0)
	{
		gtp_file_close(file_ck, old_fs);
		return -ERROR_FILE_TYPE;
	}
	
	//set file point to the begining of the file
	file_ck->f_op->llseek(file_ck, 0, SEEK_SET);	
	oldcrc32 = 0xFFFFFFFF;
	init_crc32_table();
	while(length > 0)
	{
		ret = file_ck->f_op->read(file_ck, buffer, sizeof(buffer), &file_ck->f_pos);
		if(ret > 0)
		{
			for(count = 0; count < ret;  count++) 	
				GenerateCRC32(&buffer[count],1);			
		}
		else 
		{
			gtp_file_close(file_ck, old_fs);
			return -ERROR_FILE_READ;
		}
		length -= ret;
	}
	oldcrc32 = ~oldcrc32;

	GTP_DEBUG("CRC_Check: %u\n", oldcrc32);
		
	gtp_file_close(file_ck, old_fs);
	
	return 1;	
}

s32 gtp_update_write_fw_old(struct i2c_client *client, s8 *file_data, s32 file_len)
{
    u8 *pos = NULL;
    u32 frame_number = 0;
    u32 rd_len = 0;
    s32 ret = -1;
    u8 i2c_data_buf[PACK_SIZE+8];
    u8 send_crc = 0;
    
    total_len = file_len;
    if(file_data == NULL)
    {
        return -1;
    }
    while((file_len >= 0) && (!send_crc))
    {
        GTP_INFO("[OLD_REMAIN_LEN]:%d", file_len);
        show_len = total_len - file_len;
        rd_len = (file_len >= PACK_SIZE) ? PACK_SIZE : file_len;
        if(file_len)
        {
            pos = &file_data[PACK_SIZE*frame_number];
            memcpy(i2c_data_buf, pos, rd_len);
        }
        if(file_len < PACK_SIZE)
        {
            GTP_INFO("Update send crc.");
            send_crc = 1;
            i2c_data_buf[file_len+0] = oldcrc32&0xff;
            i2c_data_buf[file_len+1] = (oldcrc32>>8)&0xff;
            i2c_data_buf[file_len+2] = (oldcrc32>>16)&0xff;
            i2c_data_buf[file_len+3] = (oldcrc32>>24)&0xff;
            ret = gtp_set_regs(client, ADDR_DAT_OLD, i2c_data_buf, file_len+4);
            if(ret < 0)
            {
                GTP_ERROR("Update i2c write failed.");
                return -1;
            }
            break;
        }
        else
        {
            ret = gtp_set_regs(client, ADDR_DAT_OLD, i2c_data_buf, rd_len);
            if(ret < 0)
            {
                GTP_ERROR("Update i2c write failed.");
                return -1;
            }
        }
        
        GTP_INFO("Update frame wait slave ready.");
        ret = gtp_wait_for_slave(client, SLAVE_READY);
        if (ret <= 0)
        {
            GTP_ERROR("Update frame wait slave ready failed.");
            return -1;
        }
        frame_number++;
        file_len -= rd_len;

    }//end of while((file_len >= 0) && (!send_crc))
    
    return 0;
}
/*******************************************************	
Function:
	Write firmware data to GTP chip.

Input:
	client:i2c client.
	fw_buf:firmware data buffer to write.
	
Output:
	Executive outcomes.0！！success.
*********************************************************/
s32 gtp_update_write_fw(struct i2c_client *client, char *fw_buf, int len)
{
    s32 ret,data_len,i,check_len,frame_checksum,frame_number = 0;
    u8 *p,i2c_data_buf[PACK_SIZE+8];
    u8 i2c_rd_buf[PACK_SIZE+8];
	
    u8 retries = 0;
    u8 check_state = 0;
    total_len = len;	
    if (!client || !fw_buf)
    {
        return -1;
    }

    while(len)
    {
        frame_checksum = 0;
        retries = 0;
        check_len = (len >= PACK_SIZE) ? PACK_SIZE : len;
        data_len = check_len+8;
        show_len = total_len-len;
        GTP_INFO("SHOW_LEN:%d,TOTAL:%d",show_len, total_len);
        //GTP_INFO("PACK[%d]:prepare data,remained len = %d",frame_number,len);
        p = &fw_buf[frame_number*PACK_SIZE];
        for (i=0; i<check_len; i++)
        {
            frame_checksum += *p++;
        }
        frame_checksum = 0 - frame_checksum;
        p = i2c_data_buf;
        *p++ = (frame_number>>24)&0xff;
        *p++ = (frame_number>>16)&0xff;
        *p++ = (frame_number>>8)&0xff;
        *p++ = frame_number&0xff;
        memcpy(p, &fw_buf[frame_number*PACK_SIZE],check_len);
        p += check_len;
        *p++ = frame_checksum&0xff;
        *p++ = (frame_checksum>>8)&0xff;
        *p++ = (frame_checksum>>16)&0xff;
        *p++ = (frame_checksum>>24)&0xff;
        GTP_DEBUG_ARRAY(i2c_data_buf, data_len);
        GTP_DEBUG("PACK[%d]:write to slave",frame_number);
resend:
        ret = gtp_set_regs(client,ADDR_DAT, i2c_data_buf, data_len);
        if (ret < 0)
        {
            return ret;
        }
        GTP_DEBUG_ARRAY(i2c_data_buf, data_len);
        msleep(10);
        GTP_DEBUG("PACK[%d]:read data",frame_number);
        memset(i2c_rd_buf, 0, sizeof(i2c_rd_buf));
        ret = gtp_read_regs(client,ADDR_DAT, i2c_rd_buf, data_len);
        if (ret < 0)
        {
            return ret;
        }
        GTP_DEBUG_ARRAY(i2c_data_buf, data_len);
        msleep(10);
        GTP_DEBUG("PACK[%d]:check data",frame_number);
        if (memcmp(&i2c_rd_buf[4],&fw_buf[frame_number*PACK_SIZE],check_len))
        {
            GTP_ERROR("PACK[%d]:File Data Frame readback check Error!",frame_number);
            i2c_rd_buf[0] = 0x03;
            ret = gtp_set_regs(client, ADDR_CMD, i2c_rd_buf, 1);
            if (ret < 0)
            {
                return ret;
            }
            check_state = 0x01;
        }
        else
        {
            GTP_DEBUG("PACK[%d]:tell slave check data pass",frame_number);
            i2c_rd_buf[0] = 0x04;
            ret = gtp_set_regs(client,ADDR_CMD, i2c_rd_buf, 1);
            if (ret < 0)
            {
                return ret;
            }
            GTP_DEBUG("PACK[%d]:wait for slave to start next frame",frame_number);
        }
		
        ret = gtp_wait_for_slave(client, SLAVE_READY);
        if ((ret & CHECKSUM_ERROR) || (ret & FRAME_ERROR) || (ret == ERROR_I2C_TRANSFER) || (ret < 0) || (check_state == 0x01))
        {
            if (((ret & CHECKSUM_ERROR) || (ret & FRAME_ERROR) || (check_state == 0x01))&&(retries < 5))
            {
                if (check_state != 0x01)
                {
                    GTP_ERROR("checksum error or miss frame error!");
                }
                check_state = 0x00;
                retries++;
                msleep(20);
                goto resend;
            }
            GTP_DEBUG("wait slave return state:%d", ret);
            show_len = total_len;
            return ret;
        }
        GTP_INFO("PACK[%d]:frame transfer finished",frame_number);
        if (len < PACK_SIZE)
        {
            return 0;
        }
        frame_number++;
        len -= check_len;
    }
    total_len = -1;
    show_len = total_len;
    return 0;
}
/*******************************************************	
Function:
	Update main function.

Input:
	arg:update argument.
	
Output:
	Executive outcomes.0！！success.
*********************************************************/
static s32 gtp_update_fw(void* arg)
{
    s32 ret = 0,file_len,update_need_config;
    u8 i2c_control_buf[10];
    s32 fw_len;
    const s8 version_base[17]={"GT81XNI"};
    struct goodix_ts_data* ts;
    struct i2c_client* client = i2c_connect_client;
    ts = i2c_get_clientdata(i2c_connect_client);
	
    GTP_INFO("Firmware update start...");
    fw_len = gtp_get_file_length(fw_path);
    ret = gtp_get_file_data(fw_path);
    if (ret != 1)
    {
        GTP_ERROR("Get update file data failed.");
        goto exit_update_fw;
    }
		
    if(ts->chip_type == GT801PLUS)
    {
        GTP_INFO("Step 0:CRC file");
        ret = gtp_file_crc(fw_path);
        if(ret <= 0)
        {
            GTP_ERROR("Update file crc failed.");
            goto exit_update_fw;
        }
    }
		
    gtp_wakeup_green(ts, 0);
    GTP_INFO("Step 1:read version...");
    ret = gtp_read_version(ts);
    if (ret)
    {
        GTP_ERROR("Read version failed!");
        goto exit_update_fw;
    }
    ts->enter_update = 1;
    GTP_INFO("Step 1 done!");
    GTP_INFO("Step 2:disable irq...");
    disable_irq(client->irq);
    GTP_INFO("Step 2 done!");
    GTP_INFO("Step 3:set update start...");
    //wakeup green mode
    GTP_GPIO_OUTPUT(GTP_INT_PORT, 0);
    msleep(5);
    GTP_GPIO_AS_INT(GTP_INT_PORT);
    i2c_control_buf[0] = UPDATE_START;
    ret = gtp_set_regs(client,ADDR_CMD, i2c_control_buf, 1);
    if (ret < 0)
    {
        GTP_ERROR("Update i2c write failed.");
        goto exit_update_fw;
    }
    //the time include time(APROM -> LDROM) and time(LDROM init)
    msleep(1000);
    GTP_INFO("Step 3 done!");
    GTP_INFO("Step 4:wait for slave start...");
    ret = gtp_wait_for_slave(client, UPDATE_START);
    if (ret < 0)
    {
        GTP_ERROR("Update wait slave failed.");
        goto exit_update_fw;
    }
    if (!(ret & UPDATE_START))
    {
        GTP_ERROR("Update enter start failed.");
        goto exit_update_fw;
    }
    if (!(ret & NEW_UPDATE_START))
    {
        update_need_config = 1;
    }
    GTP_INFO("Step 4 done!");
    GTP_INFO("Step 5:write the fw length...");
    file_len = fw_len + 4;
    GTP_DEBUG("Update file length is:%d", file_len);
    i2c_control_buf[0] = (file_len>>24) & 0xff;
    i2c_control_buf[1] = (file_len>>16) & 0xff;
    i2c_control_buf[2] = (file_len>>8) & 0xff;
    i2c_control_buf[3] = file_len & 0xff;
    ret = gtp_set_regs(client,ADDR_DAT, i2c_control_buf, 4);
    if (ret < 0)
    {
        GTP_ERROR("Update i2c write failed.");
        goto exit_update_fw;
    }
    GTP_INFO("Step 5 done!");
    GTP_INFO("Step 6:wait for slave ready");
    ret = gtp_wait_for_slave(client, SLAVE_READY);
    if (ret < 0)
    {
        GTP_ERROR("Update wait slave ready failed.");
        goto exit_update_fw;
    }
    GTP_INFO("Strp 6 done!");
    GTP_INFO("Step 7:write data...");
    if(ts->chip_type == GT801PLUS)
    {
        GTP_INFO("GT801PLUS write frame data.");
        ret = gtp_update_write_fw_old(client, gt81x_fw, fw_len);
        if (ret<0)
        {
            GTP_ERROR("GT801PLUS Update write frame data failed.");
            goto exit_update_fw;
        }
    }
    else
    {
        GTP_INFO("GT81XPLUS write frame data.");
        ret = gtp_update_write_fw(client, gt81x_fw, fw_len);
        if (ret < 0)
        {
            GTP_ERROR("Update write frame data failed.");
            goto exit_update_fw;
        }
    }
    GTP_INFO("Step 7 done!");
    if ((cfg_path != NULL)&&(ts->chip_type != GT801PLUS))
    {
        GTP_INFO("Step 8:write config...");
        ret = gtp_update_write_config(client, cfg_path);
        if (ret < 0)
        {
            return ret;
        }
        GTP_INFO("Step 8 done!");
        gtp_wait_for_slave(client,SLAVE_READY);
        msleep(1000);
    }
    else
    {
        GTP_INFO("Step 9:wait for slave ready...");
        ret = gtp_wait_for_slave(client,SLAVE_READY);
        if (ret < 0)
        {
            GTP_ERROR("Update wait slave ready failed.");
            goto exit_update_fw;
        }
        if (ret & SLAVE_READY)
        {
            GTP_INFO("The firmware updating succeed!update state:0x%x",ret);
        }
    }
    GTP_INFO("Step 10:enable irq...");
    enable_irq(client->irq);
    GTP_INFO("Step 10 done!");
    msleep(2000);						//wait slave reset
    vfree(gt81x_fw);
    GTP_INFO("Step 11:read new version...");
    ret = gtp_read_version(ts);
    if (ret) 
    {
        GTP_ERROR("Update read new version failed.");
        goto exit_update_fw;
    }
    ts->enter_update = 0;
    GTP_DEBUG("ts->Version:%s---",ts->version);
    if (strcmp(ts->version ,version_base))
    {
        show_len = total_len;
        vfree(fw_path);
        return 0;
    }
exit_update_fw:
    show_len = 2;
    total_len = 1;
    vfree(fw_path);
    return -1;
}
