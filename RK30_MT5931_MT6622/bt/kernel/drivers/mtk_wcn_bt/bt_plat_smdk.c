/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein is
 * confidential and proprietary to MediaTek Inc. and/or its licensors. Without
 * the prior written permission of MediaTek inc. and/or its licensors, any
 * reproduction, modification, use or disclosure of MediaTek Software, and
 * information contained herein, in whole or in part, shall be strictly
 * prohibited.
 * 
 * MediaTek Inc. (C) 2010. All rights reserved.
 * 
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
 * RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER
 * ON AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL
 * WARRANTIES, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR
 * NONINFRINGEMENT. NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH
 * RESPECT TO THE SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY,
 * INCORPORATED IN, OR SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES
 * TO LOOK ONLY TO SUCH THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO.
 * RECEIVER EXPRESSLY ACKNOWLEDGES THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO
 * OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES CONTAINED IN MEDIATEK
 * SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE
 * RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S
 * ENTIRE AND CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE
 * RELEASED HEREUNDER WILL BE, AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE
 * MEDIATEK SOFTWARE AT ISSUE, OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE
 * CHARGE PAID BY RECEIVER TO MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 *
 * The following software/firmware and/or related documentation ("MediaTek
 * Software") have been modified by MediaTek Inc. All revisions are subject to
 * any receiver's applicable license agreements with MediaTek Inc.
 */

#include <linux/module.h>
#include <linux/delay.h>
#include <linux/irq.h>
#include <linux/interrupt.h>
#include <linux/gpio.h>
#include <mach/gpio.h>
//#include <plat/gpio-cfg.h>

#include "bt_hwctl.h"

/****************************************************************************
 *                           C O N S T A N T S                              *
*****************************************************************************/
#define MODULE_TAG         "[BT-PLAT] "

/* SMDK platform PIN definition */
#define GPIO_BT_PWR_EN_PIN RK30_PIN3_PD0
#define GPIO_BT_RESET_PIN  RK30_PIN3_PD0
#define GPIO_BT_EINT_PIN   RK30_PIN3_PD0
#define GPIO_BT_URXD_PIN
#define GPIO_BT_UTXD_PIN

static struct gpio_set bt_gpio_set[3] = {
    { GPIO_BT_PWR_EN_PIN, 0, "BT_PWR_EN" },
    { GPIO_BT_RESET_PIN,  0, "BT_RESET"  },
    { GPIO_BT_EINT_PIN,   0, "BT_EINT"   }
};

static int irq_num = -1;

/****************************************************************************
 *                       I R Q   F U N C T I O N S                          *
*****************************************************************************/
static int mt_bt_request_irq(void)
{
    int iRet;
    iRet = request_irq(irq_num, mt_bt_eirq_handler, 
        /*IRQF_TRIGGER_RISING*/IRQF_TRIGGER_HIGH, "BT_INT_B", NULL);
    if (iRet){
        printk(KERN_ALERT MODULE_TAG "request_irq IRQ%d fails, errno %d\n", irq_num, iRet);
    }
    else{
        printk(KERN_INFO MODULE_TAG "request_irq IRQ%d success\n", irq_num);
        mt_bt_disable_irq();
        /* enable irq when driver init complete, at hci_uart_open */
    }
    
    return iRet;
}

static void mt_bt_free_irq(void)
{
    if(irq_num != -1) {
        free_irq(irq_num, NULL);
        irq_num = -1;
    }
}

void mt_bt_enable_irq(void)
{
    if(irq_num != -1) {
        enable_irq(irq_num);
    }
}
EXPORT_SYMBOL(mt_bt_enable_irq);

void mt_bt_disable_irq(void)
{
    if(irq_num != -1) {	
        disable_irq_nosync(irq_num);
    }        
}
EXPORT_SYMBOL(mt_bt_disable_irq);

/****************************************************************************
 *                      P O W E R   C O N T R O L                           *
*****************************************************************************/
void mt_bt_gpio_init(void)
{
    int i;
    for(i = 0; i < 3; i++){
        if (gpio_request(bt_gpio_set[i].gpio, bt_gpio_set[i].label)){
            /* GPIO pin is already requested, check who is using now */
            printk(KERN_ALERT MODULE_TAG "GPIO%d is busy!\n", bt_gpio_set[i].gpio);
        }
        else{
            printk(KERN_INFO MODULE_TAG "GPIO%d is requested as %s\n", bt_gpio_set[i].gpio, bt_gpio_set[i].label);
            bt_gpio_set[i].requested = 1;
        }
    }
}
EXPORT_SYMBOL(mt_bt_gpio_init);

void mt_bt_gpio_release(void)
{
    int i;
    for(i = 0; i < 3; i++){
        if (bt_gpio_set[i].requested){
            /* GPIO pin is requested by BT, release it */
            gpio_free(bt_gpio_set[i].gpio);
            bt_gpio_set[i].requested = 0;
        }
    }
}
EXPORT_SYMBOL(mt_bt_gpio_release);


int mt_bt_power_on(void)
{
    int error;
    printk(KERN_INFO MODULE_TAG "mt_bt_power_on ++\n");
    
    // UART TX/RX
    
    // PCMIN, PCMOUT, PCMCLK, PCMSYNC
    
    // EINT
    //--s3c_gpio_cfgpin(GPIO_BT_EINT_PIN, S3C_GPIO_SFN(0));
    //--s3c_gpio_setpull(GPIO_BT_EINT_PIN, S3C_GPIO_PULL_DOWN);
    gpio_direction_input(GPIO_BT_EINT_PIN);
    gpio_pull_updown(GPIO_BT_EINT_PIN, GPIOPullDown);
    /* set to EINT mode */
    //--s3c_gpio_cfgpin(GPIO_BT_EINT_PIN, S3C_GPIO_SFN(0xF));
    /* get irq number */
    irq_num = gpio_to_irq(GPIO_BT_EINT_PIN);
    //mt_set_gpio_mode(GPIO_BT_EINT_PIN, GPIO_BT_EINT_PIN_M_GPIO);
    //mt_set_gpio_pull_enable(GPIO_BT_EINT_PIN, 1);
    //mt_set_gpio_pull_select(GPIO_BT_EINT_PIN, GPIO_PULL_DOWN);
    //mt_set_gpio_mode(GPIO_BT_EINT_PIN, GPIO_BT_EINT_PIN_M_EINT);
    
    // 32k CLK
    //mt_set_gpio_mode(GPIO_BT_CLK_PIN , GPIO_BT_CLK_PIN_M_CLK);
    //mt_set_clock_output(GPIO_BT_CLK_PIN_CLK, CLK_SRC_F32K, 1);
    
    // PWR_EN and RESET
    /* PWR_EN set to gpio output low */
    gpio_direction_output(GPIO_BT_PWR_EN_PIN, 0);
    /* RESET set to gpio output low */
    gpio_direction_output(GPIO_BT_RESET_PIN, 0);
    msleep(200);
    
    /* PWR_EN pull up */
    gpio_direction_output(GPIO_BT_PWR_EN_PIN, 1);
    msleep(200);
    /* RESET pull up */
    gpio_direction_output(GPIO_BT_RESET_PIN, 1);
    msleep(1000);
    
    error = mt_bt_request_irq();
    if (error){
        gpio_direction_output(GPIO_BT_PWR_EN_PIN, 0);
        gpio_direction_output(GPIO_BT_RESET_PIN, 0);
        //--s3c_gpio_cfgpin(GPIO_BT_EINT_PIN, S3C_GPIO_SFN(1));
        gpio_direction_output(GPIO_BT_EINT_PIN, 0);
        return error;
    }
    
    printk(KERN_INFO MODULE_TAG "mt_bt_power_on --\n");
    
    return 0;
}

EXPORT_SYMBOL(mt_bt_power_on);


void mt_bt_power_off(void)
{
    printk(KERN_INFO MODULE_TAG "mt_bt_power_off ++\n");
    
    // PWR_EN and RESET
    gpio_direction_output(GPIO_BT_PWR_EN_PIN, 0);
    gpio_direction_output(GPIO_BT_RESET_PIN, 0);
    
    // EINT
    //--s3c_gpio_cfgpin(GPIO_BT_EINT_PIN, S3C_GPIO_SFN(1));
    gpio_direction_output(GPIO_BT_EINT_PIN, 0);
    
    mt_bt_free_irq();
    
    printk(KERN_INFO MODULE_TAG "mt_bt_power_off --\n");
}

EXPORT_SYMBOL(mt_bt_power_off);
