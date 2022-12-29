#include <stdint.h>
#include "driverlib/sysctl.h"

#include "INC/timer.h"
#include "INC/debug.h"
#include "INC/sys.h"
#include "Global/Include.h"

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

#define TIMER_STR "Timer"

// *** Below code is generated by register_map_data_extract.py ***
#define GPTM_GPTMCFG_OFF         0x000  // RW 0x0000.0000 GPTM Configuration 727
#define GPTM_GPTMTAMR_OFF        0x004  // RW 0x0000.0000 GPTM Timer A Mode 729
#define GPTM_GPTMTBMR_OFF        0x008  // RW 0x0000.0000 GPTM Timer B Mode 733
#define GPTM_GPTMCTL_OFF         0x00C  // RW 0x0000.0000 GPTM Control 737
#define GPTM_GPTMSYNC_OFF        0x010  // RW 0x0000.0000 GPTM Synchronize 741
#define GPTM_GPTMIMR_OFF         0x018  // RW 0x0000.0000 GPTM Interrupt Mask 745
#define GPTM_GPTMRIS_OFF         0x01C  // RO 0x0000.0000 GPTM Raw Interrupt Status 748
#define GPTM_GPTMMIS_OFF         0x020  // RO 0x0000.0000 GPTM Masked Interrupt Status 751
#define GPTM_GPTMICR_OFF         0x024  // W1C 0x0000.0000 GPTM Interrupt Clear 754
#define GPTM_GPTMTAILR_OFF       0x028  // RW 0xFFFF.FFFF GPTM Timer A Interval Load 756
#define GPTM_GPTMTBILR_OFF       0x02C  // RW - GPTM Timer B Interval Load 757
#define GPTM_GPTMTAMATCHR_OFF    0x030  // RW 0xFFFF.FFFF GPTM Timer A Match 758
#define GPTM_GPTMTBMATCHR_OFF    0x034  // RW - GPTM Timer B Match 759
#define GPTM_GPTMTAPR_OFF        0x038  // RW 0x0000.0000 GPTM Timer A Prescale 760
#define GPTM_GPTMTBPR_OFF        0x03C  // RW 0x0000.0000 GPTM Timer B Prescale 761
#define GPTM_GPTMTAPMR_OFF       0x040  // RW 0x0000.0000 GPTM TimerA Prescale Match 762
#define GPTM_GPTMTBPMR_OFF       0x044  // RW 0x0000.0000 GPTM TimerB Prescale Match 763
#define GPTM_GPTMTAR_OFF         0x048  // RO 0xFFFF.FFFF GPTM Timer A 764
#define GPTM_GPTMTBR_OFF         0x04C  // RO - GPTM Timer B 765
#define GPTM_GPTMTAV_OFF         0x050  // RW 0xFFFF.FFFF GPTM Timer A Value 766
#define GPTM_GPTMTBV_OFF         0x054  // RW - GPTM Timer B Value 767
#define GPTM_GPTMRTCPD_OFF       0x058  // RO 0x0000.7FFF GPTM RTC Predivide 768
#define GPTM_GPTMTAPS_OFF        0x05C  // RO 0x0000.0000 GPTM Timer A Prescale Snapshot 769
#define GPTM_GPTMTBPS_OFF        0x060  // RO 0x0000.0000 GPTM Timer B Prescale Snapshot 770
#define GPTM_GPTMTAPV_OFF        0x064  // RO 0x0000.0000 GPTM Timer A Prescale Value 771
#define GPTM_GPTMTBPV_OFF        0x068  // RO 0x0000.0000 GPTM Timer B Prescale Value 772
#define GPTM_GPTMPP_OFF          0xFC0  // RO 0x0000.0000 GPTM Peripheral Properties 773

regBitInfo GPTMCFG_Info[] {
    {"GPTMCFG", 0, 2},
    {"NULL", 32, 32}
};

