#include "main.h"
#include "define.h"
#include "usart.h"
#include "tim.h"
#include "led.h"
#include "clock.h"

#include "pinmacro.h"
#include "hardware.h"
#include "usb_lib.h"
#include "usb_defs.h"

#include "stm32f1xx.h"
#include <string.h>
int main()
{
    // InitSysClockHSE8();
    InitSysClockHSE72();
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN | RCC_APB2ENR_IOPCEN | RCC_APB2ENR_IOPAEN;
    USB_setup();
    InitLed();
    InitTIM2();
    InitUSART(BAUDRATE);
    NVIC_EnableIRQ(USART1_IRQn);
    NVIC_EnableIRQ(TIM2_IRQn);
    __enable_irq();

    USARTSendStr("ok\n");
    USB_ALIGN uint8_t buf[ENDP_DATA_SIZE];
    while (1)
    {
        // usb_class_poll();

        int len = usb_ep_read_double(ENDP_DATA_OUT, (uint16_t *)buf);
        USARTSendData(buf, len);
        // USARTSendStr(iota());
        if (len - 1 != 0)
        {
            usb_ep_write_double(ENDP_DATA_IN | 0x80, (uint16_t *)buf, len);
        }
    }
}

void TIM2_IRQHandler()
{
    TIM2->SR &= ~TIM_SR_UIF;
    ToggleLed();
}

void USART1_IRQHandler()
{
    char data = USARTReadByte();
    USARTSendByte(data);
    // USART1->SR &= ~USART_SR_RXNE;
    // char data[10]={0};
    // USARTReadStr(data);
    // USARTSendStr(data);
}