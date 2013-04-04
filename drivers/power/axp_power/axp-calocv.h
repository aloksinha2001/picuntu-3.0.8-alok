#if defined (CONFIG_USE_OCV) 
static void axp_battery_infoinit(struct axp_charger *charger)
{
	uint8_t val1,val2;
	int Cur_CoulombCounter,k;
	
	axp_charger_update_state(charger);
	axp_charger_update(charger);
#if defined (CONFIG_KP_AXP20)
	axp_read(charger->master,AXP_DATA_BUFFER1,&val1);
	axp_read(charger->master,AXP_CAP,&val2);
#endif
#if defined (CONFIG_KP_AXP19)
	axp_read(charger->master,AXP_DATA_BUFFER0,&val1);
	val2 = axp_ocv_restcap(charger);
#endif

	Cur_CoulombCounter = ABS(axp_get_coulomb(charger));

	if(ABS((val1 & 0x7F)-(val2 & 0x7F)) >= 10 || Cur_CoulombCounter > 50)
		charger->rest_cap = (int) (val2 & 0x7F);
	else
		charger->rest_cap = (int) (val1 & 0x7F);
	
	if((charger->bat_det == 0) || (charger->rest_cap == 127)){
		charger->rest_cap = 100;
	}

	printk("last_rest_cap = %d, now_rest_cap = %d\n",(val1 & 0x7F),(val2 & 0x7F));
	memset(Bat_Cap_Buffer, 0, sizeof(Bat_Cap_Buffer));
	for(k = 0;k < AXP_VOL_MAX; k++){
		Bat_Cap_Buffer[k] = charger->rest_cap;
	}
	
	Total_Cap = charger->rest_cap * AXP_VOL_MAX;
}