regBitInfo GPTMTAMR_Info[] {
    {"TAPLO", 11, 11},
    {"TAMRSU", 10, 10},
    {"TAPWMIE", 9, 9},
    {"TAILD", 8, 8},
    {"TASNAPS", 7, 7},
    {"TAWOT", 6, 6},
    {"TAMIE", 5, 5},
    {"TACDIR", 4, 4},
    {"TAAMS", 3, 3},
    {"TACMR", 2, 2},
    {"TAMR", 0, 1},
    {"NULL", 32, 32}
};

regBitInfo GPTMTBMR_Info[] {
    {"TBPLO", 11, 11},
    {"TBMRSU", 10, 10},
    {"TBPWMIE", 9, 9},
    {"TBILD", 8, 8},
    {"TBSNAPS", 7, 7},
    {"TBWOT", 6, 6},
    {"TBMIE", 5, 5},
    {"TBCDIR", 4, 4},
    {"TBAMS", 3, 3},
    {"TBCMR", 2, 2},
    {"TBMR", 0, 1},
    {"NULL", 32, 32}
};

regBitInfo GPTMCTL_Info[] {
    {"TBPWML", 14, 14},
    {"TBOTE", 13, 13},
    {"TBEVENT", 10, 11},
    {"TBSTALL", 9, 9},
    {"TBEN", 8, 8},
    {"TAPWML", 6, 6},
    {"TAOTE", 5, 5},
    {"RTCEN", 4, 4},
    {"TAEVENT", 2, 3},
    {"TASTALL", 1, 1},
    {"TAEN", 0, 0},
    {"NULL", 32, 32}
};

regBitInfo GPTMSYNC_Info[] {
    {"NULL", 32, 32}
};

regBitInfo GPTMIMR_Info[] {
    {"WUEIM", 16, 16},
    {"TBMIM", 11, 11},
    {"CBEIM", 10, 10},
    {"CBMIM", 9, 9},
    {"TBTOIM", 8, 8},
    {"TAMIM", 4, 4},
    {"RTCIM", 3, 3},
    {"CAEIM", 2, 2},
    {"CAMIM", 1, 1},
    {"TATOIM", 0, 0},
    {"NULL", 32, 32}
};

regBitInfo GPTMRIS_Info[] {
    {"WUERIS", 16, 16},
    {"TBMRIS", 11, 11},
    {"CBERIS", 10, 10},
    {"CBMRIS", 9, 9},
    {"TBTORIS", 8, 8},
    {"TAMRIS", 4, 4},
    {"RTCRIS", 3, 3},
    {"CAERIS", 2, 2},
    {"CAMRIS", 1, 1},
    {"TATORIS", 0, 0},
    {"NULL", 32, 32}
};

regBitInfo GPTMMIS_Info[] {
    {"NULL", 32, 32}
};

regBitInfo GPTMICR_Info[] {
    {"NULL", 32, 32}
};

regBitInfo GPTMTAILR_Info[] {
    {"TAILR", 0, 31},
    {"NULL", 32, 32}
};

regBitInfo GPTMTBILR_Info[] {
    {"TBILR", 0, 31},
    {"NULL", 32, 32}
};

regBitInfo GPTMTAMATCHR_Info[] {
    {"NULL", 32, 32}
};

regBitInfo GPTMTBMATCHR_Info[] {
    {"NULL", 32, 32}
};

regBitInfo GPTMTAPR_Info[] {
    {"NULL", 32, 32}
};

regBitInfo GPTMTBPR_Info[] {
    {"NULL", 32, 32}
};

regBitInfo GPTMTAPMR_Info[] {
    {"NULL", 32, 32}
};

regBitInfo GPTMTBPMR_Info[] {
    {"NULL", 32, 32}
};

regBitInfo GPTMTAR_Info[] {
    {"NULL", 32, 32}
};

regBitInfo GPTMTBR_Info[] {
    {"NULL", 32, 32}
};

