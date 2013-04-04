/*
 * rk29_rt5633.c  --  SoC audio for rockchip
 *
 * Driver for rockchip rt5633 audio
 *
 *  This program is free software; you can redistribute  it and/or modify it
 *  under  the terms of  the GNU General  Public License as published by the
 *  Free Software Foundation;  either version 2 of the  License, or (at your
 *  option) any later version.
 *
 *
 */

#include <linux/module.h>
#include <linux/device.h>
#include <linux/clk.h>
#include <linux/i2c.h>
#include <linux/slab.h>
#include <sound/core.h>
#include <sound/pcm.h>
#include <sound/soc.h>
#include <sound/soc-dapm.h>
#include <asm/io.h>
#include <mach/hardware.h>
#include "../codecs/rt5633.h"
#include "rk29_pcm.h"
#include "rk29_i2s.h"
#include <linux/hdmi.h>

#if 0
#define	DBG(x...)	printk(KERN_INFO x)
#else
#define	DBG(x...)
#endif

extern struct snd_soc_dai_driver rt5633_dai;
extern struct snd_soc_codec_driver soc_codec_dev_rt5633;

static int rk29_hw_params(struct snd_pcm_substream *substream,
	struct snd_pcm_hw_params *params)
{
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct snd_soc_dai *codec_dai = rtd->codec_dai;
	struct snd_soc_dai *cpu_dai = rtd->cpu_dai;
	unsigned int pll_out = 0; 
	unsigned int lrclk = 0;
	int ret;
          
        DBG("Enter::%s----%d\n",__FUNCTION__,__LINE__);    
        /*by Vincent Hsiung for EQ Vol Change*/
        #define HW_PARAMS_FLAG_EQVOL_ON 0x21
        #define HW_PARAMS_FLAG_EQVOL_OFF 0x22
        if ((params->flags == HW_PARAMS_FLAG_EQVOL_ON)||(params->flags == HW_PARAMS_FLAG_EQVOL_OFF))
        {
        	ret = codec_dai->driver->ops->hw_params(substream, params, codec_dai); //by Vincent
        	DBG("Enter::%s----%d\n",__FUNCTION__,__LINE__);
        }
        else
        {
                
                /* set codec DAI configuration */
                #if defined (CONFIG_SND_RK29_CODEC_SOC_SLAVE) 
                ret = snd_soc_dai_set_fmt(codec_dai, SND_SOC_DAIFMT_I2S |
                                SND_SOC_DAIFMT_NB_NF | SND_SOC_DAIFMT_CBS_CFS);
                #endif	
                #if defined (CONFIG_SND_RK29_CODEC_SOC_MASTER) 
                ret = snd_soc_dai_set_fmt(codec_dai, SND_SOC_DAIFMT_I2S |
                                SND_SOC_DAIFMT_NB_NF | SND_SOC_DAIFMT_CBM_CFM ); 
                #endif
                if (ret < 0)
                  return ret; 

                /* set cpu DAI configuration */
                #if defined (CONFIG_SND_RK29_CODEC_SOC_SLAVE) 
                ret = snd_soc_dai_set_fmt(cpu_dai, SND_SOC_DAIFMT_I2S |
                                SND_SOC_DAIFMT_NB_NF | SND_SOC_DAIFMT_CBM_CFM);
                #endif	
                #if defined (CONFIG_SND_RK29_CODEC_SOC_MASTER) 
                ret = snd_soc_dai_set_fmt(cpu_dai, SND_SOC_DAIFMT_I2S |
                                SND_SOC_DAIFMT_NB_NF | SND_SOC_DAIFMT_CBS_CFS);	
                #endif		
                if (ret < 0)
                  return ret;

        }


        switch(params_rate(params)) {
        case 8000:
        case 16000:
        case 24000:
        case 32000:
        case 48000:
                pll_out = 12288000;
                break;
        case 11025:
        case 22050:
        case 44100:
                pll_out = 11289600;
                break;
        default:
                DBG("Enter:%s, %d, Error rate=%d\n",__FUNCTION__,__LINE__,params_rate(params));
                return -EINVAL;
                break;
        }
        DBG("Enter:%s, %d, rate=%d\n",__FUNCTION__,__LINE__,params_rate(params));

