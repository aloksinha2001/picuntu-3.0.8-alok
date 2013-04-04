/* arch/arm/mach-rk30/board-rk30-sdk.c
 *
 * Copyright (C) 2012 ROCKCHIP, Inc.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/input.h>
#include <linux/io.h>
#include <linux/delay.h>
#include <linux/i2c.h>
#include <linux/skbuff.h>
#include <linux/spi/spi.h>
#include <linux/mmc/host.h>
#include <linux/ion.h>
#include <linux/cpufreq.h>
#include <linux/clk.h>
#include <mach/dvfs.h>

#include <asm/setup.h>
#include <asm/mach-types.h>
#include <asm/mach/arch.h>
#include <asm/mach/map.h>
#include <asm/mach/flash.h>
#include <asm/hardware/gic.h>

#include <mach/board.h>
#include <mach/hardware.h>
#include <mach/io.h>
#include <mach/gpio.h>
#include <mach/iomux.h>
#include <linux/fb.h>
#include <linux/power/smb347-charger.h>
#include <linux/regulator/machine.h>
#include <linux/rfkill-rk.h>
#include <linux/sensor-dev.h>
#ifdef CONFIG_RK30_PWM_REGULATOR
#include <linux/regulator/rk29-pwm-regulator.h>
#endif
#if defined(CONFIG_HDMI_RK30)
	#include "../../../drivers/video/rockchip/hdmi/rk_hdmi.h"
#endif

#if defined(CONFIG_SPIM_RK29)
#include "../../../drivers/spi/rk29_spim.h"
#endif
#if defined(CONFIG_MU509)
#include <linux/mu509.h>
#endif
#if defined(CONFIG_MW100)
#include <linux/mw100.h>
#endif
#if defined(CONFIG_MT6229)
#include <linux/mt6229.h>
#endif
#if defined(CONFIG_SEW868)
#include <linux/sew868.h>
#endif
#if defined(CONFIG_ANDROID_TIMED_GPIO)
#include "../../../drivers/staging/android/timed_gpio.h"
#endif

#ifdef  CONFIG_THREE_FB_BUFFER
#define RK30_FB0_MEM_SIZE 12*SZ_1M
#else
#define RK30_FB0_MEM_SIZE 8*SZ_1M
#endif

#ifdef CONFIG_VIDEO_RK29
/*---------------- Camera Sensor Macro Define Begin  ------------------------*/
/*---------------- Camera Sensor Configuration Macro Begin ------------------------*/
#define CONFIG_SENSOR_0 RK29_CAM_SENSOR_GT2005						/* back camera sensor */
#define CONFIG_SENSOR_IIC_ADDR_0		0x78
#define CONFIG_SENSOR_IIC_ADAPTER_ID_0	  3
#define CONFIG_SENSOR_CIF_INDEX_0                    0
#define CONFIG_SENSOR_ORIENTATION_0 	  90
#define CONFIG_SENSOR_POWER_PIN_0		  INVALID_GPIO
#define CONFIG_SENSOR_RESET_PIN_0		  INVALID_GPIO
#define CONFIG_SENSOR_POWERDN_PIN_0 	  RK30_PIN4_PC3
#define CONFIG_SENSOR_FALSH_PIN_0		  INVALID_GPIO
#define CONFIG_SENSOR_POWERACTIVE_LEVEL_0 RK29_CAM_POWERACTIVE_L
#define CONFIG_SENSOR_RESETACTIVE_LEVEL_0 RK29_CAM_RESETACTIVE_L
#define CONFIG_SENSOR_POWERDNACTIVE_LEVEL_0 RK29_CAM_POWERDNACTIVE_L
#define CONFIG_SENSOR_FLASHACTIVE_LEVEL_0 RK29_CAM_FLASHACTIVE_L

#define CONFIG_SENSOR_QCIF_FPS_FIXED_0		15000
#define CONFIG_SENSOR_240X160_FPS_FIXED_0   15000
#define CONFIG_SENSOR_QVGA_FPS_FIXED_0		15000
#define CONFIG_SENSOR_CIF_FPS_FIXED_0		15000
#define CONFIG_SENSOR_VGA_FPS_FIXED_0		15000
#define CONFIG_SENSOR_480P_FPS_FIXED_0		15000
#define CONFIG_SENSOR_SVGA_FPS_FIXED_0		15000
#define CONFIG_SENSOR_720P_FPS_FIXED_0		30000

#define CONFIG_SENSOR_01  RK29_CAM_SENSOR_SP2518                   /* back camera sensor 1 */
#define CONFIG_SENSOR_IIC_ADDR_01 	    0x60
#define CONFIG_SENSOR_CIF_INDEX_01                    0
#define CONFIG_SENSOR_IIC_ADAPTER_ID_01    3
#define CONFIG_SENSOR_ORIENTATION_01       90
#define CONFIG_SENSOR_POWER_PIN_01         INVALID_GPIO
#define CONFIG_SENSOR_RESET_PIN_01         INVALID_GPIO
#define CONFIG_SENSOR_POWERDN_PIN_01       RK30_PIN4_PC3
#define CONFIG_SENSOR_FALSH_PIN_01         INVALID_GPIO
#define CONFIG_SENSOR_POWERACTIVE_LEVEL_01 RK29_CAM_POWERACTIVE_L
#define CONFIG_SENSOR_RESETACTIVE_LEVEL_01 RK29_CAM_RESETACTIVE_L
#define CONFIG_SENSOR_POWERDNACTIVE_LEVEL_01 RK29_CAM_POWERDNACTIVE_H
#define CONFIG_SENSOR_FLASHACTIVE_LEVEL_01 RK29_CAM_FLASHACTIVE_L

#define CONFIG_SENSOR_QCIF_FPS_FIXED_01      15000
#define CONFIG_SENSOR_240X160_FPS_FIXED_01   15000
#define CONFIG_SENSOR_QVGA_FPS_FIXED_01      15000
#define CONFIG_SENSOR_CIF_FPS_FIXED_01       15000
#define CONFIG_SENSOR_VGA_FPS_FIXED_01       15000
#define CONFIG_SENSOR_480P_FPS_FIXED_01      15000
#define CONFIG_SENSOR_SVGA_FPS_FIXED_01      15000
#define CONFIG_SENSOR_720P_FPS_FIXED_01     30000

#define CONFIG_SENSOR_02 RK29_CAM_SENSOR_GC2035                      /* back camera sensor 2 */
#define CONFIG_SENSOR_IIC_ADDR_02 	    0x78
#define CONFIG_SENSOR_CIF_INDEX_02                    0
#define CONFIG_SENSOR_IIC_ADAPTER_ID_02    3
#define CONFIG_SENSOR_ORIENTATION_02       90
#define CONFIG_SENSOR_POWER_PIN_02         INVALID_GPIO
#define CONFIG_SENSOR_RESET_PIN_02         INVALID_GPIO
#define CONFIG_SENSOR_POWERDN_PIN_02       RK30_PIN4_PC3
#define CONFIG_SENSOR_FALSH_PIN_02         INVALID_GPIO
#define CONFIG_SENSOR_POWERACTIVE_LEVEL_02 RK29_CAM_POWERACTIVE_L
#define CONFIG_SENSOR_RESETACTIVE_LEVEL_02 RK29_CAM_RESETACTIVE_L
#define CONFIG_SENSOR_POWERDNACTIVE_LEVEL_02 RK29_CAM_POWERDNACTIVE_H
#define CONFIG_SENSOR_FLASHACTIVE_LEVEL_02 RK29_CAM_FLASHACTIVE_L

#define CONFIG_SENSOR_QCIF_FPS_FIXED_02      15000
#define CONFIG_SENSOR_240X160_FPS_FIXED_02   15000
#define CONFIG_SENSOR_QVGA_FPS_FIXED_02      15000
#define CONFIG_SENSOR_CIF_FPS_FIXED_02       15000
#define CONFIG_SENSOR_VGA_FPS_FIXED_02       15000
#define CONFIG_SENSOR_480P_FPS_FIXED_02      15000
#define CONFIG_SENSOR_SVGA_FPS_FIXED_02      15000
#define CONFIG_SENSOR_720P_FPS_FIXED_02      30000

#define CONFIG_SENSOR_03 RK29_CAM_SENSOR_HI253                      /* back camera sensor 3 */
#define CONFIG_SENSOR_IIC_ADDR_03 	    0x40
#define CONFIG_SENSOR_CIF_INDEX_03                    0
#define CONFIG_SENSOR_IIC_ADAPTER_ID_03    3
#define CONFIG_SENSOR_ORIENTATION_03       90
#define CONFIG_SENSOR_POWER_PIN_03         INVALID_GPIO
#define CONFIG_SENSOR_RESET_PIN_03         INVALID_GPIO
#define CONFIG_SENSOR_POWERDN_PIN_03       RK30_PIN4_PC3
#define CONFIG_SENSOR_FALSH_PIN_03         INVALID_GPIO
#define CONFIG_SENSOR_POWERACTIVE_LEVEL_03 RK29_CAM_POWERACTIVE_L
#define CONFIG_SENSOR_RESETACTIVE_LEVEL_03 RK29_CAM_RESETACTIVE_L
#define CONFIG_SENSOR_POWERDNACTIVE_LEVEL_03 RK29_CAM_POWERDNACTIVE_H
#define CONFIG_SENSOR_FLASHACTIVE_LEVEL_03 RK29_CAM_FLASHACTIVE_L

#define CONFIG_SENSOR_QCIF_FPS_FIXED_03      15000
#define CONFIG_SENSOR_240X160_FPS_FIXED_03   15000
#define CONFIG_SENSOR_QVGA_FPS_FIXED_03      15000
#define CONFIG_SENSOR_CIF_FPS_FIXED_03       15000
#define CONFIG_SENSOR_VGA_FPS_FIXED_03       15000
#define CONFIG_SENSOR_480P_FPS_FIXED_03      15000
#define CONFIG_SENSOR_SVGA_FPS_FIXED_03      15000
#define CONFIG_SENSOR_720P_FPS_FIXED_03      30000

#define CONFIG_SENSOR_04 RK29_CAM_SENSOR_GC2015                      /* back camera sensor 4 */
#define CONFIG_SENSOR_IIC_ADDR_04 	    0x60
#define CONFIG_SENSOR_CIF_INDEX_04                    0
#define CONFIG_SENSOR_IIC_ADAPTER_ID_04    3
#define CONFIG_SENSOR_ORIENTATION_04       90
#define CONFIG_SENSOR_POWER_PIN_04         INVALID_GPIO
#define CONFIG_SENSOR_RESET_PIN_04         INVALID_GPIO
#define CONFIG_SENSOR_POWERDN_PIN_04       RK30_PIN4_PC3
#define CONFIG_SENSOR_FALSH_PIN_04         INVALID_GPIO
#define CONFIG_SENSOR_POWERACTIVE_LEVEL_04 RK29_CAM_POWERACTIVE_L
#define CONFIG_SENSOR_RESETACTIVE_LEVEL_04 RK29_CAM_RESETACTIVE_L
#define CONFIG_SENSOR_POWERDNACTIVE_LEVEL_04 RK29_CAM_POWERDNACTIVE_H
#define CONFIG_SENSOR_FLASHACTIVE_LEVEL_04 RK29_CAM_FLASHACTIVE_L

#define CONFIG_SENSOR_QCIF_FPS_FIXED_04      15000
#define CONFIG_SENSOR_240X160_FPS_FIXED_04   15000
#define CONFIG_SENSOR_QVGA_FPS_FIXED_04      15000
#define CONFIG_SENSOR_CIF_FPS_FIXED_04       15000
#define CONFIG_SENSOR_VGA_FPS_FIXED_04       15000
#define CONFIG_SENSOR_480P_FPS_FIXED_04      15000
#define CONFIG_SENSOR_SVGA_FPS_FIXED_04      15000
#define CONFIG_SENSOR_720P_FPS_FIXED_04      30000

#define CONFIG_SENSOR_05 RK29_CAM_SENSOR_OV5640                      /* back camera sensor 5 */
#define CONFIG_SENSOR_IIC_ADDR_05 	    0x78
#define CONFIG_SENSOR_CIF_INDEX_05                    0
#define CONFIG_SENSOR_IIC_ADAPTER_ID_05    3
#define CONFIG_SENSOR_ORIENTATION_05       90
#define CONFIG_SENSOR_POWER_PIN_05         INVALID_GPIO
#define CONFIG_SENSOR_RESET_PIN_05         INVALID_GPIO
#define CONFIG_SENSOR_POWERDN_PIN_05       RK30_PIN4_PC3
#define CONFIG_SENSOR_FALSH_PIN_05         INVALID_GPIO
#define CONFIG_SENSOR_POWERACTIVE_LEVEL_05 RK29_CAM_POWERACTIVE_L
#define CONFIG_SENSOR_RESETACTIVE_LEVEL_05 RK29_CAM_RESETACTIVE_L
#define CONFIG_SENSOR_POWERDNACTIVE_LEVEL_05 RK29_CAM_POWERDNACTIVE_H
#define CONFIG_SENSOR_FLASHACTIVE_LEVEL_05 RK29_CAM_FLASHACTIVE_L

#define CONFIG_SENSOR_QCIF_FPS_FIXED_05      15000
#define CONFIG_SENSOR_240X160_FPS_FIXED_05   15000
#define CONFIG_SENSOR_QVGA_FPS_FIXED_05      15000
#define CONFIG_SENSOR_CIF_FPS_FIXED_05       15000
#define CONFIG_SENSOR_VGA_FPS_FIXED_05       15000
#define CONFIG_SENSOR_480P_FPS_FIXED_05      15000
#define CONFIG_SENSOR_SVGA_FPS_FIXED_05      15000
#define CONFIG_SENSOR_720P_FPS_FIXED_05      30000

#define CONFIG_SENSOR_06 RK29_CAM_SENSOR_OV5640                      /* back camera sensor 6 */
#define CONFIG_SENSOR_IIC_ADDR_06 	    0x00
#define CONFIG_SENSOR_CIF_INDEX_06                    0
#define CONFIG_SENSOR_IIC_ADAPTER_ID_06    3
#define CONFIG_SENSOR_ORIENTATION_06       90
#define CONFIG_SENSOR_POWER_PIN_06         INVALID_GPIO
#define CONFIG_SENSOR_RESET_PIN_06         INVALID_GPIO
#define CONFIG_SENSOR_POWERDN_PIN_06       RK30_PIN4_PC3
#define CONFIG_SENSOR_FALSH_PIN_06         INVALID_GPIO
#define CONFIG_SENSOR_POWERACTIVE_LEVEL_06 RK29_CAM_POWERACTIVE_L
#define CONFIG_SENSOR_RESETACTIVE_LEVEL_06 RK29_CAM_RESETACTIVE_L
#define CONFIG_SENSOR_POWERDNACTIVE_LEVEL_06 RK29_CAM_POWERDNACTIVE_H
#define CONFIG_SENSOR_FLASHACTIVE_LEVEL_06 RK29_CAM_FLASHACTIVE_L

#define CONFIG_SENSOR_QCIF_FPS_FIXED_06      15000
#define CONFIG_SENSOR_240X160_FPS_FIXED_06   15000
#define CONFIG_SENSOR_QVGA_FPS_FIXED_06      15000
#define CONFIG_SENSOR_CIF_FPS_FIXED_06       15000
#define CONFIG_SENSOR_VGA_FPS_FIXED_06       15000
#define CONFIG_SENSOR_480P_FPS_FIXED_06      15000
#define CONFIG_SENSOR_SVGA_FPS_FIXED_06      15000
#define CONFIG_SENSOR_720P_FPS_FIXED_06      30000

#define CONFIG_SENSOR_07 RK29_CAM_SENSOR_OV5640                      /* back camera sensor 7 */
#define CONFIG_SENSOR_IIC_ADDR_07 	    0x00
#define CONFIG_SENSOR_CIF_INDEX_07                    0
#define CONFIG_SENSOR_IIC_ADAPTER_ID_07    3
#define CONFIG_SENSOR_ORIENTATION_07       90
#define CONFIG_SENSOR_POWER_PIN_07         INVALID_GPIO
#define CONFIG_SENSOR_RESET_PIN_07         INVALID_GPIO
#define CONFIG_SENSOR_POWERDN_PIN_07       RK30_PIN4_PC3
#define CONFIG_SENSOR_FALSH_PIN_07         INVALID_GPIO
#define CONFIG_SENSOR_POWERACTIVE_LEVEL_07 RK29_CAM_POWERACTIVE_L
#define CONFIG_SENSOR_RESETACTIVE_LEVEL_07 RK29_CAM_RESETACTIVE_L
#define CONFIG_SENSOR_POWERDNACTIVE_LEVEL_07 RK29_CAM_POWERDNACTIVE_H
#define CONFIG_SENSOR_FLASHACTIVE_LEVEL_07 RK29_CAM_FLASHACTIVE_L

#define CONFIG_SENSOR_QCIF_FPS_FIXED_07      15000
#define CONFIG_SENSOR_240X160_FPS_FIXED_07   15000
#define CONFIG_SENSOR_QVGA_FPS_FIXED_07      15000
#define CONFIG_SENSOR_CIF_FPS_FIXED_07       15000
#define CONFIG_SENSOR_VGA_FPS_FIXED_07       15000
#define CONFIG_SENSOR_480P_FPS_FIXED_07      15000
#define CONFIG_SENSOR_SVGA_FPS_FIXED_07      15000
#define CONFIG_SENSOR_720P_FPS_FIXED_07      30000

#define CONFIG_SENSOR_08 RK29_CAM_SENSOR_OV5640                      /* back camera sensor 8 */
#define CONFIG_SENSOR_IIC_ADDR_08 	    0x00
#define CONFIG_SENSOR_CIF_INDEX_08                    0
#define CONFIG_SENSOR_IIC_ADAPTER_ID_08    3
#define CONFIG_SENSOR_ORIENTATION_08       90
#define CONFIG_SENSOR_POWER_PIN_08         INVALID_GPIO
#define CONFIG_SENSOR_RESET_PIN_08         INVALID_GPIO
#define CONFIG_SENSOR_POWERDN_PIN_08       RK30_PIN4_PC3
#define CONFIG_SENSOR_FALSH_PIN_08         INVALID_GPIO
#define CONFIG_SENSOR_POWERACTIVE_LEVEL_08 RK29_CAM_POWERACTIVE_L
#define CONFIG_SENSOR_RESETACTIVE_LEVEL_08 RK29_CAM_RESETACTIVE_L
#define CONFIG_SENSOR_POWERDNACTIVE_LEVEL_08 RK29_CAM_POWERDNACTIVE_H
#define CONFIG_SENSOR_FLASHACTIVE_LEVEL_08 RK29_CAM_FLASHACTIVE_L

#define CONFIG_SENSOR_QCIF_FPS_FIXED_08      15000
#define CONFIG_SENSOR_240X160_FPS_FIXED_08   15000
#define CONFIG_SENSOR_QVGA_FPS_FIXED_08      15000
#define CONFIG_SENSOR_CIF_FPS_FIXED_08       15000
#define CONFIG_SENSOR_VGA_FPS_FIXED_08       15000
#define CONFIG_SENSOR_480P_FPS_FIXED_08      15000
#define CONFIG_SENSOR_SVGA_FPS_FIXED_08      15000
#define CONFIG_SENSOR_720P_FPS_FIXED_08      30000

#define CONFIG_SENSOR_09 RK29_CAM_SENSOR_OV5640                      /* back camera sensor 9 */
#define CONFIG_SENSOR_IIC_ADDR_09 	    0x00
#define CONFIG_SENSOR_CIF_INDEX_09                    0
#define CONFIG_SENSOR_IIC_ADAPTER_ID_09    3
#define CONFIG_SENSOR_ORIENTATION_09       90
#define CONFIG_SENSOR_POWER_PIN_09         INVALID_GPIO
#define CONFIG_SENSOR_RESET_PIN_09         INVALID_GPIO
#define CONFIG_SENSOR_POWERDN_PIN_09       RK30_PIN4_PC3
#define CONFIG_SENSOR_FALSH_PIN_09         INVALID_GPIO
#define CONFIG_SENSOR_POWERACTIVE_LEVEL_09 RK29_CAM_POWERACTIVE_L
#define CONFIG_SENSOR_RESETACTIVE_LEVEL_09 RK29_CAM_RESETACTIVE_L
#define CONFIG_SENSOR_POWERDNACTIVE_LEVEL_09 RK29_CAM_POWERDNACTIVE_H
#define CONFIG_SENSOR_FLASHACTIVE_LEVEL_09 RK29_CAM_FLASHACTIVE_L

