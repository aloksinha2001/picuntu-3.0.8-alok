/* drivers/input/touchscreen/gt813_827_828_update.c
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

#include "gt813_827_828.h"

//-----------------------------------------------
//Do not change the order                       |
#define HEADER_UPDATE_DATA NULL         //      |
#if GUP_USE_HEADER_FILE                 //      |
#include "gt813_827_828_firmware.h"     //      |
#endif                                  //      |
//-----------------------------------------------

//#ifdef GTP_AUTO_UPDATE

#define BIT_NVRAM_STROE        0
#define BIT_NVRAM_RECALL       1
#define BIT_NVRAM_LOCK         2
#define REG_NVRCS              0x1201

#define READ_FW_MSG_ADDR       0x0F7C
#define READ_MSK_VER_ADDR      0xC009

#define FW_HEAD_LENGTH         30
#define FILE_HEAD_LENGTH       100
#define IGNORE_LENGTH          100
#define FW_MSG_LENGTH          7
#define UPDATE_DATA_LENGTH     5000

#define fail    0
#define success 1
#define false   0
#define true    1

u16 show_len;
u16 total_len;

struct i2c_client *guitar_client = NULL;

extern s32  gtp_i2c_read(struct i2c_client *client, uint8_t *buf, s32 len);
extern s32  gtp_i2c_write(struct i2c_client *client,uint8_t *data,s32 len);
extern s32  gtp_i2c_end_cmd(struct i2c_client *client);
extern void gtp_reset_guitar(s32 ms);
extern s32  gtp_send_cfg(struct i2c_client *client);

#pragma pack(1)
typedef struct 
{
    u8  type;          //产品类型//
    u16 version;       //FW版本号//
    u8  msk_ver[4];    //MASK版本//
    u8  st_addr[2];    //烧录的起始地址//
    u16 lenth;         //FW长度//
    u8  chk_sum[3];
    u8  force_update[6];//强制升级标志,为"GOODIX"则强制升级//
}st_fw_head;
#pragma pack()

typedef struct
{
    u8 force_update;
    u8 fw_flag;
    loff_t gt_loc;
    struct file *file; 
    st_fw_head  ic_fw_msg;
    mm_segment_t old_fs;
}st_update_msg;

st_update_msg update_msg;
//******************************************************************************

static u8 gup_get_ic_msg(struct i2c_client *client, u16 addr, u8* msg, s32 len)
{
    s32 i = 0;

    msg[0] = (addr >> 8) & 0xff;
    msg[1] = addr & 0xff;

    for (i = 0; i < 5; i++)
    {
        if (gtp_i2c_read(client, msg, GTP_ADDR_LENGTH + len) > 0)
        {
            break;
        }
    }
    gtp_i2c_end_cmd(client);

    if (i >= 5)
    {
        GTP_ERROR("Read data from 0x%02x%02x failed!", msg[0], msg[1]);
        return fail;
    }

    return success;
}

static u8 gup_clear_mix_flag(struct i2c_client *client)
{
    s32 i = 0;
    u8 buf[3];

    buf[0] = 0x14;
    buf[1] = 0x00;
    buf[2] = 0x8C;

    for (i = 0; i < 5; i++)
    {
        if (gtp_i2c_write(client, buf, 3) > 0)
        {
            break;
        }
    }
    gtp_i2c_end_cmd(client);

    if (i >= 5)
    {
        GTP_DEBUG("Clear mix flag failed!");
        return fail;
    }

    return success;
}

static u8 gup_get_ic_fw_msg(struct i2c_client *client)
{
    s32 ret = 0;
    s32 i = 0;
    u8 buf[32];
    
    if (fail == gup_clear_mix_flag(client))
    {
        return fail;
    }

    //Get the mask version in rom of IC
    if (fail == gup_get_ic_msg(client, READ_MSK_VER_ADDR, buf, 4))
    {
        GTP_ERROR("Read mask version failed!");
        return fail;
    }

    memcpy(update_msg.ic_fw_msg.msk_ver, &buf[GTP_ADDR_LENGTH], 4);
    GTP_DEBUG("IC The mask version in rom is %c%c%c%c.",
                 update_msg.ic_fw_msg.msk_ver[0],update_msg.ic_fw_msg.msk_ver[1],
                 update_msg.ic_fw_msg.msk_ver[2],update_msg.ic_fw_msg.msk_ver[3]);

    //Get the firmware msg in IC, include firmware version and checksum flag
    for (i = 0; i < 2; i++)
    {
        if (fail == gup_get_ic_msg(client, READ_FW_MSG_ADDR, buf, 4))
        {
            GTP_DEBUG("Get firmware msg in IC error.");
            return fail;
        }
        update_msg.force_update = buf[GTP_ADDR_LENGTH];
        if (i == 0 && update_msg.force_update == 0xAA)
        {
            GTP_INFO("The check sum in ic is error.");
            GTP_INFO("IC will be reset.");
            GTP_INFO("If the check sum is still error, ");
            GTP_INFO("The IC will be updated by force.");

            gtp_reset_guitar(10);
            continue;
            //msleep(100);
        }
        break;
    }
    //ic_fw_msg.type = buf[GTP_ADDR_LENGTH + 1];
    update_msg.ic_fw_msg.version = buf[GTP_ADDR_LENGTH + 2] << 8 | buf[GTP_ADDR_LENGTH + 3];
    GTP_DEBUG("IC VID:0x%x", (int)update_msg.ic_fw_msg.version);
    GTP_DEBUG("IC force update:%x", update_msg.force_update);

    //Cuts the frequency
    buf[0] = 0x15;
    buf[1] = 0x22;
    buf[2] = 0x18;
    ret =  gtp_i2c_write(client, buf, 3);
    if (ret <= 0)
    {
        return fail;
    }
    gtp_i2c_end_cmd(client);
    
    //Get the pid at 0x4011 in nvram
    if (fail == gup_get_ic_msg(client, 0x4011, buf, 1))
    {
        GTP_ERROR("Read pid failed!");
        return fail;
    }
    update_msg.ic_fw_msg.type = buf[GTP_ADDR_LENGTH];
    
    GTP_DEBUG("IC PID:%x", update_msg.ic_fw_msg.type);

    return success;
}

/*
* Steps of reset guitar
*1. INT脚输出低，延时5ms
*2. RESET脚拉低100ms，转输入悬浮态
*3. I2C寻址GUITAR
*4. 延时100ms读取0xff(3、4轮询80次，直至成功)
*5. Oxff等于0x55则返回成功，否则失败
*/
s32 gup_enter_update_mode(struct i2c_client *client)
{
    int ret = 1;
    u8 retry;
    unsigned char inbuf[3] = {0,0xff,0};

    // step 1
    GTP_GPIO_OUTPUT(GTP_INT_PORT, 0);
    msleep(5);

    //step 2
    gtp_reset_guitar(100);

    for(retry=0;retry < 20; retry++)
    {
        //step 3
        ret = gtp_i2c_write(client, inbuf, 2);   //Test I2C connection.
        if (ret > 0)
        {
            GTP_DEBUG("<Set update mode>I2C is OK!");
            //step 4
            msleep(100);
            ret = gtp_i2c_read(client, inbuf, 3);
            if (ret > 0)
            {
                GTP_DEBUG("The value of 0x00ff is 0x%02x", inbuf[2]);
                //step 5
                if(inbuf[2] == 0x55)
                {
                    return success;
                }
            }
        }
        msleep(40);
    }
    GTP_ERROR(KERN_INFO"Detect address %0X", client->addr);

    return fail;
}

