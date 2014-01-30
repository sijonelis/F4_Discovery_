#include "stm32f4xx_dma.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_adc.h"

//usb driver
#include "usbd_cdc_vcp.h"

void adcConfigure();
float* adcConvert();
void adcCfg1();
