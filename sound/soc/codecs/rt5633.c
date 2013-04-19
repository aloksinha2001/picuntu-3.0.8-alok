#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/pm.h>
#include <linux/i2c.h>
#include <linux/platform_device.h>
#include <linux/spi/spi.h>
#include <sound/core.h>
#include <sound/pcm.h>
#include <sound/pcm_params.h>
#include <sound/soc.h>
#include <sound/soc-dapm.h>
#include <sound/initval.h>

#include <mach/board.h>
#include <mach/gpio.h>

#include "rt5633.h"
#define VERSION_rt5633 "alsa 1.0.21 version 0.5"
static const u16 rt5633_reg[0x80];
static unsigned int BLCK_FREQ=32;	//32fs,bitclk is 32 fs
module_param(BLCK_FREQ, int, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
MODULE_PARM_DESC(BLCK_FREQ, "relationship between bclk and fs");

#define SPK_CON		INVALID_GPIO

#define VIRTUAL_POWER_CONTROL 0x90

//******************************************************************************************************
//thread to check speaker or hp and enable related EQ
//******************************************************************************************************
#define RT5633_SPK_TIMER	1	//if enable this, MUST enable RT5633_EQ_FUNC_ENA and RT5633_EQ_FUNC_SEL==RT5633_EQ_FOR_MANUAL first!

#if (RT5633_SPK_TIMER == 1)

static struct timer_list spk_timer;
struct work_struct  spk_work;
static bool last_is_spk = false;	// need modify.

#endif

//*******************************************************************************************************
//ALC setting
//*******************************************************************************************************
#define RT5633_ALC_FUNC_ENA 	0

#if RT5633_ALC_FUNC_ENA

#define	RT5633_ALC_DISABLE	0
#define	RT5633_ALC_FOR_DAC	1
#define	RT5633_ALC_FOR_ADC	2
#define	RT5633_ALC_FOR_MANUAL	3

#define RT5633_ALC_FUNC_SEL RT5633_ALC_FOR_ADC

#endif

//*******************************************************************************************************
//EQ setting
//*******************************************************************************************************
#define RT5633_EQ_FUNC_ENA  	1

#if RT5633_EQ_FUNC_ENA

#define	RT5633_EQ_DISABLE 	0
#define	RT5633_EQ_FOR_DAC 	1
#define	RT5633_EQ_FOR_ADC 	2
#define	RT5633_EQ_FOR_MANUAL 	3

#define RT5633_EQ_FUNC_SEL RT5633_EQ_FOR_MANUAL

#endif
//********************************************************************************************************
//static unsigned int reg90;
static int rt5633_write(struct snd_soc_codec *codec, unsigned int reg, unsigned int val);
static unsigned int rt5633_read(struct snd_soc_codec *codec, unsigned int reg);
#define rt5633_reset(c) rt5633_write(c, RT5633_RESET, 0)
#define rt5633_write_mask(c, reg, val, mask) snd_soc_update_bits(c, reg, mask, val)
static int rt5633_reg_init(struct snd_soc_codec *codec);

struct snd_soc_codec *rt5633_codec;

static void rt5633_work(struct work_struct *work);

static struct workqueue_struct *rt5633_workq;
static DECLARE_DELAYED_WORK(delayed_work, rt5633_work);

static unsigned int rt5633_read(struct snd_soc_codec *codec, unsigned int reg)
{
	return snd_soc_read(codec, reg);
}

static int rt5633_write(struct snd_soc_codec *codec, unsigned int reg, unsigned int val)
{
	int ret =-1;

	ret = snd_soc_write(codec, reg, val);
	if(ret <0)
		printk("wirte error-----> reg == %d, val == %d\n",reg,val);

	return ret;
}

struct rt5633_init_reg{
	u8 reg;
	u16 val;
};

static int rt5633_write_index(struct snd_soc_codec *codec, unsigned int index,unsigned int value)
{
    unsigned char RetVal = 0;   

    RetVal = rt5633_write(codec,RT5633_PRI_REG_ADD,index);
    
    if(RetVal != 0)
      return RetVal;
    
    RetVal = rt5633_write(codec,RT5633_PRI_REG_DATA,value);
    return RetVal;    
}

unsigned int rt5633_read_index(struct snd_soc_codec *codec, unsigned int reg)
{
	unsigned int value = 0x0;
	rt5633_write(codec,0x6a,reg);
	value=rt5633_read(codec,0x6c);	
	
	return value;
}

void rt5633_write_index_mask(struct snd_soc_codec *codec, unsigned int reg,unsigned int value,unsigned int mask)
{
	unsigned  int CodecData;

	if(!mask)
		return; 

	if(mask!=0xffff)
	 {
		CodecData=rt5633_read_index(codec,reg);		
		CodecData&=~mask;
		CodecData|=(value&mask);
		rt5633_write_index(codec,reg,CodecData);
	 }		
	else
	{
		rt5633_write_index(codec,reg,value);
	}
}


static struct rt5633_init_reg init_list[] = {
	{RT5633_SPK_OUT_VOL		, 0x8000},//speaker output volume is 0db by default
	{RT5633_SPK_HP_MIXER_CTRL	, 0x0020},//HP from HP_VOL	
	{RT5633_HP_OUT_VOL 		, 0xc0c0},//HP output volume is 0 db by default
	{RT5633_AUXOUT_VOL		, 0x0010},//Auxout volume is 0db by default
	{RT5633_REC_MIXER_CTRL		, 0x7d7d},//ADC Record Mixer Control
	{RT5633_MIC_CTRL_2		, 0x5500},//boost 20db
	{RT5633_HPMIXER_CTRL		, 0x3e3e},//"HP Mixer Control"
//	{RT5633_AUXMIXER_CTRL		, 0x3e3e},//"AUX Mixer Control"
	{RT5633_SPKMIXER_CTRL		, 0x08fc},//"SPK Mixer Control"
	{RT5633_ZC_SM_CTRL_1		, 0x0001},//Disable Zero Cross
	{RT5633_ZC_SM_CTRL_2		, 0x3000},//Disable Zero cross
	{RT5633_MIC_CTRL_1       	, 0x8808}, //set mic1 to differnetial mode	
	{RT5633_DEPOP_CTRL_2		, 0xB000},
	{RT5633_SPK_AMP_CTRL		, 0x0000}, //set to class AB
	{RT5633_GEN_PUR_CTRL_1		, 0x8C00}, //set spkratio to auto  
	{RT5633_PRI_REG_ADD		, 0x0056},
	{RT5633_PRI_REG_DATA		, 0x303f},
//JD setting	
   	{RT5633_ZC_SM_CTRL_1    	, 0x04b0},   
   	{RT5633_ZC_SM_CTRL_2        	, 0x3000},
	{RT5633_JACK_DET_CTRL           , 0x6980},		
};

#define RT5633_INIT_REG_LEN 	ARRAY_SIZE(init_list)

//*******************************************************************************************************************
//ALC setting function
//*******************************************************************************************************************
/*
static void rt5633_alc_enable(struct snd_soc_codec *codec,unsigned int EnableALC)
{
	if(EnableALC)
	{
		rt5633_write(codec, 0x64,0x0a0f);
		rt5633_write(codec, 0x65,0x0005);
		rt5633_write(codec, 0x66,0xe080);
		
	}
	else
	{
		rt5633_write_mask(codec, 0x66,0x0000,0x4000); //disable ALC		
	}		
}
*/
//*******************************************************************************************************************
//EQ parameter
static enum
{
	NORMAL=0,
	CLUB,
	DANCE,
	LIVE,	
	POP,
	ROCK,
	OPPO,
	TREBLE,
	BASS,
	RECORD,	
	HFREQ,
	SPK_FR,	
};

typedef struct  _HW_EQ_PRESET
{
	u16 	HwEqType;
	u16 	EqValue[22];
	u16  HwEQCtrl;

}HW_EQ_PRESET;


static HW_EQ_PRESET HwEq_Preset1[]={
/*       0x0    0x1    0x2     0x3   0x4    0x5	   0x6	  0x7	0x8     0x9    0xa    0xb    0xc    0x0d   0x0e    0x0f	   0x10  0x11  0x12   0x13    0x14  0x15    0x70*/
{NORMAL,{0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0003,0x0000,0x0000,0x0000},0x0000},			
{CLUB  ,{0x1C10,0x0000,0xC1CC,0x1E5D,0x0699,0xCD48,0x188D,0x0699,0xC3B6,0x1CD0,0x0699,0x0436,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0003,0x0000,0x0000,0x0000},0x000E},
{DANCE ,{0x1F2C,0x095B,0xC071,0x1F95,0x0616,0xC96E,0x1B11,0xFC91,0xDCF2,0x1194,0xFAF2,0x0436,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0003,0x0000,0x0000,0x0000},0x000F},
{LIVE  ,{0x1EB5,0xFCB6,0xC24A,0x1DF8,0x0E7C,0xC883,0x1C10,0x0699,0xDA41,0x1561,0x0295,0x0436,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0003,0x0000,0x0000,0x0000},0x000F},
{POP   ,{0x1EB5,0xFCB6,0xC1D4,0x1E5D,0x0E23,0xD92E,0x16E6,0xFCB6,0x0000,0x0969,0xF988,0x0436,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0003,0x0000,0x0000,0x0000},0x000F},
{ROCK  ,{0x1EB5,0xFCB6,0xC071,0x1F95,0x0424,0xC30A,0x1D27,0xF900,0x0C5D,0x0FC7,0x0E23,0x0436,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0003,0x0000,0x0000,0x0000},0x000F},
{OPPO  ,{0x0000,0x0000,0xCA4A,0x17F8,0x0FEC,0xCA4A,0x17F8,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0003,0x0000,0x0000,0x0000},0x000F},
{TREBLE,{0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x188D,0x1699,0x0000,0x0000,0x0000,0x0000,0x0000,0x0003,0x0000,0x0000,0x0000},0x0010},
{BASS  ,{0x1A43,0x0C00,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0003,0x0000,0x0000,0x0000},0x0001},
{RECORD,{0x1E3C,0xF405,0xC1E0,0x1E39,0x2298,0xDF29,0x0701,0x1D18,0xF34B,0x0CA9,0xF805,0xF9CC,0xF405,0x0000,0x0000,0x0000,0x0000,0x0000,0x0003,0x0000,0x0000,0x0000},0x001F},
{HFREQ, {0x1C10,0x0000,0xC883,0x1C10,0x0000,0xD588,0x1C10,0x0000,0xE904,0x1C10,0x0000,0xFBCA,0x0699,0x05E9,0x1A97,0x1D2B,0x0000,0x0003,0x0007,0x0388,0x15FF,0xFE43},0x0050},
{SPK_FR,{0x1CD4,0xF405,0xe0bf,0x0f9e,0xfa19,0x07CA,0x12AF,0xF805,0xE904,0x1C10,0x0000,0x1C8B,0x0000,0xc5e1,0x1afb,0x1d46,0x0000,0x0000,0x0003,0x0000,0x0000,0x0000},0x0090},
	
};
//*******************************************************************************************************************
#define EQ_DEFAULT_PRESET BASS
//*******************************************************************************************************************
static void rt5633_update_eqmode(struct snd_soc_codec *codec, int mode)
{
	u16 HwEqIndex=0;

		if(mode==RECORD)
		{	 
			rt5633_write_mask(codec, 0x3a,0x1800,0x1800); //enable ADC power
		}
		else
		{	 
			rt5633_write_mask(codec, 0x3a,0x0700,0x0700);//enable DAC power
		}		

	if(mode==NORMAL)
	{
		/*clear EQ parameter*/
		for(HwEqIndex=0;HwEqIndex<=0x15;HwEqIndex++)
		{
			rt5633_write_index(codec, HwEqIndex, HwEq_Preset1[mode].EqValue[HwEqIndex]);
		}
		
		rt5633_write(codec, 0x70,0x0);		/*disable EQ block*/
	}
	else
	{		
		rt5633_write(codec, 0x70,HwEq_Preset1[mode].HwEQCtrl);

		/*Fill EQ parameter*/
		for(HwEqIndex=0;HwEqIndex<=0x15;HwEqIndex++)
		{
			rt5633_write_index(codec, HwEqIndex, HwEq_Preset1[mode].EqValue[HwEqIndex]);
		}		
		
		if(mode==RECORD)
		{	//for ADC update EQ 
			rt5633_write_mask(codec, 0x6e,0xa000,0xa000);
		}
		else
		{	//for DAC update EQ 
			rt5633_write_mask(codec, 0x6e,0x2000,0xa000);
		}	
	}
}
//*******************************************************************************************************************

#if (RT5633_SPK_TIMER == 1)

static void spk_work_handler(struct work_struct *work)
{
	struct snd_soc_codec *codec = rt5633_codec;
	bool is_spk = (rt5633_read(codec, 0x4a)) & 0x10;	//detect rt5633 reg4a[4], 1'b:SPK, 0'b:HP ;
	
	if(last_is_spk != is_spk)
		printk("%s---%s is in use.last is %s in use\n", __FUNCTION__,is_spk?"speaker":"headphone",last_is_spk?"speaker":"headphone");
		
	if(is_spk && !last_is_spk){

#if (RT5633_EQ_FUNC_SEL==RT5633_EQ_FOR_MANUAL) 		

		rt5633_update_eqmode(codec, NORMAL);		// SPK is in use, enable EQ mode of NORMAL.
		
#endif		
		
	}else if(!is_spk && last_is_spk){

#if (RT5633_EQ_FUNC_SEL==RT5633_EQ_FOR_MANUAL)
		
		rt5633_update_eqmode(codec,HFREQ);		

#endif
		
	}
	last_is_spk = is_spk;
}

/* timer to judge SPK or HP in use, and handle EQ issues accordingly. */
void spk_timer_callback(unsigned long data )
{	
	int ret = 0;

	schedule_work(&spk_work);

  ret = mod_timer(&spk_timer, jiffies + msecs_to_jiffies(1000));
  if (ret) printk("Error in mod_timer\n");
}

#endif


static int rt5633_reg_init(struct snd_soc_codec *codec)
{
	int i;
	for (i = 0; i < RT5633_INIT_REG_LEN; i ++) {
		rt5633_write(codec, init_list[i].reg, init_list[i].val);	
	}	
	
	return 0;
}

struct _coeff_div{
	unsigned int mclk;       //pllout or MCLK
	unsigned int bclk;       //master mode
	unsigned int rate;
	unsigned int reg_val;
};
/*PLL divisors*/
struct _pll_div {
	u32 pll_in;
	u32 pll_out;
	u16 regvalue;
};

static const struct _pll_div codec_master_pll_div[] = {
	
	{  2048000,  8192000,	0x0ea0},
	{  3686400,  8192000,	0x4e27},
	{ 12000000,  8192000,	0x456b},
	{ 13000000,  8192000,	0x495f},
	{ 13100000,	 8192000,	0x0320},
	{  2048000,  11289600,	0xf637},
	{  3686400,  11289600,	0x2f22},
	{ 12000000,  11289600,	0x3e2f},
	{ 13000000,  11289600,	0x4d5b},
	{ 13100000,	 11289600,	0x363b},
	{  2048000,  16384000,	0x1ea0},
	{  3686400,  16384000,	0x9e27},
	{ 12000000,  16384000,	0x452b},
	{ 13000000,  16384000,	0x542f},
	{ 13100000,	 16384000,	0x03a0},
	{  2048000,  16934400,	0xe625},
	{  3686400,  16934400,	0x9126},
	{ 12000000,  16934400,	0x4d2c},
	{ 13000000,  16934400,	0x742f},
	{ 13100000,	 16934400,	0x3c27},
	{  2048000,  22579200,	0x2aa0},
	{  3686400,  22579200,	0x2f20},
	{ 12000000,  22579200,	0x7e2f},
	{ 13000000,  22579200,	0x742f},
	{ 13100000,	 22579200,	0x3c27},
	{  2048000,  24576000,	0x2ea0},
	{  3686400,  24576000,	0xee27},
	{ 12000000,  24576000,	0x2915},
	{ 13000000,  24576000,	0x772e},
	{ 13100000,	 24576000,	0x0d20},
	{ 26000000,  24576000,	0x2027},
	{ 26000000,  22579200,	0x392f},
	{ 24576000,  22579200,	0x0921},
	{ 24576000,  24576000,	0x02a0},
};

static const struct _pll_div codec_slave_pll_div[] = {
	
	{  1024000,  16384000,  0x3ea0},	
	{  1411200,  22579200,	0x3ea0},
	{  1536000,	 24576000,	0x3ea0},	
	{  2048000,  16384000,  0x1ea0},	
	{  2822400,  22579200,	0x1ea0},
	{  3072000,	 24576000,	0x1ea0},
	{	705600,  11289600, 	0x3ea0},
	{	705600,   8467200, 	0x3ab0},
			
};

struct _coeff_div rt5633_coeff_div[] = {

	//sysclk is 256fs
	{ 2048000,  8000 * 32,  8000, 0x1000},
	{ 2048000,  8000 * 64,  8000, 0x0000},
	{ 2822400, 11025 * 32, 11025, 0x1000},
	{ 2822400, 11025 * 64, 11025, 0x0000},
	{ 4096000, 16000 * 32, 16000, 0x1000},
	{ 4096000, 16000 * 64, 16000, 0x0000},
	{ 5644800, 22050 * 32, 22050, 0x1000},
	{ 5644800, 22050 * 64, 22050, 0x0000},
	{ 8192000, 32000 * 32, 32000, 0x1000},
	{ 8192000, 32000 * 64, 32000, 0x0000},
	{11289600, 44100 * 32, 44100, 0x1000},
	{11289600, 44100 * 64, 44100, 0x0000},
	{12288000, 48000 * 32, 48000, 0x1000},
	{12288000, 48000 * 64, 48000, 0x0000},
	//sysclk is 512fs
	{ 4096000,  8000 * 32,  8000, 0x3000},
	{ 4096000,  8000 * 64,  8000, 0x2000},
	{ 5644800, 11025 * 32, 11025, 0x3000},
	{ 5644800, 11025 * 64, 11025, 0x2000},
	{ 8192000, 16000 * 32, 16000, 0x3000},
	{ 8192000, 16000 * 64, 16000, 0x2000},
	{11289600, 22050 * 32, 22050, 0x3000},
	{11289600, 22050 * 64, 22050, 0x2000},
	{16384000, 32000 * 32, 32000, 0x3000},
	{16384000, 32000 * 64, 32000, 0x2000},
	{22579200, 44100 * 32, 44100, 0x3000},
	{22579200, 44100 * 64, 44100, 0x2000},
	{24576000, 48000 * 32, 48000, 0x3000},
	{24576000, 48000 * 64, 48000, 0x2000},
	//SYSCLK is 22.5792Mhz or 24.576Mhz(8k to 48k)	
	{24576000, 48000 * 32, 48000, 0x3000},
	{24576000, 48000 * 64, 48000, 0x2000},
	{22579200, 44100 * 32, 44100, 0x3000},
	{22579200, 44100 * 64, 44100, 0x2000},
	{24576000, 32000 * 32, 32000, 0x1080},
	{24576000, 32000 * 64, 32000, 0x0080},
	{22579200, 22050 * 32, 22050, 0x5000},
	{22579200, 22050 * 64, 22050, 0x4000},	
	{24576000, 16000 * 32, 16000, 0x3080},
	{24576000, 16000 * 64, 16000, 0x2080},	
	{22579200, 11025 * 32, 11025, 0x7000},
	{22579200, 11025 * 64, 11025, 0x6000},	
	{24576000, 	8000 * 32, 	8000, 0x7080},
	{24576000, 	8000 * 64, 	8000, 0x6080},	

};


static int get_coeff(int mclk, int rate, int timesofbclk)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(rt5633_coeff_div); i++) {
		if ((rt5633_coeff_div[i].mclk == mclk) 
				&& (rt5633_coeff_div[i].rate == rate)
				&& ((rt5633_coeff_div[i].bclk / rt5633_coeff_div[i].rate) == timesofbclk))
				return i;
	}

		return -1;
}

