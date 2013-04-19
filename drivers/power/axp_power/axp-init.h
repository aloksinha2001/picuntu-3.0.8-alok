static void axp_clear_irq(struct axp_charger *charger)
{
#if defined (CONFIG_KP_AXP20)
	uint8_t irq_w[9];
#endif 
#if defined (CONFIG_KP_AXP19)
	uint8_t irq_w[7];
#endif 
	irq_w[0] = 0xff;
	irq_w[1] = AXP_INTSTS2;
	irq_w[2] = 0xff;
	irq_w[3] = AXP_INTSTS3;
	irq_w[4] = 0xff;
	irq_w[5] = AXP_INTSTS4;
	irq_w[6] = 0xff;
#if defined (CONFIG_KP_AXP20)	
	irq_w[7] = AXP_INTSTS5;
	irq_w[8] = 0xff;
    axp_writes(charger->master, AXP_INTSTS1, 9, irq_w);
#endif
#if defined (CONFIG_KP_AXP19)	
	axp_writes(charger->master, AXP_INTSTS1, 7, irq_w);
#endif
}

static int axp_get_freq(void)
{
	int  ret = 25;
	uint8_t  temp;
	axp_read(axpcharger->master, AXP_ADC_CONTROL3,&temp);
	temp &= 0xc0;
	switch(temp >> 6){
		case 0:	ret = 25; break;
		case 1:	ret = 50; break;
		case 2:	ret = 100;break;
		case 3:	ret = 200;break;
		default:break;
	}
	return ret;
}

static int axp_set_freq(int frq)
{
	int ret = 0;
#if defined (CONFIG_AXP_FRQSET)
	uint8_t val;	
	if(frq >= 25 && frq <= 200){
		if(frq < 50)
			val = 0x0;
		else if (frq < 100)
			val = 0x40;
		else if (frq < 200)
			val = 0x80;
		else 
			val = 0xc0;		
		ret = axp_update(axpcharger->master, AXP_ADC_CONTROL3, val, 0xC0);
	}
#endif
	return ret;
}

static int axp_set_chargecurrent(int cur)
{
	int ret = 0;
#if defined (CONFIG_AXP_CHGCURSET)
	uint8_t tmp;
	if(cur == 0)
		ret = axp_clr_bits(axpcharger->master,AXP_CHARGE_CONTROL1,0x80);
	else
		ret = axp_set_bits(axpcharger->master,AXP_CHARGE_CONTROL1,0x80);
	if(ret)
		return ret;
	printk("charging current = %d mA\n", cur);
#if defined (CONFIG_KP_AXP20)
	if(cur >= 300 && cur <= 1800){
		tmp = (cur -201)/100;
		ret = axp_update(axpcharger->master, AXP_CHARGE_CONTROL1, tmp, 0x0F);
	}
#endif

#if defined (CONFIG_KP_AXP19)
	if(cur >= 100 && cur <= 1320){
		if(cur < 190)
			tmp = 0x00;
		else if(cur < 280)
			tmp = 0x01;
		else if(cur < 360)
			tmp = 0x02;
		else if(cur < 450)
			tmp = 0x03;
		else if(cur < 550)
			tmp = 0x04;
		else if(cur < 630)
			tmp = 0x05;
		else if(cur < 700)
			tmp = 0x06;
		else if(cur < 780)
			tmp = 0x07;
		else if(cur < 880)
			tmp = 0x08;
		else if(cur < 960)
			tmp = 0x09;
		else if(cur < 1000)
			tmp = 0x0A;
		else if(cur < 1080)
			tmp = 0x0B;
		else if(cur < 1160)
			tmp = 0x0C;
		else if(cur < 1240)
			tmp = 0x0D;
		else if(cur < 1320)
			tmp = 0x0E;
		else 
			tmp = 0x0F;
		ret = axp_update(axpcharger->master, AXP_CHARGE_CONTROL1, tmp, 0x0F);
	}
#endif

#endif
	return ret;
}

static int axp_set_chargevoltage(int vol)
{
	int ret = 0;
#if defined (CONFIG_AXP_CHGVOLSET)
	uint8_t val = 0;
	if(vol >=4100 && vol <=4360){
		if(vol < 4150)
			val &= ~(3 << 5);
		else if (vol<4200){
			val &= ~(3 << 5);
			val |= 1 << 5;
		}
		else if (vol<4360){
			val &= ~(3 << 5);
			val |= 1 << 6;
		}
		else
			val |= 3 << 5;
    	ret = axp_update(axpcharger->master, AXP_CHARGE_CONTROL1,val,0x60);
    }
#endif
	return ret;
}

static int axp_set_chargeendrate(int end)
{
	int ret = 0;
#if defined (CONFIG_AXP_CHGRATESET)
	switch (end){
		case 10: ret = axp_clr_bits(axpcharger->master, AXP_CHARGE_CONTROL1,0x10);break;
		case 15: ret = axp_set_bits(axpcharger->master, AXP_CHARGE_CONTROL1,0x10);break;
		default: break;
	}
#endif
	return ret;
}