regBitInfo GPTMTAV_Info[] {
    {"NULL", 32, 32}
};

regBitInfo GPTMTBV_Info[] {
    {"NULL", 32, 32}
};

regBitInfo GPTMRTCPD_Info[] {
    {"NULL", 32, 32}
};

regBitInfo GPTMTAPS_Info[] {
    {"NULL", 32, 32}
};

regBitInfo GPTMTBPS_Info[] {
    {"NULL", 32, 32}
};

regBitInfo GPTMTAPV_Info[] {
    {"NULL", 32, 32}
};

regBitInfo GPTMTBPV_Info[] {
    {"NULL", 32, 32}
};

regBitInfo GPTMPP_Info[] {
    {"NULL", 32, 32}
};

static PHY_Register GPTMCFG;
static PHY_Register GPTMTAMR;
static PHY_Register GPTMTBMR;
static PHY_Register GPTMCTL;
static PHY_Register GPTMSYNC;
static PHY_Register GPTMIMR;
static PHY_Register GPTMRIS;
static PHY_Register GPTMMIS;
static PHY_Register GPTMICR;
static PHY_Register GPTMTAILR;
static PHY_Register GPTMTBILR;
static PHY_Register GPTMTAMATCHR;
static PHY_Register GPTMTBMATCHR;
static PHY_Register GPTMTAPR;
static PHY_Register GPTMTBPR;
static PHY_Register GPTMTAPMR;
static PHY_Register GPTMTBPMR;
static PHY_Register GPTMTAR;
static PHY_Register GPTMTBR;
static PHY_Register GPTMTAV;
static PHY_Register GPTMTBV;
static PHY_Register GPTMRTCPD;
static PHY_Register GPTMTAPS;
static PHY_Register GPTMTBPS;
static PHY_Register GPTMTAPV;
static PHY_Register GPTMTBPV;
static PHY_Register GPTMPP;

