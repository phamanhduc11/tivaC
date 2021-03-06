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

// UART Data register
#define UARTDR_REG 0x0
#define UARTDR_BIT_DATA_POS 0
#define UARTDR_BIT_DATA_MASK 0xFF
#define UARTDR_BIT_FE_POS 8
#define UARTDR_BIT_FE_MASK 0x1
#define UARTDR_BIT_PE_POS 9
#define UARTDR_BIT_PE_MASK 0x1
#define UARTDR_BIT_BE_POS 10
#define UARTDR_BIT_BE_MASK 0x1
#define UARTDR_BIT_OE_POS 11
#define UARTDR_BIT_OE_MASK 0x1

// UART Receive Status/Error Clear registers
#define UARTRSR_REG 0x004
#define UARTDR_BIT_FE_POS 0
#define UARTDR_BIT_FE_MASK 0x1
#define UARTDR_BIT_PE_POS 1
#define UARTDR_BIT_PE_MASK 0x1
#define UARTDR_BIT_BE_POS 2
#define UARTDR_BIT_BE_MASK 0x1
#define UARTDR_BIT_OE_POS 3
#define UARTDR_BIT_OE_MASK 0x1

#define UARTECR_REG 0x004
#define UARTECR_BIT_DATA_POS 0 
#define UARTECR_BIT_DATA_MASK 0xFF

// UART Flag register
#define UARTFR_REG 0x018
#define UARTFR_BIT_CTS_POS 0
#define UARTFR_BIT_CTS_MASK 0x1
#define UARTFR_BIT_BUSY_POS 3
#define UARTFR_BIT_BUSY_MASK 0x1
#define UARTFR_BIT_RXFE_POS 4
#define UARTFR_BIT_RXFE_MASK 0x1
#define UARTFR_BIT_TXFF_POS 5
#define UARTFR_BIT_TXFF_MASK 0x1
#define UARTFR_BIT_RXFF_POS 6
#define UARTFR_BIT_RXFF_MASK 0x1
#define UARTFR_BIT_TXFE_POS 7
#define UARTFR_BIT_TXFE_MASK 0x1

#define UARTILPR_REG 0x020


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

#define UARTCTL_REG 0x030
#define UARTCTL_BIT_UARTEN_POS 0
#define UARTCTL_BIT_UARTEN_MASK 0x1
#define UARTCTL_BIT_SIREN_POS 1
#define UARTCTL_BIT_SIREN_MASK 0x1
#define UARTCTL_BIT_SIRLP_POS 2
#define UARTCTL_BIT_SIRLP_MASK 0x1
#define UARTCTL_BIT_SMART_POS 3
#define UARTCTL_BIT_SMART_MASK 0x1
#define UARTCTL_BIT_EOT_POS 4
#define UARTCTL_BIT_EOT_MASK 0x1
#define UARTCTL_BIT_HSE_POS 5
#define UARTCTL_BIT_HSE_MASK 0x1
#define UARTCTL_BIT_LBE_POS 7
#define UARTCTL_BIT_LBE_MASK 0x1
#define UARTCTL_BIT_TXE_POS 8
#define UARTCTL_BIT_TXE_MASK 0x1
#define UARTCTL_BIT_RXE_POS 9
#define UARTCTL_BIT_RXE_MASK 0x1
#define UARTCTL_BIT_RTS_POS 11
#define UARTCTL_BIT_RTS_MASK 0x1
#define UARTCTL_BIT_RTSEN_POS 14
#define UARTCTL_BIT_RTSEN_MASK 0x1
#define UARTCTL_BIT_CTSEN_POS 15
#define UARTCTL_BIT_CTSEN_MASK 0x1

// UART Interrupt FIFO Level Select
#define UARTIFLS_REG 0x034
#define UARTIFLS_BIT_TXIFLS_POS 0
#define UARTIFLS_BIT_TXIFLS_MASK 0x3
#define UARTIFLS_BIT_RXIFLS_POS 3
#define UARTIFLS_BIT_RXIFLS_MASK 0x3

