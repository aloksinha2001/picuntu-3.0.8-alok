#ifndef __LINUX_AXP_MFD_H_
#define __LINUX_AXP_MFD_H_

/* Unified sub device IDs for AXP */
enum {
	AXP_ID_LDO1,
	AXP_ID_LDO2,
	AXP_ID_LDO3,
	AXP_ID_LDO4,

	AXP_ID_BUCK1,
	AXP_ID_BUCK2,
	AXP_ID_BUCK3,

	AXP_ID_LDOIO0,

	AXP_ID_SUPPLY,
};

/* AXP battery charger data */
struct power_supply_info;

struct axp_supply_init_data {
	/* battery parameters */
	struct power_supply_info *battery_info;

	/* current and voltage to use for battery charging */
	unsigned int chgcur;
	unsigned int chgearcur;
	unsigned int chgsuscur;
	unsigned int chgclscur;
	unsigned int chgvol;
	unsigned int chgend;
	
	/* charger control */
	bool limit_on;
	
	/* charger time */
	unsigned int chgpretime;
	unsigned int chgcsttime;

	/*adc frequency*/
	unsigned int adc_freq;
	
};

struct axp_funcdev_info {
	int		id;
	const char	*name;
	void	*platform_data;
};

struct axp_platform_data {
	int num_regl_devs;
	int num_sply_devs;
	
	struct axp_funcdev_info *regl_devs;
	struct axp_funcdev_info *sply_devs;
};

struct axp_mfd_chip {
	struct i2c_client	*client;
	struct device		*dev;
	struct axp_mfd_chip_ops	*ops;

	int		type;
	uint64_t	irqs_enabled;

	struct mutex		lock;
	struct work_struct	irq_work;

	struct blocking_notifier_head notifier_list;
};

struct axp_mfd_chip_ops {
	int	(*init_chip)(struct axp_mfd_chip *);
	int	(*enable_irqs)(struct axp_mfd_chip *, uint64_t irqs);
	int	(*disable_irqs)(struct axp_mfd_chip *, uint64_t irqs);
	int	(*read_irqs)(struct axp_mfd_chip *, uint64_t *irqs);
	int	(*clear_irqs)(struct axp_mfd_chip *, uint64_t irqs);
};

#define AXP18                      			18
#define POWER18_STATUS            			(0x00)
#define POWER18_IPS_SET             		(0x01)
#define POWER18_ONOFF               		(0x02)
#define POWER18_CHARGE1             		(0x03)
#define POWER18_CHARGE2             		(0x04)
#define POWER18_PEK                 		(0x05)
#define POWER18_INTEN1              		(0x06)
#define POWER18_INTEN2              		(0x07)
#define POWER18_INTEN3              		(0x08)
#define POWER18_INTSTS1             		(0x09)
#define POWER18_INTSTS2             		(0x0A)
#define POWER18_INTSTS3             		(0x0B)
#define POWER18_VENDER_USED1        		(0x0C)
#define POWER18_DCDCCTL             		(0x0D)
#define POWER18_DC12OUT_VOL         		(0x0E)
#define POWER18_LDOOUT_VOL          		(0x0F)
#define POWER18_SW_CTL              		(0x10)
#define POWER18_BATTERY_VOL         		(0x11)
#define POWER18_BATTERY_CURRENT     		(0x12)
#define POWER18_DCIN_VOL            		(0x13)
#define POWER18_DCIN_CURRENT        		(0x14)
#define POWER18_ADCSW_CTL           		(0x15)
#define POWER18_VENDER_USED2        		(0x16)
#define POWER18_EPT_SW              		(0x17)
#define POWER18_DATA_BUFFER1        		(0x18)
#define POWER18_DATA_BUFFER2        		(0x19)
#define POWER18_VENDER_USED3        		(0x1A)


