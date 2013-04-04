//*****************************************************************************
//*****************************************************************************
//  FILENAME: Driver.h
//  TrueTouch Host Emulator Version Information: 2.1.400
//  TrueTouch Firmware Version Information: 1.1.304289
//
//  DESCRIPTION: This file contains configuration values.
//-----------------------------------------------------------------------------
//  Copyright (c) Cypress Semiconductor 2011. All Rights Reserved.
//*****************************************************************************
//*****************************************************************************
//-----------------------------------------------------------------------------
/* Touchscreen Parameters */
static const uint8_t cyttsp4_param_regs[] = {
/*	H, L		  Enum	Name				*/
	0x00,  /* 00	DEBUGDATA0 */
	0x00,  /* 01	DEBUGDATA1 */
	0x14,  /* 02	NUM_ROWS */
	0x1E,  /* 03	NUM_COLS */
	0x04, 0x00,  /* 04	TOUCH_RES_X */
	0x03, 0x00,  /* 05	TOUCH_RES_Y */
	0x4D, 0x95,  /* 06	SCREEN_SIZE_X */
	0x3A, 0x62,  /* 07	SCREEN_SIZE_Y */
	0x00, 0x0A,  /* 08	CENTROID_THRESHOLD */
	0x00, 0x20,  /* 09	THRESHOLD_NOISE */
	0x03, 0xE8,  /* 0A	THRESHOLD_NEG_NOISE */
	0x00,  /* 0B	RESERVED03 */
	0x00, 0x00,  /* 0C	RESERVED04 */
	0x00, 0x28,  /* 0D	THRESHOLD_FINGER */
	0x02, 0x00,  /* 0E	VEL_FINGER_X */
	0x02, 0x00,  /* 0F	VEL_FINGER_Y */
	0x02,  /* 10	FINGER_DEBOUNCE */
	0x00, 0x11,  /* 11	NUM_TX_CLOCKS */
	0x00,  /* 12	IDAC_RANGE */
	0x5D,  /* 13	IDAC_VALUE */
	0x4B,  /* 14	TX_DIV_VALUE_0 */
	0x57,  /* 15	TX_DIV_VALUE_1 */
	0x73,  /* 16	TX_DIV_VALUE_2 */
	0x00, 0x00,  /* 17	RESERVED05 */
	0x0C,  /* 18	LARGE_OBJ_THRES */
	0x09,  /* 19	FAT_FINGER_THRES */
	0x00,  /* 1A	RESERVED06 */
	0x00,  /* 1B	RESERVED07 */
	0x64,  /* 1C	LOW_POWER_INTV */
	0x0A,  /* 1D	REF_INTV */
	0x07, 0xD0,  /* 1E	ACTIVE_MODE_TIMEOUT */
	0x01,  /* 1F	WAKE_UP_SRC */
	0x00, 0x64,  /* 20	HOST_ALERT_PULSE_WIDTH */
	0x00,  /* 21	RESERVED08 */
	0x04,  /* 22	XYFILTERMOVETHOLD */
	0x08,  /* 23	XYFILTERFASTTHOLD */
	0x02,  /* 24	XYFILTERSLOWCOEFF */
	0x10,  /* 25	XYFILTERFASTCOEFF */
	0x0A,  /* 26	FATFINGERLIMIT */
	0x04,  /* 27	IDACHOLDOFF */
	0x01,  /* 28	FreqHoppingOperationalMode */
	0x03, 0xE8,  /* 29	FreqHoppingNoiseThreshold */
	0x00, 0x32,  /* 2A	FreqHoppingBaseLevelDegradation */
	0x00, 0x32,  /* 2B	FreqHoppingRefreshCounterThreshold */
	0x00,  /* 2C	FreqHoppingRegretThreshold */
	0x00,  /* 2D	FreqHoppingTimeIncreaseMultiplier */
	0x03, 0xE8,  /* 2E	BaselineUpdateRate */
	0x01,  /* 2F	AdvancedFingerSeparation */
	0x09,  /* 30	InnerEdgeGain */
	0x04,  /* 31	OuterEdgeGain */
	0x4D, 0x95,  /* 32	XDISPLAYSIZE */
	0x3A, 0x62,  /* 33	YDISPLAYSIZE */
	0x00,  /* 34	XFLIP */
	0x01,  /* 35	YFLIP */
	0x00, 0x64,  /* 36	XOFFSET */
	0x00, 0x64,  /* 37	YOFFSET */
	0x03, 0x20,  /* 38	FATFINGERTRENTH */
	0x05, 0xDC,  /* 39	MULTIFINGERSTRENTH */
	0x20,  /* 3A	AJFSINGALTHRES */
	0x00, 0x80,  /* 3B	XACCERLERATION */
	0x00, 0x78,  /* 3C	YACCERLERATION */
};

