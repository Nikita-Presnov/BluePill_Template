#ifndef __PINMACRO_H__
#define __PINMACRO_H__
#define asm __ASM

#ifndef USB_DP
#define USB_DP
#define GPIO_USB_DP 12
#endif

#define GPIO_MASK 0b1111 // mask

#define GPIO_OUT 0b0000 // output
#define GPIO_ALT 0b1000 // alternative function output
#define GPIO_PP 0b0000  // push-pull
#define GPIO_OD 0b0100  // open-drain
#define GPIO_2M 0b0010  // max freq = 2 MHz
#define GPIO_10M 0b0001 // max freq = 10 MHz
#define GPIO_50M 0b0011 // max freq = 50 MHz

// basic functions

#define GPIO_PP2 0b0010
#define GPIO_PP10 0b0001
#define GPIO_PP50 0b0011
#define GPIO_OD2 0b0110
#define GPIO_OD10 0b0101
#define GPIO_OD50 0b0111

// alternative functions

#define GPIO_APP2 0b1010
#define GPIO_APP10 0b1001
#define GPIO_APP50 0b1011
#define GPIO_AOD2 0b1110
#define GPIO_AOD10 0b1101
#define GPIO_AOD50 0b1111

// input

#define GPIO_ADC 0b0000
#define GPIO_HIZ 0b0100
#define GPIO_PULL 0b1000
#endif
