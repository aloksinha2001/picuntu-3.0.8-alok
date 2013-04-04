/*
 * Regulators driver for X-Powers AXP
 *
 * Copyright (C) 2011 X-Powers, Ltd.
 *  Zhang Donglu <zhangdonglu@x-powers.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/kernel.h>
#include <linux/err.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/module.h>

#include "axp-cfg.h"
#include "axp-mfd.h"
#include "axp-regu.h"

#if defined (CONFIG_KP_AXP20)
static int axp_ldo4_data[] = { 1250, 1300, 1400, 1500, 1600, 1700,
				   1800, 1900, 2000, 2500, 2700, 2800,
				   3000, 3100, 3200, 3300 };
#endif

static inline struct device *to_axp_dev(struct regulator_dev *rdev)
{
	return rdev_get_dev(rdev)->parent->parent;
}

static inline int check_range(struct axp_regulator_info *info,
				int min_uV, int max_uV)
{
	if (min_uV < info->min_uV || min_uV > info->max_uV)
		return -EINVAL;

	return 0;
}


/* AXP common operations */
static int axp_set_voltage(struct regulator_dev *rdev,
				  int min_uV, int max_uV, unsigned *selector)
{
	struct axp_regulator_info *info = rdev_get_drvdata(rdev);
	struct device *axp_dev = to_axp_dev(rdev);
	uint8_t val, mask;
	

	if (check_range(info, min_uV, max_uV)) {
		pr_err("invalid voltage range (%d, %d) uV\n", min_uV, max_uV);
		return -EINVAL;
	}

	val = (min_uV - info->min_uV + info->step_uV - 1) / info->step_uV;
	*selector = val;
	val <<= info->vol_shift;
	mask = ((1 << info->vol_nbits) - 1)  << info->vol_shift;

	return axp_update(axp_dev, info->vol_reg, val, mask);
}

static int axp_get_voltage(struct regulator_dev *rdev)
{
	struct axp_regulator_info *info = rdev_get_drvdata(rdev);
	struct device *axp_dev = to_axp_dev(rdev);
	uint8_t val, mask;
	int ret;

	ret = axp_read(axp_dev, info->vol_reg, &val);
	if (ret)
		return ret;
  
	mask = ((1 << info->vol_nbits) - 1)  << info->vol_shift;
	val = (val & mask) >> info->vol_shift;

	return info->min_uV + info->step_uV * val;
	
}

static int axp_enable(struct regulator_dev *rdev)
{
	struct axp_regulator_info *info = rdev_get_drvdata(rdev);
	struct device *axp_dev = to_axp_dev(rdev);

	return axp_set_bits(axp_dev, info->enable_reg,
					1 << info->enable_bit);
}

static int axp_disable(struct regulator_dev *rdev)
{
	struct axp_regulator_info *info = rdev_get_drvdata(rdev);
	struct device *axp_dev = to_axp_dev(rdev);

	return axp_clr_bits(axp_dev, info->enable_reg,
					1 << info->enable_bit);
}

static int axp_is_enabled(struct regulator_dev *rdev)
{
	struct axp_regulator_info *info = rdev_get_drvdata(rdev);
	struct device *axp_dev = to_axp_dev(rdev);
	uint8_t reg_val;
	int ret;

	ret = axp_read(axp_dev, info->enable_reg, &reg_val);
	if (ret)
		return ret;

	return !!(reg_val & (1 << info->enable_bit));
}

static int axp_list_voltage(struct regulator_dev *rdev, unsigned selector)
{
	struct axp_regulator_info *info = rdev_get_drvdata(rdev);
	int ret;
#if defined (CONFIG_KP_AXP20)
	if(info->desc.id == AXP_ID_LDO4)
		return axp_ldo4_data[selector] * 1000;
#endif
	
	ret = info->min_uV + info->step_uV * selector;
	if (ret > info->max_uV)
		return -EINVAL;
	return ret;
}
#if defined (CONFIG_KP_AXP20)
static int axp_set_ldo4_voltage(struct regulator_dev *rdev,
				  int min_uV, int max_uV, unsigned *selector)
{
	struct axp_regulator_info *info = rdev_get_drvdata(rdev);
	struct device *axp_dev = to_axp_dev(rdev);
	uint8_t val, mask;
	int i;
	
	if (check_range(info, min_uV, max_uV)) {
		pr_err("invalid voltage range (%d, %d) uV\n", min_uV, max_uV);
		return -EINVAL;
	}

	for(i = 0,val = 0; i < sizeof(axp_ldo4_data);i++){
		if(min_uV <= axp_ldo4_data[i] * 1000){
			val = i;
			break;
		}
	}
	
	*selector = val;
	val <<= info->vol_shift;
	mask = ((1 << info->vol_nbits) - 1)  << info->vol_shift;
	return axp_update(axp_dev, info->vol_reg, val, mask);
}

