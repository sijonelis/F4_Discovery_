//-fp16_format=ieee

/*******************************************************************************
 * @file    main.h
 * @author  Original F4 Discovery code modified by SOC Robotics, Inc. Development Team
 * @version V1.0.0
 * @date    07-November-2013
 * @brief   This file is a template for the STM F4 Discovery board.
 *
 * The original Discovery F4 code has been ported to the Coocox IDE V1.7.5 with USB CDC support
 * built-in.  This code can be used as a starting point for further development. 
 *
 * CooCox provides a nice user friendly development environment with ongoing community support.  
 * This version uses the latest ARM GNU tool chain (4.7-2013-q3) maintained by a team of ARM employees
 * and is available here:     https://launchpad.net/gcc-arm-embedded
 * CooCox is available here:  www.coocox.org
 *
 * *****************************************************************************/
/**
  ******************************************************************************
  * @file    main.c
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    19-September-2011
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */ 
/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_flash.h"
#include "main.h"
#include "usbd_cdc_core.h"
#include "usbd_usr.h"
#include "usbd_desc.h"
#include "usbd_cdc_vcp.h"

#include "stdbool.h"

#include "adc.h"

#include "dac.h"

#include "usart.h"

//Library config for this project!!!!!!!!!!!
#include "stm32f4xx_conf.h"
#include "string.h"
#include "math.h"
#include "stdarg.h"
#include <stdio.h>
#include <stdlib.h>


/** @addtogroup STM32F4-Discovery_Demo
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

#define TESTRESULT_ADDRESS         0x080FFFFC
#define ALLTEST_PASS               0x00000000
#define ALLTEST_FAIL               0x55555555

#define  XON    0x11
#define  XOFF   0x13
#define  LF	    0x0a
#define  CR	    0x0d
#define  BS     0x08
#define  ESC	0x1b

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
  #if defined ( __ICCARM__ ) /*!< IAR Compiler */
    #pragma data_alignment = 4   
  #endif
#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */
__ALIGN_BEGIN USB_OTG_CORE_HANDLE  USB_OTG_dev __ALIGN_END;

 uint32_t TimingDelay1=0,TimingDelay2=0,TimingDelay3=0,TimingDelay4=0;
 int rxbufptr=0,rxbuflen=0;

char inbuf[100];
char uartSwitch;

enum {READY, UARTTXPACKET, WAITFORUARTTXPACKET, WAITFORRFTXPACKET, UARTTXACK} uartState;

/* Private function prototypes -----------------------------------------------*/

// the variable below indicates USB device is configured by host and ready to communicate.
// this variable is declared and set in the file, usb_usr.c
extern unsigned char gbUsbDeviceReady;

extern float sinfp(float x);
extern float cosfp(float x);
extern float tanfp(float x);
extern float tanfp2(float x);
extern float arctanfp(float x);
extern float lnfp(float x);
extern float sqrtp(float m);

/* Private functions ---------------------------------------------------------*/

// Errno return for math functions
int __errno(void)
 {
	 int i=0;

	 i += 1;
	 return i;
 }

void PutCharu(char inchar)
{
	unsigned char outstrg[2];

	outstrg[0] = inchar;
	outstrg[1] = 0;
	VCP_send_str(&outstrg);
}

// Grab a bunch of chars from USB RX buffer but trickly them out one by one
char GetCharnw(void)
{
	char inchar;
	int tempptr;

	if(!rxbuflen) {
		// Get next batch of inchars
		rxbuflen = VCP_get_string(&inbuf[0]);
		// If buf empty nothing to return
		if(!rxbuflen)  return 0;
		inchar = inbuf[rxbufptr++];
		}
	else {
		if(rxbufptr==rxbuflen) {
			rxbuflen = rxbufptr = 0;
			return 0;
			}
		else inchar = inbuf[rxbufptr++];
		}
	return inchar;
}

char GetCharu(void)
{
	char inchar;

	while(!(inchar=GetCharnw())) ;
	return inchar;
}

void Putsu(char *inchar)
{
	int i;

	for(i=0; i<strlen(inchar); i++) PutChar(inchar[i]);
}

int printf(const char *fmt, ...)
{
	va_list args;
	int i,j;
	char printbuffer[250];

	va_start (args, fmt);
	i = vsnprintf(printbuffer, 250, fmt, args);
	va_end (args);

	// Print the string
	j = 0;
	while(printbuffer[j]!=0) PutCharu(printbuffer[j++]);

	return i;
}

