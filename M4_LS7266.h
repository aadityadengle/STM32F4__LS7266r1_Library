/*
 * LS7266 Chip Driver for STM32F405.
 *
 * @author	Aaditya Dengle
 * @email	aadityadengle@gmail.com
 * @version	v1.0 (Beta)
 * @Release 17_Jan_2015.
 * @ide		CooCox	CoIDE
 * @license	GNU GPL v3
 *
 * |----------------------------------------------------------------------
 * | Copyright (C) Aaditya Dengle, 2015
 * |
 * | This program is free software: you can redistribute it and/or modify
 * | it under the terms of the GNU General Public License as published by
 * | the Free Software Foundation, either version 3 of the License, or
 * | any later version.
 * |
 * | This program is distributed in the hope that it will be useful,
 * | but WITHOUT ANY WARRANTY; without even the implied warranty of
 * | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * | GNU General Public License for more details.
 * |
 * | You should have received a copy of the GNU General Public License
 * | along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * |----------------------------------------------------------------------
 *
 *	This library drives LS7266r1 from LSI/CSI to read Quadrature Encoder.
 *	Although STM32F4xx and other ARM Cortex MCUs has good capability to decode
 *	the Quadrature Encoder; It is always better to use dedicated hardware to do the
 *	type of work.
 *************** IMPORTANT *******************************************************************
 *	This library uses Tilen Majerle's tm_stm32f4_delay library which can be fetched from
 *	http://stm32f4-discovery.com/2014/04/library-03-stm32f429-discovery-system-clock-and-pretty-precise-delay-library/
 *********************************************************************************************
 */

#ifndef M4_LS7266_H

#define M4_LS7266_H
/* M4_LS7266_Defines.h has all the custom declarations about LS_Data_Port.
 * You can define all your custom data_bus in the M4_LS7266_Defines.h file.
 * if not declared in M4_LS7266_Defines.h default bus structure will act.
 * Use following to define your custom bus.
 * This library uses single port for DATA & COMMAND bus.
 *
 * In this library point of view:
 * Pin 4 to Pin 11 on LS7266R1 makes DATA BUS.
 * Pin 13 to pin 17 on LS7266R1 makes COMMAND BUS.
 * At the moment (17 Jan 2015) This library do not use complementary inputs from
 * LS7266R1(i.e. xFlg1, xFlg2, yFlg1, yFlg2) and shall be included in next release.
 *
 *
 * Definition		|		Expected Value
 * --------------------------------------------------------------------
 * #define LS_DATA_PORT	|	Your Port on which Data bus is located.
 * #define LS_DATA_BUS	|	8 pins connected to D0.....D7 on LS7266R1.
 * #define LS_Pin_CD	|	Pin connected to C/(D) pin on LS7266R1.
 * #define LS_Pin_WR	|	Pin connected to (WR) pin on LS7266R1.
 * #define LS_Pin_CS	|	Pin connected to (CS) pin on LS7266R1
 * #define LS_Pin_RD	|	Pin connected to (RD) pin on LS7266R1
 * #define LS_Pin_XY	|	Pin connected to (X)/Y pin on LS7266R1
 * ---------------------------------------------------------------------
 */
#include "M4_LS7266_Defines.h"

#include <misc.h>
#include <stm32f4xx.h>
#include <stm32f4xx_rcc.h>
#include <stm32f4xx_gpio.h>
#include <stm32f4xx_tim.h>
#include <tm_stm32f4_Delay.h>
#include <stdbool.h>

#ifndef LS_DATA_PORT
#define DATA_PORT	GPIOC
#else
#define DATA_PORT	LS_DATA_PORT
#endif

#ifndef LS_DATA_BUS
#define DATA_BUS		GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7
#else
#define DATA_BUS		LS_DATA_BUS
#endif


#ifndef LS_Pin_CD
#define Pin_CD		GPIO_Pin_8
#else
#define Pin_CD		LS_Pin_CD
#endif

#ifndef LS_Pin_WR
#define Pin_WR		GPIO_Pin_9
#else
#define Pin_WR		LS_Pin_WR
#endif