void gup_leave_update_mode(void)
{
    GTP_GPIO_AS_INT(GTP_INT_PORT);
}

u8 gup_load_update_file(struct i2c_client *client, st_fw_head* fw_head, u8* data, u8* path)
{
    u8 mask_num = 0;
    int ret = 0;
    int i = 0;
    u8 buf[FW_HEAD_LENGTH];

    if (path)
    {
        GTP_DEBUG("File path:%s, %d", path, strlen(path));
        update_msg.file = filp_open(path, O_RDONLY, 0644);

        if (IS_ERR(update_msg.file))
        {
            GTP_ERROR("Open update file(%s) error!", path);
            return fail;
        }
    }
    else
    {
        //Begin to search update file
        for (i = 0; i < SEARCH_FILE_TIMES; i++)
        {
            update_msg.file = filp_open(UPDATE_FILE_PATH_1, O_RDWR, 0666);
            if (IS_ERR(update_msg.file))
            {
                update_msg.file = filp_open(UPDATE_FILE_PATH_2, O_RDWR, 0666);//O_RDWR
                if (IS_ERR(update_msg.file))
                {
                    GTP_DEBUG("%3d:Searching file...", i);
                    msleep(3000);
                    continue;
                }
                else
                {
                    break;
                }
            }
            else
            {
                break;
            }
        }
        if (i >= SEARCH_FILE_TIMES)
        {
            GTP_ERROR("Can't find update file.");
            return fail;
        }
        GTP_DEBUG("Find the update file.");
    }
    
    update_msg.old_fs = get_fs();
    set_fs(KERNEL_DS);

    update_msg.file->f_pos = IGNORE_LENGTH;

    //Make sure the file is the right file.(By compare the "Guitar" flag)
    ret = update_msg.file->f_op->read(update_msg.file, (char*)&buf, 6, &update_msg.file->f_pos);
    if (ret < 0)
    {
        GTP_ERROR("Read \"Guitar\" flag error.");
        goto load_failed;
    }
    if (memcmp(buf, "Guitar", 6))
    {
        GTP_ERROR("The flag is %s.Not equal!"
                     "The update file is incorrect!", buf);
        goto load_failed; 
    }
    GTP_DEBUG("The file flag is :%s.", buf);
    
    //Get the total number of masks
    update_msg.file->f_pos++; //ignore one byte.
    ret = update_msg.file->f_op->read(update_msg.file, &mask_num, 1, &update_msg.file->f_pos);
    if (ret < 0)
    {
        GTP_ERROR("Didn't get the mask number from the file.");
        goto load_failed;
    }
    GTP_DEBUG("FILE The total number of masks is:%d.", mask_num);
    update_msg.file->f_pos = FILE_HEAD_LENGTH + IGNORE_LENGTH;

    //Get the correct nvram data
    //The correct conditions: 
    //1. the product id is the same
    //2. the mask id is the same
    //3. the nvram version in update file is greater than the nvram version in ic 
    //or force update flag is marked or the check sum in ic is wrong
    update_msg.gt_loc = -1;
    for ( i = 0; i < mask_num; i++)
    {        
        ret = update_msg.file->f_op->read(update_msg.file, (char*)buf, FW_HEAD_LENGTH, &update_msg.file->f_pos);
        if (ret < 0)
        {
            GTP_ERROR("Read update file head error.");
            goto load_failed;
        }
        memcpy(fw_head, buf, sizeof(st_fw_head));
        fw_head->version = buf[1] << 8 | buf[2];
        fw_head->lenth = buf[9] << 8 | buf[10];
        GTP_DEBUG("No.%d firmware", i);
        GTP_DEBUG("FILE PID:%x", fw_head->type);
        GTP_DEBUG("FILE VID:0x%04x", fw_head->version);
        GTP_DEBUG("FILE mask version:%c%c%c%c.", fw_head->msk_ver[0],
                     fw_head->msk_ver[1],fw_head->msk_ver[2],fw_head->msk_ver[3]);
        GTP_DEBUG("FILE start address:0x%02x%02x.", fw_head->st_addr[0], fw_head->st_addr[1]);
        GTP_DEBUG("FILE length:%d", (int)fw_head->lenth);
        GTP_DEBUG("FILE force update flag:%s", fw_head->force_update);
        GTP_DEBUG("FILE chksum:0x%02x%02x%02x", fw_head->chk_sum[0], 
                                 fw_head->chk_sum[1], fw_head->chk_sum[2]);

        //First two conditions
        if (update_msg.ic_fw_msg.type == fw_head->type
            && !memcmp(fw_head->msk_ver, update_msg.ic_fw_msg.msk_ver, sizeof(update_msg.ic_fw_msg.msk_ver)))
        {
            GTP_DEBUG("Get the same mask version and same pid.");
            //The third condition
            if (fw_head->version > update_msg.ic_fw_msg.version
                || !memcmp(fw_head->force_update, "GOODIX", 6) 
                || update_msg.force_update == 0xAA)
            {
               // GTP_DEBUG("FILE read position:%d", file->f_pos);
               // file->f_pos = FW_HEAD_LENGTH + FILE_HEAD_LENGTH + IGNORE_LENGTH;
                if (!memcmp(fw_head->force_update, "GOODIX", 6))
                {
                    update_msg.gt_loc = update_msg.file->f_pos - FW_HEAD_LENGTH + sizeof(st_fw_head) - sizeof(fw_head->force_update);
                }
                
                ret = update_msg.file->f_op->read(update_msg.file, (char*)data, fw_head->lenth, &update_msg.file->f_pos);
                if (ret <= 0)
                {
                    GTP_ERROR("Read firmware data in file error.");
                    goto load_failed;
                }
               // GTP_DEBUG_ARRAY(data, 512);
               // set_fs(ts->old_fs);
              //  filp_close(ts->file, NULL);
                GTP_DEBUG("Load data from file successfully.");
                return success;
            }
            GTP_ERROR("Don't meet the third condition.");
            goto load_failed;
        }

        update_msg.file->f_pos += UPDATE_DATA_LENGTH;
    }

load_failed:    
    set_fs(update_msg.old_fs);
    filp_close(update_msg.file, NULL);
    return fail;
}

