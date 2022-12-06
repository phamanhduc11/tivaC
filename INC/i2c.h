#ifndef __I2C_HW__
#define __I2C_HW__
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C"
{
#endif

// Master Registers
#define I2C_BASE 0x40020000
#define I2CMSA_REG      *(volatile unsigned int*)(I2C_BASE    + 0x0)   // M/Slave Address                  (RW)
#define I2CMCS_REG      *(volatile unsigned int*)(I2C_BASE    + 0x4)   // Master Control/Status            (RW)
#define I2CMDR_REG      *(volatile unsigned int*)(I2C_BASE    + 0x8)   // Master Data                      (RW)
#define I2CMTPR_REG     *(volatile unsigned int*)(I2C_BASE    + 0xC)   // Master Timer Period              (RW)
#define I2CMIMR_REG     *(volatile unsigned int*)(I2C_BASE    + 0x10)  // Master Interrupt Mask            (RW)
#define I2CMRIS_REG     *(volatile unsigned int*)(I2C_BASE    + 0x14)  // Master Raw Interrupt Mask        (RO)
#define I2CMMIS_REG     *(volatile unsigned int*)(I2C_BASE    + 0x18)  // Master Masked Interrupt Status   (RO)
#define I2CMICR_REG     *(volatile unsigned int*)(I2C_BASE    + 0x1C)  // Master Interrupt Clear           (WO)
#define I2CMCR_REG      *(volatile unsigned int*)(I2C_BASE    + 0x20)  // Master Configuration             (RW)
#define I2CMCLKOCNT_REG *(volatile unsigned int*)(I2C_BASE    + 0x24)  // Master Clock Low Timeout Count   (RW)
#define I2CMBMON_REG    *(volatile unsigned int*)(I2C_BASE   + 0x2C)  // Master Bus Monitor               (RO)
#define I2CMCR2_REG     *(volatile unsigned int*)(I2C_BASE    + 0x38)  // Master Configuration 2           (RW)

// Slave Registers
#define I2CSOAR_REG     *(volatile unsigned int*)(I2C_BASE    + 0x800) // Slave Own Address                (RW)
#define I2CSCSR_REG     *(volatile unsigned int*)(I2C_BASE    + 0x804) // Slave Control/Status             (RO)
#define I2CSDR_REG      *(volatile unsigned int*)(I2C_BASE    + 0x808) // Slave Data                       (RW)
#define I2CSIMR_REG     *(volatile unsigned int*)(I2C_BASE    + 0x80C) // Slave Interrupt Mask             (RW)
#define I2CSRIS_REG     *(volatile unsigned int*)(I2C_BASE    + 0x810) // Slave Raw Interrupt Status       (RO)
#define I2CSMIS_REG     *(volatile unsigned int*)(I2C_BASE    + 0x814) // Slave Masked Interrupt Status    (RO)
#define I2CSICR_REG     *(volatile unsigned int*)(I2C_BASE    + 0x818) // Slave Interrupt Clear            (WO)
#define I2CSOAR2_REG    *(volatile unsigned int*)(I2C_BASE   + 0x81C) // Slave Own Address 2              (RW)
#define I2CSACKCTL_REG  *(volatile unsigned int*)(I2C_BASE + 0x820) // Slave ACK Control                (RW)

// Status and Control
#define I2CPP_REG   *(volatile unsigned int*)(I2C_BASE    + 0xFC0) // Peripheral Properties            (RO)
#define I2CPC_REG   *(volatile unsigned int*)(I2C_BASE    + 0xFC4) // Peripheral Configuration         (RO)

/*------------------- API -------------------*/
// Init APIs
void I2C_Init(void);
// Master Control APIs
bool I2C_WriteBytes(uint8_t slaveAddress, uint32_t count, uint8_t *data);
bool I2C_ReadBytes(uint32_t slaveAddress, uint32_t count, uint8_t *data);
// Slave Control APIs

#ifdef __cplusplus
}
#endif

#endif