#ifndef LS_Pin_CS
	#define Pin_CS		GPIO_Pin_12
#else
	#define	Pin_CS		LS_Pin_CS
#endif

#ifndef LS_Pin_RD
#define Pin_RD		GPIO_Pin_10
#else
#define Pin_RD		LS_Pin_RD
#endif

#ifndef LS_Pin_XY
#define Pin_XY		GPIO_Pin_11
#else
#define Pin_XY		LS_Pin_XY
#endif

// Types of Communcations

#define XDATA	0
#define YDATA 	1
#define XCMD 	2
#define YCMD 	3
#define XFLG	4
#define YFLG	5

// RLD Reg.
#define RLD(arg) (arg | 0x80)
#define XRLD(arg) (arg | 0)
#define YRLD(arg) (arg | 0)
#define Rst_BP 0x01
#define Rst_CNTR 0x02
#define Rst_FLAGS 0x04
#define Rst_E 0x06
#define Trf_PR_CNTR 0x08
#define Trf_CNTR_OL 0x10
#define Trf_PS0_PSC 0x18

//CMR Reg.
#define CMR(arg) (arg | 0xA0)
#define XCMR(arg) (arg | 0x20)
#define YCMR(arg) (arg | 0x20)
#define BINCnt 0x00
#define BCDCnt 0x01
#define NrmCnt 0x00
#define RngLmt 0x02
#define NRcyc 0x04
#define ModN 0x06
#define NQDX 0x00
#define QDX1 0x08
#define QDX2 0x10
#define QDX4 0x18
//IOR Reg.
#define IOR(arg) (arg | 0xC0)
#define XIOR(arg) (arg | 0x40)
#define YIOR(arg) (arg | 0x40)
#define DisAB 0x00
#define EnAB 0x01

#define LCNTR 0x00
#define LOL 0x02
#define RCNTR 0x00
#define ABGate 0x04
#define CYBW 0x00
#define CPBW 0x08
#define CB_UPDN 0x10
#define IDX_ERR 0x18
// IDR
#define IDR(arg) (arg | 0xE0)
#define XIDR(arg) (arg | 0x60)
#define YIDR(arg) (arg | 0x60)
#define DisIDX 0x00
#define EnIDX 0x01
#define NIDX 0x00
#define PIDX 0x02
#define LIDX 0x00
#define RIDX 0x04

typedef enum{
	Data_INPUT,
	Data_OUTPUT
}Data_Direction;

typedef enum{
	xEncoder,
	yEncoder,
	bothEncoder,
}EncoderName;

typedef enum  {
	Normal = 0x00,
	RangeLimit = 0x02,
	NonReCycle = 0x04,
	ModuloN = 0x06

}EncCounterMode;

typedef enum  {
	Binary = 0x00,
	BCD = 0x01,
}EncCounterOPMode;

typedef enum {
	X1 = 0x08,
	X2 = 0x10,
	X4 = 0x18

}EncQuadratureMode;

typedef enum {
	Negative = 0x00,
	Positive = 0x02
}EncIndexPolarity;

typedef enum {
	nLOLPin = 0x00,
	nRCNTRpin = 0x04
}EncIndexPin;

typedef struct{
	unsigned long prescaler;
	int maxCount;
	EncCounterMode countMode;
	EncCounterOPMode countOutputMode;
	EncQuadratureMode	quadMode;
	bool EnableIndex;
	EncIndexPin	indexPin;
	EncIndexPolarity indexPolarity;


}LS7266_Chip_InitDef;

unsigned long Data[3];
volatile int rdCYL;
unsigned long DataByte;

unsigned char Get_7266_Flags(EncoderName EncName);
bool Init_7266(LS7266_Chip_InitDef xEncInitDef,LS7266_Chip_InitDef yEncInitDef );
void Write_7266_PR(EncoderName EncName,unsigned long Data);
unsigned char Get_7266_Flags(EncoderName EncName);
bool setCounter(EncoderName EncName,int cVal);

/*
 * Function	:	DataDirection
 * Purpose	:	Toggle 8 bit Parallel Bus Direction.
 * Inputs	:	Expected Direction (Data_INPUT / Data_OUTPUT)
 * Outputs	:	Change in Bus Direction.
 */
