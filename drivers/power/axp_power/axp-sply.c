/*
 * Battery charger driver for X-Powers AXP
 *
 * Copyright (C) 2011 X-Powers, Ltd.
 *  Zhang Donglu <zhangdonglu@x-powers.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/types.h>
#include <linux/device.h>
#include <linux/workqueue.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/power_supply.h>

#include <linux/delay.h>
#include <linux/kthread.h>
#include <linux/slab.h>

#include <linux/seq_file.h>
#include <linux/input.h>
#include <asm/div64.h>
#include <mach/gpio.h>
#include <mach/iomux.h>
#ifdef CONFIG_HAS_EARLYSUSPEND
#include <linux/earlysuspend.h>
#endif

struct wake_lock batt_wake_lock;
extern int dwc_otg_check_dpdm(void);
extern int charger_in_logo;
extern int dwc_vbus_status(void);
extern int dwc_otg_get_phy_status(void);
extern void rk30_pm_power_off(void);


int pc_or_charger = -1;
int otg_phy_suspend = -1;

#define DBG_AXP_PSY 1
#if  DBG_AXP_PSY
#define DBG_PSY_MSG(format,args...)   printk(KERN_DEBUG "[AXP]"format,##args)
#else
#define DBG_PSY_MSG(format,args...)   do {} while (0)
#endif

#include "axp-sply.h"
static void axp_charger_update_state(struct axp_charger *charger);
static void axp_charger_update(struct axp_charger *charger);
static int axp_ibat_to_mA(uint16_t reg);
static void axp_close(struct axp_charger *charger);
#include "axp-init.h"
#include "axp-debug.h"
#include "axp-noirq.h"

#include "axp-calocv.h"
#include "axp-calcou.h"

#ifdef CONFIG_HAS_EARLYSUSPEND
static struct early_suspend axp_early_suspend;
#endif

static inline int axp_vbat_to_mV(uint16_t reg)
{
	return (reg) * 1100 / 1000;
}

static inline int axp_vdc_to_mV(uint16_t reg)
{
	return (reg) * 1700 / 1000;
}

static inline int axp_ibat_to_mA(uint16_t reg)
{
	return (reg) * 500 / 1000;
}

static inline int axp_iac_to_mA(uint16_t reg)
{
	return (reg) * 625 / 1000;
}

static inline int axp_iusb_to_mA(uint16_t reg)
{
	return (reg) * 375 / 1000;
}

static inline void axp_read_adc(struct axp_charger *charger,
  struct axp_adc_res *adc)
{
	uint8_t tmp[8];
	axp_reads(charger->master,AXP_VACH_RES,8,tmp);
	adc->vac_res = ((uint16_t) tmp[0] << 4 )| (tmp[1] & 0x0f);
	adc->iac_res = ((uint16_t) tmp[2] << 4 )| (tmp[3] & 0x0f);
	adc->vusb_res = ((uint16_t) tmp[4] << 4 )| (tmp[5] & 0x0f);
	adc->iusb_res = ((uint16_t) tmp[6] << 4 )| (tmp[7] & 0x0f);
	axp_reads(charger->master,AXP_VBATH_RES,6,tmp);
	adc->vbat_res = ((uint16_t) tmp[0] << 4 )| (tmp[1] & 0x0f);
#if defined (CONFIG_KP_AXP20)
	adc->ichar_res = ((uint16_t) tmp[2] << 4 )| (tmp[3] & 0x0f);
#endif
#if defined (CONFIG_KP_AXP19)
	adc->ichar_res = ((uint16_t) tmp[2] << 5 )| (tmp[3] & 0x1f);
#endif
	adc->idischar_res = ((uint16_t) tmp[4] << 5 )| (tmp[5] & 0x1f);
}

static void axp_charger_update_state(struct axp_charger *charger)
{
	uint8_t val[2];
	uint16_t tmp;

	axp_reads(charger->master,AXP_CHARGE_STATUS,2,val);
	tmp = (val[1] << 8 )+ val[0];
	charger->is_on 					= (tmp & AXP_STATUS_INCHAR)?1:0;
	charger->bat_det 				= (tmp & AXP_STATUS_BATEN)?1:0;
	charger->ac_det 				= (tmp & AXP_STATUS_ACEN)?1:0;
	charger->usb_det 				= (tmp & AXP_STATUS_USBEN)?1:0;
	charger->usb_valid 				= (tmp & AXP_STATUS_USBVA)?1:0;
	charger->ac_valid 				= (tmp & AXP_STATUS_ACVA)?1:0;
	charger->ext_valid 				= charger->ac_valid | charger->usb_valid;
	charger->bat_current_direction 	= (tmp & AXP_STATUS_BATCURDIR)?1:0;
	charger->in_short 				= (tmp& AXP_STATUS_ACUSBSH)?1:0;
	charger->batery_active 			= (tmp & AXP_STATUS_BATINACT)?1:0;
	charger->low_charge_current 	= (tmp & AXP_STATUS_CHACURLOEXP)?1:0;
	charger->int_over_temp 			= (tmp & AXP_STATUS_ICTEMOV)?1:0;
	axp_read(charger->master,AXP_CHARGE_CONTROL1,val);
	charger->charge_on 				= ((val[0] & 0x80)?1:0);

	//printk("fun %s , line %d , charger->usb_valid  = %d , charger->ac_valid = %d\n",
	//		__FUNCTION__, __LINE__, charger->usb_valid ,charger->ac_valid );
}

static void axp_charger_update(struct axp_charger *charger)
{
	uint16_t tmp;	
	axp_read_adc(charger, &charger->adc);
	tmp = charger->adc.vbat_res;
	charger->vbat = axp_vbat_to_mV(tmp);
	charger->ibat = ABS(axp_ibat_to_mA(charger->adc.ichar_res)-axp_ibat_to_mA(charger->adc.idischar_res));
	tmp = charger->adc.vac_res;
	charger->vac = axp_vdc_to_mV(tmp);
	tmp = charger->adc.iac_res;
	charger->iac = axp_iac_to_mA(tmp);
	tmp = charger->adc.vusb_res;
	charger->vusb = axp_vdc_to_mV(tmp);
	tmp = charger->adc.iusb_res;
	charger->iusb = axp_iusb_to_mA(tmp);
	if(!charger->ext_valid){
		charger->disvbat =  charger->vbat;
		charger->disibat =  charger->ibat;
	}
}

static enum power_supply_property axp_battery_props[] = {
	POWER_SUPPLY_PROP_MODEL_NAME,
	POWER_SUPPLY_PROP_STATUS,
	POWER_SUPPLY_PROP_PRESENT,
	//POWER_SUPPLY_PROP_ONLINE,
	POWER_SUPPLY_PROP_HEALTH,
	POWER_SUPPLY_PROP_TECHNOLOGY,
	POWER_SUPPLY_PROP_VOLTAGE_MAX_DESIGN,
	POWER_SUPPLY_PROP_VOLTAGE_MIN_DESIGN,
	POWER_SUPPLY_PROP_VOLTAGE_NOW,
	POWER_SUPPLY_PROP_CURRENT_NOW,
	POWER_SUPPLY_PROP_ENERGY_FULL_DESIGN,
	POWER_SUPPLY_PROP_CAPACITY,
	POWER_SUPPLY_PROP_TEMP,
};

static enum power_supply_property axp_ac_props[] = {
	POWER_SUPPLY_PROP_MODEL_NAME,
	POWER_SUPPLY_PROP_PRESENT,
	POWER_SUPPLY_PROP_ONLINE,
	POWER_SUPPLY_PROP_VOLTAGE_NOW,
	POWER_SUPPLY_PROP_CURRENT_NOW,
};

static enum power_supply_property axp_usb_props[] = {
	POWER_SUPPLY_PROP_MODEL_NAME,
	POWER_SUPPLY_PROP_PRESENT,
	POWER_SUPPLY_PROP_ONLINE,
	POWER_SUPPLY_PROP_VOLTAGE_NOW,
	POWER_SUPPLY_PROP_CURRENT_NOW,
};

static void axp_battery_check_status(struct axp_charger *charger,
            union power_supply_propval *val)
{
	if (charger->bat_det) {
		if (charger->ext_valid){
			if( charger->rest_cap == 100)
				val->intval = POWER_SUPPLY_STATUS_FULL;
			else if(charger->charge_on)
    			val->intval = POWER_SUPPLY_STATUS_CHARGING;
			else
				val->intval = POWER_SUPPLY_STATUS_NOT_CHARGING;
		} else {
			val->intval = POWER_SUPPLY_STATUS_DISCHARGING;
		}
	} else {
		val->intval = POWER_SUPPLY_STATUS_FULL;
	}
}

static void axp_battery_check_health(struct axp_charger *charger,
            union power_supply_propval *val)
{
	val->intval = POWER_SUPPLY_HEALTH_GOOD;
}

static int axp_battery_get_property(struct power_supply *psy,
           enum power_supply_property psp,
           union power_supply_propval *val)
{
	struct axp_charger *charger;
	int ret = 0;
	charger = container_of(psy, struct axp_charger, batt);

	switch (psp) {
		case POWER_SUPPLY_PROP_STATUS:
			axp_battery_check_status(charger, val);break;
		case POWER_SUPPLY_PROP_HEALTH:
			axp_battery_check_health(charger, val);break;
		case POWER_SUPPLY_PROP_TECHNOLOGY:
			val->intval = charger->battery_info->technology;break;
		case POWER_SUPPLY_PROP_VOLTAGE_MAX_DESIGN:
			val->intval = charger->battery_info->voltage_max_design;break;
		case POWER_SUPPLY_PROP_VOLTAGE_MIN_DESIGN:
			val->intval = charger->battery_info->voltage_min_design;break;
		case POWER_SUPPLY_PROP_VOLTAGE_NOW:
			val->intval = charger->ocv * 1000;break;
		case POWER_SUPPLY_PROP_CURRENT_NOW:
			val->intval = charger->ibat * 1000;break;
		case POWER_SUPPLY_PROP_MODEL_NAME:
			val->strval = charger->batt.name;break;
		case POWER_SUPPLY_PROP_ENERGY_FULL_DESIGN:
			val->intval = charger->battery_info->energy_full_design;break;
		case POWER_SUPPLY_PROP_CAPACITY:
			val->intval = charger->rest_cap;break;
		//case POWER_SUPPLY_PROP_ONLINE:
		//	val->intval = (!charger->is_on)&&(charger->bat_det) && (! charger->ext_valid);break;
		case POWER_SUPPLY_PROP_PRESENT:
			val->intval = charger->bat_det;break;
		case POWER_SUPPLY_PROP_TEMP:
			val->intval =  300;break;
		default:
			ret = -EINVAL;break;
	}

	return ret;
}

static int axp_ac_get_property(struct power_supply *psy,
           enum power_supply_property psp,
           union power_supply_propval *val)
{
	struct axp_charger *charger;
	int ret = 0;
	charger = container_of(psy, struct axp_charger, ac);

	switch(psp){
		case POWER_SUPPLY_PROP_MODEL_NAME:
			val->strval = charger->ac.name;break;
		case POWER_SUPPLY_PROP_PRESENT:
			val->intval = charger->ac_det;break;
		case POWER_SUPPLY_PROP_ONLINE:
			val->intval = charger->ac_valid;break;
		case POWER_SUPPLY_PROP_VOLTAGE_NOW:
			val->intval = charger->vac * 1000;break;
		case POWER_SUPPLY_PROP_CURRENT_NOW:
			val->intval = charger->iac * 1000;break;
		default:
			ret = -EINVAL;break;
	}
	return ret;
}

static int axp_usb_get_property(struct power_supply *psy,
           enum power_supply_property psp,
           union power_supply_propval *val)
{
	struct axp_charger *charger;
	int ret = 0;
	charger = container_of(psy, struct axp_charger, usb);

	switch(psp){
		case POWER_SUPPLY_PROP_MODEL_NAME:
			val->strval = charger->usb.name;break;
		case POWER_SUPPLY_PROP_PRESENT:
			val->intval = charger->usb_det;break;
		case POWER_SUPPLY_PROP_ONLINE:
			val->intval = charger->usb_valid;break;
		case POWER_SUPPLY_PROP_VOLTAGE_NOW:
			val->intval = charger->vusb * 1000;break;
		case POWER_SUPPLY_PROP_CURRENT_NOW:
			val->intval = charger->iusb * 1000;break;
		default:
			ret = -EINVAL;break;
	}
	return ret;
}

extern void axp_power_off();
static void axp_change(struct axp_charger *charger)
{
	printk("battery state change\n");
	axp_charger_update_state(charger);
	axp_charger_update(charger);
	state_count = 1;
	power_supply_changed(&charger->batt);

	if (charger_in_logo == 1) {
		 power_supply_changed(&charger->usb);
        	 power_supply_changed(&charger->ac);
		 if(charger->usb_valid == 0 && charger->ac_valid == 0 )
		 {
			rk30_pm_power_off();
		 }
	}
}

static void axp_presslong(struct axp_charger *charger)
{
	printk("press long\n");
#if defined (CONFIG_AXP_PEKINFO)
	input_report_key(powerkeydev, KEY_POWER, 1);
	input_sync(powerkeydev);
	ssleep(2);
	printk("press long up\n");
	input_report_key(powerkeydev, KEY_POWER, 0);
	input_sync(powerkeydev);
#endif
}

static void axp_pressshort(struct axp_charger *charger)
{
	printk("press short\n");
#if defined (CONFIG_AXP_PEKINFO)
	input_report_key(powerkeydev, KEY_POWER, 1);
	input_sync(powerkeydev);
	msleep(100);
	input_report_key(powerkeydev, KEY_POWER, 0);
	input_sync(powerkeydev);
#endif
}

static void axp_capchange(struct axp_charger *charger)
{
	printk("battery change\n");
	ssleep(2);
	axp_charger_update_state(charger);
	axp_charger_update(charger);
	if(charger->bat_det == 1){
		axp_battery_infoinit(charger);
		schedule_delayed_work(&charger->work, charger->interval);
	} else {
		charger->rest_cap = 100;
		charger->ocv = 0;
		cancel_delayed_work_sync(&charger->work);
	}
	power_supply_changed(&charger->batt);
	if (charger_in_logo == 1) {
		power_supply_changed(&charger->usb);
         power_supply_changed(&charger->ac);
	}
}

static void axp_close(struct axp_charger *charger)
{
	charger->rest_cap = SHUTDOWNRATE;
#if defined (CONFIG_KP_AXP20)
	axp_write(charger->master,AXP_DATA_BUFFER1,(0x80|SHUTDOWNRATE));
#endif
	printk("\n==================event in close==============\n");
	power_supply_changed(&charger->batt);
	if (charger_in_logo == 1) {
		power_supply_changed(&charger->usb);
         power_supply_changed(&charger->ac);
	}
}

#if defined (CONFIG_KP_USEIRQ)
static int axp_battery_event(struct notifier_block *nb, unsigned long event,
        void *data)
{
	struct axp_charger *charger =
	container_of(nb, struct axp_charger, nb);
#if defined (CONFIG_KP_AXP20)
	uint8_t w[9];
#endif
#if defined (CONFIG_KP_AXP19)
	uint8_t w[7];
#endif
	w[0] = (uint8_t) ((event) & 0xFF);
	w[1] = AXP_INTSTS2;
	w[2] = (uint8_t) ((event >> 8) & 0xFF);
	w[3] = AXP_INTSTS3;
	w[4] = (uint8_t) ((event >> 16) & 0xFF);
	w[5] = AXP_INTSTS4;
	w[6] = (uint8_t) ((event >> 24) & 0xFF);
#if defined (CONFIG_KP_AXP20)
	w[7] = AXP_INTSTS5;
	w[8] = (uint8_t) (((uint64_t) event >> 32) & 0xFF);
#endif

	if(event & (AXP_IRQ_BATIN|AXP_IRQ_BATRE)) {
		axp_capchange(charger);
	}


	//if (charger_in_logo != 0) {
	if(0){
		pc_or_charger = dwc_otg_check_dpdm();
	} else {
		pc_or_charger = dwc_vbus_status();
	}
	otg_phy_suspend = dwc_otg_get_phy_status();

	//printk("cdy---pc_or_charger = %d,otg_phy_suspend = %d ,\n",pc_or_charger,otg_phy_suspend );

	if(event & (AXP_IRQ_ACIN|AXP_IRQ_USBIN|AXP_IRQ_ACOV|AXP_IRQ_USBOV|AXP_IRQ_CHAOV
				|AXP_IRQ_CHAST|AXP_IRQ_TEMOV|AXP_IRQ_TEMLO)) {
		if(event & AXP_IRQ_USBIN)
		{
			if((otg_phy_suspend == 0) && (pc_or_charger == 0))
			{	
			
			}else
			{
				axp_change(charger);
			}
		}else
		{
			axp_change(charger);
		}
	}

	if(event & (AXP_IRQ_ACRE|AXP_IRQ_USBRE)) {
		if(event & AXP_IRQ_USBRE)
		{
			if((otg_phy_suspend == 0) && (pc_or_charger == 0))
			{
				
			}else
			{
				axp_change(charger);
			}
		}else
		{
			axp_change(charger);
		}
		/*if (charger_in_logo == 1) {
		 	if(charger->usb_valid == 0 && charger->ac_valid == 0 )
			 {
		 		printk("cdy #### AXP_IRQ_USBRE--- poweroff\n");
				axp_power_off();
		 	}
		}*/
	}

	if(event & AXP_IRQ_PEKLO) {
		axp_presslong(charger);
	}

	if(event & AXP_IRQ_PEKSH) {
		axp_pressshort(charger);
	}

	printk("event = 0x%x\n",(int) event);