static void axp_charging_monitor(struct work_struct *work)
{
	struct axp_charger *charger;
	uint8_t val;
	uint8_t v[5];
	int pre_rest_cap;
	int rdc,k;
	int rt_rest_cap;
	int rest_cap;
	int cap_index_p;
#if defined (CONFIG_AXP_DEBUG)
	uint8_t data_mm[AXP_DATA_NUM];
	int mm;
#endif

	charger = container_of(work, struct axp_charger, work.work);
	/* 更新电池信息 */
	pre_rest_cap = charger->rest_cap;
	axp_charger_update_state(charger);
	axp_charger_update(charger);

	/* 计算电池内阻rdc */
	axp_cal_rdc(charger);

#if defined (CONFIG_KP_AXP19)
	val = axp_ocv_restcap(charger);
#endif

	if(state_count){
#if defined (CONFIG_KP_AXP20)
		axp_read(charger->master, AXP_CAP,&val);
#endif
		rt_rest_cap = (int) (val & 0x7F);
		rest_cap = charger->rest_cap;
		state_count ++;
		if(state_count >= TIMER2){
			state_count = 0;
		}
	} else {
#if defined (CONFIG_KP_AXP20)
		axp_read(charger->master, AXP_CAP,&val);
#endif
		rt_rest_cap = (int) (val & 0x7F);
		if((charger->bat_det == 0) || (rt_rest_cap == 127) ){
			rt_rest_cap = 100;
		}

		Total_Cap -= Bat_Cap_Buffer[Cap_Index];
		if(Cap_Index == 0){
			cap_index_p = AXP_VOL_MAX - 1;
		} else {
			cap_index_p = Cap_Index - 1;
		}
		if(ABS(rt_rest_cap - Bat_Cap_Buffer[cap_index_p]) > 5){
			if(axp_debug)
				DBG_PSY_MSG("correct rdc\n");
			axp_clr_bits(charger->master,AXP_DATA_BUFFER0,0x08);
		}

		Bat_Cap_Buffer[Cap_Index] = rt_rest_cap;
		Total_Cap += Bat_Cap_Buffer[Cap_Index];
		Cap_Index++;
		if(Cap_Index == AXP_VOL_MAX){
			Cap_Index = 0;
		}

		rest_cap = (Total_Cap + AXP_VOL_MAX / 2 ) / AXP_VOL_MAX;

if(axp_debug){
		for(k = 0;k < AXP_VOL_MAX ; k++){
			DBG_PSY_MSG("Bat_Cap_Buffer[%d] = %d\n",k,Bat_Cap_Buffer[k]);
		}
}
		if(axp_debug)
			DBG_PSY_MSG("Before Modify:Cap_Index = %d,val = 0x%x,pre_rest_cap = %d,rest_cap = %d\n",Cap_Index,val,pre_rest_cap,rest_cap);

		if(charger->is_on && (rest_cap < pre_rest_cap)){
			rest_cap = pre_rest_cap;
		} else if (!charger->ext_valid && (rest_cap > pre_rest_cap)){
			rest_cap = pre_rest_cap;
		}
		if(axp_debug)
			DBG_PSY_MSG("After Modify:val = 0x%x,pre_rest_cap = %d,rest_cap = %d\n",val,pre_rest_cap,rest_cap);

		/* full */
		if(charger->ocv >= 4100 && !charger->is_on && charger->ext_valid && charger->charge_on){
			rest_cap = 100;
			for(k = 0;k < AXP_VOL_MAX; k++){
				Bat_Cap_Buffer[k] = rest_cap;
			}
			Total_Cap = rest_cap * AXP_VOL_MAX;
			charger->bat_current_direction = 1;
		}

		/* charging*/
		if(charger->is_on && rest_cap == 100){
			rest_cap = 99;
		}

		if(rest_cap > pre_rest_cap){
			if(cap_count >= TIMER5){
				charger->rest_cap++;
				cap_count = 0;
			} else {
				cap_count++;
			}
		} else if(rest_cap < pre_rest_cap){
			if(cap_count >= TIMER5){
				charger->rest_cap --;
				cap_count = 0;
			} else {
				cap_count++;
			}
		} else {
			cap_count = 0;
		}
	}
#if defined (CONFIG_KP_AXP20)
	/* 更新电池开路电压 */
	axp_reads(charger->master,AXP_OCV_BUFFER0,2,v);
	charger->ocv = ((v[0] << 4) + (v[1] & 0x0f)) * 11 /10 ;
	/* usb 限流 */
	axp_usb_curlim(USBCURLIM);
#endif
	
	if(charge_index >= TIMER4){
		charger->disvbat = 0;
		charger->disibat = 0;
		charge_index = 0;
	}		
	if(charger->ext_valid == 1)
		charge_index ++;
	else 
		charge_index = 0;

if(axp_debug){
	DBG_PSY_MSG("charger->ic_temp = %d\n",charger->ic_temp);
	DBG_PSY_MSG("charger->vbat = %d\n",charger->vbat);
	DBG_PSY_MSG("charger->ibat = %d\n",charger->ibat);
	DBG_PSY_MSG("charger->disvbat = %d\n",charger->disvbat);
	DBG_PSY_MSG("charger->disibat = %d\n",charger->disibat);
	DBG_PSY_MSG("charger->vusb = %d\n",charger->vusb);
	DBG_PSY_MSG("charger->iusb = %d\n",charger->iusb);
	DBG_PSY_MSG("charger->vac = %d\n",charger->vac);
	DBG_PSY_MSG("charger->iac = %d\n",charger->iac);
	DBG_PSY_MSG("charger->ocv = %d\n",charger->ocv);
	DBG_PSY_MSG("charger->chgcur = %d\n",charger->chgcur);
	DBG_PSY_MSG("charger->chgearcur = %d\n",charger->chgearcur);
	DBG_PSY_MSG("charger->chgsuscur = %d\n",charger->chgsuscur);
	DBG_PSY_MSG("charger->chgclscur = %d\n",charger->chgclscur);
	DBG_PSY_MSG("rt_rest_cap = %d\n",rt_rest_cap);
	DBG_PSY_MSG("rest_cap = %d\n",rest_cap);
	DBG_PSY_MSG("charger->rest_cap = %d\n",charger->rest_cap);
#if defined (CONFIG_KP_AXP20)
	axp_reads(charger->master,AXP_RDC_BUFFER0,2,v);
	rdc = (((v[0] & 0x1F) << 8) | v[1]) * 10742 / 10000;
	DBG_PSY_MSG("rdc = %d\n",rdc);
#endif
#if defined (CONFIG_KP_AXP19)
	axp_reads(charger->master,AXP_RDC_BUFFER0,2,v);
	rdc = v[1]*2;
	DBG_PSY_MSG("rdc = %d,flag = 0x%x\n",rdc,v[0]);
#endif
	DBG_PSY_MSG("charger->is_on = %d\n",charger->is_on);
	DBG_PSY_MSG("charger->charge_on = %d\n",charger->charge_on);
	DBG_PSY_MSG("charger->ext_valid = %d\n",charger->ext_valid);
	DBG_PSY_MSG("charge_index = %d\n",charge_index);
	DBG_PSY_MSG("state_count = %d\n",state_count);
	DBG_PSY_MSG("rdc_count = %d\n",rdc_count);
	DBG_PSY_MSG("cap_count = %d\n",cap_count);
#if defined (CONFIG_AXP_DEBUG)
	axp_reads(charger->master,AXP_DATA_BUFFER0,AXP_DATA_NUM,data_mm);
	for( mm = 0; mm < AXP_DATA_NUM; mm++){
		DBG_PSY_MSG("REG[0x%x] = 0x%x\n",mm+AXP_DATA_BUFFER0,data_mm[mm]);	
	}
#endif
}

	/* if battery volume changed, inform uevent */
	if(charger->rest_cap - pre_rest_cap){
		printk("battery vol change: %d->%d \n", pre_rest_cap, charger->rest_cap);
		pre_rest_cap = charger->rest_cap;
#if defined (CONFIG_KP_AXP20)
		axp_write(charger->master,AXP_DATA_BUFFER1,charger->rest_cap | 0x80);
#endif
#if defined (CONFIG_KP_AXP19)
		axp_write(charger->master,AXP_DATA_BUFFER0,charger->rest_cap | 0x80);
#endif
		power_supply_changed(&charger->batt);
	}

	/* reschedule for the next time */
	schedule_delayed_work(&charger->work, charger->interval);
}