#define CONFIG_SENSOR_QCIF_FPS_FIXED_09      15000
#define CONFIG_SENSOR_240X160_FPS_FIXED_09   15000
#define CONFIG_SENSOR_QVGA_FPS_FIXED_09      15000
#define CONFIG_SENSOR_CIF_FPS_FIXED_09       15000
#define CONFIG_SENSOR_VGA_FPS_FIXED_09       15000
#define CONFIG_SENSOR_480P_FPS_FIXED_09      15000
#define CONFIG_SENSOR_SVGA_FPS_FIXED_09      15000
#define CONFIG_SENSOR_720P_FPS_FIXED_09      30000

#define CONFIG_SENSOR_1 RK29_CAM_SENSOR_GC0308                     /* front camera sensor 0 */
#define CONFIG_SENSOR_IIC_ADDR_1 	    0x42
#define CONFIG_SENSOR_IIC_ADAPTER_ID_1	  3
#define CONFIG_SENSOR_CIF_INDEX_1				  0
#define CONFIG_SENSOR_ORIENTATION_1       270
#define CONFIG_SENSOR_POWER_PIN_1         INVALID_GPIO
#define CONFIG_SENSOR_RESET_PIN_1         INVALID_GPIO
#define CONFIG_SENSOR_POWERDN_PIN_1 	  RK30_PIN1_PB7
#define CONFIG_SENSOR_FALSH_PIN_1         INVALID_GPIO
#define CONFIG_SENSOR_POWERACTIVE_LEVEL_1 RK29_CAM_POWERACTIVE_L
#define CONFIG_SENSOR_RESETACTIVE_LEVEL_1 RK29_CAM_RESETACTIVE_L
#define CONFIG_SENSOR_POWERDNACTIVE_LEVEL_1 RK29_CAM_POWERDNACTIVE_H
#define CONFIG_SENSOR_FLASHACTIVE_LEVEL_1 RK29_CAM_FLASHACTIVE_L

#define CONFIG_SENSOR_QCIF_FPS_FIXED_1		15000
#define CONFIG_SENSOR_240X160_FPS_FIXED_1   15000
#define CONFIG_SENSOR_QVGA_FPS_FIXED_1		15000
#define CONFIG_SENSOR_CIF_FPS_FIXED_1		15000
#define CONFIG_SENSOR_VGA_FPS_FIXED_1		15000
#define CONFIG_SENSOR_480P_FPS_FIXED_1		15000
#define CONFIG_SENSOR_SVGA_FPS_FIXED_1		15000
#define CONFIG_SENSOR_720P_FPS_FIXED_1		30000

#define CONFIG_SENSOR_11 RK29_CAM_SENSOR_SP0838                      /* front camera sensor 1 */
#define CONFIG_SENSOR_IIC_ADDR_11 	    0x30
#define CONFIG_SENSOR_IIC_ADAPTER_ID_11    3
#define CONFIG_SENSOR_CIF_INDEX_11				  0
#define CONFIG_SENSOR_ORIENTATION_11       270
#define CONFIG_SENSOR_POWER_PIN_11         INVALID_GPIO
#define CONFIG_SENSOR_RESET_PIN_11         INVALID_GPIO
#define CONFIG_SENSOR_POWERDN_PIN_11       RK30_PIN1_PB7
#define CONFIG_SENSOR_FALSH_PIN_11         INVALID_GPIO
#define CONFIG_SENSOR_POWERACTIVE_LEVEL_11 RK29_CAM_POWERACTIVE_L
#define CONFIG_SENSOR_RESETACTIVE_LEVEL_11 RK29_CAM_RESETACTIVE_L
#define CONFIG_SENSOR_POWERDNACTIVE_LEVEL_11 RK29_CAM_POWERDNACTIVE_H
#define CONFIG_SENSOR_FLASHACTIVE_LEVEL_11 RK29_CAM_FLASHACTIVE_L

#define CONFIG_SENSOR_QCIF_FPS_FIXED_11      15000
#define CONFIG_SENSOR_240X160_FPS_FIXED_11   15000
#define CONFIG_SENSOR_QVGA_FPS_FIXED_11      15000
#define CONFIG_SENSOR_CIF_FPS_FIXED_11       15000
#define CONFIG_SENSOR_VGA_FPS_FIXED_11       15000
#define CONFIG_SENSOR_480P_FPS_FIXED_11      15000
#define CONFIG_SENSOR_SVGA_FPS_FIXED_11      15000
#define CONFIG_SENSOR_720P_FPS_FIXED_11      30000

#define CONFIG_SENSOR_12 RK29_CAM_SENSOR_GC2035                     /* front camera sensor 2 */
#define CONFIG_SENSOR_IIC_ADDR_12 	   0x78
#define CONFIG_SENSOR_IIC_ADAPTER_ID_12    3
#define CONFIG_SENSOR_CIF_INDEX_12				  0
#define CONFIG_SENSOR_ORIENTATION_12       270
#define CONFIG_SENSOR_POWER_PIN_12         INVALID_GPIO
#define CONFIG_SENSOR_RESET_PIN_12         INVALID_GPIO
#define CONFIG_SENSOR_POWERDN_PIN_12       RK30_PIN1_PB7
#define CONFIG_SENSOR_FALSH_PIN_12         INVALID_GPIO
#define CONFIG_SENSOR_POWERACTIVE_LEVEL_12 RK29_CAM_POWERACTIVE_L
#define CONFIG_SENSOR_RESETACTIVE_LEVEL_12 RK29_CAM_RESETACTIVE_L
#define CONFIG_SENSOR_POWERDNACTIVE_LEVEL_12 RK29_CAM_POWERDNACTIVE_H
#define CONFIG_SENSOR_FLASHACTIVE_LEVEL_12 RK29_CAM_FLASHACTIVE_L

#define CONFIG_SENSOR_QCIF_FPS_FIXED_12      15000
#define CONFIG_SENSOR_240X160_FPS_FIXED_12   15000
#define CONFIG_SENSOR_QVGA_FPS_FIXED_12      15000
#define CONFIG_SENSOR_CIF_FPS_FIXED_12       15000
#define CONFIG_SENSOR_VGA_FPS_FIXED_12       15000
#define CONFIG_SENSOR_480P_FPS_FIXED_12      15000
#define CONFIG_SENSOR_SVGA_FPS_FIXED_12      15000
#define CONFIG_SENSOR_720P_FPS_FIXED_12      30000
#define CONFIG_SENSOR_13 RK29_CAM_SENSOR_HI253                      /* front camera sensor 3 */
#define CONFIG_SENSOR_IIC_ADDR_13 	   0x40
#define CONFIG_SENSOR_IIC_ADAPTER_ID_13    3
#define CONFIG_SENSOR_CIF_INDEX_13				  0
#define CONFIG_SENSOR_ORIENTATION_13       270
#define CONFIG_SENSOR_POWER_PIN_13         INVALID_GPIO
#define CONFIG_SENSOR_RESET_PIN_13         INVALID_GPIO
#define CONFIG_SENSOR_POWERDN_PIN_13       RK30_PIN1_PB7
#define CONFIG_SENSOR_FALSH_PIN_13         INVALID_GPIO
#define CONFIG_SENSOR_POWERACTIVE_LEVEL_13 RK29_CAM_POWERACTIVE_L
#define CONFIG_SENSOR_RESETACTIVE_LEVEL_13 RK29_CAM_RESETACTIVE_L
#define CONFIG_SENSOR_POWERDNACTIVE_LEVEL_13 RK29_CAM_POWERDNACTIVE_H
#define CONFIG_SENSOR_FLASHACTIVE_LEVEL_13 RK29_CAM_FLASHACTIVE_L

#define CONFIG_SENSOR_QCIF_FPS_FIXED_13      15000
#define CONFIG_SENSOR_240X160_FPS_FIXED_13   15000
#define CONFIG_SENSOR_QVGA_FPS_FIXED_13      15000
#define CONFIG_SENSOR_CIF_FPS_FIXED_13       15000
#define CONFIG_SENSOR_VGA_FPS_FIXED_13       15000
#define CONFIG_SENSOR_480P_FPS_FIXED_13      15000
#define CONFIG_SENSOR_SVGA_FPS_FIXED_13      15000
#define CONFIG_SENSOR_720P_FPS_FIXED_13      30000

#define CONFIG_SENSOR_14 RK29_CAM_SENSOR_GT2005                      /* front camera sensor 4 */
#define CONFIG_SENSOR_IIC_ADDR_14 	   0x78
#define CONFIG_SENSOR_IIC_ADAPTER_ID_14    3
#define CONFIG_SENSOR_CIF_INDEX_14				  0
#define CONFIG_SENSOR_ORIENTATION_14       270
#define CONFIG_SENSOR_POWER_PIN_14         INVALID_GPIO
#define CONFIG_SENSOR_RESET_PIN_14         INVALID_GPIO
#define CONFIG_SENSOR_POWERDN_PIN_14       RK30_PIN1_PB7
#define CONFIG_SENSOR_FALSH_PIN_14         INVALID_GPIO
#define CONFIG_SENSOR_POWERACTIVE_LEVEL_14 RK29_CAM_POWERACTIVE_L
#define CONFIG_SENSOR_RESETACTIVE_LEVEL_14 RK29_CAM_RESETACTIVE_L
#define CONFIG_SENSOR_POWERDNACTIVE_LEVEL_14 RK29_CAM_POWERDNACTIVE_L
#define CONFIG_SENSOR_FLASHACTIVE_LEVEL_14 RK29_CAM_FLASHACTIVE_L

#define CONFIG_SENSOR_QCIF_FPS_FIXED_14      15000
#define CONFIG_SENSOR_240X160_FPS_FIXED_14   15000
#define CONFIG_SENSOR_QVGA_FPS_FIXED_14      15000
#define CONFIG_SENSOR_CIF_FPS_FIXED_14       15000
#define CONFIG_SENSOR_VGA_FPS_FIXED_14       15000
#define CONFIG_SENSOR_480P_FPS_FIXED_14      15000
#define CONFIG_SENSOR_SVGA_FPS_FIXED_14      15000
#define CONFIG_SENSOR_720P_FPS_FIXED_14      30000

#define CONFIG_SENSOR_15 RK29_CAM_SENSOR_HI704                      /* front camera sensor 5 */
#define CONFIG_SENSOR_IIC_ADDR_15 	   0x60
#define CONFIG_SENSOR_IIC_ADAPTER_ID_15    3
#define CONFIG_SENSOR_CIF_INDEX_15				  0
#define CONFIG_SENSOR_ORIENTATION_15       270
#define CONFIG_SENSOR_POWER_PIN_15         INVALID_GPIO
#define CONFIG_SENSOR_RESET_PIN_15         INVALID_GPIO
#define CONFIG_SENSOR_POWERDN_PIN_15       RK30_PIN1_PB7
#define CONFIG_SENSOR_FALSH_PIN_15         INVALID_GPIO
#define CONFIG_SENSOR_POWERACTIVE_LEVEL_15 RK29_CAM_POWERACTIVE_L
#define CONFIG_SENSOR_RESETACTIVE_LEVEL_15 RK29_CAM_RESETACTIVE_L
#define CONFIG_SENSOR_POWERDNACTIVE_LEVEL_15 RK29_CAM_POWERDNACTIVE_H
#define CONFIG_SENSOR_FLASHACTIVE_LEVEL_15 RK29_CAM_FLASHACTIVE_L

#define CONFIG_SENSOR_QCIF_FPS_FIXED_15      15000
#define CONFIG_SENSOR_240X160_FPS_FIXED_15   15000
#define CONFIG_SENSOR_QVGA_FPS_FIXED_15      15000
#define CONFIG_SENSOR_CIF_FPS_FIXED_15       15000
#define CONFIG_SENSOR_VGA_FPS_FIXED_15       15000
#define CONFIG_SENSOR_480P_FPS_FIXED_15      15000
#define CONFIG_SENSOR_SVGA_FPS_FIXED_15      15000
#define CONFIG_SENSOR_720P_FPS_FIXED_15      30000

#define CONFIG_SENSOR_16 RK29_CAM_SENSOR_OV2655                      /* front camera sensor 6 */
#define CONFIG_SENSOR_IIC_ADDR_16 	   0x60
#define CONFIG_SENSOR_IIC_ADAPTER_ID_16    3
#define CONFIG_SENSOR_CIF_INDEX_16				  0
#define CONFIG_SENSOR_ORIENTATION_16       270
#define CONFIG_SENSOR_POWER_PIN_16         INVALID_GPIO
#define CONFIG_SENSOR_RESET_PIN_16         INVALID_GPIO
#define CONFIG_SENSOR_POWERDN_PIN_16       RK30_PIN1_PB7
#define CONFIG_SENSOR_FALSH_PIN_16         INVALID_GPIO
#define CONFIG_SENSOR_POWERACTIVE_LEVEL_16 RK29_CAM_POWERACTIVE_L
#define CONFIG_SENSOR_RESETACTIVE_LEVEL_16 RK29_CAM_RESETACTIVE_L
#define CONFIG_SENSOR_POWERDNACTIVE_LEVEL_16 RK29_CAM_POWERDNACTIVE_H
#define CONFIG_SENSOR_FLASHACTIVE_LEVEL_16 RK29_CAM_FLASHACTIVE_L

#define CONFIG_SENSOR_QCIF_FPS_FIXED_16      15000
#define CONFIG_SENSOR_240X160_FPS_FIXED_16   15000
#define CONFIG_SENSOR_QVGA_FPS_FIXED_16      15000
#define CONFIG_SENSOR_CIF_FPS_FIXED_16       15000
#define CONFIG_SENSOR_VGA_FPS_FIXED_16       15000
#define CONFIG_SENSOR_480P_FPS_FIXED_16      15000
#define CONFIG_SENSOR_SVGA_FPS_FIXED_16      15000
#define CONFIG_SENSOR_720P_FPS_FIXED_16      30000

#define CONFIG_SENSOR_17 RK29_CAM_SENSOR_SP2518                      /* front camera sensor 7 */
#define CONFIG_SENSOR_IIC_ADDR_17 	   0x60
#define CONFIG_SENSOR_IIC_ADAPTER_ID_17    3
#define CONFIG_SENSOR_CIF_INDEX_17				  0
#define CONFIG_SENSOR_ORIENTATION_17       270
#define CONFIG_SENSOR_POWER_PIN_17         INVALID_GPIO
#define CONFIG_SENSOR_RESET_PIN_17         INVALID_GPIO
#define CONFIG_SENSOR_POWERDN_PIN_17       RK30_PIN1_PB7
#define CONFIG_SENSOR_FALSH_PIN_17         INVALID_GPIO
#define CONFIG_SENSOR_POWERACTIVE_LEVEL_17 RK29_CAM_POWERACTIVE_L
#define CONFIG_SENSOR_RESETACTIVE_LEVEL_17 RK29_CAM_RESETACTIVE_L
#define CONFIG_SENSOR_POWERDNACTIVE_LEVEL_17 RK29_CAM_POWERDNACTIVE_H
#define CONFIG_SENSOR_FLASHACTIVE_LEVEL_17 RK29_CAM_FLASHACTIVE_L

#define CONFIG_SENSOR_QCIF_FPS_FIXED_17      15000
#define CONFIG_SENSOR_240X160_FPS_FIXED_17   15000
#define CONFIG_SENSOR_QVGA_FPS_FIXED_17      15000
#define CONFIG_SENSOR_CIF_FPS_FIXED_17       15000
#define CONFIG_SENSOR_VGA_FPS_FIXED_17       15000
#define CONFIG_SENSOR_480P_FPS_FIXED_17      15000
#define CONFIG_SENSOR_SVGA_FPS_FIXED_17      15000
#define CONFIG_SENSOR_720P_FPS_FIXED_17      30000

#define CONFIG_SENSOR_18 RK29_CAM_SENSOR_SP2518                      /* front camera sensor 8 */
#define CONFIG_SENSOR_IIC_ADDR_18 	   0x00
#define CONFIG_SENSOR_IIC_ADAPTER_ID_18    3
#define CONFIG_SENSOR_CIF_INDEX_18				  0
#define CONFIG_SENSOR_ORIENTATION_18       270
#define CONFIG_SENSOR_POWER_PIN_18         INVALID_GPIO
#define CONFIG_SENSOR_RESET_PIN_18         INVALID_GPIO
#define CONFIG_SENSOR_POWERDN_PIN_18       RK30_PIN1_PB7
#define CONFIG_SENSOR_FALSH_PIN_18         INVALID_GPIO
#define CONFIG_SENSOR_POWERACTIVE_LEVEL_18 RK29_CAM_POWERACTIVE_L
#define CONFIG_SENSOR_RESETACTIVE_LEVEL_18 RK29_CAM_RESETACTIVE_L
#define CONFIG_SENSOR_POWERDNACTIVE_LEVEL_18 RK29_CAM_POWERDNACTIVE_H
#define CONFIG_SENSOR_FLASHACTIVE_LEVEL_18 RK29_CAM_FLASHACTIVE_L

#define CONFIG_SENSOR_QCIF_FPS_FIXED_18      15000
#define CONFIG_SENSOR_240X160_FPS_FIXED_18   15000
#define CONFIG_SENSOR_QVGA_FPS_FIXED_18      15000
#define CONFIG_SENSOR_CIF_FPS_FIXED_18       15000
#define CONFIG_SENSOR_VGA_FPS_FIXED_18       15000
#define CONFIG_SENSOR_480P_FPS_FIXED_18      15000
#define CONFIG_SENSOR_SVGA_FPS_FIXED_18      15000
#define CONFIG_SENSOR_720P_FPS_FIXED_18      30000

#define CONFIG_SENSOR_19 RK29_CAM_SENSOR_SP2518                      /* front camera sensor 9 */
#define CONFIG_SENSOR_IIC_ADDR_19 	   0x00
#define CONFIG_SENSOR_IIC_ADAPTER_ID_19    3
#define CONFIG_SENSOR_CIF_INDEX_19				  0
#define CONFIG_SENSOR_ORIENTATION_19       270
#define CONFIG_SENSOR_POWER_PIN_19         INVALID_GPIO
#define CONFIG_SENSOR_RESET_PIN_19         INVALID_GPIO
#define CONFIG_SENSOR_POWERDN_PIN_19       RK30_PIN1_PB7
#define CONFIG_SENSOR_FALSH_PIN_19         INVALID_GPIO
#define CONFIG_SENSOR_POWERACTIVE_LEVEL_19 RK29_CAM_POWERACTIVE_L
#define CONFIG_SENSOR_RESETACTIVE_LEVEL_19 RK29_CAM_RESETACTIVE_L
#define CONFIG_SENSOR_POWERDNACTIVE_LEVEL_19 RK29_CAM_POWERDNACTIVE_H
#define CONFIG_SENSOR_FLASHACTIVE_LEVEL_19 RK29_CAM_FLASHACTIVE_L

#define CONFIG_SENSOR_QCIF_FPS_FIXED_19      15000
#define CONFIG_SENSOR_240X160_FPS_FIXED_19   15000
#define CONFIG_SENSOR_QVGA_FPS_FIXED_19      15000
#define CONFIG_SENSOR_CIF_FPS_FIXED_19       15000
#define CONFIG_SENSOR_VGA_FPS_FIXED_19       15000
#define CONFIG_SENSOR_480P_FPS_FIXED_19      15000
#define CONFIG_SENSOR_SVGA_FPS_FIXED_19      15000
#define CONFIG_SENSOR_720P_FPS_FIXED_19      30000


#endif  //#ifdef CONFIG_VIDEO_RK29
/*---------------- Camera Sensor Configuration Macro End------------------------*/
#include "../../../drivers/media/video/rk30_camera.c"
/*---------------- Camera Sensor Macro Define End  ---------*/

#define PMEM_CAM_SIZE PMEM_CAM_NECESSARY
/*****************************************************************************************
 * camera  devices
 * author: ddl@rock-chips.com
 *****************************************************************************************/
#ifdef CONFIG_VIDEO_RK29
#define CONFIG_SENSOR_POWER_IOCTL_USR	   1 //define this refer to your board layout
#define CONFIG_SENSOR_RESET_IOCTL_USR	   0
#define CONFIG_SENSOR_POWERDOWN_IOCTL_USR	   0
#define CONFIG_SENSOR_FLASH_IOCTL_USR	   0