static inline  void DataDirection(Data_Direction dtDirect){
	DATA_PORT->BSRRH =DATA_BUS;
	switch (dtDirect) {
	case Data_INPUT:
		DATA_PORT->MODER = (DATA_PORT->MODER & 0xffff0000);
		Delay(100);
		break;
	case Data_OUTPUT:
		DATA_PORT->MODER = (DATA_PORT->MODER & 0xffff0000) | (uint16_t) 0x5555;
//		DATA_PORT->OTYPER =(DATA_PORT->OTYPER & 0xffff0000);
		Delay(100);
		break;
	}

}

/*
 * Function	:	outp
 * Purpose	:	Output to LS7266r1.
 * Inputs	:	cAdr = Output type (Command / Data) | cData = Data to be send.
 * Outputs	:	Data and Commands reflected on respective bus.
 */
static inline void outp(uint8_t cAdr,unsigned long cData){
	/*
	 * WRITE sequential steps:
	 * Step1:
	 * CS/ (pin15) = high. (When CS/ = high, the Data Bus (D7..D0) goes into high impedance tri-state).
	 * WR/ (pin14) = high;
	 * RD/ (pin16) = high;
	 * C/D/ (pin13) = low, if writing to PR, C/D/ = high, if writing to any other register;
	 * X/Y = low, if writing to X-axis, X/Y = high, if writing to Y-axis
	 *
	 * Step2:
	 * Place the output data to be written to LS7266R1 on the uC IO port feeding the LS7266R1 Data Bus.
	 * All other pins held as in step1.
	 *
	 * Step3:
	 * CS/ = low;
	 * WR/ = low.
	 * All other pins held as in step1.
	 * Wait for a minimum of 45nS ( there is no upper limit)
	 *
	 * Step4.
	 * CS/ = high
	 * WR/ = high
	 * All other pins held as in step1.
	 */
	cData = cData & 0xFF;	//validate data only for first 8bits
	DataDirection(Data_OUTPUT);
	switch (cAdr) {
		case XCMD:
			//Step1:
			DATA_PORT->BSRRL = Pin_CS | Pin_WR | Pin_RD | Pin_CD;
			DATA_PORT->BSRRH = Pin_XY;

			//Step2:
			if (cData<=0xFF) {
				DATA_PORT->BSRRL = cData;
			}else {
				DATA_PORT->BSRRL = cData & 0xFF;
			}


			//Step3:
			DATA_PORT->BSRRH=Pin_CS|Pin_WR;
			Delay(10);

			//Step4:
			DATA_PORT->BSRRL=Pin_CS|Pin_WR;

		break;

		case YCMD:
			//Step1:
			DATA_PORT->BSRRL = Pin_CS | Pin_WR | Pin_RD | Pin_CD | Pin_XY;


			//Step2:
				if (cData<=0xFF) {
					DATA_PORT->BSRRL = cData;
				}else {
					DATA_PORT->BSRRL = cData & 0xFF;
				}

			//Step3:
				DATA_PORT->BSRRH=Pin_CS|Pin_WR;
				Delay(10);

			//Step4:
				DATA_PORT->BSRRL=Pin_CS|Pin_WR;

		break;

		case XDATA:
			//Step1:
				DATA_PORT->BSRRL = Pin_CS | Pin_WR | Pin_RD ;
				DATA_PORT->BSRRH = Pin_XY | Pin_CD;

			//Step2:

				DATA_PORT->BSRRL = cData;

			//Step3:
				DATA_PORT->BSRRH=Pin_CS|Pin_WR;
				Delay(10);

			//Step4:
				DATA_PORT->BSRRL=Pin_CS|Pin_WR;

		break;

		case YDATA:
			//Step1:
				DATA_PORT->BSRRL = Pin_CS | Pin_WR | Pin_RD | Pin_XY;
				DATA_PORT->BSRRH = Pin_CD;


			//Step2:

				DATA_PORT->BSRRL = cData;

			//Step3:
				DATA_PORT->BSRRH=Pin_CS|Pin_WR;
				Delay(10);

			//Step4:
				DATA_PORT->BSRRL=Pin_CS|Pin_WR;

		break;

		default:
		break;
	}

}


