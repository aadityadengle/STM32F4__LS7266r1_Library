#include <M4_LS7266.h>

GPIO_InitTypeDef GPIO_InitStruct;
TIM_TimeBaseInitTypeDef TIM_BaseInitStruct;


/*
 * Function	:	Init_7266
 * Purpose	:	Initialize Ls7266 according to InitDef
 * Inputs	:	InitDef for X-Counter, InitDef for Y-Counter
 * Outputs	:	Returns true against process completion
 */



bool Init_7266(LS7266_Chip_InitDef xEncInitDef,LS7266_Chip_InitDef yEncInitDef ){

	TM_DELAY_Init();
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
	GPIO_InitStruct.GPIO_Pin = Pin_CD|Pin_CS|Pin_RD|Pin_WR|Pin_XY|DATA_BUS;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOC,&GPIO_InitStruct);

	// Setup X Encoder
	//Setup IOR Reg.
	outp(XCMD,XIOR(DisAB | LOL | ABGate | CYBW)); //Disable Counters and Set CY BW Mode

	//Setup RLD Reg.
	outp(XCMD,XRLD(Rst_BP | Rst_FLAGS)); //Reset Byte Pointer(BP) And Flags
	outp(XDATA,xEncInitDef.prescaler); //Load 6 to PR0 to setup Transfer to PS0
	outp(XCMD,XRLD(Rst_E | Trf_PS0_PSC)); //Reset E Flag and Transfer PR0 to PSC
	outp(XCMD,XRLD(Rst_BP | Rst_CNTR)); //Reset BP and Reset Counter

	//Setup IDR Reg.
	outp(XCMD,XIDR(xEncInitDef.EnableIndex | xEncInitDef.indexPolarity | xEncInitDef.indexPin)); //Enable Positive Index on LCNTR/LOL Input

	//Setup CMR Reg.
	outp(XCMD,XCMR(xEncInitDef.countOutputMode | xEncInitDef.countMode | xEncInitDef.quadMode)); //Set Binary Mondulo N Quadrature x4
	unsigned int iData = xEncInitDef.maxCount;
	unsigned long tData[3];
	for(int i = 0; i <= 2;i++){
		tData[i]=iData & 0xff;
		iData = iData >> 8;
	}
	//Setup PR Reg. for Modulo N Counter to 0x123456
	outp(XDATA,tData[0]); //LSB
	outp(XDATA,tData[1]);
	outp(XDATA,tData[2]); //MSB


	// Setup Y Encoder
	//Setup IOR Reg.
	outp(YCMD,YIOR(DisAB | LOL | ABGate | CYBW)); //Disable Counters and Set CY BW Mode

	//Setup RLD Reg.
	outp(YCMD,YRLD(Rst_BP | Rst_FLAGS)); //Reset Byte Pointer(BP) And Flags
	outp(YDATA,yEncInitDef.prescaler); //Load 6 to PR0 to setup Transfer to PS0
	outp(YCMD,YRLD(Rst_E | Trf_PS0_PSC)); //Reset E Flag and Transfer PR0 to PSC
	outp(YCMD,YRLD(Rst_BP | Rst_CNTR)); //Reset BP and Reset Counter

	//Setup IDR Reg.
	outp(YCMD,YIDR(yEncInitDef.EnableIndex | yEncInitDef.indexPolarity | yEncInitDef.indexPin)); //Enable Positive Index on LCNTR/LOL Input

	//Setup CMR Reg.
	outp(YCMD,YCMR(yEncInitDef.countOutputMode | yEncInitDef.countMode | yEncInitDef.quadMode)); //Set Binary Mondulo N Quadrature x4
	iData = yEncInitDef.maxCount;

	for(int i = 0; i <= 2;i++){
		tData[i]=iData & 0xff;
		iData = iData >> 8;
	}
	//Setup PR Reg. for Modulo N Counter to 0x123456
	outp(YDATA,tData[0]); //LSB
	outp(YDATA,tData[1]);
	outp(YDATA,tData[2]); //MSB

	//Enable Counters
	outp(XCMD,XIOR(EnAB));
	outp(YCMD,YIOR(EnAB));
	return true;
}



