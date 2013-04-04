/*µ÷ÊÔ½Ó¿Ú*/
static uint8_t axp_reg_addr = 0;

static ssize_t axp_reg_show(struct class *class, 
			struct class_attribute *attr,	char *buf)
{
    uint8_t val;
	axp_read(axpcharger->master,axp_reg_addr,&val);
	return sprintf(buf,"REG[%x]=%x\n",axp_reg_addr,val);
}

static ssize_t axp_reg_store(struct class *class, 
			struct class_attribute *attr,	const char *buf, size_t count)
{
	int tmp;
	uint8_t val;
	tmp = simple_strtoul(buf, NULL, 16);
	if( tmp < 256 )
		axp_reg_addr = tmp;
	else {
		val = tmp & 0x00FF;
		axp_reg_addr= (tmp >> 8) & 0x00FF;
		axp_write(axpcharger->master,axp_reg_addr, val);
	}
	return count;
}

static ssize_t axp_regs_show(struct class *class, 
			struct class_attribute *attr,	char *buf)
{
  uint8_t val[4];
	axp_reads(axpcharger->master,axp_reg_addr,4,val);
	return sprintf(buf,"REG[0x%x]=0x%x,REG[0x%x]=0x%x,REG[0x%x]=0x%x,REG[0x%x]=0x%x\n",axp_reg_addr,val[0],axp_reg_addr+1,val[1],axp_reg_addr+2,val[2],axp_reg_addr+3,val[3]);
}

static ssize_t axp_regs_store(struct class *class, 
			struct class_attribute *attr,	const char *buf, size_t count)
{
	int tmp;
	uint8_t val[5];
	tmp = simple_strtoul(buf, NULL, 16);
	if( tmp < 256 )
		axp_reg_addr = tmp;
	else {
		axp_reg_addr= (tmp >> 24) & 0xFF;
		val[0] = (tmp >> 16) & 0xFF;
		val[1] =  axp_reg_addr + 1;
		val[2] = (tmp >>  8)& 0xFF;
		val[3] =  axp_reg_addr + 2;
		val[4] = (tmp >>  0)& 0xFF;
		axp_writes(axpcharger->master,axp_reg_addr,5,val);
	}
	return count;
}

static ssize_t axpdebug_store(struct class *class, 
			struct class_attribute *attr,	const char *buf, size_t count)
{
	if(buf[0] == '1'){
	   axp_debug = 1; 
    }
    else{
	   axp_debug = 0;         
    }        
	return count;
}

static ssize_t axpdebug_show(struct class *class, 
			struct class_attribute *attr,	char *buf)
{
	return sprintf(buf, "bat-debug value is %d\n", axp_debug);
}

static struct class_attribute axppower_class_attrs[] = {
	__ATTR(axpdebug,S_IRUGO|S_IWUSR,axpdebug_show,axpdebug_store),
	__ATTR(axpreg,	S_IRUGO|S_IWUSR,axp_reg_show,axp_reg_store),
	__ATTR(axpregs,	S_IRUGO|S_IWUSR,axp_regs_show,axp_regs_store),
	__ATTR_NULL
};
static struct class axppower_class = {
    .name = "axppower",
    .class_attrs = axppower_class_attrs,
};

#if defined (CONFIG_AXP_DEBUG)
static ssize_t adcfreq_show(struct device *dev,
    struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "%d\n",axp_get_freq());
}

static ssize_t adcfreq_store(struct device *dev,
        struct device_attribute *attr, const char *buf, size_t count)
{
	int var;
	var = simple_strtoul(buf, NULL, 10);
	axp_set_freq(var);
	return count;
}

static ssize_t chgmillivol_show(struct device *dev,
    struct device_attribute *attr, char *buf)
{
	int var = 0;
	return sprintf(buf, "%d\n",var);
}

static ssize_t chgmillivol_store(struct device *dev,
        struct device_attribute *attr, const char *buf, size_t count)
{
	int var;
	var = simple_strtoul(buf, NULL, 10);
	axp_set_chargevoltage(var);
	return count;
}

static ssize_t chgendrate_show(struct device *dev,
    struct device_attribute *attr, char *buf)
{
	int var = 0;
	return sprintf(buf, "%d\n",var);
}