static int get_coeff_in_slave_mode(int mclk, int rate)
{
	return get_coeff(mclk, rate, BLCK_FREQ);
}

static int get_coeff_in_master_mode(int mclk, int rate)
{
	return get_coeff(mclk, rate ,BLCK_FREQ);	
}

static int rt5633_hifi_pcm_params(struct snd_pcm_substream *substream, 
			struct snd_pcm_hw_params *params,
			struct snd_soc_dai *dai)
{
	struct snd_soc_codec *codec = dai->codec;
	struct rt5633_priv *rt5633 = snd_soc_codec_get_drvdata(codec);
	unsigned int iface = 0;
	int rate = params_rate(params);
	int coeff = 0;

	printk(KERN_DEBUG "enter %s\n", __func__);	
	if (!rt5633->master)
		coeff = get_coeff_in_slave_mode(rt5633->sysclk, rate);
	else
		coeff = get_coeff_in_master_mode(rt5633->sysclk, rate);
	if (coeff < 0) {
		printk(KERN_ERR "%s get_coeff err!\n", __func__);
	//	return -EINVAL;
	}
	switch (params_format(params))
	{
		case SNDRV_PCM_FORMAT_S16_LE:
			break;
		case SNDRV_PCM_FORMAT_S20_3LE:
			iface |= 0x0004;
			break;
		case SNDRV_PCM_FORMAT_S24_LE:
			iface |= 0x0008;
			break;
		case SNDRV_PCM_FORMAT_S8:
			iface |= 0x000c;
			break;
		default:
			return -EINVAL;
	}
	