// UART Interrupt Mask
#define UARTIM_REG 0x038
#define UARTIM_BIT_CTSIM_POS 1
#define UARTIM_BIT_CTSIM_MASK 0x1
#define UARTIM_BIT_RXIM_POS 4
#define UARTIM_BIT_RXIM_MASK 0x1
#define UARTIM_BIT_TXIM_POS 5
#define UARTIM_BIT_TXIM_MASK 0x1
#define UARTIM_BIT_RTIM_POS 6
#define UARTIM_BIT_RTIM_MASK 0x1
#define UARTIM_BIT_FEIM_POS 7
#define UARTIM_BIT_FEIM_MASK 0x1
#define UARTIM_BIT_PEIM_POS 8
#define UARTIM_BIT_PEIM_MASK 0x1
#define UARTIM_BIT_BEIM_POS 9
#define UARTIM_BIT_BEIM_MASK 0x1
#define UARTIM_BIT_OEIM_POS 10
#define UARTIM_BIT_OEIM_MASK 0x1
#define UARTIM_BIT_9BITM_POS 12
#define UARTIM_BIT_9BITM_MASK 0x1

// UART Raw Interrupt Status
#define UARTRIS_REG 0x3C
#define UARTRIS_BIT_9BITRIS_POS 12
#define UARTRIS_BIT_9BITRIS_MASK 0x1
#define UARTRIS_BIT_OERIS_POS 10
#define UARTRIS_BIT_OERIS_MASK 0x1
#define UARTRIS_BIT_BERIS_POS 9
#define UARTRIS_BIT_BERIS_MASK 0x1
#define UARTRIS_BIT_PERIS_POS 8
#define UARTRIS_BIT_PERIS_MASk 0x1
#define UARTRIS_BIT_FERIS_POS 7
#define UARTRIS_BIT_FERIS_MASK 0x1
#define UARTRIS_BIT_RTRIS_POS 6
#define UARTRIS_BIT_RTRIS_MASK 0x1
#define UARTRIS_BIT_TXRIS_POS 5
#define UARTRIS_BIT_TXRIS_MASK 0x1
#define UARTRIS_BIT_RXRIS_POS 4
#define UARTRIS_BIT_RXRIS_MASK 0x1
#define UARTRIS_BIT_CTSRIS_POS 1
#define UARTRIS_BIT_CTSRIS_MASK 0x1

// UART Masked Interrupt Status 
#define UARTTMIS_REG 0x040    

// UART Interrupt Clear 
#define UARTICR_REG 0x044
#define UARTICR_BIT_9BITIC_POS 12
#define UARTICR_BIT_9BITIC_MASK 0x1
#define UARTICR_BIT_OEIC_POS    10
#define UARTICR_BIT_OEIC_MASK   0x1
#define UARTICR_BIT_BEIC_POS    9
#define UARTICR_BIT_BEIC_MASK   0x1
#define UARTICR_BIT_PEIC_POS    8
#define UARTICR_BIT_PEIC_MASK   0x1
#define UARTICR_BIT_FEIC_POS    7
#define UARTICR_BIT_FEIC_MASK   0x1
#define UARTICR_BIT_RTIC_POS    6
#define UARTICR_BIT_RTIC_MASK   0x1
#define UARTICR_BIT_TXIC_POS    5
#define UARTICR_BIT_TXIC_MASK   0x1
#define UARTICR_BIT_RXIC_POS    4
#define UARTICR_BIT_RXIC_MASK   0x1
#define UARTICR_BIT_CTSMIC_POS  1
#define UARTICR_BIT_CTSMIC_MASK 0x1

// UART DMA Control
#define UARTDMACTL_REG 0x048

// UART 9-Bit Self Address
#define UART9BITADDR_REG 0x0A4

// UART 9-Bit Self Address Mask
#define UART9BITAMASK_REG 0x0A8

// UART Peripheral Properties
#define UARTPP_REG 0xFC0 

// UART Clock Conffiguration
#define UARTCC 0xFC8
#define UARTCC_BIT_CS_POS 0
#define UARTCC_BIT_CS_MASK 0xF


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


enum uartError{
    UART_SUCCESS = 0,
    UART_FRAME_ERROR,
    UART_PARITY_ERROR,
    UART_BREAK_ERROR,
    UART_OVERRUN_ERROR
};




/* public function*/
int uartInitialize(int uartBase);
#endif