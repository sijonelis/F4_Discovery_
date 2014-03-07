#include "stm32f4xx_gpio.h"
#include "stm32f4xx_usart.h"

void USARTInit(void);
void USARTSendByte(int);
void USARTReadByte(void);
void txDataArrayToMsp(char*);