	rt5633_write_mask(codec, RT5633_SDP_CTRL, iface, SDP_I2S_DL_MASK);
	rt5633_write(codec, RT5633_STEREO_AD_DA_CLK_CTRL, rt5633_coeff_div[coeff].reg_val);
	rt5633_write_mask(codec, RT5633_PWR_MANAG_ADD1, 0x81C0, 0x81C0);

	return 0;
}

static int rt5633_hifi_codec_set_dai_fmt(struct snd_soc_dai *codec_dai, unsigned int fmt)
{
	struct snd_soc_codec *codec = codec_dai->codec;
	struct rt5633_priv *rt5633 = snd_soc_codec_get_drvdata(codec);
	u16 iface = 0;
	
	printk(KERN_DEBUG "enter %s\n", __func__);	
	switch (fmt & SND_SOC_DAIFMT_MASTER_MASK) {
	case SND_SOC_DAIFMT_CBM_CFM:
		rt5633->master = 1;
		break;
	case SND_SOC_DAIFMT_CBS_CFS:
		iface |= (0x0001 << 15);
		rt5633->master = 0;
		break;
	default:
		return -EINVAL;
	}
	
	switch (fmt & SND_SOC_DAIFMT_FORMAT_MASK) {
	case SND_SOC_DAIFMT_I2S:
		break;
	case SND_SOC_DAIFMT_LEFT_J:
		iface |= (0x0001);
		break;
	case SND_SOC_DAIFMT_DSP_A:
		iface |= (0x0002);
		break;
	case SND_SOC_DAIFMT_DSP_B:
		iface  |= (0x0003);
		break;
	default:
		return -EINVAL;
	}
	
	switch (fmt & SND_SOC_DAIFMT_INV_MASK) {
	case SND_SOC_DAIFMT_NB_NF:
		break;
	case SND_SOC_DAIFMT_IB_NF:
		iface |= (0x0001 << 7);
		break;
	default:
		return -EINVAL;	
	}
	
	rt5633_write(codec, RT5633_SDP_CTRL, iface);
	return 0;
}
static int rt5633_hifi_codec_set_dai_sysclk(struct snd_soc_dai *codec_dai, 
		int clk_id, unsigned int freq, int dir)
{
	struct snd_soc_codec *codec = codec_dai->codec;
	struct rt5633_priv *rt5633 = snd_soc_codec_get_drvdata(codec);
	
	printk(KERN_DEBUG "enter %s\n", __func__);	
	if ((freq >= (256 * 8000)) && (freq <= (512 * 48000))) {
		rt5633->sysclk = freq;
		return 0;	
	}
	
	printk(KERN_ERR "unsupported sysclk freq %u for audio i2s\n", freq);
	return 0;
}

