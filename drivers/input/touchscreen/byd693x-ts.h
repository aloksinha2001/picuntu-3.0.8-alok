//a4, 52
#define MAX_TOUCH_MAJOR		10		//Charles added
#define MAX_WIDTH_MAJOR		15		//Charles added
#define MAX_TRACKID_ITEM		10	//Charles added

#define REPORT_TOUCH_MAJOR		5		//Charles added
#define REPORT_WIDTH_MAJOR		8		//Charles added

#define FINGER_NO_MAX		10		//Define the max finger number, but the really finger number: fetch from .fex file
#define BYD_COORD_READ_ADDR 0x5c
#define BYD_RW_HIGH_THRD		0X08
#define BYD_RW_LOW_THRD			0X0A
#define BYD_RW_RESO_X				0X0C
#define BYD_RW_RESO_Y				0X0E
#define BYD_Read_CTP_TYPE		0x10
#define BYD_RW_HW_PARA			0X12
#define BYD_RW_RX_SEQ				0X14
#define BYD_RW_TX_SEQ				0X16
#define BYD_RW_ADAPT_PARA		0X18
#define BYD_RW_SW_PARA			0X1A
#define BYD_WRITE_CFG_OK		0XE0
#define REPORT_TPKEY_DOWN		1
#define REPORT_TPKEY_UP			0

//#define RK29xx_ANDROID2_3_REPORT		//if the Android system is V2.3
//#undef RK29xx_ANDROID2_3_REPORT
#define RK29xx_ANDROID4_0_REPORT		//if the Android system is V4.0
//#undef RK29xx_ANDROID4_0_REPORT

//----------------------------------------//
//#define TOUCH_INT_PIN				RK29_PINx_PAx		//define INT Pin	Should be changed to the INT GPIO Port and Pin
//#define TOUCH_RESET_PIN			RK29_PINx_PAx			//define Reset Pin  Should be changed to the Reset GPIO Port and Pin
//#define SW_INT_IRQNO_PIO    TOUCH_INT_PIN

#define	byd693x_I2C_RATE	400*1000   //400KHz

#define USE_TOUCH_KEY

#ifdef USE_TOUCH_KEY
static const uint32_t TPKey_code[4] ={ KEY_SEARCH,KEY_MENU,KEY_HOME,KEY_BACK };
#endif

//struct ChipSetting byd693xcfg_Table1[]={							
//{ 2,0x08,	200/256,	200%256},	//	1	FTHD_H;FTHD_L	//手指按键阈值
//{ 2,0x0A,	120/256,	120%256},	//	2	NTHD_H;NTHD_L	//噪声阈值
//{ 2,0x0C,	SCREEN_MAX_X/256,	SCREEN_MAX_X%256},	//	3 RESX_H;RESX_L	//X分辨率
//{ 2,0x0E,	SCREEN_MAX_Y/256,	SCREEN_MAX_Y%256},	//	4	RESY_H;RESY_L	//Y分辨率
//};

struct sTYPEID_Info
{
	unsigned char	sTYPEID_TYPEH;		//????	2??
	unsigned char	sTYPEID_TYPEL;
	unsigned char	sTYPEID_CoverID;	//????,F--  P--  G--
	unsigned char	sTYPEID_DateNo[5];	//?????,?EXCEL??????????
	unsigned char	sTYPEID_ChipNo[8];	//?????,
	unsigned char	sTYPEID_Type_Str[11];		//??????,?0??
	unsigned char	sTYPEID_FPC_Str[9];		//FPC??????,?0??
	unsigned char	sTYPEID_Chip_Str[6];		//????????,?0??
	unsigned char	sTYPEID_Ver_Str[5];		//??????????,?0??
	unsigned char	sTYPEID_Reserve_Str[9];		//????
	unsigned char sTEST_MODE_MAX;
	unsigned char sTEST_SAMPLE_TIMES;
//	unsigned short sTYPEID_CFG_RESO_X;		//?????? X
//	unsigned short sTYPEID_CFG_RESO_Y;		//?????? Y
	unsigned char sTYPEID_RESO_X_Hbyte;
	unsigned char sTYPEID_RESO_X_Lbyte;
	unsigned char sTYPEID_RESO_Y_Hbyte;
	unsigned char sTYPEID_RESO_Y_Lbyte;
	unsigned char sTYPEID_Reserve_Byte;
	unsigned char	sTYPEID_IIC_Address;		//IIC ??
};