static int axp_get_ldo4_voltage(struct regulator_dev *rdev)
{
	struct axp_regulator_info *info = rdev_get_drvdata(rdev);
	struct device *axp_dev = to_axp_dev(rdev);
	uint8_t val, mask;
	int ret;

	ret = axp_read(axp_dev, info->vol_reg, &val);
	if (ret)
		return ret;
  
	mask = ((1 << info->vol_nbits) - 1)  << info->vol_shift;
	val = (val & mask) >> info->vol_shift;
	ret = axp_ldo4_data[val]*1000;
	return ret;
}
#endif

static int axp_ldoio0_enable(struct regulator_dev *rdev)
{
	struct axp_regulator_info *info = rdev_get_drvdata(rdev);
	struct device *axp_dev = to_axp_dev(rdev);
#if defined (CONFIG_KP_AXP20)
	 axp_set_bits(axp_dev, info->enable_reg,0x03);
	 return axp_clr_bits(axp_dev, info->enable_reg,0x04);
#endif
#if defined (CONFIG_KP_AXP19)
	 axp_set_bits(axp_dev, info->enable_reg,0x02);
	 return axp_clr_bits(axp_dev, info->enable_reg,0x05);
#endif
}

static int axp_ldoio0_disable(struct regulator_dev *rdev)
{
	struct axp_regulator_info *info = rdev_get_drvdata(rdev);
	struct device *axp_dev = to_axp_dev(rdev);
#if defined (CONFIG_KP_AXP20)
	return axp_clr_bits(axp_dev, info->enable_reg,0x07);
#endif
#if defined (CONFIG_KP_AXP19)
	axp_set_bits(axp_dev, info->enable_reg,0x05);
	return axp_clr_bits(axp_dev, info->enable_reg,0x02);
#endif
}

static int axp_ldoio0_is_enabled(struct regulator_dev *rdev)
{
	struct axp_regulator_info *info = rdev_get_drvdata(rdev);
	struct device *axp_dev = to_axp_dev(rdev);
	uint8_t reg_val;
	int ret;

	ret = axp_read(axp_dev, info->enable_reg, &reg_val);
	if (ret)
		return ret;
#if defined (CONFIG_KP_AXP20)
	return (((reg_val &= 0x07)== 0x03)?1:0);
#endif
#if defined (CONFIG_KP_AXP19)
	return (((reg_val &= 0x07)== 0x02)?1:0);
#endif
}

static int axp_set_suspend_voltage(struct regulator_dev *rdev, int uV)
{
	int ldo = rdev_get_id(rdev);
	unsigned int selector;

	switch (ldo) {
	
	case AXP_ID_LDO1 ... AXP_ID_LDO3:
		return axp_set_voltage(rdev, uV, uV, &selector);
	case AXP_ID_LDO4:
#if defined (CONFIG_KP_AXP20)
		return axp_set_ldo4_voltage(rdev, uV, uV, &selector);
#endif
#if defined (CONFIG_KP_AXP19)
		return axp_set_voltage(rdev, uV, uV, &selector);
#endif
	case AXP_ID_BUCK1 ... AXP_ID_LDOIO0:
		return axp_set_voltage(rdev, uV, uV, &selector);
	default:
		return -EINVAL;
	}
}


