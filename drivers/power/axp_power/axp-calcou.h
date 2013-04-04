#if defined (CONFIG_USE_COL)

extern struct wake_lock batt_wake_lock;
//extern void rk30_pm_power_off(void);
extern uint8_t axp_ocv_restcap(struct axp_charger *charger);

static void axp_set_basecap(struct axp_charger *charger, int base_cap)
{
	uint8_t val;
	if(base_cap >= 0)
		val = base_cap & 0x7F;
	else
		val = ABS(base_cap) | 0x80;
#if defined (CONFIG_KP_AXP20)
	axp_write(charger->master, AXP_DATA_BUFFER4, val);
#endif
#if defined (CONFIG_KP_AXP19)
	axp_write(charger->master, AXP_DATA_BUFFER0, val);
#endif
}

static int axp_get_basecap(struct axp_charger *charger)
{
	uint8_t val;
#if defined (CONFIG_KP_AXP20)
	axp_read(charger->master, AXP_DATA_BUFFER4, &val);
#endif
#if defined (CONFIG_KP_AXP19)
	axp_read(charger->master, AXP_DATA_BUFFER0, &val);
#endif
	if((val & 0x80) >> 7)
		return (int) (0 - (val & 0x7F));
	else
		return (int) (val & 0x7F);
}

static void axp_set_batcap(struct axp_charger *charger,int cou)
{
#if defined (CONFIG_KP_AXP20)
	uint8_t temp[3];
	cou |= 0x8000;
	temp[0] = ((cou & 0xff00) >> 8);
	temp[1] = AXP_DATA_BUFFER3;
	temp[2] = (cou & 0x00ff);
	axp_writes(charger->master,AXP_DATA_BUFFER2,3,temp);
#endif
#if defined (CONFIG_KP_AXP19)
	uint8_t temp[3];
	axp_reads(charger->master, AXP_DATA_BUFFER1,2,temp);
	cou = cou/4;
	temp[0] = ((cou >> 4) | 0x80);
	temp[2] = temp[1] & 0x0f;
	temp[2] |= ((cou & 0x0f) << 4);
	temp[1] = AXP_DATA_BUFFER2;
	axp_writes(charger->master,AXP_DATA_BUFFER1,3,temp);
#endif
}