static int axp_set_prechargetime(int pre)
{
	int ret = 0;
#if defined (CONFIG_AXP_PRECHGSET)
	uint8_t val = 0;
#if defined (CONFIG_KP_AXP20)
	if(pre >= 40 && pre <= 70){
		if (pre < 50)
			val = 0x00;
		else if(pre < 60)
			val = 0x40;
		else if(pre < 70)
			val = 0x80;
		else
			val = 0xc0;
		ret = axp_update(axpcharger->master, AXP_CHARGE_CONTROL2,val,0xC0);
	}
#endif

#if defined (CONFIG_KP_AXP19)
	if(pre >= 30 && pre <= 60){
		if (pre < 40)
			val = 0x00;
		else if(pre < 50)
			val = 0x40;
		else if(pre < 60)
			val = 0x80;
		else
			val = 0xc0;
		ret = axp_update(axpcharger->master, AXP_CHARGE_CONTROL2,val,0xC0);
	}
#endif
#endif
	return ret;
}

static int axp_set_cstchargetime(int cst)
{
	int ret = 0;
#if defined (CONFIG_AXP_CSTCHGSET)
	uint8_t val = 0;
#if defined (CONFIG_KP_AXP20)
	if(cst >= 360 && cst <= 720){
		if (cst < 480)
			val = 0x00;
		else if(cst < 600)
			val = 0x01;
		else if(cst < 720)
			val = 0x02;
		else
			val = 0x03;
		ret = axp_update(axpcharger->master, AXP_CHARGE_CONTROL2,val,0x03);
	}
#endif
	
#if defined (CONFIG_KP_AXP19)
	if(cst >= 420 && cst <= 600){
		if (cst < 480)
			val = 0x00;
		else if(cst < 540)
			val = 0x01;
		else if(cst < 600)
			val = 0x02;
		else
			val = 0x03;
		ret = axp_update(axpcharger->master, AXP_CHARGE_CONTROL2,val,0x03);
	}
#endif
#endif
	return ret;
}

static int axp_set_charge(struct axp_charger *charger)
{
	int ret = 0;
	/* 目标充电电压 */
	ret = axp_set_chargevoltage(charger->chgvol);
	if(ret){
		printk("[AXP]error in set charge voltage\n");
		return ret;
	}
	/* 充电电流 */
	ret = axp_set_chargecurrent(charger->chgcur);
	if(ret){
		printk("[AXP]error in set charge current\n");
		return ret;
	}
	/* 结束充电 */
	ret = axp_set_chargeendrate(charger->chgend);
	if(ret){
		printk("[AXP]error in set charge end current rate\n");
		return ret;
	}
	/* 预充电时间设置 */
	ret = axp_set_prechargetime(charger->chgpretime);
	if(ret){
		printk("[AXP]error in set precharge time\n");
		return ret;
	}	
	/* 恒流充电设置 */
	ret = axp_set_cstchargetime(charger->chgcsttime);
	if(ret){
		printk("[AXP]error in set constant charge time\n");
	}

	return ret;
}

static int axp_battery_adc_set(struct axp_charger *charger)
{
	int ret ;
	uint8_t val;
	/* 使能adc */
	val= AXP_ADC_BATVOL_ENABLE | AXP_ADC_BATCUR_ENABLE
		| AXP_ADC_DCINCUR_ENABLE | AXP_ADC_DCINVOL_ENABLE
		| AXP_ADC_USBVOL_ENABLE | AXP_ADC_USBCUR_ENABLE;

	ret = axp_update(charger->master, AXP_ADC_CONTROL1, val , val);
	if (ret){
		printk("[AXP]Unable to enable adc\n");
		return ret;
	}
	/* 设置adc采样率 */
	ret = axp_set_freq(charger->adc_freq);
	if(ret){
		printk("[AXP]Unable to set adc frequency\n");
	}
	return ret;
}

static int axp_set_pekinfo(struct platform_device *pdev)
{
	int ret = 0;
#if defined (CONFIG_AXP_PEKINFO)
	powerkeydev = input_allocate_device();
	if (!powerkeydev) {
		kfree(powerkeydev);
		return -ENODEV;
	}
	powerkeydev->name = pdev->name;
	powerkeydev->phys = "m1kbd/input2";
	powerkeydev->id.bustype = BUS_HOST;
	powerkeydev->id.vendor = 0x0001;
	powerkeydev->id.product = 0x0001;
	powerkeydev->id.version = 0x0100;
	powerkeydev->open = NULL;
	powerkeydev->close = NULL;
	powerkeydev->dev.parent = &pdev->dev;

	set_bit(EV_KEY, powerkeydev->evbit);
	set_bit(EV_REL, powerkeydev->evbit);
  
	set_bit(KEY_POWER, powerkeydev->keybit);

	ret = input_register_device(powerkeydev);
	if(ret) {
		printk("[AXP]Unable to Register the power key\n");
	}
#endif
	return ret;
}

