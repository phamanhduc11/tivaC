#include <stdint.h>

#include "INC/timer.h"
#include "INC/debug.h"

#define GPT0_BASE   0x40030000
#define GPT1_BASE   0x40031000
#define GPT2_BASE   0x40032000
#define GPT3_BASE   0x40033000
#define GPT4_BASE   0x40034000
#define GPT5_BASE   0x40035000
#define GPTW0_BASE  0x40036000
#define GPTW1_BASE  0x40037000
#define GPTW2_BASE  0x4004C000
#define GPTW3_BASE  0x4004D000
#define GPTW4_BASE  0x4004E000
#define GPTW5_BASE  0x4004F000

#define GPT_GPTMCFG_OFF         0x000  // RW 0x0000.0000 GPTM Configuration 727
#define GPT_GPTMTAMR_OFF        0x004  // RW 0x0000.0000 GPTM Timer A Mode 729
#define GPT_GPTMTBMR_OFF        0x008  // RW 0x0000.0000 GPTM Timer B Mode 733
#define GPT_GPTMCTL_OFF         0x00C  // RW 0x0000.0000 GPTM Control 737
#define GPT_GPTMSYNC_OFF        0x010  // RW 0x0000.0000 GPTM Synchronize 741
#define GPT_GPTMIMR_OFF         0x018  // RW 0x0000.0000 GPTM Interrupt Mask 745
#define GPT_GPTMRIS_OFF         0x01C  // RO 0x0000.0000 GPTM Raw Interrupt Status 748
#define GPT_GPTMMIS_OFF         0x020  // RO 0x0000.0000 GPTM Masked Interrupt Status 751
#define GPT_GPTMICR_OFF         0x024  // W1C 0x0000.0000 GPTM Interrupt Clear 754
#define GPT_GPTMTAILR_OFF       0x028  // RW 0xFFFF.FFFF GPTM Timer A Interval Load 756
#define GPT_GPTMTBILR_OFF       0x02C  // RW - GPTM Timer B Interval Load 757
#define GPT_GPTMTAMATCHR_OFF    0x030  // RW 0xFFFF.FFFF GPTM Timer A Match 758
#define GPT_GPTMTBMATCHR_OFF    0x034  // RW - GPTM Timer B Match 759
#define GPT_GPTMTAPR_OFF        0x038  // RW 0x0000.0000 GPTM Timer A Prescale 760
#define GPT_GPTMTBPR_OFF        0x03C  // RW 0x0000.0000 GPTM Timer B Prescale 761
#define GPT_GPTMTAPMR_OFF       0x040  // RW 0x0000.0000 GPTM TimerA Prescale Match 762
#define GPT_GPTMTBPMR_OFF       0x044  // RW 0x0000.0000 GPTM TimerB Prescale Match 763
#define GPT_GPTMTAR_OFF         0x048  // RO 0xFFFF.FFFF GPTM Timer A 764
#define GPT_GPTMTBR_OFF         0x04C  // RO - GPTM Timer B 765
#define GPT_GPTMTAV_OFF         0x050  // RW 0xFFFF.FFFF GPTM Timer A Value 766
#define GPT_GPTMTBV_OFF         0x054  // RW - GPTM Timer B Value 767
#define GPT_GPTMRTCPD_OFF       0x058  // RO 0x0000.7FFF GPTM RTC Predivide 768
#define GPT_GPTMTAPS_OFF        0x05C  // RO 0x0000.0000 GPTM Timer A Prescale Snapshot 769
#define GPT_GPTMTBPS_OFF        0x060  // RO 0x0000.0000 GPTM Timer B Prescale Snapshot 770
#define GPT_GPTMTAPV_OFF        0x064  // RO 0x0000.0000 GPTM Timer A Prescale Value 771
#define GPT_GPTMTBPV_OFF        0x068  // RO 0x0000.0000 GPTM Timer B Prescale Value 772
#define GPT_GPTMPP_OFF          0xFC0  // RO 0x0000.0000 GPTM Peripheral Properties 773

uint32_t getGPTModuleAddr(eGPTModule module) {
    uint32_t retAddr = 0;
    switch (module) {
        case GPT_Module_0:
            retAddr = GPT0_BASE;
            break;

        case GPT_Module_1:
            retAddr = GPT1_BASE;
            break;

        case GPT_Module_2:
            retAddr = GPT2_BASE;
            break;

        case GPT_Module_3:
            retAddr = GPT3_BASE;
            break;

        case GPT_Module_4:
            retAddr = GPT4_BASE;
            break;

        case GPT_Module_5:
            retAddr = GPT5_BASE;
            break;

        case GPTW_Module_0:
            retAddr = GPTW0_BASE;
            break;

        case GPTW_Module_1:
            retAddr = GPTW1_BASE;
            break;

        case GPTW_Module_2:
            retAddr = GPTW2_BASE;
            break;

        case GPTW_Module_3:
            retAddr = GPTW3_BASE;
            break;

        case GPTW_Module_4:
            retAddr = GPTW4_BASE;
            break;

        case GPTW_Module_5:
            retAddr = GPTW5_BASE;
            break;
        
        default:
            retAddr = 0xffffffff;
            SystemDebug.log(DEBUG_ERR, "Assign wrong timer module!!!");
    }
    return retAddr;
}