static int axp_get_batcap(struct axp_charger *charger)
{
	uint8_t temp[2];
	int rValue;
#if defined (CONFIG_KP_AXP20)
	axp_reads(charger->master, AXP_DATA_BUFFER2,2,temp);
	rValue = ((temp[0] << 8) + temp[1]);
	if(rValue & 0x8000){
		return ((rValue & 0x7fff));
#endif
#if defined (CONFIG_KP_AXP19)
	axp_reads(charger->master, AXP_DATA_BUFFER1,2,temp);
	rValue = ((temp[0] << 4) + (temp[1]>>4));
	if(rValue & 0x800){
		return ((rValue & 0x7ff)*4);
#endif
	} else {
		axp_set_batcap(charger,BATCAP);
		return BATCAP;
	}
}
#if defined (CONFIG_KP_AXP19)
static int startcorcap = 0;
#endif
static void axp_charging_monitor(struct work_struct *work)
{
	struct axp_charger *charger;
	int pre_rest_cap,rt_rest_cap,rest_cap;
	int rdc = 0,Cur_CoulombCounter = 0,base_cap = 0,bat_cap = 0;
	uint8_t val,v[2];
	bool flag_coulomb,flag_batcap,flag_rdc;
#if defined (CONFIG_AXP_DEBUG)
	uint8_t data_mm[AXP_DATA_NUM];
	int mm;
#endif

	charger = container_of(work, struct axp_charger, work.work);
	/* 更新电池信息 */
	pre_rest_cap = charger->rest_cap;
	axp_charger_update_state(charger);
	axp_charger_update(charger);
#if defined (CONFIG_KP_AXP20)
	axp_read(charger->master,AXP_DATA_BUFFER0,&val);
	flag_coulomb = ((val >> 6) & 0x01);
	flag_batcap = ((val >> 5) & 0x1);
	flag_rdc = ((val >> 3) & 0x1);

	/* 更新电池开路电压 */
	axp_reads(charger->master,AXP_OCV_BUFFER0,2,v);
	charger->ocv = ((v[0] << 4) + (v[1] & 0x0f)) * 11 /10 ;
#endif
#if defined (CONFIG_KP_AXP19)
	axp_read(charger->master,AXP_DATA_BUFFER2,&val);
	flag_coulomb = ((val >> 2) & 0x01);
	flag_batcap = ((val >> 1) & 0x1);
	flag_rdc = ((val >> 0) & 0x1);
	
	/* 更新电池开路电压及其对应百分比 */
	val = axp_ocv_restcap(charger);
#endif
	/* 计算电池内阻rdc */
	axp_cal_rdc(charger);
	
	if(state_count){
		rt_rest_cap = charger->ocv_rest_cap;
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
		if(ABS(rt_rest_cap - charger->ocv_rest_cap) > 5){
			printk("correct rdc\n");
#if defined (CONFIG_KP_AXP20)
			axp_clr_bits(charger->master,AXP_DATA_BUFFER0,0x08);
#endif
#if defined (CONFIG_KP_AXP19)
			axp_clr_bits(charger->master,AXP_DATA_BUFFER2,0x01);
#endif
		}
	}
	
	charger->ocv_rest_cap = rt_rest_cap;
	
	Cur_CoulombCounter = axp_get_coulomb(charger);
	base_cap = axp_get_basecap(charger);
	bat_cap = axp_get_batcap(charger);

	if(flag_coulomb){
		charger->rest_cap = 100 * (Cur_CoulombCounter) / bat_cap + base_cap;
		
		/* 校正库仑计 在实时ocv剩余容量小于BATCAPCORRATE%，计算过rdc 且未校正过库仑计时 ，注意这个时候有可能ocv和coulomb不同步而出现跳变。*/
		if((rt_rest_cap < (BATCAPCORRATE+1)) && (flag_rdc) && (!flag_batcap)){			
			flag_batcap = 1;					
			axp_set_basecap(charger,rt_rest_cap);
#if defined (CONFIG_KP_AXP20)
			axp_write(charger->master, AXP_DATA_BUFFER5, rt_rest_cap);
			axp_clr_coulomb(charger);
			axp_set_bits(charger->master,AXP_DATA_BUFFER0,0x20);
#endif
#if defined (CONFIG_KP_AXP19)
			startcorcap = rt_rest_cap;
			axp_clr_coulomb(charger);
			axp_set_bits(charger->master,AXP_DATA_BUFFER2,0x02);
#endif
			printk("\n ============Capacity Calibration Start============ \n");
			printk("\n ============   Rest  Capacity  =  %d  ============ \n",rt_rest_cap);
		}
		/* 超过量程校正 */
		if(charger->rest_cap > 100){
			if(axp_debug)
				DBG_PSY_MSG("%s->%d: charger->rest_vol > 100\n",__FUNCTION__,__LINE__);
			charger->rest_cap = 100;
		}
		else if (charger->rest_cap < 0){
			if(axp_debug)
				DBG_PSY_MSG("%s->%d: charger->rest_vol < 0\n",__FUNCTION__,__LINE__);
			charger->rest_cap = 0;
		}
		/*出错处理，如果初始base_cap估计偏小，导致充满后用库仑计计算的电量低于100%*/
		if((rt_rest_cap > 98) && (charger->rest_cap < 99) && (charger->bat_current_direction == 1)){
			if(axp_debug)
				DBG_PSY_MSG("%s->%d:(rt_rest_cap > 98) && (charger->rest_cap < 99)\n",__FUNCTION__,__LINE__);
			if(cap_count1 >= TIMER5) {
				charger->rest_cap ++;
				base_cap ++;
				axp_set_basecap(charger,base_cap);
				cap_count1 = 0;
			} else {
				cap_count1++;
			}
		} else {
			cap_count1 = 0;
		}
		/*出错处理，如果初始base_cap估计偏小，导致充满后用库仑计计算的电量低于100%*/
		if((charger->ocv >= 4100) && (charger->rest_cap < 100) && (charger->bat_current_direction == 0) && (charger->ext_valid) && charger->charge_on){
			if(axp_debug)
				DBG_PSY_MSG("%s->%d:(charger->ocv >= 4100) && (charger->rest_vol < 100) && (charger->bat_current_direction == 0) && (charger->ext_valid)\n",__FUNCTION__,__LINE__);
			if(cap_count2 >= TIMER5) {
				charger->rest_cap++;
				base_cap++;
				axp_set_basecap(charger,base_cap);
				cap_count2 = 0;
			} else {
				cap_count2++;
			}
		} else {
			cap_count2 = 0;
		}

		/*出错处理，如果初始base_cap估计偏小，导致放电后用库仑计计算的电量低于0%*/
		if((rt_rest_cap > (BATCAPCORRATE-1)) && (charger->rest_cap < BATCAPCORRATE) && charger->bat_current_direction == 0){
			if(axp_debug)
				DBG_PSY_MSG("%s->%d(rt_rest_cap > %d) && (charger->rest_cap < %d) && charger->bat_current_direction == 0\n",__FUNCTION__,__LINE__,BATCAPCORRATE-1,BATCAPCORRATE);
			if(pre_rest_cap > charger->rest_cap){
				base_cap = pre_rest_cap - 100 * (Cur_CoulombCounter) / bat_cap;
				axp_set_basecap(charger,base_cap);
			}
			charger->rest_cap =  pre_rest_cap;
		}
			
		/*出错处理，如果初始base_cap估计偏大，导致放电后用库仑计计算的电量过高*/
		if((rt_rest_cap < BATCAPCORRATE) && (charger->rest_cap > (BATCAPCORRATE-1)) && (charger->bat_current_direction == 0)){
			if(axp_debug)
				DBG_PSY_MSG("%s->%d(rt_rest_cap < %d) && (charger->rest_cap > %d) && (charger->bat_current_direction == 0)\n",__FUNCTION__,__LINE__,BATCAPCORRATE,BATCAPCORRATE-1);
			if(cap_count3 >= TIMER5){
				charger->rest_cap --;
				base_cap --;
				axp_set_basecap(charger,base_cap);
				cap_count3 = 0;
			} else {
				cap_count3++;
			}
		} else {
			cap_count3 = 0;
		}
		
		/*出错处理，如果初始base_cap估计偏小，导致放电后用库仑计计算的电量低于0%*/
		if((rt_rest_cap > (SHUTDOWNRATE+1)) && (charger->rest_cap < (SHUTDOWNRATE+2)) && charger->bat_current_direction == 0){
			if(axp_debug)
				DBG_PSY_MSG("%s->%d(rt_rest_cap > %d) && (charger->rest_cap < %d) && charger->bat_current_direction == 0\n",__FUNCTION__,__LINE__,(SHUTDOWNRATE+1),(SHUTDOWNRATE+2));
			if(pre_rest_cap > charger->rest_cap){
				base_cap = pre_rest_cap - 100 * (Cur_CoulombCounter) / bat_cap;
				axp_set_basecap(charger,base_cap);
			}
			charger->rest_cap =  pre_rest_cap;
		}
			
		/*出错处理，如果初始base_cap估计偏大，导致放电后用库仑计计算的电量过高*/
		if((rt_rest_cap < (SHUTDOWNRATE+2)) && (charger->rest_cap > (SHUTDOWNRATE+1)) && (charger->bat_current_direction == 0)){
			if(axp_debug)
				DBG_PSY_MSG("%s->%d(rt_rest_cap < %d) && (charger->rest_cap > %d) && (charger->bat_current_direction == 0)\n",__FUNCTION__,__LINE__,(SHUTDOWNRATE+2),(SHUTDOWNRATE+1));
			if(cap_count4 >= TIMER5){
				charger->rest_cap --;
				base_cap --;
				axp_set_basecap(charger,base_cap);
				cap_count4 = 0;
			} else {
				cap_count4++;
			}
		} else {
			cap_count4 = 0;
		}

		/*出错处理，如果还在充电，即使返回100%，也置为99%*/
		if((charger->bat_current_direction == 1) && (charger->rest_cap >= 100)){
			if(axp_debug)
				DBG_PSY_MSG("%s->%d:[Before fix] charger->rest_vol = %d\n",__FUNCTION__,__LINE__, charger->rest_cap);
			charger->rest_cap = 99;
			base_cap = 99 - 100 * (Cur_CoulombCounter) / bat_cap;
			axp_set_basecap(charger,base_cap);
		}

		if(flag_batcap && (charger->bat_current_direction  == 0) && (charger->rest_cap == 100) && (charger->ext_valid)){
#if defined (CONFIG_KP_AXP20)
			axp_read(charger->master, AXP_DATA_BUFFER5, &val);
			bat_cap = ABS(Cur_CoulombCounter) / ( 100 - val) * 100;
			axp_set_batcap(charger,bat_cap);
			flag_batcap = 0; 
			axp_clr_bits(charger->master,AXP_DATA_BUFFER0,0x20);
#endif
#if defined (CONFIG_KP_AXP19)
			bat_cap = ABS(Cur_CoulombCounter) / ( 100 - startcorcap) * 100;
			axp_set_batcap(charger,bat_cap);
			flag_batcap = 0; 
			axp_clr_bits(charger->master,AXP_DATA_BUFFER2,0x02);  
#endif
			printk("\n ===========Capacity calibration finished============ \n");
			printk("\n ============   Battery capacity =  %d   ============ \n",bat_cap);
			base_cap = 100 - 100 * (Cur_CoulombCounter) / bat_cap;
			axp_set_basecap(charger,base_cap);
		}
	} else {
		charger->rest_cap = rt_rest_cap;
		if(axp_debug)
			DBG_PSY_MSG(" first init charger->rest_cap = %d\n",charger->rest_cap);   
		/* full */
		if(charger->vbat >= 4100 && !charger->is_on && charger->ext_valid && charger->charge_on)
			charger->rest_cap = 100;
		/* charging */
		if(charger->is_on && charger->rest_cap >= 100)
			charger->rest_cap = 99;

		axp_set_basecap(charger,charger->rest_cap);
		axp_clr_coulomb(charger);
#if defined (CONFIG_KP_AXP20)
		axp_set_bits(charger->master, AXP_DATA_BUFFER0, 0x40);
#endif
#if defined (CONFIG_KP_AXP19)
		axp_set_bits(charger->master, AXP_DATA_BUFFER2, 0x04);
#endif
	}

#if defined (CONFIG_KP_AXP20)	
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
	
	if(charger->rest_cap - pre_rest_cap){
		printk("battery vol change: %d->%d \n", pre_rest_cap, charger->rest_cap);
		pre_rest_cap = charger->rest_cap;
#if defined (CONFIG_KP_AXP20)
		axp_write(charger->master,AXP_DATA_BUFFER1,charger->rest_cap | 0x80);
#endif
		power_supply_changed(&charger->batt);
	}
	
if(0&&axp_debug){
	printk("Cur_CoulombCounter = %d\n", Cur_CoulombCounter);
	DBG_PSY_MSG("charger->ic_temp = %d\n",charger->ic_temp);
	printk("charger->vbat = %d\n",charger->vbat);
	printk("charger->ibat = %d\n",charger->ibat);
	printk("charger->disvbat = %d\n",charger->disvbat);
	printk("charger->disibat = %d\n",charger->disibat);
	DBG_PSY_MSG("charger->vusb = %d\n",charger->vusb);
	DBG_PSY_MSG("charger->iusb = %d\n",charger->iusb);
	printk("charger->vac = %d\n",charger->vac);
	printk("charger->iac = %d\n",charger->iac);
	printk("charger->ocv = %d\n",charger->ocv);
	printk("charger->chgcur = %d\n",charger->chgcur);
	DBG_PSY_MSG("charger->chgearcur = %d\n",charger->chgearcur);
	DBG_PSY_MSG("charger->chgsuscur = %d\n",charger->chgsuscur);
	DBG_PSY_MSG("charger->chgclscur = %d\n",charger->chgclscur);
	printk("rt_rest_cap = %d\n",rt_rest_cap);
	printk("bat_cap = %d\n",bat_cap);
	printk("base_cap = %d\n",base_cap);
	printk("charger->ocv_rest_cap = %d\n",charger->ocv_rest_cap);
	printk("charger->rest_cap = %d\n",charger->rest_cap);
	axp_reads(charger->master,AXP_RDC_BUFFER0,2,v);
#if defined (CONFIG_KP_AXP20)
	rdc = (((v[0] & 0x1F) << 8) | v[1]) * 10742 / 10000;
	DBG_PSY_MSG("rdc = %d\n",rdc);
#endif
#if defined (CONFIG_KP_AXP19)
	rdc = v[1]*2;
	DBG_PSY_MSG("rdc = %d,flag = 0x%x\n",rdc,v[0]);
#endif
	DBG_PSY_MSG("charger->is_on = %d\n",charger->is_on);
	DBG_PSY_MSG("charger->ext_valid = %d\n",charger->ext_valid);
	DBG_PSY_MSG("rdc_count = %d\n",rdc_count);
	DBG_PSY_MSG("state_count = %d\n",state_count);
	DBG_PSY_MSG("cap_count1 = %d,cap_count2 = %d,cap_count3 = %d,cap_count4 = %d\n",cap_count1,cap_count2,cap_count3,cap_count4);
#if defined (CONFIG_AXP_DEBUG)
	axp_reads(charger->master,AXP_DATA_BUFFER0,AXP_DATA_NUM,data_mm);
	for( mm = 0; mm < AXP_DATA_NUM; mm++){
		DBG_PSY_MSG("REG[0x%x] = 0x%x\n",mm+AXP_DATA_BUFFER0,data_mm[mm]);	
	}
#endif
}

	if( ( 1 == charger->ac_valid || 1==charger->usb_valid )){  // charge
                if (!wake_lock_active(&batt_wake_lock)) {
                        wake_lock(&batt_wake_lock);  //lock
                }
        } else {
                if (wake_lock_active(&batt_wake_lock)) {
                        wake_unlock(&batt_wake_lock);
                }
        }

	if (charger_in_logo == 1) {
		
		 if(charger->usb_valid == 0 && charger->ac_valid == 0 )
		 {
			printk("cdy charge monitor charger->usb_valid = %d, charger->ac_valid = %d \n",
				charger->usb_valid , charger->ac_valid);
				rk30_pm_power_off();
		 }
	}


	/* reschedule for the next time */
	schedule_delayed_work(&charger->work, charger->interval);
}

static void axp_battery_infoinit(struct axp_charger *charger)
{
	uint8_t val,val1,val2;
	int bat_cap,base_cap,Cur_CoulombCounter; 
#if defined (CONFIG_KP_AXP20)
	axp_read(charger->master, AXP_CAP,&val2);
	charger->ocv_rest_cap = ((val2 & 0x7F)==127)? 100:(val2 & 0x7F);
	
	axp_read(charger->master,AXP_DATA_BUFFER1,&val1);
	printk("last_rest_cap = %d, now_ocv_rest_cap = %d\n",(val1 & 0x7F),charger->ocv_rest_cap);
  	base_cap = axp_get_basecap(charger);
	bat_cap = axp_get_batcap(charger);
	axp_read(charger->master, AXP_DATA_BUFFER0,&val);
	if((val >> 6)& 0x01){
		Cur_CoulombCounter = (axp_get_coulomb(charger));
#endif
#if defined (CONFIG_KP_AXP19)
	val2 = axp_ocv_restcap(charger);
	charger->ocv_rest_cap = val2;
	
	axp_read(charger->master,AXP_DATA_BUFFER0,&val1);
	printk("base_cap = 0x%x, now_ocv_rest_cap = %d\n",(val1),charger->ocv_rest_cap);
  	base_cap = axp_get_basecap(charger);
	bat_cap = axp_get_batcap(charger);
	axp_read(charger->master,AXP_DATA_BUFFER2,&val);
	if((val >> 2)& 0x01){
		Cur_CoulombCounter = axp_get_coulomb(charger);
#endif
		printk("Cur_CoulombCounter = %d\n",Cur_CoulombCounter);
		charger->rest_cap = 100 * Cur_CoulombCounter / bat_cap + base_cap;
		if(charger->rest_cap > 100){
			charger->rest_cap = 100;
			if(Cur_CoulombCounter < bat_cap){
				base_cap = 100 - 100 * (Cur_CoulombCounter) / bat_cap;
				axp_set_basecap(charger,base_cap);
			}
			else{
				bat_cap = Cur_CoulombCounter * 100 /(100 - base_cap);
				axp_set_batcap(charger,bat_cap);
			}
		}
		else if (charger->rest_cap < 0){
			charger->rest_cap = 0;
			axp_set_basecap(charger,0);
			axp_clr_coulomb(charger);
		}
	}
	else{
		if(ABS((val1 & 0x7F)-(charger->ocv_rest_cap)) >= 10)
			charger->rest_cap = charger->ocv_rest_cap;
		else
			charger->rest_cap = (val1 & 0x7F);
	}
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
	
	/* 设置休眠充电电流 */
#if defined (CONFIG_AXP_CHGCURCHG)
	axp_set_chargecurrent(charger->chgsuscur);
#endif

	/* 设置阈值时间为127分钟 */
	axp_write(charger->master, AXP_TIMER_CTL, 0x80);
	axp_write(charger->master, AXP_TIMER_CTL, 0x7F);

	return 0;
}

static int axp_resume(struct platform_device *dev)
{
	struct axp_charger *charger = platform_get_drvdata(dev);

	int pre_rest_cap;
	int rt_rest_cap = 0;
	int Cur_CoulombCounter = 0,bat_cap = 0,base_cap = 0;
	uint8_t val,v[2];
	int rdc;
	bool flag_coulomb,flag_batcap,flag_rdc;
	/* 打开irq */
#if defined (CONFIG_KP_USEIRQ)
	//axp_register_notifier(charger->master, &charger->nb, AXP_NOTIFIER_ON );
#endif

	axp_charger_update_state(charger);
	axp_charger_update(charger);
	if(charger->bat_det == 1){
		pre_rest_cap = charger->rest_cap;
#if defined (CONFIG_KP_AXP20)
		axp_read(charger->master, AXP_CAP,&val);
		charger->rest_cap = (int) (val & 0x7F);
		rt_rest_cap = charger->rest_cap;
		if((charger->bat_det == 0) || (charger->rest_cap == 127)){
			charger->rest_cap = 100;
		}
		axp_read(charger->master,AXP_DATA_BUFFER0,&val);
		flag_coulomb = ((val >> 6) & 0x01);
		flag_batcap = ((val >> 5) & 0x1);
		flag_rdc = ((val >> 3) & 0x1);
#endif
#if defined (CONFIG_KP_AXP19)
		val = axp_ocv_restcap(charger);
		charger->rest_cap = (int) (val & 0x7F);
		rt_rest_cap = charger->rest_cap;
		axp_read(charger->master,AXP_DATA_BUFFER2,&val);
		flag_coulomb = ((val >> 2) & 0x01);
		flag_batcap = ((val >> 1) & 0x1);
		flag_rdc = ((val >> 0) & 0x1);
#endif
		base_cap = axp_get_basecap(charger);
		bat_cap = axp_get_batcap(charger);

		if(flag_coulomb){
			Cur_CoulombCounter = axp_get_coulomb(charger);
			if(axp_debug)
				DBG_PSY_MSG("Cur_CoulombCounter = %d\n", Cur_CoulombCounter);
			charger->rest_cap = 100 * (Cur_CoulombCounter) / bat_cap + base_cap;
			/*出错处理，如果初始Pre_rest_cap估计偏大，导致充电时用库仑计计算的电量大于100%*/
			if(charger->rest_cap > 100){
				if(axp_debug)
					DBG_PSY_MSG("%s->%d\n",__FUNCTION__,__LINE__);
				charger->rest_cap = 100;
			} else if(charger->rest_cap < 0 ){
				if(axp_debug)
					DBG_PSY_MSG("%s->%d\n",__FUNCTION__,__LINE__);
				charger->rest_cap = 0;
			}
			/*电池容量校正*/	
			if((rt_rest_cap < (BATCAPCORRATE+1)) && (flag_rdc) && (charger->bat_current_direction  == 1) && (!flag_batcap)){				
				flag_batcap = 1;					
				axp_set_basecap(charger,rt_rest_cap);
#if defined (CONFIG_KP_AXP20)
				axp_write(charger->master, AXP_DATA_BUFFER5, rt_rest_cap);
				axp_clr_coulomb(charger);
				axp_set_bits(charger->master,AXP_DATA_BUFFER0,0x20);
#endif
#if defined (CONFIG_KP_AXP19)
				startcorcap = rt_rest_cap;
				axp_clr_coulomb(charger);
				axp_set_bits(charger->master,AXP_DATA_BUFFER2,0x02);
#endif
				printk("\n ============Capacity Calibration Start============ \n");
				printk("\n ============   Rest  Capacity  =  %d  ============ \n",rt_rest_cap);
			}
			
			/*出错处理，如果初始base_cap估计偏小，导致充满后用库仑计计算的电量低于100%*/
			if((rt_rest_cap > 98) && (charger->rest_cap < 100) && (charger->bat_current_direction == 0x0) && (charger->ext_valid == 1)){
				if(axp_debug)
					DBG_PSY_MSG("%s->%d\n",__FUNCTION__,__LINE__);
				charger->rest_cap = 100;
				base_cap = 100 - 100 *(Cur_CoulombCounter) / bat_cap;
				axp_set_basecap(charger,base_cap);
			}
			/*出错处理，如果还在充电，即使返回100%，也置为99%*/
			if((charger->bat_current_direction == 1) && (charger->rest_cap >= 100)){
				if(axp_debug)
					DBG_PSY_MSG("%s->%d:charger->rest_cap = %d\n",__FUNCTION__,__LINE__,charger->rest_cap);
				charger->rest_cap = 99;
				base_cap = 99 - 100 * (Cur_CoulombCounter) / bat_cap;
				axp_set_basecap(charger,base_cap);
			}

			if(flag_batcap && (charger->bat_current_direction  == 0) && (charger->rest_cap == 100) && (charger->ext_valid)){
#if defined (CONFIG_KP_AXP20)
				axp_read(charger->master, AXP_DATA_BUFFER5, &val);
				bat_cap = ABS(Cur_CoulombCounter) / ( 100 - val) * 100;
				axp_set_batcap(charger,bat_cap);
				flag_batcap = 0; 
				axp_clr_bits(charger->master,AXP_DATA_BUFFER0,0x20);
#endif
#if defined (CONFIG_KP_AXP19)
				bat_cap = ABS(Cur_CoulombCounter) / ( 100 - startcorcap) * 100;
				axp_set_batcap(charger,bat_cap);
				flag_batcap = 0; 
				axp_clr_bits(charger->master,AXP_DATA_BUFFER2,0x02);
#endif   
				printk("\n ===========Capacity calibration finished============ \n");
				printk("\n ============   Battery capacity =  %d   ============ \n",bat_cap);
				base_cap = 100 - 100 * (Cur_CoulombCounter) / bat_cap;
				axp_set_basecap(charger,base_cap);
			}		
		} else {
			if(charger->bat_current_direction && (charger->rest_cap < pre_rest_cap))
				charger->rest_cap = pre_rest_cap;
			if(axp_debug)
				DBG_PSY_MSG("val = 0x%x,pre_rest_cap = %d,rest_cap = %d\n",val,pre_rest_cap,charger->rest_cap);
			/* full */
			if((charger->ocv) >= 4100 && !charger->bat_current_direction && charger->ext_valid)
				charger->rest_cap = 100;
			/* charging*/
			if(charger->bat_current_direction && charger->rest_cap == 100)
				charger->rest_cap = 99;

			if(axp_debug)
				DBG_PSY_MSG("\n ocv:charger->rest_cap = %d \n",charger->rest_cap);
			axp_set_basecap(charger,charger->rest_cap);
			axp_clr_coulomb(charger);
			axp_set_bits(charger->master, AXP_DATA_BUFFER0, 0x40);
    	}

		axp_read(charger->master, AXP_TIMER_CTL, &val);
		if((val >> 7) && (pre_rest_cap > charger->rest_cap) && !charger->bat_current_direction){
			if(pre_rest_cap > rt_rest_cap){
				charger->rest_cap = rt_rest_cap;
				base_cap = rt_rest_cap - 100* (Cur_CoulombCounter) / bat_cap;
				if(axp_debug)
					DBG_PSY_MSG("\n TIMER:correct rest vol!!!\n");
				axp_set_basecap(charger,base_cap);
			}
		}

		charger->ocv_rest_cap = rt_rest_cap;
		
			/* if battery volume changed, inform uevent */
		if(charger->rest_cap - pre_rest_cap){
			printk("battery vol change: %d->%d \n", pre_rest_cap, charger->rest_cap);
			pre_rest_cap = charger->rest_cap;
#if defined (CONFIG_KP_AXP20)
			axp_write(charger->master,AXP_DATA_BUFFER1,charger->rest_cap | 0x80);
#endif
			power_supply_changed(&charger->batt);
		}
		
	} else {
		charger->rest_cap = 100;
		charger->ocv = 0;
		power_supply_changed(&charger->batt);
	}
		
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
#if defined (CONFIG_KP_AXP20)
	axp_reads(charger->master,0xbc,2,v);
	charger->ocv = ((v[0] << 4) + (v[1] & 0x0f)) * 11 /10;
#endif
	DBG_PSY_MSG("charger->ocv = %d\n",charger->ocv);
	DBG_PSY_MSG("rt_rest_cap1 = %d\n",rt_rest_cap);
	/* 更新电池开路电压及其对应百分比 */
	val = axp_ocv_restcap(charger);
#if defined (CONFIG_KP_AXP20)
		axp_read(charger->master, AXP_CAP,&val);
#endif
	rt_rest_cap = (int) (val & 0x7F);
	DBG_PSY_MSG("rt_rest_cap2 = %d\n",rt_rest_cap);
	DBG_PSY_MSG("bat_cap = %d\n",bat_cap);
	DBG_PSY_MSG("base_cap = %d\n",base_cap);
	DBG_PSY_MSG("charger->ocv_rest_cap = %d\n",charger->ocv_rest_cap);
	DBG_PSY_MSG("charger->rest_cap = %d\n",charger->rest_cap);
	axp_reads(charger->master,AXP_RDC_BUFFER0,2,v);
#if defined (CONFIG_KP_AXP20)
	rdc = (((v[0] & 0x1F) << 8) | v[1]) * 10742 / 10000;
	DBG_PSY_MSG("rdc = %d\n",rdc);
	axp_read(charger->master,0x8,v);
	DBG_PSY_MSG("base restvol = %d\n",(int) v);
#endif
#if defined (CONFIG_KP_AXP19)
	rdc = v[1] * 2;
	DBG_PSY_MSG("rdc = %d,flag =0x%x\n",rdc,v[0]);
#endif
	DBG_PSY_MSG("charger->is_on = %d\n",charger->is_on);
	DBG_PSY_MSG("charger->ext_valid = %d\n",charger->ext_valid);
}
		
#if defined (CONFIG_KP_AXP20)
	/* 休眠低电关机接口 */
	axp_read(charger->master, AXP_DATA_BUFFERA,&val);
	printk("[AXP]suspend power off val = 0x%x\n",val);
	if(val){
		axp_close(charger);
		axp_set_bits(charger->master,AXP_INTSTS5,0x03);
	}
	axp_write(charger->master, AXP_DATA_BUFFERA,0x00);
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