static int axp_set_pekopen(int pek)
{
	int ret =0;
#if defined (CONFIG_AXP_PEKOPEN)
	uint8_t val;
#if defined (CONFIG_KP_AXP20)
	if(pek >=128 && pek <=3000){	
		if (pek < 1000)
			val = 0x0;
		else if(pek < 2000)
			val = 0x80;
		else if(pek < 3000)
			val = 0xc0;
		else 
			val = 0x40;
		ret = axp_update(axpcharger->master,AXP_PEK_SET,val,0xc0);
		if(ret)
			printk("[AXP]Unabel to set pek open time\n");
	}
#endif

#if defined (CONFIG_KP_AXP19)
	if(pek >=128 && pek <=2000){	
		if (pek < 500)
			val = 0x0;
		else if(pek < 1000)
			val = 0x40;
		else if(pek < 2000)
			val = 0x80;
		else 
			val = 0xC0;
		ret = axp_update(axpcharger->master,AXP_PEK_SET,val,0xc0);
		if(ret)
			printk("[AXP]Unabel to set pek open time\n");
	}
#endif

#endif
	return ret;
}

static int axp_set_peklong(int pek)
{
	int ret =0;
#if defined (CONFIG_AXP_PEKLONG)
	uint8_t val;
	if(pek >=1000 && pek <=2500){
		if (pek < 1500)
			val = 0x0;
		else if(pek < 2000)
			val = 0x10;
		else if(pek < 2500)
			val = 0x20;
		else 
			val = 0x30;
		ret = axp_update(axpcharger->master,AXP_PEK_SET,val,0x30);
		if(ret){
			printk("[AXP]Unabel to set pek long time\n");
		}
	}
#endif
	return ret;
}

static int axp_set_pekoffen(int pek)
{
	int ret =0;
#if defined (CONFIG_AXP_PEKOFFEN)
	switch(pek){
		case 0: ret = axp_clr_bits(axpcharger->master, AXP_PEK_SET,0x08);break;
		case 1: ret = axp_set_bits(axpcharger->master, AXP_PEK_SET,0x08);break;
		default: ret = -1;break;
	}
	if(ret){
		printk("[AXP]Unabel to set pek long time\n");
	}
#endif
	return ret;
}

static int axp_set_pekdelay(int pek)
{
	int ret =0;
#if defined (CONFIG_AXP_PEKDELAY)
	switch(pek){
		case 8: ret = axp_clr_bits(axpcharger->master, AXP_PEK_SET,0x04);break;
		case 64: ret = axp_set_bits(axpcharger->master, AXP_PEK_SET,0x04);break;
		default: ret = -1;break;
	}
	if(ret){
		printk("[AXP]Unabel to set pek powerok delay time\n");
	}
#endif
	return ret;
}

static int axp_set_pekoff(int pek)
{
	int ret = 0;
#if defined (CONFIG_AXP_PEKOFF)
	uint8_t val;
	if(pek >=4000 && pek <=10000){	
		if (PEKOFF < 6000)
			val = 0x0;
		else if(pek < 8000)
			val = 0x01;
		else if(pek < 10000)
			val = 0x02;
		else 
			val = 0x03;
		ret = axp_update(axpcharger->master,AXP_PEK_SET,val,0x03);
		if(ret){
			printk("[AXP]Unabel to set pek power off time\n");
		}
	}
#endif
	return ret;
}

static int axp_set_pek(void)
{
	int ret = 0;
	/* 开机时间 */
	ret = axp_set_pekopen(PEKOPEN);
	if(ret){
		printk("[AXP]Unable to set pek startup\n");
		return ret;
	}
	/* 长按时间 */
	ret = axp_set_peklong(PEKLONG);
	if(ret){
		printk("[AXP]Unable to set longtime pek\n");
		return ret;
	}
	/* 长按关机使能 */
	ret = axp_set_pekoffen(PEKOFFEN);
	if(ret){
		printk("[AXP]Unable to set pekoff\n");
		return ret;
	}
	/* power ok 信号延迟*/
	ret = axp_set_pekdelay(PEKDELAY);
	if(ret){
		printk("[AXP]Unable to set power ok delay\n");
		return ret;
	}
	/* 长按pmu关机时间 */
	ret = axp_set_pekoff(PEKOFF);
	if(ret){
		printk("[AXP]Unable to set power ok delay\n");
	}
	return ret;
}