static void rk_cif_power(int on)
{
#ifdef  CONFIG_KP_AXP20
	//gpio_direction_output(RK30_PIN6_PB0, on ? GPIO_HIGH : GPIO_LOW);
#else
    struct regulator *ldo_18,*ldo_28;
#if defined (CONFIG_MFD_WM831X)
	ldo_28 = regulator_get(NULL, "ldo7");	// vcc28_cif
	ldo_18 = regulator_get(NULL, "ldo1");	// vcc18_cif
#endif
#if defined (CONFIG_MFD_TPS65910)
	ldo_28 = regulator_get(NULL, "vmmc");	// vcc28_cif
	ldo_18 = regulator_get(NULL, "vdig1");	// vcc18_cif
#endif
	if (ldo_28 == NULL || IS_ERR(ldo_28) || ldo_18 == NULL || IS_ERR(ldo_18)){
        printk("get cif ldo failed!\n");
		return;
	    }
    if(on == 0){	
    	regulator_disable(ldo_28);
    	regulator_put(ldo_28);
    	regulator_disable(ldo_18);
    	regulator_put(ldo_18);
    	mdelay(500);
        }
    else{
    	regulator_set_voltage(ldo_28, 2800000, 2800000);
    	regulator_enable(ldo_28);
   // 	printk("%s set ldo7 vcc28_cif=%dmV end\n", __func__, regulator_get_voltage(ldo_28));
    	regulator_put(ldo_28);

    	regulator_set_voltage(ldo_18, 1800000, 1800000);
    //	regulator_set_suspend_voltage(ldo, 1800000);
    	regulator_enable(ldo_18);
    //	printk("%s set ldo1 vcc18_cif=%dmV end\n", __func__, regulator_get_voltage(ldo_18));
    	regulator_put(ldo_18);
        }
#endif
}

#if CONFIG_SENSOR_POWER_IOCTL_USR
static int sensor_power_usr_cb (struct rk29camera_gpio_res *res,int on)
{
	//#error "CONFIG_SENSOR_POWER_IOCTL_USR is 1, sensor_power_usr_cb function must be writed!!";
    rk_cif_power(on);
}
#endif

#if CONFIG_SENSOR_RESET_IOCTL_USR
static int sensor_reset_usr_cb (struct rk29camera_gpio_res *res,int on)
{
	#error "CONFIG_SENSOR_RESET_IOCTL_USR is 1, sensor_reset_usr_cb function must be writed!!";
}
#endif

#if CONFIG_SENSOR_POWERDOWN_IOCTL_USR
static int sensor_powerdown_usr_cb (struct rk29camera_gpio_res *res,int on)
{
	#error "CONFIG_SENSOR_POWERDOWN_IOCTL_USR is 1, sensor_powerdown_usr_cb function must be writed!!";
}
#endif

#if CONFIG_SENSOR_FLASH_IOCTL_USR
static int sensor_flash_usr_cb (struct rk29camera_gpio_res *res,int on)
{
	#error "CONFIG_SENSOR_FLASH_IOCTL_USR is 1, sensor_flash_usr_cb function must be writed!!";
}
#endif

static struct rk29camera_platform_ioctl_cb	sensor_ioctl_cb = {
	#if CONFIG_SENSOR_POWER_IOCTL_USR
	.sensor_power_cb = sensor_power_usr_cb,
	#else
	.sensor_power_cb = NULL,
	#endif

	#if CONFIG_SENSOR_RESET_IOCTL_USR
	.sensor_reset_cb = sensor_reset_usr_cb,
	#else
	.sensor_reset_cb = NULL,
	#endif

	#if CONFIG_SENSOR_POWERDOWN_IOCTL_USR
	.sensor_powerdown_cb = sensor_powerdown_usr_cb,
	#else
	.sensor_powerdown_cb = NULL,
	#endif

	#if CONFIG_SENSOR_FLASH_IOCTL_USR
	.sensor_flash_cb = sensor_flash_usr_cb,
	#else
	.sensor_flash_cb = NULL,
	#endif
};

#if CONFIG_SENSOR_IIC_ADDR_0
static struct reginfo_t rk_init_data_sensor_reg_0[] =
{
		{0x0000, 0x00,0,0}
	};
static struct reginfo_t rk_init_data_sensor_winseqreg_0[] ={
	{0x0000, 0x00,0,0}
	};
#endif

#if CONFIG_SENSOR_IIC_ADDR_1
static struct reginfo_t rk_init_data_sensor_reg_1[] =
{
    {0x0000, 0x00,0,0}
};
static struct reginfo_t rk_init_data_sensor_winseqreg_1[] =
{
       {0x0000, 0x00,0,0}
};
#endif
#if CONFIG_SENSOR_IIC_ADDR_01
static struct reginfo_t rk_init_data_sensor_reg_01[] =
{
    {0x0000, 0x00,0,0}
};
static struct reginfo_t rk_init_data_sensor_winseqreg_01[] =
{
       {0x0000, 0x00,0,0}
};
#endif
#if CONFIG_SENSOR_IIC_ADDR_02
static struct reginfo_t rk_init_data_sensor_reg_02[] =
{
    {0x0000, 0x00,0,0}
};
static struct reginfo_t rk_init_data_sensor_winseqreg_02[] =
{
       {0x0000, 0x00,0,0}
};
#endif
#if CONFIG_SENSOR_IIC_ADDR_03
static struct reginfo_t rk_init_data_sensor_reg_03[] =
{
    {0x0000, 0x00,0,0}
};
static struct reginfo_t rk_init_data_sensor_winseqreg_03[] =
{
       {0x0000, 0x00,0,0}
};
#endif
#if CONFIG_SENSOR_IIC_ADDR_04
static struct reginfo_t rk_init_data_sensor_reg_04[] =
{
    {0x0000, 0x00,0,0}
};
static struct reginfo_t rk_init_data_sensor_winseqreg_04[] =
{
       {0x0000, 0x00,0,0}
};
#endif
#if CONFIG_SENSOR_IIC_ADDR_05
static struct reginfo_t rk_init_data_sensor_reg_05[] =
{
    {0x0000, 0x00,0,0}
};
static struct reginfo_t rk_init_data_sensor_winseqreg_05[] =
{
       {0x0000, 0x00,0,0}
};
#endif
#if CONFIG_SENSOR_IIC_ADDR_06
static struct reginfo_t rk_init_data_sensor_reg_06[] =
{
    {0x0000, 0x00,0,0}
};
static struct reginfo_t rk_init_data_sensor_winseqreg_06[] =
{
       {0x0000, 0x00,0,0}
};
#endif
#if CONFIG_SENSOR_IIC_ADDR_07
static struct reginfo_t rk_init_data_sensor_reg_07[] =
{
    {0x0000, 0x00,0,0}
};
static struct reginfo_t rk_init_data_sensor_winseqreg_07[] =
{
       {0x0000, 0x00,0,0}
};
#endif
#if CONFIG_SENSOR_IIC_ADDR_08
static struct reginfo_t rk_init_data_sensor_reg_08[] =
{
    {0x0000, 0x00,0,0}
};
static struct reginfo_t rk_init_data_sensor_winseqreg_08[] =
{
       {0x0000, 0x00,0,0}
};
#endif
#if CONFIG_SENSOR_IIC_ADDR_09
static struct reginfo_t rk_init_data_sensor_reg_09[] =
{
    {0x0000, 0x00,0,0}
};
static struct reginfo_t rk_init_data_sensor_winseqreg_09[] =
{
       {0x0000, 0x00,0,0}
};
#endif
#if CONFIG_SENSOR_IIC_ADDR_11
static struct reginfo_t rk_init_data_sensor_reg_11[] =
{
    {0x0000, 0x00,0,0}
};
static struct reginfo_t rk_init_data_sensor_winseqreg_11[] =
{
       {0x0000, 0x00,0,0}
};
#endif
#if CONFIG_SENSOR_IIC_ADDR_12
static struct reginfo_t rk_init_data_sensor_reg_12[] =
{
    {0x0000, 0x00,0,0}
};
static struct reginfo_t rk_init_data_sensor_winseqreg_12[] =
{
       {0x0000, 0x00,0,0}
};
#endif
#if CONFIG_SENSOR_IIC_ADDR_13
static struct reginfo_t rk_init_data_sensor_reg_13[] =
{
    {0x0000, 0x00,0,0}
};
static struct reginfo_t rk_init_data_sensor_winseqreg_13[] =
{
       {0x0000, 0x00,0,0}
};
#endif
#if CONFIG_SENSOR_IIC_ADDR_14
static struct reginfo_t rk_init_data_sensor_reg_14[] =
{
    {0x0000, 0x00,0,0}
};
static struct reginfo_t rk_init_data_sensor_winseqreg_14[] =
{
       {0x0000, 0x00,0,0}
};
#endif
#if CONFIG_SENSOR_IIC_ADDR_15
static struct reginfo_t rk_init_data_sensor_reg_15[] =
{
    {0x0000, 0x00,0,0}
};
static struct reginfo_t rk_init_data_sensor_winseqreg_15[] =
{
       {0x0000, 0x00,0,0}
};
#endif
#if CONFIG_SENSOR_IIC_ADDR_16
static struct reginfo_t rk_init_data_sensor_reg_16[] =
{
    {0x0000, 0x00,0,0}
};
static struct reginfo_t rk_init_data_sensor_winseqreg_16[] =
{
       {0x0000, 0x00,0,0}
};
#endif
#if CONFIG_SENSOR_IIC_ADDR_17
static struct reginfo_t rk_init_data_sensor_reg_17[] =
{
    {0x0000, 0x00,0,0}
};
static struct reginfo_t rk_init_data_sensor_winseqreg_17[] =
{
       {0x0000, 0x00,0,0}
};
#endif
#if CONFIG_SENSOR_IIC_ADDR_18
static struct reginfo_t rk_init_data_sensor_reg_18[] =
{
    {0x0000, 0x00,0,0}
};
static struct reginfo_t rk_init_data_sensor_winseqreg_18[] =
{
       {0x0000, 0x00,0,0}
};
#endif
#if CONFIG_SENSOR_IIC_ADDR_19
static struct reginfo_t rk_init_data_sensor_reg_19[] =
{
    {0x0000, 0x00,0,0}
};
static struct reginfo_t rk_init_data_sensor_winseqreg_19[] =
{
       {0x0000, 0x00,0,0}
};
#endif
static rk_sensor_user_init_data_s rk_init_data_sensor[RK_CAM_NUM] = 
{
    #if CONFIG_SENSOR_IIC_ADDR_0
    {
       .rk_sensor_init_width = INVALID_VALUE,
       .rk_sensor_init_height = INVALID_VALUE,
       .rk_sensor_init_bus_param = INVALID_VALUE,
       .rk_sensor_init_pixelcode = INVALID_VALUE,
       .rk_sensor_init_data = rk_init_data_sensor_reg_0,
       .rk_sensor_init_winseq = rk_init_data_sensor_winseqreg_0,
       .rk_sensor_winseq_size = sizeof(rk_init_data_sensor_winseqreg_0) / sizeof(struct reginfo_t),
       .rk_sensor_init_data_size = sizeof(rk_init_data_sensor_reg_0) / sizeof(struct reginfo_t),
    },
    #else
    {
       .rk_sensor_init_width = INVALID_VALUE,
       .rk_sensor_init_height = INVALID_VALUE,
       .rk_sensor_init_bus_param = INVALID_VALUE,
       .rk_sensor_init_pixelcode = INVALID_VALUE,
       .rk_sensor_init_data = NULL,
       .rk_sensor_init_winseq = NULL,
       .rk_sensor_winseq_size = 0,
       .rk_sensor_init_data_size = 0,
    },
    #endif
    #if CONFIG_SENSOR_IIC_ADDR_1
    {
       .rk_sensor_init_width = INVALID_VALUE,
       .rk_sensor_init_height = INVALID_VALUE,
       .rk_sensor_init_bus_param = INVALID_VALUE,
       .rk_sensor_init_pixelcode = INVALID_VALUE,
       .rk_sensor_init_data = rk_init_data_sensor_reg_1,
       .rk_sensor_init_winseq = rk_init_data_sensor_winseqreg_1,
       .rk_sensor_winseq_size = sizeof(rk_init_data_sensor_winseqreg_1) / sizeof(struct reginfo_t),
       .rk_sensor_init_data_size = sizeof(rk_init_data_sensor_reg_1) / sizeof(struct reginfo_t),
    },
    #else
    {
       .rk_sensor_init_width = INVALID_VALUE,
       .rk_sensor_init_height = INVALID_VALUE,
       .rk_sensor_init_bus_param = INVALID_VALUE,
       .rk_sensor_init_pixelcode = INVALID_VALUE,
       .rk_sensor_init_data = NULL,
       .rk_sensor_init_winseq = NULL,
       .rk_sensor_winseq_size = 0,
       .rk_sensor_init_data_size = 0,
    },
    #endif
    #if CONFIG_SENSOR_IIC_ADDR_01
    {
       .rk_sensor_init_width = INVALID_VALUE,
       .rk_sensor_init_height = INVALID_VALUE,
       .rk_sensor_init_bus_param = INVALID_VALUE,
       .rk_sensor_init_pixelcode = INVALID_VALUE,
       .rk_sensor_init_data = rk_init_data_sensor_reg_01,
       .rk_sensor_init_winseq = rk_init_data_sensor_winseqreg_01,
       .rk_sensor_winseq_size = sizeof(rk_init_data_sensor_winseqreg_01) / sizeof(struct reginfo_t),
       .rk_sensor_init_data_size = sizeof(rk_init_data_sensor_reg_01) / sizeof(struct reginfo_t),
    },
    #else
    {
       .rk_sensor_init_width = INVALID_VALUE,
       .rk_sensor_init_height = INVALID_VALUE,
       .rk_sensor_init_bus_param = INVALID_VALUE,
       .rk_sensor_init_pixelcode = INVALID_VALUE,
       .rk_sensor_init_data = NULL,
       .rk_sensor_init_winseq = NULL,
       .rk_sensor_winseq_size = 0,
       .rk_sensor_init_data_size = 0,
    },
    #endif
    #if CONFIG_SENSOR_IIC_ADDR_02
    {
       .rk_sensor_init_width = INVALID_VALUE,
       .rk_sensor_init_height = INVALID_VALUE,
       .rk_sensor_init_bus_param = INVALID_VALUE,
       .rk_sensor_init_pixelcode = INVALID_VALUE,
       .rk_sensor_init_data = rk_init_data_sensor_reg_02,
       .rk_sensor_init_winseq = rk_init_data_sensor_winseqreg_02,
       .rk_sensor_winseq_size = sizeof(rk_init_data_sensor_winseqreg_02) / sizeof(struct reginfo_t),
       .rk_sensor_init_data_size = sizeof(rk_init_data_sensor_reg_02) / sizeof(struct reginfo_t),
    },
    #else
    {
       .rk_sensor_init_width = INVALID_VALUE,
       .rk_sensor_init_height = INVALID_VALUE,
       .rk_sensor_init_bus_param = INVALID_VALUE,
       .rk_sensor_init_pixelcode = INVALID_VALUE,
       .rk_sensor_init_data = NULL,
       .rk_sensor_init_winseq = NULL,
       .rk_sensor_winseq_size = 0,
       .rk_sensor_init_data_size = 0,
    },
    #endif
    #if CONFIG_SENSOR_IIC_ADDR_03
    {
       .rk_sensor_init_width = INVALID_VALUE,
       .rk_sensor_init_height = INVALID_VALUE,
       .rk_sensor_init_bus_param = INVALID_VALUE,
       .rk_sensor_init_pixelcode = INVALID_VALUE,
       .rk_sensor_init_data = rk_init_data_sensor_reg_03,
       .rk_sensor_init_winseq = rk_init_data_sensor_winseqreg_03,
       .rk_sensor_winseq_size = sizeof(rk_init_data_sensor_winseqreg_03) / sizeof(struct reginfo_t),
       .rk_sensor_init_data_size = sizeof(rk_init_data_sensor_reg_03) / sizeof(struct reginfo_t),
    },
    #else
    {
       .rk_sensor_init_width = INVALID_VALUE,
       .rk_sensor_init_height = INVALID_VALUE,
       .rk_sensor_init_bus_param = INVALID_VALUE,
       .rk_sensor_init_pixelcode = INVALID_VALUE,
       .rk_sensor_init_data = NULL,
       .rk_sensor_init_winseq = NULL,
       .rk_sensor_winseq_size = 0,
       .rk_sensor_init_data_size = 0,
    },
    #endif
    #if CONFIG_SENSOR_IIC_ADDR_04
    {
       .rk_sensor_init_width = INVALID_VALUE,
       .rk_sensor_init_height = INVALID_VALUE,
       .rk_sensor_init_bus_param = INVALID_VALUE,
       .rk_sensor_init_pixelcode = INVALID_VALUE,
       .rk_sensor_init_data = rk_init_data_sensor_reg_04,
       .rk_sensor_init_winseq = rk_init_data_sensor_winseqreg_04,
       .rk_sensor_winseq_size = sizeof(rk_init_data_sensor_winseqreg_04) / sizeof(struct reginfo_t),
       .rk_sensor_init_data_size = sizeof(rk_init_data_sensor_reg_04) / sizeof(struct reginfo_t),
    },
    #else
    {
       .rk_sensor_init_width = INVALID_VALUE,
       .rk_sensor_init_height = INVALID_VALUE,
       .rk_sensor_init_bus_param = INVALID_VALUE,
       .rk_sensor_init_pixelcode = INVALID_VALUE,
       .rk_sensor_init_data = NULL,
       .rk_sensor_init_winseq = NULL,
       .rk_sensor_winseq_size = 0,
       .rk_sensor_init_data_size = 0,
    },
    #endif
    #if CONFIG_SENSOR_IIC_ADDR_05
    {
       .rk_sensor_init_width = INVALID_VALUE,
       .rk_sensor_init_height = INVALID_VALUE,
       .rk_sensor_init_bus_param = INVALID_VALUE,
       .rk_sensor_init_pixelcode = INVALID_VALUE,
       .rk_sensor_init_data = rk_init_data_sensor_reg_05,
       .rk_sensor_init_winseq = rk_init_data_sensor_winseqreg_05,
       .rk_sensor_winseq_size = sizeof(rk_init_data_sensor_winseqreg_05) / sizeof(struct reginfo_t),
       .rk_sensor_init_data_size = sizeof(rk_init_data_sensor_reg_05) / sizeof(struct reginfo_t),
    },
    #else
    {
       .rk_sensor_init_width = INVALID_VALUE,
       .rk_sensor_init_height = INVALID_VALUE,
       .rk_sensor_init_bus_param = INVALID_VALUE,
       .rk_sensor_init_pixelcode = INVALID_VALUE,
       .rk_sensor_init_data = NULL,
       .rk_sensor_init_winseq = NULL,
       .rk_sensor_winseq_size = 0,
       .rk_sensor_init_data_size = 0,
    },
    #endif
    #if CONFIG_SENSOR_IIC_ADDR_06
    {
       .rk_sensor_init_width = INVALID_VALUE,
       .rk_sensor_init_height = INVALID_VALUE,
       .rk_sensor_init_bus_param = INVALID_VALUE,
       .rk_sensor_init_pixelcode = INVALID_VALUE,
       .rk_sensor_init_data = rk_init_data_sensor_reg_06,
       .rk_sensor_init_winseq = rk_init_data_sensor_winseqreg_06,
       .rk_sensor_winseq_size = sizeof(rk_init_data_sensor_winseqreg_06) / sizeof(struct reginfo_t),
       .rk_sensor_init_data_size = sizeof(rk_init_data_sensor_reg_06) / sizeof(struct reginfo_t),
    },
    #else
    {
       .rk_sensor_init_width = INVALID_VALUE,
       .rk_sensor_init_height = INVALID_VALUE,
       .rk_sensor_init_bus_param = INVALID_VALUE,
       .rk_sensor_init_pixelcode = INVALID_VALUE,
       .rk_sensor_init_data = NULL,
       .rk_sensor_init_winseq = NULL,
       .rk_sensor_winseq_size = 0,
       .rk_sensor_init_data_size = 0,
    },
    #endif
    #if CONFIG_SENSOR_IIC_ADDR_07
    {
       .rk_sensor_init_width = INVALID_VALUE,
       .rk_sensor_init_height = INVALID_VALUE,
       .rk_sensor_init_bus_param = INVALID_VALUE,
       .rk_sensor_init_pixelcode = INVALID_VALUE,
       .rk_sensor_init_data = rk_init_data_sensor_reg_07,
       .rk_sensor_init_winseq = rk_init_data_sensor_winseqreg_07,
       .rk_sensor_winseq_size = sizeof(rk_init_data_sensor_winseqreg_07) / sizeof(struct reginfo_t),
       .rk_sensor_init_data_size = sizeof(rk_init_data_sensor_reg_07) / sizeof(struct reginfo_t),
    },
    #else
    {
       .rk_sensor_init_width = INVALID_VALUE,
       .rk_sensor_init_height = INVALID_VALUE,
       .rk_sensor_init_bus_param = INVALID_VALUE,
       .rk_sensor_init_pixelcode = INVALID_VALUE,
       .rk_sensor_init_data = NULL,
       .rk_sensor_init_winseq = NULL,
       .rk_sensor_winseq_size = 0,
       .rk_sensor_init_data_size = 0,
    },
    #endif
    #if CONFIG_SENSOR_IIC_ADDR_08
    {
       .rk_sensor_init_width = INVALID_VALUE,
       .rk_sensor_init_height = INVALID_VALUE,
       .rk_sensor_init_bus_param = INVALID_VALUE,
       .rk_sensor_init_pixelcode = INVALID_VALUE,
       .rk_sensor_init_data = rk_init_data_sensor_reg_08,
       .rk_sensor_init_winseq = rk_init_data_sensor_winseqreg_08,
       .rk_sensor_winseq_size = sizeof(rk_init_data_sensor_winseqreg_08) / sizeof(struct reginfo_t),
       .rk_sensor_init_data_size = sizeof(rk_init_data_sensor_reg_08) / sizeof(struct reginfo_t),
    },
    #else
    {
       .rk_sensor_init_width = INVALID_VALUE,
       .rk_sensor_init_height = INVALID_VALUE,
       .rk_sensor_init_bus_param = INVALID_VALUE,
       .rk_sensor_init_pixelcode = INVALID_VALUE,
       .rk_sensor_init_data = NULL,
       .rk_sensor_init_winseq = NULL,
       .rk_sensor_winseq_size = 0,
       .rk_sensor_init_data_size = 0,
    },
    #endif
    #if CONFIG_SENSOR_IIC_ADDR_09
    {
       .rk_sensor_init_width = INVALID_VALUE,
       .rk_sensor_init_height = INVALID_VALUE,
       .rk_sensor_init_bus_param = INVALID_VALUE,
       .rk_sensor_init_pixelcode = INVALID_VALUE,
       .rk_sensor_init_data = rk_init_data_sensor_reg_09,
       .rk_sensor_init_winseq = rk_init_data_sensor_winseqreg_09,
       .rk_sensor_winseq_size = sizeof(rk_init_data_sensor_winseqreg_09) / sizeof(struct reginfo_t),
       .rk_sensor_init_data_size = sizeof(rk_init_data_sensor_reg_09) / sizeof(struct reginfo_t),
    },
    #else
    {
       .rk_sensor_init_width = INVALID_VALUE,
       .rk_sensor_init_height = INVALID_VALUE,
       .rk_sensor_init_bus_param = INVALID_VALUE,
       .rk_sensor_init_pixelcode = INVALID_VALUE,
       .rk_sensor_init_data = NULL,
       .rk_sensor_init_winseq = NULL,
       .rk_sensor_winseq_size = 0,
       .rk_sensor_init_data_size = 0,
    },
    #endif
    #if CONFIG_SENSOR_IIC_ADDR_11
    {
       .rk_sensor_init_width = INVALID_VALUE,
       .rk_sensor_init_height = INVALID_VALUE,
       .rk_sensor_init_bus_param = INVALID_VALUE,
       .rk_sensor_init_pixelcode = INVALID_VALUE,
       .rk_sensor_init_data = rk_init_data_sensor_reg_11,
       .rk_sensor_init_winseq = rk_init_data_sensor_winseqreg_11,
       .rk_sensor_winseq_size = sizeof(rk_init_data_sensor_winseqreg_11) / sizeof(struct reginfo_t),
       .rk_sensor_init_data_size = sizeof(rk_init_data_sensor_reg_11) / sizeof(struct reginfo_t),
    },
    #else
    {
       .rk_sensor_init_width = INVALID_VALUE,
       .rk_sensor_init_height = INVALID_VALUE,
       .rk_sensor_init_bus_param = INVALID_VALUE,
       .rk_sensor_init_pixelcode = INVALID_VALUE,
       .rk_sensor_init_data = NULL,
       .rk_sensor_init_winseq = NULL,
       .rk_sensor_winseq_size = 0,
       .rk_sensor_init_data_size = 0,
    },
    #endif
    #if CONFIG_SENSOR_IIC_ADDR_12
    {
       .rk_sensor_init_width = INVALID_VALUE,
       .rk_sensor_init_height = INVALID_VALUE,
       .rk_sensor_init_bus_param = INVALID_VALUE,
       .rk_sensor_init_pixelcode = INVALID_VALUE,
       .rk_sensor_init_data = rk_init_data_sensor_reg_12,
       .rk_sensor_init_winseq = rk_init_data_sensor_winseqreg_12,
       .rk_sensor_winseq_size = sizeof(rk_init_data_sensor_winseqreg_12) / sizeof(struct reginfo_t),
       .rk_sensor_init_data_size = sizeof(rk_init_data_sensor_reg_12) / sizeof(struct reginfo_t),
    },
    #else
    {
       .rk_sensor_init_width = INVALID_VALUE,
       .rk_sensor_init_height = INVALID_VALUE,
       .rk_sensor_init_bus_param = INVALID_VALUE,
       .rk_sensor_init_pixelcode = INVALID_VALUE,
       .rk_sensor_init_data = NULL,
       .rk_sensor_init_winseq = NULL,
       .rk_sensor_winseq_size = 0,
       .rk_sensor_init_data_size = 0,
    },
    #endif
    #if CONFIG_SENSOR_IIC_ADDR_13
    {
       .rk_sensor_init_width = INVALID_VALUE,
       .rk_sensor_init_height = INVALID_VALUE,
       .rk_sensor_init_bus_param = INVALID_VALUE,
       .rk_sensor_init_pixelcode = INVALID_VALUE,
       .rk_sensor_init_data = rk_init_data_sensor_reg_13,
       .rk_sensor_init_winseq = rk_init_data_sensor_winseqreg_13,
       .rk_sensor_winseq_size = sizeof(rk_init_data_sensor_winseqreg_13) / sizeof(struct reginfo_t),
       .rk_sensor_init_data_size = sizeof(rk_init_data_sensor_reg_13) / sizeof(struct reginfo_t),
    },
    #else
    {
       .rk_sensor_init_width = INVALID_VALUE,
       .rk_sensor_init_height = INVALID_VALUE,
       .rk_sensor_init_bus_param = INVALID_VALUE,
       .rk_sensor_init_pixelcode = INVALID_VALUE,
       .rk_sensor_init_data = NULL,
       .rk_sensor_init_winseq = NULL,
       .rk_sensor_winseq_size = 0,
       .rk_sensor_init_data_size = 0,
    },
    #endif
    #if CONFIG_SENSOR_IIC_ADDR_14
    {
       .rk_sensor_init_width = INVALID_VALUE,
       .rk_sensor_init_height = INVALID_VALUE,
       .rk_sensor_init_bus_param = INVALID_VALUE,
       .rk_sensor_init_pixelcode = INVALID_VALUE,
       .rk_sensor_init_data = rk_init_data_sensor_reg_14,
       .rk_sensor_init_winseq = rk_init_data_sensor_winseqreg_14,
       .rk_sensor_winseq_size = sizeof(rk_init_data_sensor_winseqreg_14) / sizeof(struct reginfo_t),
       .rk_sensor_init_data_size = sizeof(rk_init_data_sensor_reg_14) / sizeof(struct reginfo_t),
    },
    #else
    {
       .rk_sensor_init_width = INVALID_VALUE,
       .rk_sensor_init_height = INVALID_VALUE,
       .rk_sensor_init_bus_param = INVALID_VALUE,
       .rk_sensor_init_pixelcode = INVALID_VALUE,
       .rk_sensor_init_data = NULL,
       .rk_sensor_init_winseq = NULL,
       .rk_sensor_winseq_size = 0,
       .rk_sensor_init_data_size = 0,
    },
    #endif
    #if CONFIG_SENSOR_IIC_ADDR_15
    {
       .rk_sensor_init_width = INVALID_VALUE,
       .rk_sensor_init_height = INVALID_VALUE,
       .rk_sensor_init_bus_param = INVALID_VALUE,
       .rk_sensor_init_pixelcode = INVALID_VALUE,
       .rk_sensor_init_data = rk_init_data_sensor_reg_15,
       .rk_sensor_init_winseq = rk_init_data_sensor_winseqreg_15,
       .rk_sensor_winseq_size = sizeof(rk_init_data_sensor_winseqreg_15) / sizeof(struct reginfo_t),
       .rk_sensor_init_data_size = sizeof(rk_init_data_sensor_reg_15) / sizeof(struct reginfo_t),
    },
    #else
    {
       .rk_sensor_init_width = INVALID_VALUE,
       .rk_sensor_init_height = INVALID_VALUE,
       .rk_sensor_init_bus_param = INVALID_VALUE,
       .rk_sensor_init_pixelcode = INVALID_VALUE,
       .rk_sensor_init_data = NULL,
       .rk_sensor_init_winseq = NULL,
       .rk_sensor_winseq_size = 0,
       .rk_sensor_init_data_size = 0,
    },
    #endif
    #if CONFIG_SENSOR_IIC_ADDR_16
    {
       .rk_sensor_init_width = INVALID_VALUE,
       .rk_sensor_init_height = INVALID_VALUE,
       .rk_sensor_init_bus_param = INVALID_VALUE,
       .rk_sensor_init_pixelcode = INVALID_VALUE,
       .rk_sensor_init_data = rk_init_data_sensor_reg_16,
       .rk_sensor_init_winseq = rk_init_data_sensor_winseqreg_16,
       .rk_sensor_winseq_size = sizeof(rk_init_data_sensor_winseqreg_16) / sizeof(struct reginfo_t),
       .rk_sensor_init_data_size = sizeof(rk_init_data_sensor_reg_16) / sizeof(struct reginfo_t),
    },
    #else
    {
       .rk_sensor_init_width = INVALID_VALUE,
       .rk_sensor_init_height = INVALID_VALUE,
       .rk_sensor_init_bus_param = INVALID_VALUE,
       .rk_sensor_init_pixelcode = INVALID_VALUE,
       .rk_sensor_init_data = NULL,
       .rk_sensor_init_winseq = NULL,
       .rk_sensor_winseq_size = 0,
       .rk_sensor_init_data_size = 0,
    },
    #endif
    #if CONFIG_SENSOR_IIC_ADDR_17
    {
       .rk_sensor_init_width = INVALID_VALUE,
       .rk_sensor_init_height = INVALID_VALUE,
       .rk_sensor_init_bus_param = INVALID_VALUE,
       .rk_sensor_init_pixelcode = INVALID_VALUE,
       .rk_sensor_init_data = rk_init_data_sensor_reg_17,
       .rk_sensor_init_winseq = rk_init_data_sensor_winseqreg_17,
       .rk_sensor_winseq_size = sizeof(rk_init_data_sensor_winseqreg_17) / sizeof(struct reginfo_t),
       .rk_sensor_init_data_size = sizeof(rk_init_data_sensor_reg_17) / sizeof(struct reginfo_t),
    },
    #else
    {
       .rk_sensor_init_width = INVALID_VALUE,
       .rk_sensor_init_height = INVALID_VALUE,
       .rk_sensor_init_bus_param = INVALID_VALUE,
       .rk_sensor_init_pixelcode = INVALID_VALUE,
       .rk_sensor_init_data = NULL,
       .rk_sensor_init_winseq = NULL,
       .rk_sensor_winseq_size = 0,
       .rk_sensor_init_data_size = 0,
    },
    #endif
    #if CONFIG_SENSOR_IIC_ADDR_18
    {
       .rk_sensor_init_width = INVALID_VALUE,
       .rk_sensor_init_height = INVALID_VALUE,
       .rk_sensor_init_bus_param = INVALID_VALUE,
       .rk_sensor_init_pixelcode = INVALID_VALUE,
       .rk_sensor_init_data = rk_init_data_sensor_reg_18,
       .rk_sensor_init_winseq = rk_init_data_sensor_winseqreg_18,
       .rk_sensor_winseq_size = sizeof(rk_init_data_sensor_winseqreg_18) / sizeof(struct reginfo_t),
       .rk_sensor_init_data_size = sizeof(rk_init_data_sensor_reg_18) / sizeof(struct reginfo_t),
    },
    #else
    {
       .rk_sensor_init_width = INVALID_VALUE,
       .rk_sensor_init_height = INVALID_VALUE,
       .rk_sensor_init_bus_param = INVALID_VALUE,
       .rk_sensor_init_pixelcode = INVALID_VALUE,
       .rk_sensor_init_data = NULL,
       .rk_sensor_init_winseq = NULL,
       .rk_sensor_winseq_size = 0,
       .rk_sensor_init_data_size = 0,
    },
    #endif
    #if CONFIG_SENSOR_IIC_ADDR_19
    {
       .rk_sensor_init_width = INVALID_VALUE,
       .rk_sensor_init_height = INVALID_VALUE,
       .rk_sensor_init_bus_param = INVALID_VALUE,
       .rk_sensor_init_pixelcode = INVALID_VALUE,
       .rk_sensor_init_data = rk_init_data_sensor_reg_19,
       .rk_sensor_init_winseq = rk_init_data_sensor_winseqreg_19,
       .rk_sensor_winseq_size = sizeof(rk_init_data_sensor_winseqreg_19) / sizeof(struct reginfo_t),
       .rk_sensor_init_data_size = sizeof(rk_init_data_sensor_reg_19) / sizeof(struct reginfo_t),
    },
    #else
    {
       .rk_sensor_init_width = INVALID_VALUE,
       .rk_sensor_init_height = INVALID_VALUE,
       .rk_sensor_init_bus_param = INVALID_VALUE,
       .rk_sensor_init_pixelcode = INVALID_VALUE,
       .rk_sensor_init_data = NULL,
       .rk_sensor_init_winseq = NULL,
       .rk_sensor_winseq_size = 0,
       .rk_sensor_init_data_size = 0,
    },
    #endif

 };
