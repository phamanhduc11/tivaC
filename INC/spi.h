#ifndef __SPI_HW__
#define __SPI_HW__
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C"
{
#endif

//APIs
bool SPI_WriteBytes(uint32_t count, uint8_t *data);
bool SPI_ReadBytes(uint32_t count, uint8_t *data);

#ifdef __cplusplus
}
#endif

typedef enum {
    SPI0_DEV,
    SPI1_DEV,
    SPI2_DEV,
    SPI3_DEV,
} SPI_DEV;

// Current code only use SSI0 device
class SPIInterface {
        static uint8_t isInitialized;
        uint8_t *deviceName;
        bool isUsable;
        uint8_t GPIOInit(SPI_DEV eModule);
        uint8_t moduleInitialize(SPI_DEV eModule);
    public:
        SPIInterface(SPI_DEV device);
        uint8_t write(uint32_t wSize, uint8_t *wBuffer);
        uint8_t read(uint32_t rSize, uint8_t *rBuffer);
};

#endif
