#ifndef __LINUX_AXP_CFG_H_
#define __LINUX_AXP_CFG_H_

/*设备地址*/
/*
	一般不改变：
	AXP20:0x34
	AXP19:0x34
*/
#define	AXP_DEVICES_ADDR	(0x68 >> 1)
/*i2c控制器的设备号:具体看所使用平台硬件的连接，由于使用了自动探测，此处可以不设置，方案2、3需要配置*/
#define	AXP_I2CBUS			1
/*电源芯片对应的中断号：具体看所使用的平台硬件的连接，中断线nmi连接cpu的哪路irq或者gpio*/
#define AXP_IRQNO			RK30_PIN6_PA4

/*初始化各路输出，单位mV，0都为关闭*/
/*
	ldo1：
		由硬件决定输出电压，软件改不了，只是软件的显示电压，有两档：
		LDO1SET接地:	1300
		LDO1SET接VINT:	3300
*/
#define AXP_LDO1_VALUE		1300
/*
	ldo2：
		AXP20:1800~3300，100/step
		AXP19:1800~3300，100/step
*/
#define AXP_LDO2_VALUE		3000
/*
	ldo3：
		AXP20:700~3500，25/step
		AXP19:700~3500，25/step
*/
#define AXP_LDO3_VALUE		2800
/*
	ldo4：
		AXP20:1250/1300/1400/1500/1600/1700/1800/1900/2000/2500/2700/2800/3000/3100/3200/3300
		AXP19:1800~3300,100/step
*/
#define AXP_LDO4_VALUE		2800
/*
	DCDC1:
		AXP20:无
		AXP19:700~3500，25/step
*/
#define AXP_DCDC1_VALUE		1400
/*
	DCDC2：
		AXP20:700~2275，25/step
		AXP19:700~2275，25/step
*/
#define AXP_DCDC2_VALUE		1500
/*
	DCDC3：
		AXP20:700~3500，25/step
		AXP19:700~3500，25/step
*/
#define AXP_DCDC3_VALUE		3000

/*电池容量，mAh：根据实际电池容量来定义，对库仑计方法来说这个参数很重要，必须配置*/
#define BATCAP				4400

/*初始化电池内阻，mΩ：一般在100~200之间，不过最好根据实际测试出来的确定，方法是打开打印信息，不接电池烧好固件后，上电池，不接充电器，开机，开机1分钟后，接上充电器，充1~2分钟，看打印信息中的rdc值，填入这里*/
#define BATRDC				172
/*开路电压方法中的电池电压的缓存*/
#define AXP_VOL_MAX			12

/*
	充电电流设置，mA，0为关闭：
		AXP20:300~1800,100/step
		AXP19:100/190/280/360/450/550/630/700/780/880/960/1000/1080/1160/1240/1320
*/
/*开机充电电流，mA*/
#define STACHGCUR			1200
/*关屏充电电流，mA*/
#define EARCHGCUR			1200
/*休眠充电电流，mA*/
#define SUSCHGCUR			EARCHGCUR
/*关机充电电流，mA*/
#define CLSCHGCUR			EARCHGCUR

/*目标充电电压，mV*/
/*
	AXP20:4100/4150/4200/4360
	AXP19:4100/4150/4200/4360
*/
#define CHGVOL				4200
/*充电电流小于设置电流的ENDCHGRATE%时，停止充电，%*/
/*
	AXP20:10\15
	AXP19:10\15
*/ 
#define ENDCHGRATE			10
/*adc采样率设置，Hz*/
/*
	AXP20:25\50\100\200
	AXP19:25\50\100\200
*/
#define ADCFREQ				25
/*预充电超时时间，min*/
/*
	AXP20:40\50\60\70
	AXP19:30\40\50\60
*/
#define CHGPRETIME			50
/*恒流充电超时时间，min*/
/*
	AXP20:360\480\600\720
	AXP19:420\480\540\600
*/
#define CHGCSTTIME			480


/*pek开机时间，ms*/
/*
	按power键硬件开机时间：
		AXP20:128/1000/2000/3000
		AXP19:128/512/1000/2000
*/
#define PEKOPEN				1000
/*pek长按时间，ms*/
/*
	按power键发长按中断的时间，短于此时间是短按，发短按键irq，长于此时间是长按，发长按键irq：
		AXP20:1000/1500/2000/2500
		AXP19:1000/1500/2000/2500
*/
#define PEKLONG				1500
/*pek长按关机使能*/
/*
	按power键超过关机时长硬件关机功能使能：
		AXP20:0-不关，1-关机
		AXP19:0-不关，1-关机
*/
#define PEKOFFEN			1
/*pekpwr延迟时间，ms*/
/*
	开机后powerok信号的延迟时间：
		AXP20:8/64
		AXP19:8/64
*/
#define PEKDELAY			64
/*pek长按关机时间，ms*/
/*
	按power键的关机时长：
		AXP20:4000/6000/8000/10000
		AXP19:4000/6000/8000/10000
*/
#define PEKOFF				6000