static int axp_set_noe(int noe)
{
	int ret =0;
#if defined (CONFIG_AXP_NOESET)
	uint8_t val;
	if(noe >=128 && noe <=3000){	
		if (noe < 1000)
			val = 0x0;
		else if(noe < 2000)
			val = 0x01;
		else if(noe < 3000)
			val = 0x02;
		else 
			val = 0x03;
		ret = axp_update(axpcharger->master,AXP_OFF_CTL,val,0x03);
		if(ret){
			printk("[AXP]Unabel to set n_oe power off time\n");
		}
	}
#endif
	return ret;
}

static int axp_set_otpoffen(int otp)
{
	int ret = 0;
#if defined (CONFIG_AXP_OTPOFFEN)
	switch(otp){
		case 0: ret = axp_clr_bits(axpcharger->master, AXP_HOTOVER_CTL,0x04);break;
		case 1: ret = axp_set_bits(axpcharger->master, AXP_HOTOVER_CTL,0x04);break;
		default: ret = -1;break;
	}
	if(ret){
		printk("[AXP]Unabel to enable over temperture power off function\n");
	}
#endif
	return ret;
}

static int axp_set_opclinit(void)
{
	int ret = 0;
	/* 设置pek按键属性 */
	ret = axp_set_pek();
	if(ret){
		printk("[AXP]Unable to set pek property\n");
		return ret;
	}
	/* 设置N_OE开关属性 */
	ret = axp_set_noe(NOEOFF);
	if(ret){
		printk("[AXP]Unable to set noe\n");
		return ret;
	}
	/* 设置过温关机使能*/
	ret = axp_set_otpoffen(OTPOFFEN);
	if(ret){
		printk("[AXP]Unable to set over temperture power off\n");
	}
	return ret;
}

static int axp_set_ocv(void)
{
	int ret = 0;
#if defined (CONFIG_KP_AXP20)
	uint8_t ocv_cap[31];
	ocv_cap[0]  = OCVREG0;
	ocv_cap[1]  = 0xC1;
	ocv_cap[2]  = OCVREG1;
	ocv_cap[3]  = 0xC2;
	ocv_cap[4]  = OCVREG2;
	ocv_cap[5]  = 0xC3;
	ocv_cap[6]  = OCVREG3;
	ocv_cap[7]  = 0xC4;
	ocv_cap[8]  = OCVREG4;
	ocv_cap[9]  = 0xC5;
	ocv_cap[10] = OCVREG5;
	ocv_cap[11] = 0xC6;
	ocv_cap[12] = OCVREG6;
	ocv_cap[13] = 0xC7;
	ocv_cap[14] = OCVREG7;
	ocv_cap[15] = 0xC8;
	ocv_cap[16] = OCVREG8;
	ocv_cap[17] = 0xC9;
	ocv_cap[18] = OCVREG9;
	ocv_cap[19] = 0xCA;
	ocv_cap[20] = OCVREGA;
	ocv_cap[21] = 0xCB;
	ocv_cap[22] = OCVREGB;
	ocv_cap[23] = 0xCC;
	ocv_cap[24] = OCVREGC;
	ocv_cap[25] = 0xCD;
	ocv_cap[26] = OCVREGD;
	ocv_cap[27] = 0xCE;
	ocv_cap[28] = OCVREGE;
	ocv_cap[29] = 0xCF;
	ocv_cap[30] = OCVREGF;
	ret = axp_writes(axpcharger->master, 0xC0,31,ocv_cap);
	if(ret){
		printk("[AXP]Unable to set \n");
	}
#endif
	return ret;
}

static int axp_set_warning(int warn1,int warn2)
{
	int ret = 0;
#if defined (CONFIG_AXP_WARNVOL)
	uint8_t val;
	if(warn1 >= 2867200 && warn1 <= 4200000)
		val = (warn1 - 2867200) / 5600;	
	else
		val = 0x68;
	/* 3.5552V--%5 close*/
	ret = axp_write(axpcharger->master, AXP_APS_WARNING1,val);
	if(ret){
		printk("[AXP]Unable to set warning voltage 1\n");
		return ret;
	}

	if(warn2 >= 2867200 && warn2 <= 4200000)
		val = (warn2 - 2867200) / 5600;	
	else
		val = 0x5F;
	ret = axp_write(axpcharger->master, AXP_APS_WARNING2,val);
	if(ret){
		printk("[AXP]Unable to set warning voltage 1\n");
	}
#endif
	return ret;
}