		#if defined (CONFIG_SND_RK29_CODEC_SOC_SLAVE)
#if 0		//use pll from blck
          /*Set the pll of rt5633,the Pll source from BITCLK on CPU is master mode*/
         //bitclk is 64fs           
		    ret=snd_soc_dai_set_pll(codec_dai,0,params_rate(params)*64,pll_out);
		    if (ret < 0)
		    { 
		       DBG("rk29_hw_params_rt5633:failed to set the pll for codec side\n"); 
		  	   return ret;
		    }
#endif	    
		   /*Set the system clk for codec*/
		    ret=snd_soc_dai_set_sysclk(codec_dai, 0,pll_out,SND_SOC_CLOCK_IN);
		    if (ret < 0)
		    {
		       DBG("rk29_hw_params_rt5633:failed to set the sysclk for codec side\n"); 
		   	   return ret;
		   	}	    
		#endif
  
/*
        #if defined (CONFIG_SND_RK29_CODEC_SOC_MASTER) 

		   if((24576000%params_rate(params))==0)	//for 8k,16k,32k,48k
			{
				snd_soc_dai_set_pll(codec_dai,0,pll_out, 24576000);
				snd_soc_dai_set_sysclk(codec_dai,0, 24576000, SND_SOC_CLOCK_IN);			
			}
		   else if((22579200%params_rate(params))==0)	//for 11k,22k,44k
			{
				snd_soc_dai_set_pll(codec_dai,0,pll_out, 22579200);
				snd_soc_dai_set_sysclk(codec_dai,0, 22579200, SND_SOC_CLOCK_IN);			
			}
      
        #endif
*/

        #if defined (CONFIG_SND_RK29_CODEC_SOC_SLAVE)
        snd_soc_dai_set_sysclk(cpu_dai, 0, pll_out, 0);
        snd_soc_dai_set_clkdiv(cpu_dai, ROCKCHIP_DIV_BCLK, (pll_out/4)/params_rate(params)-1);
        snd_soc_dai_set_clkdiv(cpu_dai, ROCKCHIP_DIV_MCLK, 3);
        #endif

        DBG("Enter:%s, %d, LRCK=%d\n",__FUNCTION__,__LINE__,(pll_out/4)/params_rate(params));
        
        return 0;
}

static struct snd_soc_ops rk29_ops = {
	  .hw_params = rk29_hw_params,
};

static struct snd_soc_dai_link rk29_dai = {
	.name = "RT5633",
	.stream_name = "RT5633 PCM",
	.codec_name = "RT5633.0-001c",
	.platform_name = "rockchip-audio",
	.cpu_dai_name = "rk29_i2s.0",
	.codec_dai_name = "RT5633 HiFi",
	.ops = &rk29_ops,
};

static struct snd_soc_card snd_soc_card_rk29 = {
	.name = "RK29_RT5633",
	.dai_link = &rk29_dai,
	.num_links = 1,
};

static struct platform_device *rk29_snd_device;

static int audio_card_init(void)
{
	int ret =0;	
	
	//rk29_speaker = rk29_speaker_init(RK29_PIN6_PB6, GPIO_HIGH, 2, (200*1000*1000));

	DBG("Enter::%s----%d\n",__FUNCTION__,__LINE__);
	rk29_snd_device = platform_device_alloc("soc-audio", -1);
	if (!rk29_snd_device) {
		  DBG("platform device allocation failed\n");
		  ret = -ENOMEM;
		  return ret;
	}
	platform_set_drvdata(rk29_snd_device, &snd_soc_card_rk29);
	ret = platform_device_add(rk29_snd_device);
	if (ret) {
	    DBG("platform device add failed\n");
	    platform_device_put(rk29_snd_device);
		return ret;
	}
	return ret;
}

static void audio_card_exit(void)
{
	platform_device_unregister(rk29_snd_device);
}