/*
 * Function	:	Write_7266_PR
 * Purpose	:	Write to PR Register of Ls7266r1
 * Inputs	:	Encoder Name, Data
 * Outputs	:	None
 */



void Write_7266_PR(EncoderName EncName,unsigned long Data){
	switch (EncName){
	case bothEncoder:
		outp(XCMD,RLD(Rst_BP)); //Reset Byte Pointer to Synchronize Byte Writing
		outp(XDATA,(unsigned char)Data);
		break;
	case xEncoder:
		outp(XCMD,XRLD(Rst_BP)); //Reset Byte Pointer to Synchronize Byte Writing
		outp(XDATA,(unsigned char)Data);
		break;
	case yEncoder:
		outp(YCMD,YRLD(Rst_BP)); //Reset Byte Pointer to Synchronize Byte Writing
		outp(YDATA,(unsigned char)Data);
		break;
	default:
		break;
	}
}

/*
 * Function	:	Read_7266_OL
 * Purpose	:	Read OL Register of Ls7266r1
 * Inputs	:	Encoder Name
 * Outputs	:	24 bit data
 */



unsigned long Read_7266_OL(EncoderName EncName){
	unsigned long oData=0;
	switch (EncName) {
		case xEncoder:
			outp(XCMD,XRLD(Rst_BP + Trf_CNTR_OL)); //Reset Byte Pointer to Synchronize Byte reading and Transferring of data from counters to OL.
			oData |= inp(XDATA);
			Delay(100);
			oData |= oData | ( inp(XDATA)<<8);
			Delay(100);
			oData |= oData | ( inp(XDATA)<<16);
			break;
		case yEncoder:
			outp(YCMD,YRLD(Rst_BP + Trf_CNTR_OL)); //Reset Byte Pointer to Synchronize Byte reading and Transferring of data from counters to OL.
			oData |= inp(YDATA);
			Delay(100);
			oData |= oData | ( inp(YDATA)<<8);
			Delay(100);
			oData |= oData | ( inp(YDATA)<<16);

		default:
		break;
	}


return(oData);
}



/*
 * Function	:	Get_7266_Flags
 * Purpose	:	Read nFLG Register of Ls7266r1
 * Inputs	:	Encoder Name
 * Outputs	:	8 bit flag data
 */


unsigned char Get_7266_Flags(EncoderName EncName)
{
switch (EncName) {
	case xEncoder:
		return(inp(XFLG));
	break;
	case yEncoder:
		return(inp(YFLG));
	break;

	default:
	break;
}

}

/*
 * Function	:	setCounter
 * Purpose	:	Set nCNTR Register of Ls7266r1
 * Inputs	:	Encoder Name, New Counter Value
 * Outputs	:	Return true if Confirmation of new counter value by reading LS766r1 OL Register, else, returns false.
 */

bool setCounter(EncoderName EncName,int cVal){
	uint8_t D[3];
	for (int r=0; r<=2; r++){
		D[r] = cVal & 0xff;
		cVal = cVal >>8;
	}

	switch (EncName) {
		case xEncoder:
			outp(XCMD,XRLD(Rst_BP));
			outp(XDATA,D[0]);
			outp(XDATA,D[1]);
			outp(XDATA,D[2]);
			outp(XCMD,XRLD(Trf_PR_CNTR));
		break;
		case yEncoder:
			outp(YCMD,YRLD(Rst_BP));
			outp(YDATA,D[0]);
			outp(YDATA,D[1]);
			outp(YDATA,D[2]);
			outp(YCMD,YRLD(Trf_PR_CNTR));
		case bothEncoder:
			outp(XCMD,RLD(Rst_BP));
			outp(XDATA,D[0]);
			outp(XDATA,D[1]);
			outp(XDATA,D[2]);
			outp(XCMD,XRLD(Trf_PR_CNTR));

			outp(YCMD,YRLD(Rst_BP));
			outp(YDATA,D[0]);
			outp(YDATA,D[1]);
			outp(YDATA,D[2]);
			outp(YCMD,YRLD(Trf_PR_CNTR));
		default:
		break;
		unsigned long chkval;
		chkval = Read_7266_OL(0);
		if(chkval == cVal){
			return true;
		}else {
			return false;
		}

	}
}