static int rt5633_codec_set_dai_pll(struct snd_soc_dai *codec_dai, 
		int pll_id, int source,	unsigned int freq_in, unsigned int freq_out)
{
	int i;
	int ret = -EINVAL;
	struct snd_soc_codec *codec = codec_dai->codec;	
	struct rt5633_priv *rt5633 = snd_soc_codec_get_drvdata(codec);
	
	printk(KERN_DEBUG "enter %s\n", __func__);	
	rt5633_write_mask(codec, RT5633_PWR_MANAG_ADD2, 0, PWR_PLL);

	if (!freq_in || !freq_out)
		return 0;
		
	if (rt5633->master) {
		for (i = 0; i < ARRAY_SIZE(codec_master_pll_div); i ++) {
			if ((freq_in == codec_master_pll_div[i].pll_in) && (freq_out == codec_master_pll_div[i].pll_out)) {
				rt5633_write(codec, RT5633_PLL_CTRL, codec_master_pll_div[i].regvalue);
				rt5633_write_mask(codec, RT5633_PWR_MANAG_ADD2, PWR_PLL, PWR_PLL);
				schedule_timeout_uninterruptible(msecs_to_jiffies(20));
				rt5633_write(codec, RT5633_GBL_CLK_CTRL, 0x0000);	
				ret = 0;
			}
		}	
	} else {
		for (i = 0; i < ARRAY_SIZE(codec_slave_pll_div); i ++) {
			if ((freq_in == codec_slave_pll_div[i].pll_in) && (freq_out == codec_slave_pll_div[i].pll_out))  {
				rt5633_write(codec, RT5633_PLL_CTRL, codec_slave_pll_div[i].regvalue);
				rt5633_write_mask(codec, RT5633_PWR_MANAG_ADD2, PWR_PLL, PWR_PLL);
				schedule_timeout_uninterruptible(msecs_to_jiffies(20));
				rt5633_write(codec, RT5633_GBL_CLK_CTRL, 0x6000);
				ret = 0;
			}
		}
	}
	
	return 0;
}