u8 gup_load_update_header(struct i2c_client *client, st_fw_head* fw_head, u8** data)
{
    const u8* pos;
    int i = 0;
    u8 mask_num = 0;

    pos = HEADER_UPDATE_DATA;

    pos += IGNORE_LENGTH;

    GTP_INFO("The file flag is :%c%c%c%c%c%c.", pos[0], pos[1], pos[2], pos[3], pos[4], pos[5]);
    if (memcmp((u8 *)pos, "Guitar", 6))
    {
        GTP_INFO("The update Header is incorrect!");

        return fail;
    }

    pos += 7;
    mask_num = pos[0];
    GTP_INFO("Header The total number of masks is:%d.", mask_num);

    pos = HEADER_UPDATE_DATA;
    pos += FILE_HEAD_LENGTH + IGNORE_LENGTH;

   // update_msg.gt_loc = -1;
    for ( i = 0; i < mask_num; i++)
    {        
        memcpy(fw_head, pos, sizeof(st_fw_head));
        fw_head->version = pos[1] << 8 | pos[2];
        fw_head->lenth = pos[9] << 8 | pos[10];
        GTP_INFO("No.%d firmware", i);
        GTP_INFO("Header PID:%x", fw_head->type);
        GTP_INFO("Header VID:0x%x", fw_head->version);
        GTP_INFO("Header mask version:%c%c%c%c.", fw_head->msk_ver[0],
                     fw_head->msk_ver[1],fw_head->msk_ver[2],fw_head->msk_ver[3]);
        GTP_INFO("Header start address:0x%02x%02x.", fw_head->st_addr[0], fw_head->st_addr[1]);
        GTP_INFO("Header length:%d", (int)fw_head->lenth);
        GTP_INFO("Header force update flag:%s", fw_head->force_update);
        GTP_INFO("Header chksum:0x%02x%02x%02x", fw_head->chk_sum[0], 
                                 fw_head->chk_sum[1], fw_head->chk_sum[2]);

        pos += FW_HEAD_LENGTH;

        //First two conditions
        if (update_msg.ic_fw_msg.type == fw_head->type
            && !memcmp(fw_head->msk_ver, update_msg.ic_fw_msg.msk_ver, sizeof(update_msg.ic_fw_msg.msk_ver)))
        {
            GTP_INFO("Get the same mask version and same pid.");
            //The third condition
            /*if (fw_head->version > update_msg.ic_fw_msg.version
                || !memcmp(fw_head->force_update, "GOODIX", 6) 
                || update_msg.force_update == 0xAA)    */
            if (fw_head->version > update_msg.ic_fw_msg.version
                || update_msg.force_update == 0xAA)
            {

                *data = (u8 *)pos;

                GTP_INFO("Load data from Header successfully.");
                return success;
            }
            GTP_INFO("Don't meet the third condition.");
            return fail;
        }

        pos += UPDATE_DATA_LENGTH;
    }

    return fail;
}