#if defined (CONFIG_KP_AXP20)
	axp_writes(charger->master,AXP_INTSTS1,9,w);
#endif
#if defined (CONFIG_KP_AXP19)
	axp_writes(charger->master,AXP_INTSTS1,7,w);
#endif
	return 0;
}
#endif

static char *supply_list[] = {
	"battery",
};  

static void axp_battery_setup_psy(struct axp_charger *charger)
{
	struct power_supply *batt = &charger->batt;
	struct power_supply *ac = &charger->ac;
	struct power_supply *usb = &charger->usb;
	struct power_supply_info *info = charger->battery_info;

	batt->name = "battery";
	batt->use_for_apm = info->use_for_apm;
	batt->type = POWER_SUPPLY_TYPE_BATTERY;
	batt->get_property = axp_battery_get_property;

	batt->properties = axp_battery_props;
	batt->num_properties = ARRAY_SIZE(axp_battery_props);

	ac->name = "ac";
	ac->type = POWER_SUPPLY_TYPE_MAINS;
	ac->get_property = axp_ac_get_property;

	ac->supplied_to = supply_list,
	ac->num_supplicants = ARRAY_SIZE(supply_list),

	ac->properties = axp_ac_props;
	ac->num_properties = ARRAY_SIZE(axp_ac_props);

	usb->name = "usb";
	usb->type = POWER_SUPPLY_TYPE_USB;
	usb->get_property = axp_usb_get_property;

	usb->supplied_to = supply_list,
	usb->num_supplicants = ARRAY_SIZE(supply_list),

	usb->properties = axp_usb_props;
	usb->num_properties = ARRAY_SIZE(axp_usb_props);
};