/*
 * Function	:	inp
 * Purpose	:	read LS7266r1 data.
 * Inputs	:	Adr = Read type (Flag / Data)
 * Outputs	:	Returns 8 bit data in unsigned long format
 */

static inline unsigned long inp(uint8_t Adr){
	/*
	 * READ sequential steps:
	 * Step1.
	 * CS/ (pin15) = high. (When CS/ = high, the Data Bus (D7..D0) goes into high impedance tri-state).
	 * WR/ (pin14) = high;
	 * RD/ (pin16) = high;
	 * C/D/ (pin13) = low, if reading OL, C/D/ = high, if reading FLAG register;
	 * X/Y = low, if reading X-axis, X/Y = high, if reading Y-axis
	 *
	 * Step2.
	 * CS/ = low;
	 * RD/ = low.
	 * All other pins held as in step1.
	 * Wait for a minimum of 80nS ( there is no upper limit)
	 * The output data from the LS7266R1 is now stable on the Data Bus and remains stable as long as
	 * CS/ and RD/ are held low leaving it up to the uC to store this data.
	 * Step3.
	 * If reading the FLAG register:
	 * RD/ = high
	 * CS/ = high
	 * All other pins held as in step1.
	 * The read operation is complete.
	 * If reading the OL:
	 * RD/ = high
	 * All other pins held as in step2.
	 * Step2 and step3 sequence is iterated 3 times for reading the 3 bytes of OL data; during third
	 * iteration of step3 CS/ is brought high.
	 * A minimum delay of 90nS ( no upper limit) must be allowed following step3 before another READ
	 * cycle (or step2) can be initiated
	 */

	unsigned long RetData = 0;
	DataDirection(Data_INPUT);
	switch (Adr) {
	case XDATA:
		DATA_PORT->BSRRL = Pin_CS | Pin_WR | Pin_RD;
		DATA_PORT->BSRRH = Pin_CD | Pin_XY ;
		Delay(10);
		DATA_PORT->BSRRH = Pin_CS | Pin_RD;
		Delay(10);
		RetData = DATA_PORT->IDR & 0xff;
		if (rdCYL == 2){
			DATA_PORT->BSRRL = Pin_RD | Pin_CS;
			rdCYL =0;
			Delay(10);
		}else {
			DATA_PORT->BSRRL = Pin_RD;
			rdCYL++;
			Delay(10);
		}

	break;

	case YDATA:

		DATA_PORT->BSRRL = Pin_CS | Pin_WR | Pin_RD | Pin_XY;
		DATA_PORT->BSRRH = Pin_CD ;
		Delay(10);
		DATA_PORT->BSRRH = Pin_CS | Pin_RD;
		Delay(10);
		RetData = DATA_PORT->IDR & 0xff;
		if (rdCYL == 2){
			DATA_PORT->BSRRL = Pin_RD | Pin_CS;
			rdCYL = 0;
			Delay(10);
		}else {
			DATA_PORT->BSRRL = Pin_RD;
			rdCYL++;
			Delay(10);
		}

	break;

	case XFLG:
		DATA_PORT->BSRRL = Pin_CS | Pin_WR | Pin_RD | Pin_CD;
		DATA_PORT->BSRRH = Pin_XY;
		DATA_PORT->BSRRH = Pin_CS | Pin_RD;
		Delay(10);
		RetData = DATA_PORT->IDR & 0xff;
		DATA_PORT->BSRRL = Pin_CS | Pin_RD;
	break;

	case YFLG:
		DATA_PORT->BSRRL = Pin_CS | Pin_WR | Pin_RD | Pin_XY | Pin_CD;
		DATA_PORT->BSRRH = Pin_CS | Pin_RD;
		Delay(10);
		RetData = DATA_PORT->IDR & 0xff;
		DATA_PORT->BSRRL = Pin_CS | Pin_RD;
	break;

	default:
	break;
	}
return RetData;
}
#endif