static u8 gup_nvram_store(struct i2c_client *client)
{
    int ret;
    int i;
    u8 inbuf[3] = {REG_NVRCS >> 8, REG_NVRCS & 0xff, 0x18};

    ret = gtp_i2c_read(client, inbuf, 3);
    if ( ret < 0 )
    {
        return fail;
    }

    if ((inbuf[2] & BIT_NVRAM_LOCK ) == BIT_NVRAM_LOCK)
    {
        return fail;
    }

    inbuf[2] = 0x18;
    inbuf[2] |= (1<<BIT_NVRAM_STROE);        //store command

    for ( i = 0 ; i < 300 ; i++ )
    {
        ret = gtp_i2c_write(client, inbuf, 3);
        if ( ret > 0 )
            return success;
    }

    return fail;
}

static u8 gup_nvram_recall(struct i2c_client *client)
{
    int ret;
    u8 inbuf[3] = {REG_NVRCS >> 8, REG_NVRCS & 0xff, 0};

    ret = gtp_i2c_read(client, inbuf, 3);
    if ( ret < 0 )
    {
        return fail;
    }

    if ( ( inbuf[2]&BIT_NVRAM_LOCK) == BIT_NVRAM_LOCK )
    {
        return fail;
    }

    inbuf[2] = ( 1 << BIT_NVRAM_RECALL );        //recall command
    ret = gtp_i2c_write(client , inbuf, 3);

    if (ret <= 0)
    {
        return fail;
    }
    return success;
}