PHY_Reg_Bit GPTMCFG_bitList[sizeof(GPTMCFG_Info)/sizeof(GPTMCFG_Info[0]) - 1];
PHY_Reg_Bit GPTMTAMR_bitList[sizeof(GPTMTAMR_Info)/sizeof(GPTMTAMR_Info[0]) - 1];
PHY_Reg_Bit GPTMTBMR_bitList[sizeof(GPTMTBMR_Info)/sizeof(GPTMTBMR_Info[0]) - 1];
PHY_Reg_Bit GPTMCTL_bitList[sizeof(GPTMCTL_Info)/sizeof(GPTMCTL_Info[0]) - 1];
PHY_Reg_Bit GPTMSYNC_bitList[sizeof(GPTMSYNC_Info)/sizeof(GPTMSYNC_Info[0]) - 1];
PHY_Reg_Bit GPTMIMR_bitList[sizeof(GPTMIMR_Info)/sizeof(GPTMIMR_Info[0]) - 1];
PHY_Reg_Bit GPTMRIS_bitList[sizeof(GPTMRIS_Info)/sizeof(GPTMRIS_Info[0]) - 1];
PHY_Reg_Bit GPTMMIS_bitList[sizeof(GPTMMIS_Info)/sizeof(GPTMMIS_Info[0]) - 1];
PHY_Reg_Bit GPTMICR_bitList[sizeof(GPTMICR_Info)/sizeof(GPTMICR_Info[0]) - 1];
PHY_Reg_Bit GPTMTAILR_bitList[sizeof(GPTMTAILR_Info)/sizeof(GPTMTAILR_Info[0]) - 1];
PHY_Reg_Bit GPTMTBILR_bitList[sizeof(GPTMTBILR_Info)/sizeof(GPTMTBILR_Info[0]) - 1];
PHY_Reg_Bit GPTMTAMATCHR_bitList[sizeof(GPTMTAMATCHR_Info)/sizeof(GPTMTAMATCHR_Info[0]) - 1];
PHY_Reg_Bit GPTMTBMATCHR_bitList[sizeof(GPTMTBMATCHR_Info)/sizeof(GPTMTBMATCHR_Info[0]) - 1];
PHY_Reg_Bit GPTMTAPR_bitList[sizeof(GPTMTAPR_Info)/sizeof(GPTMTAPR_Info[0]) - 1];
PHY_Reg_Bit GPTMTBPR_bitList[sizeof(GPTMTBPR_Info)/sizeof(GPTMTBPR_Info[0]) - 1];
PHY_Reg_Bit GPTMTAPMR_bitList[sizeof(GPTMTAPMR_Info)/sizeof(GPTMTAPMR_Info[0]) - 1];
PHY_Reg_Bit GPTMTBPMR_bitList[sizeof(GPTMTBPMR_Info)/sizeof(GPTMTBPMR_Info[0]) - 1];
PHY_Reg_Bit GPTMTAR_bitList[sizeof(GPTMTAR_Info)/sizeof(GPTMTAR_Info[0]) - 1];
PHY_Reg_Bit GPTMTBR_bitList[sizeof(GPTMTBR_Info)/sizeof(GPTMTBR_Info[0]) - 1];
PHY_Reg_Bit GPTMTAV_bitList[sizeof(GPTMTAV_Info)/sizeof(GPTMTAV_Info[0]) - 1];
PHY_Reg_Bit GPTMTBV_bitList[sizeof(GPTMTBV_Info)/sizeof(GPTMTBV_Info[0]) - 1];
PHY_Reg_Bit GPTMRTCPD_bitList[sizeof(GPTMRTCPD_Info)/sizeof(GPTMRTCPD_Info[0]) - 1];
PHY_Reg_Bit GPTMTAPS_bitList[sizeof(GPTMTAPS_Info)/sizeof(GPTMTAPS_Info[0]) - 1];
PHY_Reg_Bit GPTMTBPS_bitList[sizeof(GPTMTBPS_Info)/sizeof(GPTMTBPS_Info[0]) - 1];
PHY_Reg_Bit GPTMTAPV_bitList[sizeof(GPTMTAPV_Info)/sizeof(GPTMTAPV_Info[0]) - 1];
PHY_Reg_Bit GPTMTBPV_bitList[sizeof(GPTMTBPV_Info)/sizeof(GPTMTBPV_Info[0]) - 1];
PHY_Reg_Bit GPTMPP_bitList[sizeof(GPTMPP_Info)/sizeof(GPTMPP_Info[0]) - 1];

// *** End of generated code by register_map_data_extract.py ***

