#ifndef	_LINUX_AXP_SPLY_H_
#define	_LINUX_AXP_SPLY_H_

#include "axp-cfg.h"
#include "axp-mfd.h"
/* AXP20 */
#if defined (CONFIG_KP_AXP20)
#define AXP_CHARGE_STATUS				POWER20_STATUS

#define AXP_CHARGE_CONTROL1				POWER20_CHARGE1
#define AXP_CHARGE_CONTROL2				POWER20_CHARGE2

#define AXP_ADC_CONTROL1				POWER20_ADC_EN1
#define AXP_ADC_BATVOL_ENABLE			(1 << 7)
#define AXP_ADC_BATCUR_ENABLE			(1 << 6)
#define AXP_ADC_DCINVOL_ENABLE			(1 << 5)
#define AXP_ADC_DCINCUR_ENABLE			(1 << 4)
#define AXP_ADC_USBVOL_ENABLE			(1 << 3)
#define AXP_ADC_USBCUR_ENABLE			(1 << 2)
#define AXP_ADC_APSVOL_ENABLE			(1 << 1)
#define AXP_ADC_TSVOL_ENABLE			(1 << 0)
#define AXP_ADC_CONTROL3				POWER20_ADC_SPEED

#define AXP_VACH_RES					POWER20_ACIN_VOL_H8
#define AXP_VACL_RES					POWER20_ACIN_VOL_L4
#define AXP_IACH_RES					POWER20_ACIN_CUR_H8
#define AXP_IACL_RES					POWER20_ACIN_CUR_L4
#define AXP_VUSBH_RES					POWER20_VBUS_VOL_H8
#define AXP_VUSBL_RES					POWER20_VBUS_VOL_L4
#define AXP_IUSBH_RES					POWER20_VBUS_CUR_H8
#define AXP_IUSBL_RES					POWER20_VBUS_CUR_L4

#define AXP_VBATH_RES					POWER20_BAT_AVERVOL_H8
#define AXP_VBATL_RES					POWER20_BAT_AVERVOL_L4
#define AXP_ICHARH_RES					POWER20_BAT_AVERCHGCUR_H8
#define AXP_ICHARL_RES					POWER20_BAT_AVERCHGCUR_L5
#define AXP_IDISCHARH_RES				POWER20_BAT_AVERDISCHGCUR_H8
#define AXP_IDISCHARL_RES				POWER20_BAT_AVERDISCHGCUR_L5


#define AXP_COULOMB_CONTROL				POWER20_COULOMB_CTL

#define AXP_CCHAR3_RES					POWER20_BAT_CHGCOULOMB3
#define AXP_CCHAR2_RES					POWER20_BAT_CHGCOULOMB2
#define AXP_CCHAR1_RES					POWER20_BAT_CHGCOULOMB1
#define AXP_CCHAR0_RES					POWER20_BAT_CHGCOULOMB0
#define AXP_CDISCHAR3_RES				POWER20_BAT_DISCHGCOULOMB3
#define AXP_CDISCHAR2_RES				POWER20_BAT_DISCHGCOULOMB2
#define AXP_CDISCHAR1_RES				POWER20_BAT_DISCHGCOULOMB1
#define AXP_CDISCHAR0_RES				POWER20_BAT_DISCHGCOULOMB0

#define AXP_DATA_BUFFER0				POWER20_DATA_BUFFER1
#define AXP_DATA_BUFFER1				POWER20_DATA_BUFFER2
#define AXP_DATA_BUFFER2				POWER20_DATA_BUFFER3
#define AXP_DATA_BUFFER3				POWER20_DATA_BUFFER4
#define AXP_DATA_BUFFER4				POWER20_DATA_BUFFER5
#define AXP_DATA_BUFFER5				POWER20_DATA_BUFFER6
#define AXP_DATA_BUFFER6				POWER20_DATA_BUFFER7
#define AXP_DATA_BUFFER7				POWER20_DATA_BUFFER8
#define AXP_DATA_BUFFER8				POWER20_DATA_BUFFER9
#define AXP_DATA_BUFFER9				POWER20_DATA_BUFFERA
#define AXP_DATA_BUFFERA				POWER20_DATA_BUFFERB
#define AXP_DATA_BUFFERB				POWER20_DATA_BUFFERC
#define AXP_IC_TYPE						POWER20_IC_TYPE