#define AXP19                       		19
#define POWER19_STATUS              		(0x00)
#define POWER19_MODE_CHGSTATUS      		(0x01)
#define POWER19_OTG_STATUS          		(0x02)
#define POWER19_IC_TYPE             		(0x03)
#define POWER19_DATA_BUFFER1        		(0x06)
#define POWER19_DATA_BUFFER2        		(0x07)
#define POWER19_DATA_BUFFER3        		(0x08)
#define POWER19_DATA_BUFFER4        		(0x09)
#define POWER19_VERSION             		(0x0C)
#define POWER19_LDO3_DC2_CTL        		(0x10)
#define POWER19_LDO24_DC13_CTL      		(0x12)
#define POWER19_DC2OUT_VOL          		(0x23)
#define POWER19_LDO3_DC2_DVM        		(0x25)
#define POWER19_DC1OUT_VOL          		(0x26)
#define POWER19_DC3OUT_VOL          		(0x27)
#define POWER19_LDO24OUT_VOL        		(0x28)
#define POWER19_LDO3OUT_VOL         		(0x29)
#define POWER19_IPS_SET             		(0x30)
#define POWER19_VOFF_SET            		(0x31)
#define POWER19_OFF_CTL             		(0x32)
#define POWER19_CHARGE1             		(0x33)
#define POWER19_CHARGE2             		(0x34)
#define POWER19_BACKUP_CHG          		(0x35)
#define POWER19_POK_SET             		(0x36)
#define POWER19_DCDC_FREQSET        		(0x37)
#define POWER19_VLTF_CHGSET         		(0x38)
#define POWER19_VHTF_CHGSET         		(0x39)
#define POWER19_APS_WARNING1        		(0x3A)
#define POWER19_APS_WARNING2        		(0x3B)
#define POWER19_VLTF_DISCHGSET      		(0x3C)
#define POWER19_VHTF_DISCHGSET      		(0x3D)
#define POWER19_DCDC_MODESET        		(0x80)
#define POWER19_VOUT_MONITOR        		(0x81)
#define POWER19_ADC_EN1             		(0x82)
#define POWER19_ADC_EN2             		(0x83)
#define POWER19_ADC_SPEED           		(0x84)
#define POWER19_ADC_INPUTRANGE      		(0x85)
#define POWER19_TIMER_CTL           		(0x8A)
#define POWER19_VBUS_DET_SRP        		(0x8B)
#define POWER19_HOTOVER_CTL         		(0x8F)
#define POWER19_GPIO0_CTL           		(0x90)
#define POWER19_GPIO0_VOL           		(0x91)
#define POWER19_GPIO1_CTL           		(0x92)
#define POWER19_GPIO2_CTL           		(0x93)
#define POWER19_GPIO012_SIGNAL      		(0x94)
#define POWER19_SENSE_CTL           		(0x95)
#define POWER19_SENSE_SIGNAL        		(0x96)
#define POWER19_GPIO20_PDCTL        		(0x97)
#define POWER19_PWM1_FREQ           		(0x98)
#define POWER19_PWM1_DUTYDE         		(0x99)
#define POWER19_PWM1_DUTY           		(0x9A)
#define POWER19_PWM2_FREQ           		(0x9B)
#define POWER19_PWM2_DUTYDE         		(0x9C)
#define POWER19_PWM2_DUTY           		(0x9D)
#define POWER19_RSTO_CTL            		(0x9E)
#define POWER19_GPIO67_CTL          		(0x9F)
#define POWER19_INTEN1              		(0x40)
#define POWER19_INTEN2              		(0x41)
#define POWER19_INTEN3              		(0x42)
#define POWER19_INTEN4              		(0x43)
#define POWER19_INTSTS1             		(0x44)
#define POWER19_INTSTS2             		(0x45)
#define POWER19_INTSTS3             		(0x46)
#define POWER19_INTSTS4             		(0x47)
#define POWER19_GPIO67_CFG          		(0xE0)