static int rt5633_set_bias_level(struct snd_soc_codec *codec, enum snd_soc_bias_level level);
void rt5633_shutdown(void)
{
    printk("%s\n", __func__);
	rt5633_set_bias_level(rt5633_codec, SND_SOC_BIAS_OFF);
}
EXPORT_SYMBOL(rt5633_shutdown);


static void rt5633_hifi_shutdown(struct snd_pcm_substream *substream,
		struct snd_soc_dai *codec_dai)
{

	
}

#define RT5633_STEREO_RATES		(SNDRV_PCM_RATE_44100)

#define RT5633_FORMAT					(SNDRV_PCM_FMTBIT_S16_LE |\
			SNDRV_PCM_FMTBIT_S20_3LE |\
			SNDRV_PCM_FMTBIT_S24_LE |\
			SNDRV_PCM_FMTBIT_S8)
			
struct snd_soc_dai_ops rt5633_ops = {
			.hw_params = rt5633_hifi_pcm_params,
			.set_fmt = rt5633_hifi_codec_set_dai_fmt,
			.set_sysclk = rt5633_hifi_codec_set_dai_sysclk,
			.set_pll = rt5633_codec_set_dai_pll,
			.shutdown = rt5633_hifi_shutdown,	
};

			
struct snd_soc_dai_driver rt5633_dai = { 
		.name = "RT5633 HiFi",
		.id = 1,
		.playback = {
			.stream_name = "HIFI Playback",
			.channels_min = 1,
			.channels_max = 2,
			.rates = RT5633_STEREO_RATES,
			.formats = RT5633_FORMAT,	
		}	,
		.capture = {
			.stream_name = "HIFI Capture",
			.channels_min = 1,
			.channels_max = 2,
			.rates = RT5633_STEREO_RATES,	
			.formats = RT5633_FORMAT,	
		},

