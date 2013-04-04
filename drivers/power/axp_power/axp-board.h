/*
 * Base driver for X-Powers AXP
 *
 * Copyright (C) 2011 X-Powers, Ltd.
 *  Zhang Donglu <zhangdonglu@x-powers.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/init.h>
#include <linux/device.h>
#include <linux/platform_device.h>
#include <linux/i2c.h>
#include <mach/irqs.h>
#include <linux/power_supply.h>
#include <mach/gpio.h>

#include "axp-regu.h"
#include "axp-mfd.h"
#include "axp-cfg.h"

/* Reverse engineered partly from Platformx drivers */
enum axp_regls{
	vcc_ldo1,
	vcc_ldo2,
	vcc_ldo3,
	vcc_ldo4,

    vcc_buck1,
	vcc_buck2,
	vcc_buck3,
	
	vcc_ldoio0,
};

static struct regulator_consumer_supply ldo1_data[] = {
	{
		.supply = "ldo1",
	},
};

static struct regulator_consumer_supply ldo2_data[] = {
	{
		.supply = "ldo2",
	},
};

static struct regulator_consumer_supply ldo3_data[] = {
	{
		.supply = "ldo3",
	},
};

static struct regulator_consumer_supply ldo4_data[] = {
	{
		.supply = "ldo4",
	},
};

static struct regulator_consumer_supply buck1_data[] = {
	{
		.supply = "dcdc1",
	},
};

static struct regulator_consumer_supply buck2_data[] = {
	{
		.supply = "dcdc2",
	},
};

static struct regulator_consumer_supply buck3_data[] = {
	{
		.supply = "dcdc3",
	},
};

static struct regulator_consumer_supply ldoio0_data[] = {
	{
		.supply = "ldoio0",
	},
};



static struct regulator_init_data axp_regl_init_data[] = {
	
	[vcc_ldo1] = {
		.constraints = { 
			.name = "axp_ldo1",
			.min_uV =  AXP_LDO1_MIN,
			.max_uV =  AXP_LDO1_MAX,
		},
		.num_consumer_supplies = ARRAY_SIZE(ldo1_data),
		.consumer_supplies = ldo1_data,
	},
	[vcc_ldo2] = {
		.constraints = { 
			.name = "axp_ldo2",
			.min_uV = AXP_LDO2_MIN,
			.max_uV = AXP_LDO2_MAX,
			.valid_ops_mask = REGULATOR_CHANGE_VOLTAGE | REGULATOR_CHANGE_STATUS,
#if defined (CONFIG_KP_OUTPUTINIT)
			.initial_state = PM_SUSPEND_STANDBY,
			.state_standby = {
				.uV = AXP_LDO2_VALUE * 1000,
				.enabled = 1,
			}
#endif
		},
		.num_consumer_supplies = ARRAY_SIZE(ldo2_data),
		.consumer_supplies = ldo2_data,
	},
	[vcc_ldo3] = {
		.constraints = {
			.name = "axp_ldo3",
			.min_uV =  AXP_LDO3_MIN,
			.max_uV =  AXP_LDO3_MAX,
			.valid_ops_mask = REGULATOR_CHANGE_VOLTAGE | REGULATOR_CHANGE_STATUS,
#if defined (CONFIG_KP_OUTPUTINIT)
			.initial_state = PM_SUSPEND_STANDBY,
			.state_standby = {
				.uV = AXP_LDO3_VALUE * 1000,
				.enabled = 1,
			}
#endif
		},
		.num_consumer_supplies = ARRAY_SIZE(ldo3_data),
		.consumer_supplies = ldo3_data,
	},
	[vcc_ldo4] = {
		.constraints = {
			.name = "axp_ldo4",
			.min_uV = AXP_LDO4_MIN,
			.max_uV = AXP_LDO4_MAX,
			.valid_ops_mask = REGULATOR_CHANGE_VOLTAGE | REGULATOR_CHANGE_STATUS,
#if defined (CONFIG_KP_OUTPUTINIT)
			.initial_state = PM_SUSPEND_STANDBY,
			.state_standby = {
				.uV = AXP_LDO4_VALUE * 1000,
				.enabled = 1,
			}
#endif
		},
		.num_consumer_supplies = ARRAY_SIZE(ldo4_data),
		.consumer_supplies = ldo4_data,
	},
	[vcc_buck1] = {
		.constraints = { 
			.name = "axp_buck1",
			.min_uV = AXP_DCDC1_MIN,
			.max_uV = AXP_DCDC1_MAX,
			.valid_ops_mask = REGULATOR_CHANGE_VOLTAGE | REGULATOR_CHANGE_STATUS,
#if defined (CONFIG_KP_OUTPUTINIT)
			.initial_state = PM_SUSPEND_STANDBY,
			.state_standby = {
				.uV = AXP_DCDC1_VALUE * 1000,
				.enabled = 1,
			}
#endif
		},
		.num_consumer_supplies = ARRAY_SIZE(buck1_data),
		.consumer_supplies = buck1_data,
	},
	[vcc_buck2] = {
		.constraints = { 
			.name = "axp_buck2",
			.min_uV = AXP_DCDC2_MIN,
			.max_uV = AXP_DCDC2_MAX,
			.valid_ops_mask = REGULATOR_CHANGE_VOLTAGE | REGULATOR_CHANGE_STATUS,
#if defined (CONFIG_KP_OUTPUTINIT)
			.initial_state = PM_SUSPEND_STANDBY,
			.state_standby = {
				.uV = AXP_DCDC2_VALUE * 1000,
				.enabled = 1,
			}
#endif
		},
		.num_consumer_supplies = ARRAY_SIZE(buck2_data),
		.consumer_supplies = buck2_data,
	},
	[vcc_buck3] = {
		.constraints = { 
			.name = "axp_buck3",
			.min_uV = AXP_DCDC3_MIN,
			.max_uV = AXP_DCDC3_MAX,
			.valid_ops_mask = REGULATOR_CHANGE_VOLTAGE | REGULATOR_CHANGE_STATUS,
#if defined (CONFIG_KP_OUTPUTINIT)
			.initial_state = PM_SUSPEND_STANDBY,
			.state_standby = {
				.uV = AXP_DCDC3_VALUE * 1000,
				.enabled = 1,
			}
#endif
		},
		.num_consumer_supplies = ARRAY_SIZE(buck3_data),
		.consumer_supplies = buck3_data,
	},
	[vcc_ldoio0] = {
		.constraints = { 
			.name = "axp_ldoio0",
			.min_uV = AXP_LDOIO0_MIN,
			.max_uV = AXP_LDOIO0_MAX,
			.valid_ops_mask = REGULATOR_CHANGE_VOLTAGE | REGULATOR_CHANGE_STATUS,
		},
		.num_consumer_supplies = ARRAY_SIZE(ldoio0_data),
		.consumer_supplies = ldoio0_data,
	},
};