#define AXP_CAP							(0xB9)
#define AXP_RDC_BUFFER0					(0xBA)
#define AXP_RDC_BUFFER1					(0xBB)
#define AXP_OCV_BUFFER0					(0xBC)
#define AXP_OCV_BUFFER1					(0xBD)

#define AXP_HOTOVER_CTL					POWER20_HOTOVER_CTL
#define AXP_CHARGE_VBUS					POWER20_IPS_SET
#define AXP_APS_WARNING1				POWER20_APS_WARNING1
#define AXP_APS_WARNING2				POWER20_APS_WARNING2
#define AXP_TIMER_CTL					POWER20_TIMER_CTL
#define AXP_PEK_SET						POWER20_PEK_SET
#define AXP_DATA_NUM					12
#endif

/* AXP19 */
#if defined (CONFIG_KP_AXP19)
#define AXP_CHARGE_STATUS				POWER19_STATUS

#define AXP_CHARGE_CONTROL1				POWER19_CHARGE1
#define AXP_CHARGE_CONTROL2				POWER19_CHARGE2

#define AXP_ADC_CONTROL1				POWER19_ADC_EN1
#define AXP_ADC_BATVOL_ENABLE			(1 << 7)
#define AXP_ADC_BATCUR_ENABLE			(1 << 6)
#define AXP_ADC_DCINVOL_ENABLE			(1 << 5)
#define AXP_ADC_DCINCUR_ENABLE			(1 << 4)
#define AXP_ADC_USBVOL_ENABLE			(1 << 3)
#define AXP_ADC_USBCUR_ENABLE			(1 << 2)
#define AXP_ADC_APSVOL_ENABLE			(1 << 1)
#define AXP_ADC_TSVOL_ENABLE			(1 << 0)
#define AXP_ADC_CONTROL3				POWER19_ADC_SPEED

#define AXP_VACH_RES					POWER19_ACIN_VOL_H8
#define AXP_VACL_RES					POWER19_ACIN_VOL_L4
#define AXP_IACH_RES					POWER19_ACIN_CUR_H8
#define AXP_IACL_RES					POWER19_ACIN_CUR_L4
#define AXP_VUSBH_RES					POWER19_VBUS_VOL_H8
#define AXP_VUSBL_RES					POWER19_VBUS_VOL_L4
#define AXP_IUSBH_RES					POWER19_VBUS_CUR_H8
#define AXP_IUSBL_RES					POWER19_VBUS_CUR_L4

#define AXP_VBATH_RES					POWER19_BAT_AVERVOL_H8
#define AXP_VBATL_RES					POWER19_BAT_AVERVOL_L4
#define AXP_ICHARH_RES					POWER19_BAT_AVERCHGCUR_H8
#define AXP_ICHARL_RES					POWER19_BAT_AVERCHGCUR_L5
#define AXP_IDISCHARH_RES				POWER19_BAT_AVERDISCHGCUR_H8
#define AXP_IDISCHARL_RES				POWER19_BAT_AVERDISCHGCUR_L5


#define AXP_COULOMB_CONTROL				POWER19_COULOMB_CTL

#define AXP_CCHAR3_RES					POWER19_BAT_CHGCOULOMB3
#define AXP_CCHAR2_RES					POWER19_BAT_CHGCOULOMB2
#define AXP_CCHAR1_RES					POWER19_BAT_CHGCOULOMB1
#define AXP_CCHAR0_RES					POWER19_BAT_CHGCOULOMB0
#define AXP_CDISCHAR3_RES				POWER19_BAT_DISCHGCOULOMB3
#define AXP_CDISCHAR2_RES				POWER19_BAT_DISCHGCOULOMB2
#define AXP_CDISCHAR1_RES				POWER19_BAT_DISCHGCOULOMB1
#define AXP_CDISCHAR0_RES				POWER19_BAT_DISCHGCOULOMB0

