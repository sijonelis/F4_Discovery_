#include "usart.h"

void USARTInit(){
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	/* enable peripheral clock for USART2 */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);


	/* GPIOA clock enable */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	/* GPIOA Configuration:  USART2 TX on PA2 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* GPIOA Configuration:  USART2 RX on PA2 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* Connect USART2 pins to AF2 */
	// TX = PA2
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);

	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx;
	USART_Init(USART2, &USART_InitStructure);

	USART_Cmd(USART2, ENABLE); // enable USART2

}

USARTSendByte(){
	USART_SendData(USART2, 'h');
	while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
	  {}
}

USARTReadByte(){
	char asd;
	while(USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == RESET)
	{}
	asd = USART_ReceiveData(USART2);

}


void USART2_IRQHandler(void){

        // check if the USART1 receive interrupt flag was set
        if( USART_GetITStatus(USART2, USART_IT_RXNE) ){

                static uint8_t cnt = 0; // this counter is used to determine the string length
                char t = USART2->DR; // the character from the USART1 data register is saved in t

                /* check if the received character is not the LF character (used to determine end of string)
                 * or the if the maximum string length has been been reached
                 */
                if( (t != '\n')){// && (cnt < MAX_STRLEN) ){
                      //  received_string[cnt] = t;
                        cnt++;
                }
               // else{ // otherwise reset the character counter and print the received string
               //         cnt = 0;
               //         USART_puts(USART1, received_string);
               // }
        }
}