#include "../../../drivers/media/video/rk30_camera.c"

#endif /* CONFIG_VIDEO_RK29 */

#if defined(CONFIG_TOUCHSCREEN_CYTTSP4)
#define CY_I2C_NAME     "cyttsp4-i2c"
#define CY_I2C_TCH_ADR	0x67
#define CY_I2C_LDR_ADR	0x69
#define CY_MAXX 1024//1368 //1296 //480
#define CY_MAXY 768//782 //828 //256

#define TOUCH_GPIO_RST_CYTTSP   RK30_PIN4_PD0
#define TOUCH_GPIO_IRQ_CYTTSP   RK30_PIN4_PC2

static int cyttsp4_i2c_init(void)
{
#if 0
	return cyttsp4_vkey_init(CY_I2C_VKEY_NAME);;
#else
	return 0;
#endif
}

static int cyttsp4_hw_reset(void)
{
	int ret = 0;

	gpio_set_value(TOUCH_GPIO_RST_CYTTSP, 1);
	pr_info("%s: gpio_set_value(step%d)=%d\n", __func__, 1, 1);
	msleep(20);
	gpio_set_value(TOUCH_GPIO_RST_CYTTSP, 0);
	pr_info("%s: gpio_set_value(step%d)=%d\n", __func__, 2, 0);
	msleep(40);
	gpio_set_value(TOUCH_GPIO_RST_CYTTSP, 1);
	msleep(20);
	pr_info("%s: gpio_set_value(step%d)=%d\n", __func__, 3, 1);

	return ret;
}

static int cyttsp4_hw_recov(int on)
{
	int retval = 0;

	pr_info("%s: on=%d\n", __func__, on);
	if (on == 0) {
		cyttsp4_hw_reset();
		retval = 0;
	} else
		retval = -EINVAL;
#if 0 // KEVKEV
	else {
		retval = gpio_direction_output(TOUCH_GPIO_IRQ_CYTTSP, 0);
		if (retval < 0) {
			pr_err("%s: Fail switch IRQ pin to output"
				" r=%d\n", __func__, retval);
		} else {
			cyttsp4_hw_reset();
			retval = gpio_direction_input(TOUCH_GPIO_IRQ_CYTTSP);
			if (retval < 0) {
				pr_err("%s: Fail switch IRQ pin to input"
					" r=%d\n", __func__, retval);
			}
		}
	}
#endif
	return 0;
}

static int cyttsp4_irq_stat(void)
{
	return gpio_get_value(TOUCH_GPIO_IRQ_CYTTSP);
}

#define CY_ABS_MIN_X 0
#define CY_ABS_MIN_Y 0
#define CY_ABS_MIN_P 0
#define CY_ABS_MIN_W 0
#define CY_ABS_MIN_T 0//1
#define CY_ABS_MAX_X CY_MAXX
#define CY_ABS_MAX_Y CY_MAXY
#define CY_ABS_MAX_P 255
#define CY_ABS_MAX_W 255
#define CY_ABS_MAX_T 9//10
#define CY_IGNORE_VALUE 0xFFFF

#include <linux/input/touch_platform.h>
#include "../../../drivers/input/touchscreen/cyttsp4_params.h"

static struct touch_settings cyttsp4_sett_param_regs = {
	.data = (uint8_t *)&cyttsp4_param_regs[0],
	.size = sizeof(cyttsp4_param_regs),
	.tag = 0,
};

static struct touch_settings cyttsp4_sett_param_size = {
	.data = (uint8_t *)&cyttsp4_param_size[0],
	.size = sizeof(cyttsp4_param_size),
	.tag = 0,
};

/* Design Data Table */
static u8 cyttsp4_ddata[] = {
	0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
	16, 17, 18, 19, 20, 21, 22, 23, 24 /* test padding, 25, 26, 27, 28, 29, 30, 31 */
};

static struct touch_settings cyttsp4_sett_ddata = {
	.data = (uint8_t *)&cyttsp4_ddata[0],
	.size = sizeof(cyttsp4_ddata),
	.tag = 0,
};

/* Manufacturing Data Table */
static u8 cyttsp4_mdata[] = {
	65, 64, /* test truncation */63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48,
	47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32,
	31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16,
	15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0
};

static struct touch_settings cyttsp4_sett_mdata = {
	.data = (uint8_t *)&cyttsp4_mdata[0],
	.size = sizeof(cyttsp4_mdata),
	.tag = 0,
};



#define CY_USE_INCLUDE_FBL
#ifdef CY_USE_INCLUDE_FBL
#include "../../../drivers/input/touchscreen/cyttsp4_img.h"
static struct touch_firmware cyttsp4_firmware = {
	.img = cyttsp4_img,
	.size = sizeof(cyttsp4_img),
	.ver = cyttsp4_ver,
	.vsize = sizeof(cyttsp4_ver),
};
#else
static u8 test_img[] = {0, 1, 2, 4, 8, 16, 32, 64, 128};
static u8 test_ver[] = {0, 0, 0, 0, 0x10, 0x20, 0, 0, 0};
static struct touch_firmware cyttsp4_firmware = {
	.img = test_img,
	.size = sizeof(test_img),
	.ver = test_ver,
	.vsize = sizeof(test_ver),
};
#endif

static const uint16_t cyttsp4_abs[] = {
	ABS_MT_POSITION_X, CY_ABS_MIN_X, CY_ABS_MAX_X, 0, 0,
	ABS_MT_POSITION_Y, CY_ABS_MIN_Y, CY_ABS_MAX_Y, 0, 0,
	ABS_MT_PRESSURE, CY_ABS_MIN_P, CY_ABS_MAX_P, 0, 0,
//	CY_IGNORE_VALUE/*ABS_MT_TOUCH_MAJOR*/, CY_ABS_MIN_W, CY_ABS_MAX_W, 0, 0,
	ABS_MT_TOUCH_MAJOR, CY_ABS_MIN_W, CY_ABS_MAX_W, 0, 0,
	ABS_MT_TRACKING_ID, CY_ABS_MIN_T, CY_ABS_MAX_T, 0, 0,
};

struct touch_framework cyttsp4_framework = {
	.abs = (uint16_t *)&cyttsp4_abs[0],
	.size = sizeof(cyttsp4_abs)/sizeof(uint16_t),
	.enable_vkeys = 1,
};

struct touch_platform_data cyttsp4_i2c_touch_platform_data = {
	.sett = {
		NULL,	/* Reserved */
		NULL,	/* Command Registers */
		NULL,	/* Touch Report */
		NULL,	/* Cypress Data Record */
		NULL,	/* Test Record */
		NULL,	/* Panel Configuration Record */
		&cyttsp4_sett_param_regs,
		&cyttsp4_sett_param_size,
		NULL,	/* Reserved */
		NULL,	/* Reserved */
		NULL,	/* Operational Configuration Record */
		&cyttsp4_sett_ddata,	/* Design Data Record */
		&cyttsp4_sett_mdata,	/* Manufacturing Data Record */
	},
	.fw = &cyttsp4_firmware,
	.frmwrk = &cyttsp4_framework,
	.addr = {CY_I2C_TCH_ADR, CY_I2C_LDR_ADR},
	.flags = /*0x01 | 0x02 | */0x20 | 0x40,
	.hw_reset = cyttsp4_hw_reset,
	.hw_recov = cyttsp4_hw_recov,
	.irq_stat = cyttsp4_irq_stat,
};
#endif