static u8 gup_update_nvram(struct i2c_client *client, st_fw_head* fw_head, u8 *nvram)
{
    int length = 0;
    int ret = 0;
    int write_bytes = 0;
    int retry = 0;
    int i = 0;
    int comp = 0;
    u16 st_addr = 0;
    u8 w_buf[PACK_SIZE + GTP_ADDR_LENGTH];
    u8 r_buf[PACK_SIZE + GTP_ADDR_LENGTH];

    if (fw_head->lenth > PACK_SIZE)
    {
        write_bytes = PACK_SIZE;
    }
    else
    {
        write_bytes = fw_head->lenth;
    }

   // gup_clear_mix_flag(ts);
    st_addr = (fw_head->st_addr[0] << 8) | (fw_head->st_addr[1]&0xff);
    memcpy(&w_buf[2], &nvram[length], write_bytes);
    GTP_DEBUG("Total length:%d", (int)fw_head->lenth);
    while(length < fw_head->lenth)
    {
        w_buf[0] = st_addr >> 8;
        w_buf[1] = st_addr & 0xff;
        GTP_DEBUG("Write address:0x%02x%02x\tlength:%d", w_buf[0], w_buf[1], write_bytes);
        ret =  gtp_i2c_write(client, w_buf, GTP_ADDR_LENGTH + write_bytes);
        if (ret <= 0)
        {
            if (retry++ > 10)
            {
                GTP_DEBUG("Write the same address 10 times.Give up!");
                return fail;
            }
            GTP_DEBUG("Write error![gup_update_nvram]");
            continue;
        }
        else
        {
//            GTP_DEBUG("w_buf:");
//            GTP_DEBUG_ARRAY(w_buf, GTP_ADDR_LENGTH + write_bytes);
/*            r_buf[0] = 0x14;
            r_buf[1] = 0x00;
            r_buf[2] = 0x80;
            gtp_i2c_write(ts->client, r_buf, 3);
            r_buf[0] = 0x14;
            r_buf[1] = 0x00;
            gtp_i2c_read(ts->client, r_buf, 3);
            GTP_DEBUG("I2CCS:0x%x", r_buf[2]);//*/
            
            r_buf[0] = w_buf[0];
            r_buf[1] = w_buf[1];

            for (i = 0; i < 10; i++)
            {
                ret = gtp_i2c_read(client, r_buf, GTP_ADDR_LENGTH + write_bytes);
                if (ret <= 0)
                {
                    continue;
                }
                break;
            }
            if (i >= 10)
            {
                GTP_DEBUG("Read error! Can't check the nvram data.");
                return fail;
            }
//            GTP_DEBUG("r_buf:");
//            GTP_DEBUG_ARRAY(r_buf, GTP_ADDR_LENGTH + write_bytes);
#if 0
            if (fail == gup_nvram_store(ts))
            {
                GTP_DEBUG("Store nvram failed.");
                //continue;
            }
            return fail;
#endif
            if (memcmp(r_buf, w_buf, GTP_ADDR_LENGTH + write_bytes))
            {   
                if (comp ++ > 10)
                {
                    GTP_DEBUG("Compare error!");
                    return fail;
                }
                GTP_DEBUG("Updating nvram: Not equal!");

                GTP_DEBUG("r_buf:");
                GTP_DEBUG_ARRAY(r_buf, GTP_ADDR_LENGTH + write_bytes);

                
                GTP_DEBUG("w_buf:");
//                GTP_DEBUG_ARRAY(w_buf, GTP_ADDR_LENGTH + write_bytes);
                continue;
                //return fail;
            }
        }
        comp = 0;
        retry = 0;
        length += PACK_SIZE;
        st_addr += PACK_SIZE;
        if ((length + PACK_SIZE) > fw_head->lenth)
        {
            write_bytes = fw_head->lenth - length;
        }
        memcpy(&w_buf[2], &nvram[length], write_bytes);
    }

    return success;
}