#define AXP_DATA_BUFFER0				POWER19_DATA_BUFFER1
#define AXP_DATA_BUFFER1				POWER19_DATA_BUFFER2
#define AXP_DATA_BUFFER2				POWER19_DATA_BUFFER3
#define AXP_DATA_BUFFER3				POWER19_DATA_BUFFER4

#define AXP_IC_TYPE						POWER19_IC_TYPE

#define AXP_CAP							(0xB9)
#define AXP_RDC_BUFFER0					(POWER19_DATA_BUFFER3)
#define AXP_RDC_BUFFER1					(POWER19_DATA_BUFFER4)
#define AXP_OCV_BUFFER0					(0xBC)
#define AXP_OCV_BUFFER1					(0xBD)

#define AXP_HOTOVER_CTL					POWER19_HOTOVER_CTL
#define AXP_CHARGE_VBUS					POWER19_IPS_SET
#define AXP_APS_WARNING1				POWER19_APS_WARNING1
#define AXP_APS_WARNING2				POWER19_APS_WARNING2
#define AXP_TIMER_CTL					POWER19_TIMER_CTL
#define AXP_PEK_SET						POWER19_POK_SET
#define AXP_DATA_NUM					4
#endif

#define AXP_CHG_ATTR(_name)					\
{											\
	.attr = { .name = #_name,.mode = 0664 },\
	.show =  _name##_show,					\
	.store = _name##_store,					\
}

struct axp_adc_res {//struct change
	uint16_t vbat_res;
	uint16_t ibat_res;
	uint16_t ichar_res;
	uint16_t idischar_res;
	uint16_t vac_res;
	uint16_t iac_res;
	uint16_t vusb_res;
	uint16_t iusb_res;
};

struct axp_charger {
	/*power supply sysfs*/
	struct power_supply batt;
	struct power_supply	ac;
	struct power_supply	usb;
	struct power_supply bubatt;

	/*i2c device*/
	struct device *master;

	/* adc */
	struct axp_adc_res adc;
	unsigned int adc_freq;

	/*monitor*/
	struct delayed_work work;
	unsigned int interval;
#ifndef CONFIG_KP_USEIRQ
	struct delayed_work workstate;
	unsigned int intervalstate;
#endif

	/*battery info*/
	struct power_supply_info *battery_info;

	/*charger control*/
	bool chgen;
	bool limit_on;
	unsigned int chgcur;
	unsigned int chgearcur;
	unsigned int chgsuscur;
	unsigned int chgclscur;
	unsigned int chgvol;
	unsigned int chgend;

	/*charger time */
	int chgpretime;
	int chgcsttime;

	/*external charger*/
	bool chgexten;
	int chgextcur;

	/* charger status */
	bool bat_det;
	bool is_on;
	bool is_finish;
	bool ac_not_enough;
	bool ac_det;
	bool usb_det;
	bool ac_valid;
	bool usb_valid;
	bool ext_valid;
	bool bat_current_direction;
	bool in_short;
	bool batery_active;
	bool low_charge_current;
	bool int_over_temp;
	bool charge_on;

	int vbat;
	int ibat;
	int pbat;
	int vac;
	int iac;
	int vusb;
	int iusb;
	int ocv;
	
	int disvbat;
	int disibat;

	/*rest time*/
	int rest_cap;
	int ocv_rest_cap;
	int base_restcap;
	int rest_time;

	/*ic temperature*/
	int ic_temp;

	/*irq*/
	struct notifier_block nb;
};

static struct input_dev *powerkeydev;
static struct axp_charger *axpcharger;
#if defined (CONFIG_USE_OCV) 
static int Total_Cap = 0;
static int Cap_Index = 0;
static int cap_count = 0;
static int Bat_Cap_Buffer[AXP_VOL_MAX];
#endif
static int charge_index = 0;
static int state_count = 0;
static int rdc_count = 0;
#if defined (CONFIG_USE_COL) 
static int cap_count1 = 0,cap_count2 = 0,cap_count3 = 0,cap_count4 = 0;
#endif
static int axp_debug = 0;
#endif