static struct regulator_ops axp_ops = {
	.set_voltage	= axp_set_voltage,
	.get_voltage	= axp_get_voltage,
	.list_voltage	= axp_list_voltage,
	.enable		= axp_enable,
	.disable	= axp_disable,
	.is_enabled	= axp_is_enabled,
	.set_suspend_enable		= axp_enable,
	.set_suspend_disable	= axp_disable,
	.set_suspend_voltage	= axp_set_suspend_voltage,
};
#if defined (CONFIG_KP_AXP20)
static struct regulator_ops axp_ldo4_ops = {
	.set_voltage	= axp_set_ldo4_voltage,
	.get_voltage	= axp_get_ldo4_voltage,
	.list_voltage	= axp_list_voltage,
	.enable		= axp_enable,
	.disable	= axp_disable,
	.is_enabled	= axp_is_enabled,
	.set_suspend_enable		= axp_enable,
	.set_suspend_disable	= axp_disable,
	.set_suspend_voltage	= axp_set_suspend_voltage,
};
#endif
#if defined (CONFIG_KP_AXP19)
static struct regulator_ops axp_ldo4_ops = {
	.set_voltage	= axp_set_voltage,
	.get_voltage	= axp_get_voltage,
	.list_voltage	= axp_list_voltage,
	.enable		= axp_enable,
	.disable	= axp_disable,
	.is_enabled	= axp_is_enabled,
	.set_suspend_enable		= axp_enable,
	.set_suspend_disable	= axp_disable,
	.set_suspend_voltage	= axp_set_suspend_voltage,
};
#endif


static struct regulator_ops axp_ldoio0_ops = {
	.set_voltage	= axp_set_voltage,
	.get_voltage	= axp_get_voltage,
	.list_voltage	= axp_list_voltage,
	.enable		= axp_ldoio0_enable,
	.disable	= axp_ldoio0_disable,
	.is_enabled	= axp_ldoio0_is_enabled,
	.set_suspend_enable		= axp_ldoio0_enable,
	.set_suspend_disable	= axp_ldoio0_disable,
	.set_suspend_voltage	= axp_set_suspend_voltage,
};

static struct axp_regulator_info axp_regulator_info[] = {
	AXP_LDO(1),
	AXP_LDO(2),
	AXP_LDO(3),
	AXP_LDO(4),
	
	AXP_BUCK(1),
	AXP_BUCK(2),
	AXP_BUCK(3),
	
	AXP_LDO(IO0),
};

static inline struct axp_regulator_info *find_regulator_info(int id)
{
	struct axp_regulator_info *ri;
	int i;

	for (i = 0; i < ARRAY_SIZE(axp_regulator_info); i++) {
		ri = &axp_regulator_info[i];
		if (ri->desc.id == id)
			return ri;
	}
	return NULL;
}

static int __devinit axp_regulator_probe(struct platform_device *pdev)
{
	struct axp_regulator_info *ri = NULL;
	struct regulator_dev *rdev;
	

	ri = find_regulator_info(pdev->id);
	if (ri == NULL) {
		dev_err(&pdev->dev, "invalid regulator ID specified\n");
		return -EINVAL;
	}

	if (ri->desc.id == AXP_ID_LDO1 || ri->desc.id == AXP_ID_LDO2 \
		|| ri->desc.id == AXP_ID_LDO3 || ri->desc.id == AXP_ID_BUCK1 \
		|| ri->desc.id == AXP_ID_BUCK2 ||ri->desc.id == AXP_ID_BUCK3)
		ri->desc.ops = &axp_ops;
	
	if(ri->desc.id == AXP_ID_LDO4)
		ri->desc.ops = &axp_ldo4_ops;


	if(ri->desc.id == AXP_ID_LDOIO0)
		ri->desc.ops = &axp_ldoio0_ops;


	rdev = regulator_register(&ri->desc, &pdev->dev,
				  pdev->dev.platform_data, ri);
	if (IS_ERR(rdev)) {
		dev_err(&pdev->dev, "failed to register regulator %s\n",
				ri->desc.name);
		return PTR_ERR(rdev);
	}
	platform_set_drvdata(pdev, rdev);
	
	return 0;
}

static int __devexit axp_regulator_remove(struct platform_device *pdev)
{
	struct regulator_dev *rdev = platform_get_drvdata(pdev);

	regulator_unregister(rdev);
	return 0;
}

static struct platform_driver axp_regulator_driver = {
	.driver	= {
		.name	= "axp-regulator",
		.owner	= THIS_MODULE,
	},
	.probe		= axp_regulator_probe,
	.remove		= axp_regulator_remove,
};

static int __init axp_regulator_init(void)
{
	return platform_driver_register(&axp_regulator_driver);
}
subsys_initcall(axp_regulator_init);

static void __exit axp_regulator_exit(void)
{
	platform_driver_unregister(&axp_regulator_driver);
}
module_exit(axp_regulator_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Donglu Zhang, <zhangdonglu@x-powers.com>");
MODULE_DESCRIPTION("Regulator Driver for X-Powers AXP PMIC");
MODULE_ALIAS("platform:axp-regulator");