static ssize_t chgendrate_store(struct device *dev,
        struct device_attribute *attr, const char *buf, size_t count)
{
	int var;
	var = simple_strtoul(buf, NULL, 10);
	axp_set_chargeendrate(var);
	return count;
}

static ssize_t chgpretimemin_show(struct device *dev,
    struct device_attribute *attr, char *buf)
{
	int var = 0;
	return sprintf(buf, "%d\n",var);
}

static ssize_t chgpretimemin_store(struct device *dev,
        struct device_attribute *attr, const char *buf, size_t count)
{
	int var;
	var = simple_strtoul(buf, NULL, 10);
	axp_set_prechargetime(var);
	return count;
}

static ssize_t chgcsttimemin_show(struct device *dev,
    struct device_attribute *attr, char *buf)
{
	int var = 0;
	return sprintf(buf, "%d\n",var);
}

static ssize_t chgcsttimemin_store(struct device *dev,
        struct device_attribute *attr, const char *buf, size_t count)
{
	int var;
	var = simple_strtoul(buf, NULL, 10);
	axp_set_cstchargetime(var);
	return count;
}

static ssize_t pekopen_show(struct device *dev,
    struct device_attribute *attr, char *buf)
{
	int var = 0;
	return sprintf(buf, "%d\n",var);
}

static ssize_t pekopen_store(struct device *dev,
        struct device_attribute *attr, const char *buf, size_t count)
{
	int var;
	var = simple_strtoul(buf, NULL, 10);
	axp_set_pekopen(var);
	return count;
}

static ssize_t peklong_show(struct device *dev,
    struct device_attribute *attr, char *buf)
{
	int var = 0;
	return sprintf(buf, "%d\n",var);
}

static ssize_t peklong_store(struct device *dev,
        struct device_attribute *attr, const char *buf, size_t count)
{
	int var;
	var = simple_strtoul(buf, NULL, 10);
	axp_set_peklong(var);
	return count;
}

static ssize_t pekoffen_show(struct device *dev,
    struct device_attribute *attr, char *buf)
{
	int var = 0;
	return sprintf(buf, "%d\n",var);
}

static ssize_t pekoffen_store(struct device *dev,
        struct device_attribute *attr, const char *buf, size_t count)
{
	int var;
	var = simple_strtoul(buf, NULL, 10);
	axp_set_pekoffen(var);
	return count;
}

static ssize_t pekdelay_show(struct device *dev,
    struct device_attribute *attr, char *buf)
{
	int var = 0;
	return sprintf(buf, "%d\n",var);
}

static ssize_t pekdelay_store(struct device *dev,
        struct device_attribute *attr, const char *buf, size_t count)
{
	int var;
	var = simple_strtoul(buf, NULL, 10);
	axp_set_pekdelay(var);
	return count;
}

static ssize_t pekoff_show(struct device *dev,
    struct device_attribute *attr, char *buf)
{
	int var = 0;
	return sprintf(buf, "%d\n",var);
}

static ssize_t pekoff_store(struct device *dev,
        struct device_attribute *attr, const char *buf, size_t count)
{
	int var;
	var = simple_strtoul(buf, NULL, 10);
	axp_set_peklong(var);
	return count;
}

static ssize_t noe_show(struct device *dev,
    struct device_attribute *attr, char *buf)
{
	int var = 0;
	return sprintf(buf, "%d\n",var);
}

static ssize_t noe_store(struct device *dev,
        struct device_attribute *attr, const char *buf, size_t count)
{
	int var;
	var = simple_strtoul(buf, NULL, 10);
	axp_set_noe(var);
	return count;
}

static ssize_t otpoffen_show(struct device *dev,
    struct device_attribute *attr, char *buf)
{
	int var = 0;
	return sprintf(buf, "%d\n",var);
}

static ssize_t otpoffen_store(struct device *dev,
        struct device_attribute *attr, const char *buf, size_t count)
{
	int var;
	var = simple_strtoul(buf, NULL, 10);
	axp_set_otpoffen(var);
	return count;
}

static ssize_t vhold_show(struct device *dev,
    struct device_attribute *attr, char *buf)
{
	int var = 0;
	return sprintf(buf, "%d\n",var);
}

static ssize_t vhold_store(struct device *dev,
        struct device_attribute *attr, const char *buf, size_t count)
{
	int var;
	var = simple_strtoul(buf, NULL, 10);
	axp_usb_vollim(var);
	return count;
}

