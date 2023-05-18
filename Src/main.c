#include "main.h"
#include "define.h"
#include "usart.h"
#include "tim.h"
#include "led.h"
#include "clock.h"
#include "task.h"
// #include "pinmacro.h"
// #include "hardware.h"
#include "usb_lib.h"
#include "usb_defs.h"

#include "stm32f1xx.h"
#include <string.h>

extern uint8_t rx_data_readed;
extern int rx_data_len;
extern USB_ALIGN uint8_t buffer[ENDP_DATA_SIZE];
void UsbAnswer(void);
int main()
{
    // InitSysClockHSE8();
    InitSysClockHSE72();
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN | RCC_APB2ENR_IOPCEN | RCC_APB2ENR_IOPAEN;
    USB_setup();
    InitLed();
    // InitTIM2();
    InitUSART(BAUDRATE);
    // NVIC_EnableIRQ(USART1_IRQn);
    // NVIC_EnableIRQ(TIM2_IRQn);
    __enable_irq();
    // __ASM volatile("bkpt");
    USARTSendStr("ok\n");
    // USB_ALIGN uint8_t buf[ENDP_DATA_SIZE];
    ClearTask();
    AddTask(ToggleLed, TOP, LOOP, 1000);
    // AddTask(UsbAnswer, BOTTOM, LOOP, 1);
    if (SysTick_Config(72000)) // set 1ms
    {
        while(1); // error
    }
    __enable_irq();
    while (1)
    {
        // usb_class_poll();
        // while (!rx_data_readed)
        // {
        //     /* code */
        // }
        // USB->ISTR &= ~USB_ISTR_SOF;

        // int len;
        // len = usb_ep_read_double(ENDP_DATA_OUT, (uint16_t *)buf);

        // USARTSendData(buf, len);

    }
}

void UsbAnswer()
{
    if (rx_data_readed)
    {
        usb_ep_write_double(ENDP_DATA_IN | 0x80, (uint16_t *)buffer, rx_data_len);
        rx_data_readed = 0;
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