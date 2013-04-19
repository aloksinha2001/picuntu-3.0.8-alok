#ifndef CONFIG_KP_USEIRQ
extern struct wake_lock batt_wake_lock;
static void axp_chargingstate_monitor(struct work_struct *work)
{
	struct axp_charger *charger;
	uint8_t press;
	static int pre_ac,pre_usb;
#if defined(CONFIG_KP_AXP19)
	static int press_count = -1;
#endif

	charger = container_of(work, struct axp_charger, workstate.work);

#if defined (CONFIG_KP_AXP20)	
	axp_read(charger->master,AXP_INTSTS5,&press);
	/* 按下 */
	if(press & 0x20){
		printk("press down\n");
		input_report_key(powerkeydev, KEY_POWER, 1);
		input_sync(powerkeydev);
		axp_write(charger->master,AXP_INTSTS5,press);
	}
	
	/* 放开 */
	if(press & 0x40){
		printk("press up\n");
		input_report_key(powerkeydev, KEY_POWER, 0);
		input_sync(powerkeydev);
		axp_write(charger->master,AXP_INTSTS5,press);
	}
#endif

#if defined(CONFIG_KP_AXP19)
	axp_read(charger->master,AXP_INTSTS3,&press);
	if(press_count > 0)
		press_count--;
	/* 短按 */
	if(press & 0x2){
		printk("press short\n");
		input_report_key(powerkeydev, KEY_POWER, 1);
		input_sync(powerkeydev);
		axp_write(charger->master,AXP_INTSTS3,press);
		press_count = 1;
	}
	
	if(press & 0x1){
		printk("press short\n");
		input_report_key(powerkeydev, KEY_POWER, 1);
		input_sync(powerkeydev);
		axp_write(charger->master,AXP_INTSTS3,press);
		press_count = 20;
	}
	
	if(press_count == 0){
		input_report_key(powerkeydev, KEY_POWER, 0);
		input_sync(powerkeydev);
	}
#endif
	/* 更新电池信息 */
	axp_charger_update_state(charger);

	if((pre_ac != charger->ac_valid) || (pre_usb != charger->usb_valid)){
		printk("battery state change: acvalid:%d->%d,usbvaild:%d->%d\n", pre_ac, charger->ac_valid,pre_usb, charger->usb_valid);
		pre_ac = charger->ac_valid;
		pre_usb = charger->usb_valid;
		state_count = 1;
		power_supply_changed(&charger->batt);
	}


	/* reschedule for the next time */
	schedule_delayed_work(&charger->workstate, charger->intervalstate);
}
#endif