static int axp_set_rdcclose(void)
{
	int ret;
	uint8_t val[2];
	int rdc;
#if defined (CONFIG_KP_AXP20)
	axp_set_bits(axpcharger->master,AXP_CAP,0x80);
	axp_clr_bits(axpcharger->master,AXP_RDC_BUFFER0,0x80);
	/* 设置电池内阻的默认值 */
	axp_reads(axpcharger->master,AXP_RDC_BUFFER0,2,val);
	rdc = (((val[0] & 0x1F) << 8) | val[1]) * 10742 / 10000;
	axp_read(axpcharger->master,AXP_DATA_BUFFER0,&val[0]);
	if((((val[0] >> 7) & 0x1) == 0)||(rdc > BATRDC*3 )||(rdc < BATRDC/4)){
		rdc = (BATRDC * 10000 + 5371) / 10742;
		axp_write(axpcharger->master,AXP_RDC_BUFFER1,rdc & 0x00FF);
		axp_update(axpcharger->master, AXP_RDC_BUFFER0, (rdc >> 8), 0x1F);
	}
	ret = axp_clr_bits(axpcharger->master,AXP_CAP,0x80);
#endif

#if defined (CONFIG_KP_AXP19)
	ret = axp_reads(axpcharger->master,AXP_RDC_BUFFER0,2,val);
	rdc = val[1] * 2;
	if(!(val[0] & 0x08)|| rdc > BATRDC*3 || rdc < BATRDC/4){
		val[1] = (BATRDC) / 2;
		axp_set_bits(axpcharger->master, AXP_RDC_BUFFER0, 0x08);
		axp_write(axpcharger->master,AXP_RDC_BUFFER1,val[1]);
	}
#endif

	return ret;
}

static int axp_usb_vollim(int vol)
{
	int ret = 0;
#if defined (CONFIG_AXP_USBVOLLIM)
	uint8_t val;
	if(vol){
		axp_set_bits(axpcharger->master,AXP_CHARGE_VBUS, 0x40);
		if(vol >= 4000 && vol <=4700){
			val = ((vol - 4000)/100) << 3;
			ret = axp_update(axpcharger->master, AXP_CHARGE_VBUS,val,(0x7 << 3));
			if(ret)
				printk("[AXP]Unable to set usb voltage set %d mV\n",val);
		}
	}
	else{
		ret = axp_clr_bits(axpcharger->master, AXP_CHARGE_VBUS, 0x40);	
		if(ret)
			printk("[AXP]Unable to set usb voltage set\n");
	}	
#endif
	return ret;	
}

static int axp_usb_curlim(int cur)
{
	int ret = 0;
#if defined (CONFIG_AXP_USBCURLIM)
#if defined (CONFIG_KP_AXP20)
	if(cur){
		axp_clr_bits(axpcharger->master, AXP_CHARGE_VBUS, 0x01);
		if(cur == 900){
			ret = axp_clr_bits(axpcharger->master, AXP_CHARGE_VBUS, 0x03);
			if(ret)
				printk("[AXP]Uable to set usb current set 900mA\n");
    	}
		else if (cur == 500){
			ret = axp_update(axpcharger->master, AXP_CHARGE_VBUS,0x01, 0x03);
			if(ret)
				printk("[AXP]Uable to set usb current set 500mA\n");
		}
		else if (cur == 100){
			ret = axp_update(axpcharger->master, AXP_CHARGE_VBUS,0x10, 0x03);
			if(ret)
				printk("[AXP]Uable to set usb current set 100mA\n");
		}
	}
	else
		ret = axp_set_bits(axpcharger->master, AXP_CHARGE_VBUS, 0x03);
		if(ret)
			printk("[AXP]Uable to set usb current set 0mA\n");
#endif

#if defined (CONFIG_KP_AXP19)
	if(cur){
		axp_set_bits(axpcharger->master, AXP_CHARGE_VBUS, 0x02);
		if (cur == 500){
			ret = axp_clr_bits(axpcharger->master, AXP_CHARGE_VBUS, 0x01);
			if(ret)
				printk("[AXP]Uable to set usb current set 500mA\n");
		}
		else if (cur == 100){
			ret = axp_set_bits(axpcharger->master, AXP_CHARGE_VBUS, 0x01);
			if(ret)
				printk("[AXP]Uable to set usb current set 100mA\n");
		}
	}
	else
		ret = axp_clr_bits(axpcharger->master, AXP_CHARGE_VBUS, 0x02);
		if(ret)
			printk("[AXP]Uable to set usb current set 0mA\n");
#endif

#endif
	return ret;	
}