int scanfu(const char *fmt, ...)
{
	char inchar;
	va_list args;
	int i,ptr;
	char scanbuffer[120];

	//printf("\n\rFormat: %s  args=%d",fmt,args);

	ptr = 0;
	while((inchar=GetCharu())!=CR) {
        scanbuffer[ptr++] = inchar;
		//putchar(inchar);
		}
	scanbuffer[ptr] = 0;

	va_start (args, fmt);
	i = vsscanf(scanbuffer, fmt, args);
	va_end (args);

	return i;
}


//UART SWITCH SETTER/GETTER
void uartSwitchSet(char v){
	uartSwitch = v;
}

char uartSwitchGet(){
	return uartSwitch;
}

/**
  * @brief  Inserts a delay time.
  * @param  nTime: specifies the delay time length, in 10 ms.
  * @retval None
  */
void Delay(uint32_t nTime)
{
  TimingDelay1 = nTime;

  while(TimingDelay1 != 0);
}

/**
  * @brief  Decrements the TimingDelay variable.
  * @param  None
  * @retval None
  */
void TimingDelay_Decrement(void)
{
	if(TimingDelay1 != 0x00) TimingDelay1--;
	if(TimingDelay2 != 0x00) TimingDelay2--;
	if(TimingDelay3 != 0x00) TimingDelay3--;
	if(TimingDelay4 != 0x00) TimingDelay4--;
	//printfu("%d    \r",TimingDelay1);
}

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
	int i = 0, j = 0;
	int addr;

	//usb atjungimo kintamasis
	bool usbDisabled = true;

	//adc variables
	float *result;
	char mockResult[200];

	//usart state machine switch
	uartSwitch = 'r';

	//USB buffer variables
    uint8_t buf[255],outstrg[100],inchar;
    uint8_t len;

    //dac variables
    uint16_t waveform[32];
    uint8_t continueReading = 1;
    uint8_t intCount = 0;
    uint8_t charCount = 0;
    bool bufferReadSuccessful = false;
	char tempString[4];

    int cs;
    volatile float a,b,elapsedtime,angle,radius,angleinc;

    RCC_ClocksTypeDef RCC_Clocks;

	// Initialize System and Setup clocks.
	SystemInit();
	SystemCoreClockUpdate();

	//ini ADC
	adcConfigure();

	//init USART
	USARTInit();

	USART_puts(USART2, "P_Cmd\n\n");
	//USARTSendByte();
	//USARTReadByte();

	/* SysTick end of count event each 40ms */
	RCC_GetClocksFreq(&RCC_Clocks);
	//SysTick_Config(RCC_Clocks.HCLK_Frequency /100);

	// Initialize FPU
	*((volatile unsigned long*)0xE000ED88) = 0xF << 20;

	// Initialize LEDs and User_Button on STM32F4-Discovery
    STM_EVAL_LEDInit(LED4);
    STM_EVAL_LEDInit(LED3);
    STM_EVAL_LEDInit(LED5);
    STM_EVAL_LEDInit(LED6);

    if(!usbDisabled){
		// USB Device Initialize
		USBD_Init(&USB_OTG_dev,
				USB_OTG_FS_CORE_ID,
				&USR_desc,
				&USBD_CDC_cb,
				&USR_cb);

		// Wait for USB connection goes live
		while (!gbUsbDeviceReady);

		// Clean USB RX buffer
		while(VCP_get_string(&buf[0]) == 0);

		// Output signon
		printf("USB serial DEMO\r\n");
		printf("test\n\r>");
    }
    //  Main loop
    while(1) {
    	//SPI (WITH MSP) STATE MACHINE
    	if(uartSwitchGet() != 'r')
    		switch(uartSwitchGet()){
    		case 't':
    			for (i=0;i<200;i++)
    				mockResult[i] = i;
    			txDataArrayToMsp(mockResult);
    			break;
    		default:
    			break;
    		}
    	if(!usbDisabled){
			//USB COMM STATE MACHINE
			inchar = GetCharnw();

			if(inchar) {
				switch (inchar){
					case 'a':
						result = adcConvert();
						float* k;
						uint16_t j;
						//for (j=0;j<250;j++){
							for(k=result;k<result+10/*28000*/;k++){
								printf("%f \n",*k);
							}
					//	}
						break;
					case 'd':
						  intCount = charCount = bufferReadSuccessful = 0;
						  while(continueReading){
							for(i=0;i<rxbuflen;i++){
							  if(inbuf[i]!='d' && inbuf[i]!='e' && inbuf[i]!=' ' && continueReading == true){
								tempString[charCount]=inbuf[i];
								charCount++;
								if(charCount==4){
								  waveform[intCount] = 0;
								  for(j=0;j<4;j++)
								  waveform[intCount] += (int)(tempString[j]-'0')*pow(10,3-j);
								  intCount++;
								  charCount = 0;
								}
							  }
							  if(inbuf[i]=='e'||intCount == 32) { continueReading = 0; bufferReadSuccessful = true;}
							}
						  rxbuflen = 0;
						  char inchar = GetCharnw();
						  }
						if(bufferReadSuccessful)DAC_SignalsGenerate(&waveform);
						break;

					case 's':
						printf("\n\rF4 Discovery Test V0.55\n\r>");
						break;
					case 't':
						printf("\n\rDo 10000 circular interpolation calculations\n\r");
						TimingDelay4 = 10000;
						angle = 0.125;
						radius = 2.56;
						angleinc = 0.0001;
						for(i=0; i<100000; i++) {
							a = radius * sinf(angle);
							b = radius * cosf(angle);
							angle += angleinc;
							}
						elapsedtime = ((float)(10000 - TimingDelay4))/25.0;
						printf("timing delay=%d\n\r",TimingDelay4);
						printf("Single precision finished in %f seconds or %f usec/loop\n\r",elapsedtime,elapsedtime*10.0);
						TimingDelay4 = 10000;
						angle = 0.125;
						radius = 2.56;
						angleinc = 0.0001;
						for(i=0; i<100000; i++) {
							a = radius * sinfp(angle);
							b = radius * cosfp(angle);
							angle += angleinc;
							}
						elapsedtime = ((float)(10000 - TimingDelay4))/25.0;
						printf("timing delay=%d\n\r",TimingDelay4);
						printf("Single prec fp finished in %f seconds or %f usec/loop\n\r",elapsedtime,elapsedtime*10.0);
						TimingDelay4 = 10000;
						angle = 0.125;
						radius = 2.56;
						angleinc = 0.0001;
						printf("angle=%f radius=%f angleinc=%f\n\r",angle,radius,angleinc);
						for(i=0; i<100000; i++) {
							a = radius * sin(angle);
							b = radius * cos(angle);
							angle += angleinc;
							}
						printf("timing delay=%d\n\r",TimingDelay4);
						elapsedtime = ((float)(10000 - TimingDelay4))/25.0;
						printf("Double precision finished in %f seconds or %f usec/loop\n\r>",elapsedtime,elapsedtime*10.0);
						break;
					case 'f':
						printf("f\n\rTry float output: 1.234\n\r");
						a = 1.234;
						printf("a = %f\n\r",a);
						i = 35;
						printf("i = %d\n\r",i);
						a = 35.45;
						printf("a = %f\n\r",a);
						printf("a = %f\n\r",12.345);
						printf("a = %f\n\r",-12.345);
						printf("i = %d\n\r",i);
						break;
					case 'g':
						printf("d\n\rRCC_Clocks.HCLK_Frequency=%ld",RCC_Clocks.HCLK_Frequency);
						printf("\n\rDelay 2 second\n\r");
						Delay(200);
						printf("finished\n\r>");
						break;
					case CR:
						printf("\n\r>");
						break;
					default:
						printf("%c\n\r>",inchar);
						break;
					}
				}
    	}

        if (i == 0x100000) {
            STM_EVAL_LEDOff(LED4);
            STM_EVAL_LEDOff(LED3);
            STM_EVAL_LEDOff(LED5);
            STM_EVAL_LEDOff(LED6);
        	}
        if (i++ == 0x200000) {
        	i = 0;
            STM_EVAL_LEDOn(LED4);
            STM_EVAL_LEDOn(LED3);
            STM_EVAL_LEDOn(LED5);
            STM_EVAL_LEDOn(LED6);
        	}
    	}
}

/**
  * @brief  This function handles the test program fail.
  * @param  None
  * @retval None
  */
void Fail_Handler(void)
{
  /* Erase last sector */ 
  FLASH_EraseSector(FLASH_Sector_11, VoltageRange_3);
  /* Write FAIL code at last word in the flash memory */
  FLASH_ProgramWord(TESTRESULT_ADDRESS, ALLTEST_FAIL);
 
  while(1)
  {
    /* Toggle Red LED */
    STM_EVAL_LEDToggle(LED5);
    Delay(5);
  }
}


#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
