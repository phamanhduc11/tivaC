#ifndef _UART_H_
#define _UART_H_
/*UART Base and register*/
#define UART0_BASE 0x4000C000
#define UART1_BASE 0x4000D000
#define UART2_BASE 0x4000E000
#define UART3_BASE 0x4000F000
#define UART4_BASE 0x40010000
#define UART5_BASE 0x40011000
#define UART6_BASE 0x40012000
#define UART7_BASE 0x40013000


#define UARTDR_REG 0x0
#define UARTRSR_REG 0x004
#define UARTFR_REG 0x018
#define UARTILPR_REG 0x020
#define UARTCTL_REG 0x030
#define UARTCTL_BIT_UARTEN_POS 0
#define UARTCTL_BIT_UARTEN_MASK 0x1

#define UARTIBRD_REG 0x024
#define UARTFBRD_REG 0x028
#define UARTLCRH_REG 0x02C
#define UARTLCRH_BIT_BRK_POS 0
#define UARTLCRH_BIT_BRK_MASK 0x1
#define UARTLCRH_BIT_PEN_POS 1
#define UARTLCRH_BIT_PEN_MASK 0x1
#define UARTLCRH_BIT_ESP_POS 2
#define UARTLCRH_BIT_ESP_MASK 0x1
#define UARTLCRH_BIT_STP2_POS 3
#define UARTLCRH_BIT_STP2_MASK 0x1
#define UARTLCRH_BIT_FEN_POS 4
#define UARTLCRH_BIT_FEN_MASK 0x1
#define UARTLCRH_BIT_WLEN_POS 5
#define UARTLCRH_BIT_WLEN_MASK 0x3
#define UARTLCRH_BIT_SPS_POS 7
#define UARTLCRH_BIT_SPS_MASK 0x1
/* 
BRD = BRDI + BRDF = UARTSysClk / (ClkDiv*Baud Rate)
ClkDiv = 16 (set HSE bit) or 8 (clear HSE bit)

*/


enum uartNum {
    uart0 = 0,
    uart1,
    uart2,
    uart3,
    uart4,
    uart5,
    uart6,
    uart7
};






/* public function*/
int uartInitialize(int uartBase);
#endif