static int axp_battery_first_init(struct axp_charger *charger)
{
	int ret;	
	/* 使能adc，设置adc采样率 */
	ret = axp_battery_adc_set(charger);
	if(ret){
		printk("[AXP]Unable to enable adc nor set adc frequency\n");
		return ret;
	}
	/* 设置充电参数 */
	ret = axp_set_charge(charger);
	if(ret){
		printk("[AXP]Uable to set charger parameters\n");
		return ret;
	}
	/* 设置开关机参数 */
	ret = axp_set_opclinit();
	if(ret){
		printk("[AXP]Unable to set pek/noe/otp \n");
		return ret;
	}
	/* 设置ocv对应剩余电量表 */
	ret = axp_set_ocv();
	if(ret){
		printk("[AXP]Unable to set ocv/capicaty\n");
		return ret;
	}
	/* 设置低电唤醒警告电压 */
	ret = axp_set_warning(WARNVOL1,WARNVOL2);
	if(ret){
		printk("[AXP]Unable to set warning voltage\n");
		return ret;
	}
	/* 设置电池内阻的默认值 */
	ret = axp_set_rdcclose();
	if(ret){
		printk("[AXP]Unable to close rdc calculate\n");
		return ret;
	}
	/* irq pin 唤醒使能和16s长按ldo1关闭使能 */
	ret = axp_set_bits(charger->master,AXP_HOTOVER_CTL,0x88);
	if(ret){
		printk("[AXP]Unable to enable irq wake up and close ldo1\n");
		return ret;
	}
	/* usb 限压 */
	ret = axp_usb_vollim(USBVOLLIM);
	if(ret){
		printk("[AXP]Unable to set usb voltage limite\n");
		return ret;
	}
	/* usb 限流 */
	ret = axp_usb_curlim(USBCURLIM);
	if(ret)
		printk("[AXP]Unable to set usb current limite\n");
	return ret;
}

static int axp_get_rdc(struct axp_charger *charger)
{
	int temp;
	int rdc;
	uint8_t v[2];
	axp_reads(charger->master,AXP_RDC_BUFFER0,2,v);
#if defined (CONFIG_KP_AXP20)
	rdc = (((v[0] & 0x1F) << 8) | v[1]) * 10742 / 10000;
	axp_read(charger->master,AXP_DATA_BUFFER0,&v[0]);
#endif

#if defined (CONFIG_KP_AXP19)
	rdc = v[1]*2;
#endif
	printk("%s->%d,calculate rdc: rdc = %d\n",__FUNCTION__,__LINE__,rdc);
	axp_charger_update_state(charger);	
	axp_charger_update(charger);
	if(!charger->bat_det){
		charger->disvbat = 0;
		charger->disibat = 0;
		return rdc;
	}
	if( charger->ext_valid){
		if( (axp_ibat_to_mA(charger->adc.idischar_res) == 0) && (axp_ibat_to_mA(charger->adc.ichar_res) > 200)){
		}
		else{
			printk("%s->%d\n",__FUNCTION__,__LINE__);
			charger->disvbat = 0;
			charger->disibat = 0;
			return rdc;
		}
		printk("CHARGING:      charger->vbat = %d,   charger->ibat = %d\n",charger->vbat,charger->ibat);
		printk("DISCHARGING:charger->disvbat = %d,charger->disibat = %d\n",charger->disvbat,charger->disibat);
		if(!charger->bat_current_direction){
			charger->disvbat = 0;
			charger->disibat = 0;
			return rdc;
		}
		if(charger->disvbat == 0){
			charger->disvbat = 0;
			charger->disibat = 0;
			return rdc;
		}
		else{
			temp = 1000 * ABS(charger->vbat - charger->disvbat) / ABS(charger->ibat + charger->disibat);
			printk("CALRDC:temp = %d\n",temp);
			charger->disvbat = 0;
			charger->disibat = 0;
			if((temp < BATRDC/4) || (temp > BATRDC*3))
				return rdc;
			else {
#if defined (CONFIG_KP_AXP20)
				axp_set_bits(charger->master,AXP_DATA_BUFFER0,0x08);
#endif 
#if defined (CONFIG_KP_AXP19)
				axp_set_bits(charger->master,AXP_DATA_BUFFER2,0x01);
#endif 
				return (temp);
			}
		}
	}
	else{
		charger->disvbat = 0;
		charger->disibat = 0;
		return rdc;
	}
}