static u8 gup_update_firmware(struct i2c_client *client, st_fw_head* fw_head, u8 *nvram)
{
    s32 retry;
    s32 i, j;
    s32 ret;
    s32 right_count = 0;
    u32 status = 0;
    u16 check_sum = 0;
    u8 buf[150];
    u8  tmp[150];

    //Cuts the frequency
    buf[0] = 0x15;
    buf[1] = 0x22;
    buf[2] = 0x18;
    ret =  gtp_i2c_write(client, buf, 3);
    if (ret <= 0)
    {
        return fail;
    }

    gup_get_ic_msg(client, 0x1522, buf, 1);
    GTP_DEBUG("IC OSC_CAL:0x%02x.", buf[2]);

    gup_clear_mix_flag(client);

    for (retry = 0; retry < 10; retry++)
    {
        if (!(status & 0x01))
        {
            tmp[0] = 0x4f;
            tmp[1] = 0x70;
            gtp_i2c_read(client, tmp, 130);
            
            for (i = 0; i < 50; i++)
            {
                buf[0] = 0x4f;
                buf[1] = 0x70;
                ret = gtp_i2c_read(client, buf, 130);
                if (ret <= 0)
                {
                    continue;
                }
                else
                {
                    GTP_DEBUG("Read solidified config successfully!");
                    ret = !memcmp(tmp, buf, 130);
                    memcpy(tmp, buf, 130);
                    if (true == ret)
                    {
                        if (right_count++ < 3)
                        {
                            continue;
                        }
                        else
                        {
                            for (j = 0; j < 128; j++)
                            {
                                check_sum += buf[j + 2];
                            }
                            GTP_DEBUG("check sum of solidified config is %04x", check_sum);
                            status |= 0x01;
                            break;
                        }
                    }
                    else
                    {
                        right_count = 0;
                    }
                    if (i >= 40)
                    {
                        GTP_DEBUG("Compare solidified config failed!");
                        return fail;
                    }
                }
            }
        }
        //Write the 2nd part (nvram)
        if (!(status & 0x02))
        {
            if (fail == gup_update_nvram(client, fw_head, nvram))
            {
                continue;
            }
            else
            {
                GTP_DEBUG("Update nvram successfully!");
                status |= 0x02;
                msleep(1);
            }
        }

        //Write the 3rd part (check sum)
        if (1)
        {
            u32 sum = 0;
            sum |= fw_head->chk_sum[0] << 16;
            sum |= fw_head->chk_sum[1] << 8;
            sum |= fw_head->chk_sum[2];
            sum += check_sum;

            fw_head->chk_sum[0] = sum >> 16;
            fw_head->chk_sum[1] = sum >> 8;
            fw_head->chk_sum[2] = sum;

            GTP_DEBUG("FILE chksum after addition:0x%02x%02x%02x", fw_head->chk_sum[0], 
                                     fw_head->chk_sum[1], fw_head->chk_sum[2]);
            buf[0] = 0x4f;
            buf[1] = 0xf3;
            memcpy(&buf[2], fw_head->chk_sum, sizeof(fw_head->chk_sum));
            ret = gtp_i2c_write(client, buf, 5);
            if (ret <= 0)
            {
                continue;
            }
            else
            {
                GTP_DEBUG("Update check sum successfully!");
                break;
            }
        }
    }

    if (retry >= 10)
    {
        return fail;
    }
    else
    {
        for (retry = 0; retry < 10; retry++)
        {
            buf[0] = 0x00;
            buf[1] = 0xff;
            buf[2] = 0x44;
            ret = gtp_i2c_write(client, buf, 3);
            if (ret > 0)
            {
                break;
            }
        }

        if (retry >= 10)
        {
            GTP_DEBUG("Write address at 0x00ff error!\n");
            return fail;
        }
        msleep(10);
    }

    for (retry = 0; retry < 30; retry++)
    {
        msleep(1);
        if (fail == gup_get_ic_msg(client, 0x00ff, buf, 1))
        {
            GTP_DEBUG("Read address at 0x00ff error!\t retry:%d\n", retry);
            continue;
        }

        if (0xcc == buf[GTP_ADDR_LENGTH])
        {
            return success;
        }
        else
        {
            GTP_DEBUG("The value of 0x00ff: 0x%02x!\t retry:%d\n", buf[GTP_ADDR_LENGTH], retry);
            continue;
        }
    }

    GTP_DEBUG("The value of 0x00ff error.\n");
    return fail;
}

