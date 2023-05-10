#include "main.h"
#include "define.h"
#include "usart.h"
#include "tim.h"
#include "led.h"
#include "clock.h"
#include "stm32f1xx.h"

int main()
{
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN | RCC_APB2ENR_IOPAEN;
    // InitSysClockHSE8();
    InitSysClockHSE72();
    InitLed();
    InitTIM2();
    InitUSART(BAUDRATE);
    NVIC_EnableIRQ(USART1_IRQn);
    NVIC_EnableIRQ(TIM2_IRQn);
    SendStr("ok\n");
    while (1)
    {
    }
}

void TIM2_IRQHandler()
{
    TIM2->SR &= ~TIM_SR_UIF;
    ToggleLed();
}

void USART1_IRQHandler()
{
    char data = ReadByte();
    SendByte(data);
    // USART1->SR &= ~USART_SR_RXNE;
    // char data[10]={0};
    // ReadStr(data);
    // SendStr(data);
}