static ssize_t ihold_show(struct device *dev,
    struct device_attribute *attr, char *buf)
{
	int var = 0;
	return sprintf(buf, "%d\n",var);
}

static ssize_t ihold_store(struct device *dev,
        struct device_attribute *attr, const char *buf, size_t count)
{
	int var;
	var = simple_strtoul(buf, NULL, 10);
	axp_usb_curlim(var);
	return count;
}
#endif

#if defined (CONFIG_AXP_CHGCURCHG) || (CONFIG_AXP_DEBUG)
static ssize_t chgmillicur_show(struct device *dev,
    struct device_attribute *attr, char *buf)
{
	int var = 0;
	return sprintf(buf, "%d\n",var);
}

static ssize_t chgmillicur_store(struct device *dev,
        struct device_attribute *attr, const char *buf, size_t count)
{
	struct axp_charger *charger = dev_get_drvdata(dev);
	int var;
	var = simple_strtoul(buf, NULL, 10);
	charger->chgcur = var;
	axp_set_chargecurrent(var);
	return count;
}

static ssize_t chgmilliearcur_show(struct device *dev,
    struct device_attribute *attr, char *buf)
{
	int var = 0;
	return sprintf(buf, "%d\n",var);
}

static ssize_t chgmilliearcur_store(struct device *dev,
        struct device_attribute *attr, const char *buf, size_t count)
{
	struct axp_charger *charger = dev_get_drvdata(dev);
	int var;
	var = simple_strtoul(buf, NULL, 10);
	charger->chgearcur = var;
	return count;
}

static ssize_t chgmillisuscur_show(struct device *dev,
    struct device_attribute *attr, char *buf)
{
	int var = 0;
	return sprintf(buf, "%d\n",var);
}

static ssize_t chgmillisuscur_store(struct device *dev,
        struct device_attribute *attr, const char *buf, size_t count)
{
	struct axp_charger *charger = dev_get_drvdata(dev);
	int var;
	var = simple_strtoul(buf, NULL, 10);
	charger->chgsuscur = var;
	return count;
}

static ssize_t chgmilliclscur_show(struct device *dev,
    struct device_attribute *attr, char *buf)
{
	int var = 0;
	return sprintf(buf, "%d\n",var);
}

static ssize_t chgmilliclscur_store(struct device *dev,
        struct device_attribute *attr, const char *buf, size_t count)
{
	struct axp_charger *charger = dev_get_drvdata(dev);
	int var;
	var = simple_strtoul(buf, NULL, 10);
	charger->chgclscur = var;
	return count;
}
#endif
static struct device_attribute axp_charger_attrs[] = {
#if defined (CONFIG_AXP_DEBUG)
	AXP_CHG_ATTR(adcfreq),
	AXP_CHG_ATTR(chgmillivol),
	AXP_CHG_ATTR(chgendrate),
	AXP_CHG_ATTR(chgpretimemin),
	AXP_CHG_ATTR(chgcsttimemin),
	AXP_CHG_ATTR(pekopen),
	AXP_CHG_ATTR(peklong),
	AXP_CHG_ATTR(pekoffen),
	AXP_CHG_ATTR(pekdelay),
	AXP_CHG_ATTR(pekoff),
	AXP_CHG_ATTR(noe),
	AXP_CHG_ATTR(otpoffen),
	AXP_CHG_ATTR(vhold),
	AXP_CHG_ATTR(ihold),
#endif
#if defined (CONFIG_AXP_CHGCURCHG) || (CONFIG_AXP_DEBUG)
	AXP_CHG_ATTR(chgmillicur),
	AXP_CHG_ATTR(chgmilliearcur),
	AXP_CHG_ATTR(chgmillisuscur),
	AXP_CHG_ATTR(chgmilliclscur),
#endif
};

#if defined (CONFIG_AXP_CHGCURCHG) || (CONFIG_AXP_DEBUG)
int axp_charger_create_attrs(struct power_supply *psy)
{
  int j,ret;
  for (j = 0; j < ARRAY_SIZE(axp_charger_attrs); j++) {
    ret = device_create_file(psy->dev,
          &axp_charger_attrs[j]);
    if (ret)
      goto sysfs_failed;
  }
    goto succeed;

sysfs_failed:
  while (j--)
    device_remove_file(psy->dev,
         &axp_charger_attrs[j]);
succeed:
  return ret;
}
#endif