/*novatek nt11003 touchpad*/
#if defined (CONFIG_TOUCHSCREEN_NOVATEK)

#define TOUCH_NOVATEK_RESET_PIN RK30_PIN4_PD0
#define TOUCH_NOVATEK_INT_PIN RK30_PIN4_PC2

static u8 novatek_gpio_init_status = 0;

static int novatek_exit_platform_hw(void)
{
    if (novatek_gpio_init_status & (1<<0)) {
        gpio_free(TOUCH_NOVATEK_INT_PIN);
    }
    if (novatek_gpio_init_status & (1<<1)) {
        gpio_free(TOUCH_NOVATEK_RESET_PIN);
    }

    novatek_gpio_init_status = 0;
}

static int novatek_init_platform_hw(void)
{
    s32 ret = 0;

   /* request interrupt */
    if((ret = gpio_request(TOUCH_NOVATEK_INT_PIN, NULL)) != 0)
    {
	    printk("%s,%s,gpio request error!3\n", __LINE__, __FUNCTION__);
	    goto exit_novatek_init;
    }
    novatek_gpio_init_status |= (1 << 0);

    /* set reset pin */
    if((ret = gpio_request(TOUCH_NOVATEK_RESET_PIN, NULL)) != 0)
    {
	    printk("%s,%s,gpio request error!4\n", __LINE__, __FUNCTION__);
	    goto exit_novatek_init;
    }      
    novatek_gpio_init_status |= (1 << 1);

    gpio_direction_output(TOUCH_NOVATEK_RESET_PIN, GPIO_LOW);
    mdelay(100);
    gpio_set_value(TOUCH_NOVATEK_RESET_PIN, GPIO_HIGH);
    mdelay(100);

exit_novatek_init:
    if (ret < 0) {
        novatek_exit_platform_hw();
    }

    return ret;
}

static struct novatek_i2c_platform_data novatek_info = {
    .version = 11003,
    .ts_init_platform_hw = novatek_init_platform_hw,
    .ts_exit_platform_hw = novatek_exit_platform_hw,
    .gpio_rst = TOUCH_NOVATEK_RESET_PIN,
    .gpio_irq = TOUCH_NOVATEK_INT_PIN,
    .irq_edge = 1,
    .screen_max_x = 1024,
    .screen_max_y = 768,
};
#endif

/*ft5x0x touchpad*/
#if defined (CONFIG_TOUCHSCREEN_FT5306)

#define TOUCH_FT5X0X_RESET_PIN RK30_PIN4_PD0
#define TOUCH_FT5X0X_INT_PIN RK30_PIN4_PC2
#define TOUCH_FT5X0X_PWR_PIN INVALID_GPIO

static u8 ft5x0x_gpio_init_status = 0;

static int ft5x0x_exit_platform_hw(void)
{
    if (ft5x0x_gpio_init_status & (1<<0)) {
        gpio_free(TOUCH_FT5X0X_INT_PIN);
    }
    if (ft5x0x_gpio_init_status & (1<<1)) {
        gpio_free(TOUCH_FT5X0X_RESET_PIN);
    }
    if (ft5x0x_gpio_init_status & (1<<2)) {
        gpio_free(TOUCH_FT5X0X_PWR_PIN);
    }

    ft5x0x_gpio_init_status = 0;
}

static int ft5x0x_init_platform_hw(void)
{
    s32 ret = 0;
#if defined (CONFIG_MFD_WM831X)
	struct regulator *ldo;

	ldo = regulator_get(NULL, "ldo9");	//vcc_tp
	regulator_set_voltage(ldo, 2800000, 2800000);
	regulator_set_suspend_voltage(ldo, 2800000);
	regulator_disable(ldo);
//	printk("%s set ldo9 vcc_tp=%dmV end\n", __func__, regulator_get_voltage(ldo));
	regulator_put(ldo);
    mdelay(100);
#endif

#if defined (CONFIG_MFD_TPS65910)
	struct regulator *ldo;

	ldo = regulator_get(NULL, "vaux33");	//vcc_tp
	regulator_set_voltage(ldo, 2800000, 2800000);
	regulator_set_suspend_voltage(ldo, 2800000);
	regulator_disable(ldo);
	printk("%s set vaux33 vcc_tp=%dmV end\n", __func__, regulator_get_voltage(ldo));
	regulator_put(ldo);
    mdelay(100);
#endif

   /* request power enable */
    if (TOUCH_FT5X0X_PWR_PIN != INVALID_GPIO) {
        if((ret = gpio_request(TOUCH_FT5X0X_PWR_PIN, NULL)) != 0)
        {
    	    printk("%s,%s,gpio request error!3\n", __LINE__, __FUNCTION__);
    	    goto exit_ft5x0x_init;
        }
        ft5x0x_gpio_init_status |= (1 << 2);
    }

   /* request interrupt */
    if((ret = gpio_request(TOUCH_FT5X0X_INT_PIN, NULL)) != 0)
    {
	    printk("%s,%s,gpio request error!3\n", __LINE__, __FUNCTION__);
	    goto exit_ft5x0x_init;
    }
    ft5x0x_gpio_init_status |= (1 << 0);
    //gpio_pull_updown(RK29_PIN0_PA2, 1);

    /* set reset pin */
    if((ret = gpio_request(TOUCH_FT5X0X_RESET_PIN, NULL)) != 0)
    {
	    printk("%s,%s,gpio request error!4\n", __LINE__, __FUNCTION__);
	    goto exit_ft5x0x_init;
    }      
    ft5x0x_gpio_init_status |= (1 << 1);

    gpio_direction_output(TOUCH_FT5X0X_PWR_PIN, 1);
    gpio_direction_output(TOUCH_FT5X0X_RESET_PIN, GPIO_LOW);
    mdelay(100);
    gpio_set_value(TOUCH_FT5X0X_RESET_PIN, GPIO_HIGH);
    mdelay(100);

exit_ft5x0x_init:
    if (ret < 0) {
        ft5x0x_exit_platform_hw();
    }

#if defined (CONFIG_MFD_TPS65910)
	ldo = regulator_get(NULL, "vaux33");	//vcc_tp
	regulator_set_voltage(ldo, 2800000, 2800000);
	regulator_set_suspend_voltage(ldo, 2800000);
	regulator_enable(ldo);
	printk("%s set vaux33 vcc_tp=%dmV end\n", __func__, regulator_get_voltage(ldo));
	regulator_put(ldo);
    mdelay(100);
#endif

#if defined (CONFIG_MFD_WM831X)
	ldo = regulator_get(NULL, "ldo9");	//vcc_tp
	regulator_set_voltage(ldo, 2800000, 2800000);
	regulator_set_suspend_voltage(ldo, 2800000);
	regulator_enable(ldo);
//	printk("%s set ldo9 vcc_tp=%dmV end\n", __func__, regulator_get_voltage(ldo));
	regulator_put(ldo);
    mdelay(100);
#endif

    return ret;
}

static struct ft5x0x_platform_data ft5x0x_info = {
    .model= 5006,
    .init_platform_hw= ft5x0x_init_platform_hw,
    .exit_platform_hw= ft5x0x_exit_platform_hw,

};
#endif

#if defined(CONFIG_TOUCHSCREEN_GT8XX)
#define TOUCH_RESET_PIN  RK30_PIN4_PD0
#define TOUCH_PWR_PIN    INVALID_GPIO
int goodix_init_platform_hw(void)
{
	int ret;
	
	rk30_mux_api_set(GPIO4D0_SMCDATA8_TRACEDATA8_NAME, GPIO4D_GPIO4D0);
	rk30_mux_api_set(GPIO4C2_SMCDATA2_TRACEDATA2_NAME, GPIO4C_GPIO4C2);
	printk("%s:0x%x,0x%x\n",__func__,rk30_mux_api_get(GPIO4D0_SMCDATA8_TRACEDATA8_NAME),rk30_mux_api_get(GPIO4C2_SMCDATA2_TRACEDATA2_NAME));

	if (TOUCH_PWR_PIN != INVALID_GPIO) {
		ret = gpio_request(TOUCH_PWR_PIN, "goodix power pin");
		if (ret != 0) {
			gpio_free(TOUCH_PWR_PIN);
			printk("goodix power error\n");
			return -EIO;
		}
		gpio_direction_output(TOUCH_PWR_PIN, 0);
		gpio_set_value(TOUCH_PWR_PIN, GPIO_LOW);
		msleep(100);
	}

	if (TOUCH_RESET_PIN != INVALID_GPIO) {
		ret = gpio_request(TOUCH_RESET_PIN, "goodix reset pin");
		if (ret != 0) {
			gpio_free(TOUCH_RESET_PIN);
			printk("goodix gpio_request error\n");
			return -EIO;
		}
		gpio_direction_output(TOUCH_RESET_PIN, 1);
                msleep(100);
		//gpio_set_value(TOUCH_RESET_PIN, GPIO_LOW);
		//msleep(100);
		//gpio_set_value(TOUCH_RESET_PIN, GPIO_HIGH);
		//msleep(500);
	}
	return 0;
}

struct goodix_platform_data goodix_info = {
	.model = 8105,
	.irq_pin = RK30_PIN4_PC2,
	.rest_pin = TOUCH_RESET_PIN,
	.init_platform_hw = goodix_init_platform_hw,
};
#endif

#if defined (CONFIG_TOUCHSCREEN_BYD693X)
#define BYD_BF693X_MODEL_NAME           14
#define	BYD_BF693X_TOUCH_POWER_PIN		RK30_PIN2_PC2
#define BYD_BF693X_TOUCH_RESET_PIN		INVALID_GPIO
#define BYD_BF693X_TOUCH_INT_PIN        RK30_PIN4_PC2

int tp_probe_flag;

int get_probe_state(void)
{
	return tp_probe_flag;
}

void set_probe_state(int state)
{
	tp_probe_flag = state;
}

static struct byd_platform_data byd693x_info = {
	.get_probe_state = get_probe_state,
	.set_probe_state = set_probe_state,
	.model = BYD_BF693X_MODEL_NAME,
	.pwr_pin = BYD_BF693X_TOUCH_POWER_PIN,
	.rst_pin = BYD_BF693X_TOUCH_RESET_PIN,
	.int_pin = BYD_BF693X_TOUCH_INT_PIN,
	.pwr_on_value = 0,
	.tp_flag = 0,
	.screen_max_x = 800,
	.screen_max_y = 480,
	.swap_xy = 0,
	.xpol = 1,
	.ypol = 0,
};
#endif

#if defined (CONFIG_TOUCHSCREEN_BF6931A)
#define TOUCH_PWR_PIN   RK30_PIN2_PC2
#define TOUCH_INT_PIN   RK30_PIN4_PC2
int bf6931a_init_platform_hw(void)
{
	printk("bf6931a_init_platform_hw\n");
    if(gpio_request(TOUCH_PWR_PIN,NULL) != 0){
      gpio_free(TOUCH_PWR_PIN);
      printk("bf6931a_init_platform_hw gpio_request error\n");
      return -EIO;
    }

    if(gpio_request(TOUCH_INT_PIN,NULL) != 0){
      gpio_free(TOUCH_INT_PIN);
      printk("bf6931a_init_platform_hw gpio_request error\n");
      return -EIO;
    }

	gpio_direction_output(TOUCH_PWR_PIN, 1);
	mdelay(10);
	gpio_direction_input(TOUCH_INT_PIN);
	msleep(300);
    return 0;
}

void bf6931a_exit_platform_hw(void)
{
	printk("bf6931a_exit_platform_hw\n");
	gpio_free(TOUCH_PWR_PIN);
	gpio_free(TOUCH_INT_PIN);
}


int bf6931a_platform_sleep(void)
{
	printk("bf6931a_platform_sleep\n");
	//gpio_set_value(TOUCH_RESET_PIN,GPIO_LOW);
	return 0;
}

int bf6931a_platform_wakeup(void)
{
	printk("bf6931a_platform_wakeup\n");
	gpio_set_value(TOUCH_PWR_PIN,GPIO_HIGH);
	msleep(300);
	return 0;
}

struct bf6931a_platform_data bf6931a_info = {

  .init_platform_hw= bf6931a_init_platform_hw,
  .exit_platform_hw= bf6931a_exit_platform_hw,
  .bf6931a_platform_sleep  = bf6931a_platform_sleep,
  .bf6931a_platform_wakeup = bf6931a_platform_wakeup,
};
#endif

static struct spi_board_info board_spi_devices[] = {
};

/***********************************************************
*	rk30  backlight
************************************************************/
#ifdef CONFIG_BACKLIGHT_RK29_BL
#define PWM_ID            2
#define PWM_MUX_NAME      GPIO0D6_PWM2_NAME
#define PWM_MUX_MODE      GPIO0D_PWM2
#define PWM_MUX_MODE_GPIO GPIO0D_GPIO0D6
#define PWM_GPIO 	  RK30_PIN0_PD6
#define PWM_EFFECT_VALUE  0

#define LCD_DISP_ON_PIN

#ifdef  LCD_DISP_ON_PIN
//#define BL_EN_MUX_NAME    GPIOF34_UART3_SEL_NAME
//#define BL_EN_MUX_MODE    IOMUXB_GPIO1_B34

#define BL_EN_PIN         RK30_PIN6_PB3
#define BL_EN_VALUE       GPIO_HIGH
#endif
static int rk29_backlight_io_init(void)
{
	int ret = 0;
	rk30_mux_api_set(PWM_MUX_NAME, PWM_MUX_MODE);
#ifdef  LCD_DISP_ON_PIN
	// rk30_mux_api_set(BL_EN_MUX_NAME, BL_EN_MUX_MODE);

	ret = gpio_request(BL_EN_PIN, NULL);
	if (ret != 0) {
		gpio_free(BL_EN_PIN);
	}

	gpio_direction_output(BL_EN_PIN, 0);
	gpio_set_value(BL_EN_PIN, !BL_EN_VALUE);
#endif
	return ret;
}

static int rk29_backlight_io_deinit(void)
{
	int ret = 0;
#ifdef  LCD_DISP_ON_PIN
	gpio_free(BL_EN_PIN);
#endif
	rk30_mux_api_set(PWM_MUX_NAME, PWM_MUX_MODE_GPIO);
	return ret;
}

static int rk29_backlight_pwm_suspend(void)
{
	int ret = 0;
	rk30_mux_api_set(PWM_MUX_NAME, PWM_MUX_MODE_GPIO);
	if (gpio_request(PWM_GPIO, NULL)) {
		printk("func %s, line %d: request gpio fail\n", __FUNCTION__, __LINE__);
		return -1;
	}
	gpio_direction_output(PWM_GPIO, GPIO_LOW);
#ifdef  LCD_DISP_ON_PIN
	gpio_direction_output(BL_EN_PIN, 0);
	gpio_set_value(BL_EN_PIN, !BL_EN_VALUE);
#endif
	return ret;
}

static int rk29_backlight_pwm_resume(void)
{
	gpio_free(PWM_GPIO);
	rk30_mux_api_set(PWM_MUX_NAME, PWM_MUX_MODE);
#ifdef  LCD_DISP_ON_PIN
	msleep(30);
	gpio_direction_output(BL_EN_PIN, 1);
	gpio_set_value(BL_EN_PIN, BL_EN_VALUE);
#endif
	return 0;
}

static struct rk29_bl_info rk29_bl_info = {
	.pwm_id = PWM_ID,
	.bl_ref = PWM_EFFECT_VALUE,
	.io_init = rk29_backlight_io_init,
	.io_deinit = rk29_backlight_io_deinit,
    .min_brightness = 80,
	.pwm_suspend = rk29_backlight_pwm_suspend,
	.pwm_resume = rk29_backlight_pwm_resume,
};

static struct platform_device rk29_device_backlight = {
	.name	= "rk29_backlight",
	.id 	= -1,
	.dev	= {
		.platform_data  = &rk29_bl_info,
	}
};

#endif

#ifdef CONFIG_RK29_SUPPORT_MODEM

#define RK30_MODEM_POWER        RK30_PIN4_PD1
#define RK30_MODEM_POWER_IOMUX  rk29_mux_api_set(GPIO4D1_SMCDATA9_TRACEDATA9_NAME, GPIO4D_GPIO4D1)

static int rk30_modem_io_init(void)
{
    printk("%s\n", __FUNCTION__);
    RK30_MODEM_POWER_IOMUX;

	return 0;
}

static struct rk29_io_t rk30_modem_io = {
    .io_addr    = RK30_MODEM_POWER,
    .enable     = GPIO_HIGH,
    .disable    = GPIO_LOW,
    .io_init    = rk30_modem_io_init,
};

static struct platform_device rk30_device_modem = {
	.name	= "rk30_modem",
	.id 	= -1,
	.dev	= {
		.platform_data  = &rk30_modem_io,
	}
};
#endif
#if defined(CONFIG_MU509)
static int mu509_io_init(void)
{

	rk30_mux_api_set(GPIO2B6_LCDC1DATA14_SMCADDR18_TSSYNC_NAME, GPIO2B_GPIO2B6);
        rk30_mux_api_set(GPIO4D2_SMCDATA10_TRACEDATA10_NAME, GPIO4D_GPIO4D2);
	rk30_mux_api_set(GPIO2B7_LCDC1DATA15_SMCADDR19_HSADCDATA7_NAME, GPIO2B_GPIO2B7);
	rk30_mux_api_set(GPIO2C0_LCDCDATA16_GPSCLK_HSADCCLKOUT_NAME, GPIO2C_GPIO2C0);
	return 0;
}

static int mu509_io_deinit(void)
{
	
	return 0;
}
 
struct rk29_mu509_data rk29_mu509_info = {
	.io_init = mu509_io_init,
  	.io_deinit = mu509_io_deinit,
	.modem_power_en = RK30_PIN6_PB2,//RK30_PIN4_PD1,
	.bp_power = RK30_PIN2_PB6,//RK30_PIN4_PD1,
	.bp_reset = RK30_PIN4_PD2,
	.ap_wakeup_bp = RK30_PIN2_PB7,
	.bp_wakeup_ap = RK30_PIN6_PA0, 
};
struct platform_device rk29_device_mu509 = {	
        .name = "mu509",	
    	.id = -1,	
	.dev		= {
		.platform_data = &rk29_mu509_info,
	}    	
    };
#endif
#if defined(CONFIG_MW100)
static int mw100_io_init(void)
{
	 rk30_mux_api_set(GPIO2B6_LCDC1DATA14_SMCADDR18_TSSYNC_NAME, GPIO2B_GPIO2B6);
	 rk30_mux_api_set(GPIO4D2_SMCDATA10_TRACEDATA10_NAME, GPIO4D_GPIO4D2);
	 rk30_mux_api_set(GPIO2B7_LCDC1DATA15_SMCADDR19_HSADCDATA7_NAME, GPIO2B_GPIO2B7);
	 rk30_mux_api_set(GPIO2C0_LCDCDATA16_GPSCLK_HSADCCLKOUT_NAME, GPIO2C_GPIO2C0);
	return 0;
}

static int mw100_io_deinit(void)
{
	
	return 0;
}
 
struct rk29_mw100_data rk29_mw100_info = {
	.io_init = mw100_io_init,
  	.io_deinit = mw100_io_deinit,
	.modem_power_en = RK30_PIN6_PB2,
	.bp_power = RK30_PIN2_PB6,
	.bp_reset = RK30_PIN4_PD2,
	.ap_wakeup_bp = RK30_PIN2_PB7,
	.bp_wakeup_ap = RK30_PIN6_PA0,
};
struct platform_device rk29_device_mw100 = {	
        .name = "mw100",	
    	.id = -1,	
	.dev		= {
		.platform_data = &rk29_mw100_info,
	}    	
    };
