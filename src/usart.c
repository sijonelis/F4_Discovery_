#include "usart.h"
#include "main.h"
#include <string.h>

void USARTInit(){
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	/* enable peripheral clock for USART2 */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	NVIC_InitTypeDef NVIC_InitStructure;

    /* Configure the NVIC Preemption Priority Bits */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

	/* Enable the USART2 Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* GPIOA clock enable */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);

	/* GPIOA Configuration:  USART2 TX on PA2 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_Init(USART2, &USART_InitStructure);

	//USART_ITConfig(USART2, USART_IT_TXE, ENABLE);

	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);

	USART_Cmd(USART2, ENABLE); // enable USART2

}

USARTSendByte(){
	//USART_SendData(USART2, 'h');
	//USART_pu
	//while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
	//  {}
}

USARTReadByte(){
	char asd;
	while(USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == RESET)
	{}
	asd = USART_ReceiveData(USART2);

}

void USART_puts(USART_TypeDef* USARTx, volatile char *s){
    char data;
	while(*s){
		// wait until data register is empty
		while( !(USARTx->SR & 0x00000040) );
	    data = *s;
		USART_SendData(USARTx, data);
		s++;
	}
}

void txDataArrayToMsp(char *dataArray){
	int i, j;
	//while(*dataArray){ TODO veliau ideti sita
	for(i=0;i<100;i++){
		//while( !(USART2->SR & 0x00000040) )
		//USART_SendData(USART2, 'X');
		USART_puts(USART2, "X");//*dataArray);
		dataArray++;
	}
}


void USART2_IRQHandler(void){

	int MAX_STRLEN = 6;
	char received_string[6];
	    // check if the USART2 receive interrupt flag was set
    if( USART_GetITStatus(USART2, USART_IT_RXNE) ){
    	static uint8_t cnt = 0; // this counter is used to determine the string length
        char t = USART2->DR; // the character from the USART1 data register is saved in t

        /* check if the received character is not the LF character (used to determine end of string)
        * or the if the maximum string length has been been reached
        */
        if( (t != '\n') && (cnt < MAX_STRLEN) ){
        	received_string[cnt] = t;
        	cnt++;
        }
        else{ // otherwise reset the character counter and print the received string
        	if(strcmp(received_string, "PRAck")){
        		uartSwitchSet('t');
        	}

        	cnt = 0;
        	//USART_puts(USART2, received_string);
        }
    }
}