s32 gup_update_proc(void *dir)
{
    s32 ret;
    u32 retry = 100;
    u32 i = 0;
    struct goodix_ts_data* ts = NULL;
    u8* data = NULL;
    u8* ic_nvram = NULL;
    st_fw_head fw_head;
    u8 buf[32];

    show_len = 20;
    total_len = 100;

    ts = i2c_get_clientdata(guitar_client);

    if (!(HEADER_UPDATE_DATA != NULL && dir == NULL))
    {
        data = kzalloc(UPDATE_DATA_LENGTH, GFP_KERNEL);
        if (NULL == data)
        {
            GTP_ERROR("data failed apply for memory.");
            return fail;
        }
    }

    ic_nvram = kzalloc(UPDATE_DATA_LENGTH, GFP_KERNEL);
    if (NULL == ic_nvram)
    {
        GTP_ERROR("ic_nvram failed apply for memory.");
        goto app_mem_failed;
    }
    GTP_DEBUG("Apply for memory successfully.memory size: %d.", UPDATE_DATA_LENGTH);

    if (dir != NULL)
    {
        gup_get_ic_fw_msg(guitar_client);
        gtp_reset_guitar(10);
    }
    else
    {
        msleep(1000);
    }
    GTP_DEBUG("Updating...");

    if (!(HEADER_UPDATE_DATA != NULL && dir == NULL))
    {
        if (fail == gup_load_update_file(guitar_client, &fw_head, data, (u8*)dir))
        {
            GTP_ERROR("Load file data failed!");
            goto load_failed;
        }
    }
    else
    {
        if (fail == gup_load_update_header(guitar_client, &fw_head, &data))
        {
            GTP_ERROR("Load file data failed!");
            goto load_failed;
        }
    }
    GTP_DEBUG("Load file data successfully!");

    show_len = 40;
    total_len = 100;

    ts->enter_update = 1;
    
    for (i = 0; i < 5; i++)
    {
        if (fail == gup_enter_update_mode(guitar_client))
        {
            GTP_ERROR("Next try![Enter update mode]");
            continue;
        }
        else
        {
            GTP_ERROR("Set update mode successfully.");
            break;
        }
    }
    if (i >= 5)
    {
        GTP_ERROR("Set update mode failed.");
        return fail;
    }
    
    retry = 0;
    while(retry++ < 5)
    {
        if (fail == gup_update_firmware(guitar_client, &fw_head, data))
        {
            GTP_ERROR("Update firmware failed.");
            continue;
        }
        GTP_DEBUG("Update firmware successfully.");

        //while(1)  // simulation store operation failed
        if (fail == gup_nvram_store(guitar_client))
        {
            GTP_ERROR("Store nvram failed.");
            continue;
        }

        msleep(100);

        if (fail == gup_get_ic_msg(guitar_client, 0x1201, buf, 1))
        {
            GTP_ERROR("Read NVRCS failed.(Store)");
            continue;
        }
        if (buf[GTP_ADDR_LENGTH] & 0x01)
        {
            GTP_ERROR("Check NVRCS(0x%02x) failed.(Store)", buf[GTP_ADDR_LENGTH]);
            continue;
        }

        GTP_DEBUG("Store nvram successfully.");

        if (fail == gup_nvram_recall(guitar_client))
        {
            GTP_ERROR("Recall nvram failed.");
            continue;
        }
        msleep(5);
        
        if (fail == gup_get_ic_msg(guitar_client, 0x1201, buf, 1))
        {
            GTP_ERROR("Read NVRCS failed.(Recall)");
            continue;
        }
        if (buf[GTP_ADDR_LENGTH] & 0x02)
        {
            GTP_ERROR("Check NVRCS(0x%02x) failed.(Recall)", buf[GTP_ADDR_LENGTH]);
            continue;
        }
        GTP_DEBUG("Recall nvram successfully.");

        ic_nvram[0] = fw_head.st_addr[0];
        ic_nvram[1] = fw_head.st_addr[1];

        for ( i = 0; i < 10; i++)
        {
            ret = gtp_i2c_read(guitar_client, ic_nvram, GTP_ADDR_LENGTH + fw_head.lenth);
            if (ret <= 0)
            {
                continue;
            }
            break;
        }

        if (i >= 10)
        {
            GTP_ERROR("Read nvram failed!");
            continue;
        }
        GTP_DEBUG("Read nvram successfully!");

        if (memcmp(data, &ic_nvram[2], fw_head.lenth))
        {
            GTP_ERROR("Nvram not equal!");
            continue;
        }
        GTP_DEBUG("Check nvram by byte successfully!");

  //      if (HEADER_UPDATE_DATA == NULL || dir != NULL)
        if (HEADER_UPDATE_DATA == NULL && dir == NULL)
        {
            if (update_msg.gt_loc > 0)
            {
                GTP_DEBUG("Location:%d, Ret:%d.", (s32)update_msg.gt_loc, (s32)ret);
                memset(buf, 0, sizeof(buf));
                ret = update_msg.file->f_op->write(update_msg.file, buf, 6, &update_msg.gt_loc);
                if (ret < 0)
                {
                    GTP_ERROR("Didn't clear the focre update flag in file.");
                }
                else
                {
                    GTP_DEBUG("Clear the focre update flag in file.Location:%d, Ret:%d.", (s32)update_msg.gt_loc, (s32)ret);
                }
            }
        }
        
        GTP_DEBUG("Update successfully!");
        break;
    }
    if (!(HEADER_UPDATE_DATA != NULL && dir == NULL))
    {
        set_fs(update_msg.old_fs);
        filp_close(update_msg.file, NULL);
    }
    gup_leave_update_mode();
    GTP_DEBUG("Leave update mode!");
    
    //Reset guitar
    GTP_DEBUG("Reset IC and send config!");
    gtp_reset_guitar(10);

    ret = gtp_send_cfg(guitar_client);
    if (ret < 0)
    {
        GTP_ERROR("Send config data failed.");
    }
    
    msleep(10);
    ts->enter_update = 0;
    
load_failed:
    kfree(ic_nvram);
app_mem_failed:
    if (!(HEADER_UPDATE_DATA != NULL && dir == NULL))
    {
        kfree(data);
    }

    if (retry < 5)
    {
        show_len = 100;
        return success;
    }
    
    show_len = 200;
    GTP_DEBUG("Update failed!");
    return fail;    
}