		.ops =&rt5633_ops,
};


static u32 StrtoInt(const char *str)
{
	u32 i,CodecValue=0;
		
	printk("strtoin=%s \n",str);
	if(!str)
	{
		printk("%s(): NULL pointer input\n", __FUNCTION__);
		return -1;
	}
	for(i=0; *str; str++)
	{
		if((*str>='A' && *str<='F')||(*str>='a' && *str<='f')||(*str>='0' && *str<='9'))
		{
			 CodecValue*=0x10;
			if(*str>='A' && *str<='F')
				CodecValue += *str-'A'+10;
			else if(*str>='a' && *str<='f')
				CodecValue += *str-'a'+10;
			else if(*str>='0' && *str<='9')
				CodecValue += *str-'0';				
		}
		else
				return CodecValue;
	}
	return CodecValue;
}


enum
{
	WRITE_REG=0,
	READ_REG,
	WRITE_INDEX,
	READ_INDEX,
	BAD_FORMAT,	
};

static u32 last_command=0;
static u32 read_codec_reg=0;
static u32 read_codec_value=0;

static ssize_t rt5633_codec_reg_show(struct device *dev,struct device_attribute *attr ,char *buf)
{
	if(last_command==READ_REG)
	{
		return sprintf(buf,"%04x\n",read_codec_value);
	}
	if(last_command==READ_INDEX)
	{
		return sprintf(buf,"%04x\n",read_codec_value);
	}	
	
	return sprintf(buf,"read fail\n");
}