#endif
#if defined(CONFIG_MT6229)
static int mt6229_io_init(void)
{
	 rk30_mux_api_set(GPIO2B6_LCDC1DATA14_SMCADDR18_TSSYNC_NAME, GPIO2B_GPIO2B6);
	 rk30_mux_api_set(GPIO4D2_SMCDATA10_TRACEDATA10_NAME, GPIO4D_GPIO4D2);
	 rk30_mux_api_set(GPIO2B7_LCDC1DATA15_SMCADDR19_HSADCDATA7_NAME, GPIO2B_GPIO2B7);
	 rk30_mux_api_set(GPIO2C0_LCDCDATA16_GPSCLK_HSADCCLKOUT_NAME, GPIO2C_GPIO2C0);
	 rk30_mux_api_set(GPIO2C1_LCDC1DATA17_SMCBLSN0_HSADCDATA6_NAME, GPIO2C_GPIO2C1);
	 rk30_mux_api_set(GPIO2C1_LCDC1DATA17_SMCBLSN0_HSADCDATA6_NAME, GPIO2C_GPIO2C1);
	return 0;

	 return 0;
}

static int mt6229_io_deinit(void)
{
	
	return 0;
}
 
struct rk29_mt6229_data rk29_mt6229_info = {
	.io_init = mt6229_io_init,
  	.io_deinit = mt6229_io_deinit,
	.modem_power_en = RK30_PIN6_PB2,
	.bp_power = RK30_PIN2_PB6,
	.modem_usb_en = RK30_PIN2_PC0,
	.modem_uart_en = RK30_PIN2_PC1,
	.bp_wakeup_ap = RK30_PIN6_PA1,
	.ap_ready = RK30_PIN2_PB7,

};
struct platform_device rk29_device_mt6229 = {	
        .name = "mt6229",	
    	.id = -1,	
	.dev		= {
		.platform_data = &rk29_mt6229_info,
	}    	
    };
#endif
#if defined(CONFIG_SEW868)
static int sew868_io_init(void)
{
	rk30_mux_api_set(GPIO2B6_LCDC1DATA14_SMCADDR18_TSSYNC_NAME, GPIO2B_GPIO2B6);
    rk30_mux_api_set(GPIO4D2_SMCDATA10_TRACEDATA10_NAME, GPIO4D_GPIO4D2);
	rk30_mux_api_set(GPIO4D4_SMCDATA12_TRACEDATA12_NAME, GPIO4D_GPIO4D4);
	return 0;
}
static int sew868_io_deinit(void)
{
	return 0;
}
struct rk30_sew868_data rk30_sew868_info = {
	.io_init = sew868_io_init,
  	.io_deinit = sew868_io_deinit,
	.bp_power = RK30_PIN6_PB2, 
	.bp_power_active_low = 1,
	.bp_sys = RK30_PIN2_PB6, 
	.bp_reset = RK30_PIN4_PD2, 
	.bp_reset_active_low = 1,
	.bp_wakeup_ap = RK30_PIN4_PD4, 
	.ap_wakeup_bp = NULL,
};

struct platform_device rk30_device_sew868 = {	
        .name = "sew868",	
    	.id = -1,	
	.dev		= {
		.platform_data = &rk30_sew868_info,
	}    	
    };
#endif

/*MMA7660gsensor*/
#if defined (CONFIG_GS_MMA7660)
#define MMA8452_INT_PIN   RK30_PIN4_PC0

static int mma8452_init_platform_hw(void)
{
	rk30_mux_api_set(GPIO4C0_SMCDATA0_TRACEDATA0_NAME, GPIO4C_GPIO4C0);

	if (gpio_request(MMA8452_INT_PIN, NULL) != 0) {
		gpio_free(MMA8452_INT_PIN);
		printk("mma8452_init_platform_hw gpio_request error\n");
		return -EIO;
	}
	gpio_pull_updown(MMA8452_INT_PIN, 1);
	return 0;
}

static struct gsensor_platform_data mma8452_info = {
	.model = 8452,
	.swap_xy = 0,
	.swap_xyz = 1,
	.init_platform_hw = mma8452_init_platform_hw,
	.orientation = {-1, 0, 0, 0, 0, 1, 0, -1, 0},
};
#endif

/*MMA8452 gsensor*/
#if defined (CONFIG_GS_MMA8452)
#define MMA8452_INT_PIN   RK30_PIN4_PC0

static int mma8452_init_platform_hw(void)
{
	rk30_mux_api_set(GPIO4C0_SMCDATA0_TRACEDATA0_NAME, GPIO4C_GPIO4C0);

	return 0;
}

static struct sensor_platform_data mma8452_info = {
	.type = SENSOR_TYPE_ACCEL,
	.irq_enable = 1,
	.poll_delay_ms = 30,
        .init_platform_hw = mma8452_init_platform_hw,
        .orientation = {-1, 0, 0, 0, 0, 1, 0, -1, 0},
};
#endif
#if defined (CONFIG_GS_LIS3DH)
#define LIS3DH_INT_PIN   RK30_PIN4_PC0

static int lis3dh_init_platform_hw(void)
{
        rk30_mux_api_set(GPIO4C0_SMCDATA0_TRACEDATA0_NAME, GPIO4C_GPIO4C0);

        return 0;
}

static struct sensor_platform_data lis3dh_info = {
	.type = SENSOR_TYPE_ACCEL,
	.irq_enable = 1,
	.poll_delay_ms = 30,
        .init_platform_hw = lis3dh_init_platform_hw,
	.orientation = {-1, 0, 0, 0, 0, 1, 0, -1, 0},
};
#endif
#if defined(CONFIG_CHARGER_SMB347)
struct smb347_info smb347_info = {
        .chg_en_pin = RK30_PIN4_PD5,    // charge enable pin      (smb347's c4 pin)
        .chg_ctl_pin = RK30_PIN6_PB2,   // charge control pin     (smb347's d2 pin)
        .chg_stat_pin = RK30_PIN4_PD6,  // charge stat pin        (smb347's f5 pin)
        .chg_susp_pin = RK30_PIN4_PD7,  // charge usb suspend pin (smb347's d3 pin)
        .max_current = 1800,            // dc and hc input current limit can set 300/500/700/900/1200/1500/1800/2000/2200/2500(ma)
        .otg_power_form_smb = 0,        // if otg 5v power form smb347 set 1 otherwise set 0
};
#endif
#if defined (CONFIG_COMPASS_AK8975)
static struct sensor_platform_data akm8975_info =
{
	.type = SENSOR_TYPE_COMPASS,
	.irq_enable = 1,
	.poll_delay_ms = 30,
	.m_layout = 
	{
		{
			{1, 0, 0},
			{0, 1, 0},
			{0, 0, 1},
		},

		{
			{1, 0, 0},
			{0, 1, 0},
			{0, 0, 1},
		},

		{
			{1, 0, 0},
			{0, 1, 0},
			{0, 0, 1},
		},

		{
			{1, 0, 0},
			{0, 1, 0},
			{0, 0, 1},
		},
	}
};

#endif

#if defined(CONFIG_GYRO_L3G4200D)

#include <linux/l3g4200d.h>
#define L3G4200D_INT_PIN  RK30_PIN4_PC3

static int l3g4200d_init_platform_hw(void)
{
	rk30_mux_api_set(GPIO4C3_SMCDATA3_TRACEDATA3_NAME, GPIO4C_GPIO4C3);
	
	return 0;
}

static struct sensor_platform_data l3g4200d_info = {
	.type = SENSOR_TYPE_GYROSCOPE,
	.irq_enable = 1,
	.poll_delay_ms = 30,
	.orientation = {0, 1, 0, -1, 0, 0, 0, 0, 1},
	.init_platform_hw = l3g4200d_init_platform_hw,
	.x_min = 40,//x_min,y_min,z_min = (0-100) according to hardware
	.y_min = 40,
	.z_min = 20,
};

#endif

#ifdef CONFIG_LS_CM3217
static struct sensor_platform_data cm3217_info = {
	.type = SENSOR_TYPE_LIGHT,
	.irq_enable = 0,
	.poll_delay_ms = 500,
};

#endif

#ifdef CONFIG_FB_ROCKCHIP

#ifdef CONFIG_MFD_RK610
#define LCD_CS_MUX_NAME    GPIO1C3_CIFDATA5_RMIITXD0_NAME
#define LCD_CS_PIN         RK30_PIN1_PC3
#define LCD_CS_VALUE       GPIO_HIGH

#define LCD_STB_MUX_NAME    GPIO2C6_LCDC1DATA22_SPI1RXD_HSADCDATA3_NAME
#define LCD_STB_PIN         RK30_PIN2_PC6
#define LCD_STB_VALUE       GPIO_HIGH

#define LCD_EN_MUX_NAME    GPIO4C7_SMCDATA7_TRACEDATA7_NAME
#define LCD_EN_PIN         RK30_PIN6_PB0
#define LCD_EN_VALUE       GPIO_LOW
#else
#define LCD_CS_MUX_NAME    GPIO4C7_SMCDATA7_TRACEDATA7_NAME
#define LCD_CS_PIN         RK30_PIN4_PC7
#define LCD_CS_VALUE       GPIO_HIGH

#define LCD_EN_MUX_NAME    GPIO4C7_SMCDATA7_TRACEDATA7_NAME
#define LCD_EN_PIN         RK30_PIN6_PB4
#define LCD_EN_VALUE       GPIO_LOW
#endif

static int rk_fb_io_init(struct rk29_fb_setting_info *fb_setting)
{
	int ret = 0;
#ifdef CONFIG_MFD_RK610
	rk30_mux_api_set(LCD_CS_MUX_NAME, GPIO1C_GPIO1C3);
#else
	rk30_mux_api_set(LCD_CS_MUX_NAME, GPIO4C_GPIO4C7);
#endif
	ret = gpio_request(LCD_CS_PIN, NULL);
	if (ret != 0)
	{
		gpio_free(LCD_CS_PIN);
		printk(KERN_ERR "request lcd cs pin fail!\n");
		return -1;
	}
	else
	{
		gpio_direction_output(LCD_CS_PIN, LCD_CS_VALUE);
	}
#ifdef CONFIG_MFD_RK610
	rk30_mux_api_set(LCD_STB_MUX_NAME, GPIO2C_GPIO2C6);
	ret = gpio_request(LCD_STB_PIN, NULL);
	if (ret != 0)
	{
		gpio_free(LCD_STB_PIN);
		printk(KERN_ERR "request lcd stb pin fail!\n");
		return -1;
	}
	else
	{
		gpio_direction_output(LCD_STB_PIN, LCD_STB_VALUE);
	}
#endif
	ret = gpio_request(LCD_EN_PIN, NULL);
	if (ret != 0)
	{
		gpio_free(LCD_EN_PIN);
		printk(KERN_ERR "request lcd en pin fail!\n");
		return -1;
	}
	else
	{
		gpio_direction_output(LCD_EN_PIN, LCD_EN_VALUE);
	}
	return 0;
}
static int rk_fb_io_disable(void)
{
	gpio_set_value(LCD_CS_PIN, LCD_CS_VALUE? 0:1);
	gpio_set_value(LCD_EN_PIN, LCD_EN_VALUE? 0:1);
#ifdef CONFIG_MFD_RK610
	gpio_direction_output(LCD_STB_PIN, LCD_STB_VALUE? 0:1);
#endif
	return 0;
}
static int rk_fb_io_enable(void)
{
#ifdef CONFIG_MFD_RK610
	gpio_direction_output(LCD_STB_PIN, LCD_STB_VALUE);
#endif
#ifdef CONFIG_MALATA_C1006
	gpio_set_value(LCD_EN_PIN, LCD_EN_VALUE);
	mdelay(100);
	gpio_set_value(LCD_CS_PIN, LCD_CS_VALUE);
#else
	gpio_set_value(LCD_CS_PIN, LCD_CS_VALUE);
	gpio_set_value(LCD_EN_PIN, LCD_EN_VALUE);
#endif
	return 0;
}

static int rk_fb_io_deinit(void)
{
	printk("%s enter!\n", __FUNCTION__);

	rk29_backlight_pwm_suspend();
	gpio_direction_output(LCD_CS_PIN, !LCD_CS_VALUE);
	gpio_set_value(LCD_CS_PIN, !LCD_CS_VALUE);
	mdelay(50);
	gpio_direction_output(LCD_EN_PIN, !LCD_EN_VALUE);
	gpio_set_value(LCD_EN_PIN, !LCD_EN_VALUE);
	mdelay(500);
	return 0;
}

#if defined(CONFIG_LCDC0_RK30)
struct rk29fb_info lcdc0_screen_info = {
	.prop	   = PRMRY,		//primary display device
	.io_init   = rk_fb_io_init,
	.io_deinit = rk_fb_io_deinit,
	.io_disable = rk_fb_io_disable,
	.io_enable = rk_fb_io_enable,
	.set_screen_info = set_lcd_info,
};
#endif

#if defined(CONFIG_LCDC1_RK30)
struct rk29fb_info lcdc1_screen_info = {
	#if defined(CONFIG_HDMI_RK30)
	.prop		= EXTEND,	//extend display device
	.lcd_info  = NULL,
	.set_screen_info = hdmi_init_lcdc,
	#endif
};
#endif

static struct resource resource_fb[] = {
	[0] = {
		.name  = "fb0 buf",
		.start = 0,
		.end   = 0,//RK30_FB0_MEM_SIZE - 1,
		.flags = IORESOURCE_MEM,
	},
	[1] = {
		.name  = "ipp buf",  //for rotate
		.start = 0,
		.end   = 0,//RK30_FB0_MEM_SIZE - 1,
		.flags = IORESOURCE_MEM,
	},
	[2] = {
		.name  = "fb2 buf",
		.start = 0,
		.end   = 0,//RK30_FB0_MEM_SIZE - 1,
		.flags = IORESOURCE_MEM,
	},
};

static struct platform_device device_fb = {
	.name		= "rk-fb",
	.id		= -1,
	.num_resources	= ARRAY_SIZE(resource_fb),
	.resource	= resource_fb,
};
#endif

#ifdef CONFIG_ANDROID_TIMED_GPIO
static struct timed_gpio timed_gpios[] = {
	{
		.name = "vibrator",
		.gpio = RK30_PIN0_PA4,
		.max_timeout = 1000,
		.active_low = 0,
		.adjust_time =20,      //adjust for diff product
	},
};

static struct timed_gpio_platform_data rk29_vibrator_info = {
	.num_gpios = 1,
	.gpios = timed_gpios,
};

static struct platform_device rk29_device_vibrator = {
	.name = "timed-gpio",
	.id = -1,
	.dev = {
		.platform_data = &rk29_vibrator_info,
	},

};
#endif

#ifdef CONFIG_LEDS_GPIO_PLATFORM
static struct gpio_led rk29_leds[] = {
	{
		.name = "button-backlight",
		.gpio = RK30_PIN4_PD7,
		.default_trigger = "timer",
		.active_low = 0,
		.retain_state_suspended = 0,
		.default_state = LEDS_GPIO_DEFSTATE_OFF,
	},
};

static struct gpio_led_platform_data rk29_leds_pdata = {
	.leds = rk29_leds,
	.num_leds = ARRAY_SIZE(rk29_leds),
};

static struct platform_device rk29_device_gpio_leds = {
	.name	= "leds-gpio",
	.id	= -1,
	.dev	= {
		.platform_data  = &rk29_leds_pdata,
	},
};
#endif

#ifdef CONFIG_RK_IRDA
#define IRDA_IRQ_PIN           RK30_PIN6_PA1

static int irda_iomux_init(void)
{
	int ret = 0;

	//irda irq pin
	ret = gpio_request(IRDA_IRQ_PIN, NULL);
	if (ret != 0) {
		gpio_free(IRDA_IRQ_PIN);
		printk(">>>>>> IRDA_IRQ_PIN gpio_request err \n ");
	}
	gpio_pull_updown(IRDA_IRQ_PIN, PullDisable);
	gpio_direction_input(IRDA_IRQ_PIN);

	return 0;
}

static int irda_iomux_deinit(void)
{
	gpio_free(IRDA_IRQ_PIN);
	return 0;
}

static struct irda_info rk29_irda_info = {
	.intr_pin = IRDA_IRQ_PIN,
	.iomux_init = irda_iomux_init,
	.iomux_deinit = irda_iomux_deinit,
	//.irda_pwr_ctl = bu92747guw_power_ctl,
};

static struct platform_device irda_device = {
#ifdef CONFIG_RK_IRDA_NET
	.name = "rk_irda",
#else
	.name = "bu92747_irda",
#endif
	.id = -1,
	.dev = {
		.platform_data = &rk29_irda_info,
	}
};
#endif

#ifdef CONFIG_ION
#define ION_RESERVE_SIZE        (80 * SZ_1M)
static struct ion_platform_data rk30_ion_pdata = {
	.nr = 1,
	.heaps = {
		{
			.type = ION_HEAP_TYPE_CARVEOUT,
			.id = ION_NOR_HEAP_ID,
			.name = "norheap",
			.size = ION_RESERVE_SIZE,
		}
	},
};

static struct platform_device device_ion = {
	.name = "ion-rockchip",
	.id = 0,
	.dev = {
		.platform_data = &rk30_ion_pdata,
	},
};
#endif

/**************************************************************************************************
 * SDMMC devices,  include the module of SD,MMC,and sdio.noted by xbw at 2012-03-05
**************************************************************************************************/
#ifdef CONFIG_SDMMC_RK29
#include "board-rk30-sdk-sdmmc.c"

#if defined(CONFIG_SDMMC0_RK29_WRITE_PROTECT)
#define SDMMC0_WRITE_PROTECT_PIN	RK30_PIN3_PB2	//According to your own project to set the value of write-protect-pin.
#endif

#if defined(CONFIG_SDMMC1_RK29_WRITE_PROTECT)
#define SDMMC1_WRITE_PROTECT_PIN	RK30_PIN3_PB3	//According to your own project to set the value of write-protect-pin.
#endif

#define RK29SDK_WIFI_SDIO_CARD_DETECT_N    RK30_PIN6_PB2
#define RK29SDK_WIFI_SDIO_CARD_INT         RK30_PIN3_PD2

#define RK29SDK_SD_CARD_DETECT_N        RK30_PIN3_PB6  //According to your own project to set the value of card-detect-pin.
#define RK29SDK_SD_CARD_INSERT_LEVEL    GPIO_LOW         // set the voltage of insert-card. Please pay attention to the default setting.
#endif //endif ---#ifdef CONFIG_SDMMC_RK29

#ifdef CONFIG_SDMMC0_RK29
static int rk29_sdmmc0_cfg_gpio(void)
{
#ifdef CONFIG_SDMMC_RK29_OLD
	rk30_mux_api_set(GPIO3B1_SDMMC0CMD_NAME, GPIO3B_SDMMC0_CMD);
	rk30_mux_api_set(GPIO3B0_SDMMC0CLKOUT_NAME, GPIO3B_SDMMC0_CLKOUT);
	rk30_mux_api_set(GPIO3B2_SDMMC0DATA0_NAME, GPIO3B_SDMMC0_DATA0);
	rk30_mux_api_set(GPIO3B3_SDMMC0DATA1_NAME, GPIO3B_SDMMC0_DATA1);
	rk30_mux_api_set(GPIO3B4_SDMMC0DATA2_NAME, GPIO3B_SDMMC0_DATA2);
	rk30_mux_api_set(GPIO3B5_SDMMC0DATA3_NAME, GPIO3B_SDMMC0_DATA3);

	rk30_mux_api_set(GPIO3B6_SDMMC0DETECTN_NAME, GPIO3B_GPIO3B6);

	rk30_mux_api_set(GPIO3A7_SDMMC0PWREN_NAME, GPIO3A_GPIO3A7);
	gpio_request(RK30_PIN3_PA7, "sdmmc-power");
	gpio_direction_output(RK30_PIN3_PA7, GPIO_LOW);

#else
	rk29_sdmmc_set_iomux(0, 0xFFFF);

#if defined(CONFIG_SDMMC0_RK29_SDCARD_DET_FROM_GPIO)
     rk30_mux_api_set(GPIO3B6_SDMMC0DETECTN_NAME, GPIO3B_GPIO3B6);
#else
	rk30_mux_api_set(GPIO3B6_SDMMC0DETECTN_NAME, GPIO3B_SDMMC0_DETECT_N);
#endif	

#if defined(CONFIG_SDMMC0_RK29_WRITE_PROTECT)
	gpio_request(SDMMC0_WRITE_PROTECT_PIN, "sdmmc-wp");
	gpio_direction_input(SDMMC0_WRITE_PROTECT_PIN);
#endif

#endif

	return 0;
}

#define CONFIG_SDMMC0_USE_DMA
struct rk29_sdmmc_platform_data default_sdmmc0_data = {
	.host_ocr_avail =
	    (MMC_VDD_25_26 | MMC_VDD_26_27 | MMC_VDD_27_28 | MMC_VDD_28_29 |
	     MMC_VDD_29_30 | MMC_VDD_30_31 | MMC_VDD_31_32 | MMC_VDD_32_33 |
	     MMC_VDD_33_34 | MMC_VDD_34_35 | MMC_VDD_35_36),
	.host_caps =
	    (MMC_CAP_4_BIT_DATA | MMC_CAP_MMC_HIGHSPEED | MMC_CAP_SD_HIGHSPEED),
	.io_init = rk29_sdmmc0_cfg_gpio,

#if !defined(CONFIG_SDMMC_RK29_OLD)
	.set_iomux = rk29_sdmmc_set_iomux,
#endif

