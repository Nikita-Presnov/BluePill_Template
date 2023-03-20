#ifndef __MAIN_H__
#define __MAIN_H__
#include "stm32f1xx.h"
#include "stm32f1xx_ll_gpio.h"
#include "stm32f1xx_ll_tim.h"
#include "stm32f1xx_ll_bus.h"
#include "stm32f1xx_ll_usart.h"
#include "stm32f1xx_ll_rcc.h"

#define AUTORELOAD 8000
#define PRESCALER  500
#define BAUDRATE 9600
#define FCLK 36000000
#endif