static struct axp_funcdev_info axp_regldevs[] = {
		{
		.name = "axp-regulator",
		.id = AXP_ID_LDO1,
		.platform_data = &axp_regl_init_data[vcc_ldo1],
	}, {
		.name = "axp-regulator",
		.id = AXP_ID_LDO2,
		.platform_data = &axp_regl_init_data[vcc_ldo2],
	}, {
		.name = "axp-regulator",
		.id = AXP_ID_LDO3,
		.platform_data = &axp_regl_init_data[vcc_ldo3],
	}, {
		.name = "axp-regulator",
		.id = AXP_ID_LDO4,
		.platform_data = &axp_regl_init_data[vcc_ldo4],
	}, {
		.name = "axp-regulator",
		.id = AXP_ID_BUCK1,
		.platform_data = &axp_regl_init_data[vcc_buck1],
	},{
		.name = "axp-regulator",
		.id = AXP_ID_BUCK2,
		.platform_data = &axp_regl_init_data[vcc_buck2],
	}, {
		.name = "axp-regulator",
		.id = AXP_ID_BUCK3,
		.platform_data = &axp_regl_init_data[vcc_buck3],
	}, {
		.name = "axp-regulator",
		.id = AXP_ID_LDOIO0,
		.platform_data = &axp_regl_init_data[vcc_ldoio0],
	},
};

static struct power_supply_info battery_data ={
	.name ="EASTROAD",
	.technology = POWER_SUPPLY_TECHNOLOGY_LION,
	.voltage_max_design = CHGVOL,
	.voltage_min_design = SHUTDOWNVOL,
	.energy_full_design = BATCAP,
};


static struct axp_supply_init_data axp_sply_init_data = {
	.battery_info = &battery_data,
	.chgcur = STACHGCUR,
	.chgearcur = EARCHGCUR,
	.chgsuscur = SUSCHGCUR,
	.chgclscur = CLSCHGCUR,
	.chgvol = CHGVOL,
	.chgend = ENDCHGRATE,
	.adc_freq = ADCFREQ,
	.chgpretime = CHGPRETIME,
	.chgcsttime = CHGCSTTIME,
};

static struct axp_funcdev_info axp_splydev[]={
	{
		.name = "axp-supplyer",
		.id = AXP_ID_SUPPLY,
		.platform_data = &axp_sply_init_data,
	},
};

static struct axp_platform_data axp_pdata = {
	.num_regl_devs = ARRAY_SIZE(axp_regldevs),
	.num_sply_devs = ARRAY_SIZE(axp_splydev),
	.regl_devs = axp_regldevs,
	.sply_devs = axp_splydev,
};

static int axp_detect(struct i2c_client *client, struct i2c_board_info *info)
{
	struct i2c_adapter *adapter = client->adapter;
	
	if (i2c_smbus_read_byte_data(client, AXP_IC_TYPE) < 0) {
		pr_err("AXP: Not Detected chip %s at adapter %d, address 0x%02x\n",
		"axp", i2c_adapter_id(adapter), client->addr);
		return -ENODEV;
	}
		          
	pr_info("AXP: Detected chip %s at adapter %d, address 0x%02x\n",
		"axp", i2c_adapter_id(adapter), client->addr);

	strlcpy(info->type, "axp_mfd", I2C_NAME_SIZE);

	info->platform_data  = &axp_pdata;
	info->irq  = gpio_to_irq(AXP_IRQNO);

	return 0;
}