/*axp 19 adc data register*/
#define POWER19_BAT_AVERVOL_H8      		(0x78)
#define POWER19_BAT_AVERVOL_L4      		(0x79)
#define POWER19_BAT_AVERCHGCUR_H8   		(0x7A)
#define POWER19_BAT_AVERCHGCUR_L5   		(0x7B)
#define POWER19_ACIN_VOL_H8         		(0x56)
#define POWER19_ACIN_VOL_L4         		(0x57)
#define POWER19_ACIN_CUR_H8         		(0x58)
#define POWER19_ACIN_CUR_L4         		(0x59)
#define POWER19_VBUS_VOL_H8         		(0x5A)
#define POWER19_VBUS_VOL_L4         		(0x5B)
#define POWER19_VBUS_CUR_H8         		(0x5C)
#define POWER19_VBUS_CUR_L4         		(0x5D)
#define POWER19_BAT_AVERDISCHGCUR_H8		(0x7C)
#define POWER19_BAT_AVERDISCHGCUR_L5		(0x7D)
#define POWER19_APS_AVERVOL_H8      		(0x7E)
#define POWER19_APS_AVERVOL_L4      		(0x7F)
#define POWER19_BAT_CHGCOULOMB3     		(0xB0)
#define POWER19_BAT_CHGCOULOMB2     		(0xB1)
#define POWER19_BAT_CHGCOULOMB1     		(0xB2)
#define POWER19_BAT_CHGCOULOMB0     		(0xB3)
#define POWER19_BAT_DISCHGCOULOMB3  		(0xB4)
#define POWER19_BAT_DISCHGCOULOMB2  		(0xB5)
#define POWER19_BAT_DISCHGCOULOMB1  		(0xB6)
#define POWER19_BAT_DISCHGCOULOMB0  		(0xB7)
#define POWER19_COULOMB_CTL         		(0xB8)
#define POWER19_BAT_POWERH8         		(0x70)
#define POWER19_BAT_POWERM8         		(0x71)
#define POWER19_BAT_POWERL8         		(0x72)

#define AXP20                  				20
#define POWER20_STATUS         				(0x00)
#define POWER20_MODE_CHGSTATUS 				(0x01)
#define POWER20_OTG_STATUS     				(0x02)
#define POWER20_IC_TYPE        				(0x03)
#define POWER20_DATA_BUFFER1   				(0x04)
#define POWER20_DATA_BUFFER2   				(0x05)
#define POWER20_DATA_BUFFER3   				(0x06)
#define POWER20_DATA_BUFFER4   				(0x07)
#define POWER20_DATA_BUFFER5   				(0x08)
#define POWER20_DATA_BUFFER6   				(0x09)
#define POWER20_DATA_BUFFER7   				(0x0A)
#define POWER20_DATA_BUFFER8   				(0x0B)
#define POWER20_DATA_BUFFER9   				(0x0C)
#define POWER20_DATA_BUFFERA   				(0x0D)
#define POWER20_DATA_BUFFERB   				(0x0E)
#define POWER20_DATA_BUFFERC   				(0x0F)
#define POWER20_LDO234_DC23_CTL				(0x12)
#define POWER20_DC2OUT_VOL     				(0x23)
#define POWER20_LDO3_DC2_DVM   				(0x25)
#define POWER20_DC3OUT_VOL     				(0x27)
#define POWER20_LDO24OUT_VOL   				(0x28)
#define POWER20_LDO3OUT_VOL    				(0x29)
#define POWER20_IPS_SET        				(0x30)
#define POWER20_VOFF_SET       				(0x31)
#define POWER20_OFF_CTL        				(0x32)
#define POWER20_CHARGE1        				(0x33)
#define POWER20_CHARGE2        				(0x34)
#define POWER20_BACKUP_CHG     				(0x35)
#define POWER20_PEK_SET        				(0x36)
#define POWER20_DCDC_FREQSET   				(0x37)
#define POWER20_VLTF_CHGSET    				(0x38)
#define POWER20_VHTF_CHGSET    				(0x39)
#define POWER20_APS_WARNING1   				(0x3A)
#define POWER20_APS_WARNING2   				(0x3B)
#define POWER20_TLTF_DISCHGSET 				(0x3C)
#define POWER20_THTF_DISCHGSET 				(0x3D)
#define POWER20_DCDC_MODESET   				(0x80)
#define POWER20_ADC_EN1        				(0x82)
#define POWER20_ADC_EN2        				(0x83)
#define POWER20_ADC_SPEED      				(0x84)
#define POWER20_ADC_INPUTRANGE 				(0x85)
#define POWER20_ADC_IRQ_RETFSET				(0x86)
#define POWER20_ADC_IRQ_FETFSET				(0x87)
#define POWER20_TIMER_CTL      				(0x8A)
#define POWER20_VBUS_DET_SRP   				(0x8B)
#define POWER20_HOTOVER_CTL					(0x8F)
#define POWER20_GPIO0_CTL      				(0x90)
#define POWER20_GPIO0_VOL      				(0x91)
#define POWER20_GPIO1_CTL      				(0x92)
#define POWER20_GPIO2_CTL      				(0x93)
#define POWER20_GPIO012_SIGNAL 				(0x94)
#define POWER20_GPIO3_CTL      				(0x95)
#define POWER20_INTEN1         				(0x40)
#define POWER20_INTEN2         				(0x41)
#define POWER20_INTEN3         				(0x42)
#define POWER20_INTEN4         				(0x43)
#define POWER20_INTEN5         				(0x44)
#define POWER20_INTSTS1        				(0x48)
#define POWER20_INTSTS2        				(0x49)
#define POWER20_INTSTS3        				(0x4A)
#define POWER20_INTSTS4        				(0x4B)
#define POWER20_INTSTS5        				(0x4C)