	.dma_name = "sd_mmc",
#ifdef CONFIG_SDMMC0_USE_DMA
	.use_dma = 1,
#else
	.use_dma = 0,
#endif
#if defined(CONFIG_WIFI_COMBO_MODULE_CONTROL_FUNC)
    .status = rk29sdk_wifi_mmc0_status,
    .register_status_notify = rk29sdk_wifi_mmc0_status_register,
#endif
#if defined(CONFIG_SDMMC0_RK29_SDCARD_DET_FROM_GPIO)
    .detect_irq = RK29SDK_SD_CARD_DETECT_N,
    .insert_card_level = RK29SDK_SD_CARD_INSERT_LEVEL,
#else
	.detect_irq = RK30_PIN3_PB6,	// INVALID_GPIO
#endif
	.enable_sd_wakeup = 0,

#if defined(CONFIG_SDMMC0_RK29_WRITE_PROTECT)
	.write_prt = SDMMC0_WRITE_PROTECT_PIN,
#else
	.write_prt = INVALID_GPIO,
#endif
};
#endif // CONFIG_SDMMC0_RK29

#ifdef CONFIG_SDMMC1_RK29
#define CONFIG_SDMMC1_USE_DMA
static int rk29_sdmmc1_cfg_gpio(void)
{
#if defined(CONFIG_SDMMC_RK29_OLD)
	rk30_mux_api_set(GPIO3C0_SMMC1CMD_NAME, GPIO3C_SMMC1_CMD);
	rk30_mux_api_set(GPIO3C5_SDMMC1CLKOUT_NAME, GPIO3C_SDMMC1_CLKOUT);
	rk30_mux_api_set(GPIO3C1_SDMMC1DATA0_NAME, GPIO3C_SDMMC1_DATA0);
	rk30_mux_api_set(GPIO3C2_SDMMC1DATA1_NAME, GPIO3C_SDMMC1_DATA1);
	rk30_mux_api_set(GPIO3C3_SDMMC1DATA2_NAME, GPIO3C_SDMMC1_DATA2);
	rk30_mux_api_set(GPIO3C4_SDMMC1DATA3_NAME, GPIO3C_SDMMC1_DATA3);
	//rk30_mux_api_set(GPIO3C6_SDMMC1DETECTN_NAME, GPIO3C_SDMMC1_DETECT_N);

#else

#if defined(CONFIG_SDMMC1_RK29_WRITE_PROTECT)
	gpio_request(SDMMC1_WRITE_PROTECT_PIN, "sdio-wp");
	gpio_direction_input(SDMMC1_WRITE_PROTECT_PIN);
#endif

#endif

	return 0;
}

struct rk29_sdmmc_platform_data default_sdmmc1_data = {
	.host_ocr_avail =
	    (MMC_VDD_25_26 | MMC_VDD_26_27 | MMC_VDD_27_28 | MMC_VDD_28_29 |
	     MMC_VDD_29_30 | MMC_VDD_30_31 | MMC_VDD_31_32 | MMC_VDD_32_33 |
	     MMC_VDD_33_34),

#if !defined(CONFIG_USE_SDMMC1_FOR_WIFI_DEVELOP_BOARD)
	.host_caps = (MMC_CAP_4_BIT_DATA | MMC_CAP_SDIO_IRQ |
		      MMC_CAP_MMC_HIGHSPEED | MMC_CAP_SD_HIGHSPEED),
#else
	.host_caps =
	    (MMC_CAP_4_BIT_DATA | MMC_CAP_MMC_HIGHSPEED | MMC_CAP_SD_HIGHSPEED),
#endif

	.io_init = rk29_sdmmc1_cfg_gpio,

#if !defined(CONFIG_SDMMC_RK29_OLD)
	.set_iomux = rk29_sdmmc_set_iomux,
#endif

	.dma_name = "sdio",
#ifdef CONFIG_SDMMC1_USE_DMA
	.use_dma = 1,
#else
	.use_dma = 0,
#endif

#if !defined(CONFIG_USE_SDMMC1_FOR_WIFI_DEVELOP_BOARD)
#if defined(CONFIG_WIFI_CONTROL_FUNC) || defined(CONFIG_WIFI_COMBO_MODULE_CONTROL_FUNC)
	.status = rk29sdk_wifi_status,
	.register_status_notify = rk29sdk_wifi_status_register,
#endif
#if defined(CONFIG_SDMMC0_RK29_SDCARD_DET_FROM_GPIO)	
	.detect_irq = INVALID_GPIO,//RK29SDK_WIFI_SDIO_CARD_DETECT_N,
#else
	//.detect_irq = RK29SDK_WIFI_SDIO_CARD_DETECT_N, 	
#endif

#if defined(CONFIG_SDMMC1_RK29_WRITE_PROTECT)
	.write_prt = SDMMC1_WRITE_PROTECT_PIN,
#else
	.write_prt = INVALID_GPIO,
#endif

#if defined(CONFIG_RK29_SDIO_IRQ_FROM_GPIO)
    .sdio_INT_gpio = RK29SDK_WIFI_SDIO_CARD_INT,
#endif

#else
	.detect_irq = INVALID_GPIO,
	.enable_sd_wakeup = 0,
#endif

};
#endif //endif--#ifdef CONFIG_SDMMC1_RK29

/**************************************************************************************************
 * the end of setting for SDMMC devices
**************************************************************************************************/

#if defined(CONFIG_BATTERY_RK30_ADC)||defined(CONFIG_BATTERY_RK30_ADC_FAC)
static struct rk30_adc_battery_platform_data rk30_adc_battery_platdata = {
        .dc_det_pin      = RK30_PIN6_PA5,
        .batt_low_pin    = INVALID_GPIO,
        .charge_set_pin  = INVALID_GPIO,
        .charge_ok_pin   = RK30_PIN6_PA6,
        .dc_det_level    = GPIO_LOW,
        .charge_ok_level = GPIO_HIGH,
};

static struct platform_device rk30_device_adc_battery = {
        .name   = "rk30-battery",
        .id     = -1,
        .dev = {
                .platform_data = &rk30_adc_battery_platdata,
        },
};
#endif

#ifdef CONFIG_RK29_VMAC
#define PHY_PWR_EN_GPIO	RK30_PIN1_PD6
#include "board-rk30-sdk-vmac.c"
#endif

#ifdef CONFIG_RFKILL_RK
// bluetooth rfkill device, its driver in net/rfkill/rfkill-rk.c
static struct rfkill_rk_platform_data rfkill_rk_platdata = {
    .type               = RFKILL_TYPE_BLUETOOTH,

    .poweron_gpio       = { // BT_REG_ON
        .io             = RK30_PIN3_PC7,
        .enable         = GPIO_HIGH,
        .iomux          = {
            .name       = GPIO3C7_SDMMC1WRITEPRT_NAME,
            .fgpio      = GPIO3C_GPIO3C7,
        },
    },

    .reset_gpio         = { // BT_RST
        .io             = RK30_PIN3_PD1, // set io to INVALID_GPIO for disable it
        .enable         = GPIO_LOW,
        .iomux          = {
            .name       = GPIO3D1_SDMMC1BACKENDPWR_NAME,
            .fgpio      = GPIO3D_GPIO3D1,
        },
    },

    .wake_gpio          = { // BT_WAKE, use to control bt's sleep and wakeup
        .io             = RK30_PIN3_PC6, // set io to INVALID_GPIO for disable it
        .enable         = GPIO_HIGH,
        .iomux          = {
            .name       = GPIO3C6_SDMMC1DETECTN_NAME,
            .fgpio      = GPIO3C_GPIO3C6,
        },
    },

    .wake_host_irq      = { // BT_HOST_WAKE, for bt wakeup host when it is in deep sleep
        .gpio           = {
            .io         = RK30_PIN6_PA7, // set io to INVALID_GPIO for disable it
            .enable     = GPIO_LOW,      // set GPIO_LOW for falling, set 0 for rising
            .iomux      = {
                .name   = NULL,
            },
        },
    },

    .rts_gpio           = { // UART_RTS, enable or disable BT's data coming
        .io             = RK30_PIN1_PA3, // set io to INVALID_GPIO for disable it
        .enable         = GPIO_LOW,
        .iomux          = {
            .name       = GPIO1A3_UART0RTSN_NAME,
            .fgpio      = GPIO1A_GPIO1A3,
            .fmux       = GPIO1A_UART0_RTS_N,
        },
    },
};

static struct platform_device device_rfkill_rk = {
    .name   = "rfkill_rk",
    .id     = -1,
    .dev    = {
        .platform_data = &rfkill_rk_platdata,
    },
};
#endif

#ifdef CONFIG_RK30_PWM_REGULATOR
const static int pwm_voltage_map[] = {
#ifdef  CONFIG_KP_AXP20
	1000000, 1050000, 1075000, 1100000, 1125000, 1150000, 1175000, 1200000, 1225000, 1250000, 1275000, 1300000, 1325000, 1350000, 1375000, 1400000, 1425000
#else
	1000000, 1025000, 1050000, 1075000, 1100000, 1125000, 1150000, 1175000, 1200000, 1225000, 1250000, 1275000, 1300000, 1325000, 1350000, 1375000, 1400000
#endif
};

static struct regulator_consumer_supply pwm_dcdc1_consumers[] = {
	{
		.supply = "vdd_core",
	}
};

struct regulator_init_data pwm_regulator_init_dcdc[1] =
{
	{
		.constraints = {
			.name = "PWM_DCDC1",
			.min_uV = 600000,
			.max_uV = 1800000,	//0.6-1.8V
			.apply_uV = true,
			.valid_ops_mask = REGULATOR_CHANGE_STATUS | REGULATOR_CHANGE_VOLTAGE,
		},
		.num_consumer_supplies = ARRAY_SIZE(pwm_dcdc1_consumers),
		.consumer_supplies = pwm_dcdc1_consumers,
	},
};
static struct pwm_platform_data pwm_regulator_info[1] = {
	{
		.pwm_id = 3,
		.pwm_gpio = RK30_PIN0_PD7,
		.pwm_iomux_name = GPIO0D7_PWM3_NAME,
		.pwm_iomux_pwm = GPIO0D_PWM3,
		.pwm_iomux_gpio = GPIO0D_GPIO0D6,
		.pwm_voltage = 1100000,
		.suspend_voltage = 1050000,
		.min_uV = 1000000,
#ifdef  CONFIG_KP_AXP20
		.max_uV	= 1425000,
#else
		.max_uV	= 1400000,
#endif
		.coefficient = 455,	//45.5%
		.pwm_voltage_map = pwm_voltage_map,
		.init_data	= &pwm_regulator_init_dcdc[0],
	},
};

#ifdef  CONFIG_KP_AXP20
//-----------------
const static int pwm_voltage_map_1[] = {
	1000000, 1050000, 1075000, 1100000, 1125000, 1150000, 1175000, 1200000, 1225000, 1250000, 1275000, 1300000, 1325000, 1350000, 1375000, 1400000, 1425000
};
static struct regulator_consumer_supply pwm_dcdc1_consumers_1[] = {
 	{
		.supply = "vdd_cpu", 
	}
};

struct regulator_init_data pwm_regulator_init_dcdc_1[1] =
{
	{
		.constraints = {
			.name = "PWM_DCDC1",
			.min_uV = 600000,
			.max_uV = 1800000,	//0.6-1.8V
			.apply_uV = true,
			.valid_ops_mask = REGULATOR_CHANGE_STATUS | REGULATOR_CHANGE_VOLTAGE,
		},
		.num_consumer_supplies = ARRAY_SIZE(pwm_dcdc1_consumers_1),
		.consumer_supplies = pwm_dcdc1_consumers_1,
	},
};

static struct pwm_platform_data pwm_regulator_info_1[1] = {
	{
		.pwm_id = 0,
		.pwm_gpio = RK30_PIN0_PA3,
		.pwm_iomux_name = GPIO0A3_PWM0_NAME,
		.pwm_iomux_pwm = GPIO0A_PWM0,
		.pwm_iomux_gpio = GPIO0D_GPIO0D6,
		.pwm_voltage = 1100000,
		.suspend_voltage = 1050000,
		.min_uV = 1000000,
		.max_uV	= 1425000,
		.coefficient = 455,	//45.5%
		.pwm_voltage_map = pwm_voltage_map_1,
		.init_data	= &pwm_regulator_init_dcdc_1[0],
	},
};

struct platform_device pwm_regulator_device[2] = {
	{
		.name = "pwm-voltage-regulator",
		.id = 0,
		.dev		= {
			.platform_data = &pwm_regulator_info[0],
		}
	},
	{
		.name = "pwm-voltage-regulator",
		.id = 1,
		.dev		= {
			.platform_data = &pwm_regulator_info_1[0],
		}
	},
};
#else
struct platform_device pwm_regulator_device[1] = {
	{
		.name = "pwm-voltage-regulator",
		.id = 0,
		.dev		= {
			.platform_data = &pwm_regulator_info[0],
		}
	},
};
#endif
#endif

static struct platform_device *devices[] __initdata = {
#ifdef CONFIG_BACKLIGHT_RK29_BL
	&rk29_device_backlight,
#endif
#ifdef CONFIG_FB_ROCKCHIP
	&device_fb,
#endif
#ifdef CONFIG_ION
	&device_ion,
#endif
#ifdef CONFIG_ANDROID_TIMED_GPIO
	&rk29_device_vibrator,
#endif
#ifdef CONFIG_LEDS_GPIO_PLATFORM
	&rk29_device_gpio_leds,
#endif
#ifdef CONFIG_RK_IRDA
	&irda_device,
#endif
#if defined(CONFIG_WIFI_CONTROL_FUNC)||defined(CONFIG_WIFI_COMBO_MODULE_CONTROL_FUNC)
	&rk29sdk_wifi_device,
#endif
#ifdef CONFIG_RK29_SUPPORT_MODEM
	&rk30_device_modem,
#endif
#if defined(CONFIG_MU509)
	&rk29_device_mu509,
#endif
#if defined(CONFIG_MW100)
	&rk29_device_mw100,
#endif
#if defined(CONFIG_MT6229)
	&rk29_device_mt6229,
#endif
#if defined(CONFIG_SEW868)
	&rk30_device_sew868,
#endif
#if defined(CONFIG_BATTERY_RK30_ADC)||defined(CONFIG_BATTERY_RK30_ADC_FAC)
 	&rk30_device_adc_battery,
#endif
#ifdef CONFIG_RFKILL_RK
	&device_rfkill_rk,
#endif
#ifdef CONFIG_RK30_PWM_REGULATOR
	&pwm_regulator_device[0],
#ifdef  CONFIG_KP_AXP20
	&pwm_regulator_device[1],
#endif
#endif
};

// i2c
#ifdef CONFIG_I2C0_RK30
static struct i2c_board_info __initdata i2c0_info[] = {
#if defined (CONFIG_GS_MMA8452)
	{
		.type	        = "gs_mma8452",
		.addr	        = 0x1d,
		.flags	        = 0,
		.irq	        = MMA8452_INT_PIN,
		.platform_data = &mma8452_info,
	},
#endif
#if defined (CONFIG_GS_LIS3DH)
	{
		.type	        = "gs_lis3dh",
		.addr	        = 0x19,   //0x19(SA0-->VCC), 0x18(SA0-->GND)
		.flags	        = 0,
		.irq	        = LIS3DH_INT_PIN,
		.platform_data = &lis3dh_info,
	},
#endif
#if defined (CONFIG_GS_MMA7660)
	{
		.type	        = "gs_mma8452",
		.addr	        = 0x4c,
		.flags	        = 0,
		.irq	        = MMA8452_INT_PIN,
		.platform_data = &mma8452_info,
	},
#endif
#if defined (CONFIG_COMPASS_AK8975)
	{
		.type          = "ak8975",
		.addr          = 0x0d,
		.flags         = 0,
		.irq           = RK30_PIN4_PC1,
		.platform_data = &akm8975_info,
	},
#endif
#if defined (CONFIG_GYRO_L3G4200D)
	{
		.type          = "l3g4200d_gryo",
		.addr          = 0x69,
		.flags         = 0,
		.irq           = L3G4200D_INT_PIN,
		.platform_data = &l3g4200d_info,
	},
#endif
#if defined (CONFIG_SND_SOC_RK1000)
	{
		.type          = "rk1000_i2c_codec",
		.addr          = 0x60,
		.flags         = 0,
	},
	{
		.type          = "rk1000_control",
		.addr          = 0x40,
		.flags         = 0,
	},
#endif
#if defined (CONFIG_SND_SOC_RT5631)
        {
                .type                   = "rt5631",
                .addr                   = 0x1a,
                .flags                  = 0,
        },
#endif

#if defined (CONFIG_SND_SOC_RT5633)
        {
                .type                   = "rt5633",
                .addr                   = 0x1c,
                .flags                  = 0,
        },
#endif
#ifdef CONFIG_MFD_RK610
		{
			.type			= "rk610_ctl",
			.addr			= 0x40,
			.flags			= 0,
		},
#ifdef CONFIG_RK610_TVOUT
		{
			.type			= "rk610_tvout",
			.addr			= 0x42,
			.flags			= 0,
		},
#endif
#ifdef  CONFIG_HDMI_RK610
		{
			.type			= "rk610_hdmi",
			.addr			= 0x46,
			.flags			= 0,
			.irq			= RK30_PIN6_PB4,
		},
#endif
#ifdef CONFIG_SND_SOC_RK610
		{//RK610_CODEC addr  from 0x60 to 0x80 (0x60~0x80)
			.type			= "rk610_i2c_codec",
			.addr			= 0x60,
			.flags			= 0,
		},
#endif
#endif

};
#endif

#define PMIC_TYPE_WM8326	1
#define PMIC_TYPE_TPS65910	2
int __sramdata g_pmic_type =  0;
#ifdef CONFIG_I2C1_RK30
#ifdef CONFIG_MFD_WM831X_I2C
#include "board-rk30-sdk-wm8326.c"
#endif
#ifdef CONFIG_MFD_TPS65910
#define TPS65910_HOST_IRQ        RK30_PIN6_PA4
#include "board-rk30-sdk-tps65910.c"
#endif

#ifdef CONFIG_KP_AXP20
#include "../../../drivers/power/axp_power/axp-board.h"
#endif

static struct i2c_board_info __initdata i2c1_info[] = {
#if defined (CONFIG_MFD_WM831X_I2C)
	{
		.type          = "wm8326",
		.addr          = 0x34,
		.flags         = 0,
		.irq           = RK30_PIN6_PA4,
		.platform_data = &wm831x_platdata,
	},
#endif
#if defined (CONFIG_CHARGER_SMB347)
        {
                .type                   = "smb347",
                .addr                   = 0x06,
                .flags                  = 0,
                .platform_data = &smb347_info,
        },
#endif
#if defined  (CONFIG_KP_AXP20)
	{
		.type = "axp_mfd",
		.addr = 0x34,
		.flags  = 0,		
		.irq =RK30_PIN6_PA4,
		.platform_data = &axp_pdata,
	},
#endif
#if defined (CONFIG_MFD_TPS65910)
	{
        .type           = "tps65910",
        .addr           = TPS65910_I2C_ID0,
        .flags          = 0,
        .irq            = TPS65910_HOST_IRQ,
    	.platform_data = &tps65910_data,
	},
#endif
#if defined (CONFIG_RTC_HYM8563)
         {
                 .type                   = "rtc_hym8563",
                 .addr           = 0x51,
                 .flags                  = 0,
                 .irq            = RK30_PIN6_PA0,
         },
 #endif
};
#endif

#ifdef  CONFIG_KP_AXP20
#define cru_readl(offset)	readl_relaxed(RK30_CRU_BASE + offset)
#define cru_writel(v, offset)	do { writel_relaxed(v, RK30_CRU_BASE + offset); dsb(); } while (0)
#define grf_readl(offset)	readl_relaxed(RK30_GRF_BASE + offset)
#define grf_writel(v, offset)	do { writel_relaxed(v, RK30_GRF_BASE + offset); dsb(); } while (0)

