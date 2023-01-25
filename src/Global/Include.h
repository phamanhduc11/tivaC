#ifndef __STANDARD_DEF__
#define __STANDARD_DEF__
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C"
{
#endif


void __error__(char *pcFilename, uint32_t ui32Line);

extern void
UARTprintf(const char *pcString, ...);
#define printf UARTprintf

static inline unsigned int bit_mask(unsigned int high, unsigned int low)
{
    if (low > high) {
        high += low;
        low = high - low;
        high = high -low;
    }
    if ((high == low) && (high == 0))
        return 1;
    if ((high > 31) | (high == 0) | (low > 31))
        return 0x10101010; // error handle

    return ((1ULL << (high + 1)) - 1) - ((1 << low) - 1);
}

static inline unsigned int GET_SHIFTBITS_MASK(unsigned int MASK)
{  
    unsigned int _mask=MASK;                 
    while(_mask) {                  
        if(_mask&1) break;           
        _mask = _mask >> 1;          
    }                               
    return _mask;                          
}

static inline unsigned int GET_MASK_POS(unsigned int MASK)
{    
    unsigned int _pos = 0, _mask=MASK;       
    while(_mask) {                  
        if(_mask&1) break;           
        _pos++;                      
        _mask = _mask >> 1;          
    }                               
    return _pos;                           
}

#define SET_MASK_VAL(REG, MASK, MASK_VAL) (REG = (REG & ~MASK) | ((MASK_VAL & GET_SHIFTBITS_MASK(MASK)) << GET_MASK_POS(MASK)))



#define REGW(ADDRESS,VALUE) (*(volatile unsigned int*) (ADDRESS) = (VALUE))
#define REGR(ADDRESS)       (*(volatile unsigned int*) (ADDRESS))


#define BIT0    (1<<0)
#define BIT1    (1<<1)
#define BIT2    (1<<2)
#define BIT3    (1<<3)
#define BIT4    (1<<4)
#define BIT5    (1<<5)
#define BIT6    (1<<6)
#define BIT7    (1<<7)
#define BIT8    (1<<8)
#define BIT9    (1<<9)
#define BIT10   (1<<10)
#define BIT11   (1<<11)
#define BIT12   (1<<12)
#define BIT13   (1<<13)
#define BIT14   (1<<14)
#define BIT15   (1<<15)
#define BIT16   (1<<16)
#define BIT17   (1<<17)
#define BIT18   (1<<18)
#define BIT19   (1<<19)
#define BIT20   (1<<20)
#define BIT21   (1<<21)
#define BIT22   (1<<22)
#define BIT23   (1<<23)
#define BIT24   (1<<24)
#define BIT25   (1<<25)
#define BIT26   (1<<26)
#define BIT27   (1<<27)
#define BIT28   (1<<28)
#define BIT29   (1<<29)
#define BIT30   (1<<30)
#define BIT31   (1<<31)

#ifdef __cplusplus
}
#endif

uint32_t getIndexFromStrList(void *listName, const char *targetStr, uint32_t offset);

typedef struct {
    const char *name;
    uint8_t lowBit;
    uint8_t highBit;
} regBitInfo;

typedef struct {
    const char *name;
} stringSearch;

class PHY_Reg_Bit;
class PHY_Register;
class PHY_Reg_Bit{
    public:
        const char* name;
        PHY_Register *parent;
        uint32_t pos;
        uint32_t bitmask;
        PHY_Reg_Bit(PHY_Register *parent, const char *bitName, uint8_t lowBit, uint8_t highBit); // no dynamic allocation so can't use this
        PHY_Reg_Bit(void);
        void update(PHY_Register *parent, const char *bitName, uint8_t lowBit, uint8_t highBit);
        uint32_t operator()(void);
        void operator=(uint32_t value);
        void dumpInfo(void);
};

class PHY_Register{
    private:
        uint32_t baseAddr;
        uint32_t addrOffset;
        uint32_t *regAddress;
        regBitInfo *bitInfoList;
        PHY_Reg_Bit *bitList;
    public:
        const char *name;
        PHY_Register(const char *name, uint32_t baseAddr, uint32_t offset, PHY_Reg_Bit *bitList, regBitInfo *bitInfoList);
        PHY_Register(void);
        void update(const char *name, uint32_t baseAddr, uint32_t offset, PHY_Reg_Bit *bitList, regBitInfo *bitInfoList);
        void baseAddrUpdate(uint32_t baseAddr);
        uint32_t operator()(void);
        PHY_Reg_Bit operator[](const char *bitName);
        void operator=(int32_t value);
};

#endif