u8 gup_init_update_proc(struct goodix_ts_data *ts)
{
    u8 flag = 0;
    struct task_struct *thread = NULL;
    s32 retry = 0;

    GTP_INFO("Ready to run update thread.");

    guitar_client = ts->client;

    update_msg.fw_flag = gup_get_ic_fw_msg(ts->client);
    if (fail == update_msg.fw_flag)
    {
        GTP_DEBUG("Try get ic msg in update mode.");
        for (retry = 0; retry < 5; retry++)
        {
            if (success == gup_enter_update_mode(ts->client))
            {
                break;
            }
        }
        if (retry >= 5)
        {
            update_msg.fw_flag = fail;
        }
        else
        {
            GTP_DEBUG("Get ic msg in update mode.");
            update_msg.fw_flag = gup_get_ic_fw_msg(ts->client);
            update_msg.ic_fw_msg.version = 0xfff0;
            if (update_msg.force_update == 0xAA)
            {
                flag = 0xff;
            }
        }
        gup_leave_update_mode();
    }

    gtp_reset_guitar(10);

    if (success == update_msg.fw_flag)
    {
        update_msg.gt_loc = -1;
        thread = kthread_run(gup_update_proc, (void*)NULL, "guitar_update");
        if (IS_ERR(thread))
        {
            GTP_ERROR("Failed to create update thread.");
            return -1;
        }
    }

    return 0;
}
//#endif   //endif GTP_AUTO_UPDATE
//******************************End of firmware update surpport*******************************