/*n_oe关机时间，ms*/
/*
	N_OE由低到高硬件关机延迟时间
		AXP20:128/1000/2000/3000
		AXP19:128/1000/2000/3000
*/
#define NOEOFF				2000

/*过温关机使能*/
/*
	AXP内部温度过高硬件关机功能使能：
		AXP20:0-不关，1-关机
		AXP19:0-不关，1-关机
*/
#define OTPOFFEN			1

/* usb 限压，mV，0为不限制*/
/*
	AXP20:4100~4700，100/step
	AXP19:4100~4700，100/step
*/
#define USBVOLLIM			4000

/* usb 限流，mA，0为不限制*/
/*
	AXP20:100/500/900
	AXP19:100/500
*/
#define USBCURLIM			0

/*电池信息剩余容量百分比的工作队列更新时间，s*/
#define TIMER1				10
/*电池信息充电状态改变，测量电池信息稳定时间，Timer1XTimer2 s*/
#define TIMER2				3
/*电池内阻计算时间，Timer1XTimer3 s*/
#define TIMER3				2
/*放电电压、放电电流清零时间，Timer1XTimer4 s,注意Timer4 > Timer3*/
#define TIMER4				4
/*对库仑计方法是校准时，电池剩余电量最快更新时间，Timer1XTimer5 s*/
/*对开路电压方法是电池剩余电量最快更新时间，Timer1XTimer5 s*/
#define TIMER5				3
/*未使用irq时，电池信息状态工作队列更新时间，Timer6X0.1 s*/
#define TIMER6				1

/*库仑计算法中，电池容量开始校准剩余容量百分比，需要在放电经过开路电压校准后再做*/
/*
	需要实际测量电池，并且这级的开路电压对应剩余电量比较准
*/
#define BATCAPCORRATE		10
/*关机剩余电量百分比*/
#define SHUTDOWNRATE		0
/*关机电压，mV*/
/*
	系统设计的关机过后的电池端电压，需要与关机百分比、开路电压对应百分比表及低电警告电压相互配合才会有作用
*/
#define SHUTDOWNVOL			3500
/*低电警告电压1，mV*/
/*
	根据系统设计来定，硬件可以定：
		AXP20:2867~4295，6/step
		AXP19:2867~4295，6/step
*/
#define WARNVOL1			(SHUTDOWNVOL * 1000)
#define WARNVOL2			((SHUTDOWNVOL - 50) * 1000)

/*初始化开路电压对应百分比表*/
/*
	可以使用默认值，但是最好根据实际测试的电池来确定每级对应的剩余百分比，特别需要注意，关机电压SHUTDOWNVOL和电池容量开始校准剩余容量百分比BATCAPCORRATE这两级的准确性
	AXP20，19都适用
*/
#if 0
#define OCVREG0				0x00		//3.1328
#define OCVREG1				0x01		//3.2736
#define OCVREG2				0x02		//3.4144
#define OCVREG3				0x05		//3.5552
#define OCVREG4				0x07		//3.6256
#define OCVREG5				0x0D		//3.6608
#define OCVREG6				0x10		//3.6960
#define OCVREG7				0x1A		//3.7312
#define OCVREG8				0x24		//3.7664
#define OCVREG9				0x2E		//3.8016
#define OCVREGA				0x35		//3.8368
#define OCVREGB				0x3D		//3.8720
#define OCVREGC				0x49		//3.9424
#define OCVREGD				0x54		//4.0128
#define OCVREGE				0x5C		//4.0832
#define OCVREGF				0x64		//4.1536
#else
#define OCVREG0				0		  //3132
#define OCVREG1				0		  //3273
#define OCVREG2				0		  //3414
#define OCVREG3				0		//3555
#define OCVREG4				10//10		//3625
#define OCVREG5				14//17		//3660
#define OCVREG6				18//28		//3696
#define OCVREG7				26//38		//3731
#define OCVREG8				40//47		//3766
#define OCVREG9				48//53		//3801
#define OCVREGA				54//60		//3836
#define OCVREGB				59//67		//3872
#define OCVREGC				67//75		//3942
#define OCVREGD				75//81		//4012
#define OCVREGE				85//89		//4083
#define OCVREGF				100		//4153
#endif