#if defined CONFIG_HAS_EARLYSUSPEND
static void axp_earlysuspend(struct early_suspend *h)
{
	printk("======early suspend=======\n");
#if defined (CONFIG_AXP_CHGCURCHG) || (CONFIG_AXP_DEBUG)
	axp_set_chargecurrent(axpcharger->chgearcur);
#endif
}
static void axp_lateresume(struct early_suspend *h)
{
	printk("======late resume=======\n");
#if defined (CONFIG_AXP_CHGCURCHG) || (CONFIG_AXP_DEBUG)
	axp_set_chargecurrent(axpcharger->chgcur);
#endif
}
#endif

static int axp_battery_probe(struct platform_device *pdev)
{
	struct axp_charger *charger;
	struct axp_supply_init_data *pdata = pdev->dev.platform_data;
	int ret;

	if (pdata == NULL)
		return -EINVAL;

	/* 设置pek按键消息 */
	ret = axp_set_pekinfo(pdev);
	if(ret){
		printk("[AXP]Unable to set pek input device\n");
		return ret;
	}

	charger = kzalloc(sizeof(*charger), GFP_KERNEL);
	if (charger == NULL)
		return -ENOMEM;

	charger->master = pdev->dev.parent;

	charger->chgcur			= pdata->chgcur;
	charger->chgearcur		= pdata->chgearcur;
	charger->chgsuscur		= pdata->chgsuscur;
	charger->chgclscur		= pdata->chgclscur;
	charger->chgvol			= pdata->chgvol;
	charger->chgend			= pdata->chgend;
	charger->adc_freq		= pdata->adc_freq;
	charger->chgpretime		= pdata->chgpretime;
	charger->chgcsttime		= pdata->chgcsttime;
	charger->battery_info	= pdata->battery_info;
	
	/* 设置全局变量，用于调试 */
	axpcharger = charger;

	/* 初始化设置 */
	ret = axp_battery_first_init(charger);
	if (ret)
		goto err_charger_init;
#if defined (CONFIG_KP_USEIRQ)
	charger->nb.notifier_call = axp_battery_event;
	ret = axp_register_notifier(charger->master, &charger->nb, AXP_NOTIFIER_ON);
	if (ret)
		goto err_notifier;
#endif

	axp_battery_setup_psy(charger);
	ret = power_supply_register(&pdev->dev, &charger->batt);
	if (ret)
		goto err_ps_register;

	ret = power_supply_register(&pdev->dev, &charger->ac);
	if (ret){
		power_supply_unregister(&charger->batt);
		goto err_ps_register;
	}
  	
	ret = power_supply_register(&pdev->dev, &charger->usb);
	if (ret){
		power_supply_unregister(&charger->ac);
		power_supply_unregister(&charger->batt);
		goto err_ps_register;
	}

#if defined (CONFIG_AXP_CHGCURCHG) || (CONFIG_AXP_DEBUG)
	ret = axp_charger_create_attrs(&charger->batt);
	if(ret){
		return ret;
	}
#endif

	platform_set_drvdata(pdev, charger);

	/* 初始化显示电池信息 */
	axp_charger_update_state(charger);
	axp_charger_update(charger);
	if(charger->bat_det == 1)
		axp_battery_infoinit(charger);

	/* 创建更新电池信息队列 */
	charger->interval = msecs_to_jiffies(TIMER1 * 1000);
	INIT_DELAYED_WORK(&charger->work, axp_charging_monitor);
	if(charger->bat_det == 1){
		schedule_delayed_work(&charger->work, charger->interval);
	} else {
		charger->rest_cap = 100;
	}
	
#ifndef CONFIG_KP_USEIRQ
	/* 未打开中断时，轮询检测 */
	charger->intervalstate = msecs_to_jiffies(TIMER6 * 100);
	INIT_DELAYED_WORK(&charger->workstate, axp_chargingstate_monitor);
	schedule_delayed_work(&charger->workstate, charger->intervalstate);
#endif

#ifdef CONFIG_HAS_EARLYSUSPEND
	axp_early_suspend.suspend = axp_earlysuspend;
	axp_early_suspend.resume = axp_lateresume;
	axp_early_suspend.level = EARLY_SUSPEND_LEVEL_BLANK_SCREEN + 2;
	register_early_suspend(&axp_early_suspend);
#endif
	/* 调试接口注册 */
	class_register(&axppower_class);

	rk30_mux_api_set(0, GPIO4D_GPIO4D7);
	/*ret = gpio_request(GPIO4D_GPIO4D7, NULL);
	if (ret != 0)
	{
		gpio_free(GPIO4D_GPIO4D7);
		printk(KERN_ERR "request axp n_vbusen  pin fail!\n");
		return -1;
	}else
	{
		printk("cdy ---- %s ,%d \n", __FUNCTION__, __LINE__);
		gpio_direction_output(GPIO4D_GPIO4D7, GPIO_LOW);
	}*/

	 wake_lock_init(&batt_wake_lock, WAKE_LOCK_SUSPEND, "batt_lock");

	return ret;

err_ps_register:
#if defined (CONFIG_KP_USEIRQ)
	axp_unregister_notifier(charger->master, &charger->nb, AXP_NOTIFIER_ON);
#endif

#if defined (CONFIG_KP_USEIRQ)
err_notifier:
#endif
	cancel_delayed_work_sync(&charger->work);

err_charger_init:
	kfree(charger);
	input_unregister_device(powerkeydev);
	kfree(powerkeydev);

	return ret;
}

