#include "adc.h"

#define ADC1_DR_ADDRESS    ((uint32_t)0x4001204C)

#define signalSize ((uint32_t)28000)

__IO uint16_t ADC1ConvertedValue = 0;
__IO uint16_t ADC1ConvertedVoltage = 0;

__IO uint16_t ADC1ConvertedSignalv;
__IO uint16_t ADC1ConvertedSignalmv;

float ADC1ConvertedSignal[signalSize];
float voltsPerBit = 3.0/4095;


void delay(uint32_t nTime)
{
  __IO uint32_t TimingDelay1 = 0;
  TimingDelay1 = nTime;

  while(TimingDelay1 != 0)
	  TimingDelay1--;
}

void adcConfigure(){

  //for ADC1 on PC0 using IN10
  ADC_InitTypeDef ADC_init_structure; //Structure for adc confguration
  GPIO_InitTypeDef GPIO_initStructre; //Structure for analog input pin
  ADC_CommonInitTypeDef ADC_CommonInitStructure;
  DMA_InitTypeDef DMA_InitStructure;  // structure for DMA

  //Clock configuration
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);//The ADC1 is connected the APB2 peripheral bus thus we will use its clock source
  RCC_AHB1PeriphClockCmd(RCC_AHB1ENR_GPIOCEN,ENABLE);//Clock for the ADC port!! Do not forget about this one ;)
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2 ,ENABLE);//Clock for the dma

  /* DMA2 Stream0 channel0 configuration **************************************/
  DMA_DeInit(DMA2_Stream0);
  DMA_InitStructure.DMA_Channel = DMA_Channel_0;
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;
  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&ADC1ConvertedValue;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
  DMA_InitStructure.DMA_BufferSize = 1;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
  DMA_Init(DMA2_Stream0, &DMA_InitStructure);
  /* DMA2_Stream0 enable */
  DMA_Cmd(DMA2_Stream0, ENABLE);

  //Analog pin configuration
  GPIO_initStructre.GPIO_Pin = GPIO_Pin_1;//The channel 11 is connected to PC1
  GPIO_initStructre.GPIO_Mode = GPIO_Mode_AN; //The PC0 pin is configured in analog mode
  GPIO_initStructre.GPIO_PuPd = GPIO_PuPd_NOPULL; //We don't need any pull up or pull down
  GPIO_Init(GPIOC,&GPIO_initStructre);//Affecting the port with the initialization structure configuration

  /* ADC Common Init **********************************************************/
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
  ADC_CommonInit(&ADC_CommonInitStructure);

  //ADC structure configuration
  ADC_DeInit();
  ADC_init_structure.ADC_DataAlign = ADC_DataAlign_Right;//data converted will be shifted to right
  ADC_init_structure.ADC_Resolution = ADC_Resolution_12b;//Input voltage is converted into a 12bit number giving a maximum value of 4096
  ADC_init_structure.ADC_ContinuousConvMode = ENABLE; //the conversion is continuous, the input data is converted more than once
  ADC_init_structure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;// conversion is synchronous with TIM1 and CC1 (actually I'm not sure about this one :/)
  ADC_init_structure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//no trigger for conversion
  ADC_init_structure.ADC_NbrOfConversion = 1;//I think this one is clear :p
  ADC_init_structure.ADC_ScanConvMode = DISABLE;//The scan is configured in one channel
  ADC_Init(ADC1,&ADC_init_structure);//Initialize ADC with the previous configuration

  /* Enable DMA request after last transfer (Single-ADC mode) */
  ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);

  /* Enable ADC1 DMA */
  ADC_DMACmd(ADC1, ENABLE);

  //Enable ADC conversion
  ADC_Cmd(ADC1,ENABLE);

  //Select the channel to be read from
  ADC_RegularChannelConfig(ADC1,ADC_Channel_11,1,ADC_SampleTime_28Cycles);

  /* Enable DMA2 Stream0 Transfer complete interrupt */
   DMA_ITConfig(DMA2_Stream0, DMA_IT_TC, ENABLE);

  ADC_SoftwareStartConv(ADC1); // ADC start converting
}

float* adcConvert(){
  uint32_t v=0,mv=0;
  float t=0;
  int i;

  //v=(ADC1ConvertedValue)/1365;
  //mv = (ADC1ConvertedValue%1365)/100;
 for(i=0;i<signalSize;i++){
    ADC1ConvertedSignal[i]=ADC1ConvertedValue*voltsPerBit;
 }
  return ADC1ConvertedSignal;
}


void DMA2_IRQHANDLER(void)
{
    if(DMA_GetITStatus(DMA2_Stream0, DMA_IT_TCIF0) != RESET)
    {
        DMA_ClearITPendingBit(DMA2_Stream0, DMA_IT_TCIF0);      // Reset du Flag

        ADC1->SR = ADC1->SR & 0xFFED;

        /* Change l'etat de la LED4 */
        GPIO_ToggleBits(GPIOD, GPIO_Pin_12);
    }
}