struct sCongig_Para{
	unsigned char sConfig_script[60];
	unsigned char sConfig_Update_Enable;
	unsigned char sConfig_Length;
	unsigned char sConfig_Write_Addr;
	unsigned char sConfig_Table[49];		//sConfig_Table[0] is sConfig_Write_Addr
};
static struct ChipSetting Resume[]={
{ 1, 0x07, 0x01, 0x00},	// Wakeup TP from Sleep mode
};

static struct ChipSetting Suspend[] ={
{ 1, 0x07, 0x00, 0x00}, // Enter Sleep mode
};
struct sCongig_Para gs_HW_Config_Table={
	{
		"Raysens CTP RS7MA0201V1.1,Hardware 12-10-18",
	},
	0x00,
	48,
	BYD_RW_HW_PARA,		//0x12
	{
		BYD_RW_HW_PARA,
		0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x04,0x00,0x03,0x00,0x02,0x00,0x30,0x00,0x30,
		0x00,0x2A,0x00,0x2A,0x00,0x0F,0x00,0x0F,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x01,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x08,0x20,0x00,0x00,0x00,0x00,0x00,0x00,
	},
};
struct sCongig_Para gs_RXSEQ_Config_Table={
	{
		"Raysens CTP RS7MA0201V1.1,RX Sequence 12-10-18"
	},
	0x00,
	48,
	BYD_RW_RX_SEQ,		//0x14
	{
		BYD_RW_RX_SEQ,
		0x00,0x1A,0xAF,0x90,0x31,0xF5,0x00,0x07,0x4F,0x2E,0x00,0x0C,0x14,0x0C,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,
		0x08,0x09,0x0A,0x0B,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	},
};
struct sCongig_Para gs_TXSEQ_Config_Table={
	{
		"Raysens CTP RS7MA0201V1.1,TX Sequence 12-10-18"
	},
	0x00,
	48,
	BYD_RW_TX_SEQ,		//0x16
	{
		BYD_RW_TX_SEQ,		//0x16
		0x0B,0x00,0x0C,0x01,0x0D,0x02,0x0E,0x03,0x0F,0x04,0x10,0x05,0x11,0x06,0x12,0x07,
		0x13,0x08,0x09,0x0A,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	},
};
struct sCongig_Para gs_Adapt_Config_Table={
	{
		"Raysens CTP RS7MA0201V1.1,Adapt Config 12-10-18"
	},
	0x00,
	48,
	BYD_RW_ADAPT_PARA,		//0x18
	{
		BYD_RW_ADAPT_PARA,		//0x18
		0x00,0x05,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x30,0x30,0x30,0x30,0x30,0x30,
		0x30,0x30,0x00,0x10,0x20,0x30,0x50,0x60,0x68,0x70,0x78,0x80,0x88,0x90,0x98,0xA0,
		0xA8,0xA8,0xB0,0xB0,0xB0,0xC0,0x90,0x82,0x82,0x64,0x64,0x00,0x00,0xA0,0x02,0xBC,
	},
};
struct sCongig_Para gs_SW_Config_Table={
	{
		"Raysens CTP RS7MA0201V1.1,Software Config 12-10-18"
	},
	0x00,
	48,
	BYD_RW_SW_PARA,		//0x1A
	{
		BYD_RW_SW_PARA,		//0x1A
		0x03,0x0A,0x02,0x08,0x78,0x50,0x3C,0x33,0x35,0x90,0x70,0x50,0x90,0x1E,0x14,0x36,
		0x01,0x18,0x00,0xC8,0x00,0xA0,0x05,0x00,0x18,0x18,0x10,0x10,0x70,0x78,0x62,0x62,
		0x00,0x00,0x00,0x00,0x0E,0x00,0x00,0xE0,0x0A,0x12,0x02,0x00,0x00,0x00,0x00,0x00,
	},
};