static ssize_t rt5633_codec_reg_store(struct device *dev,struct device_attribute *attr, const char *buf,
 size_t count)
{
	const char * p=buf;
	u32 reg=0, val=0;
	printk("store buf=%s \n",buf);	

	if(!strncmp(buf, "readr", strlen("readr")))
	{
		p+=strlen("readr");
		read_codec_reg=(u32)StrtoInt(p);
		read_codec_value=rt5633_read(rt5633_codec, read_codec_reg);
		last_command=READ_REG;
		printk("%s(): get 0x%04x=0x%04x\n", __FUNCTION__, read_codec_reg, val);
	}
	else if(!strncmp(buf, "writer", strlen("writer")))
	{
		p+=strlen("writer");
		reg=(u32)StrtoInt(p);
		p=strchr(buf, '=');
		if(p)
		{
			++ p;
			val=(u32)StrtoInt(p);
			rt5633_write(rt5633_codec, reg, val);
			last_command=WRITE_REG;
			printk("%s(): set 0x%04x=0x%04x\n", __FUNCTION__, reg, val);
		}
		else
		{
			last_command=BAD_FORMAT;
			printk("%s(): Bad string format input!\n", __FUNCTION__);
		}	
	}
	else if(!strncmp(buf, "writei", strlen("writei")))
	{
		p+=strlen("writei");
		reg=(u32)StrtoInt(p);
		p=strchr(buf, '=');
		if(p)
		{
			++ p;
			val=(u32)StrtoInt(p);
			rt5633_write(rt5633_codec, 0x6a, reg);
			rt5633_write(rt5633_codec, 0x6c, val);
			last_command=WRITE_INDEX;
			printk("%s(): set 0x%04x=0x%04x\n", __FUNCTION__, reg, val);
		}
		else
		{
			last_command=BAD_FORMAT;
			printk("%s(): Bad string format input!\n", __FUNCTION__);
		}	
	}
	else if(!strncmp(buf, "readi", strlen("readi")))
	{
		p+=strlen("readi");
		read_codec_reg=(u32)StrtoInt(p);
		rt5633_write(rt5633_codec, 0x6a, read_codec_reg);	
		read_codec_value=rt5633_read(rt5633_codec, 0x6c);
		last_command=READ_INDEX;
		printk("%s(): get 0x%04x=0x%04x\n", __FUNCTION__, read_codec_reg, val);
	}
	else
	{
		last_command=BAD_FORMAT;
		printk("%s(): Bad string format input!\n", __FUNCTION__);
	}	
	
	return count;
} 

static DEVICE_ATTR(rt_codec_reg, 0666, rt5633_codec_reg_show, rt5633_codec_reg_store);


static int rt5633_set_bias_level(struct snd_soc_codec *codec, enum snd_soc_bias_level level)
{
	printk(KERN_DEBUG "enter %s\n", __func__);

	switch (level) {
	case SND_SOC_BIAS_ON:
	case SND_SOC_BIAS_PREPARE:
		//SPK open
		rt5633_write_mask(codec, RT5633_SPK_OUT_VOL, 0x0000, 0xE000);
		rt5633_write_mask(codec, RT5633_PWR_MANAG_ADD1, 0x3E20, 0x3E20);		

		//HP open
		rt5633_write_mask(codec,RT5633_DEPOP_CTRL_1,PW_SOFT_GEN|EN_SOFT_FOR_S_M_DEPOP|
				EN_HP_R_M_UM_DEPOP|EN_HP_L_M_UM_DEPOP,
				PW_SOFT_GEN|EN_SOFT_FOR_S_M_DEPOP|EN_HP_R_M_UM_DEPOP|EN_HP_L_M_UM_DEPOP);
		rt5633_write_mask(codec, RT5633_HP_OUT_VOL, 0x0000, 0x8080);
		mdelay(80);
		rt5633_write_mask(codec,RT5633_DEPOP_CTRL_1,0,PW_SOFT_GEN|EN_SOFT_FOR_S_M_DEPOP|
				EN_HP_R_M_UM_DEPOP|EN_HP_L_M_UM_DEPOP);

		rt5633_write_mask(codec, RT5633_PWR_MANAG_ADD3,PWR_VREF|PWR_MAIN_BIAS|PWR_HP_AMP_DRI|PWR_HP_DIS_DEPOP|PWR_HP_AMP,
			PWR_VREF|PWR_MAIN_BIAS|PWR_HP_AMP_DRI|PWR_HP_DIS_DEPOP|PWR_HP_AMP);
		rt5633_write_mask(codec, RT5633_PWR_MANAG_ADD2,0xcc39, 0xcc39);
		rt5633_write_mask(codec, RT5633_PWR_MANAG_ADD4, 0xcc00, 0xcc00);
		break;
	case SND_SOC_BIAS_STANDBY:
		break;
	case SND_SOC_BIAS_OFF:
		rt5633_write_mask(codec, RT5633_SPK_OUT_VOL, 0x8080, 0x8080);	//mute speaker volume
		rt5633_write_mask(codec, RT5633_HP_OUT_VOL, 0x8080, 0x8080);	//mute hp volume
		rt5633_write(codec, RT5633_PWR_MANAG_ADD1, 0x0000);
		rt5633_write(codec, RT5633_PWR_MANAG_ADD2, 0x0000);
		rt5633_write(codec, RT5633_PWR_MANAG_ADD3, 0x0000);
		rt5633_write(codec, RT5633_PWR_MANAG_ADD4, 0x0000);
		break;
	}
	
	codec->dapm.bias_level = level;
	return 0;
}