static int axp_suspend(struct platform_device *dev, pm_message_t state)
{
	struct axp_charger *charger = platform_get_drvdata(dev);
	
	if(charger->bat_det == 1)
		cancel_delayed_work_sync(&charger->work);
	
	/* 清irq */
	axp_clear_irq(charger);

	/* 关闭irq */
#if defined (CONFIG_KP_USEIRQ)
	//axp_unregister_notifier(charger->master, &charger->nb, AXP_NOTIFIER_ON);
#endif

	/* 设置休眠充电电流*/
#if defined (CONFIG_AXP_CHGCURCHG)
	axp_set_chargecurrent(charger->chgsuscur);
#endif

	/* 设置阈值时间 */
	axp_write(charger->master, AXP_TIMER_CTL, 0x80);
	axp_write(charger->master, AXP_TIMER_CTL, 0x02);

	/* 清库仑计 */
	axp_clr_coulomb(charger);
	
    return 0;
}

static int axp_resume(struct platform_device *dev)
{
	struct axp_charger *charger = platform_get_drvdata(dev);

	int pre_rest_cap,k;
	uint8_t val,val1;
#if defined (CONFIG_KP_AXP20)
	uint8_t v[2];
#endif
	int rt_rest_cap;
	int Cur_CoulombCounter;

	/* 打开irq */
#if defined (CONFIG_KP_USEIRQ)
	//axp_register_notifier(charger->master, &charger->nb, AXP_NOTIFIER_ON);
#endif

	axp_charger_update_state(charger);
	if(charger->bat_det == 1){
#if defined (CONFIG_KP_AXP20)
		axp_reads(charger->master,0xbc,2,v);
		charger->ocv = ((v[0] << 4) + (v[1] & 0x0f)) * 11 /10 ;

		axp_read(charger->master, AXP_TIMER_CTL, &val1);
		pre_rest_cap = charger->rest_cap;

		axp_read(charger->master, AXP_CAP,&val);
#endif
#if defined (CONFIG_KP_AXP19)
		axp_charger_update(charger);

		axp_read(charger->master, AXP_TIMER_CTL, &val1);
		pre_rest_cap = charger->rest_cap;
		val = axp_ocv_restcap(charger);
#endif

		Cur_CoulombCounter = (axp_get_coulomb(charger));

		if((val1 >> 7)){
			charger->rest_cap = (int) (val & 0x7F);
			if((charger->bat_det == 0) || (charger->rest_cap == 127)){
				charger->rest_cap = 100;
			}

			if ((ABS(Cur_CoulombCounter) <= 5) && (charger->rest_cap > pre_rest_cap)){
				charger->rest_cap = pre_rest_cap;
			}

			if( ((pre_rest_cap - charger->rest_cap) < 10) && (pre_rest_cap > charger->rest_cap )){
				charger->rest_cap = --pre_rest_cap;
			}

			if(charger->is_on && (charger->rest_cap < pre_rest_cap)){
				charger->rest_cap = pre_rest_cap;
			}
			if(axp_debug)
				DBG_PSY_MSG("val = 0x%x,pre_rest_cap = %d,rest_cap = %d\n",val,pre_rest_cap,charger->rest_cap);

			/* full */
			if((charger->ocv) >= 4100 && !charger->is_on && charger->ext_valid && charger->charge_on){
				charger->rest_cap = 100;
			}

			/* charging*/
			if(charger->is_on && charger->rest_cap == 100){
				charger->rest_cap = 99;
			}

			memset(Bat_Cap_Buffer, 0, sizeof(Bat_Cap_Buffer));
			for(k = 0;k < AXP_VOL_MAX; k++){
				Bat_Cap_Buffer[k] = charger->rest_cap;
			}
			Total_Cap = charger->rest_cap * AXP_VOL_MAX;
		} else if (charger->ext_valid && charger->charge_on){
			rt_rest_cap = (int) (val & 0x7F);
			if((pre_rest_cap < 99) && (rt_rest_cap > pre_rest_cap)){
				charger->rest_cap ++;
			}
		}

		/* if battery volume changed, inform uevent */
		if(charger->rest_cap - pre_rest_cap){
			printk("battery vol change: %d->%d \n", pre_rest_cap, charger->rest_cap);
			pre_rest_cap = charger->rest_cap;
#if defined (CONFIG_KP_AXP20)
			axp_write(charger->master,AXP_DATA_BUFFER1,charger->rest_cap | 0x80);
#endif
#if defined (CONFIG_KP_AXP19)
			axp_write(charger->master,AXP_DATA_BUFFER0,charger->rest_cap | 0x80);
#endif
			power_supply_changed(&charger->batt);
		}
	} else {
		charger->rest_cap = 100;
		charger->ocv = 0;
		power_supply_changed(&charger->batt);	
	}
#if defined (CONFIG_KP_AXP20)
	/* 休眠低电关机接口 */
	axp_read(charger->master, AXP_DATA_BUFFERA,&val);
	printk("[AXP]suspend power off val = 0x%x\n",val);
	if(val){
		axp_close(charger);
		axp_set_bits(charger->master,AXP_INTSTS5,0x03);
	}
	axp_write(charger->master, 0x0e,0x00);
#endif

	/* 设置开机充电电流 */
#if defined (CONFIG_AXP_CHGCURCHG)
	axp_set_chargecurrent(charger->chgcur);
#endif
	if(charger->bat_det == 1)
		schedule_delayed_work(&charger->work, charger->interval);

	return 0;
}
#endif