/*axp 20 adc data register*/
#define POWER20_BAT_AVERVOL_H8          	(0x78)
#define POWER20_BAT_AVERVOL_L4          	(0x79)
#define POWER20_BAT_AVERCHGCUR_H8       	(0x7A)
#define POWER20_BAT_AVERCHGCUR_L5       	(0x7B)
#define POWER20_ACIN_VOL_H8             	(0x56)
#define POWER20_ACIN_VOL_L4             	(0x57)
#define POWER20_ACIN_CUR_H8             	(0x58)
#define POWER20_ACIN_CUR_L4             	(0x59)
#define POWER20_VBUS_VOL_H8             	(0x5A)
#define POWER20_VBUS_VOL_L4             	(0x5B)
#define POWER20_VBUS_CUR_H8             	(0x5C)
#define POWER20_VBUS_CUR_L4             	(0x5D)

#define POWER20_BAT_AVERDISCHGCUR_H8    	(0x7C)
#define POWER20_BAT_AVERDISCHGCUR_L5    	(0x7D)
#define POWER20_APS_AVERVOL_H8          	(0x7E)
#define POWER20_APS_AVERVOL_L4          	(0x7F)
#define POWER20_BAT_CHGCOULOMB3         	(0xB0)
#define POWER20_BAT_CHGCOULOMB2         	(0xB1)
#define POWER20_BAT_CHGCOULOMB1         	(0xB2)
#define POWER20_BAT_CHGCOULOMB0         	(0xB3)
#define POWER20_BAT_DISCHGCOULOMB3      	(0xB4)
#define POWER20_BAT_DISCHGCOULOMB2      	(0xB5)
#define POWER20_BAT_DISCHGCOULOMB1      	(0xB6)
#define POWER20_BAT_DISCHGCOULOMB0      	(0xB7)
#define POWER20_COULOMB_CTL             	(0xB8)
#define POWER20_BAT_POWERH8             	(0x70)
#define POWER20_BAT_POWERM8             	(0x71)
#define POWER20_BAT_POWERL8             	(0x72)

/* Status Query Interface */
/*  AXP  */
#define AXP_STATUS_SOURCE					(1<< 0)
#define AXP_STATUS_ACUSBSH					(1<< 1)
#define AXP_STATUS_BATCURDIR				(1<< 2)
#define AXP_STATUS_USBLAVHO					(1<< 3)
#define AXP_STATUS_USBVA					(1<< 4)
#define AXP_STATUS_USBEN					(1<< 5)
#define AXP_STATUS_ACVA						(1<< 6)
#define AXP_STATUS_ACEN						(1<< 7)