#define CRU_CLKGATE5_CON_ADDR 0x00e4
#define GRF_GPIO6L_DIR_ADDR 0x0030
#define GRF_GPIO6L_DO_ADDR 0x0068
#define GRF_GPIO6L_EN_ADDR 0x00a0
#define CRU_CLKGATE5_GRFCLK_ON 0x00100000
#define CRU_CLKGATE5_GRFCLK_OFF 0x00100010
#define GPIO6_PB1_DIR_OUT  0x02000200
#define GPIO6_PB1_DO_LOW  0x02000000
#define GPIO6_PB1_DO_HIGH  0x02000200
#define GPIO6_PB1_EN_MASK  0x02000200
#define GPIO6_PB1_UNEN_MASK  0x02000000

void __sramfunc board_pmu_axp202_suspend(void)
{	
	cru_writel(CRU_CLKGATE5_GRFCLK_ON,CRU_CLKGATE5_CON_ADDR); //open grf clk
	grf_writel(GPIO6_PB1_DIR_OUT, GRF_GPIO6L_DIR_ADDR);
	grf_writel(GPIO6_PB1_DO_HIGH, GRF_GPIO6L_DO_ADDR);  //set gpio6_b1 output low
	grf_writel(GPIO6_PB1_EN_MASK, GRF_GPIO6L_EN_ADDR);
}
void __sramfunc board_pmu_axp202_resume(void)
{
	grf_writel(GPIO6_PB1_DIR_OUT, GRF_GPIO6L_DIR_ADDR);
	grf_writel(GPIO6_PB1_DO_LOW, GRF_GPIO6L_DO_ADDR);     //set gpio6_b1 output high
	grf_writel(GPIO6_PB1_EN_MASK, GRF_GPIO6L_EN_ADDR);
#ifdef CONFIG_CLK_SWITCH_TO_32K
	sram_32k_udelay(10000);
#else
	sram_udelay(10000);
#endif
}
#endif

void __sramfunc board_pmu_suspend(void)
{      
	#if defined (CONFIG_MFD_WM831X_I2C)
       if(g_pmic_type == PMIC_TYPE_WM8326)
       board_pmu_wm8326_suspend();
	#endif
	#if defined (CONFIG_MFD_TPS65910)
       if(g_pmic_type == PMIC_TYPE_TPS65910)
       board_pmu_tps65910_suspend(); 
    #endif   
	#if defined (CONFIG_KP_AXP20)
       board_pmu_axp202_suspend(); 
    #endif   
}

void __sramfunc board_pmu_resume(void)
{      
	#if defined (CONFIG_MFD_WM831X_I2C)
       if(g_pmic_type == PMIC_TYPE_WM8326)
       board_pmu_wm8326_resume();
	#endif
	#if defined (CONFIG_MFD_TPS65910)
       if(g_pmic_type == PMIC_TYPE_TPS65910)
       board_pmu_tps65910_resume(); 
	#endif
	#if defined (CONFIG_KP_AXP20)
       board_pmu_axp202_resume(); 
	#endif   
}

#ifdef  CONFIG_KP_AXP20
int __sramdata gpio0a3_iomux,gpio0a3_do,gpio0a3_dir,gpio0a3_en;

void __sramfunc rk30_pwm_arm_suspend_voltage(void)
{
#ifdef CONFIG_RK30_PWM_REGULATOR
//	int gpio0d7_iomux,gpio0d7_do,gpio0d7_dir,gpio0d7_en;
	sram_udelay(10000);
	gpio0a3_iomux = readl_relaxed(GRF_GPIO0A_IOMUX);
	gpio0a3_do = grf_readl(GRF_GPIO0L_DO);
	gpio0a3_dir = grf_readl(GRF_GPIO0L_DIR);
	gpio0a3_en = grf_readl(GRF_GPIO0L_EN);

	writel_relaxed((1<<22), GRF_GPIO0A_IOMUX);
	grf_writel((1<<19)|(1<<3), GRF_GPIO0L_DIR);
	grf_writel((1<<19)|(1<<3), GRF_GPIO0L_DO);
	grf_writel((1<<19)|(1<<3), GRF_GPIO0L_EN);
#endif 
}

void __sramfunc rk30_pwm_arm_resume_voltage(void)
{
#ifdef CONFIG_RK30_PWM_REGULATOR
	writel_relaxed((1<<22)|gpio0a3_iomux, GRF_GPIO0A_IOMUX);
	grf_writel((1<<19)|gpio0a3_en, GRF_GPIO0L_EN);
	grf_writel((1<<19)|gpio0a3_dir, GRF_GPIO0L_DIR);
	grf_writel((1<<19)|gpio0a3_do, GRF_GPIO0L_DO);
	sram_udelay(10000);
#endif
}
#endif

int __sramdata gpio0d7_iomux,gpio0d7_do,gpio0d7_dir,gpio0d7_en;

void __sramfunc rk30_pwm_logic_suspend_voltage(void)
{
#ifdef CONFIG_RK30_PWM_REGULATOR

//	int gpio0d7_iomux,gpio0d7_do,gpio0d7_dir,gpio0d7_en;
	sram_udelay(10000);
	gpio0d7_iomux = readl_relaxed(GRF_GPIO0D_IOMUX);
	gpio0d7_do = grf_readl(GRF_GPIO0H_DO);
	gpio0d7_dir = grf_readl(GRF_GPIO0H_DIR);
	gpio0d7_en = grf_readl(GRF_GPIO0H_EN);

	writel_relaxed((1<<30), GRF_GPIO0D_IOMUX);
	grf_writel((1<<31)|(1<<15), GRF_GPIO0H_DIR);
	grf_writel((1<<31)|(1<<15), GRF_GPIO0H_DO);
	grf_writel((1<<31)|(1<<15), GRF_GPIO0H_EN);
#endif 
#ifdef  CONFIG_KP_AXP20
	rk30_pwm_arm_suspend_voltage();
#endif
}
void __sramfunc rk30_pwm_logic_resume_voltage(void)
{
#ifdef CONFIG_RK30_PWM_REGULATOR
#ifdef  CONFIG_KP_AXP20
	rk30_pwm_arm_resume_voltage();
	sram_udelay(10000);
#endif
	writel_relaxed((1<<30)|gpio0d7_iomux, GRF_GPIO0D_IOMUX);
	grf_writel((1<<31)|gpio0d7_en, GRF_GPIO0H_EN);
	grf_writel((1<<31)|gpio0d7_dir, GRF_GPIO0H_DIR);
	grf_writel((1<<31)|gpio0d7_do, GRF_GPIO0H_DO);
	sram_udelay(10000);

#endif

}
extern void pwm_suspend_voltage(void);
extern void pwm_resume_voltage(void);
void  rk30_pwm_suspend_voltage_set(void)
{
#ifdef CONFIG_RK30_PWM_REGULATOR
	pwm_suspend_voltage();
#endif
}
void  rk30_pwm_resume_voltage_set(void)
{
#ifdef CONFIG_RK30_PWM_REGULATOR
	pwm_resume_voltage();
#endif
}
#ifdef CONFIG_I2C2_RK30
static struct i2c_board_info __initdata i2c2_info[] = {
#if defined (CONFIG_TOUCHSCREEN_GT8XX)
	{
		.type          = "Goodix-TS",
		.addr          = 0x55,
		.flags         = 0,
		.irq           = RK30_PIN4_PC2,
		.platform_data = &goodix_info,
	},
#endif
#if defined (CONFIG_TOUCHSCREEN_BYD693X)
	{
		.type			= "byd693x-ts",
		.addr			= 0x52,
		.flags			= 0,
		.irq			= RK30_PIN4_PC2,
		.platform_data = &byd693x_info,
	},	
#endif
#if defined (CONFIG_TOUCHSCREEN_BF6931A)
    {
		.type	="bf6931a_ts",
		.addr 	= 0x2c,    //0x70,
		.flags      =0,
		.irq		=RK30_PIN4_PC2, // support goodix tp detect, 20110706
		.platform_data = &bf6931a_info,
    },
#endif

#if defined (CONFIG_TOUCHSCREEN_SSL)	
 	{		
		.type          ="ssd253x-ts",		
		.addr          =0x48,		
		.flags          =0,		
		.irq		=RK30_PIN0_PA2,	
	},
#endif

#if defined (CONFIG_TOUCHSCREEN_CYTTSP4)
	{
		.type          = CY_I2C_NAME,
		.addr          = CY_I2C_LDR_ADR,
		.irq           = TOUCH_GPIO_IRQ_CYTTSP,
		.platform_data = &cyttsp4_i2c_touch_platform_data,
	},
#endif
#if defined (CONFIG_TOUCHSCREEN_GT801PLUS_819_8105_8110)
	{
		.type          = "GT801PLUS-TS",
		.addr          = 0x55,
	},
#endif
#if defined (CONFIG_TOUCHSCREEN_GT813_827_828)
    {
        .type           = "GT828-TS",
        .addr           = 0x5d,
    },
#endif

#if defined (CONFIG_TOUCHSCREEN_GT800_801)
    {
		.type	= "Goodix-TS",
		.addr 	= 0x55,
		.flags         = 0,
		.irq           = RK30_PIN4_PC2,
    },
#endif

#if defined (CONFIG_TOUCHSCREEN_NOVATEK)
    {
        .type           = "novatek-ts",
        .addr           = 0x09,
        .flags          = 0,
        .irq            = RK30_PIN4_PC2,
        .platform_data  = &novatek_info,
    },
#endif

#if defined(CONFIG_TOUCHSCREEN_GSLX680)
        {
	.type           = "gslX680",
	.addr           = 0x40,
        },

#endif

#if defined (CONFIG_TOUCHSCREEN_ZET6221)	
 	{		
		.type          ="zet6221_ts",		
		.addr          =0x76,		
		.flags          =0,		
		.irq		=RK30_PIN4_PC2,	
	},
#endif

#if defined (CONFIG_TOUCHSCREEN_FT5306)
        {
            .type           = "ft5x0x_ts",
            .addr           = 0x38,
            .flags          = 0,
            .irq            = RK30_PIN4_PC2,
            .platform_data  = &ft5x0x_info,
        },
#endif

#if defined (CONFIG_LS_CM3217)
	{
		.type          = "lightsensor",
		.addr          = 0x10,
		.flags         = 0,
		.platform_data = &cm3217_info,
	},
#endif
};
#endif

#ifdef CONFIG_I2C3_RK30
static struct i2c_board_info __initdata i2c3_info[] = {
};
#endif

#ifdef CONFIG_I2C4_RK30
static struct i2c_board_info __initdata i2c4_info[] = {
};
#endif

#ifdef CONFIG_I2C_GPIO_RK30
#define I2C_SDA_PIN     INVALID_GPIO// RK30_PIN2_PD6   //set sda_pin here
#define I2C_SCL_PIN     INVALID_GPIO//RK30_PIN2_PD7   //set scl_pin here
static int rk30_i2c_io_init(void)
{
        //set iomux (gpio) here
        //rk30_mux_api_set(GPIO2D7_I2C1SCL_NAME, GPIO2D_GPIO2D7);
        //rk30_mux_api_set(GPIO2D6_I2C1SDA_NAME, GPIO2D_GPIO2D6);

        return 0;
}
struct i2c_gpio_platform_data default_i2c_gpio_data = {
       .sda_pin = I2C_SDA_PIN,
       .scl_pin = I2C_SCL_PIN,
       .udelay = 5, // clk = 500/udelay = 100Khz
       .timeout = 100,//msecs_to_jiffies(100),
       .bus_num    = 5,
       .io_init = rk30_i2c_io_init,
};
static struct i2c_board_info __initdata i2c_gpio_info[] = {
};
#endif

static void __init rk30_i2c_register_board_info(void)
{
#ifdef CONFIG_I2C0_RK30
	i2c_register_board_info(0, i2c0_info, ARRAY_SIZE(i2c0_info));
#endif
#ifdef CONFIG_I2C1_RK30
	i2c_register_board_info(1, i2c1_info, ARRAY_SIZE(i2c1_info));
#endif
#ifdef CONFIG_I2C2_RK30
	i2c_register_board_info(2, i2c2_info, ARRAY_SIZE(i2c2_info));
#endif
#ifdef CONFIG_I2C3_RK30
	i2c_register_board_info(3, i2c3_info, ARRAY_SIZE(i2c3_info));
#endif
#ifdef CONFIG_I2C4_RK30
	i2c_register_board_info(4, i2c4_info, ARRAY_SIZE(i2c4_info));
#endif
#ifdef CONFIG_I2C_GPIO_RK30
	i2c_register_board_info(5, i2c_gpio_info, ARRAY_SIZE(i2c_gpio_info));
#endif
}
//end of i2c

#if defined (CONFIG_KP_AXP20)
extern  void axp_power_off(void);
#endif

#ifdef CONFIG_SND_SOC_RT5633
extern void rt5633_shutdown(void);
#endif

#ifdef  CONFIG_KP_AXP20
#define POWER_ON_PIN RK30_PIN6_PB2   //power_hold
void rk30_pm_power_off(void)
#else
#define POWER_ON_PIN RK30_PIN6_PB0   //power_hold
static void rk30_pm_power_off(void)
#endif
{
	printk(KERN_ERR "rk30_pm_power_off start...\n");
#ifdef CONFIG_SND_SOC_RT5633
	rt5633_shutdown();
#endif
	gpio_direction_output(POWER_ON_PIN, GPIO_LOW);
#if defined(CONFIG_MFD_WM831X)
	if(g_pmic_type == PMIC_TYPE_WM8326)
	{
	wm831x_set_bits(Wm831x,WM831X_GPIO_LEVEL,0x0001,0x0000);  //set sys_pwr 0
	wm831x_device_shutdown(Wm831x);//wm8326 shutdown
	}
	#endif
	#if defined(CONFIG_MFD_TPS65910)
	if(g_pmic_type == PMIC_TYPE_TPS65910)
	{
		tps65910_device_shutdown();//tps65910 shutdown
	}
	#endif

#ifdef CONFIG_KP_AXP20
	axp_power_off();
#endif
	while (1);
}

static void __init machine_rk30_board_init(void)
{
	avs_init();
	gpio_request(POWER_ON_PIN, "poweronpin");
	gpio_direction_output(POWER_ON_PIN, GPIO_HIGH);
	
	pm_power_off = rk30_pm_power_off;
	
	rk30_i2c_register_board_info();
	spi_register_board_info(board_spi_devices, ARRAY_SIZE(board_spi_devices));
	platform_add_devices(devices, ARRAY_SIZE(devices));
	board_usb_detect_init(RK30_PIN6_PA3);

#ifdef CONFIG_WIFI_CONTROL_FUNC
	rk29sdk_wifi_bt_gpio_control_init();
#endif
}

static void __init rk30_reserve(void)
{
#ifdef CONFIG_ION
	rk30_ion_pdata.heaps[0].base = board_mem_reserve_add("ion", ION_RESERVE_SIZE);
#endif
#ifdef CONFIG_FB_ROCKCHIP
	resource_fb[0].start = board_mem_reserve_add("fb0", RK30_FB0_MEM_SIZE);
	resource_fb[0].end = resource_fb[0].start + RK30_FB0_MEM_SIZE - 1;
	resource_fb[1].start = board_mem_reserve_add("ipp buf", RK30_FB0_MEM_SIZE);
	resource_fb[1].end = resource_fb[1].start + RK30_FB0_MEM_SIZE - 1;
	resource_fb[2].start = board_mem_reserve_add("fb2", RK30_FB0_MEM_SIZE);
	resource_fb[2].end = resource_fb[2].start + RK30_FB0_MEM_SIZE - 1;
#endif
#ifdef CONFIG_VIDEO_RK29
	rk30_camera_request_reserve_mem();
#endif
	board_mem_reserved();
}

/**
 * dvfs_cpu_logic_table: table for arm and logic dvfs 
 * @frequency	: arm frequency
 * @cpu_volt	: arm voltage depend on frequency
 * @logic_volt	: logic voltage arm requests depend on frequency
 * comments	: min arm/logic voltage
 */
#ifdef  CONFIG_KP_AXP20
static struct dvfs_arm_table dvfs_cpu_logic_table[] = {
	{.frequency = 252 * 1000,	.cpu_volt = 1075 * 1000,	.logic_volt = 1125 * 1000},//0.975V/1.000V
	{.frequency = 504 * 1000,	.cpu_volt = 1100 * 1000,	.logic_volt = 1125 * 1000},//0.975V/1.000V
	{.frequency = 816 * 1000,	.cpu_volt = 1200 * 1000,	.logic_volt = 1200 * 1000},//1.000V/1.025V
	{.frequency = 1008 * 1000,	.cpu_volt = 1225 * 1000,	.logic_volt = 1200 * 1000},//1.025V/1.050V
	{.frequency = 1200 * 1000,	.cpu_volt = 1275 * 1000,	.logic_volt = 1250 * 1000},//1.100V/1.050V
	{.frequency = 1272 * 1000,	.cpu_volt = 1275 * 1000,	.logic_volt = 1250 * 1000},//1.150V/1.100V
	{.frequency = 1416 * 1000,	.cpu_volt = 1350 * 1000,	.logic_volt = 1250 * 1000},//1.225V/1.100V
	{.frequency = 1512 * 1000,	.cpu_volt = 1400 * 1000,	.logic_volt = 1300 * 1000},//1.300V/1.150V
	{.frequency = 1608 * 1000,	.cpu_volt = 1425 * 1000,	.logic_volt = 1300 * 1000},//1.325V/1.175V
	{.frequency = CPUFREQ_TABLE_END},
};
#else
static struct dvfs_arm_table dvfs_cpu_logic_table[] = {
	{.frequency = 252 * 1000,	.cpu_volt = 1075 * 1000,	.logic_volt = 1125 * 1000},//0.975V/1.000V
	{.frequency = 504 * 1000,	.cpu_volt = 1100 * 1000,	.logic_volt = 1125 * 1000},//0.975V/1.000V
	{.frequency = 816 * 1000,	.cpu_volt = 1125 * 1000,	.logic_volt = 1150 * 1000},//1.000V/1.025V
	{.frequency = 1008 * 1000,	.cpu_volt = 1125 * 1000,	.logic_volt = 1150 * 1000},//1.025V/1.050V
	{.frequency = 1200 * 1000,	.cpu_volt = 1175 * 1000,	.logic_volt = 1200 * 1000},//1.100V/1.050V
	{.frequency = 1272 * 1000,	.cpu_volt = 1225 * 1000,	.logic_volt = 1200 * 1000},//1.150V/1.100V
	{.frequency = 1416 * 1000,	.cpu_volt = 1300 * 1000,	.logic_volt = 1200 * 1000},//1.225V/1.100V
	{.frequency = 1512 * 1000,	.cpu_volt = 1350 * 1000,	.logic_volt = 1250 * 1000},//1.300V/1.150V
	{.frequency = 1608 * 1000,	.cpu_volt = 1425 * 1000,	.logic_volt = 1300 * 1000},//1.325V/1.175V
	{.frequency = CPUFREQ_TABLE_END},
};
#endif

static struct cpufreq_frequency_table dvfs_gpu_table[] = {
	{.frequency = 266 * 1000,	.index = 1050 * 1000},
	{.frequency = 400 * 1000,	.index = 1275 * 1000},
	{.frequency = CPUFREQ_TABLE_END},
};

static struct cpufreq_frequency_table dvfs_ddr_table[] = {
	{.frequency = 300 * 1000,	.index = 1050 * 1000},
	{.frequency = 400 * 1000,	.index = 1125 * 1000},
	{.frequency = CPUFREQ_TABLE_END},
};

#define DVFS_CPU_TABLE_SIZE	(ARRAY_SIZE(dvfs_cpu_logic_table))
static struct cpufreq_frequency_table cpu_dvfs_table[DVFS_CPU_TABLE_SIZE];
static struct cpufreq_frequency_table dep_cpu2core_table[DVFS_CPU_TABLE_SIZE];

void __init board_clock_init(void)
{
	rk30_clock_data_init(periph_pll_default, codec_pll_default, RK30_CLOCKS_DEFAULT_FLAGS);
	dvfs_set_arm_logic_volt(dvfs_cpu_logic_table, cpu_dvfs_table, dep_cpu2core_table);
	dvfs_set_freq_volt_table(clk_get(NULL, "gpu"), dvfs_gpu_table);
	dvfs_set_freq_volt_table(clk_get(NULL, "ddr"), dvfs_ddr_table);
}

MACHINE_START(RK30, "RK30board")
	.boot_params	= PLAT_PHYS_OFFSET + 0x800,
	.fixup		= rk30_fixup,
	.reserve	= &rk30_reserve,
	.map_io		= rk30_map_io,
	.init_irq	= rk30_init_irq,
	.timer		= &rk30_timer,
	.init_machine	= machine_rk30_board_init,
MACHINE_END