uint32_t getGPTModuleAddr(eGPTMModule module) {
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

uint16_t TimerModule::isInitialized = 0;

TimerModule::TimerModule(eGPTMModule module, eGPTMMode mode, uint32_t tickPerSec) {
    this->moduleName = TIMER_STR;
    this->baseModuleAddr = getGPTModuleAddr(module);
    this->mode           = mode;
    this->tickMs        = (uint32_t)(SysCtlClockGet()/tickPerSec);
    PAD_SysPeripheralClockEnable(SYSCTL_RCGCTIMER_ADDR, eModule_0);

    if( (1 << module) & this->isInitialized) {
        SystemDebug.log(DEBUG_WRN, "%s %d module is already initialized and used!!", this->moduleName, module);
        this->isUsable = false;
        return;
    }
    else {
        this->isUsable = true;
        this->isInitialized = 1 << module;
    }
// *** Below code is generated by register_map_data_extract.py ***
    GPTMCFG.update("GPTMCFG", this->baseModuleAddr, GPTM_GPTMCFG_OFF, GPTMCFG_bitList, GPTMCFG_Info);
    GPTMTAMR.update("GPTMTAMR", this->baseModuleAddr, GPTM_GPTMTAMR_OFF, GPTMTAMR_bitList, GPTMTAMR_Info);
    GPTMTBMR.update("GPTMTBMR", this->baseModuleAddr, GPTM_GPTMTBMR_OFF, GPTMTBMR_bitList, GPTMTBMR_Info);
    GPTMCTL.update("GPTMCTL", this->baseModuleAddr, GPTM_GPTMCTL_OFF, GPTMCTL_bitList, GPTMCTL_Info);
    GPTMSYNC.update("GPTMSYNC", this->baseModuleAddr, GPTM_GPTMSYNC_OFF, GPTMSYNC_bitList, GPTMSYNC_Info);
    GPTMIMR.update("GPTMIMR", this->baseModuleAddr, GPTM_GPTMIMR_OFF, GPTMIMR_bitList, GPTMIMR_Info);
    GPTMRIS.update("GPTMRIS", this->baseModuleAddr, GPTM_GPTMRIS_OFF, GPTMRIS_bitList, GPTMRIS_Info);
    GPTMMIS.update("GPTMMIS", this->baseModuleAddr, GPTM_GPTMMIS_OFF, GPTMMIS_bitList, GPTMMIS_Info);
    GPTMICR.update("GPTMICR", this->baseModuleAddr, GPTM_GPTMICR_OFF, GPTMICR_bitList, GPTMICR_Info);
    GPTMTAILR.update("GPTMTAILR", this->baseModuleAddr, GPTM_GPTMTAILR_OFF, GPTMTAILR_bitList, GPTMTAILR_Info);
    GPTMTBILR.update("GPTMTBILR", this->baseModuleAddr, GPTM_GPTMTBILR_OFF, GPTMTBILR_bitList, GPTMTBILR_Info);
    GPTMTAMATCHR.update("GPTMTAMATCHR", this->baseModuleAddr, GPTM_GPTMTAMATCHR_OFF, GPTMTAMATCHR_bitList, GPTMTAMATCHR_Info);
    GPTMTBMATCHR.update("GPTMTBMATCHR", this->baseModuleAddr, GPTM_GPTMTBMATCHR_OFF, GPTMTBMATCHR_bitList, GPTMTBMATCHR_Info);
    GPTMTAPR.update("GPTMTAPR", this->baseModuleAddr, GPTM_GPTMTAPR_OFF, GPTMTAPR_bitList, GPTMTAPR_Info);
    GPTMTBPR.update("GPTMTBPR", this->baseModuleAddr, GPTM_GPTMTBPR_OFF, GPTMTBPR_bitList, GPTMTBPR_Info);
    GPTMTAPMR.update("GPTMTAPMR", this->baseModuleAddr, GPTM_GPTMTAPMR_OFF, GPTMTAPMR_bitList, GPTMTAPMR_Info);
    GPTMTBPMR.update("GPTMTBPMR", this->baseModuleAddr, GPTM_GPTMTBPMR_OFF, GPTMTBPMR_bitList, GPTMTBPMR_Info);
    GPTMTAR.update("GPTMTAR", this->baseModuleAddr, GPTM_GPTMTAR_OFF, GPTMTAR_bitList, GPTMTAR_Info);
    GPTMTBR.update("GPTMTBR", this->baseModuleAddr, GPTM_GPTMTBR_OFF, GPTMTBR_bitList, GPTMTBR_Info);
    GPTMTAV.update("GPTMTAV", this->baseModuleAddr, GPTM_GPTMTAV_OFF, GPTMTAV_bitList, GPTMTAV_Info);
    GPTMTBV.update("GPTMTBV", this->baseModuleAddr, GPTM_GPTMTBV_OFF, GPTMTBV_bitList, GPTMTBV_Info);
    GPTMRTCPD.update("GPTMRTCPD", this->baseModuleAddr, GPTM_GPTMRTCPD_OFF, GPTMRTCPD_bitList, GPTMRTCPD_Info);
    GPTMTAPS.update("GPTMTAPS", this->baseModuleAddr, GPTM_GPTMTAPS_OFF, GPTMTAPS_bitList, GPTMTAPS_Info);
    GPTMTBPS.update("GPTMTBPS", this->baseModuleAddr, GPTM_GPTMTBPS_OFF, GPTMTBPS_bitList, GPTMTBPS_Info);
    GPTMTAPV.update("GPTMTAPV", this->baseModuleAddr, GPTM_GPTMTAPV_OFF, GPTMTAPV_bitList, GPTMTAPV_Info);
    GPTMTBPV.update("GPTMTBPV", this->baseModuleAddr, GPTM_GPTMTBPV_OFF, GPTMTBPV_bitList, GPTMTBPV_Info);
    GPTMPP.update("GPTMPP", this->baseModuleAddr, GPTM_GPTMPP_OFF, GPTMPP_bitList, GPTMPP_Info);
// *** End of generated code by register_map_data_extract.py ***

// Initialize with 16/32 timer and concatenated timer
    this->initialize();
}

void TimerModule::initialize(void) {

    if (this->mode == GPTM_MODE_ONESHOT || this->mode == GPTM_MODE_PERIOD) {
        // Below flow is for 16/32b timer with 32b concatenated mode

        // 1. Ensure the timer is disable
        GPTMCTL["TAEN"] = 0;
        GPTMCTL["TBEN"] = 0;

        // 2. Write below regs with value 0x0
        // Concatenated reg
        GPTMCFG = 0;

        // 3. Configure corresponding timer mode
        if (this->mode == GPTM_MODE_ONESHOT) {
            GPTMTAMR["TAMR"] = 0x1;
            GPTMTBMR["TBMR"] = 0x1;
        }
        else {
            GPTMTAMR["TAMR"] = 0x2;
            GPTMTBMR["TBMR"] = 0x2;
        }

        // 4. Optional configure
        // - Snap-shot mode: counting clk between time-out event and read after time-out

        // 5. Load start value
        GPTMTAILR = this->tickMs;
        // Write in concatenate mode 16/32b timer ignore this write
//        GPTMTBILR = this->tickMs;

        // 6. Set interrupt bit mask
        GPTMIMR["TATOIM"] = 1;
        GPTMIMR["TBTOIM"] = 1;

        // 7. Set enable bit -> will do it when manually set
        // GPTMCTL["TAEN"] = 1;
        // GPTMCTL["TBEN"] = 1;
    }
    // other mode not supported yet
}

void TimerModule::setTime(uint32_t ms) {
    if(false == this->isUsable)
        return;

    this->tickMs        = (uint32_t)((SysCtlClockGet()/ 1000) * tickMs);

    // Timer module has counting update condition but have yet SW supported yet
    GPTMTAILR = this->tickMs;
    GPTMTBILR = this->tickMs;

}

void TimerModule::enable(void) {
    if(false == this->isUsable)
        return;

    // Set enable bit -> will do it when manually set
    GPTMCTL["TAEN"] = 1;
    GPTMCTL["TBEN"] = 1;
}

void TimerModule::disable(void) {
    if(false == this->isUsable)
        return;

    // Set enable bit -> will do it when manually set
    GPTMCTL["TAEN"] = 0;
    GPTMCTL["TBEN"] = 0;
}

uint32_t TimerModule::getIntStatus(void) {
    if(false == this->isUsable)
        return 0xffffffff;
    
    return GPTMMIS();
}

void TimerModule::clearInt(uint32_t clearFlags) {
    if(false == this->isUsable)
        return;

    GPTMICR = clearFlags;
}

void TimerModule::setTimerBreakpointDebug(bool isOn) {
    if (false == this->isUsable)
        return;

    if (true == isOn) {
        GPTMCTL["TASTALL"] = 1;
        GPTMCTL["TBSTALL"] = 1;
        GPTMCTL["RTCEN"]   = 0;
    }
    else {
        GPTMCTL["TASTALL"] = 0;
        GPTMCTL["TBSTALL"] = 0;
    }
}