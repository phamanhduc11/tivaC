#ifndef __GPIO_H__
#define __GPIO_H__

#ifdef __cplusplus
extern "C"
{
#endif

#define GPIOPA_APB_BASE  0x40004000
#define GPIOPB_APB_BASE  0x40005000
#define GPIOPC_APB_BASE  0x40006000
#define GPIOPD_APB_BASE  0x40007000
#define GPIOPE_APB_BASE  0x40024000
#define GPIOPF_APB_BASE  0x40025000

#define GPIOPA_AHB_BASE  0x40058000
#define GPIOPB_AHB_BASE  0x40059000
#define GPIOPC_AHB_BASE  0x4005A000
#define GPIOPD_AHB_BASE  0x4005B000
#define GPIOPE_AHB_BASE  0x4005C000
#define GPIOPF_AHB_BASE  0x4005D000


#define GPIOPCTL_REG_OFF    0x52C
#define GPIOAFSEL_REG_OFF   0x420
#define GPIOODR_REG_OFF     0x50C
#define GPIODEN_REG_OFF     0x51C
#define GPIODR2R_REG_OFF    0x500
#define GPIOPUR_REG_OFF     0x510


// APIs
void PAD_GPIOI2CPinConfig(void);
void PAD_GPIOSSIPinConfig(void);

#ifdef __cplusplus
}
#endif

#endif
