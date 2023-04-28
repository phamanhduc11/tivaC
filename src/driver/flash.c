// Erase size 1KBs as a erase block
// Program size 1 word (4 Bytes), have continuous program upto 32 words (128 bytes)
#define HW_WORDREG(n) = *(unsigned int *)(n)
#define HW_HWORDREG(n) = *(unsigned short *)(n)
#define HW_CHREG(n) = *(unsigned char*)(n)

#define BOOTCFG_REG (0x400FE000 + 0x1D0)

#define FLASH_BASE 0x400FD000
#define FLASH_FMA_REG  (FLASH_BASE + 0x0)
#define FLASH_FMD_REG  (FLASH_BASE + 0x4)
#define FLASH_FMC_REG (FLASH_BASE + 0x8)
#define FLASH_FCRIS_REG (FLASH_BASE +0xC)
#define FLASH_FCIM_REG  (FLASH_BASE + 0x10)
#define FLASH_FCMISC_REG (FLASH_BASE + 0x14)
#define FLASH_FMC2_REG (FLASH_BASE + 0x20)
#define FLASH_FWBVAL_REG (FLASH_BASE + 0x30)
#define FLASH_FWB_REG(n) (FLASH_BASE + 0x100 + 0x4*(n))
#define FLASH_FSIZE_REG (FLASH_BASE + 0xFC0)
#define RAMSIZE_REG (FLASH_BASE + 0xFC4)


uint8_t flashBlockErase(uint32_t addr, int32_t blockSize) {
    uint32_t addressAlign = addr & 0x3ff
    uint8_t keyBit = 0;
    uint16_t wrKey = 0;
    
    if (blockSize <= 0)
        return 0x1;
  
    HW_WORDREG(FLASH_FMA_REG) = addr;
    
    keyBit = (HW_WORDREG(BOOTCFG_REG) & BIT7) ? 1 : 0;
  
    wrKey = keyBit ? 0xa442 : 0x71d5;

    if ((HW_WORDREG(FLASH_FMC_REG) & BIT1)
        {
          // printf("Why still erasing?!\r\n"
        }
    

    // Polling
    while (blockSize--)
        {
           HW_WORDREG(FLASH_FMC_REG) = wrKey << 16 | BIT1;
          while ((HW_WORDREG(FLASH_FMC_REG) & BIT1)
                 {
                   // do nothing
                 }
        }
    
   return 0x0;
}

uint8_t flashMassErase(void) {
    uint8_t keyBit = 0;
    uint16_t wrKey = 0;
    
    keyBit = (HW_WORDREG(BOOTCFG_REG) & BIT7) ? 1 : 0;
    wrKey = keyBit ? 0xa442 : 0x71d5;
    
    HW_WORDREG(FLASH_FMC_REG) = wrKey << 16 | BIT2;
    
    while ((HW_WORDREG(FLASH_FMC_REG) & BIT2)
    {
        // do nothing
    }
}
           
uint8_t flashProgram(uint32_t addr, uint8_t *src, uint32_t fSize) {
}