/*AXP19 初始化开路电压*/
/*
	只针对AXP19，可以改变，注意和上表的剩余电量百分比一一对应
*/
#define OCVVOL0				3132
#define OCVVOL1				3273
#define OCVVOL2				3414
#define OCVVOL3				3555
#define OCVVOL4				3625
#define OCVVOL5				3660
#define OCVVOL6				3696
#define OCVVOL7				3731
#define OCVVOL8				3766
#define OCVVOL9				3801
#define OCVVOLA				3836
#define OCVVOLB				3872
#define OCVVOLC				3942
#define OCVVOLD				4012
#define OCVVOLE				4083
#define OCVVOLF				4153

/*  AXP中断使能  */
#define	AXP_IRQ_USBLO		(1<< 1)	//usb 低电
#define	AXP_IRQ_USBRE		(1<< 2)	//usb 拔出
#define	AXP_IRQ_USBIN		(1<< 3)	//usb 插入
#define	AXP_IRQ_USBOV		(1<< 4)	//usb 过压
#define	AXP_IRQ_ACRE		(1<< 5)	//ac  拔出
#define	AXP_IRQ_ACIN		(1<< 6)	//ac  插入
#define	AXP_IRQ_ACOV		(1<< 7) //ac  过压
#define	AXP_IRQ_TEMLO		(1<< 8) //电池低温
#define	AXP_IRQ_TEMOV		(1<< 9) //电池过温
#define	AXP_IRQ_CHAOV		(1<<10) //电池充电开始
#define	AXP_IRQ_CHAST		(1<<11) //电池充电结束
#define	AXP_IRQ_BATATOU		(1<<12) //电池退出激活模式
#define	AXP_IRQ_BATATIN		(1<<13) //电池进入激活模式
#define AXP_IRQ_BATRE		(1<<14) //电池拔出
#define AXP_IRQ_BATIN		(1<<15) //电池插入
#define	AXP_IRQ_PEKLO		(1<<16) //power键短按
#define	AXP_IRQ_PEKSH		(1<<17) //power键长按

#define AXP_IRQ_DCDC3LO		(1<<19) //DCDC3小于设置值
#define AXP_IRQ_DCDC2LO		(1<<20) //DCDC2小于设置值
#define AXP_IRQ_DCDC1LO		(1<<21) //DCDC1小于设置值
#define AXP_IRQ_CHACURLO	(1<<22) //充电电流小于设置值
#define AXP_IRQ_ICTEMOV		(1<<23) //AXP芯片内部过温
#define AXP_IRQ_EXTLOWARN2	(1<<24) //APS低压警告电压1
#define AXP_IRQ_EXTLOWARN1	(1<<25) //APS低压警告电压2
#define AXP_IRQ_USBSESUN	(1<<26) //USB Session End
#define AXP_IRQ_USBSESVA	(1<<27) //USB Session A/B
#define AXP_IRQ_USBUN		(1<<28) //USB 无效
#define AXP_IRQ_USBVA		(1<<29) //USB 有效
#define AXP_IRQ_NOECLO		(1<<30) //N_OE关机
#define AXP_IRQ_NOEOPE		(1<<31) //N_OE开机
#define AXP_IRQ_GPIO0TG		(1<<32) //GPIO0输入边沿触发
#define AXP_IRQ_GPIO1TG		(1<<33) //GPIO1输入边沿触发，ADC输入IRQ
#define AXP_IRQ_GPIO2TG		(1<<34) //GPIO2输入边沿触发
#define AXP_IRQ_GPIO3TG		(1<<35) //GPIO3输入边沿触发

#define AXP_IRQ_PEKFE		(1<<37) //power键下降沿触发
#define AXP_IRQ_PEKRE		(1<<38) //power键上升沿触发
#define AXP_IRQ_TIMER		(1<<39) //计时器超时

/*选择需要打开的中断使能*/
#define AXP_NOTIFIER_ON	   (AXP_IRQ_USBIN |\
				       		AXP_IRQ_USBRE |\
				       		AXP_IRQ_ACIN |\
				       		AXP_IRQ_ACRE |\
				       		AXP_IRQ_BATIN |\
				       		AXP_IRQ_BATRE |\
				       		AXP_IRQ_PEKLO |\
				       		AXP_IRQ_PEKSH |\
				       		AXP_IRQ_CHAST |\
				       		AXP_IRQ_CHAOV)

#define ABS(x)				((x) >0 ? (x) : -(x) )

#endif