static int axp_battery_remove(struct platform_device *dev)
{
	struct axp_charger *charger = platform_get_drvdata(dev);
#if defined (CONFIG_KP_USEIRQ)
	axp_unregister_notifier(charger->master, &charger->nb, AXP_NOTIFIER_ON);
#endif
	cancel_delayed_work_sync(&charger->work);
#ifndef CONFIG_KP_USEIRQ
	cancel_delayed_work_sync(&charger->workstate);
#endif
	power_supply_unregister(&charger->usb);
	power_supply_unregister(&charger->ac);
	power_supply_unregister(&charger->batt);

	kfree(charger);
	input_unregister_device(powerkeydev);
	kfree(powerkeydev);

	return 0;
}

static void axp_shutdown(struct platform_device *dev)
{
	struct axp_charger *charger = platform_get_drvdata(dev);

	cancel_delayed_work_sync(&charger->work);
#ifndef CONFIG_KP_USEIRQ
	cancel_delayed_work_sync(&charger->workstate);
#endif
	axp_set_rdcclose();
	/*设置关机充电电流*/
#if defined (CONFIG_AXP_CHGCURCHG) || (CONFIG_AXP_DEBUG)
	axp_set_chargecurrent(charger->chgclscur);
#endif
}

static struct platform_driver axp_battery_driver = {
	.driver 	= {
		.name 	= "axp-supplyer",
		.owner	= THIS_MODULE,
	},
	.probe 		= axp_battery_probe,
	.remove 	= axp_battery_remove,
	.suspend	= axp_suspend,
	.resume 	= axp_resume,
	.shutdown	= axp_shutdown,
};

static int axp_battery_init(void)
{
	return platform_driver_register(&axp_battery_driver);
}

static void axp_battery_exit(void)
{
	platform_driver_unregister(&axp_battery_driver);
}

subsys_initcall(axp_battery_init);
module_exit(axp_battery_exit);

MODULE_DESCRIPTION("Battery Driver for X-Powers AXP PMIC");
MODULE_AUTHOR("Donglu Zhang, <zhangdonglu@x-powers.com>");
MODULE_LICENSE("GPL");
