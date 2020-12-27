#include "uart.h"
#include "macros.h"
#include <stdio.h>


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
static int GPIOModuleEnable(int uartBase);
static int uartModuleClockEnable(int uartBase);
static int uartModuleClockDisable(int uartBase);
static int uartGPIOAltFunctionSet(int uartBase);
static int uartGPIOAltFunctionClear(int uartBase);
static int uartPortControl(int uartBase); // config as table 23-5 page 1351


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

static int uartCtxPortControl(struct uList const * uartCtx){
    int uart = uartCtx->uartN;
}

// Develop as Initialization from Page 902
int uartInitialize(int uartBase){
    struct uList const *uartCtx = uartCtxLookUp(uartBase);
    GPIOModuleEnable(uartCtx); // enable GPIO port
    uartModuleClockEnable(uartCtx); // enable clock for uartCtx module
    uartGPIOAltFunctionSet(uartCtx); //  enable alternative function for gpio port
    // step 4 ?
    uartCtxPortControl(uartCtx); // select gpio pin according gpio port from uartCtxGPIOAltFunctionSet
}