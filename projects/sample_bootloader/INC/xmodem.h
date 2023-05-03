#include <stdint.h>
#include <stdbool.h>

extern bool bModemRunning;

// only xmodem 128bytes block
void xmodemReceiveHandler(uint8_t rcvData);