static void axp_cal_rdc(struct axp_charger *charger)
{
	uint8_t val;
	int rdc;
	if(charger->is_on && (axp_ibat_to_mA(charger->adc.ichar_res) > 200) && (charger->vbat > 3600) && (charger->disibat >= 400)){
#if defined (CONFIG_KP_AXP20)
		axp_read(charger->master,AXP_DATA_BUFFER0,&val);
		if((((val >> 7) == 0) || (((val >> 3) & 0x01) == 0) )&& (rdc_count >= TIMER3)){
			rdc = (axp_get_rdc(charger) * 10000 + 5371) / 10742;
			axp_set_bits(charger->master,AXP_CAP,0x80);
			axp_clr_bits(charger->master,AXP_RDC_BUFFER0,0x80);
			axp_write(charger->master,AXP_RDC_BUFFER1,rdc & 0x00FF);
			axp_update(charger->master, AXP_RDC_BUFFER0, (rdc >> 8), 0x1F);
			axp_clr_bits(charger->master,AXP_CAP,0x80);
			/* 设置标志位 */
			axp_set_bits(charger->master,AXP_DATA_BUFFER0,0x80);																																									
			printk("rdc = %d\n",rdc * 10742 / 10000);
			state_count = 1;
		} else if((((val >> 7) == 0) || (((val >> 3) & 0x1) == 0)) && (rdc_count < TIMER3)){
			rdc_count ++;  
#endif

#if defined (CONFIG_KP_AXP19)
		axp_read(charger->master,AXP_DATA_BUFFER2,&val);
		if(((((val >> 3)& 0x01) == 0) || (((val >> 0) & 0x01) == 0) )&& (rdc_count >= TIMER3)){
			rdc = axp_get_rdc(charger);
			axp_write(charger->master,AXP_RDC_BUFFER1,(rdc / 2) & 0x00FF);
			axp_set_bits(charger->master,AXP_RDC_BUFFER0,0x08);
			printk("rdc = %d\n",rdc);
			state_count = 1;
		} else if(((((val >> 3)& 0x01) == 0) || (((val >> 0) & 0x1) == 0)) && (rdc_count < TIMER3)){
			rdc_count ++;  
#endif
		} else {
			rdc_count = 0;
		}
	} else {
		rdc_count = 0;
	}
}

#if defined (CONFIG_KP_AXP20)
static int axp_get_coulomb(struct axp_charger *charger)
{
	uint8_t  temp[8];
	int64_t  rValue1,rValue2,rValue;
	int Cur_CoulombCounter_tmp,m;

	axp_reads(charger->master, AXP_CCHAR3_RES,8,temp);
	rValue1 = ((temp[0] << 24) + (temp[1] << 16) + (temp[2] << 8) + temp[3]);
	rValue2 = ((temp[4] << 24) + (temp[5] << 16) + (temp[6] << 8) + temp[7]);
	if(1){
		DBG_PSY_MSG("%s->%d -     CHARGINGOULB:[0]=0x%x,[1]=0x%x,[2]=0x%x,[3]=0x%x\n",__FUNCTION__,__LINE__,temp[0],temp[1],temp[2],temp[3]);
		DBG_PSY_MSG("%s->%d - DISCHARGINGCLOUB:[4]=0x%x,[5]=0x%x,[6]=0x%x,[7]=0x%x\n",__FUNCTION__,__LINE__,temp[4],temp[5],temp[6],temp[7]);
	}
	rValue = (ABS(rValue1 - rValue2)) * 4369;
	m = axp_get_freq() * 480;
	do_div(rValue,m);
	if(rValue1 >= rValue2)
		Cur_CoulombCounter_tmp = (int)rValue;
	else
		Cur_CoulombCounter_tmp = (int)(0 - rValue);
	if(1)
		DBG_PSY_MSG("Cur_CoulombCounter_tmp = %d\n",Cur_CoulombCounter_tmp);
	return Cur_CoulombCounter_tmp;	//unit mAh
}
#endif

#if defined (CONFIG_KP_AXP19)
static int axp_get_coulomb(struct axp_charger *charger)
{
	uint8_t temp[8];
	int64_t rValue1,rValue2,rValue;
	uint8_t ic_type;
	int Cur_CoulombCounter_tmp,m;
	
	axp_read(charger->master,AXP_IC_TYPE, &temp[0]);
	if( (temp[0] & 0x0F) == 0x03){
		ic_type = 1;
	}
	else{
		ic_type = 0;
	}
	axp_reads(charger->master,AXP_CCHAR3_RES,8,temp);
	if(ic_type){
		rValue1 = ((temp[0] << 24) + (temp[1] << 16) + (temp[2] << 8) + temp[3]);
		rValue2 = ((temp[4] << 24) + (temp[5] << 16) + (temp[6] << 8) + temp[7]);
		rValue = ABS((rValue1 - rValue2))* 2048;
		m = axp_get_freq() * 225;
		do_div(rValue,m);
	}
	else{
		rValue1 = ((temp[0] << 24) + (temp[1] << 16) + (temp[2] << 8) + temp[3]);
		rValue2 = ((temp[4] << 24) + (temp[5] << 16) + (temp[6] << 8) + temp[7]);
		rValue = ABS(rValue1 - rValue2);
		m = axp_get_freq()* 3600 * 2;
		do_div(rValue,m);
	}
	if(axp_debug){
		DBG_PSY_MSG("%s->%d:ic_type = %d\n",__FUNCTION__,__LINE__,ic_type);
		DBG_PSY_MSG("%s->%d -     CHARGINGOULB:[0]=0x%x,[1]=0x%x,[2]=0x%x,[3]=0x%x\n",__FUNCTION__,__LINE__,temp[0],temp[1],temp[2],temp[3]);
		DBG_PSY_MSG("%s->%d - DISCHARGINGCLOUB:[4]=0x%x,[5]=0x%x,[6]=0x%x,[7]=0x%x\n",__FUNCTION__,__LINE__,temp[4],temp[5],temp[6],temp[7]);
	}
	if(rValue1 >= rValue2)
		Cur_CoulombCounter_tmp = (int)rValue;
	else
		Cur_CoulombCounter_tmp = (int)(0 - rValue);
	if(axp_debug)
		DBG_PSY_MSG("Cur_CoulombCounter_tmp = %d\n",Cur_CoulombCounter_tmp);
	return Cur_CoulombCounter_tmp;	//unit mAh
}
#endif

static void axp_clr_coulomb(struct axp_charger *charger)
{
	uint8_t temp;
	axp_read(charger->master, AXP_COULOMB_CONTROL, &temp);
	temp |= 0x20;
	temp &= 0xbf;
	axp_write(charger->master, AXP_COULOMB_CONTROL, temp);
	temp |= 0x80;
	temp &= 0xbf;
	axp_write(charger->master, AXP_COULOMB_CONTROL, temp);
}

//#if defined (CONFIG_KP_AXP19)
static int axp_bat_vol(bool dir,int vol,int cur,int rdc)
{
	if(dir)
		return (vol-cur*rdc/1000);
	else
		return (vol+cur*rdc/1000);
}

static uint8_t axp_vol2cap(int ocv)
{
    if(ocv >= OCVVOLF)
    {
        return OCVREGF;
    }
    else if(ocv < OCVVOL0)
    {
        return OCVREG0;
    }
    else if(ocv < OCVVOL1)
    {
        return OCVREG0 + (OCVREG1 - OCVREG0) * (ocv - OCVVOL0) / (OCVVOL1 - OCVVOL0);
    }
    else if(ocv < OCVVOL2)
    {
        return OCVREG1 + (OCVREG2 - OCVREG1) * (ocv - OCVVOL1) / (OCVVOL2 - OCVVOL1);
    }
    else if(ocv < OCVVOL3)
    {
        return OCVREG2 + (OCVREG3 - OCVREG2) * (ocv - OCVVOL2) / (OCVVOL3 - OCVVOL2);
    }
    else if(ocv < OCVVOL4)
    {
        return OCVREG3 + (OCVREG4 - OCVREG3) * (ocv - OCVVOL3) / (OCVVOL4 - OCVVOL3);
    }
    else if(ocv < OCVVOL5)
    {
        return OCVREG4 + (OCVREG5 - OCVREG4) * (ocv - OCVVOL4) / (OCVVOL5 - OCVVOL4);
    }
    else if(ocv < OCVVOL6)                               
    {
        return OCVREG5 + (OCVREG6 - OCVREG5) * (ocv - OCVVOL5) / (OCVVOL6 - OCVVOL5);
    }
    else if(ocv < OCVVOL7)
    {
        return OCVREG6 + (OCVREG7 - OCVREG6) * (ocv - OCVVOL6) / (OCVVOL7 - OCVVOL6);
    }
    else if(ocv < OCVVOL8)
    {
        return OCVREG7 + (OCVREG8 - OCVREG7) * (ocv - OCVVOL7) / (OCVVOL8 - OCVVOL7);
    }
    else if(ocv < OCVVOL9)
    {
        return OCVREG8 + (OCVREG9 - OCVREG8) * (ocv - OCVVOL8) / (OCVVOL9 - OCVVOL8);
    }
    else if(ocv < OCVVOLA)
    {
        return OCVREG9 + (OCVREGA - OCVREG9) * (ocv - OCVVOL9) / (OCVVOLA - OCVVOL9);
    }
    else if(ocv < OCVVOLB)
    {
        return OCVREGA + (OCVREGB - OCVREGA) * (ocv - OCVVOLA) / (OCVVOLB - OCVVOLA);
    }
    else if(ocv < OCVVOLC)
    {
        return OCVREGB + (OCVREGC - OCVREGB) * (ocv - OCVVOLB) / (OCVVOLC - OCVVOLB);
    }
    else if(ocv < OCVVOLD)
    {
        return OCVREGC + (OCVREGD - OCVREGC) * (ocv - OCVVOLC) / (OCVVOLD - OCVVOLC);
    }
    else if(ocv < OCVVOLE)
    {
        return OCVREGD + (OCVREGE - OCVREGD) * (ocv - OCVVOLD) / (OCVVOLE - OCVVOLD);
    }
    else if(ocv < OCVVOLF)
    {
        return OCVREGE + (OCVREGF - OCVREGE) * (ocv - OCVVOLE) / (OCVVOLF - OCVVOLE);
    }
    else
    {
        return 0;
    }
}

 uint8_t axp_ocv_restcap(struct axp_charger *charger)
{
	int rdc;
	uint8_t val[2];
	axp_reads(charger->master,AXP_RDC_BUFFER0,2,val);
	rdc = val[1] * 2;
	charger->ocv = axp_bat_vol(charger->bat_current_direction,charger->vbat,charger->ibat,rdc);
	return axp_vol2cap(charger->ocv);
}
//#endif
