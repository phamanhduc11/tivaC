#include <stdint.h>
#include <stdbool.h>

#define APP_ADDR 0x00008000

extern volatile uint8_t gRemainInBoot;
extern volatile uint32_t g_u321sSystick;
extern volatile uint32_t g_u321msSystick;

void JumpToApp(uint32_t ui32StartAddr);
void InterruptEnd(void);