#ifdef CONFIG_HDMI
bool rt5633SPKon=true;
extern struct snd_soc_codec *rt5633_codec;
static bool rt5633_codec_get_spk(void)
{
   

    if(rt5633SPKon)
    	return HDMI_ENABLE;
    else     	
	return HDMI_DISABLE;

    return HDMI_ENABLE;
}
void rt5633_codec_set_spk(bool on)
{
	struct snd_soc_codec *codec = rt5633_codec; 
	rt5633SPKon= on;
	if (on) {
		DBG("snd_soc_dapm_enable_pin\n");
		snd_soc_dapm_enable_pin(&codec->dapm, "Headphone Jack");
		snd_soc_dapm_enable_pin(&codec->dapm, "Ext Spk");
	} else {
		DBG("snd_soc_dapm_disable_pin\n");
		snd_soc_dapm_disable_pin(&codec->dapm, "Headphone Jack");
		snd_soc_dapm_disable_pin(&codec->dapm, "Ext Spk");
	}
}

static struct hdmi_codec_driver hdmi_codec_driver = {
    .hdmi_get_spk = rt5633_codec_get_spk,
    .hdmi_set_spk = rt5633_codec_set_spk,
};
#endif

static int rk29_rt5633_detect_device(struct i2c_client *i2c)
{
	u8 data[2]={0};
	unsigned int value=0x0;
	int ret;

	ret = i2c_master_reg8_recv(i2c, RT5633_VENDOR_ID1, data, 2,100 * 1000);   
	if(ret < 0){
		printk("%s: read RT5633_VENDOR_ID1 err!\n", __FUNCTION__);
		return -ENODEV;
	}

	value = (data[0]<<8) | data[1];
	if(value != 0x10ec){
		printk("%s: check vendor ID err!, id=0x%04x\n", __FUNCTION__, value);
		return -ENODEV;	
	}

	return 0;
}

static const struct i2c_device_id rt5633_i2c_id[] = {
	{ "rt5633", 0 },
	{ }
};
MODULE_DEVICE_TABLE(i2c, rt5633_i2c_id);

static int rt5633_i2c_probe(struct i2c_client *i2c,
		    const struct i2c_device_id *id)
{
	struct rt5633_priv *rt5633;
	int ret;
	
    printk("%s\n", __FUNCTION__);

	ret = rk29_rt5633_detect_device(i2c);
	if(ret < 0){
		printk("%s: detect alc5633 error!\n", __FUNCTION__);
		return ret;
	}

    audio_card_init();
    
	rt5633 = kzalloc(sizeof(struct rt5633_priv), GFP_KERNEL);
	if (NULL == rt5633)
		return -ENOMEM;

	i2c_set_clientdata(i2c, rt5633);

	ret = snd_soc_register_codec(&i2c->dev,
		&soc_codec_dev_rt5633, &rt5633_dai, 1);
	if (ret < 0)
		kfree(rt5633);

	#ifdef CONFIG_HDMI
	hdmi_codec_driver.name = i2c->name;
	ret = hdmi_codec_register(&hdmi_codec_driver);
    	if (ret != 0) {
		printk("Failed to register HDMI_codec: %d\n", ret);
		kfree(rt5633);

	}
	#endif


	return ret;
}

static __devexit int rt5633_i2c_remove(struct i2c_client *i2c)
{
	audio_card_exit();
	#ifdef CONFIG_HDMI
       hdmi_codec_unregister(&hdmi_codec_driver);  
       #endif
	
	snd_soc_unregister_codec(&i2c->dev);
	kfree(i2c_get_clientdata(i2c));
	return 0;
}

struct i2c_driver rt5633_i2c_driver = {
	.driver = {
		.name = "RT5633",
		.owner = THIS_MODULE,
	},
	.probe = rt5633_i2c_probe,
	.remove   = __devexit_p(rt5633_i2c_remove),
	.id_table = rt5633_i2c_id,
};

static int __init rt5633_modinit(void)
{
	return i2c_add_driver(&rt5633_i2c_driver);
}
module_init(rt5633_modinit);

static void __exit rt5633_modexit(void)
{
	i2c_del_driver(&rt5633_i2c_driver);
}
module_exit(rt5633_modexit);

/* Module information */
MODULE_AUTHOR("rockchip");
MODULE_DESCRIPTION("ROCKCHIP i2s ASoC Interface");
MODULE_LICENSE("GPL");
