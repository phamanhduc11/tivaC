/* GPIO PORT Base Macro */
#define IO_PORTA_BASE 0x40004000
#define IO_PORTB_BASE 0x40005000
#define IO_PORTC_BASE 0x40006000
#define IO_PORTD_BASE 0x40007000
#define IO_PORTE_BASE 0x4002C000
#define IO_PORTF_BASE 0x40025000

/* GPIO Registers* offset */
/* belows are theirs bits' info  and position*/
#define GPIOAFSEL_REG 0x420
// bit AFSEL 7:0

#define GPIOPCTL_REG 0x52C
// bit PMC7n 4*(n+1)-1:4*(n+1)-4 : Port Mux control, n = 0->7
// n for select port PIN and value in "Table 10-2. GPIO Pins and Alternate Functions (64LQFP)"
