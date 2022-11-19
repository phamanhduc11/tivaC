#include "uart.h"
#include "macros.h"


struct uList {
    char uartN;
    unsigned int UARTbase;
    unsigned int GPIOport;
} uartList[] = {{uart0,UART0_BASE,IO_PORTA_BASE},
                 {uart1,UART1_BASE,IO_PORTB_BASE},
                 {uart2,UART2_BASE,IO_PORTD_BASE},
                 {uart3,UART3_BASE,IO_PORTC_BASE},
                 {uart4,UART4_BASE,IO_PORTC_BASE},
                 {uart5,UART5_BASE,IO_PORTE_BASE},
                 {uart6,UART6_BASE,IO_PORTD_BASE},
                 {uart7,UART7_BASE,IO_PORTE_BASE},
                 {0xFF,0xFFFFFFFF,0xFFFFFFFF}
};

/* private function, will be move to c file when finish develop*/
static int GPIOModuleEnable(struct uList const * uartCtx);
static int uartModuleClockEnable(struct uList const * uartCtx);
static int uartModuleClockDisable(struct uList const * uartCtx);
static int uartGPIOAltFunctionSet(struct uList const * uartCtx);
static int uartGPIOAltFunctionClear(struct uList const * uartCtx);
static int uartPortControl(struct uList const * uartCtx); // config as table 23-5 page 1351
static int uartSendData(struct uList const* uartCtx, char * ch, int len);
static int uartReadData(struct uList const* uartCtx, char *ch, int len);


struct uList const * uartLookUp(int uartVal){
    struct uList const *uartLooker;
    uartLooker = uartList;
    while(uartLooker->uartN != 0xFF){
        // printf("debug \n");
        if (uartVal == uartLooker->uartN){
            printf("uartBase is %x\n",uartLooker->UARTbase);
            return uartLooker;
        }
        else{
            uartLooker++;
        }
    }
    printf("Not found uart in List\n");
    return uartLooker;
}

static int GPIOModuleEnable(struct uList const * uartCtx){
    int shift = uartCtx->uartN;
    int regVal = REGR(SYSCTL_BASE+RCGCUART_REG);
    regVal = regVal | 1<<shift;
    REGW(SYSCTL_BASE+RCGCUART_REG,regVal);
    return 0;
}

// enable gpio clock
static int uartModuleClockEnable(struct uList const * uartCtx){
    // TODO : add select which GPIO  that uart come from
    int port = uartCtx->GPIOport;
    int shift;
    int regVal;
    switch(port){
        case IO_PORTA_BASE:
            shift = 0;
            break;
        case IO_PORTB_BASE:
            shift = 1;
            break;
        case IO_PORTC_BASE:
            shift = 2;
            break;
        case IO_PORTD_BASE:
            shift = 3;
            break;
        case IO_PORTE_BASE:
            shift = 4;
            break;
        case IO_PORTF_BASE:
            shift = 5;
            break;
        default:
            return -1;
    }
    regVal = REGR(SYSCTL_BASE+RCGCGPIO_REG);
    regVal = regVal | (1<<shift);
    REGW(SYSCTL_BASE+RCGCGPIO_REG,regVal);
    return 0;
}

static int uartGPIOAltFunctionSet(struct uList const * uartCtx){
    // TODO: add select GPIO to set GPIOREG
    int port = uartCtx->GPIOport;
    int uart = uartCtx->uartN;

    REGW(port+GPIOAFSEL_REG,1);
    return 0;
}

static int uartPortControl(struct uList const * uartCtx){
    int uart = uartCtx->uartN;
    int port = uartCtx->GPIOport;
    int regVal = REGR(port+GPIOPCTL_REG);
    regVal = regVal | (GPIO_UART0_RX_VAL << GPIO_UART0_RX_PIN*4 | GPIO_UART0_TX_VAL << GPIO_UART0_TX_PIN*4);
    REGW(port+GPIOPCTL_REG,regVal);
    return 0;
}

// Develop as Initialization from Page 902
int uartInitialize(int uartBase){
    struct uList const *uartCtx = uartLookUp(uartBase);
    GPIOModuleEnable(uartCtx); // enable GPIO port
    uartModuleClockEnable(uartCtx); // enable clock for uartCtx module
    uartGPIOAltFunctionSet(uartCtx); //  enable alternative function for gpio port
    // step 4 ?
    uartPortControl(uartCtx); // select gpio pin according gpio port from uartCtxGPIOAltFunctionSet
}

int uartConfigure(int systemClock, unsigned int baudrate, struct uList *uartCtx){
    int regVal;
    int uartBase = uartCtx->UARTbase;
    unsigned int divisor;
    // clear UARTEN bit from UARTCTL register
    regVal = REGR(uartBase+UARTCTL_REG);
    regVal = regVal & ~(1<<UARTCTL_BIT_UARTEN_POS);
    REGW(uartBase+UARTCTL_REG,regVal); // Set integer divisor in UARTIBRD register divisor = (((systemClock * 8)/baudrate) + 1)/2;
       REGW(uartBase+UARTIBRD_REG, divisor/64); // set fraction divisor in UARTFBRD register REGW(uartBase+UARTIBRD_REG, divisor%64); // set line control in UARTLCRH register
          regVal = REGR(uartBase+UARTLCRH_REG);
    regVal = regVal & (0x3&UARTLCRH_BIT_WLEN_MASK << UARTLCRH_BIT_WLEN_POS);
    REGW(uartBase+UARTLCRH_REG,regVal);
    // config clock source UARTCC register
    // use default system Clock
    // OPTIONAL: uDMA config
    // set UARTEN in UARTCTL register
    regVal = REGR(uartBase+UARTCTL_REG);
    regVal = regVal | (1<<UARTCTL_BIT_UARTEN_POS);
    REGW(uartBase+UARTCTL_REG,regVal);
    return 0;
}

enum flagStatus{
    UART1_CTS_ASSERTED = 0,
    UART1_CTS_NASSERTED,
    UART_BUSY,
    UART_NBUSY,
    UART_RXFIFO_NEMPTY,
    UART_RXFIFO_EMPTY_FEN,
    UART_RXFIFO_EMPTY_NFEN,
    UART_TXFIFO_NFULL,
    UART_TXFIFO_FULL_FEN,
    UART_TXFIFO_FULL_NFEN,
    UART_RXFIFO_NFULL,
    UART_RXFIFO_FULL_FEN,
    UART_RXFIFO_FULL_NFEN,
    UART_TXFIFO_NEMPTY,
    UART_TXFIFO_EMPTY_FEN,
    UART_TXFIFO_EMPTY_NFEN,
    UART_F_ERROR = 0xFF
    };

static unsigned int getRegBitValue(unsigned int regBase, unsigned int regOffset, unsigned int pos, unsigned int mask){
    return (REGR(regBase+regOffset) & (1 << pos)) & mask;
}

static unsigned int uartFlagCheck(unsigned int uartBase, unsigned pos, unsigned mask){
    unsigned int fen = getRegBitValue(uartBase,UARTLCRH_REG,UARTLCRH_BIT_FEN_POS,UARTLCRH_BIT_FEN_MASK);
    unsigned int ret = getRegBitValue(uartBase,UARTFR_REG,pos,mask);
    if (UARTFR_BIT_TXFE_POS == pos){
        
    }
    else if (0);
    
}

static int uartSendData(struct uList const* uartCtx, char * ch, int len){
    unsigned int uartBase = uartCtx->UARTbase;
    // TODO Check Trans FIFO full?


    return 0;
}


static int uartReadData(struct uList const* uartCtx, char *ch, int len);