#define AXP_STATUS_CHACURLOEXP				(1<<10)
#define AXP_STATUS_BATINACT					(1<<11)

#define AXP_STATUS_BATEN					(1<<13)
#define AXP_STATUS_INCHAR					(1<<14)
#define AXP_STATUS_ICTEMOV					(1<<15)

#if defined (CONFIG_KP_AXP20)
#define AXP_IC_TYPE							POWER20_IC_TYPE

#define AXP_INTEN1							POWER20_INTEN1 
#define AXP_INTEN2							POWER20_INTEN2 
#define AXP_INTEN3							POWER20_INTEN3 
#define AXP_INTEN4							POWER20_INTEN4 
#define AXP_INTEN5							POWER20_INTEN5 
#define AXP_INTSTS1							POWER20_INTSTS1
#define AXP_INTSTS2							POWER20_INTSTS2
#define AXP_INTSTS3							POWER20_INTSTS3
#define AXP_INTSTS4							POWER20_INTSTS4
#define AXP_INTSTS5							POWER20_INTSTS5

#define AXP_STATUS							POWER20_STATUS
#define AXP_MODE_CHGSTATUS					POWER20_MODE_CHGSTATUS
#define AXP_OFF_CTL							POWER20_OFF_CTL
#define AXP_COULOMB_CTL						POWER20_COULOMB_CTL
#define AXP_DATA_BUFFER0					POWER20_DATA_BUFFER1
#define AXP_DATA_BUFFERC					POWER20_DATA_BUFFERC
#define AXP_DATA_NUM						12
#endif

#if defined (CONFIG_KP_AXP19)
#define AXP_IC_TYPE							POWER19_IC_TYPE

#define AXP_INTEN1							POWER19_INTEN1 
#define AXP_INTEN2							POWER19_INTEN2 
#define AXP_INTEN3							POWER19_INTEN3 
#define AXP_INTEN4							POWER19_INTEN4 
#define AXP_INTSTS1							POWER19_INTSTS1
#define AXP_INTSTS2							POWER19_INTSTS2
#define AXP_INTSTS3							POWER19_INTSTS3
#define AXP_INTSTS4							POWER19_INTSTS4

#define AXP_STATUS							POWER19_STATUS
#define AXP_MODE_CHGSTATUS					POWER19_MODE_CHGSTATUS
#define AXP_OFF_CTL							POWER19_OFF_CTL
#define AXP_COULOMB_CTL						POWER19_COULOMB_CTL
#define AXP_DATA_BUFFERC					POWER19_STATUS
#define AXP_DATA_BUFFER0					POWER19_DATA_BUFFER1
#define AXP_DATA_NUM						4
#endif

extern struct device *axp_get_dev(void);
extern int axp_register_notifier(struct device *dev,
		struct notifier_block *nb, uint64_t irqs);
extern int axp_unregister_notifier(struct device *dev,
		struct notifier_block *nb, uint64_t irqs);

/* NOTE: the functions below are not intended for use outside
 * of the AXP sub-device drivers
 */
extern int axp_write(struct device *dev, int reg, uint8_t val);
extern int axp_writes(struct device *dev, int reg, int len, uint8_t *val);
extern int axp_read(struct device *dev, int reg, uint8_t *val);
extern int axp_reads(struct device *dev, int reg, int len, uint8_t *val);
extern int axp_update(struct device *dev, int reg, uint8_t val, uint8_t mask);
extern int axp_set_bits(struct device *dev, int reg, uint8_t bit_mask);
extern int axp_clr_bits(struct device *dev, int reg, uint8_t bit_mask);
extern struct i2c_client *axp;
extern struct device *axp_get_dev(void);
#endif /* __LINUX_PMIC_AXP_H */