static void rt5633_work(struct work_struct *work)
{
	struct snd_soc_codec *codec = rt5633_codec;
	
	rt5633_set_bias_level(codec, codec->dapm.bias_level);	
}

static int rt5633_probe(struct snd_soc_codec *codec)
{
	int ret;
    
	ret = snd_soc_codec_set_cache_io(codec, 8, 16, SND_SOC_I2C);
	if (ret != 0) {
		dev_err(codec->dev, "Failed to set cache I/O: %d\n", ret);
		return ret;
	}
		
	codec->cache_bypass = 1; 

	rt5633_reset(codec);

	rt5633_write_mask(codec, RT5633_PWR_MANAG_ADD3,PWR_VREF|PWR_MAIN_BIAS, PWR_VREF|PWR_MAIN_BIAS);
	schedule_timeout_uninterruptible(msecs_to_jiffies(110));

	rt5633_set_bias_level(codec, SND_SOC_BIAS_PREPARE);
	codec->dapm.bias_level = SND_SOC_BIAS_STANDBY;

	rt5633_reg_init(codec);


#if (RT5633_SPK_TIMER == 1)
	/* Timer module installing */
	setup_timer( &spk_timer, spk_timer_callback, 0 );
	ret = mod_timer( &spk_timer, jiffies + msecs_to_jiffies(5000) );
	if (ret)
		printk("Error in mod_timer\n");

	INIT_WORK(&spk_work, spk_work_handler);
#endif	

	rt5633_workq = create_freezable_workqueue("rt5633");
	if (rt5633_workq == NULL) {
		printk("rt5633_probe::create_freezeable_workqueue ERROR !");
		kfree(codec);
		return -ENOMEM;
	}

	queue_delayed_work(rt5633_workq, &delayed_work,
		jiffies + msecs_to_jiffies(500)); 

	rt5633_codec = codec;

	if(SPK_CON != INVALID_GPIO){
		ret = gpio_request(SPK_CON, "spk_con");
		if(ret < 0){
			printk("gpio request spk_con error!\n");
		}
		else{
			gpio_direction_output(SPK_CON, GPIO_HIGH);
			gpio_set_value(SPK_CON, GPIO_HIGH);
		}
	}

	return 0;
}

static int rt5633_remove(struct snd_soc_codec *codec)
{
	rt5633_set_bias_level(codec, SND_SOC_BIAS_OFF);

	cancel_delayed_work_sync(&delayed_work);
	return 0;
}

static int rt5633_suspend(struct snd_soc_codec *codec, pm_message_t state)
{
	rt5633_set_bias_level(codec, SND_SOC_BIAS_OFF);
	return 0;
}

static int rt5633_resume(struct snd_soc_codec *codec)
{

	rt5633_write_mask(codec, RT5633_PWR_MANAG_ADD3,PWR_VREF|PWR_MAIN_BIAS, PWR_VREF|PWR_MAIN_BIAS);
	schedule_timeout_uninterruptible(msecs_to_jiffies(110));
	rt5633_reg_init(codec);
	rt5633_write_mask(codec, RT5633_PWR_MANAG_ADD1, 0x81C0, 0x81C0);	

#if (RT5633_SPK_TIMER == 1)

		last_is_spk = !last_is_spk;	//wired~, update eqmode right here by spk_timer.
		
#endif	
	
	if (codec->dapm.suspend_bias_level == SND_SOC_BIAS_ON) {
		rt5633_set_bias_level(codec, SND_SOC_BIAS_PREPARE);
		codec->dapm.bias_level = SND_SOC_BIAS_ON;
		queue_delayed_work(rt5633_workq, &delayed_work,
			jiffies + msecs_to_jiffies(500));
	}
	return 0;
}


void codec_set_spk(bool on)
{
	struct snd_soc_codec *codec = rt5633_codec;

	printk("%s: %d\n", __func__, on);

	if(!codec)
		return;

	if(on){
		printk("snd_soc_dapm_enable_pin\n");
		//snd_soc_dapm_enable_pin(&codec->dapm, "Headphone Jack");
		//snd_soc_dapm_enable_pin(&codec->dapm, "Ext Spk");
		codec->dapm.suspend_bias_level = SND_SOC_BIAS_ON;
		rt5633_resume(codec);
	}
	else{

		printk("snd_soc_dapm_disable_pin\n");
		//snd_soc_dapm_disable_pin(&codec->dapm, "Headphone Jack");
		//snd_soc_dapm_disable_pin(&codec->dapm, "Ext Spk");
		rt5633_set_bias_level(codec, SND_SOC_BIAS_OFF);
	}

	//snd_soc_dapm_sync(&codec->dapm);

	return;
}
EXPORT_SYMBOL(codec_set_spk);


struct snd_soc_codec_driver soc_codec_dev_rt5633 = {
	.probe = 	rt5633_probe,
	.remove = rt5633_remove,
	.suspend = rt5633_suspend,
	.resume = rt5633_resume,
	.set_bias_level = rt5633_set_bias_level,
	.reg_cache_size = ARRAY_SIZE(rt5633_reg),
	.reg_word_size = sizeof(u16),
	.reg_cache_default = rt5633_reg,
	.reg_cache_step = 1,
};