/* Touchscreen Parameters Field Sizes (Writable: 0:Readonly; 1:Writable */
static const uint8_t cyttsp4_param_size[] = {
/*   Size	  Enum	Name				*/
	1, /* 00	DEBUGDATA0 */
	1, /* 01	DEBUGDATA1 */
	1, /* 02	NUM_ROWS */
	1, /* 03	NUM_COLS */
	2, /* 04	TOUCH_RES_X */
	2, /* 05	TOUCH_RES_Y */
	2, /* 06	SCREEN_SIZE_X */
	2, /* 07	SCREEN_SIZE_Y */
	2, /* 08	CENTROID_THRESHOLD */
	2, /* 09	THRESHOLD_NOISE */
	2, /* 0A	THRESHOLD_NEG_NOISE */
	1, /* 0B	RESERVED03 */
	2, /* 0C	RESERVED04 */
	2, /* 0D	THRESHOLD_FINGER */
	2, /* 0E	VEL_FINGER_X */
	2, /* 0F	VEL_FINGER_Y */
	1, /* 10	FINGER_DEBOUNCE */
	2, /* 11	NUM_TX_CLOCKS */
	1, /* 12	IDAC_RANGE */
	1, /* 13	IDAC_VALUE */
	1, /* 14	TX_DIV_VALUE_0 */
	1, /* 15	TX_DIV_VALUE_1 */
	1, /* 16	TX_DIV_VALUE_2 */
	2, /* 17	RESERVED05 */
	1, /* 18	LARGE_OBJ_THRES */
	1, /* 19	FAT_FINGER_THRES */
	1, /* 1A	RESERVED06 */
	1, /* 1B	RESERVED07 */
	1, /* 1C	LOW_POWER_INTV */
	1, /* 1D	REF_INTV */
	2, /* 1E	ACTIVE_MODE_TIMEOUT */
	1, /* 1F	WAKE_UP_SRC */
	2, /* 20	HOST_ALERT_PULSE_WIDTH */
	1, /* 21	RESERVED08 */
	1, /* 22	XYFILTERMOVETHOLD */
	1, /* 23	XYFILTERFASTTHOLD */
	1, /* 24	XYFILTERSLOWCOEFF */
	1, /* 25	XYFILTERFASTCOEFF */
	1, /* 26	FATFINGERLIMIT */
	1, /* 27	IDACHOLDOFF */
	1, /* 28	FreqHoppingOperationalMode */
	2, /* 29	FreqHoppingNoiseThreshold */
	2, /* 2A	FreqHoppingBaseLevelDegradation */
	2, /* 2B	FreqHoppingRefreshCounterThreshold */
	1, /* 2C	FreqHoppingRegretThreshold */
	1, /* 2D	FreqHoppingTimeIncreaseMultiplier */
	2, /* 2E	BaselineUpdateRate */
	1, /* 2F	AdvancedFingerSeparation */
	1, /* 30	InnerEdgeGain */
	1, /* 31	OuterEdgeGain */
	2, /* 32	XDISPLAYSIZE */
	2, /* 33	YDISPLAYSIZE */
	1, /* 34	XFLIP */
	1, /* 35	YFLIP */
	2, /* 36	XOFFSET */
	2, /* 37	YOFFSET */
	2, /* 38	FATFINGERTRENTH */
	2, /* 39	MULTIFINGERSTRENTH */
	1, /* 3A	AJFSINGALTHRES */
	2, /* 3B	XACCERLERATION */
	2, /* 3C	YACCERLERATION */
};

