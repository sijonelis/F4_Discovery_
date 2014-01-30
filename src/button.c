#include "button.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"

const uint16_t USER_BUTTON = GPIO_Pin_0;
static uint8_t lastButtonStatus = RESET;

void buttonInit() {
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  GPIO_InitTypeDef gpio;
  GPIO_StructInit(&gpio);
  gpio.GPIO_Mode = GPIO_Mode_IN;
  gpio.GPIO_Pin = USER_BUTTON;
  GPIO_Init(GPIOA, &gpio);
}

//simple check which requires button to be pressed at the time the function is run.
int isButtonPressed(){
  uint8_t currentButtonStatus = GPIO_ReadInputDataBit(GPIOA, USER_BUTTON);
  if (lastButtonStatus != currentButtonStatus && currentButtonStatus != RESET) {
	return 1;
  }
  else{
	lastButtonStatus = currentButtonStatus;
	return 0;
  }
}
