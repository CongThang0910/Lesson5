#include "stm32f10x.h"
#include <string.h>

// ================= Khai báo =================
void GPIO_Config(void);
void USART1_Config(void);
void USART1_SendChar(char c);
void USART1_SendString(char *str);

char rxBuffer[20];
int rxIndex = 0;

// ================= Main =================
int main(void) {
    GPIO_Config();      // LED PC13
    USART1_Config();    // UART1 + Ngat RX

    USART1_SendString("Hello from STM32!\r\n");

    while (1) {
    }
}

// ================= Cau hình GPIO =================
void GPIO_Config(void) {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

    GPIO_InitTypeDef gpio;
    gpio.GPIO_Pin = GPIO_Pin_13;
    gpio.GPIO_Mode = GPIO_Mode_Out_PP;
    gpio.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOC, &gpio);

    GPIO_SetBits(GPIOC, GPIO_Pin_13); // LED tat ban dau
}

// ================= Cau hình USART1 =================
void USART1_Config(void) {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitTypeDef gpio;
    // PA9 = TX
    gpio.GPIO_Pin = GPIO_Pin_9;
    gpio.GPIO_Mode = GPIO_Mode_AF_PP;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &gpio);

    // PA10 = RX
    gpio.GPIO_Pin = GPIO_Pin_10;
    gpio.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &gpio);

    USART_InitTypeDef usart;
    usart.USART_BaudRate = 9600;
    usart.USART_WordLength = USART_WordLength_8b;
    usart.USART_StopBits = USART_StopBits_1;
    usart.USART_Parity = USART_Parity_No;
    usart.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    usart.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    USART_Init(USART1, &usart);

    // Kích hoat ngat RX
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

    // Bat USART1
    USART_Cmd(USART1, ENABLE);

    // Cau hình NVIC cho USART1
    NVIC_InitTypeDef nvic;
    nvic.NVIC_IRQChannel = USART1_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 0;
    nvic.NVIC_IRQChannelSubPriority = 0;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);
}

// ================= Hàm UART =================
void USART1_SendChar(char c) {
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
    USART_SendData(USART1, c);
}

void USART1_SendString(char *str) {
    while (*str) {
        USART1_SendChar(*str++);
    }
}

// ================= Trình phuc vu ngat USART1 =================
void USART1_IRQHandler(void) {
    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {
        char c = USART_ReceiveData(USART1);

        if (c == '\r' || c == '\n') {
            rxBuffer[rxIndex] = '\0';   
            rxIndex = 0;

            if (strcmp(rxBuffer, "off") == 0) {
                GPIO_ResetBits(GPIOC, GPIO_Pin_13);   
                USART1_SendString("LED off\r\n");
            } 
            else if (strcmp(rxBuffer, "on") == 0) {
							GPIO_SetBits(GPIOC, GPIO_Pin_13);    
                USART1_SendString("LED on\r\n");
            }
            else {
                USART1_SendString("Echo: ");
                USART1_SendString(rxBuffer);
                USART1_SendString("\r\n");
            }
        } 
        else {
            rxBuffer[rxIndex++] = c;
            if (rxIndex >= sizeof(rxBuffer)) rxIndex = 0; // tránh tràn
        }
    }
}