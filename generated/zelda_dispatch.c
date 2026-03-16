/* AUTO-GENERATED dispatch table. DO NOT EDIT. */
#include "nes_runtime.h"
extern int g_current_bank;

void call_by_address(uint16_t addr) {
    switch (addr) {
        case 0xFF50:
            func_FF50(); break;
        case 0xE484:
            func_E484(); break;
        case 0xFFF0:
            func_FFF0(); break;
        case 0x8000:
            switch (g_current_bank) {
                case 7: func_C000(); break;
                case 1: func_8000_b1(); break;
                case 4: func_8000_b4(); break;
                case 5: func_8000_b5(); break;
                case 6: func_8000_b6(); break;
                case 0: func_8000_b0(); break;
                case 2: func_8000_b2(); break;
                case 3: func_8000_b3(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xFF98:
            func_FF98(); break;
        case 0xFFAC:
            func_FFAC(); break;
        case 0xE440:
            func_E440(); break;
        case 0xA080:
            switch (g_current_bank) {
                case 7: func_E080(); break;
                case 0: func_A080_b0(); break;
                case 1: func_A080_b1(); break;
                case 2: func_A080_b2(); break;
                case 3: func_A080_b3(); break;
                case 4: func_A080_b4(); break;
                case 5: func_A080_b5(); break;
                case 6: func_A080_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x8521:
            switch (g_current_bank) {
                case 0: func_8521_b0(); break;
                case 1: func_8521_b1(); break;
                case 2: func_8521_b2(); break;
                case 3: func_8521_b3(); break;
                case 4: func_8521_b4(); break;
                case 5: func_8521_b5(); break;
                case 6: func_8521_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xE62D:
            func_E62D(); break;
        case 0x9825:
            switch (g_current_bank) {
                case 7: func_D825(); break;
                case 0: func_9825_b0(); break;
                case 1: func_9825_b1(); break;
                case 2: func_9825_b2(); break;
                case 3: func_9825_b3(); break;
                case 4: func_9825_b4(); break;
                case 5: func_9825_b5(); break;
                case 6: func_9825_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xE8F8:
            func_E8F8(); break;
        case 0xE573:
            func_E573(); break;
        case 0xEB30:
            func_EB30(); break;
        case 0x8006:
            func_8006_b5(); break;
        case 0xED89:
            func_ED89(); break;
        case 0xB4AC:
            switch (g_current_bank) {
                case 0: func_B4AC_b0(); break;
                case 1: func_B4AC_b1(); break;
                case 2: func_B4AC_b2(); break;
                case 3: func_B4AC_b3(); break;
                case 4: func_B4AC_b4(); break;
                case 5: func_B4AC_b5(); break;
                case 6: func_B4AC_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xB4E8:
            switch (g_current_bank) {
                case 0: func_B4E8_b0(); break;
                case 1: func_B4E8_b1(); break;
                case 2: func_B4E8_b2(); break;
                case 3: func_B4E8_b3(); break;
                case 4: func_B4E8_b4(); break;
                case 5: func_B4E8_b5(); break;
                case 6: func_B4E8_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xE45E:
            func_E45E(); break;
        case 0xE45B:
            func_E45B(); break;
        case 0xF182:
            func_F182(); break;
        case 0xA105:
            func_A105_b1(); break;
        case 0xA0F0:
            func_A0F0_b2(); break;
        case 0xA0E7:
            func_A0E7_b2(); break;
        case 0xA0F6:
            func_A0F6_b6(); break;
        case 0x8519:
            switch (g_current_bank) {
                case 3: func_8519_b3(); break;
                case 4: func_8519_b4(); break;
                case 2: func_8519_b2(); break;
                case 5: func_8519_b5(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xE625:
            func_E625(); break;
        case 0xE640:
            func_E640(); break;
        case 0x9AD5:
            func_9AD5_b0(); break;
        case 0x99A0:
            func_99A0_b0(); break;
        case 0x9B85:
            func_9B85_b0(); break;
        case 0x9C6B:
            func_9C6B_b0(); break;
        case 0x98CC:
            func_98CC_b0(); break;
        case 0x9807:
            func_9807_b2(); break;
        case 0x981F:
            func_981F_b3(); break;
        case 0x9814:
            switch (g_current_bank) {
                case 4: func_9814_b4(); break;
                case 2: func_9814_b2(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x8D00:
            switch (g_current_bank) {
                case 0: func_8D00_b0(); break;
                case 1: func_8D00_b1(); break;
                case 2: func_8D00_b2(); break;
                case 3: func_8D00_b3(); break;
                case 4: func_8D00_b4(); break;
                case 5: func_8D00_b5(); break;
                case 6: func_8D00_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x808C:
            switch (g_current_bank) {
                case 0: func_808C_b0(); break;
                case 1: func_808C_b1(); break;
                case 2: func_808C_b2(); break;
                case 3: func_808C_b3(); break;
                case 4: func_808C_b4(); break;
                case 5: func_808C_b5(); break;
                case 6: func_808C_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xE5E2:
            func_E5E2(); break;
        case 0xE94B:
            func_E94B(); break;
        case 0xE96F:
            func_E96F(); break;
        case 0xE977:
            func_E977(); break;
        case 0xE9D8:
            func_E9D8(); break;
        case 0x877B:
            switch (g_current_bank) {
                case 0: func_877B_b0(); break;
                case 1: func_877B_b1(); break;
                case 2: func_877B_b2(); break;
                case 3: func_877B_b3(); break;
                case 4: func_877B_b4(); break;
                case 5: func_877B_b5(); break;
                case 6: func_877B_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xEA6B:
            func_EA6B(); break;
        case 0xB07A:
            switch (g_current_bank) {
                case 0: func_B07A_b0(); break;
                case 1: func_B07A_b1(); break;
                case 2: func_B07A_b2(); break;
                case 3: func_B07A_b3(); break;
                case 4: func_B07A_b4(); break;
                case 5: func_B07A_b5(); break;
                case 6: func_B07A_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xE9A1:
            func_E9A1(); break;
        case 0x8600:
            switch (g_current_bank) {
                case 0: func_8600_b0(); break;
                case 1: func_8600_b1(); break;
                case 2: func_8600_b2(); break;
                case 3: func_8600_b3(); break;
                case 4: func_8600_b4(); break;
                case 5: func_8600_b5(); break;
                case 6: func_8600_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x910A:
            switch (g_current_bank) {
                case 0: func_910A_b0(); break;
                case 1: func_910A_b1(); break;
                case 2: func_910A_b2(); break;
                case 3: func_910A_b3(); break;
                case 4: func_910A_b4(); break;
                case 5: func_910A_b5(); break;
                case 6: func_910A_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xB0FC:
            switch (g_current_bank) {
                case 7: func_F0FC(); break;
                case 0: func_B0FC_b0(); break;
                case 1: func_B0FC_b1(); break;
                case 2: func_B0FC_b2(); break;
                case 3: func_B0FC_b3(); break;
                case 4: func_B0FC_b4(); break;
                case 5: func_B0FC_b5(); break;
                case 6: func_B0FC_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x9080:
            switch (g_current_bank) {
                case 7: func_D080(); break;
                case 0: func_9080_b0(); break;
                case 1: func_9080_b1(); break;
                case 2: func_9080_b2(); break;
                case 3: func_9080_b3(); break;
                case 4: func_9080_b4(); break;
                case 5: func_9080_b5(); break;
                case 6: func_9080_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x9097:
            switch (g_current_bank) {
                case 0: func_9097_b0(); break;
                case 1: func_9097_b1(); break;
                case 2: func_9097_b2(); break;
                case 3: func_9097_b3(); break;
                case 4: func_9097_b4(); break;
                case 5: func_9097_b5(); break;
                case 6: func_9097_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x861A:
            switch (g_current_bank) {
                case 0: func_861A_b0(); break;
                case 1: func_861A_b1(); break;
                case 2: func_861A_b2(); break;
                case 3: func_861A_b3(); break;
                case 4: func_861A_b4(); break;
                case 5: func_861A_b5(); break;
                case 6: func_861A_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xE9C3:
            func_E9C3(); break;
        case 0x8628:
            switch (g_current_bank) {
                case 0: func_8628_b0(); break;
                case 1: func_8628_b1(); break;
                case 2: func_8628_b2(); break;
                case 3: func_8628_b3(); break;
                case 4: func_8628_b4(); break;
                case 5: func_8628_b5(); break;
                case 6: func_8628_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x8A2F:
            switch (g_current_bank) {
                case 0: func_8A2F_b0(); break;
                case 1: func_8A2F_b1(); break;
                case 2: func_8A2F_b2(); break;
                case 3: func_8A2F_b3(); break;
                case 4: func_8A2F_b4(); break;
                case 5: func_8A2F_b5(); break;
                case 6: func_8A2F_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xA833:
            switch (g_current_bank) {
                case 0: func_A833_b0(); break;
                case 1: func_A833_b1(); break;
                case 2: func_A833_b2(); break;
                case 3: func_A833_b3(); break;
                case 4: func_A833_b4(); break;
                case 5: func_A833_b5(); break;
                case 6: func_A833_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xE9CB:
            func_E9CB(); break;
        case 0xF5A5:
            func_F5A5(); break;
        case 0x8012:
            switch (g_current_bank) {
                case 7: func_C012(); break;
                case 0: func_8012_b0(); break;
                case 1: func_8012_b1(); break;
                case 2: func_8012_b2(); break;
                case 3: func_8012_b3(); break;
                case 4: func_8012_b4(); break;
                case 5: func_8012_b5(); break;
                case 6: func_8012_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x8D47:
            switch (g_current_bank) {
                case 0: func_8D47_b0(); break;
                case 1: func_8D47_b1(); break;
                case 2: func_8D47_b2(); break;
                case 3: func_8D47_b3(); break;
                case 4: func_8D47_b4(); break;
                case 5: func_8D47_b5(); break;
                case 6: func_8D47_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x9000:
            switch (g_current_bank) {
                case 0: func_9000_b0(); break;
                case 1: func_9000_b1(); break;
                case 2: func_9000_b2(); break;
                case 3: func_9000_b3(); break;
                case 4: func_9000_b4(); break;
                case 5: func_9000_b5(); break;
                case 6: func_9000_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x9020:
            switch (g_current_bank) {
                case 7: func_D020(); break;
                case 0: func_9020_b0(); break;
                case 1: func_9020_b1(); break;
                case 2: func_9020_b2(); break;
                case 3: func_9020_b3(); break;
                case 4: func_9020_b4(); break;
                case 5: func_9020_b5(); break;
                case 6: func_9020_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xA591:
            switch (g_current_bank) {
                case 0: func_A591_b0(); break;
                case 1: func_A591_b1(); break;
                case 2: func_A591_b2(); break;
                case 3: func_A591_b3(); break;
                case 4: func_A591_b4(); break;
                case 5: func_A591_b5(); break;
                case 6: func_A591_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xEB96:
            func_EB96(); break;
        case 0xEBAA:
            func_EBAA(); break;
        case 0xEBC0:
            func_EBC0(); break;
        case 0xEC1B:
            func_EC1B(); break;
        case 0xEB62:
            func_EB62(); break;
        case 0xEB76:
            func_EB76(); break;
        case 0xA6AD:
            switch (g_current_bank) {
                case 0: func_A6AD_b0(); break;
                case 1: func_A6AD_b1(); break;
                case 2: func_A6AD_b2(); break;
                case 3: func_A6AD_b3(); break;
                case 4: func_A6AD_b4(); break;
                case 5: func_A6AD_b5(); break;
                case 6: func_A6AD_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x9EF0:
            switch (g_current_bank) {
                case 7: func_DEF0(); break;
                case 0: func_9EF0_b0(); break;
                case 1: func_9EF0_b1(); break;
                case 2: func_9EF0_b2(); break;
                case 3: func_9EF0_b3(); break;
                case 4: func_9EF0_b4(); break;
                case 5: func_9EF0_b5(); break;
                case 6: func_9EF0_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x9FC5:
            switch (g_current_bank) {
                case 0: func_9FC5_b0(); break;
                case 1: func_9FC5_b1(); break;
                case 2: func_9FC5_b2(); break;
                case 3: func_9FC5_b3(); break;
                case 4: func_9FC5_b4(); break;
                case 5: func_9FC5_b5(); break;
                case 6: func_9FC5_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xEB7E:
            func_EB7E(); break;
        case 0xEB86:
            func_EB86(); break;
        case 0xEB8E:
            func_EB8E(); break;
        case 0xAA10:
            switch (g_current_bank) {
                case 0: func_AA10_b0(); break;
                case 1: func_AA10_b1(); break;
                case 2: func_AA10_b2(); break;
                case 3: func_AA10_b3(); break;
                case 4: func_AA10_b4(); break;
                case 5: func_AA10_b5(); break;
                case 6: func_AA10_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x841E:
            switch (g_current_bank) {
                case 0: func_841E_b0(); break;
                case 1: func_841E_b1(); break;
                case 2: func_841E_b2(); break;
                case 3: func_841E_b3(); break;
                case 4: func_841E_b4(); break;
                case 5: func_841E_b5(); break;
                case 6: func_841E_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x8056:
            func_8056_b5(); break;
        case 0x8038:
            func_8038_b5(); break;
        case 0x8057:
            func_8057_b5(); break;
        case 0x805E:
            switch (g_current_bank) {
                case 5: func_805E_b5(); break;
                case 3: func_805E_b3(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x8062:
            func_8062_b5(); break;
        case 0x8066:
            func_8066_b5(); break;
        case 0x8076:
            func_8076_b5(); break;
        case 0x80D4:
            func_80D4_b5(); break;
        case 0x810F:
            func_810F_b5(); break;
        case 0xE220:
            func_E220(); break;
        case 0x8130:
            func_8130_b5(); break;
        case 0xB23A:
            func_B23A_b5(); break;
        case 0x805B:
            func_805B_b5(); break;
        case 0xB282:
            func_B282_b5(); break;
        case 0x8079:
            func_8079_b5(); break;
        case 0x80B6:
            func_80B6_b5(); break;
        case 0xB6A7:
            func_B6A7_b5(); break;
        case 0xB726:
            func_B726_b5(); break;
        case 0xEBA3:
            func_EBA3(); break;
        case 0x8A8E:
            func_8A8E_b5(); break;
        case 0xE5F7:
            func_E5F7(); break;
        case 0xB5EB:
            switch (g_current_bank) {
                case 5: func_B5EB_b5(); break;
                case 0: func_B5EB_b0(); break;
                case 1: func_B5EB_b1(); break;
                case 2: func_B5EB_b2(); break;
                case 3: func_B5EB_b3(); break;
                case 4: func_B5EB_b4(); break;
                case 6: func_B5EB_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x81CE:
            func_81CE_b5(); break;
        case 0x8221:
            switch (g_current_bank) {
                case 5: func_8221_b5(); break;
                case 1: func_8221_b1(); break;
                case 4: func_8221_b4(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x8283:
            switch (g_current_bank) {
                case 5: func_8283_b5(); break;
                case 4: func_8283_b4(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x8225:
            func_8225_b5(); break;
        case 0x830E:
            switch (g_current_bank) {
                case 5: func_830E_b5(); break;
                case 6: func_830E_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x82BB:
            func_82BB_b5(); break;
        case 0x831F:
            func_831F_b5(); break;
        case 0x833D:
            func_833D_b5(); break;
        case 0x835C:
            func_835C_b5(); break;
        case 0x839E:
            func_839E_b5(); break;
        case 0xEA3D:
            func_EA3D(); break;
        case 0xF238:
            func_F238(); break;
        case 0x83C2:
            func_83C2_b5(); break;
        case 0xE68F:
            func_E68F(); break;
        case 0x83D7:
            switch (g_current_bank) {
                case 5: func_83D7_b5(); break;
                case 4: func_83D7_b4(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xA8C4:
            func_A8C4_b5(); break;
        case 0x82FB:
            func_82FB_b5(); break;
        case 0x82F1:
            func_82F1_b5(); break;
        case 0xA4F6:
            func_A4F6_b5(); break;
        case 0xFF34:
            func_FF34(); break;
        case 0xA924:
            func_A924_b5(); break;
        case 0x848C:
            func_848C_b5(); break;
        case 0x8CA4:
            func_8CA4_b5(); break;
        case 0xB68D:
            func_B68D_b5(); break;
        case 0x83B2:
            func_83B2_b5(); break;
        case 0x8404:
            switch (g_current_bank) {
                case 7: func_C404(); break;
                case 5: func_8404_b5(); break;
                case 4: func_8404_b4(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x8424:
            func_8424_b5(); break;
        case 0x8439:
            func_8439_b5(); break;
        case 0x847E:
            switch (g_current_bank) {
                case 5: func_847E_b5(); break;
                case 1: func_847E_b1(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x8492:
            func_8492_b5(); break;
        case 0x84A4:
            func_84A4_b5(); break;
        case 0x84DF:
            func_84DF_b5(); break;
        case 0x84E8:
            func_84E8_b5(); break;
        case 0x84BA:
            func_84BA_b5(); break;
        case 0x84CC:
            func_84CC_b5(); break;
        case 0x8452:
            func_8452_b5(); break;
        case 0x8484:
            func_8484_b5(); break;
        case 0x8501:
            func_8501_b5(); break;
        case 0x81DD:
            func_81DD_b5(); break;
        case 0x8506:
            switch (g_current_bank) {
                case 5: func_8506_b5(); break;
                case 4: func_8506_b4(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x8CA6:
            func_8CA6_b5(); break;
        case 0xA8DE:
            switch (g_current_bank) {
                case 5: func_A8DE_b5(); break;
                case 1: func_A8DE_b1(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xE46D:
            func_E46D(); break;
        case 0xEA2B:
            func_EA2B(); break;
        case 0xEA00:
            func_EA00(); break;
        case 0xEDF4:
            func_EDF4(); break;
        case 0xA5A8:
            func_A5A8_b5(); break;
        case 0xAC16:
            func_AC16_b5(); break;
        case 0xB560:
            func_B560_b5(); break;
        case 0xEA51:
            func_EA51(); break;
        case 0xE608:
            func_E608(); break;
        case 0xB0B6:
            switch (g_current_bank) {
                case 7: func_F0B6(); break;
                case 5: func_B0B6_b5(); break;
                case 1: func_B0B6_b1(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x8859:
            func_8859_b5(); break;
        case 0xB0C3:
            func_B0C3_b5(); break;
        case 0xB05E:
            switch (g_current_bank) {
                case 5: func_B05E_b5(); break;
                case 0: func_B05E_b0(); break;
                case 1: func_B05E_b1(); break;
                case 2: func_B05E_b2(); break;
                case 3: func_B05E_b3(); break;
                case 4: func_B05E_b4(); break;
                case 6: func_B05E_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xEEE4:
            func_EEE4(); break;
        case 0x90AE:
            func_90AE_b5(); break;
        case 0x88C7:
            func_88C7_b5(); break;
        case 0x9293:
            func_9293_b5(); break;
        case 0x8903:
            func_8903_b5(); break;
        case 0x896E:
            func_896E_b5(); break;
        case 0x8941:
            switch (g_current_bank) {
                case 5: func_8941_b5(); break;
                case 1: func_8941_b1(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xB366:
            func_B366_b5(); break;
        case 0xEAD3:
            func_EAD3(); break;
        case 0x8BAB:
            func_8BAB_b5(); break;
        case 0x895D:
            func_895D_b5(); break;
        case 0x9060:
            func_9060_b5(); break;
        case 0x8A04:
            func_8A04_b5(); break;
        case 0xFE98:
            func_FE98(); break;
        case 0xF23C:
            func_F23C(); break;
        case 0xE679:
            func_E679(); break;
        case 0x8929:
            switch (g_current_bank) {
                case 5: func_8929_b5(); break;
                case 0: func_8929_b0(); break;
                case 1: func_8929_b1(); break;
                case 2: func_8929_b2(); break;
                case 3: func_8929_b3(); break;
                case 4: func_8929_b4(); break;
                case 6: func_8929_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xE78A:
            func_E78A(); break;
        case 0xB559:
            switch (g_current_bank) {
                case 5: func_B559_b5(); break;
                case 0: func_B559_b0(); break;
                case 1: func_B559_b1(); break;
                case 2: func_B559_b2(); break;
                case 3: func_B559_b3(); break;
                case 4: func_B559_b4(); break;
                case 6: func_B559_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xE85A:
            func_E85A(); break;
        case 0xE6CE:
            func_E6CE(); break;
        case 0xA3F6:
            switch (g_current_bank) {
                case 5: func_A3F6_b5(); break;
                case 0: func_A3F6_b0(); break;
                case 1: func_A3F6_b1(); break;
                case 2: func_A3F6_b2(); break;
                case 3: func_A3F6_b3(); break;
                case 4: func_A3F6_b4(); break;
                case 6: func_A3F6_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x8BFA:
            func_8BFA_b5(); break;
        case 0x8BE7:
            func_8BE7_b5(); break;
        case 0x8BE6:
            switch (g_current_bank) {
                case 5: func_8BE6_b5(); break;
                case 1: func_8BE6_b1(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x8C1A:
            func_8C1A_b5(); break;
        case 0x8C28:
            func_8C28_b5(); break;
        case 0x8C6F:
            func_8C6F_b5(); break;
        case 0x8C4C:
            func_8C4C_b5(); break;
        case 0x8C53:
            func_8C53_b5(); break;
        case 0x8C76:
            func_8C76_b5(); break;
        case 0xE862:
            func_E862(); break;
        case 0x8CA1:
            func_8CA1_b5(); break;
        case 0x8C9C:
            func_8C9C_b5(); break;
        case 0x8CAC:
            switch (g_current_bank) {
                case 5: func_8CAC_b5(); break;
                case 4: func_8CAC_b4(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x8CBE:
            func_8CBE_b5(); break;
        case 0x8CC3:
            func_8CC3_b5(); break;
        case 0x8CC8:
            func_8CC8_b5(); break;
        case 0x8CD1:
            func_8CD1_b5(); break;
        case 0x8CDA:
            func_8CDA_b5(); break;
        case 0x8CFB:
            func_8CFB_b5(); break;
        case 0x8D01:
            switch (g_current_bank) {
                case 5: func_8D01_b5(); break;
                case 6: func_8D01_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x8D0D:
            func_8D0D_b5(); break;
        case 0x8D56:
            func_8D56_b5(); break;
        case 0x8D63:
            func_8D63_b5(); break;
        case 0x80A9:
            func_80A9_b5(); break;
        case 0xB0E1:
            func_B0E1_b5(); break;
        case 0xAC10:
            func_AC10_b5(); break;
        case 0xB12E:
            func_B12E_b5(); break;
        case 0x8CC0:
            switch (g_current_bank) {
                case 5: func_8CC0_b5(); break;
                case 0: func_8CC0_b0(); break;
                case 1: func_8CC0_b1(); break;
                case 2: func_8CC0_b2(); break;
                case 3: func_8CC0_b3(); break;
                case 4: func_8CC0_b4(); break;
                case 6: func_8CC0_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x8DC5:
            func_8DC5_b5(); break;
        case 0x8E8F:
            func_8E8F_b5(); break;
        case 0x8E38:
            func_8E38_b5(); break;
        case 0x8F05:
            func_8F05_b5(); break;
        case 0x8F58:
            func_8F58_b5(); break;
        case 0xE6C6:
            func_E6C6(); break;
        case 0xF161:
            func_F161(); break;
        case 0x8FA8:
            func_8FA8_b5(); break;
        case 0xB1E6:
            switch (g_current_bank) {
                case 7: func_F1E6(); break;
                case 0: func_B1E6_b0(); break;
                case 1: func_B1E6_b1(); break;
                case 2: func_B1E6_b2(); break;
                case 3: func_B1E6_b3(); break;
                case 4: func_B1E6_b4(); break;
                case 5: func_B1E6_b5(); break;
                case 6: func_B1E6_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xB4B5:
            func_B4B5_b1(); break;
        case 0xFEA6:
            func_FEA6(); break;
        case 0xEEE6:
            func_EEE6(); break;
        case 0xE580:
            func_E580(); break;
        case 0xE47D:
            func_E47D(); break;
        case 0xE594:
            func_E594(); break;
        case 0xFA89:
            func_FA89(); break;
        case 0xFA72:
            func_FA72(); break;
        case 0xA0A2:
            func_A0A2_b6(); break;
        case 0x9D46:
            func_9D46_b0(); break;
        case 0x9C2B:
            func_9C2B_b0(); break;
        case 0x9C54:
            func_9C54_b0(); break;
        case 0x9926:
            func_9926_b0(); break;
        case 0x9992:
            func_9992_b0(); break;
        case 0x9999:
            switch (g_current_bank) {
                case 0: func_9999_b0(); break;
                case 2: func_9999_b2(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x993A:
            func_993A_b0(); break;
        case 0x994B:
            switch (g_current_bank) {
                case 0: func_994B_b0(); break;
                case 4: func_994B_b4(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x9952:
            switch (g_current_bank) {
                case 0: func_9952_b0(); break;
                case 4: func_9952_b4(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x995D:
            func_995D_b0(); break;
        case 0x996E:
            func_996E_b0(); break;
        case 0x997B:
            func_997B_b0(); break;
        case 0x9985:
            func_9985_b0(); break;
        case 0x9C68:
            func_9C68_b0(); break;
        case 0x9C9F:
            func_9C9F_b0(); break;
        case 0x9EE6:
            func_9EE6_b0(); break;
        case 0x9F72:
            func_9F72_b0(); break;
        case 0x9C21:
            func_9C21_b0(); break;
        case 0x9F7C:
            func_9F7C_b0(); break;
        case 0x9C0D:
            func_9C0D_b0(); break;
        case 0x9C03:
            func_9C03_b0(); break;
        case 0x9E95:
            func_9E95_b0(); break;
        case 0x9E26:
            func_9E26_b0(); break;
        case 0x9C3F:
            func_9C3F_b0(); break;
        case 0x9EE0:
            func_9EE0_b0(); break;
        case 0x98C9:
            func_98C9_b0(); break;
        case 0x8CD4:
            func_8CD4_b4(); break;
        case 0x9081:
            func_9081_b4(); break;
        case 0x8D69:
            func_8D69_b4(); break;
        case 0x8C8D:
            switch (g_current_bank) {
                case 4: func_8C8D_b4(); break;
                case 1: func_8C8D_b1(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xFED6:
            func_FED6(); break;
        case 0x877C:
            func_877C_b4(); break;
        case 0xD00E:
            func_D00E(); break;
        case 0x86A3:
            switch (g_current_bank) {
                case 1: func_86A3_b1(); break;
                case 6: func_86A3_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x8736:
            func_8736_b1(); break;
        case 0x85F4:
            func_85F4_b1(); break;
        case 0x8749:
            func_8749_b1(); break;
        case 0x878B:
            func_878B_b1(); break;
        case 0x8815:
            func_8815_b1(); break;
        case 0x887A:
            func_887A_b1(); break;
        case 0x89F7:
            func_89F7_b1(); break;
        case 0x8933:
            func_8933_b1(); break;
        case 0x89F6:
            func_89F6_b1(); break;
        case 0x9320:
            func_9320_b1(); break;
        case 0xACFA:
            func_ACFA_b1(); break;
        case 0xE70E:
            func_E70E(); break;
        case 0x8801:
            func_8801_b1(); break;
        case 0x87BF:
            switch (g_current_bank) {
                case 1: func_87BF_b1(); break;
                case 0: func_87BF_b0(); break;
                case 2: func_87BF_b2(); break;
                case 3: func_87BF_b3(); break;
                case 4: func_87BF_b4(); break;
                case 5: func_87BF_b5(); break;
                case 6: func_87BF_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x87F7:
            func_87F7_b1(); break;
        case 0x89DE:
            func_89DE_b1(); break;
        case 0x87F2:
            func_87F2_b1(); break;
        case 0x892A:
            switch (g_current_bank) {
                case 1: func_892A_b1(); break;
                case 4: func_892A_b4(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x8797:
            switch (g_current_bank) {
                case 1: func_8797_b1(); break;
                case 4: func_8797_b4(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x89D6:
            func_89D6_b1(); break;
        case 0x8792:
            func_8792_b1(); break;
        case 0x89E6:
            switch (g_current_bank) {
                case 1: func_89E6_b1(); break;
                case 4: func_89E6_b4(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x8CAD:
            func_8CAD_b1(); break;
        case 0x8A23:
            func_8A23_b1(); break;
        case 0x8A69:
            func_8A69_b1(); break;
        case 0x8A84:
            switch (g_current_bank) {
                case 1: func_8A84_b1(); break;
                case 6: func_8A84_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xFEB1:
            func_FEB1(); break;
        case 0x8AD3:
            func_8AD3_b1(); break;
        case 0x8B11:
            func_8B11_b1(); break;
        case 0x8B9D:
            switch (g_current_bank) {
                case 7: func_CB9D(); break;
                case 1: func_8B9D_b1(); break;
                case 4: func_8B9D_b4(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x8AF8:
            func_8AF8_b1(); break;
        case 0x8AFF:
            func_8AFF_b1(); break;
        case 0x8B45:
            func_8B45_b1(); break;
        case 0x8B57:
            func_8B57_b1(); break;
        case 0x8B95:
            func_8B95_b1(); break;
        case 0x8B00:
            func_8B00_b1(); break;
        case 0xFA93:
            func_FA93(); break;
        case 0x8BCB:
            func_8BCB_b1(); break;
        case 0x8BEF:
            func_8BEF_b1(); break;
        case 0x9858:
            func_9858_b1(); break;
        case 0x8C4D:
            func_8C4D_b1(); break;
        case 0x8C7C:
            func_8C7C_b1(); break;
        case 0x8C99:
            func_8C99_b1(); break;
        case 0xF223:
            func_F223(); break;
        case 0x8D84:
            func_8D84_b1(); break;
        case 0x800A:
            func_800A_b1(); break;
        case 0x807D:
            switch (g_current_bank) {
                case 2: func_807D_b2(); break;
                case 3: func_807D_b3(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xEFD0:
            func_EFD0(); break;
        case 0x8190:
            func_8190_b4(); break;
        case 0x807E:
            func_807E_b5(); break;
        case 0x80C6:
            func_80C6_b6(); break;
        case 0x80D7:
            func_80D7_b6(); break;
        case 0xA28A:
            switch (g_current_bank) {
                case 0: func_A28A_b0(); break;
                case 1: func_A28A_b1(); break;
                case 2: func_A28A_b2(); break;
                case 3: func_A28A_b3(); break;
                case 4: func_A28A_b4(); break;
                case 5: func_A28A_b5(); break;
                case 6: func_A28A_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x8044:
            switch (g_current_bank) {
                case 0: func_8044_b0(); break;
                case 1: func_8044_b1(); break;
                case 2: func_8044_b2(); break;
                case 3: func_8044_b3(); break;
                case 4: func_8044_b4(); break;
                case 5: func_8044_b5(); break;
                case 6: func_8044_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x803E:
            switch (g_current_bank) {
                case 7: func_C03E(); break;
                case 0: func_803E_b0(); break;
                case 1: func_803E_b1(); break;
                case 2: func_803E_b2(); break;
                case 3: func_803E_b3(); break;
                case 4: func_803E_b4(); break;
                case 5: func_803E_b5(); break;
                case 6: func_803E_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xE9F7:
            func_E9F7(); break;
        case 0xEA11:
            func_EA11(); break;
        case 0xB000:
            switch (g_current_bank) {
                case 0: func_B000_b0(); break;
                case 1: func_B000_b1(); break;
                case 2: func_B000_b2(); break;
                case 3: func_B000_b3(); break;
                case 4: func_B000_b4(); break;
                case 5: func_B000_b5(); break;
                case 6: func_B000_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xB009:
            switch (g_current_bank) {
                case 7: func_F009(); break;
                case 0: func_B009_b0(); break;
                case 1: func_B009_b1(); break;
                case 2: func_B009_b2(); break;
                case 3: func_B009_b3(); break;
                case 4: func_B009_b4(); break;
                case 5: func_B009_b5(); break;
                case 6: func_B009_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xB010:
            switch (g_current_bank) {
                case 7: func_F010(); break;
                case 0: func_B010_b0(); break;
                case 1: func_B010_b1(); break;
                case 2: func_B010_b2(); break;
                case 3: func_B010_b3(); break;
                case 4: func_B010_b4(); break;
                case 5: func_B010_b5(); break;
                case 6: func_B010_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xB016:
            switch (g_current_bank) {
                case 0: func_B016_b0(); break;
                case 1: func_B016_b1(); break;
                case 2: func_B016_b2(); break;
                case 3: func_B016_b3(); break;
                case 4: func_B016_b4(); break;
                case 5: func_B016_b5(); break;
                case 6: func_B016_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xB01D:
            switch (g_current_bank) {
                case 7: func_F01D(); break;
                case 0: func_B01D_b0(); break;
                case 1: func_B01D_b1(); break;
                case 2: func_B01D_b2(); break;
                case 3: func_B01D_b3(); break;
                case 4: func_B01D_b4(); break;
                case 5: func_B01D_b5(); break;
                case 6: func_B01D_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xB02A:
            switch (g_current_bank) {
                case 7: func_F02A(); break;
                case 0: func_B02A_b0(); break;
                case 1: func_B02A_b1(); break;
                case 2: func_B02A_b2(); break;
                case 3: func_B02A_b3(); break;
                case 4: func_B02A_b4(); break;
                case 5: func_B02A_b5(); break;
                case 6: func_B02A_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xB036:
            switch (g_current_bank) {
                case 7: func_F036(); break;
                case 0: func_B036_b0(); break;
                case 1: func_B036_b1(); break;
                case 2: func_B036_b2(); break;
                case 3: func_B036_b3(); break;
                case 4: func_B036_b4(); break;
                case 5: func_B036_b5(); break;
                case 6: func_B036_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xC6C6:
            func_C6C6(); break;
        case 0x8753:
            func_8753_b3(); break;
        case 0xB20B:
            func_B20B_b4(); break;
        case 0xB83A:
            switch (g_current_bank) {
                case 0: func_B83A_b0(); break;
                case 1: func_B83A_b1(); break;
                case 2: func_B83A_b2(); break;
                case 3: func_B83A_b3(); break;
                case 4: func_B83A_b4(); break;
                case 5: func_B83A_b5(); break;
                case 6: func_B83A_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x9328:
            switch (g_current_bank) {
                case 0: func_9328_b0(); break;
                case 1: func_9328_b1(); break;
                case 2: func_9328_b2(); break;
                case 3: func_9328_b3(); break;
                case 4: func_9328_b4(); break;
                case 5: func_9328_b5(); break;
                case 6: func_9328_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xA121:
            switch (g_current_bank) {
                case 0: func_A121_b0(); break;
                case 1: func_A121_b1(); break;
                case 2: func_A121_b2(); break;
                case 3: func_A121_b3(); break;
                case 4: func_A121_b4(); break;
                case 5: func_A121_b5(); break;
                case 6: func_A121_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xB1F4:
            func_B1F4_b4(); break;
        case 0xB15F:
            func_B15F_b4(); break;
        case 0xB464:
            func_B464_b5(); break;
        case 0xEADD:
            func_EADD(); break;
        case 0x82A8:
            switch (g_current_bank) {
                case 0: func_82A8_b0(); break;
                case 1: func_82A8_b1(); break;
                case 2: func_82A8_b2(); break;
                case 3: func_82A8_b3(); break;
                case 4: func_82A8_b4(); break;
                case 5: func_82A8_b5(); break;
                case 6: func_82A8_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xEDFA:
            func_EDFA(); break;
        case 0xF08D:
            func_F08D(); break;
        case 0x8400:
            func_8400_b0(); break;
        case 0x8420:
            func_8420_b0(); break;
        case 0xF2F0:
            func_F2F0(); break;
        case 0x9D2C:
            func_9D2C_b0(); break;
        case 0x94EA:
            func_94EA_b2(); break;
        case 0x95A2:
            func_95A2_b2(); break;
        case 0x986B:
            func_986B_b2(); break;
        case 0x972A:
            func_972A_b2(); break;
        case 0x979D:
            func_979D_b2(); break;
        case 0x973A:
            func_973A_b2(); break;
        case 0x96A7:
            func_96A7_b2(); break;
        case 0x96A2:
            func_96A2_b2(); break;
        case 0x96C5:
            func_96C5_b2(); break;
        case 0x96DD:
            func_96DD_b2(); break;
        case 0x9724:
            func_9724_b2(); break;
        case 0x9747:
            func_9747_b2(); break;
        case 0x9792:
            switch (g_current_bank) {
                case 2: func_9792_b2(); break;
                case 5: func_9792_b5(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x97D8:
            func_97D8_b2(); break;
        case 0x97BF:
            func_97BF_b2(); break;
        case 0x97EA:
            func_97EA_b2(); break;
        case 0x98C3:
            func_98C3_b2(); break;
        case 0x98F5:
            func_98F5_b2(); break;
        case 0x9943:
            func_9943_b2(); break;
        case 0x9262:
            func_9262_b4(); break;
        case 0x9155:
            func_9155_b4(); break;
        case 0x90F5:
            func_90F5_b4(); break;
        case 0xB117:
            func_B117_b5(); break;
        case 0xB133:
            func_B133_b5(); break;
        case 0xB13C:
            func_B13C_b5(); break;
        case 0xAB62:
            func_AB62_b5(); break;
        case 0xB12C:
            func_B12C_b5(); break;
        case 0xB143:
            func_B143_b5(); break;
        case 0xB199:
            func_B199_b5(); break;
        case 0xB1CD:
            func_B1CD_b5(); break;
        case 0xB118:
            func_B118_b4(); break;
        case 0xB21D:
            func_B21D_b4(); break;
        case 0xB153:
            func_B153_b5(); break;
        case 0xB147:
            func_B147_b5(); break;
        case 0xAC20:
            func_AC20_b5(); break;
        case 0xB166:
            func_B166_b5(); break;
        case 0xB173:
            func_B173_b5(); break;
        case 0xB14B:
            func_B14B_b5(); break;
        case 0xB15A:
            func_B15A_b5(); break;
        case 0xB127:
            func_B127_b5(); break;
        case 0x9018:
            func_9018_b0(); break;
        case 0x9031:
            func_9031_b0(); break;
        case 0x9030:
            func_9030_b0(); break;
        case 0x9042:
            func_9042_b0(); break;
        case 0x9068:
            func_9068_b0(); break;
        case 0x906A:
            func_906A_b0(); break;
        case 0x9069:
            switch (g_current_bank) {
                case 2: func_9069_b2(); break;
                case 4: func_9069_b4(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x9094:
            switch (g_current_bank) {
                case 7: func_D094(); break;
                case 2: func_9094_b2(); break;
                case 5: func_9094_b5(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xB14F:
            func_B14F_b5(); break;
        case 0xB130:
            func_B130_b5(); break;
        case 0xAB4D:
            func_AB4D_b5(); break;
        case 0xB16B:
            func_B16B_b5(); break;
        case 0xAB5E:
            func_AB5E_b5(); break;
        case 0xE6E2:
            func_E6E2(); break;
        case 0x8619:
            func_8619_b1(); break;
        case 0x9E11:
            switch (g_current_bank) {
                case 0: func_9E11_b0(); break;
                case 1: func_9E11_b1(); break;
                case 2: func_9E11_b2(); break;
                case 3: func_9E11_b3(); break;
                case 4: func_9E11_b4(); break;
                case 5: func_9E11_b5(); break;
                case 6: func_9E11_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x8040:
            func_8040_b3(); break;
        case 0x89B9:
            func_89B9_b2(); break;
        case 0xA90F:
            switch (g_current_bank) {
                case 2: func_A90F_b2(); break;
                case 7: func_A90F_b7(); break;
                case 1: func_A90F_b1(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xA93F:
            func_A93F_b2(); break;
        case 0xA97F:
            func_A97F_b2(); break;
        case 0xA9F5:
            switch (g_current_bank) {
                case 2: func_A9F5_b2(); break;
                case 6: func_A9F5_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xA9FF:
            switch (g_current_bank) {
                case 2: func_A9FF_b2(); break;
                case 4: func_A9FF_b4(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xA921:
            func_A921_b2(); break;
        case 0xF231:
            func_F231(); break;
        case 0xE8D8:
            func_E8D8(); break;
        case 0xE80C:
            func_E80C(); break;
        case 0xA900:
            switch (g_current_bank) {
                case 0: func_A900_b0(); break;
                case 1: func_A900_b1(); break;
                case 2: func_A900_b2(); break;
                case 3: func_A900_b3(); break;
                case 4: func_A900_b4(); break;
                case 5: func_A900_b5(); break;
                case 6: func_A900_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xF714:
            func_F714(); break;
        case 0x804F:
            switch (g_current_bank) {
                case 7: func_C04F(); break;
                case 2: func_804F_b2(); break;
                case 0: func_804F_b0(); break;
                case 1: func_804F_b1(); break;
                case 3: func_804F_b3(); break;
                case 4: func_804F_b4(); break;
                case 5: func_804F_b5(); break;
                case 6: func_804F_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x8D3B:
            func_8D3B_b3(); break;
        case 0x8D41:
            func_8D41_b4(); break;
        case 0x8F90:
            func_8F90_b0(); break;
        case 0x8FB1:
            func_8FB1_b0(); break;
        case 0x8FF1:
            func_8FF1_b0(); break;
        case 0x8FFC:
            func_8FFC_b0(); break;
        case 0x94E7:
            func_94E7_b2(); break;
        case 0x9512:
            func_9512_b2(); break;
        case 0x9545:
            func_9545_b2(); break;
        case 0x9573:
            switch (g_current_bank) {
                case 2: func_9573_b2(); break;
                case 0: func_9573_b0(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x959A:
            func_959A_b2(); break;
        case 0x90CF:
            func_90CF_b2(); break;
        case 0x903B:
            func_903B_b2(); break;
        case 0xA2A0:
            func_A2A0_b2(); break;
        case 0x905D:
            func_905D_b2(); break;
        case 0xF8A5:
            func_F8A5(); break;
        case 0x8FCD:
            func_8FCD_b5(); break;
        case 0x9007:
            func_9007_b0(); break;
        case 0x9077:
            func_9077_b4(); break;
        case 0xA59A:
            func_A59A_b2(); break;
        case 0xA5DF:
            func_A5DF_b2(); break;
        case 0xA638:
            func_A638_b2(); break;
        case 0xA621:
            func_A621_b4(); break;
        case 0xA643:
            func_A643_b4(); break;
        case 0xA61D:
            func_A61D_b4(); break;
        case 0xA584:
            func_A584_b5(); break;
        case 0xA57D:
            func_A57D_b5(); break;
        case 0xA4FC:
            func_A4FC_b5(); break;
        case 0xA4F8:
            func_A4F8_b5(); break;
        case 0xA5C9:
            switch (g_current_bank) {
                case 5: func_A5C9_b5(); break;
                case 2: func_A5C9_b2(); break;
                case 4: func_A5C9_b4(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x8109:
            switch (g_current_bank) {
                case 0: func_8109_b0(); break;
                case 1: func_8109_b1(); break;
                case 2: func_8109_b2(); break;
                case 3: func_8109_b3(); break;
                case 4: func_8109_b4(); break;
                case 5: func_8109_b5(); break;
                case 6: func_8109_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xEBF2:
            func_EBF2(); break;
        case 0xEBEA:
            func_EBEA(); break;
        case 0xEDC5:
            func_EDC5(); break;
        case 0xEBA1:
            func_EBA1(); break;
        case 0x8BA8:
            switch (g_current_bank) {
                case 0: func_8BA8_b0(); break;
                case 1: func_8BA8_b1(); break;
                case 2: func_8BA8_b2(); break;
                case 3: func_8BA8_b3(); break;
                case 4: func_8BA8_b4(); break;
                case 5: func_8BA8_b5(); break;
                case 6: func_8BA8_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xEBF8:
            func_EBF8(); break;
        case 0xB1BE:
            switch (g_current_bank) {
                case 0: func_B1BE_b0(); break;
                case 1: func_B1BE_b1(); break;
                case 2: func_B1BE_b2(); break;
                case 3: func_B1BE_b3(); break;
                case 4: func_B1BE_b4(); break;
                case 5: func_B1BE_b5(); break;
                case 6: func_B1BE_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xEDA0:
            func_EDA0(); break;
        case 0xED7D:
            func_ED7D(); break;
        case 0xF769:
            func_F769(); break;
        case 0xF36F:
            func_F36F(); break;
        case 0xF4A0:
            func_F4A0(); break;
        case 0xF92A:
            func_F92A(); break;
        case 0xF73E:
            func_F73E(); break;
        case 0xFB74:
            func_FB74(); break;
        case 0x8650:
            switch (g_current_bank) {
                case 0: func_8650_b0(); break;
                case 1: func_8650_b1(); break;
                case 2: func_8650_b2(); break;
                case 3: func_8650_b3(); break;
                case 4: func_8650_b4(); break;
                case 5: func_8650_b5(); break;
                case 6: func_8650_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x8BBC:
            switch (g_current_bank) {
                case 0: func_8BBC_b0(); break;
                case 1: func_8BBC_b1(); break;
                case 2: func_8BBC_b2(); break;
                case 3: func_8BBC_b3(); break;
                case 4: func_8BBC_b4(); break;
                case 5: func_8BBC_b5(); break;
                case 6: func_8BBC_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x8AAD:
            switch (g_current_bank) {
                case 0: func_8AAD_b0(); break;
                case 1: func_8AAD_b1(); break;
                case 2: func_8AAD_b2(); break;
                case 3: func_8AAD_b3(); break;
                case 4: func_8AAD_b4(); break;
                case 5: func_8AAD_b5(); break;
                case 6: func_8AAD_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xA5F3:
            switch (g_current_bank) {
                case 0: func_A5F3_b0(); break;
                case 1: func_A5F3_b1(); break;
                case 2: func_A5F3_b2(); break;
                case 3: func_A5F3_b3(); break;
                case 4: func_A5F3_b4(); break;
                case 5: func_A5F3_b5(); break;
                case 6: func_A5F3_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xA17C:
            switch (g_current_bank) {
                case 0: func_A17C_b0(); break;
                case 1: func_A17C_b1(); break;
                case 2: func_A17C_b2(); break;
                case 3: func_A17C_b3(); break;
                case 4: func_A17C_b4(); break;
                case 5: func_A17C_b5(); break;
                case 6: func_A17C_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xED6A:
            func_ED6A(); break;
        case 0x86E0:
            switch (g_current_bank) {
                case 0: func_86E0_b0(); break;
                case 1: func_86E0_b1(); break;
                case 2: func_86E0_b2(); break;
                case 3: func_86E0_b3(); break;
                case 4: func_86E0_b4(); break;
                case 5: func_86E0_b5(); break;
                case 6: func_86E0_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xE7FE:
            func_E7FE(); break;
        case 0xE6EA:
            func_E6EA(); break;
        case 0x8AF4:
            switch (g_current_bank) {
                case 0: func_8AF4_b0(); break;
                case 1: func_8AF4_b1(); break;
                case 2: func_8AF4_b2(); break;
                case 3: func_8AF4_b3(); break;
                case 4: func_8AF4_b4(); break;
                case 5: func_8AF4_b5(); break;
                case 6: func_8AF4_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xA6B8:
            func_A6B8_b2(); break;
        case 0xA734:
            func_A734_b2(); break;
        case 0xA803:
            func_A803_b2(); break;
        case 0x9D2A:
            func_9D2A_b2(); break;
        case 0xA437:
            func_A437_b2(); break;
        case 0xA80C:
            func_A80C_b2(); break;
        case 0xA819:
            func_A819_b2(); break;
        case 0xA3C8:
            func_A3C8_b2(); break;
        case 0xA635:
            func_A635_b3(); break;
        case 0xA5B4:
            func_A5B4_b5(); break;
        case 0x9FB0:
            func_9FB0_b2(); break;
        case 0x9F0D:
            func_9F0D_b2(); break;
        case 0x9FDE:
            func_9FDE_b2(); break;
        case 0xA204:
            func_A204_b2(); break;
        case 0xA35D:
            func_A35D_b2(); break;
        case 0x8B8B:
            switch (g_current_bank) {
                case 0: func_8B8B_b0(); break;
                case 1: func_8B8B_b1(); break;
                case 2: func_8B8B_b2(); break;
                case 3: func_8B8B_b3(); break;
                case 4: func_8B8B_b4(); break;
                case 5: func_8B8B_b5(); break;
                case 6: func_8B8B_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x8C7D:
            switch (g_current_bank) {
                case 0: func_8C7D_b0(); break;
                case 1: func_8C7D_b1(); break;
                case 2: func_8C7D_b2(); break;
                case 3: func_8C7D_b3(); break;
                case 4: func_8C7D_b4(); break;
                case 5: func_8C7D_b5(); break;
                case 6: func_8C7D_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xA858:
            switch (g_current_bank) {
                case 7: func_E858(); break;
                case 0: func_A858_b0(); break;
                case 1: func_A858_b1(); break;
                case 2: func_A858_b2(); break;
                case 3: func_A858_b3(); break;
                case 4: func_A858_b4(); break;
                case 5: func_A858_b5(); break;
                case 6: func_A858_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xAA23:
            func_AA23_b2(); break;
        case 0xAAA9:
            func_AAA9_b2(); break;
        case 0xABC4:
            switch (g_current_bank) {
                case 2: func_ABC4_b2(); break;
                case 0: func_ABC4_b0(); break;
                case 1: func_ABC4_b1(); break;
                case 3: func_ABC4_b3(); break;
                case 4: func_ABC4_b4(); break;
                case 5: func_ABC4_b5(); break;
                case 6: func_ABC4_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xAB7F:
            func_AB7F_b2(); break;
        case 0xAA62:
            func_AA62_b2(); break;
        case 0xAA33:
            func_AA33_b2(); break;
        case 0xA9EA:
            func_A9EA_b4(); break;
        case 0xFEDA:
            func_FEDA(); break;
        case 0xAC07:
            switch (g_current_bank) {
                case 5: func_AC07_b5(); break;
                case 0: func_AC07_b0(); break;
                case 1: func_AC07_b1(); break;
                case 2: func_AC07_b2(); break;
                case 3: func_AC07_b3(); break;
                case 4: func_AC07_b4(); break;
                case 6: func_AC07_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xAABF:
            func_AABF_b5(); break;
        case 0xAAF1:
            func_AAF1_b5(); break;
        case 0xAA2E:
            func_AA2E_b5(); break;
        case 0xDBDE:
            func_DBDE(); break;
        case 0xDBDC:
            func_DBDC(); break;
        case 0xCE4D:
            func_CE4D(); break;
        case 0xB601:
            func_B601_b6(); break;
        case 0x8059:
            func_8059_b5(); break;
        case 0xF003:
            func_F003(); break;
        case 0xB522:
            switch (g_current_bank) {
                case 0: func_B522_b0(); break;
                case 1: func_B522_b1(); break;
                case 2: func_B522_b2(); break;
                case 3: func_B522_b3(); break;
                case 4: func_B522_b4(); break;
                case 5: func_B522_b5(); break;
                case 6: func_B522_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xB517:
            switch (g_current_bank) {
                case 0: func_B517_b0(); break;
                case 1: func_B517_b1(); break;
                case 2: func_B517_b2(); break;
                case 3: func_B517_b3(); break;
                case 4: func_B517_b4(); break;
                case 5: func_B517_b5(); break;
                case 6: func_B517_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x98A4:
            switch (g_current_bank) {
                case 0: func_98A4_b0(); break;
                case 1: func_98A4_b1(); break;
                case 2: func_98A4_b2(); break;
                case 3: func_98A4_b3(); break;
                case 4: func_98A4_b4(); break;
                case 5: func_98A4_b5(); break;
                case 6: func_98A4_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xE70C:
            func_E70C(); break;
        case 0xE73A:
            func_E73A(); break;
        case 0xE76A:
            func_E76A(); break;
        case 0xB81C:
            switch (g_current_bank) {
                case 7: func_F81C(); break;
                case 0: func_B81C_b0(); break;
                case 1: func_B81C_b1(); break;
                case 2: func_B81C_b2(); break;
                case 3: func_B81C_b3(); break;
                case 4: func_B81C_b4(); break;
                case 5: func_B81C_b5(); break;
                case 6: func_B81C_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xE7D7:
            func_E7D7(); break;
        case 0xE7CB:
            func_E7CB(); break;
        case 0xB7C8:
            switch (g_current_bank) {
                case 7: func_F7C8(); break;
                case 0: func_B7C8_b0(); break;
                case 1: func_B7C8_b1(); break;
                case 2: func_B7C8_b2(); break;
                case 3: func_B7C8_b3(); break;
                case 4: func_B7C8_b4(); break;
                case 5: func_B7C8_b5(); break;
                case 6: func_B7C8_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xB29A:
            switch (g_current_bank) {
                case 0: func_B29A_b0(); break;
                case 1: func_B29A_b1(); break;
                case 2: func_B29A_b2(); break;
                case 3: func_B29A_b3(); break;
                case 4: func_B29A_b4(); break;
                case 5: func_B29A_b5(); break;
                case 6: func_B29A_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xEE48:
            func_EE48(); break;
        case 0xEDD7:
            func_EDD7(); break;
        case 0xEF61:
            func_EF61(); break;
        case 0xA0A8:
            switch (g_current_bank) {
                case 0: func_A0A8_b0(); break;
                case 1: func_A0A8_b1(); break;
                case 2: func_A0A8_b2(); break;
                case 3: func_A0A8_b3(); break;
                case 4: func_A0A8_b4(); break;
                case 5: func_A0A8_b5(); break;
                case 6: func_A0A8_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xEEB8:
            func_EEB8(); break;
        case 0xA13E:
            switch (g_current_bank) {
                case 0: func_A13E_b0(); break;
                case 1: func_A13E_b1(); break;
                case 2: func_A13E_b2(); break;
                case 3: func_A13E_b3(); break;
                case 4: func_A13E_b4(); break;
                case 5: func_A13E_b5(); break;
                case 6: func_A13E_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x8ED7:
            switch (g_current_bank) {
                case 0: func_8ED7_b0(); break;
                case 1: func_8ED7_b1(); break;
                case 2: func_8ED7_b2(); break;
                case 3: func_8ED7_b3(); break;
                case 4: func_8ED7_b4(); break;
                case 5: func_8ED7_b5(); break;
                case 6: func_8ED7_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x917C:
            switch (g_current_bank) {
                case 0: func_917C_b0(); break;
                case 1: func_917C_b1(); break;
                case 2: func_917C_b2(); break;
                case 3: func_917C_b3(); break;
                case 4: func_917C_b4(); break;
                case 5: func_917C_b5(); break;
                case 6: func_917C_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xF0E3:
            func_F0E3(); break;
        case 0x8F7B:
            switch (g_current_bank) {
                case 0: func_8F7B_b0(); break;
                case 1: func_8F7B_b1(); break;
                case 2: func_8F7B_b2(); break;
                case 3: func_8F7B_b3(); break;
                case 4: func_8F7B_b4(); break;
                case 5: func_8F7B_b5(); break;
                case 6: func_8F7B_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xF0AC:
            func_F0AC(); break;
        case 0xF14E:
            func_F14E(); break;
        case 0xF129:
            func_F129(); break;
        case 0xF1D0:
            func_F1D0(); break;
        case 0xF198:
            func_F198(); break;
        case 0xF1A0:
            func_F1A0(); break;
        case 0xA223:
            switch (g_current_bank) {
                case 0: func_A223_b0(); break;
                case 1: func_A223_b1(); break;
                case 2: func_A223_b2(); break;
                case 3: func_A223_b3(); break;
                case 4: func_A223_b4(); break;
                case 5: func_A223_b5(); break;
                case 6: func_A223_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xF149:
            func_F149(); break;
        case 0xF1AF:
            func_F1AF(); break;
        case 0xF1C3:
            func_F1C3(); break;
        case 0xF1DA:
            func_F1DA(); break;
        case 0xF21E:
            func_F21E(); break;
        case 0xF2E3:
            func_F2E3(); break;
        case 0xFEBB:
            func_FEBB(); break;
        case 0xB3F4:
            switch (g_current_bank) {
                case 7: func_F3F4(); break;
                case 0: func_B3F4_b0(); break;
                case 1: func_B3F4_b1(); break;
                case 2: func_B3F4_b2(); break;
                case 3: func_B3F4_b3(); break;
                case 4: func_B3F4_b4(); break;
                case 5: func_B3F4_b5(); break;
                case 6: func_B3F4_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xFA3C:
            func_FA3C(); break;
        case 0xB291:
            func_B291_b5(); break;
        case 0xB249:
            func_B249_b5(); break;
        case 0xB57F:
            func_B57F_b5(); break;
        case 0xB258:
            func_B258_b5(); break;
        case 0xB25B:
            func_B25B_b5(); break;
        case 0xE735:
            func_E735(); break;
        case 0xB5EF:
            func_B5EF_b5(); break;
        case 0xB821:
            func_B821_b5(); break;
        case 0xB7E0:
            func_B7E0_b5(); break;
        case 0xB7E5:
            func_B7E5_b5(); break;
        case 0x81B1:
            func_81B1_b5(); break;
        case 0x81AD:
            switch (g_current_bank) {
                case 5: func_81AD_b5(); break;
                case 3: func_81AD_b3(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xF1FC:
            func_F1FC(); break;
        case 0xF35A:
            func_F35A(); break;
        case 0xF41D:
            func_F41D(); break;
        case 0xF855:
            func_F855(); break;
        case 0xF455:
            func_F455(); break;
        case 0xF411:
            func_F411(); break;
        case 0xF5B6:
            func_F5B6(); break;
        case 0xF5E1:
            func_F5E1(); break;
        case 0xF6B8:
            func_F6B8(); break;
        case 0x8346:
            switch (g_current_bank) {
                case 0: func_8346_b0(); break;
                case 1: func_8346_b1(); break;
                case 2: func_8346_b2(); break;
                case 3: func_8346_b3(); break;
                case 4: func_8346_b4(); break;
                case 5: func_8346_b5(); break;
                case 6: func_8346_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xF6D3:
            func_F6D3(); break;
        case 0xF592:
            func_F592(); break;
        case 0xF720:
            func_F720(); break;
        case 0xF519:
            func_F519(); break;
        case 0xAB30:
            func_AB30_b5(); break;
        case 0xA8BE:
            switch (g_current_bank) {
                case 5: func_A8BE_b5(); break;
                case 1: func_A8BE_b1(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xB66C:
            func_B66C_b5(); break;
        case 0xA442:
            func_A442_b5(); break;
        case 0xA720:
            func_A720_b5(); break;
        case 0x8A97:
            func_8A97_b5(); break;
        case 0xB641:
            func_B641_b5(); break;
        case 0xFF28:
            func_FF28(); break;
        case 0xFF0A:
            func_FF0A(); break;
        case 0x83F1:
            func_83F1_b5(); break;
        case 0xB566:
            func_B566_b5(); break;
        case 0x905A:
            func_905A_b5(); break;
        case 0x9029:
            func_9029_b5(); break;
        case 0xE783:
            func_E783(); break;
        case 0x8C26:
            func_8C26_b5(); break;
        case 0x8C1F:
            func_8C1F_b5(); break;
        case 0x8CD7:
            func_8CD7_b5(); break;
        case 0x8D55:
            func_8D55_b5(); break;
        case 0x901F:
            func_901F_b5(); break;
        case 0xB1DD:
            func_B1DD_b4(); break;
        case 0xFAA0:
            func_FAA0(); break;
        case 0x9C20:
            func_9C20_b0(); break;
        case 0x9959:
            func_9959_b0(); break;
        case 0x9C9D:
            func_9C9D_b0(); break;
        case 0xD008:
            func_D008(); break;
        case 0xFE00:
            func_FE00(); break;
        case 0xFFE0:
            func_FFE0(); break;
        case 0xF21B:
            func_F21B(); break;
        case 0x893A:
            func_893A_b1(); break;
        case 0xE6E8:
            func_E6E8(); break;
        case 0x87A2:
            func_87A2_b1(); break;
        case 0xEFCF:
            func_EFCF(); break;
        case 0xB179:
            func_B179_b4(); break;
        case 0x80D9:
            func_80D9_b6(); break;
        case 0xA47F:
            func_A47F_b2(); break;
        case 0xA4BD:
            func_A4BD_b2(); break;
        case 0xA4C4:
            func_A4C4_b2(); break;
        case 0xA51E:
            func_A51E_b2(); break;
        case 0xA3E1:
            func_A3E1_b2(); break;
        case 0xA764:
            func_A764_b2(); break;
        case 0xA2FC:
            func_A2FC_b2(); break;
        case 0xA307:
            func_A307_b2(); break;
        case 0xA287:
            switch (g_current_bank) {
                case 4: func_A287_b4(); break;
                case 2: func_A287_b2(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xD005:
            func_D005(); break;
        case 0xD7DC:
            func_D7DC(); break;
        case 0xD8DE:
            func_D8DE(); break;
        case 0xDADC:
            func_DADC(); break;
        case 0xD9DC:
            func_D9DC(); break;
        case 0xDDDC:
            func_DDDC(); break;
        case 0xD8DC:
            func_D8DC(); break;
        case 0xD9E1:
            func_D9E1(); break;
        case 0xD8ED:
            func_D8ED(); break;
        case 0xED20:
            func_ED20(); break;
        case 0xDAED:
            func_DAED(); break;
        case 0xD9ED:
            func_D9ED(); break;
        case 0xDBDF:
            func_DBDF(); break;
        case 0xF3FC:
            func_F3FC(); break;
        case 0x8091:
            func_8091_b3(); break;
        case 0x80DC:
            func_80DC_b3(); break;
        case 0x8080:
            func_8080_b3(); break;
        case 0x80CC:
            func_80CC_b3(); break;
        case 0x80AC:
            func_80AC_b3(); break;
        case 0x80BC:
            func_80BC_b3(); break;
        case 0x8067:
            func_8067_b6(); break;
        case 0x80A4:
            func_80A4_b6(); break;
        case 0x8018:
            func_8018_b2(); break;
        case 0x8047:
            func_8047_b6(); break;
        case 0x8070:
            func_8070_b6(); break;
        case 0xB00D:
            func_B00D_b5(); break;
        case 0xFA4F:
            func_FA4F(); break;
        case 0xB01A:
            func_B01A_b5(); break;
        case 0xB018:
            func_B018_b5(); break;
        case 0xBF98:
            switch (g_current_bank) {
                case 7: func_FF98(); break;
                case 0: func_BF98_b0(); break;
                case 1: func_BF98_b1(); break;
                case 2: func_BF98_b2(); break;
                case 3: func_BF98_b3(); break;
                case 4: func_BF98_b4(); break;
                case 6: func_BF98_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xBFAC:
            switch (g_current_bank) {
                case 7: func_FFAC(); break;
                case 0: func_BFAC_b0(); break;
                case 1: func_BFAC_b1(); break;
                case 2: func_BFAC_b2(); break;
                case 3: func_BFAC_b3(); break;
                case 4: func_BFAC_b4(); break;
                case 6: func_BFAC_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xA7F1:
            func_A7F1_b5(); break;
        case 0xA18E:
            func_A18E_b2(); break;
        case 0xA011:
            func_A011_b2(); break;
        case 0xA1C3:
            func_A1C3_b2(); break;
        case 0xA1DD:
            func_A1DD_b2(); break;
        case 0xA10B:
            func_A10B_b2(); break;
        case 0xA017:
            func_A017_b2(); break;
        case 0xA000:
            func_A000_b1(); break;
        case 0xA1D7:
            func_A1D7_b2(); break;
        case 0x90E5:
            func_90E5_b5(); break;
        case 0x92D7:
            func_92D7_b5(); break;
        case 0x979F:
            func_979F_b2(); break;
        case 0x87B3:
            func_87B3_b5(); break;
        case 0xAB4F:
            func_AB4F_b5(); break;
        case 0xB135:
            func_B135_b5(); break;
        case 0x87C6:
            switch (g_current_bank) {
                case 5: func_87C6_b5(); break;
                case 1: func_87C6_b1(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xB235:
            func_B235_b4(); break;
        case 0xB288:
            func_B288_b4(); break;
        case 0xB16D:
            func_B16D_b5(); break;
        case 0xB17B:
            func_B17B_b5(); break;
        case 0xAA1B:
            func_AA1B_b5(); break;
        case 0x9DA7:
            func_9DA7_b3(); break;
        case 0x9DA9:
            func_9DA9_b3(); break;
        case 0x9EBC:
            func_9EBC_b4(); break;
        case 0xA995:
            func_A995_b2(); break;
        case 0xA9F4:
            switch (g_current_bank) {
                case 2: func_A9F4_b2(); break;
                case 5: func_A9F4_b5(); break;
                case 3: func_A9F4_b3(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xA8E6:
            func_A8E6_b4(); break;
        case 0x831E:
            func_831E_b0(); break;
        case 0x8F6F:
            func_8F6F_b0(); break;
        case 0x8F71:
            func_8F71_b0(); break;
        case 0x95AC:
            func_95AC_b2(); break;
        case 0x9B79:
            func_9B79_b2(); break;
        case 0x95C1:
            func_95C1_b2(); break;
        case 0x95EC:
            func_95EC_b2(); break;
        case 0x960C:
            func_960C_b2(); break;
        case 0x9830:
            func_9830_b2(); break;
        case 0x983C:
            func_983C_b2(); break;
        case 0xCA77:
            func_CA77(); break;
        case 0xD0C2:
            func_D0C2(); break;
        case 0xCC77:
            func_CC77(); break;
        case 0xC8C2:
            func_C8C2(); break;
        case 0xF871:
            func_F871(); break;
        case 0xF87A:
            func_F87A(); break;
        case 0xF90D:
            func_F90D(); break;
        case 0xA687:
            func_A687_b4(); break;
        case 0xA69C:
            func_A69C_b4(); break;
        case 0xA6B5:
            func_A6B5_b4(); break;
        case 0xA69D:
            switch (g_current_bank) {
                case 4: func_A69D_b4(); break;
                case 5: func_A69D_b5(); break;
                case 1: func_A69D_b1(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xA6C5:
            func_A6C5_b4(); break;
        case 0xA604:
            func_A604_b4(); break;
        case 0xC66C:
            func_C66C(); break;
        case 0xBD00:
            func_BD00_b2(); break;
        case 0xBF9D:
            func_BF9D_b2(); break;
        case 0xC483:
            func_C483(); break;
        case 0x80F0:
            func_80F0_b3(); break;
        case 0x80EC:
            func_80EC_b4(); break;
        case 0x80D0:
            func_80D0_b4(); break;
        case 0x8BA2:
            func_8BA2_b3(); break;
        case 0x8AA6:
            func_8AA6_b4(); break;
        case 0xB1CA:
            func_B1CA_b4(); break;
        case 0xB1E4:
            func_B1E4_b4(); break;
        case 0xB1DC:
            func_B1DC_b4(); break;
        case 0xEBA4:
            func_EBA4(); break;
        case 0xEA20:
            func_EA20(); break;
        case 0xF0B1:
            func_F0B1(); break;
        case 0xA90B:
            func_A90B_b4(); break;
        case 0x8DC0:
            func_8DC0_b4(); break;
        case 0xF7F2:
            func_F7F2(); break;
        case 0xF36A:
            func_F36A(); break;
        case 0xF49F:
            func_F49F(); break;
        case 0xF886:
            func_F886(); break;
        case 0xF9BF:
            func_F9BF(); break;
        case 0xF9E5:
            func_F9E5(); break;
        case 0xF9C2:
            func_F9C2(); break;
        case 0xF9C9:
            func_F9C9(); break;
        case 0xFA1B:
            func_FA1B(); break;
        case 0xFAAE:
            func_FAAE(); break;
        case 0xFAD3:
            func_FAD3(); break;
        case 0xFCDA:
            func_FCDA(); break;
        case 0xFC88:
            func_FC88(); break;
        case 0x86DB:
            switch (g_current_bank) {
                case 0: func_86DB_b0(); break;
                case 1: func_86DB_b1(); break;
                case 2: func_86DB_b2(); break;
                case 3: func_86DB_b3(); break;
                case 4: func_86DB_b4(); break;
                case 5: func_86DB_b5(); break;
                case 6: func_86DB_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xFC94:
            func_FC94(); break;
        case 0x87CC:
            switch (g_current_bank) {
                case 0: func_87CC_b0(); break;
                case 1: func_87CC_b1(); break;
                case 2: func_87CC_b2(); break;
                case 3: func_87CC_b3(); break;
                case 4: func_87CC_b4(); break;
                case 5: func_87CC_b5(); break;
                case 6: func_87CC_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x8119:
            switch (g_current_bank) {
                case 0: func_8119_b0(); break;
                case 1: func_8119_b1(); break;
                case 2: func_8119_b2(); break;
                case 3: func_8119_b3(); break;
                case 4: func_8119_b4(); break;
                case 5: func_8119_b5(); break;
                case 6: func_8119_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x8BFF:
            switch (g_current_bank) {
                case 0: func_8BFF_b0(); break;
                case 1: func_8BFF_b1(); break;
                case 2: func_8BFF_b2(); break;
                case 3: func_8BFF_b3(); break;
                case 4: func_8BFF_b4(); break;
                case 5: func_8BFF_b5(); break;
                case 6: func_8BFF_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x9B4B:
            switch (g_current_bank) {
                case 0: func_9B4B_b0(); break;
                case 1: func_9B4B_b1(); break;
                case 2: func_9B4B_b2(); break;
                case 3: func_9B4B_b3(); break;
                case 4: func_9B4B_b4(); break;
                case 5: func_9B4B_b5(); break;
                case 6: func_9B4B_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x88F8:
            switch (g_current_bank) {
                case 0: func_88F8_b0(); break;
                case 1: func_88F8_b1(); break;
                case 2: func_88F8_b2(); break;
                case 3: func_88F8_b3(); break;
                case 4: func_88F8_b4(); break;
                case 5: func_88F8_b5(); break;
                case 6: func_88F8_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x8A65:
            switch (g_current_bank) {
                case 0: func_8A65_b0(); break;
                case 1: func_8A65_b1(); break;
                case 2: func_8A65_b2(); break;
                case 3: func_8A65_b3(); break;
                case 4: func_8A65_b4(); break;
                case 5: func_8A65_b5(); break;
                case 6: func_8A65_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x8B09:
            switch (g_current_bank) {
                case 0: func_8B09_b0(); break;
                case 1: func_8B09_b1(); break;
                case 2: func_8B09_b2(); break;
                case 3: func_8B09_b3(); break;
                case 4: func_8B09_b4(); break;
                case 5: func_8B09_b5(); break;
                case 6: func_8B09_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x8798:
            switch (g_current_bank) {
                case 0: func_8798_b0(); break;
                case 1: func_8798_b1(); break;
                case 2: func_8798_b2(); break;
                case 3: func_8798_b3(); break;
                case 4: func_8798_b4(); break;
                case 5: func_8798_b5(); break;
                case 6: func_8798_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x9D57:
            switch (g_current_bank) {
                case 0: func_9D57_b0(); break;
                case 1: func_9D57_b1(); break;
                case 2: func_9D57_b2(); break;
                case 3: func_9D57_b3(); break;
                case 4: func_9D57_b4(); break;
                case 5: func_9D57_b5(); break;
                case 6: func_9D57_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x84CB:
            switch (g_current_bank) {
                case 0: func_84CB_b0(); break;
                case 1: func_84CB_b1(); break;
                case 2: func_84CB_b2(); break;
                case 3: func_84CB_b3(); break;
                case 4: func_84CB_b4(); break;
                case 5: func_84CB_b5(); break;
                case 6: func_84CB_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x8568:
            switch (g_current_bank) {
                case 0: func_8568_b0(); break;
                case 1: func_8568_b1(); break;
                case 2: func_8568_b2(); break;
                case 3: func_8568_b3(); break;
                case 4: func_8568_b4(); break;
                case 5: func_8568_b5(); break;
                case 6: func_8568_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x9BA2:
            switch (g_current_bank) {
                case 0: func_9BA2_b0(); break;
                case 1: func_9BA2_b1(); break;
                case 2: func_9BA2_b2(); break;
                case 3: func_9BA2_b3(); break;
                case 4: func_9BA2_b4(); break;
                case 5: func_9BA2_b5(); break;
                case 6: func_9BA2_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x9CC9:
            switch (g_current_bank) {
                case 0: func_9CC9_b0(); break;
                case 1: func_9CC9_b1(); break;
                case 2: func_9CC9_b2(); break;
                case 3: func_9CC9_b3(); break;
                case 4: func_9CC9_b4(); break;
                case 5: func_9CC9_b5(); break;
                case 6: func_9CC9_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x95F0:
            switch (g_current_bank) {
                case 0: func_95F0_b0(); break;
                case 1: func_95F0_b1(); break;
                case 2: func_95F0_b2(); break;
                case 3: func_95F0_b3(); break;
                case 4: func_95F0_b4(); break;
                case 5: func_95F0_b5(); break;
                case 6: func_95F0_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x9028:
            switch (g_current_bank) {
                case 7: func_D028(); break;
                case 0: func_9028_b0(); break;
                case 1: func_9028_b1(); break;
                case 2: func_9028_b2(); break;
                case 3: func_9028_b3(); break;
                case 4: func_9028_b4(); break;
                case 5: func_9028_b5(); break;
                case 6: func_9028_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x8481:
            switch (g_current_bank) {
                case 0: func_8481_b0(); break;
                case 1: func_8481_b1(); break;
                case 2: func_8481_b2(); break;
                case 3: func_8481_b3(); break;
                case 4: func_8481_b4(); break;
                case 5: func_8481_b5(); break;
                case 6: func_8481_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x8D50:
            switch (g_current_bank) {
                case 0: func_8D50_b0(); break;
                case 1: func_8D50_b1(); break;
                case 2: func_8D50_b2(); break;
                case 3: func_8D50_b3(); break;
                case 4: func_8D50_b4(); break;
                case 5: func_8D50_b5(); break;
                case 6: func_8D50_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x8886:
            switch (g_current_bank) {
                case 0: func_8886_b0(); break;
                case 1: func_8886_b1(); break;
                case 2: func_8886_b2(); break;
                case 3: func_8886_b3(); break;
                case 4: func_8886_b4(); break;
                case 5: func_8886_b5(); break;
                case 6: func_8886_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x8C6B:
            switch (g_current_bank) {
                case 0: func_8C6B_b0(); break;
                case 1: func_8C6B_b1(); break;
                case 2: func_8C6B_b2(); break;
                case 3: func_8C6B_b3(); break;
                case 4: func_8C6B_b4(); break;
                case 5: func_8C6B_b5(); break;
                case 6: func_8C6B_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x8FEF:
            switch (g_current_bank) {
                case 0: func_8FEF_b0(); break;
                case 1: func_8FEF_b1(); break;
                case 2: func_8FEF_b2(); break;
                case 3: func_8FEF_b3(); break;
                case 4: func_8FEF_b4(); break;
                case 5: func_8FEF_b5(); break;
                case 6: func_8FEF_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x9DE3:
            switch (g_current_bank) {
                case 0: func_9DE3_b0(); break;
                case 1: func_9DE3_b1(); break;
                case 2: func_9DE3_b2(); break;
                case 3: func_9DE3_b3(); break;
                case 4: func_9DE3_b4(); break;
                case 5: func_9DE3_b5(); break;
                case 6: func_9DE3_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x9FA9:
            switch (g_current_bank) {
                case 0: func_9FA9_b0(); break;
                case 1: func_9FA9_b1(); break;
                case 2: func_9FA9_b2(); break;
                case 3: func_9FA9_b3(); break;
                case 4: func_9FA9_b4(); break;
                case 5: func_9FA9_b5(); break;
                case 6: func_9FA9_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xABDA:
            switch (g_current_bank) {
                case 7: func_EBDA(); break;
                case 0: func_ABDA_b0(); break;
                case 1: func_ABDA_b1(); break;
                case 2: func_ABDA_b2(); break;
                case 3: func_ABDA_b3(); break;
                case 4: func_ABDA_b4(); break;
                case 5: func_ABDA_b5(); break;
                case 6: func_ABDA_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x90E4:
            switch (g_current_bank) {
                case 0: func_90E4_b0(); break;
                case 1: func_90E4_b1(); break;
                case 2: func_90E4_b2(); break;
                case 3: func_90E4_b3(); break;
                case 4: func_90E4_b4(); break;
                case 5: func_90E4_b5(); break;
                case 6: func_90E4_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x92D6:
            switch (g_current_bank) {
                case 0: func_92D6_b0(); break;
                case 1: func_92D6_b1(); break;
                case 2: func_92D6_b2(); break;
                case 3: func_92D6_b3(); break;
                case 4: func_92D6_b4(); break;
                case 5: func_92D6_b5(); break;
                case 6: func_92D6_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x9377:
            switch (g_current_bank) {
                case 0: func_9377_b0(); break;
                case 1: func_9377_b1(); break;
                case 2: func_9377_b2(); break;
                case 3: func_9377_b3(); break;
                case 4: func_9377_b4(); break;
                case 5: func_9377_b5(); break;
                case 6: func_9377_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x843E:
            switch (g_current_bank) {
                case 0: func_843E_b0(); break;
                case 1: func_843E_b1(); break;
                case 2: func_843E_b2(); break;
                case 3: func_843E_b3(); break;
                case 4: func_843E_b4(); break;
                case 5: func_843E_b5(); break;
                case 6: func_843E_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xFE40:
            func_FE40(); break;
        case 0x8D90:
            switch (g_current_bank) {
                case 0: func_8D90_b0(); break;
                case 1: func_8D90_b1(); break;
                case 2: func_8D90_b2(); break;
                case 3: func_8D90_b3(); break;
                case 4: func_8D90_b4(); break;
                case 5: func_8D90_b5(); break;
                case 6: func_8D90_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x9B36:
            switch (g_current_bank) {
                case 7: func_DB36(); break;
                case 0: func_9B36_b0(); break;
                case 1: func_9B36_b1(); break;
                case 2: func_9B36_b2(); break;
                case 3: func_9B36_b3(); break;
                case 4: func_9B36_b4(); break;
                case 5: func_9B36_b5(); break;
                case 6: func_9B36_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x98A9:
            switch (g_current_bank) {
                case 0: func_98A9_b0(); break;
                case 1: func_98A9_b1(); break;
                case 2: func_98A9_b2(); break;
                case 3: func_98A9_b3(); break;
                case 4: func_98A9_b4(); break;
                case 5: func_98A9_b5(); break;
                case 6: func_98A9_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xA316:
            switch (g_current_bank) {
                case 0: func_A316_b0(); break;
                case 1: func_A316_b1(); break;
                case 2: func_A316_b2(); break;
                case 3: func_A316_b3(); break;
                case 4: func_A316_b4(); break;
                case 5: func_A316_b5(); break;
                case 6: func_A316_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xFE50:
            func_FE50(); break;
        case 0xFE90:
            func_FE90(); break;
        case 0xA987:
            switch (g_current_bank) {
                case 0: func_A987_b0(); break;
                case 1: func_A987_b1(); break;
                case 2: func_A987_b2(); break;
                case 3: func_A987_b3(); break;
                case 4: func_A987_b4(); break;
                case 5: func_A987_b5(); break;
                case 6: func_A987_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xA9EB:
            switch (g_current_bank) {
                case 0: func_A9EB_b0(); break;
                case 1: func_A9EB_b1(); break;
                case 2: func_A9EB_b2(); break;
                case 3: func_A9EB_b3(); break;
                case 4: func_A9EB_b4(); break;
                case 5: func_A9EB_b5(); break;
                case 6: func_A9EB_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xA187:
            switch (g_current_bank) {
                case 0: func_A187_b0(); break;
                case 1: func_A187_b1(); break;
                case 2: func_A187_b2(); break;
                case 3: func_A187_b3(); break;
                case 4: func_A187_b4(); break;
                case 5: func_A187_b5(); break;
                case 6: func_A187_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x9786:
            switch (g_current_bank) {
                case 0: func_9786_b0(); break;
                case 1: func_9786_b1(); break;
                case 2: func_9786_b2(); break;
                case 3: func_9786_b3(); break;
                case 4: func_9786_b4(); break;
                case 5: func_9786_b5(); break;
                case 6: func_9786_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xAC7D:
            switch (g_current_bank) {
                case 0: func_AC7D_b0(); break;
                case 1: func_AC7D_b1(); break;
                case 2: func_AC7D_b2(); break;
                case 3: func_AC7D_b3(); break;
                case 4: func_AC7D_b4(); break;
                case 5: func_AC7D_b5(); break;
                case 6: func_AC7D_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xA9D8:
            switch (g_current_bank) {
                case 7: func_E9D8(); break;
                case 0: func_A9D8_b0(); break;
                case 1: func_A9D8_b1(); break;
                case 2: func_A9D8_b2(); break;
                case 3: func_A9D8_b3(); break;
                case 4: func_A9D8_b4(); break;
                case 5: func_A9D8_b5(); break;
                case 6: func_A9D8_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x9498:
            switch (g_current_bank) {
                case 0: func_9498_b0(); break;
                case 1: func_9498_b1(); break;
                case 2: func_9498_b2(); break;
                case 3: func_9498_b3(); break;
                case 4: func_9498_b4(); break;
                case 5: func_9498_b5(); break;
                case 6: func_9498_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xA4DE:
            switch (g_current_bank) {
                case 0: func_A4DE_b0(); break;
                case 1: func_A4DE_b1(); break;
                case 2: func_A4DE_b2(); break;
                case 3: func_A4DE_b3(); break;
                case 4: func_A4DE_b4(); break;
                case 5: func_A4DE_b5(); break;
                case 6: func_A4DE_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xA477:
            switch (g_current_bank) {
                case 0: func_A477_b0(); break;
                case 1: func_A477_b1(); break;
                case 2: func_A477_b2(); break;
                case 3: func_A477_b3(); break;
                case 4: func_A477_b4(); break;
                case 5: func_A477_b5(); break;
                case 6: func_A477_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xAB66:
            switch (g_current_bank) {
                case 0: func_AB66_b0(); break;
                case 1: func_AB66_b1(); break;
                case 2: func_AB66_b2(); break;
                case 3: func_AB66_b3(); break;
                case 4: func_AB66_b4(); break;
                case 5: func_AB66_b5(); break;
                case 6: func_AB66_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xFE60:
            func_FE60(); break;
        case 0xFE80:
            func_FE80(); break;
        case 0xFE88:
            func_FE88(); break;
        case 0x82D7:
            switch (g_current_bank) {
                case 0: func_82D7_b0(); break;
                case 1: func_82D7_b1(); break;
                case 2: func_82D7_b2(); break;
                case 3: func_82D7_b3(); break;
                case 4: func_82D7_b4(); break;
                case 5: func_82D7_b5(); break;
                case 6: func_82D7_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x8399:
            switch (g_current_bank) {
                case 0: func_8399_b0(); break;
                case 1: func_8399_b1(); break;
                case 2: func_8399_b2(); break;
                case 3: func_8399_b3(); break;
                case 4: func_8399_b4(); break;
                case 5: func_8399_b5(); break;
                case 6: func_8399_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x883F:
            switch (g_current_bank) {
                case 0: func_883F_b0(); break;
                case 1: func_883F_b1(); break;
                case 2: func_883F_b2(); break;
                case 3: func_883F_b3(); break;
                case 4: func_883F_b4(); break;
                case 5: func_883F_b5(); break;
                case 6: func_883F_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xFEF4:
            func_FEF4(); break;
        case 0x8F76:
            switch (g_current_bank) {
                case 0: func_8F76_b0(); break;
                case 1: func_8F76_b1(); break;
                case 2: func_8F76_b2(); break;
                case 3: func_8F76_b3(); break;
                case 4: func_8F76_b4(); break;
                case 5: func_8F76_b5(); break;
                case 6: func_8F76_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x8E44:
            switch (g_current_bank) {
                case 0: func_8E44_b0(); break;
                case 1: func_8E44_b1(); break;
                case 2: func_8E44_b2(); break;
                case 3: func_8E44_b3(); break;
                case 4: func_8E44_b4(); break;
                case 5: func_8E44_b5(); break;
                case 6: func_8E44_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x8EEB:
            switch (g_current_bank) {
                case 0: func_8EEB_b0(); break;
                case 1: func_8EEB_b1(); break;
                case 2: func_8EEB_b2(); break;
                case 3: func_8EEB_b3(); break;
                case 4: func_8EEB_b4(); break;
                case 5: func_8EEB_b5(); break;
                case 6: func_8EEB_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x8F0E:
            switch (g_current_bank) {
                case 0: func_8F0E_b0(); break;
                case 1: func_8F0E_b1(); break;
                case 2: func_8F0E_b2(); break;
                case 3: func_8F0E_b3(); break;
                case 4: func_8F0E_b4(); break;
                case 5: func_8F0E_b5(); break;
                case 6: func_8F0E_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x81F7:
            switch (g_current_bank) {
                case 0: func_81F7_b0(); break;
                case 1: func_81F7_b1(); break;
                case 2: func_81F7_b2(); break;
                case 3: func_81F7_b3(); break;
                case 4: func_81F7_b4(); break;
                case 5: func_81F7_b5(); break;
                case 6: func_81F7_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xFA0C:
            func_FA0C(); break;
        case 0xB06A:
            switch (g_current_bank) {
                case 0: func_B06A_b0(); break;
                case 1: func_B06A_b1(); break;
                case 2: func_B06A_b2(); break;
                case 3: func_B06A_b3(); break;
                case 4: func_B06A_b4(); break;
                case 5: func_B06A_b5(); break;
                case 6: func_B06A_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x9024:
            switch (g_current_bank) {
                case 0: func_9024_b0(); break;
                case 1: func_9024_b1(); break;
                case 2: func_9024_b2(); break;
                case 3: func_9024_b3(); break;
                case 4: func_9024_b4(); break;
                case 5: func_9024_b5(); break;
                case 6: func_9024_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x8A13:
            switch (g_current_bank) {
                case 0: func_8A13_b0(); break;
                case 1: func_8A13_b1(); break;
                case 2: func_8A13_b2(); break;
                case 3: func_8A13_b3(); break;
                case 4: func_8A13_b4(); break;
                case 5: func_8A13_b5(); break;
                case 6: func_8A13_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x85D0:
            switch (g_current_bank) {
                case 0: func_85D0_b0(); break;
                case 1: func_85D0_b1(); break;
                case 2: func_85D0_b2(); break;
                case 3: func_85D0_b3(); break;
                case 4: func_85D0_b4(); break;
                case 5: func_85D0_b5(); break;
                case 6: func_85D0_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xFECA:
            func_FECA(); break;
        case 0x8025:
            switch (g_current_bank) {
                case 0: func_8025_b0(); break;
                case 1: func_8025_b1(); break;
                case 2: func_8025_b2(); break;
                case 3: func_8025_b3(); break;
                case 4: func_8025_b4(); break;
                case 5: func_8025_b5(); break;
                case 6: func_8025_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x8755:
            switch (g_current_bank) {
                case 0: func_8755_b0(); break;
                case 1: func_8755_b1(); break;
                case 2: func_8755_b2(); break;
                case 3: func_8755_b3(); break;
                case 4: func_8755_b4(); break;
                case 5: func_8755_b5(); break;
                case 6: func_8755_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x8759:
            switch (g_current_bank) {
                case 0: func_8759_b0(); break;
                case 1: func_8759_b1(); break;
                case 2: func_8759_b2(); break;
                case 3: func_8759_b3(); break;
                case 4: func_8759_b4(); break;
                case 5: func_8759_b5(); break;
                case 6: func_8759_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x9B20:
            switch (g_current_bank) {
                case 7: func_DB20(); break;
                case 0: func_9B20_b0(); break;
                case 1: func_9B20_b1(); break;
                case 2: func_9B20_b2(); break;
                case 3: func_9B20_b3(); break;
                case 4: func_9B20_b4(); break;
                case 5: func_9B20_b5(); break;
                case 6: func_9B20_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x873F:
            switch (g_current_bank) {
                case 0: func_873F_b0(); break;
                case 1: func_873F_b1(); break;
                case 2: func_873F_b2(); break;
                case 3: func_873F_b3(); break;
                case 4: func_873F_b4(); break;
                case 5: func_873F_b5(); break;
                case 6: func_873F_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x874E:
            switch (g_current_bank) {
                case 0: func_874E_b0(); break;
                case 1: func_874E_b1(); break;
                case 2: func_874E_b2(); break;
                case 3: func_874E_b3(); break;
                case 4: func_874E_b4(); break;
                case 5: func_874E_b5(); break;
                case 6: func_874E_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x8561:
            switch (g_current_bank) {
                case 7: func_C561(); break;
                case 0: func_8561_b0(); break;
                case 1: func_8561_b1(); break;
                case 2: func_8561_b2(); break;
                case 3: func_8561_b3(); break;
                case 4: func_8561_b4(); break;
                case 5: func_8561_b5(); break;
                case 6: func_8561_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x8775:
            switch (g_current_bank) {
                case 0: func_8775_b0(); break;
                case 1: func_8775_b1(); break;
                case 2: func_8775_b2(); break;
                case 3: func_8775_b3(); break;
                case 4: func_8775_b4(); break;
                case 5: func_8775_b5(); break;
                case 6: func_8775_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x841D:
            switch (g_current_bank) {
                case 0: func_841D_b0(); break;
                case 1: func_841D_b1(); break;
                case 2: func_841D_b2(); break;
                case 3: func_841D_b3(); break;
                case 4: func_841D_b4(); break;
                case 5: func_841D_b5(); break;
                case 6: func_841D_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x8435:
            switch (g_current_bank) {
                case 0: func_8435_b0(); break;
                case 1: func_8435_b1(); break;
                case 2: func_8435_b2(); break;
                case 3: func_8435_b3(); break;
                case 4: func_8435_b4(); break;
                case 5: func_8435_b5(); break;
                case 6: func_8435_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x8737:
            switch (g_current_bank) {
                case 0: func_8737_b0(); break;
                case 1: func_8737_b1(); break;
                case 2: func_8737_b2(); break;
                case 3: func_8737_b3(); break;
                case 4: func_8737_b4(); break;
                case 5: func_8737_b5(); break;
                case 6: func_8737_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x873C:
            switch (g_current_bank) {
                case 0: func_873C_b0(); break;
                case 1: func_873C_b1(); break;
                case 2: func_873C_b2(); break;
                case 3: func_873C_b3(); break;
                case 4: func_873C_b4(); break;
                case 5: func_873C_b5(); break;
                case 6: func_873C_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x92C2:
            switch (g_current_bank) {
                case 0: func_92C2_b0(); break;
                case 1: func_92C2_b1(); break;
                case 2: func_92C2_b2(); break;
                case 3: func_92C2_b3(); break;
                case 4: func_92C2_b4(); break;
                case 5: func_92C2_b5(); break;
                case 6: func_92C2_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x8415:
            switch (g_current_bank) {
                case 0: func_8415_b0(); break;
                case 1: func_8415_b1(); break;
                case 2: func_8415_b2(); break;
                case 3: func_8415_b3(); break;
                case 4: func_8415_b4(); break;
                case 5: func_8415_b5(); break;
                case 6: func_8415_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x878A:
            switch (g_current_bank) {
                case 0: func_878A_b0(); break;
                case 1: func_878A_b1(); break;
                case 2: func_878A_b2(); break;
                case 3: func_878A_b3(); break;
                case 4: func_878A_b4(); break;
                case 5: func_878A_b5(); break;
                case 6: func_878A_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x9487:
            switch (g_current_bank) {
                case 0: func_9487_b0(); break;
                case 1: func_9487_b1(); break;
                case 2: func_9487_b2(); break;
                case 3: func_9487_b3(); break;
                case 4: func_9487_b4(); break;
                case 5: func_9487_b5(); break;
                case 6: func_9487_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xA161:
            switch (g_current_bank) {
                case 0: func_A161_b0(); break;
                case 1: func_A161_b1(); break;
                case 2: func_A161_b2(); break;
                case 3: func_A161_b3(); break;
                case 4: func_A161_b4(); break;
                case 5: func_A161_b5(); break;
                case 6: func_A161_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xFE48:
            func_FE48(); break;
        case 0xFE78:
            func_FE78(); break;
        case 0xA8CA:
            switch (g_current_bank) {
                case 7: func_E8CA(); break;
                case 0: func_A8CA_b0(); break;
                case 1: func_A8CA_b1(); break;
                case 2: func_A8CA_b2(); break;
                case 3: func_A8CA_b3(); break;
                case 4: func_A8CA_b4(); break;
                case 5: func_A8CA_b5(); break;
                case 6: func_A8CA_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x945A:
            switch (g_current_bank) {
                case 0: func_945A_b0(); break;
                case 1: func_945A_b1(); break;
                case 2: func_945A_b2(); break;
                case 3: func_945A_b3(); break;
                case 4: func_945A_b4(); break;
                case 5: func_945A_b5(); break;
                case 6: func_945A_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x9479:
            switch (g_current_bank) {
                case 0: func_9479_b0(); break;
                case 1: func_9479_b1(); break;
                case 2: func_9479_b2(); break;
                case 3: func_9479_b3(); break;
                case 4: func_9479_b4(); break;
                case 5: func_9479_b5(); break;
                case 6: func_9479_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xA8E4:
            switch (g_current_bank) {
                case 0: func_A8E4_b0(); break;
                case 1: func_A8E4_b1(); break;
                case 2: func_A8E4_b2(); break;
                case 3: func_A8E4_b3(); break;
                case 4: func_A8E4_b4(); break;
                case 5: func_A8E4_b5(); break;
                case 6: func_A8E4_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xA10C:
            switch (g_current_bank) {
                case 0: func_A10C_b0(); break;
                case 1: func_A10C_b1(); break;
                case 2: func_A10C_b2(); break;
                case 3: func_A10C_b3(); break;
                case 4: func_A10C_b4(); break;
                case 5: func_A10C_b5(); break;
                case 6: func_A10C_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x9447:
            switch (g_current_bank) {
                case 0: func_9447_b0(); break;
                case 1: func_9447_b1(); break;
                case 2: func_9447_b2(); break;
                case 3: func_9447_b3(); break;
                case 4: func_9447_b4(); break;
                case 5: func_9447_b5(); break;
                case 6: func_9447_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xA96F:
            switch (g_current_bank) {
                case 7: func_E96F(); break;
                case 0: func_A96F_b0(); break;
                case 1: func_A96F_b1(); break;
                case 2: func_A96F_b2(); break;
                case 3: func_A96F_b3(); break;
                case 4: func_A96F_b4(); break;
                case 5: func_A96F_b5(); break;
                case 6: func_A96F_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x93E2:
            switch (g_current_bank) {
                case 0: func_93E2_b0(); break;
                case 1: func_93E2_b1(); break;
                case 2: func_93E2_b2(); break;
                case 3: func_93E2_b3(); break;
                case 4: func_93E2_b4(); break;
                case 5: func_93E2_b5(); break;
                case 6: func_93E2_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xA08D:
            switch (g_current_bank) {
                case 0: func_A08D_b0(); break;
                case 1: func_A08D_b1(); break;
                case 2: func_A08D_b2(); break;
                case 3: func_A08D_b3(); break;
                case 4: func_A08D_b4(); break;
                case 5: func_A08D_b5(); break;
                case 6: func_A08D_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xA179:
            switch (g_current_bank) {
                case 0: func_A179_b0(); break;
                case 1: func_A179_b1(); break;
                case 2: func_A179_b2(); break;
                case 3: func_A179_b3(); break;
                case 4: func_A179_b4(); break;
                case 5: func_A179_b5(); break;
                case 6: func_A179_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xA92E:
            switch (g_current_bank) {
                case 0: func_A92E_b0(); break;
                case 1: func_A92E_b1(); break;
                case 2: func_A92E_b2(); break;
                case 3: func_A92E_b3(); break;
                case 4: func_A92E_b4(); break;
                case 5: func_A92E_b5(); break;
                case 6: func_A92E_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xFE58:
            func_FE58(); break;
        case 0xFE68:
            func_FE68(); break;
        case 0xFE70:
            func_FE70(); break;
        case 0x8015:
            switch (g_current_bank) {
                case 0: func_8015_b0(); break;
                case 1: func_8015_b1(); break;
                case 2: func_8015_b2(); break;
                case 3: func_8015_b3(); break;
                case 4: func_8015_b4(); break;
                case 5: func_8015_b5(); break;
                case 6: func_8015_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x8019:
            switch (g_current_bank) {
                case 0: func_8019_b0(); break;
                case 1: func_8019_b1(); break;
                case 2: func_8019_b2(); break;
                case 3: func_8019_b3(); break;
                case 4: func_8019_b4(); break;
                case 5: func_8019_b5(); break;
                case 6: func_8019_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xFED1:
            func_FED1(); break;
        case 0xA018:
            switch (g_current_bank) {
                case 0: func_A018_b0(); break;
                case 1: func_A018_b1(); break;
                case 2: func_A018_b2(); break;
                case 3: func_A018_b3(); break;
                case 4: func_A018_b4(); break;
                case 5: func_A018_b5(); break;
                case 6: func_A018_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xA2E2:
            switch (g_current_bank) {
                case 0: func_A2E2_b0(); break;
                case 1: func_A2E2_b1(); break;
                case 2: func_A2E2_b2(); break;
                case 3: func_A2E2_b3(); break;
                case 4: func_A2E2_b4(); break;
                case 5: func_A2E2_b5(); break;
                case 6: func_A2E2_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xA3EE:
            switch (g_current_bank) {
                case 0: func_A3EE_b0(); break;
                case 1: func_A3EE_b1(); break;
                case 2: func_A3EE_b2(); break;
                case 3: func_A3EE_b3(); break;
                case 4: func_A3EE_b4(); break;
                case 5: func_A3EE_b5(); break;
                case 6: func_A3EE_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xA31A:
            switch (g_current_bank) {
                case 0: func_A31A_b0(); break;
                case 1: func_A31A_b1(); break;
                case 2: func_A31A_b2(); break;
                case 3: func_A31A_b3(); break;
                case 4: func_A31A_b4(); break;
                case 5: func_A31A_b5(); break;
                case 6: func_A31A_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xA34A:
            switch (g_current_bank) {
                case 0: func_A34A_b0(); break;
                case 1: func_A34A_b1(); break;
                case 2: func_A34A_b2(); break;
                case 3: func_A34A_b3(); break;
                case 4: func_A34A_b4(); break;
                case 5: func_A34A_b5(); break;
                case 6: func_A34A_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x89FC:
            switch (g_current_bank) {
                case 7: func_C9FC(); break;
                case 0: func_89FC_b0(); break;
                case 1: func_89FC_b1(); break;
                case 2: func_89FC_b2(); break;
                case 3: func_89FC_b3(); break;
                case 4: func_89FC_b4(); break;
                case 5: func_89FC_b5(); break;
                case 6: func_89FC_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x8A40:
            switch (g_current_bank) {
                case 0: func_8A40_b0(); break;
                case 1: func_8A40_b1(); break;
                case 2: func_8A40_b2(); break;
                case 3: func_8A40_b3(); break;
                case 4: func_8A40_b4(); break;
                case 5: func_8A40_b5(); break;
                case 6: func_8A40_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x8AB4:
            switch (g_current_bank) {
                case 0: func_8AB4_b0(); break;
                case 1: func_8AB4_b1(); break;
                case 2: func_8AB4_b2(); break;
                case 3: func_8AB4_b3(); break;
                case 4: func_8AB4_b4(); break;
                case 5: func_8AB4_b5(); break;
                case 6: func_8AB4_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x8AD9:
            switch (g_current_bank) {
                case 0: func_8AD9_b0(); break;
                case 1: func_8AD9_b1(); break;
                case 2: func_8AD9_b2(); break;
                case 3: func_8AD9_b3(); break;
                case 4: func_8AD9_b4(); break;
                case 5: func_8AD9_b5(); break;
                case 6: func_8AD9_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x8BA6:
            switch (g_current_bank) {
                case 0: func_8BA6_b0(); break;
                case 1: func_8BA6_b1(); break;
                case 2: func_8BA6_b2(); break;
                case 3: func_8BA6_b3(); break;
                case 4: func_8BA6_b4(); break;
                case 5: func_8BA6_b5(); break;
                case 6: func_8BA6_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x8C5A:
            switch (g_current_bank) {
                case 0: func_8C5A_b0(); break;
                case 1: func_8C5A_b1(); break;
                case 2: func_8C5A_b2(); break;
                case 3: func_8C5A_b3(); break;
                case 4: func_8C5A_b4(); break;
                case 5: func_8C5A_b5(); break;
                case 6: func_8C5A_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x8F32:
            switch (g_current_bank) {
                case 0: func_8F32_b0(); break;
                case 1: func_8F32_b1(); break;
                case 2: func_8F32_b2(); break;
                case 3: func_8F32_b3(); break;
                case 4: func_8F32_b4(); break;
                case 5: func_8F32_b5(); break;
                case 6: func_8F32_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x82AF:
            func_82AF_b4(); break;
        case 0x8BB1:
            func_8BB1_b3(); break;
        case 0x9603:
            func_9603_b6(); break;
        case 0xA5E9:
            func_A5E9_b4(); break;
        case 0xA5D4:
            func_A5D4_b5(); break;
        case 0xA682:
            func_A682_b5(); break;
        case 0xA6B1:
            func_A6B1_b5(); break;
        case 0x9400:
            switch (g_current_bank) {
                case 5: func_9400_b5(); break;
                case 1: func_9400_b1(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x8AA0:
            func_8AA0_b5(); break;
        case 0xA680:
            func_A680_b5(); break;
        case 0xA1D6:
            func_A1D6_b1(); break;
        case 0xFEB3:
            func_FEB3(); break;
        case 0xE6E9:
            func_E6E9(); break;
        case 0xE6DD:
            func_E6DD(); break;
        case 0x8BDA:
            func_8BDA_b4(); break;
        case 0x8BF6:
            func_8BF6_b4(); break;
        case 0x8094:
            func_8094_b4(); break;
        case 0x87D3:
            func_87D3_b4(); break;
        case 0x806E:
            switch (g_current_bank) {
                case 4: func_806E_b4(); break;
                case 5: func_806E_b5(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xB223:
            func_B223_b4(); break;
        case 0xF213:
            func_F213(); break;
        case 0x8DEA:
            func_8DEA_b4(); break;
        case 0xB459:
            func_B459_b4(); break;
        case 0xB3BD:
            func_B3BD_b4(); break;
        case 0x8F5A:
            func_8F5A_b4(); break;
        case 0x8F3F:
            func_8F3F_b4(); break;
        case 0x8F34:
            func_8F34_b4(); break;
        case 0x8EE0:
            func_8EE0_b4(); break;
        case 0x8FD8:
            func_8FD8_b4(); break;
        case 0x9005:
            func_9005_b4(); break;
        case 0xB266:
            func_B266_b4(); break;
        case 0x9017:
            switch (g_current_bank) {
                case 7: func_D017(); break;
                case 4: func_9017_b4(); break;
                case 5: func_9017_b5(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xB308:
            func_B308_b4(); break;
        case 0xB378:
            func_B378_b4(); break;
        case 0xB260:
            func_B260_b4(); break;
        case 0xB256:
            func_B256_b4(); break;
        case 0x903F:
            func_903F_b4(); break;
        case 0x9057:
            func_9057_b4(); break;
        case 0x917D:
            func_917D_b4(); break;
        case 0x91D3:
            func_91D3_b4(); break;
        case 0x828A:
            func_828A_b4(); break;
        case 0x9242:
            func_9242_b4(); break;
        case 0x929E:
            func_929E_b4(); break;
        case 0x8293:
            func_8293_b4(); break;
        case 0x91FF:
            func_91FF_b4(); break;
        case 0x92A5:
            switch (g_current_bank) {
                case 4: func_92A5_b4(); break;
                case 0: func_92A5_b0(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xF1DF:
            func_F1DF(); break;
        case 0x8476:
            func_8476_b4(); break;
        case 0x9502:
            func_9502_b4(); break;
        case 0x979A:
            func_979A_b4(); break;
        case 0x9510:
            func_9510_b4(); break;
        case 0x952B:
            func_952B_b4(); break;
        case 0x956C:
            func_956C_b4(); break;
        case 0x9621:
            func_9621_b4(); break;
        case 0x9658:
            func_9658_b4(); break;
        case 0xB31A:
            func_B31A_b4(); break;
        case 0x9610:
            func_9610_b4(); break;
        case 0x968E:
            func_968E_b4(); break;
        case 0xB388:
            func_B388_b4(); break;
        case 0x96EB:
            func_96EB_b4(); break;
        case 0xB2CD:
            func_B2CD_b4(); break;
        case 0x9748:
            func_9748_b4(); break;
        case 0x9698:
            func_9698_b4(); break;
        case 0x96B8:
            func_96B8_b4(); break;
        case 0x96DA:
            func_96DA_b4(); break;
        case 0x9AFB:
            func_9AFB_b4(); break;
        case 0x97A2:
            func_97A2_b4(); break;
        case 0x97EF:
            func_97EF_b4(); break;
        case 0x984C:
            func_984C_b4(); break;
        case 0x800D:
            func_800D_b4(); break;
        case 0x982F:
            func_982F_b4(); break;
        case 0x9893:
            func_9893_b4(); break;
        case 0x98BD:
            func_98BD_b4(); break;
        case 0x995F:
            func_995F_b4(); break;
        case 0x99B9:
            func_99B9_b4(); break;
        case 0x9A84:
            func_9A84_b4(); break;
        case 0x98C8:
            func_98C8_b4(); break;
        case 0x9900:
            switch (g_current_bank) {
                case 4: func_9900_b4(); break;
                case 2: func_9900_b2(); break;
                case 3: func_9900_b3(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x98F2:
            func_98F2_b4(); break;
        case 0x9910:
            switch (g_current_bank) {
                case 4: func_9910_b4(); break;
                case 2: func_9910_b2(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x9958:
            func_9958_b4(); break;
        case 0x994E:
            func_994E_b4(); break;
        case 0x998A:
            func_998A_b4(); break;
        case 0x9A32:
            func_9A32_b4(); break;
        case 0x9AC3:
            func_9AC3_b4(); break;
        case 0x9AE1:
            switch (g_current_bank) {
                case 4: func_9AE1_b4(); break;
                case 6: func_9AE1_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xA852:
            func_A852_b4(); break;
        case 0xA84E:
            func_A84E_b4(); break;
        case 0xE817:
            func_E817(); break;
        case 0xA86D:
            func_A86D_b5(); break;
        case 0xA877:
            func_A877_b5(); break;
        case 0xA88F:
            func_A88F_b5(); break;
        case 0xA897:
            func_A897_b5(); break;
        case 0xA8AB:
            func_A8AB_b5(); break;
        case 0xAB3C:
            func_AB3C_b2(); break;
        case 0xAE13:
            func_AE13_b2(); break;
        case 0xABB8:
            func_ABB8_b2(); break;
        case 0xAB7E:
            func_AB7E_b2(); break;
        case 0xAF5A:
            func_AF5A_b2(); break;
        case 0xAB0A:
            func_AB0A_b5(); break;
        case 0xDBD3:
            func_DBD3(); break;
        case 0xDB9C:
            func_DB9C(); break;
        case 0xA3C2:
            func_A3C2_b1(); break;
        case 0xA420:
            func_A420_b1(); break;
        case 0xA3F3:
            func_A3F3_b2(); break;
        case 0xA42B:
            func_A42B_b2(); break;
        case 0xA419:
            func_A419_b4(); break;
        case 0xF3D4:
            func_F3D4(); break;
        case 0xB51C:
            func_B51C_b5(); break;
        case 0x988A:
            func_988A_b1(); break;
        case 0x9892:
            func_9892_b1(); break;
        case 0xE7C6:
            func_E7C6(); break;
        case 0xA1B1:
            func_A1B1_b1(); break;
        case 0xABB2:
            func_ABB2_b4(); break;
        case 0x8A11:
            func_8A11_b4(); break;
        case 0x8A03:
            func_8A03_b4(); break;
        case 0x8A4D:
            func_8A4D_b4(); break;
        case 0x89CB:
            func_89CB_b4(); break;
        case 0x8D8C:
            func_8D8C_b5(); break;
        case 0x8E00:
            switch (g_current_bank) {
                case 5: func_8E00_b5(); break;
                case 4: func_8E00_b4(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x8E1C:
            switch (g_current_bank) {
                case 5: func_8E1C_b5(); break;
                case 2: func_8E1C_b2(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x913F:
            func_913F_b5(); break;
        case 0xB38D:
            func_B38D_b5(); break;
        case 0xB3AB:
            func_B3AB_b5(); break;
        case 0xB3A9:
            func_B3A9_b5(); break;
        case 0xEEEF:
            func_EEEF(); break;
        case 0xA0F5:
            func_A0F5_b1(); break;
        case 0xA0BC:
            func_A0BC_b1(); break;
        case 0xA002:
            switch (g_current_bank) {
                case 1: func_A002_b1(); break;
                case 4: func_A002_b4(); break;
                case 6: func_A002_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xA094:
            func_A094_b4(); break;
        case 0xA11D:
            func_A11D_b4(); break;
        case 0x8E92:
            func_8E92_b0(); break;
        case 0x8E96:
            switch (g_current_bank) {
                case 0: func_8E96_b0(); break;
                case 5: func_8E96_b5(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x8E9A:
            func_8E9A_b0(); break;
        case 0x8E91:
            func_8E91_b0(); break;
        case 0x8EC6:
            func_8EC6_b5(); break;
        case 0x920F:
            func_920F_b5(); break;
        case 0x9220:
            func_9220_b5(); break;
        case 0xF148:
            func_F148(); break;
        case 0xF122:
            func_F122(); break;
        case 0xF116:
            func_F116(); break;
        case 0xA271:
            func_A271_b1(); break;
        case 0xA20A:
            func_A20A_b2(); break;
        case 0xA27C:
            func_A27C_b4(); break;
        case 0xA279:
            func_A279_b4(); break;
        case 0xB3D7:
            func_B3D7_b1(); break;
        case 0xB43A:
            func_B43A_b4(); break;
        case 0x8F61:
            func_8F61_b5(); break;
        case 0xB44C:
            func_B44C_b5(); break;
        case 0xB608:
            func_B608_b5(); break;
        case 0xB5B8:
            func_B5B8_b5(); break;
        case 0xB7CC:
            func_B7CC_b5(); break;
        case 0xF429:
            func_F429(); break;
        case 0xAB73:
            func_AB73_b5(); break;
        case 0xA434:
            func_A434_b5(); break;
        case 0xA43C:
            func_A43C_b5(); break;
        case 0xA765:
            func_A765_b5(); break;
        case 0xA7C1:
            func_A7C1_b5(); break;
        case 0xA7A8:
            func_A7A8_b5(); break;
        case 0xA74A:
            func_A74A_b5(); break;
        case 0x9050:
            func_9050_b5(); break;
        case 0xB1D9:
            func_B1D9_b4(); break;
        case 0xA2A7:
            func_A2A7_b2(); break;
        case 0xA351:
            func_A351_b2(); break;
        case 0x91DB:
            switch (g_current_bank) {
                case 0: func_91DB_b0(); break;
                case 1: func_91DB_b1(); break;
                case 2: func_91DB_b2(); break;
                case 3: func_91DB_b3(); break;
                case 4: func_91DB_b4(); break;
                case 5: func_91DB_b5(); break;
                case 6: func_91DB_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xFF00:
            func_FF00(); break;
        case 0xDD9C:
            func_DD9C(); break;
        case 0xDD94:
            func_DD94(); break;
        case 0x80B5:
            func_80B5_b6(); break;
        case 0xA82D:
            func_A82D_b5(); break;
        case 0xA1F7:
            func_A1F7_b2(); break;
        case 0xA10A:
            func_A10A_b2(); break;
        case 0x931F:
            func_931F_b5(); break;
        case 0xB243:
            func_B243_b4(); break;
        case 0xB24D:
            func_B24D_b4(); break;
        case 0x9F2E:
            func_9F2E_b4(); break;
        case 0xF854:
            func_F854(); break;
        case 0xF82A:
            func_F82A(); break;
        case 0xA69A:
            func_A69A_b4(); break;
        case 0xA696:
            func_A696_b4(); break;
        case 0xA6FA:
            func_A6FA_b4(); break;
        case 0xC667:
            func_C667(); break;
        case 0xFFFC:
            func_FFFC(); break;
        case 0xFC00:
            func_FC00(); break;
        case 0xFF02:
            func_FF02(); break;
        case 0xC456:
            func_C456(); break;
        case 0xB1EA:
            func_B1EA_b4(); break;
        case 0x8832:
            func_8832_b4(); break;
        case 0x87D1:
            func_87D1_b4(); break;
        case 0x8112:
            func_8112_b2(); break;
        case 0x80B0:
            func_80B0_b3(); break;
        case 0x8BCA:
            func_8BCA_b1(); break;
        case 0x8BF1:
            func_8BF1_b1(); break;
        case 0x8BFD:
            func_8BFD_b5(); break;
        case 0x9B3D:
            func_9B3D_b0(); break;
        case 0xB9AA:
            func_B9AA_b1(); break;
        case 0xA08A:
            func_A08A_b1(); break;
        case 0xA2FE:
            func_A2FE_b1(); break;
        case 0xD020:
            func_D020(); break;
        case 0xA3EB:
            func_A3EB_b1(); break;
        case 0xEACB:
            func_EACB(); break;
        case 0x8A21:
            func_8A21_b4(); break;
        case 0x8C65:
            func_8C65_b4(); break;
        case 0x8A6D:
            func_8A6D_b4(); break;
        case 0x9D22:
            func_9D22_b1(); break;
        case 0x9D82:
            func_9D82_b4(); break;
        case 0x9DBD:
            func_9DBD_b4(); break;
        case 0x9DD1:
            func_9DD1_b4(); break;
        case 0xB195:
            func_B195_b4(); break;
        case 0x84E1:
            func_84E1_b4(); break;
        case 0x852C:
            func_852C_b4(); break;
        case 0x8563:
            func_8563_b2(); break;
        case 0x858F:
            func_858F_b4(); break;
        case 0x8629:
            switch (g_current_bank) {
                case 4: func_8629_b4(); break;
                case 1: func_8629_b1(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xA060:
            func_A060_b5(); break;
        case 0x9CBD:
            func_9CBD_b4(); break;
        case 0x9C3A:
            func_9C3A_b4(); break;
        case 0x9BCF:
            func_9BCF_b4(); break;
        case 0x9C8F:
            func_9C8F_b4(); break;
        case 0x9C26:
            func_9C26_b4(); break;
        case 0x8297:
            func_8297_b4(); break;
        case 0x9587:
            func_9587_b4(); break;
        case 0x95EA:
            func_95EA_b4(); break;
        case 0x84A1:
            func_84A1_b4(); break;
        case 0x8D4D:
            func_8D4D_b1(); break;
        case 0x88AF:
            func_88AF_b4(); break;
        case 0x8050:
            func_8050_b1(); break;
        case 0x9DF7:
            func_9DF7_b4(); break;
        case 0x9F58:
            func_9F58_b4(); break;
        case 0xA040:
            func_A040_b4(); break;
        case 0x9E20:
            func_9E20_b4(); break;
        case 0x9EFC:
            func_9EFC_b4(); break;
        case 0x9E62:
            func_9E62_b4(); break;
        case 0x9E58:
            func_9E58_b4(); break;
        case 0x9E9D:
            func_9E9D_b4(); break;
        case 0x9F2C:
            func_9F2C_b4(); break;
        case 0x9EEB:
            func_9EEB_b4(); break;
        case 0x9E7E:
            func_9E7E_b4(); break;
        case 0x9F02:
            switch (g_current_bank) {
                case 4: func_9F02_b4(); break;
                case 0: func_9F02_b0(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xA034:
            switch (g_current_bank) {
                case 4: func_A034_b4(); break;
                case 2: func_A034_b2(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x9FED:
            switch (g_current_bank) {
                case 4: func_9FED_b4(); break;
                case 6: func_9FED_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xA035:
            func_A035_b4(); break;
        case 0x9FFD:
            func_9FFD_b4(); break;
        case 0xAC59:
            func_AC59_b4(); break;
        case 0xAC71:
            func_AC71_b4(); break;
        case 0xA9E5:
            func_A9E5_b4(); break;
        case 0x82F2:
            func_82F2_b1(); break;
        case 0x9313:
            func_9313_b1(); break;
        case 0xCB66:
            func_CB66(); break;
        case 0xFFFF:
            func_FFFF(); break;
        case 0x8454:
            func_8454_b4(); break;
        case 0x95A1:
            func_95A1_b0(); break;
        case 0x95F1:
            func_95F1_b0(); break;
        case 0x8D89:
            func_8D89_b1(); break;
        case 0x8D17:
            func_8D17_b5(); break;
        case 0x9ADE:
            func_9ADE_b5(); break;
        case 0xA380:
            func_A380_b4(); break;
        case 0xB2E2:
            func_B2E2_b4(); break;
        case 0xA3E3:
            func_A3E3_b4(); break;
        case 0xA422:
            func_A422_b4(); break;
        case 0xA3AD:
            func_A3AD_b4(); break;
        case 0xA440:
            func_A440_b1(); break;
        case 0xA90E:
            func_A90E_b2(); break;
        case 0xF229:
            func_F229(); break;
        case 0xA958:
            func_A958_b5(); break;
        case 0xA985:
            func_A985_b5(); break;
        case 0xA95E:
            switch (g_current_bank) {
                case 5: func_A95E_b5(); break;
                case 1: func_A95E_b1(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xA966:
            switch (g_current_bank) {
                case 5: func_A966_b5(); break;
                case 2: func_A966_b2(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xA96D:
            func_A96D_b5(); break;
        case 0xA945:
            func_A945_b6(); break;
        case 0xAB24:
            func_AB24_b4(); break;
        case 0xAA70:
            func_AA70_b4(); break;
        case 0xA285:
            func_A285_b4(); break;
        case 0xA28E:
            func_A28E_b4(); break;
        case 0xA225:
            func_A225_b4(); break;
        case 0xA2EF:
            func_A2EF_b4(); break;
        case 0xA1C4:
            func_A1C4_b4(); break;
        case 0x9711:
            func_9711_b0(); break;
        case 0xAC89:
            func_AC89_b4(); break;
        case 0xACC6:
            func_ACC6_b4(); break;
        case 0xACE4:
            func_ACE4_b4(); break;
        case 0xAEB5:
            func_AEB5_b4(); break;
        case 0xA9A4:
            func_A9A4_b6(); break;
        case 0xA9A8:
            func_A9A8_b6(); break;
        case 0xE078:
            func_E078(); break;
        case 0xFE9E:
            func_FE9E(); break;
        case 0xDC60:
            func_DC60(); break;
        case 0xA4D1:
            func_A4D1_b2(); break;
        case 0xA825:
            func_A825_b4(); break;
        case 0xA547:
            func_A547_b4(); break;
        case 0xA54D:
            func_A54D_b4(); break;
        case 0xA576:
            switch (g_current_bank) {
                case 4: func_A576_b4(); break;
                case 5: func_A576_b5(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xA7A4:
            func_A7A4_b4(); break;
        case 0xA6E5:
            func_A6E5_b4(); break;
        case 0xA472:
            func_A472_b3(); break;
        case 0xA4AC:
            func_A4AC_b4(); break;
        case 0xA460:
            func_A460_b5(); break;
        case 0xABAE:
            func_ABAE_b4(); break;
        case 0xAB98:
            func_AB98_b4(); break;
        case 0x8371:
            func_8371_b4(); break;
        case 0xF57B:
            func_F57B(); break;
        case 0xF391:
            func_F391(); break;
        case 0x8343:
            func_8343_b2(); break;
        case 0x837F:
            func_837F_b2(); break;
        case 0x8407:
            func_8407_b4(); break;
        case 0x8305:
            func_8305_b4(); break;
        case 0x882B:
            func_882B_b1(); break;
        case 0x8869:
            func_8869_b4(); break;
        case 0x883E:
            func_883E_b4(); break;
        case 0x834C:
            func_834C_b4(); break;
        case 0x8F5D:
            func_8F5D_b5(); break;
        case 0x8E1B:
            func_8E1B_b0(); break;
        case 0x8522:
            switch (g_current_bank) {
                case 0: func_8522_b0(); break;
                case 1: func_8522_b1(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x8EDC:
            func_8EDC_b0(); break;
        case 0x81EE:
            func_81EE_b2(); break;
        case 0x81AE:
            func_81AE_b3(); break;
        case 0x8204:
            func_8204_b4(); break;
        case 0x8263:
            func_8263_b4(); break;
        case 0x8289:
            func_8289_b4(); break;
        case 0x822C:
            func_822C_b4(); break;
        case 0x8729:
            func_8729_b3(); break;
        case 0xC907:
            func_C907(); break;
        case 0xFE04:
            func_FE04(); break;
        case 0x92BA:
            switch (g_current_bank) {
                case 0: func_92BA_b0(); break;
                case 1: func_92BA_b1(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x925F:
            func_925F_b0(); break;
        case 0x9239:
            func_9239_b5(); break;
        case 0x9235:
            func_9235_b5(); break;
        case 0x923A:
            func_923A_b5(); break;
        case 0x924A:
            func_924A_b5(); break;
        case 0x926B:
            func_926B_b5(); break;
        case 0x9251:
            func_9251_b5(); break;
        case 0xFFA0:
            func_FFA0(); break;
        case 0x8ADA:
            func_8ADA_b5(); break;
        case 0x9451:
            func_9451_b0(); break;
        case 0xA140:
            func_A140_b1(); break;
        case 0xEEDC:
            func_EEDC(); break;
        case 0xD004:
            func_D004(); break;
        case 0x9439:
            func_9439_b1(); break;
        case 0x93F4:
            func_93F4_b5(); break;
        case 0x9375:
            func_9375_b0(); break;
        case 0x937B:
            func_937B_b0(); break;
        case 0x9F10:
            func_9F10_b4(); break;
        case 0x9FDF:
            func_9FDF_b6(); break;
        case 0xA2DE:
            func_A2DE_b3(); break;
        case 0xDF20:
            func_DF20(); break;
        case 0x8030:
            func_8030_b3(); break;
        case 0xF9F9:
            func_F9F9(); break;
        case 0xA3E9:
            switch (g_current_bank) {
                case 2: func_A3E9_b2(); break;
                case 6: func_A3E9_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xA3FA:
            switch (g_current_bank) {
                case 4: func_A3FA_b4(); break;
                case 6: func_A3FA_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xA2DD:
            func_A2DD_b5(); break;
        case 0xA2FD:
            func_A2FD_b5(); break;
        case 0xA48E:
            func_A48E_b5(); break;
        case 0xA7DC:
            func_A7DC_b5(); break;
        case 0xB547:
            func_B547_b5(); break;
        case 0xA2E5:
            func_A2E5_b3(); break;
        case 0xA4BA:
            func_A4BA_b4(); break;
        case 0x84C1:
            func_84C1_b4(); break;
        case 0x812D:
            func_812D_b4(); break;
        case 0xA585:
            func_A585_b4(); break;
        case 0xA784:
            func_A784_b4(); break;
        case 0xA6E7:
            func_A6E7_b4(); break;
        case 0xA7FF:
            func_A7FF_b4(); break;
        case 0xA844:
            func_A844_b4(); break;
        case 0xAAD8:
            func_AAD8_b4(); break;
        case 0xABBC:
            func_ABBC_b4(); break;
        case 0xAEE4:
            func_AEE4_b4(); break;
        case 0x9E1D:
            func_9E1D_b4(); break;
        case 0xACFB:
            func_ACFB_b4(); break;
        case 0xAF71:
            func_AF71_b4(); break;
        case 0xAF75:
            func_AF75_b4(); break;
        case 0xADB1:
            switch (g_current_bank) {
                case 4: func_ADB1_b4(); break;
                case 1: func_ADB1_b1(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xAF9F:
            func_AF9F_b4(); break;
        case 0xAFA7:
            func_AFA7_b4(); break;
        case 0xAE6D:
            func_AE6D_b4(); break;
        case 0xAE7F:
            func_AE7F_b4(); break;
        case 0xADCC:
            func_ADCC_b4(); break;
        case 0xAE76:
            func_AE76_b4(); break;
        case 0xAE88:
            func_AE88_b4(); break;
        case 0xAF6D:
            func_AF6D_b4(); break;
        case 0xB269:
            func_B269_b4(); break;
        case 0xB27A:
            func_B27A_b4(); break;
        case 0xB39F:
            func_B39F_b4(); break;
        case 0x8BB0:
            func_8BB0_b3(); break;
        case 0xB3FA:
            func_B3FA_b4(); break;
        case 0xB307:
            func_B307_b4(); break;
        case 0xB311:
            func_B311_b4(); break;
        case 0xB36A:
            func_B36A_b4(); break;
        case 0x951F:
            func_951F_b4(); break;
        case 0x800C:
            func_800C_b4(); break;
        case 0x9989:
            func_9989_b4(); break;
        case 0xA8AA:
            func_A8AA_b5(); break;
        case 0xB528:
            func_B528_b5(); break;
        case 0x9811:
            func_9811_b1(); break;
        case 0x8DFF:
            func_8DFF_b5(); break;
        case 0xB38C:
            func_B38C_b5(); break;
        case 0xF110:
            func_F110(); break;
        case 0xA77A:
            func_A77A_b5(); break;
        case 0x91D6:
            func_91D6_b0(); break;
        case 0x91A1:
            func_91A1_b3(); break;
        case 0xB328:
            func_B328_b3(); break;
        case 0x9F4B:
            func_9F4B_b4(); break;
        case 0xC448:
            func_C448(); break;
        case 0x9D9B:
            func_9D9B_b4(); break;
        case 0x84F1:
            func_84F1_b4(); break;
        case 0xFD00:
            func_FD00(); break;
        case 0xFFFE:
            func_FFFE(); break;
        case 0x84EC:
            func_84EC_b4(); break;
        case 0x84F9:
            func_84F9_b4(); break;
        case 0x85C8:
            func_85C8_b4(); break;
        case 0x85FE:
            func_85FE_b4(); break;
        case 0x9CAE:
            switch (g_current_bank) {
                case 4: func_9CAE_b4(); break;
                case 5: func_9CAE_b5(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x84B3:
            func_84B3_b4(); break;
        case 0xB180:
            func_B180_b4(); break;
        case 0x9E17:
            func_9E17_b4(); break;
        case 0x9DEE:
            func_9DEE_b4(); break;
        case 0x9F9A:
            func_9F9A_b4(); break;
        case 0x9AD9:
            func_9AD9_b5(); break;
        case 0xA95B:
            func_A95B_b5(); break;
        case 0xAA6F:
            func_AA6F_b4(); break;
        case 0x96AF:
            func_96AF_b0(); break;
        case 0x96B7:
            func_96B7_b0(); break;
        case 0x96B3:
            func_96B3_b0(); break;
        case 0x96B9:
            func_96B9_b0(); break;
        case 0x96CA:
            func_96CA_b0(); break;
        case 0x96C6:
            func_96C6_b0(); break;
        case 0xACDF:
            func_ACDF_b4(); break;
        case 0xA81F:
            func_A81F_b4(); break;
        case 0xA4EA:
            func_A4EA_b4(); break;
        case 0xA571:
            switch (g_current_bank) {
                case 4: func_A571_b4(); break;
                case 5: func_A571_b5(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xA6CB:
            func_A6CB_b4(); break;
        case 0x833B:
            func_833B_b4(); break;
        case 0x81A9:
            func_81A9_b3(); break;
        case 0x8172:
            func_8172_b3(); break;
        case 0x8253:
            func_8253_b4(); break;
        case 0x92B4:
            func_92B4_b0(); break;
        case 0x92B9:
            func_92B9_b0(); break;
        case 0x91F7:
            func_91F7_b0(); break;
        case 0xEECA:
            func_EECA(); break;
        case 0xF9E7:
            func_F9E7(); break;
        case 0x9193:
            func_9193_b3(); break;
        case 0xFCFF:
            func_FCFF(); break;
        case 0xEE10:
            func_EE10(); break;
        case 0x9F7E:
            func_9F7E_b4(); break;
        case 0x9641:
            func_9641_b0(); break;
        case 0x92AF:
            func_92AF_b0(); break;
        case 0x93B8:
            func_93B8_b1(); break;
        case 0x9614:
            func_9614_b0(); break;
        case 0x92A6:
            func_92A6_b0(); break;
        case 0x92A0:
            func_92A0_b0(); break;
        case 0xF0F0:
            func_F0F0(); break;
        case 0xF0F1:
            func_F0F1(); break;
        case 0xF03D:
            func_F03D(); break;
        case 0xFCF0:
            func_FCF0(); break;
        case 0xFCF1:
            func_FCF1(); break;
        case 0xC0F0:
            func_C0F0(); break;
        case 0xC0F1:
            func_C0F1(); break;
        case 0xFC0F:
            func_FC0F(); break;
        case 0xFC10:
            func_FC10(); break;
        case 0xF30F:
            func_F30F(); break;
        case 0xF310:
            func_F310(); break;
        case 0xC33F:
            func_C33F(); break;
        case 0xCC30:
            func_CC30(); break;
        case 0xCFCC:
            func_CFCC(); break;
        case 0xFF0C:
            func_FF0C(); break;
        case 0xCF03:
            func_CF03(); break;
        case 0xCF00:
            func_CF00(); break;
        case 0xF00F:
            func_F00F(); break;
        case 0xF010:
            func_F010(); break;
        case 0xE3C0:
            func_E3C0(); break;
        case 0xE3C1:
            func_E3C1(); break;
        case 0xF3E3:
            func_F3E3(); break;
        case 0xF3E4:
            func_F3E4(); break;
        case 0xCC0F:
            func_CC0F(); break;
        case 0xC0CF:
            func_C0CF(); break;
        case 0xF0C4:
            func_F0C4(); break;
        case 0xCF0D:
            func_CF0D(); break;
        case 0xC3CF:
            func_C3CF(); break;
        case 0xCB0C:
            func_CB0C(); break;
        case 0xC3CC:
            func_C3CC(); break;
        case 0xCB0F:
            func_CB0F(); break;
        case 0xCB10:
            func_CB10(); break;
        case 0xFC3C:
            func_FC3C(); break;
        case 0xFC3D:
            func_FC3D(); break;
        case 0xF0FC:
            func_F0FC(); break;
        case 0xF0FD:
            func_F0FD(); break;
        case 0xCCFD:
            func_CCFD(); break;
        case 0xFC3F:
            func_FC3F(); break;
        case 0xFC03:
            func_FC03(); break;
        case 0xFC04:
            func_FC04(); break;
        case 0xC040:
            func_C040(); break;
        case 0xCF0F:
            func_CF0F(); break;
        case 0xF03F:
            func_F03F(); break;
        case 0xFF3C:
            func_FF3C(); break;
        case 0xFF3D:
            func_FF3D(); break;
        case 0xE03C:
            func_E03C(); break;
        case 0xC300:
            func_C300(); break;
        case 0xC301:
            func_C301(); break;
        case 0xF303:
            func_F303(); break;
        case 0xF304:
            func_F304(); break;
        case 0xCC3C:
            func_CC3C(); break;
        case 0xCC3D:
            func_CC3D(); break;
        case 0xF80F:
            func_F80F(); break;
        case 0xF810:
            func_F810(); break;
        case 0xF3C4:
            func_F3C4(); break;
        case 0xCF3C:
            func_CF3C(); break;
        case 0xF2C3:
            func_F2C3(); break;
        case 0xF2C4:
            func_F2C4(); break;
        case 0xCC3F:
            func_CC3F(); break;
        case 0xFC33:
            func_FC33(); break;
        case 0xF3F3:
            func_F3F3(); break;
        case 0xF3F4:
            func_F3F4(); break;
        case 0xF00C:
            func_F00C(); break;
        case 0xF00D:
            func_F00D(); break;
        case 0xF3C0:
            func_F3C0(); break;
        case 0xF3C1:
            func_F3C1(); break;
        case 0xC034:
            func_C034(); break;
        case 0xC3F0:
            func_C3F0(); break;
        case 0xFCC4:
            func_FCC4(); break;
        case 0xF2F1:
            func_F2F1(); break;
        case 0xF0F2:
            func_F0F2(); break;
        case 0xF0F3:
            func_F0F3(); break;
        case 0xFC30:
            func_FC30(); break;
        case 0xFC31:
            func_FC31(); break;
        case 0xC2F0:
            func_C2F0(); break;
        case 0xC3C2:
            func_C3C2(); break;
        case 0xE0FC:
            func_E0FC(); break;
        case 0xE0FD:
            func_E0FD(); break;
        case 0xCFE0:
            func_CFE0(); break;
        case 0xCFE1:
            func_CFE1(); break;
        case 0xFCCF:
            func_FCCF(); break;
        case 0xFCD0:
            func_FCD0(); break;
        case 0xF034:
            func_F034(); break;
        case 0xCFF3:
            func_CFF3(); break;
        case 0xFF30:
            func_FF30(); break;
        case 0xFF31:
            func_FF31(); break;
        case 0xFC0C:
            func_FC0C(); break;
        case 0xFC0D:
            func_FC0D(); break;
        case 0xF300:
            func_F300(); break;
        case 0xE30B:
            func_E30B(); break;
        case 0xE30C:
            func_E30C(); break;
        case 0xF3CF:
            func_F3CF(); break;
        case 0xFF01:
            func_FF01(); break;
        case 0xFCC0:
            func_FCC0(); break;
        case 0xFCC1:
            func_FCC1(); break;
        case 0xF3FD:
            func_F3FD(); break;
        case 0xFE0F:
            func_FE0F(); break;
        case 0xFE10:
            func_FE10(); break;
        case 0xF0FE:
            func_F0FE(); break;
        case 0xF0FF:
            func_F0FF(); break;
        case 0xF0BC:
            func_F0BC(); break;
        case 0xF0BD:
            func_F0BD(); break;
        case 0xF2CB:
            func_F2CB(); break;
        case 0xF2CC:
            func_F2CC(); break;
        case 0xF302:
            func_F302(); break;
        case 0xFF03:
            func_FF03(); break;
        case 0xF07C:
            func_F07C(); break;
        case 0xF07D:
            func_F07D(); break;
        case 0xE2C3:
            func_E2C3(); break;
        case 0xC3E3:
            func_C3E3(); break;
        case 0xFFF1:
            func_FFF1(); break;
        case 0xFC02:
            func_FC02(); break;
        case 0xC100:
            func_C100(); break;
        case 0xFCBC:
            func_FCBC(); break;
        case 0xFCBD:
            func_FCBD(); break;
        case 0xE3C3:
            func_E3C3(); break;
        case 0xE3C4:
            func_E3C4(); break;
        case 0xF0E4:
            func_F0E4(); break;
        case 0xF803:
            func_F803(); break;
        case 0xF804:
            func_F804(); break;
        case 0xFF80:
            func_FF80(); break;
        case 0xFF81:
            func_FF81(); break;
        case 0xCBCB:
            func_CBCB(); break;
        case 0xCBCC:
            func_CBCC(); break;
        case 0xF0CB:
            func_F0CB(); break;
        case 0xF0CC:
            func_F0CC(); break;
        case 0xCB3C:
            func_CB3C(); break;
        case 0xF3CB:
            func_F3CB(); break;
        case 0xF3CC:
            func_F3CC(); break;
        case 0xCE2D:
            func_CE2D(); break;
        case 0xFFF8:
            func_FFF8(); break;
        case 0xFFF9:
            func_FFF9(); break;
        case 0xF830:
            func_F830(); break;
        case 0xC3E0:
            func_C3E0(); break;
        case 0xCC39:
            func_CC39(); break;
        case 0xF2CF:
            func_F2CF(); break;
        case 0xF2D0:
            func_F2D0(); break;
        case 0xF2F2:
            func_F2F2(); break;
        case 0xC003:
            func_C003(); break;
        case 0xCCB0:
            func_CCB0(); break;
        case 0xFC0B:
            func_FC0B(); break;
        case 0xC00B:
            func_C00B(); break;
        case 0xC3FF:
            func_C3FF(); break;
        case 0xE33C:
            func_E33C(); break;
        case 0xFC3E:
            func_FC3E(); break;
        case 0xC0C3:
            func_C0C3(); break;
        case 0xC0C4:
            func_C0C4(); break;
        case 0xCCF0:
            func_CCF0(); break;
        case 0xDE00:
            func_DE00(); break;
        case 0xC000:
            func_C000(); break;
        case 0xC001:
            func_C001(); break;
        case 0xC400:
            func_C400(); break;
        case 0xF001:
            func_F001(); break;
        case 0xF0BF:
            func_F0BF(); break;
        case 0xF0C0:
            func_F0C0(); break;
        case 0xF440:
            func_F440(); break;
        case 0xF40F:
            func_F40F(); break;
        case 0xF410:
            func_F410(); break;
        case 0xD010:
            func_D010(); break;
        case 0xF800:
            func_F800(); break;
        case 0xF801:
            func_F801(); break;
        case 0xF002:
            func_F002(); break;
        case 0xFF40:
            func_FF40(); break;
        case 0xFF41:
            func_FF41(); break;
        case 0xF00B:
            func_F00B(); break;
        case 0xFF60:
            func_FF60(); break;
        case 0xFF61:
            func_FF61(); break;
        case 0xFD01:
            func_FD01(); break;
        case 0xF703:
            func_F703(); break;
        case 0xF704:
            func_F704(); break;
        case 0xD810:
            func_D810(); break;
        case 0xD030:
            func_D030(); break;
        case 0xC002:
            func_C002(); break;
        case 0xFF82:
            func_FF82(); break;
        case 0xFF83:
            func_FF83(); break;
        case 0xFF20:
            func_FF20(); break;
        case 0xFF21:
            func_FF21(); break;
        case 0xDC04:
            func_DC04(); break;
        case 0xFF08:
            func_FF08(); break;
        case 0xEC0B:
            func_EC0B(); break;
        case 0xEC0C:
            func_EC0C(); break;
        case 0xFC20:
            func_FC20(); break;
        case 0xFC21:
            func_FC21(); break;
        case 0xFA00:
            func_FA00(); break;
        case 0xFA01:
            func_FA01(); break;
        case 0xF080:
            func_F080(); break;
        case 0xF081:
            func_F081(); break;
        case 0xC203:
            func_C203(); break;
        case 0xFE81:
            func_FE81(); break;
        case 0xF700:
            func_F700(); break;
        case 0xF701:
            func_F701(); break;
        case 0xFF88:
            func_FF88(); break;
        case 0xFF89:
            func_FF89(); break;
        case 0xFC11:
            func_FC11(); break;
        case 0xF220:
            func_F220(); break;
        case 0xF221:
            func_F221(); break;
        case 0xD890:
            func_D890(); break;
        case 0xD121:
            func_D121(); break;
        case 0xFD60:
            func_FD60(); break;
        case 0xFD61:
            func_FD61(); break;
        case 0xC903:
            func_C903(); break;
        case 0xDD80:
            func_DD80(); break;
        case 0xC484:
            func_C484(); break;
        case 0xF501:
            func_F501(); break;
        case 0xFF22:
            func_FF22(); break;
        case 0xFF23:
            func_FF23(); break;
        case 0xF580:
            func_F580(); break;
        case 0xF581:
            func_F581(); break;
        case 0xFC8B:
            func_FC8B(); break;
        case 0xD604:
            func_D604(); break;
        case 0xD80B:
            func_D80B(); break;
        case 0xE920:
            func_E920(); break;
        case 0xFD41:
            func_FD41(); break;
        case 0xD551:
            func_D551(); break;
        case 0xFD2B:
            func_FD2B(); break;
        case 0xFD29:
            func_FD29(); break;
        case 0xFCA4:
            func_FCA4(); break;
        case 0xFCA5:
            func_FCA5(); break;
        case 0xF4A4:
            func_F4A4(); break;
        case 0xF4A5:
            func_F4A5(); break;
        case 0xF4A8:
            func_F4A8(); break;
        case 0xF4A9:
            func_F4A9(); break;
        case 0xF2A2:
            func_F2A2(); break;
        case 0xF2A3:
            func_F2A3(); break;
        case 0xCAA2:
            func_CAA2(); break;
        case 0xCAA3:
            func_CAA3(); break;
        case 0xDAD1:
            func_DAD1(); break;
        case 0xD03F:
            func_D03F(); break;
        case 0xD040:
            func_D040(); break;
        case 0xFF2A:
            func_FF2A(); break;
        case 0xFF2B:
            func_FF2B(); break;
        case 0xF33C:
            func_F33C(); break;
        case 0xF33D:
            func_F33D(); break;
        case 0xF3FF:
            func_F3FF(); break;
        case 0xF400:
            func_F400(); break;
        case 0xFFF3:
            func_FFF3(); break;
        case 0xFFF4:
            func_FFF4(); break;
        case 0xFF0F:
            func_FF0F(); break;
        case 0xFF10:
            func_FF10(); break;
        case 0xD75D:
            func_D75D(); break;
        case 0xDB5D:
            func_DB5D(); break;
        case 0xDB5E:
            func_DB5E(); break;
        case 0xD6DB:
            func_D6DB(); break;
        case 0xF02C:
            func_F02C(); break;
        case 0xF8FC:
            func_F8FC(); break;
        case 0xF8FD:
            func_F8FD(); break;
        case 0xF5C1:
            func_F5C1(); break;
        case 0xFFF6:
            func_FFF6(); break;
        case 0xC7FC:
            func_C7FC(); break;
        case 0xE381:
            func_E381(); break;
        case 0xC200:
            func_C200(); break;
        case 0xFC71:
            func_FC71(); break;
        case 0xE870:
            func_E870(); break;
        case 0xE871:
            func_E871(); break;
        case 0xC701:
            func_C701(); break;
        case 0xE17F:
            func_E17F(); break;
        case 0xFE39:
            func_FE39(); break;
        case 0xC038:
            func_C038(); break;
        case 0xF1FF:
            func_F1FF(); break;
        case 0xF200:
            func_F200(); break;
        case 0xE3F1:
            func_E3F1(); break;
        case 0xE3F2:
            func_E3F2(); break;
        case 0xF8E4:
            func_F8E4(); break;
        case 0xC01E:
            func_C01E(); break;
        case 0xE821:
            func_E821(); break;
        case 0xE822:
            func_E822(); break;
        case 0xC720:
            func_C720(); break;
        case 0xC7FF:
            func_C7FF(); break;
        case 0xE87C:
            func_E87C(); break;
        case 0xF8E8:
            func_F8E8(); break;
        case 0xF8E9:
            func_F8E9(); break;
        case 0xF1C1:
            func_F1C1(); break;
        case 0xFFF2:
            func_FFF2(); break;
        case 0xFC7F:
            func_FC7F(); break;
        case 0xF007:
            func_F007(); break;
        case 0xF008:
            func_F008(); break;
        case 0xE380:
            func_E380(); break;
        case 0xE0E3:
            func_E0E3(); break;
        case 0xE0E4:
            func_E0E4(); break;
        case 0xE3F0:
            func_E3F0(); break;
        case 0xC7E0:
            func_C7E0(); break;
        case 0xF1C7:
            func_F1C7(); break;
        case 0xF1C8:
            func_F1C8(); break;
        case 0xFE07:
            func_FE07(); break;
        case 0xFE08:
            func_FE08(); break;
        case 0xFF8F:
            func_FF8F(); break;
        case 0xFF90:
            func_FF90(); break;
        case 0xE07E:
            func_E07E(); break;
        case 0xE07F:
            func_E07F(); break;
        case 0xF80B:
            func_F80B(); break;
        case 0xF80C:
            func_F80C(); break;
        case 0xF820:
            func_F820(); break;
        case 0xEC0F:
            func_EC0F(); break;
        case 0xEC10:
            func_EC10(); break;
        case 0xF81C:
            func_F81C(); break;
        case 0xF81D:
            func_F81D(); break;
        case 0xE030:
            func_E030(); break;
        case 0xFA1C:
            func_FA1C(); break;
        case 0xFA1D:
            func_FA1D(); break;
        case 0xE3FF:
            func_E3FF(); break;
        case 0xE400:
            func_E400(); break;
        case 0xC703:
            func_C703(); break;
        case 0xC704:
            func_C704(); break;
        case 0xF8FF:
            func_F8FF(); break;
        case 0xF900:
            func_F900(); break;
        case 0xFC0E:
            func_FC0E(); break;
        case 0xFC60:
            func_FC60(); break;
        case 0xC71D:
            func_C71D(); break;
        case 0xF7C7:
            func_F7C7(); break;
        case 0xF7C8:
            func_F7C8(); break;
        case 0xFF1F:
            func_FF1F(); break;
        case 0xE101:
            func_E101(); break;
        case 0xF1F8:
            func_F1F8(); break;
        case 0xF1F9:
            func_F1F9(); break;
        case 0xF8E0:
            func_F8E0(); break;
        case 0xF8E1:
            func_F8E1(); break;
        case 0xFF0B:
            func_FF0B(); break;
        case 0xF0C7:
            func_F0C7(); break;
        case 0xF0C8:
            func_F0C8(); break;
        case 0xFC72:
            func_FC72(); break;
        case 0xEF00:
            func_EF00(); break;
        case 0xEF01:
            func_EF01(); break;
        case 0xF818:
            func_F818(); break;
        case 0xFEF1:
            func_FEF1(); break;
        case 0xC0FE:
            func_C0FE(); break;
        case 0xFA38:
            func_FA38(); break;
        case 0xFA39:
            func_FA39(); break;
        case 0xFF8E:
            func_FF8E(); break;
        case 0xE700:
            func_E700(); break;
        case 0xFA1F:
            func_FA1F(); break;
        case 0xFA20:
            func_FA20(); break;
        case 0xFCE1:
            func_FCE1(); break;
        case 0xE01F:
            func_E01F(); break;
        case 0xE020:
            func_E020(); break;
        case 0xE80E:
            func_E80E(); break;
        case 0xFEC0:
            func_FEC0(); break;
        case 0xFC41:
            func_FC41(); break;
        case 0xEF1D:
            func_EF1D(); break;
        case 0xFE3F:
            func_FE3F(); break;
        case 0xFF1C:
            func_FF1C(); break;
        case 0xFF1D:
            func_FF1D(); break;
        case 0xF3F0:
            func_F3F0(); break;
        case 0xF3F1:
            func_F3F1(); break;
        case 0xE2FF:
            func_E2FF(); break;
        case 0xE300:
            func_E300(); break;
        case 0xFC83:
            func_FC83(); break;
        case 0xFE21:
            func_FE21(); break;
        case 0xFE22:
            func_FE22(); break;
        case 0xC79C:
            func_C79C(); break;
        case 0xC79D:
            func_C79D(); break;
        case 0xF81E:
            func_F81E(); break;
        case 0xF038:
            func_F038(); break;
        case 0xF039:
            func_F039(); break;
        case 0xF0F8:
            func_F0F8(); break;
        case 0xF0F9:
            func_F0F9(); break;
        case 0xE3FE:
            func_E3FE(); break;
        case 0xEF8E:
            func_EF8E(); break;
        case 0xEF8F:
            func_EF8F(); break;
        case 0xEF81:
            func_EF81(); break;
        case 0xEF82:
            func_EF82(); break;
        case 0xC7EF:
            func_C7EF(); break;
        case 0xC7F0:
            func_C7F0(); break;
        case 0xFFA1:
            func_FFA1(); break;
        case 0xE201:
            func_E201(); break;
        case 0xFF9C:
            func_FF9C(); break;
        case 0xFF9D:
            func_FF9D(); break;
        case 0xFD81:
            func_FD81(); break;
        case 0xFD82:
            func_FD82(); break;
        case 0xEC71:
            func_EC71(); break;
        case 0xEC72:
            func_EC72(); break;
        case 0xFFEC:
            func_FFEC(); break;
        case 0xFFED:
            func_FFED(); break;
        case 0xFE28:
            func_FE28(); break;
        case 0xE800:
            func_E800(); break;
        case 0xE801:
            func_E801(); break;
        case 0xE080:
            func_E080(); break;
        case 0xFC75:
            func_FC75(); break;
        case 0xFC76:
            func_FC76(); break;
        case 0xF89C:
            func_F89C(); break;
        case 0xF89D:
            func_F89D(); break;
        case 0xE280:
            func_E280(); break;
        case 0xFF63:
            func_FF63(); break;
        case 0xFF64:
            func_FF64(); break;
        case 0xC0E0:
            func_C0E0(); break;
        case 0xE390:
            func_E390(); break;
        case 0xE1F8:
            func_E1F8(); break;
        case 0xD204:
            func_D204(); break;
        case 0xEC7F:
            func_EC7F(); break;
        case 0xEC80:
            func_EC80(); break;
        case 0xF8EC:
            func_F8EC(); break;
        case 0xF8ED:
            func_F8ED(); break;
        case 0xEFFF:
            func_EFFF(); break;
        case 0xF814:
            func_F814(); break;
        case 0xF815:
            func_F815(); break;
        case 0xFBC0:
            func_FBC0(); break;
        case 0xFBC1:
            func_FBC1(); break;
        case 0xFFFB:
            func_FFFB(); break;
        case 0xFB60:
            func_FB60(); break;
        case 0xEFF0:
            func_EFF0(); break;
        case 0xEFF1:
            func_EFF1(); break;
        case 0xC0EF:
            func_C0EF(); break;
        case 0xE0E0:
            func_E0E0(); break;
        case 0xFEB8:
            func_FEB8(); break;
        case 0xFEB9:
            func_FEB9(); break;
        case 0xFFBE:
            func_FFBE(); break;
        case 0xFFBF:
            func_FFBF(); break;
        case 0xC5F4:
            func_C5F4(); break;
        case 0xFF84:
            func_FF84(); break;
        case 0xFBE0:
            func_FBE0(); break;
        case 0xE802:
            func_E802(); break;
        case 0xC0E9:
            func_C0E9(); break;
        case 0xF02F:
            func_F02F(); break;
        case 0xF030:
            func_F030(); break;
        case 0xF9FA:
            func_F9FA(); break;
        case 0xF9FB:
            func_F9FB(); break;
        case 0xE3F9:
            func_E3F9(); break;
        case 0xD1C7:
            func_D1C7(); break;
        case 0xEC7E:
            func_EC7E(); break;
        case 0xC78F:
            func_C78F(); break;
        case 0xC790:
            func_C790(); break;
        case 0xD01E:
            func_D01E(); break;
        case 0xFF14:
            func_FF14(); break;
        case 0xFF15:
            func_FF15(); break;
        case 0xE3A0:
            func_E3A0(); break;
        case 0xE3A1:
            func_E3A1(); break;
        case 0xE41F:
            func_E41F(); break;
        case 0xE420:
            func_E420(); break;
        case 0xF881:
            func_F881(); break;
        case 0xE3AE:
            func_E3AE(); break;
        case 0xFB1F:
            func_FB1F(); break;
        case 0xF021:
            func_F021(); break;
        case 0xF022:
            func_F022(); break;
        case 0xC7E8:
            func_C7E8(); break;
        case 0xC7E9:
            func_C7E9(); break;
        case 0xF8BD:
            func_F8BD(); break;
        case 0xF407:
            func_F407(); break;
        case 0xF408:
            func_F408(); break;
        case 0xE7E0:
            func_E7E0(); break;
        case 0xE7E1:
            func_E7E1(); break;
        case 0xFD70:
            func_FD70(); break;
        case 0xFD71:
            func_FD71(); break;
        case 0xFC2E:
            func_FC2E(); break;
        case 0xF028:
            func_F028(); break;
        case 0xF029:
            func_F029(); break;
        case 0xF7F0:
            func_F7F0(); break;
        case 0xF7F1:
            func_F7F1(); break;
        case 0xFFB0:
            func_FFB0(); break;
        case 0xFFB1:
            func_FFB1(); break;
        case 0xFE8E:
            func_FE8E(); break;
        case 0xFC0A:
            func_FC0A(); break;
        case 0xEE7F:
            func_EE7F(); break;
        case 0xEE80:
            func_EE80(); break;
        case 0xFF3F:
            func_FF3F(); break;
        case 0xFF8A:
            func_FF8A(); break;
        case 0xFDA2:
            func_FDA2(); break;
        case 0xE3BF:
            func_E3BF(); break;
        case 0xFD08:
            func_FD08(); break;
        case 0xC404:
            func_C404(); break;
        case 0xE01D:
            func_E01D(); break;
        case 0xFE02:
            func_FE02(); break;
        case 0xE3B0:
            func_E3B0(); break;
        case 0xE3B1:
            func_E3B1(); break;
        case 0xFDFF:
            func_FDFF(); break;
        case 0xF03A:
            func_F03A(); break;
        case 0xF03B:
            func_F03B(); break;
        case 0xF802:
            func_F802(); break;
        case 0xF8A1:
            func_F8A1(); break;
        case 0xDFC0:
            func_DFC0(); break;
        case 0xDFC1:
            func_DFC1(); break;
        case 0xF401:
            func_F401(); break;
        case 0xF402:
            func_F402(); break;
        case 0xFF9F:
            func_FF9F(); break;
        case 0xC2E1:
            func_C2E1(); break;
        case 0xEA80:
            func_EA80(); break;
        case 0xEA81:
            func_EA81(); break;
        case 0xC0A0:
            func_C0A0(); break;
        case 0xF1E0:
            func_F1E0(); break;
        case 0xEFBF:
            func_EFBF(); break;
        case 0xEFC0:
            func_EFC0(); break;
        case 0xFB00:
            func_FB00(); break;
        case 0xFB01:
            func_FB01(); break;
        case 0xFE30:
            func_FE30(); break;
        case 0xFFFA:
            func_FFFA(); break;
        case 0xEFFE:
            func_EFFE(); break;
        case 0xFF7C:
            func_FF7C(); break;
        case 0xFF7D:
            func_FF7D(); break;
        case 0xCFFC:
            func_CFFC(); break;
        case 0xFDA4:
            func_FDA4(); break;
        case 0xFDA5:
            func_FDA5(); break;
        case 0xF23F:
            func_F23F(); break;
        case 0xFFB2:
            func_FFB2(); break;
        case 0xE20B:
            func_E20B(); break;
        case 0xE20C:
            func_E20C(); break;
        case 0xDBE2:
            func_DBE2(); break;
        case 0xFE9F:
            func_FE9F(); break;
        case 0xFEA0:
            func_FEA0(); break;
        case 0xFEC5:
            func_FEC5(); break;
        case 0xFEC6:
            func_FEC6(); break;
        case 0xFB62:
            func_FB62(); break;
        case 0xFDBF:
            func_FDBF(); break;
        case 0xEC05:
            func_EC05(); break;
        case 0xEC06:
            func_EC06(); break;
        case 0xFA02:
            func_FA02(); break;
        case 0xFA03:
            func_FA03(); break;
        case 0xFB02:
            func_FB02(); break;
        case 0xED01:
            func_ED01(); break;
        case 0xDC1D:
            func_DC1D(); break;
        case 0xFDE0:
            func_FDE0(); break;
        case 0xF603:
            func_F603(); break;
        case 0xF604:
            func_F604(); break;
        case 0xC096:
            func_C096(); break;
        case 0xF6C0:
            func_F6C0(); break;
        case 0xF01F:
            func_F01F(); break;
        case 0xDA82:
            func_DA82(); break;
        case 0xC9DA:
            func_C9DA(); break;
        case 0xC9DB:
            func_C9DB(); break;
        case 0xDF7F:
            func_DF7F(); break;
        case 0xDF80:
            func_DF80(); break;
        case 0xFFA3:
            func_FFA3(); break;
        case 0xFC12:
            func_FC12(); break;
        case 0xE849:
            func_E849(); break;
        case 0xE84A:
            func_E84A(); break;
        case 0xE882:
            func_E882(); break;
        case 0xFFBA:
            func_FFBA(); break;
        case 0xFFBB:
            func_FFBB(); break;
        case 0xD375:
            func_D375(); break;
        case 0xD912:
            func_D912(); break;
        case 0xD089:
            func_D089(); break;
        case 0xD08A:
            func_D08A(); break;
        case 0xFED0:
            func_FED0(); break;
        case 0xDAA4:
            func_DAA4(); break;
        case 0xFEDB:
            func_FEDB(); break;
        case 0xEC00:
            func_EC00(); break;
        case 0xEC01:
            func_EC01(); break;
        case 0xDF68:
            func_DF68(); break;
        case 0xEA12:
            func_EA12(); break;
        case 0xDA7D:
            func_DA7D(); break;
        case 0xD5DA:
            func_D5DA(); break;
        case 0xD5DB:
            func_D5DB(); break;
        case 0xD4AE:
            func_D4AE(); break;
        case 0xFBA0:
            func_FBA0(); break;
        case 0xFBA2:
            func_FBA2(); break;
        case 0xFBA3:
            func_FBA3(); break;
        case 0xD825:
            func_D825(); break;
        case 0xDB7F:
            func_DB7F(); break;
        case 0xDB80:
            func_DB80(); break;
        case 0xE081:
            func_E081(); break;
        case 0xDB4A:
            func_DB4A(); break;
        case 0xD552:
            func_D552(); break;
        case 0xD095:
            func_D095(); break;
        case 0xF404:
            func_F404(); break;
        case 0xF405:
            func_F405(); break;
        case 0xEA44:
            func_EA44(); break;
        case 0xEA45:
            func_EA45(); break;
        case 0xE928:
            func_E928(); break;
        case 0xE929:
            func_E929(); break;
        case 0xFBE9:
            func_FBE9(); break;
        case 0xFBEA:
            func_FBEA(); break;
        case 0xFDB4:
            func_FDB4(); break;
        case 0xC826:
            func_C826(); break;
        case 0xEEC8:
            func_EEC8(); break;
        case 0xEEC9:
            func_EEC9(); break;
        case 0xF702:
            func_F702(); break;
        case 0xD955:
            func_D955(); break;
        case 0xD956:
            func_D956(); break;
        case 0xDA91:
            func_DA91(); break;
        case 0xD520:
            func_D520(); break;
        case 0xFDD6:
            func_FDD6(); break;
        case 0xEF6A:
            func_EF6A(); break;
        case 0xD496:
            func_D496(); break;
        case 0xED05:
            func_ED05(); break;
        case 0xD6A4:
            func_D6A4(); break;
        case 0xD685:
            func_D685(); break;
        case 0xEA7E:
            func_EA7E(); break;
        case 0xEA7F:
            func_EA7F(); break;
        case 0xE2F1:
            func_E2F1(); break;
        case 0xE006:
            func_E006(); break;
        case 0xDDFF:
            func_DDFF(); break;
        case 0xEC7A:
            func_EC7A(); break;
        case 0xEC7B:
            func_EC7B(); break;
        case 0xF83F:
            func_F83F(); break;
        case 0xF3FE:
            func_F3FE(); break;
        case 0xFAA1:
            func_FAA1(); break;
        case 0xFE09:
            func_FE09(); break;
        case 0xE27E:
            func_E27E(); break;
        case 0xE00B:
            func_E00B(); break;
        case 0xF807:
            func_F807(); break;
        case 0xEFFC:
            func_EFFC(); break;
        case 0xE008:
            func_E008(); break;
        case 0xFA12:
            func_FA12(); break;
        case 0xE810:
            func_E810(); break;
        case 0xFFAD:
            func_FFAD(); break;
        case 0xEF80:
            func_EF80(); break;
        case 0xE3FB:
            func_E3FB(); break;
        case 0xF009:
            func_F009(); break;
        case 0xF816:
            func_F816(); break;
        case 0xFEFF:
            func_FEFF(); break;
        case 0xFF7F:
            func_FF7F(); break;
        case 0xD607:
            func_D607(); break;
        case 0xFE06:
            func_FE06(); break;
        case 0xC7FE:
            func_C7FE(); break;
        case 0xFE1F:
            func_FE1F(); break;
        case 0xFE20:
            func_FE20(); break;
        case 0xE8E0:
            func_E8E0(); break;
        case 0xE8E1:
            func_E8E1(); break;
        case 0xFCFD:
            func_FCFD(); break;
        case 0xF0BB:
            func_F0BB(); break;
        case 0xF5F8:
            func_F5F8(); break;
        case 0xFC4F:
            func_FC4F(); break;
        case 0xE8A9:
            func_E8A9(); break;
        case 0xEBFE:
            func_EBFE(); break;
        case 0xEBFF:
            func_EBFF(); break;
        case 0xFA2F:
            func_FA2F(); break;
        case 0xFA30:
            func_FA30(); break;
        case 0xE83E:
            func_E83E(); break;
        case 0xEA01:
            func_EA01(); break;
        case 0xC2FE:
            func_C2FE(); break;
        case 0xE270:
            func_E270(); break;
        case 0xFE27:
            func_FE27(); break;
        case 0xFFB4:
            func_FFB4(); break;
        case 0xFFB5:
            func_FFB5(); break;
        case 0xF5FA:
            func_F5FA(); break;
        case 0xD022:
            func_D022(); break;
        case 0xF809:
            func_F809(); break;
        case 0xFC7B:
            func_FC7B(); break;
        case 0xFB0C:
            func_FB0C(); break;
        case 0xFB0D:
            func_FB0D(); break;
        case 0xF50F:
            func_F50F(); break;
        case 0xF510:
            func_F510(); break;
        case 0xC2F6:
            func_C2F6(); break;
        case 0xC081:
            func_C081(); break;
        case 0xEFE0:
            func_EFE0(); break;
        case 0xEFE1:
            func_EFE1(); break;
        case 0xE803:
            func_E803(); break;
        case 0xF80A:
            func_F80A(); break;
        case 0xFA40:
            func_FA40(); break;
        case 0xFD17:
            func_FD17(); break;
        case 0xFD18:
            func_FD18(); break;
        case 0xEE00:
            func_EE00(); break;
        case 0xEE01:
            func_EE01(); break;
        case 0xE17E:
            func_E17E(); break;
        case 0xE0A9:
            func_E0A9(); break;
        case 0xFE3E:
            func_FE3E(); break;
        case 0xE81F:
            func_E81F(); break;
        case 0xE804:
            func_E804(); break;
        case 0xE8FC:
            func_E8FC(); break;
        case 0xE8FD:
            func_E8FD(); break;
        case 0xC0BD:
            func_C0BD(); break;
        case 0xFA0F:
            func_FA0F(); break;
        case 0xFA10:
            func_FA10(); break;
        case 0xE2F9:
            func_E2F9(); break;
        case 0xEDF8:
            func_EDF8(); break;
        case 0xEDF9:
            func_EDF9(); break;
        case 0xF023:
            func_F023(); break;
        case 0xF024:
            func_F024(); break;
        case 0xF42A:
            func_F42A(); break;
        case 0xF42B:
            func_F42B(); break;
        case 0xFF05:
            func_FF05(); break;
        case 0xFF06:
            func_FF06(); break;
        case 0xFE82:
            func_FE82(); break;
        case 0xD0FE:
            func_D0FE(); break;
        case 0xD0FF:
            func_D0FF(); break;
        case 0xC061:
            func_C061(); break;
        case 0xF752:
            func_F752(); break;
        case 0xF753:
            func_F753(); break;
        case 0xDC4E:
            func_DC4E(); break;
        case 0xDC4F:
            func_DC4F(); break;
        case 0xD0DC:
            func_D0DC(); break;
        case 0xD0DD:
            func_D0DD(); break;
        case 0xCA81:
            func_CA81(); break;
        case 0xFE89:
            func_FE89(); break;
        case 0xFE8A:
            func_FE8A(); break;
        case 0xD802:
            func_D802(); break;
        case 0xFED8:
            func_FED8(); break;
        case 0xFED9:
            func_FED9(); break;
        case 0xF17A:
            func_F17A(); break;
        case 0xF17B:
            func_F17B(); break;
        case 0xDA8F:
            func_DA8F(); break;
        case 0xC011:
            func_C011(); break;
        case 0xC012:
            func_C012(); break;
        case 0xDFE0:
            func_DFE0(); break;
        case 0xDFE1:
            func_DFE1(); break;
        case 0xED90:
            func_ED90(); break;
        case 0xED91:
            func_ED91(); break;
        case 0xFAFC:
            func_FAFC(); break;
        case 0xFAFD:
            func_FAFD(); break;
        case 0xECE8:
            func_ECE8(); break;
        case 0xECE9:
            func_ECE9(); break;
        case 0xFA80:
            func_FA80(); break;
        case 0xFA81:
            func_FA81(); break;
        case 0xC827:
            func_C827(); break;
        case 0xC828:
            func_C828(); break;
        case 0xC114:
            func_C114(); break;
        case 0xD0A0:
            func_D0A0(); break;
        case 0xFD06:
            func_FD06(); break;
        case 0xFC4B:
            func_FC4B(); break;
        case 0xFA94:
            func_FA94(); break;
        case 0xFA95:
            func_FA95(); break;
        case 0xC561:
            func_C561(); break;
        case 0xEC1E:
            func_EC1E(); break;
        case 0xEC1F:
            func_EC1F(); break;
        case 0xC04F:
            func_C04F(); break;
        case 0xC050:
            func_C050(); break;
        case 0xC56B:
            func_C56B(); break;
        case 0xF403:
            func_F403(); break;
        case 0xD094:
            func_D094(); break;
        case 0xD0AD:
            func_D0AD(); break;
        case 0xF6A0:
            func_F6A0(); break;
        case 0xF6A1:
            func_F6A1(); break;
        case 0xFDCE:
            func_FDCE(); break;
        case 0xFDCF:
            func_FDCF(); break;
        case 0xD2FC:
            func_D2FC(); break;
        case 0xD2FD:
            func_D2FD(); break;
        case 0xED80:
            func_ED80(); break;
        case 0xF005:
            func_F005(); break;
        case 0xDF01:
            func_DF01(); break;
        case 0xF236:
            func_F236(); break;
        case 0xF6B1:
            func_F6B1(); break;
        case 0xFFA9:
            func_FFA9(); break;
        case 0xE049:
            func_E049(); break;
        case 0xE04A:
            func_E04A(); break;
        case 0xC02F:
            func_C02F(); break;
        case 0xC030:
            func_C030(); break;
        case 0xDAE0:
            func_DAE0(); break;
        case 0xC1FB:
            func_C1FB(); break;
        case 0xE857:
            func_E857(); break;
        case 0xE858:
            func_E858(); break;
        case 0xF827:
            func_F827(); break;
        case 0xF828:
            func_F828(); break;
        case 0xDD90:
            func_DD90(); break;
        case 0xE80A:
            func_E80A(); break;
        case 0xE80B:
            func_E80B(); break;
        case 0xEC50:
            func_EC50(); break;
        case 0xEC8B:
            func_EC8B(); break;
        case 0xEC8C:
            func_EC8C(); break;
        case 0xEBB1:
            func_EBB1(); break;
        case 0xFA9F:
            func_FA9F(); break;
        case 0xF206:
            func_F206(); break;
        case 0xF534:
            func_F534(); break;
        case 0xF535:
            func_F535(); break;
        case 0xD17A:
            func_D17A(); break;
        case 0xD17B:
            func_D17B(); break;
        case 0xDA4B:
            func_DA4B(); break;
        case 0xFC5E:
            func_FC5E(); break;
        case 0xEA26:
            func_EA26(); break;
        case 0xFB80:
            func_FB80(); break;
        case 0xE03E:
            func_E03E(); break;
        case 0xD746:
            func_D746(); break;
        case 0xD40C:
            func_D40C(); break;
        case 0xF53F:
            func_F53F(); break;
        case 0xF540:
            func_F540(); break;
        case 0xF607:
            func_F607(); break;
        case 0xF608:
            func_F608(); break;
        case 0xEAF6:
            func_EAF6(); break;
        case 0xEAF7:
            func_EAF7(); break;
        case 0xC0EA:
            func_C0EA(); break;
        case 0xC0EB:
            func_C0EB(); break;
        case 0xE8B1:
            func_E8B1(); break;
        case 0xFE45:
            func_FE45(); break;
        case 0xFE46:
            func_FE46(); break;
        case 0xD15E:
            func_D15E(); break;
        case 0xFF2C:
            func_FF2C(); break;
        case 0xF6D0:
            func_F6D0(); break;
        case 0xF6D1:
            func_F6D1(); break;
        case 0xF617:
            func_F617(); break;
        case 0xF618:
            func_F618(); break;
        case 0xDB52:
            func_DB52(); break;
        case 0xDB53:
            func_DB53(); break;
        case 0xFA13:
            func_FA13(); break;
        case 0xC817:
            func_C817(); break;
        case 0xFB8B:
            func_FB8B(); break;
        case 0xEC03:
            func_EC03(); break;
        case 0xFD07:
            func_FD07(); break;
        case 0xED24:
            func_ED24(); break;
        case 0xFD1D:
            func_FD1D(); break;
        case 0xFD1E:
            func_FD1E(); break;
        case 0xEC04:
            func_EC04(); break;
        case 0xE87E:
            func_E87E(); break;
        case 0xE025:
            func_E025(); break;
        case 0xD0AB:
            func_D0AB(); break;
        case 0xE936:
            func_E936(); break;
        case 0xD02C:
            func_D02C(); break;
        case 0xC03E:
            func_C03E(); break;
        case 0xE04B:
            func_E04B(); break;
        case 0xEEBF:
            func_EEBF(); break;
        case 0xEEC0:
            func_EEC0(); break;
        case 0xE85F:
            func_E85F(); break;
        case 0xF601:
            func_F601(); break;
        case 0xF605:
            func_F605(); break;
        case 0xF44E:
            func_F44E(); break;
        case 0xD11C:
            func_D11C(); break;
        case 0xF409:
            func_F409(); break;
        case 0xF40A:
            func_F40A(); break;
        case 0xD700:
            func_D700(); break;
        case 0xDD00:
            func_DD00(); break;
        case 0xDD01:
            func_DD01(); break;
        case 0xDA96:
            func_DA96(); break;
        case 0xFD0A:
            func_FD0A(); break;
        case 0xFD0B:
            func_FD0B(); break;
        case 0xFEAC:
            func_FEAC(); break;
        case 0xFEAD:
            func_FEAD(); break;
        case 0xFA14:
            func_FA14(); break;
        case 0xD417:
            func_D417(); break;
        case 0xD418:
            func_D418(); break;
        case 0xD88E:
            func_D88E(); break;
        case 0xF13C:
            func_F13C(); break;
        case 0xFA05:
            func_FA05(); break;
        case 0xFA06:
            func_FA06(); break;
        case 0xFD03:
            func_FD03(); break;
        case 0xEA4E:
            func_EA4E(); break;
        case 0xEA25:
            func_EA25(); break;
        case 0xE4EA:
            func_E4EA(); break;
        case 0xE4EB:
            func_E4EB(); break;
        case 0xE859:
            func_E859(); break;
        case 0xEF03:
            func_EF03(); break;
        case 0xEA36:
            func_EA36(); break;
        case 0xEA37:
            func_EA37(); break;
        case 0xF505:
            func_F505(); break;
        case 0xF506:
            func_F506(); break;
        case 0xE2F5:
            func_E2F5(); break;
        case 0xE2F6:
            func_E2F6(); break;
        case 0xE86F:
            func_E86F(); break;
        case 0xED74:
            func_ED74(); break;
        case 0xED75:
            func_ED75(); break;
        case 0xDB65:
            func_DB65(); break;
        case 0xDB66:
            func_DB66(); break;
        case 0xE04F:
            func_E04F(); break;
        case 0xE050:
            func_E050(); break;
        case 0xEDAC:
            func_EDAC(); break;
        case 0xEDAD:
            func_EDAD(); break;
        case 0xDC8E:
            func_DC8E(); break;
        case 0xDAAB:
            func_DAAB(); break;
        case 0xDA92:
            func_DA92(); break;
        case 0xED09:
            func_ED09(); break;
        case 0xD47E:
            func_D47E(); break;
        case 0xECB4:
            func_ECB4(); break;
        case 0xECB5:
            func_ECB5(); break;
        case 0xDB69:
            func_DB69(); break;
        case 0xC4BF:
            func_C4BF(); break;
        case 0xD523:
            func_D523(); break;
        case 0xD8AD:
            func_D8AD(); break;
        case 0xE845:
            func_E845(); break;
        case 0xD820:
            func_D820(); break;
        case 0xEFA9:
            func_EFA9(); break;
        case 0xEFAA:
            func_EFAA(); break;
        case 0xE0EF:
            func_E0EF(); break;
        case 0xE0F0:
            func_E0F0(); break;
        case 0xF406:
            func_F406(); break;
        case 0xD7F4:
            func_D7F4(); break;
        case 0xD7F5:
            func_D7F5(); break;
        case 0xC89B:
            func_C89B(); break;
        case 0xD452:
            func_D452(); break;
        case 0xD4DE:
            func_D4DE(); break;
        case 0xC08E:
            func_C08E(); break;
        case 0xF527:
            func_F527(); break;
        case 0xF417:
            func_F417(); break;
        case 0xF418:
            func_F418(); break;
        case 0xEE02:
            func_EE02(); break;
        case 0xDD42:
            func_DD42(); break;
        case 0xFB40:
            func_FB40(); break;
        case 0xFB41:
            func_FB41(); break;
        case 0xD593:
            func_D593(); break;
        case 0xEDA8:
            func_EDA8(); break;
        case 0xEDA9:
            func_EDA9(); break;
        case 0xFEAB:
            func_FEAB(); break;
        case 0xD002:
            func_D002(); break;
        case 0xC11F:
            func_C11F(); break;
        case 0xDB42:
            func_DB42(); break;
        case 0xEA55:
            func_EA55(); break;
        case 0xEA56:
            func_EA56(); break;
        case 0xEC09:
            func_EC09(); break;
        case 0xDD91:
            func_DD91(); break;
        case 0xF04B:
            func_F04B(); break;
        case 0xF04C:
            func_F04C(); break;
        case 0xF552:
            func_F552(); break;
        case 0xF89F:
            func_F89F(); break;
        case 0xFDD0:
            func_FDD0(); break;
        case 0xFDD1:
            func_FDD1(); break;
        case 0xDA6E:
            func_DA6E(); break;
        case 0xE84B:
            func_E84B(); break;
        case 0xC4A0:
            func_C4A0(); break;
        case 0xEEC4:
            func_EEC4(); break;
        case 0xEEC5:
            func_EEC5(); break;
        case 0xED40:
            func_ED40(); break;
        case 0xED41:
            func_ED41(); break;
        case 0xFD84:
            func_FD84(); break;
        case 0xC497:
            func_C497(); break;
        case 0xC498:
            func_C498(); break;
        case 0xD917:
            func_D917(); break;
        case 0xF682:
            func_F682(); break;
        case 0xF683:
            func_F683(); break;
        case 0xEAAA:
            func_EAAA(); break;
        case 0xEAAB:
            func_EAAB(); break;
        case 0xEC9E:
            func_EC9E(); break;
        case 0xDB20:
            func_DB20(); break;
        case 0xDAA9:
            func_DAA9(); break;
        case 0xD442:
            func_D442(); break;
        case 0xEA0B:
            func_EA0B(); break;
        case 0xD492:
            func_D492(); break;
        case 0xF680:
            func_F680(); break;
        case 0xF681:
            func_F681(); break;
        case 0xD56D:
            func_D56D(); break;
        case 0xE096:
            func_E096(); break;
        case 0xE097:
            func_E097(); break;
        case 0xDDB5:
            func_DDB5(); break;
        case 0xDC18:
            func_DC18(); break;
        case 0xFEA5:
            func_FEA5(); break;
        case 0xE05E:
            func_E05E(); break;
        case 0xEC1D:
            func_EC1D(); break;
        case 0xC840:
            func_C840(); break;
        case 0xDAB7:
            func_DAB7(); break;
        case 0xFF55:
            func_FF55(); break;
        case 0xFF56:
            func_FF56(); break;
        case 0xE7A0:
            func_E7A0(); break;
        case 0xEB8F:
            func_EB8F(); break;
        case 0xEBF1:
            func_EBF1(); break;
        case 0xF8EB:
            func_F8EB(); break;
        case 0xFFA8:
            func_FFA8(); break;
        case 0xFD1F:
            func_FD1F(); break;
        case 0xFD20:
            func_FD20(); break;
        case 0xFFAB:
            func_FFAB(); break;
        case 0xFD0F:
            func_FD0F(); break;
        case 0xFD10:
            func_FD10(); break;
        case 0xC401:
            func_C401(); break;
        case 0xDAFB:
            func_DAFB(); break;
        case 0xDAFC:
            func_DAFC(); break;
        case 0xFE4F:
            func_FE4F(); break;
        case 0xFDA0:
            func_FDA0(); break;
        case 0xE2AF:
            func_E2AF(); break;
        case 0xF7FF:
            func_F7FF(); break;
        case 0xD401:
            func_D401(); break;
        case 0xC702:
            func_C702(); break;
        case 0xFFA7:
            func_FFA7(); break;
        case 0xD1FF:
            func_D1FF(); break;
        case 0xC7F3:
            func_C7F3(); break;
        case 0xC7F4:
            func_C7F4(); break;
        case 0xFD4F:
            func_FD4F(); break;
        case 0xEC08:
            func_EC08(); break;
        case 0xFF7E:
            func_FF7E(); break;
        case 0xDFF2:
            func_DFF2(); break;
        case 0xEFEB:
            func_EFEB(); break;
        case 0xFFEF:
            func_FFEF(); break;
        case 0xFF11:
            func_FF11(); break;
        case 0xF73F:
            func_F73F(); break;
        case 0xF740:
            func_F740(); break;
        case 0xFEB0:
            func_FEB0(); break;
        case 0xEE25:
            func_EE25(); break;
        case 0xEE26:
            func_EE26(); break;
        case 0xFF52:
            func_FF52(); break;
        case 0xFF53:
            func_FF53(); break;
        case 0xC082:
            func_C082(); break;
        case 0xD801:
            func_D801(); break;
        case 0xE97E:
            func_E97E(); break;
        case 0xE97F:
            func_E97F(); break;
        case 0xFF97:
            func_FF97(); break;
        case 0xC04E:
            func_C04E(); break;
        case 0xFA90:
            func_FA90(); break;
        case 0xFA91:
            func_FA91(); break;
        case 0xDBED:
            func_DBED(); break;
        case 0xC04D:
            func_C04D(); break;
        case 0xEC3F:
            func_EC3F(); break;
        case 0xEC40:
            func_EC40(); break;
        case 0xFA96:
            func_FA96(); break;
        case 0xFB50:
            func_FB50(); break;
        case 0xFB51:
            func_FB51(); break;
        case 0xED4B:
            func_ED4B(); break;
        case 0xEEAA:
            func_EEAA(); break;
        case 0xEEAB:
            func_EEAB(); break;
        case 0xFE75:
            func_FE75(); break;
        case 0xE7FF:
            func_E7FF(); break;
        case 0xF940:
            func_F940(); break;
        case 0xF941:
            func_F941(); break;
        case 0xDC7F:
            func_DC7F(); break;
        case 0xDC80:
            func_DC80(); break;
        case 0xD4C0:
            func_D4C0(); break;
        case 0xD413:
            func_D413(); break;
        case 0xDAB5:
            func_DAB5(); break;
        case 0xD45F:
            func_D45F(); break;
        case 0xD101:
            func_D101(); break;
        case 0xD2BD:
            func_D2BD(); break;
        case 0xD2BE:
            func_D2BE(); break;
        case 0xECFE:
            func_ECFE(); break;
        case 0xED4D:
            func_ED4D(); break;
        case 0xED4E:
            func_ED4E(); break;
        case 0xDFFE:
            func_DFFE(); break;
        case 0xDFFF:
            func_DFFF(); break;
        case 0xFED5:
            func_FED5(); break;
        case 0xFAA8:
            func_FAA8(); break;
        case 0xFEDC:
            func_FEDC(); break;
        case 0xDE81:
            func_DE81(); break;
        case 0xECAE:
            func_ECAE(); break;
        case 0xECAF:
            func_ECAF(); break;
        case 0xF895:
            func_F895(); break;
        case 0xE024:
            func_E024(); break;
        case 0xEA0A:
            func_EA0A(); break;
        case 0xD2A8:
            func_D2A8(); break;
        case 0xD2A9:
            func_D2A9(); break;
        case 0xEDD5:
            func_EDD5(); break;
        case 0xEDD6:
            func_EDD6(); break;
        case 0xEE09:
            func_EE09(); break;
        case 0xEE0A:
            func_EE0A(); break;
        case 0xDBA1:
            func_DBA1(); break;
        case 0xF15C:
            func_F15C(); break;
        case 0xF15D:
            func_F15D(); break;
        case 0xDD13:
            func_DD13(); break;
        case 0xFA97:
            func_FA97(); break;
        case 0xFA98:
            func_FA98(); break;
        case 0xEA95:
            func_EA95(); break;
        case 0xFE85:
            func_FE85(); break;
        case 0xFE86:
            func_FE86(); break;
        case 0xD12B:
            func_D12B(); break;
        case 0xD12C:
            func_D12C(); break;
        case 0xF525:
            func_F525(); break;
        case 0xF526:
            func_F526(); break;
        case 0xD945:
            func_D945(); break;
        case 0xDA97:
            func_DA97(); break;
        case 0xD775:
            func_D775(); break;
        case 0xD776:
            func_D776(); break;
        case 0xD557:
            func_D557(); break;
        case 0xEA6E:
            func_EA6E(); break;
        case 0xDDAA:
            func_DDAA(); break;
        case 0xF665:
            func_F665(); break;
        case 0xF666:
            func_F666(); break;
        case 0xD266:
            func_D266(); break;
        case 0xD267:
            func_D267(); break;
        case 0xE866:
            func_E866(); break;
        case 0xE867:
            func_E867(); break;
        case 0xFE66:
            func_FE66(); break;
        case 0xC468:
            func_C468(); break;
        case 0xDA68:
            func_DA68(); break;
        case 0xF485:
            func_F485(); break;
        case 0xF486:
            func_F486(); break;
        case 0xE45F:
            func_E45F(); break;
        case 0xFFA5:
            func_FFA5(); break;
        case 0xFF85:
            func_FF85(); break;
        case 0xFF86:
            func_FF86(); break;
        case 0xF721:
            func_F721(); break;
        case 0xE5F8:
            func_E5F8(); break;
        case 0xE581:
            func_E581(); break;
        case 0xE626:
            func_E626(); break;
        case 0xE47E:
            func_E47E(); break;
        case 0xE595:
            func_E595(); break;
        case 0xF05C:
            func_F05C(); break;
        case 0xF05D:
            func_F05D(); break;
        case 0xE3A4:
            func_E3A4(); break;
        case 0xE3A5:
            func_E3A5(); break;
        case 0xD0E3:
            func_D0E3(); break;
        case 0xD014:
            func_D014(); break;
        case 0xD017:
            func_D017(); break;
        case 0xD018:
            func_D018(); break;
        case 0xF9D0:
            func_F9D0(); break;
        case 0xF011:
            func_F011(); break;
        case 0xF012:
            func_F012(); break;
        case 0xF013:
            func_F013(); break;
        case 0xC914:
            func_C914(); break;
        case 0xF00A:
            func_F00A(); break;
        case 0xC90C:
            func_C90C(); break;
        case 0xC908:
            func_C908(); break;
        case 0xC909:
            func_C909(); break;
        case 0xC904:
            func_C904(); break;
        case 0xC905:
            func_C905(); break;
        case 0xFDA6:
            func_FDA6(); break;
        case 0xFCA6:
            func_FCA6(); break;
        case 0xD0E0:
            func_D0E0(); break;
        case 0xD600:
            func_D600(); break;
        case 0xCA01:
            func_CA01(); break;
        case 0xE4CA:
            func_E4CA(); break;
        case 0xE4CB:
            func_E4CB(); break;
        case 0xF5D0:
            func_F5D0(); break;
        case 0xE62E:
            func_E62E(); break;
        case 0xF018:
            func_F018(); break;
        case 0xF019:
            func_F019(); break;
        case 0xFAD0:
            func_FAD0(); break;
        case 0xFAD1:
            func_FAD1(); break;
        case 0xE698:
            func_E698(); break;
        case 0xE699:
            func_E699(); break;
        case 0xF821:
            func_F821(); break;
        case 0xFD86:
            func_FD86(); break;
        case 0xFC85:
            func_FC85(); break;
        case 0xFC86:
            func_FC86(); break;
        case 0xFB29:
            func_FB29(); break;
        case 0xFB2A:
            func_FB2A(); break;
        case 0xCAFA:
            func_CAFA(); break;
        case 0xCAFB:
            func_CAFB(); break;
        case 0xD0CA:
            func_D0CA(); break;
        case 0xF7D0:
            func_F7D0(); break;
        case 0xF7D1:
            func_F7D1(); break;
        case 0xC901:
            func_C901(); break;
        case 0xC0AA:
            func_C0AA(); break;
        case 0xCA20:
            func_CA20(); break;
        case 0xC801:
            func_C801(); break;
        case 0xF8A9:
            func_F8A9(); break;
        case 0xC8C9:
            func_C8C9(); break;
        case 0xF4D0:
            func_F4D0(); break;
        case 0xF4D1:
            func_F4D1(); break;
        case 0xC6F7:
            func_C6F7(); break;
        case 0xC6F8:
            func_C6F8(); break;
        case 0xEFB1:
            func_EFB1(); break;
        case 0xE8E6:
            func_E8E6(); break;
        case 0xF836:
            func_F836(); break;
        case 0xF837:
            func_F837(); break;
        case 0xF8B5:
            func_F8B5(); break;
        case 0xF8B6:
            func_F8B6(); break;
        case 0xC5F9:
            func_C5F9(); break;
        case 0xE0D1:
            func_E0D1(); break;
        case 0xF6E1:
            func_F6E1(); break;
        case 0xF896:
            func_F896(); break;
        case 0xFA55:
            func_FA55(); break;
        case 0xFA56:
            func_FA56(); break;
        case 0xF835:
            func_F835(); break;
        case 0xEB20:
            func_EB20(); break;
        case 0xEB21:
            func_EB21(); break;
        case 0xF0B5:
            func_F0B5(); break;
        case 0xF0B6:
            func_F0B6(); break;
        case 0xED4C:
            func_ED4C(); break;
        case 0xF620:
            func_F620(); break;
        case 0xF621:
            func_F621(); break;
        case 0xE785:
            func_E785(); break;
        case 0xE5E3:
            func_E5E3(); break;
        case 0xCE21:
            func_CE21(); break;
        case 0xEBA5:
            func_EBA5(); break;
        case 0xF630:
            func_F630(); break;
        case 0xF017:
            func_F017(); break;
        case 0xF02A:
            func_F02A(); break;
        case 0xD7F1:
            func_D7F1(); break;
        case 0xC9D7:
            func_C9D7(); break;
        case 0xC9D8:
            func_C9D8(); break;
        case 0xF0C9:
            func_F0C9(); break;
        case 0xF0CA:
            func_F0CA(); break;
        case 0xCF90:
            func_CF90(); break;
        case 0xC972:
            func_C972(); break;
        case 0xC973:
            func_C973(); break;
        case 0xE73B:
            func_E73B(); break;
        case 0xECBD:
            func_ECBD(); break;
        case 0xECBE:
            func_ECBE(); break;
        case 0xF01B:
            func_F01B(); break;
        case 0xD01A:
            func_D01A(); break;
        case 0xE019:
            func_E019(); break;
        case 0xE01A:
            func_E01A(); break;
        case 0xE016:
            func_E016(); break;
        case 0xE011:
            func_E011(); break;
        case 0xF0D0:
            func_F0D0(); break;
        case 0xF0D1:
            func_F0D1(); break;
        case 0xC9F1:
            func_C9F1(); break;
        case 0xECD0:
            func_ECD0(); break;
        case 0xECD1:
            func_ECD1(); break;
        case 0xE76B:
            func_E76B(); break;
        case 0xD006:
            func_D006(); break;
        case 0xE05B:
            func_E05B(); break;
        case 0xE6D1:
            func_E6D1(); break;
        case 0xE7D8:
            func_E7D8(); break;
        case 0xE4D0:
            func_E4D0(); break;
        case 0xE4D1:
            func_E4D1(); break;
        case 0xCAE4:
            func_CAE4(); break;
        case 0xCAE5:
            func_CAE5(); break;
        case 0xE0CA:
            func_E0CA(); break;
        case 0xE0CB:
            func_E0CB(); break;
        case 0xD01C:
            func_D01C(); break;
        case 0xCB4C:
            func_CB4C(); break;
        case 0xCB4D:
            func_CB4D(); break;
        case 0xE7CC:
            func_E7CC(); break;
        case 0xC820:
            func_C820(); break;
        case 0xE0B8:
            func_E0B8(); break;
        case 0xE0B9:
            func_E0B9(); break;
        case 0xD4D0:
            func_D4D0(); break;
        case 0xD4D1:
            func_D4D1(); break;
        case 0xD4F1:
            func_D4F1(); break;
        case 0xC8D0:
            func_C8D0(); break;
        case 0xCE56:
            func_CE56(); break;
        case 0xCE57:
            func_CE57(); break;
        case 0xE938:
            func_E938(); break;
        case 0xEE7A:
            func_EE7A(); break;
        case 0xE70F:
            func_E70F(); break;
        case 0xCEE8:
            func_CEE8(); break;
        case 0xF052:
            func_F052(); break;
        case 0xEBE0:
            func_EBE0(); break;
        case 0xEBE1:
            func_EBE1(); break;
        case 0xFEBA:
            func_FEBA(); break;
        case 0xF3C9:
            func_F3C9(); break;
        case 0xF3CA:
            func_F3CA(); break;
        case 0xC420:
            func_C420(); break;
        case 0xF7A6:
            func_F7A6(); break;
        case 0xF0F7:
            func_F0F7(); break;
        case 0xF786:
            func_F786(); break;
        case 0xD0F0:
            func_D0F0(); break;
        case 0xD0F1:
            func_D0F1(); break;
        case 0xF3D1:
            func_F3D1(); break;
        case 0xD068:
            func_D068(); break;
        case 0xEDD0:
            func_EDD0(); break;
        case 0xFF8D:
            func_FF8D(); break;
        case 0xE67F:
            func_E67F(); break;
        case 0xE680:
            func_E680(); break;
        case 0xF4E6:
            func_F4E6(); break;
        case 0xE94C:
            func_E94C(); break;
        case 0xE978:
            func_E978(); break;
        case 0xD8E9:
            func_D8E9(); break;
        case 0xE9D9:
            func_E9D9(); break;
        case 0xEA6C:
            func_EA6C(); break;
        case 0xE9A2:
            func_E9A2(); break;
        case 0xFC92:
            func_FC92(); break;
        case 0xC3E9:
            func_C3E9(); break;
        case 0xF5A6:
            func_F5A6(); break;
        case 0xC9F6:
            func_C9F6(); break;
        case 0xF05B:
            func_F05B(); break;
        case 0xF6A5:
            func_F6A5(); break;
        case 0xF6A6:
            func_F6A6(); break;
        case 0xF0A5:
            func_F0A5(); break;
        case 0xF0A6:
            func_F0A6(); break;
        case 0xF3A5:
            func_F3A5(); break;
        case 0xF3A6:
            func_F3A6(); break;
        case 0xD0F3:
            func_D0F3(); break;
        case 0xE611:
            func_E611(); break;
        case 0xE612:
            func_E612(); break;
        case 0xF3E6:
            func_F3E6(); break;
        case 0xF3E7:
            func_F3E7(); break;
        case 0xFF99:
            func_FF99(); break;
        case 0xF7E5:
            func_F7E5(); break;
        case 0xF7E6:
            func_F7E6(); break;
        case 0xE9F8:
            func_E9F8(); break;
        case 0xE617:
            func_E617(); break;
        case 0xE618:
            func_E618(); break;
        case 0xEA0E:
            func_EA0E(); break;
        case 0xE615:
            func_E615(); break;
        case 0xDE20:
            func_DE20(); break;
        case 0xDE21:
            func_DE21(); break;
        case 0xE67A:
            func_E67A(); break;
        case 0xE78B:
            func_E78B(); break;
        case 0xD904:
            func_D904(); break;
        case 0xEA62:
            func_EA62(); break;
        case 0xEA63:
            func_EA63(); break;
        case 0xD0EA:
            func_D0EA(); break;
        case 0xD0EB:
            func_D0EB(); break;
        case 0xEA59:
            func_EA59(); break;
        case 0xEA5A:
            func_EA5A(); break;
        case 0xF311:
            func_F311(); break;
        case 0xEBA6:
            func_EBA6(); break;
        case 0xC9EB:
            func_C9EB(); break;
        case 0xD05B:
            func_D05B(); break;
        case 0xF066:
            func_F066(); break;
        case 0xF067:
            func_F067(); break;
        case 0xD062:
            func_D062(); break;
        case 0xD063:
            func_D063(); break;
        case 0xD07B:
            func_D07B(); break;
        case 0xF710:
            func_F710(); break;
        case 0xF711:
            func_F711(); break;
        case 0xE6C7:
            func_E6C7(); break;
        case 0xDDEB:
            func_DDEB(); break;
        case 0xDDEC:
            func_DDEC(); break;
        case 0xC0F7:
            func_C0F7(); break;
        case 0xEE06:
            func_EE06(); break;
        case 0xEE07:
            func_EE07(); break;
        case 0xC912:
            func_C912(); break;
        case 0xEB97:
            func_EB97(); break;
        case 0xEBAB:
            func_EBAB(); break;
        case 0xC0EC:
            func_C0EC(); break;
        case 0xC0ED:
            func_C0ED(); break;
        case 0xEB63:
            func_EB63(); break;
        case 0xEB77:
            func_EB77(); break;
        case 0xF0A7:
            func_F0A7(); break;
        case 0xC59E:
            func_C59E(); break;
        case 0xEB7F:
            func_EB7F(); break;
        case 0xEB87:
            func_EB87(); break;
        case 0xF385:
            func_F385(); break;
        case 0xF386:
            func_F386(); break;
        case 0xF44C:
            func_F44C(); break;
        case 0xF44D:
            func_F44D(); break;
        case 0xE682:
            func_E682(); break;
        case 0xD07D:
            func_D07D(); break;
        case 0xF24C:
            func_F24C(); break;
        case 0xF24D:
            func_F24D(); break;
        case 0xEBF3:
            func_EBF3(); break;
        case 0xEA4C:
            func_EA4C(); break;
        case 0xEBEB:
            func_EBEB(); break;
        case 0xC925:
            func_C925(); break;
        case 0xC926:
            func_C926(); break;
        case 0xF8CA:
            func_F8CA(); break;
        case 0xF88D:
            func_F88D(); break;
        case 0xF08E:
            func_F08E(); break;
        case 0xC54C:
            func_C54C(); break;
        case 0xC966:
            func_C966(); break;
        case 0xD025:
            func_D025(); break;
        case 0xE7D0:
            func_E7D0(); break;
        case 0xD03D:
            func_D03D(); break;
        case 0xC9E0:
            func_C9E0(); break;
        case 0xF8A6:
            func_F8A6(); break;
        case 0xF0E0:
            func_F0E0(); break;
        case 0xF0E1:
            func_F0E1(); break;
        case 0xED8A:
            func_ED8A(); break;
        case 0xFAA5:
            func_FAA5(); break;
        case 0xFAA6:
            func_FAA6(); break;
        case 0xF0E2:
            func_F0E2(); break;
        case 0xE603:
            func_E603(); break;
        case 0xE604:
            func_E604(); break;
        case 0xF0AE:
            func_F0AE(); break;
        case 0xEDA1:
            func_EDA1(); break;
        case 0xED7E:
            func_ED7E(); break;
        case 0xF76A:
            func_F76A(); break;
        case 0xF370:
            func_F370(); break;
        case 0xF4A1:
            func_F4A1(); break;
        case 0xF92B:
            func_F92B(); break;
        case 0xD04A:
            func_D04A(); break;
        case 0xD04B:
            func_D04B(); break;
        case 0xF061:
            func_F061(); break;
        case 0xD071:
            func_D071(); break;
        case 0xFE99:
            func_FE99(); break;
        case 0xFB75:
            func_FB75(); break;
        case 0xD420:
            func_D420(); break;
        case 0xD021:
            func_D021(); break;
        case 0xCBD0:
            func_CBD0(); break;
        case 0xF320:
            func_F320(); break;
        case 0xF321:
            func_F321(); break;
        case 0xED6B:
            func_ED6B(); break;
        case 0xE5AD:
            func_E5AD(); break;
        case 0xE5AE:
            func_E5AE(); break;
        case 0xE58D:
            func_E58D(); break;
        case 0xE58E:
            func_E58E(); break;
        case 0xEA21:
            func_EA21(); break;
        case 0xE621:
            func_E621(); break;
        case 0xFF6D:
            func_FF6D(); break;
        case 0xC9C0:
            func_C9C0(); break;
        case 0xC9C1:
            func_C9C1(); break;
        case 0xF041:
            func_F041(); break;
        case 0xF8AD:
            func_F8AD(); break;
        case 0xF8AE:
            func_F8AE(); break;
        case 0xEFD1:
            func_EFD1(); break;
        case 0xCCAD:
            func_CCAD(); break;
        case 0xD2CC:
            func_D2CC(); break;
        case 0xD2CD:
            func_D2CD(); break;
        case 0xD5D2:
            func_D5D2(); break;
        case 0xDFD6:
            func_DFD6(); break;
        case 0xE0F8:
            func_E0F8(); break;
        case 0xF0A1:
            func_F0A1(); break;
        case 0xDDC0:
            func_DDC0(); break;
        case 0xDDC1:
            func_DDC1(); break;
        case 0xEE49:
            func_EE49(); break;
        case 0xF0C1:
            func_F0C1(); break;
        case 0xE401:
            func_E401(); break;
        case 0xE402:
            func_E402(); break;
        case 0xD907:
            func_D907(); break;
        case 0xEBD9:
            func_EBD9(); break;
        case 0xEBDA:
            func_EBDA(); break;
        case 0xEDEB:
            func_EDEB(); break;
        case 0xEDEC:
            func_EDEC(); break;
        case 0xF8D0:
            func_F8D0(); break;
        case 0xF8D1:
            func_F8D1(); break;
        case 0xC971:
            func_C971(); break;
        case 0xD080:
            func_D080(); break;
        case 0xC984:
            func_C984(); break;
        case 0xC985:
            func_C985(); break;
        case 0xC0B5:
            func_C0B5(); break;
        case 0xC098:
            func_C098(); break;
        case 0xC086:
            func_C086(); break;
        case 0xD00C:
            func_D00C(); break;
        case 0xD00D:
            func_D00D(); break;
        case 0xEBD0:
            func_EBD0(); break;
        case 0xEBD1:
            func_EBD1(); break;
        case 0xD3B6:
            func_D3B6(); break;
        case 0xD0D4:
            func_D0D4(); break;
        case 0xD395:
            func_D395(); break;
        case 0xD720:
            func_D720(); break;
        case 0xEDD8:
            func_EDD8(); break;
        case 0xFA21:
            func_FA21(); break;
        case 0xEDFB:
            func_EDFB(); break;
        case 0xDFB0:
            func_DFB0(); break;
        case 0xF070:
            func_F070(); break;
        case 0xD6F1:
            func_D6F1(); break;
        case 0xF036:
            func_F036(); break;
        case 0xD3D6:
            func_D3D6(); break;
        case 0xC685:
            func_C685(); break;
        case 0xD90B:
            func_D90B(); break;
        case 0xEF67:
            func_EF67(); break;
        case 0xF0EF:
            func_F0EF(); break;
        case 0xF068:
            func_F068(); break;
        case 0xEE04:
            func_EE04(); break;
        case 0xEEB9:
            func_EEB9(); break;
        case 0xD098:
            func_D098(); break;
        case 0xD099:
            func_D099(); break;
        case 0xD03E:
            func_D03E(); break;
        case 0xDBD1:
            func_DBD1(); break;
        case 0xF8BE:
            func_F8BE(); break;
        case 0xC3BA:
            func_C3BA(); break;
        case 0xD06D:
            func_D06D(); break;
        case 0xD06E:
            func_D06E(); break;
        case 0xD054:
            func_D054(); break;
        case 0xE06F:
            func_E06F(); break;
        case 0xE321:
            func_E321(); break;
        case 0xE984:
            func_E984(); break;
        case 0xE985:
            func_E985(); break;
        case 0xE971:
            func_E971(); break;
        case 0xDD3D:
            func_DD3D(); break;
        case 0xF054:
            func_F054(); break;
        case 0xF14F:
            func_F14F(); break;
        case 0xF12A:
            func_F12A(); break;
        case 0xF1D1:
            func_F1D1(); break;
        case 0xF199:
            func_F199(); break;
        case 0xE031:
            func_E031(); break;
        case 0xF1A1:
            func_F1A1(); break;
        case 0xF14A:
            func_F14A(); break;
        case 0xF885:
            func_F885(); break;
        case 0xD065:
            func_D065(); break;
        case 0xD901:
            func_D901(); break;
        case 0xDFD9:
            func_DFD9(); break;
        case 0xDFDA:
            func_DFDA(); break;
        case 0xF0DF:
            func_F0DF(); break;
        case 0xD385:
            func_D385(); break;
        case 0xE60E:
            func_E60E(); break;
        case 0xE60F:
            func_E60F(); break;
        case 0xF1B0:
            func_F1B0(); break;
        case 0xC3F2:
            func_C3F2(); break;
        case 0xF1DB:
            func_F1DB(); break;
        case 0xF1B1:
            func_F1B1(); break;
        case 0xECF0:
            func_ECF0(); break;
        case 0xFB10:
            func_FB10(); break;
        case 0xFB11:
            func_FB11(); break;
        case 0xF232:
            func_F232(); break;
        case 0xF21F:
            func_F21F(); break;
        case 0xD08D:
            func_D08D(); break;
        case 0xD08E:
            func_D08E(); break;
        case 0xC90E:
            func_C90E(); break;
        case 0xE34C:
            func_E34C(); break;
        case 0xE34D:
            func_E34D(); break;
        case 0xF2E4:
            func_F2E4(); break;
        case 0xEAD0:
            func_EAD0(); break;
        case 0xEAD1:
            func_EAD1(); break;
        case 0xD905:
            func_D905(); break;
        case 0xF20E:
            func_F20E(); break;
        case 0xF4CA:
            func_F4CA(); break;
        case 0xF0F5:
            func_F0F5(); break;
        case 0xD00A:
            func_D00A(); break;
        case 0xC93B:
            func_C93B(); break;
        case 0xC55A:
            func_C55A(); break;
        case 0xFD7A:
            func_FD7A(); break;
        case 0xF1FD:
            func_F1FD(); break;
        case 0xF1FE:
            func_F1FE(); break;
        case 0xE421:
            func_E421(); break;
        case 0xEEE5:
            func_EEE5(); break;
        case 0xF09E:
            func_F09E(); break;
        case 0xF421:
            func_F421(); break;
        case 0xFA3D:
            func_FA3D(); break;
        case 0xE605:
            func_E605(); break;
        case 0xE601:
            func_E601(); break;
        case 0xE602:
            func_E602(); break;
        case 0xE721:
            func_E721(); break;
        case 0xF35B:
            func_F35B(); break;
        case 0xC968:
            func_C968(); break;
        case 0xC969:
            func_C969(); break;
        case 0xF41E:
            func_F41E(); break;
        case 0xF50D:
            func_F50D(); break;
        case 0xF509:
            func_F509(); break;
        case 0xF50A:
            func_F50A(); break;
        case 0xC00D:
            func_C00D(); break;
        case 0xE022:
            func_E022(); break;
        case 0xE023:
            func_E023(); break;
        case 0xF057:
            func_F057(); break;
        case 0xF058:
            func_F058(); break;
        case 0xD009:
            func_D009(); break;
        case 0xF856:
            func_F856(); break;
        case 0xF6F8:
            func_F6F8(); break;
        case 0xFEAA:
            func_FEAA(); break;
        case 0xF36B:
            func_F36B(); break;
        case 0xF36C:
            func_F36C(); break;
        case 0xFCC9:
            func_FCC9(); break;
        case 0xF538:
            func_F538(); break;
        case 0xF539:
            func_F539(); break;
        case 0xF456:
            func_F456(); break;
        case 0xE538:
            func_E538(); break;
        case 0xE539:
            func_E539(); break;
        case 0xE8CA:
            func_E8CA(); break;
        case 0xD695:
            func_D695(); break;
        case 0xD696:
            func_D696(); break;
        case 0xD0E9:
            func_D0E9(); break;
        case 0xF412:
            func_F412(); break;
        case 0xFBF0:
            func_FBF0(); break;
        case 0xFBF1:
            func_FBF1(); break;
        case 0xD028:
            func_D028(); break;
        case 0xF60E:
            func_F60E(); break;
        case 0xC90F:
            func_C90F(); break;
        case 0xC918:
            func_C918(); break;
        case 0xC919:
            func_C919(); break;
        case 0xF5B7:
            func_F5B7(); break;
        case 0xE671:
            func_E671(); break;
        case 0xE028:
            func_E028(); break;
        case 0xE14D:
            func_E14D(); break;
        case 0xF6B9:
            func_F6B9(); break;
        case 0xD05C:
            func_D05C(); break;
        case 0xF715:
            func_F715(); break;
        case 0xC9F8:
            func_C9F8(); break;
        case 0xE0AC:
            func_E0AC(); break;
        case 0xD09D:
            func_D09D(); break;
        case 0xD09E:
            func_D09E(); break;
        case 0xD34C:
            func_D34C(); break;
        case 0xF6D4:
            func_F6D4(); break;
        case 0xDEF0:
            func_DEF0(); break;
        case 0xC951:
            func_C951(); break;
        case 0xF7B9:
            func_F7B9(); break;
        case 0xF7BA:
            func_F7BA(); break;
        case 0xF4F7:
            func_F4F7(); break;
        case 0xF4F8:
            func_F4F8(); break;
        case 0xE5B9:
            func_E5B9(); break;
        case 0xE5BA:
            func_E5BA(); break;
        case 0xF4E5:
            func_F4E5(); break;
        case 0xEEBA:
            func_EEBA(); break;
        case 0xF4EE:
            func_F4EE(); break;
        case 0xF4EF:
            func_F4EF(); break;
        case 0xF515:
            func_F515(); break;
        case 0xF516:
            func_F516(); break;
        case 0xF51A:
            func_F51A(); break;
        case 0xF502:
            func_F502(); break;
        case 0xF901:
            func_F901(); break;
        case 0xF902:
            func_F902(); break;
        case 0xF2F7:
            func_F2F7(); break;
        case 0xF2F8:
            func_F2F8(); break;
        case 0xF5F6:
            func_F5F6(); break;
        case 0xF6F5:
            func_F6F5(); break;
        case 0xF6F6:
            func_F6F6(); break;
        case 0xDE24:
            func_DE24(); break;
        case 0xF088:
            func_F088(); break;
        case 0xFCA9:
            func_FCA9(); break;
        case 0xFCAA:
            func_FCAA(); break;
        case 0xF74A:
            func_F74A(); break;
        case 0xE0F5:
            func_E0F5(); break;
        case 0xF00E:
            func_F00E(); break;
        case 0xF7F3:
            func_F7F3(); break;
        case 0xECC9:
            func_ECC9(); break;
        case 0xECCA:
            func_ECCA(); break;
        case 0xC54B:
            func_C54B(); break;
        case 0xDED1:
            func_DED1(); break;
        case 0xD791:
            func_D791(); break;
        case 0xF628:
            func_F628(); break;
        case 0xF629:
            func_F629(); break;
        case 0xFA8A:
            func_FA8A(); break;
        case 0xFA73:
            func_FA73(); break;
        case 0xF90E:
            func_F90E(); break;
        case 0xF07E:
            func_F07E(); break;
        case 0xF887:
            func_F887(); break;
        case 0xF91D:
            func_F91D(); break;
        case 0xF91E:
            func_F91E(); break;
        case 0xC961:
            func_C961(); break;
        case 0xF922:
            func_F922(); break;
        case 0xF923:
            func_F923(); break;
        case 0xF926:
            func_F926(); break;
        case 0xF927:
            func_F927(); break;
        case 0xD055:
            func_D055(); break;
        case 0xE54C:
            func_E54C(); break;
        case 0xE54D:
            func_E54D(); break;
        case 0xC80F:
            func_C80F(); break;
        case 0xC810:
            func_C810(); break;
        case 0xF379:
            func_F379(); break;
        case 0xF37A:
            func_F37A(); break;
        case 0xF90F:
            func_F90F(); break;
        case 0xF307:
            func_F307(); break;
        case 0xF308:
            func_F308(); break;
        case 0xF9F3:
            func_F9F3(); break;
        case 0xF9F4:
            func_F9F4(); break;
        case 0xD97A:
            func_D97A(); break;
        case 0xF9DA:
            func_F9DA(); break;
        case 0xDC79:
            func_DC79(); break;
        case 0xDC7A:
            func_DC7A(); break;
        case 0xF9DC:
            func_F9DC(); break;
        case 0xF9DD:
            func_F9DD(); break;
        case 0xF9C3:
            func_F9C3(); break;
        case 0xDC10:
            func_DC10(); break;
        case 0xDC11:
            func_DC11(); break;
        case 0xC921:
            func_C921(); break;
        case 0xF4F0:
            func_F4F0(); break;
        case 0xF4F1:
            func_F4F1(); break;
        case 0xDE13:
            func_DE13(); break;
        case 0xDE14:
            func_DE14(); break;
        case 0xFAAF:
            func_FAAF(); break;
        case 0xE4BD:
            func_E4BD(); break;
        case 0xE4BE:
            func_E4BE(); break;
        case 0xE49D:
            func_E49D(); break;
        case 0xE49E:
            func_E49E(); break;
        case 0xFAD4:
            func_FAD4(); break;
        case 0xE48E:
            func_E48E(); break;
        case 0xC989:
            func_C989(); break;
        case 0xC98A:
            func_C98A(); break;
        case 0xE3E3:
            func_E3E3(); break;
        case 0xE3E4:
            func_E3E4(); break;
        case 0xE1E3:
            func_E1E3(); break;
        case 0xF93F:
            func_F93F(); break;
        case 0xFAF9:
            func_FAF9(); break;
        case 0xFAFA:
            func_FAFA(); break;
        case 0xFF2F:
            func_FF2F(); break;
        case 0xF146:
            func_F146(); break;
        case 0xF147:
            func_F147(); break;
        case 0xFBAA:
            func_FBAA(); break;
        case 0xFBAB:
            func_FBAB(); break;
        case 0xF01E:
            func_F01E(); break;
        case 0xDA4D:
            func_DA4D(); break;
        case 0xFCDB:
            func_FCDB(); break;
        case 0xC9FC:
            func_C9FC(); break;
        case 0xDB4D:
            func_DB4D(); break;
        case 0xFC95:
            func_FC95(); break;
        case 0xFF8B:
            func_FF8B(); break;
        case 0xFF8C:
            func_FF8C(); break;
        case 0xF89B:
            func_F89B(); break;
        case 0xF888:
            func_F888(); break;
        case 0xF889:
            func_F889(); break;
        case 0xCB9D:
            func_CB9D(); break;
        case 0xC99B:
            func_C99B(); break;
        case 0xC99C:
            func_C99C(); break;
        case 0xF09C:
            func_F09C(); break;
        case 0xEF8C:
            func_EF8C(); break;
        case 0xEF8D:
            func_EF8D(); break;
        case 0xE4AB:
            func_E4AB(); break;
        case 0xE4AC:
            func_E4AC(); break;
        case 0xFE41:
            func_FE41(); break;
        case 0xFE51:
            func_FE51(); break;
        case 0xFE91:
            func_FE91(); break;
        case 0xF0A9:
            func_F0A9(); break;
        case 0xF0AA:
            func_F0AA(); break;
        case 0xF095:
            func_F095(); break;
        case 0xF096:
            func_F096(); break;
        case 0xEB95:
            func_EB95(); break;
        case 0xEBA9:
            func_EBA9(); break;
        case 0xDE94:
            func_DE94(); break;
        case 0xFE61:
            func_FE61(); break;
        case 0xD782:
            func_D782(); break;
        case 0xFEA7:
            func_FEA7(); break;
        case 0xF4FE:
            func_F4FE(); break;
        case 0xF78E:
            func_F78E(); break;
        case 0xF014:
            func_F014(); break;
        case 0xF015:
            func_F015(); break;
        case 0xC91D:
            func_C91D(); break;
        case 0xC91E:
            func_C91E(); break;
        case 0xF01D:
            func_F01D(); break;
        case 0xC94E:
            func_C94E(); break;
        case 0xF02E:
            func_F02E(); break;
        case 0xC942:
            func_C942(); break;
        case 0xC8B0:
            func_C8B0(); break;
        case 0xC8B1:
            func_C8B1(); break;
        case 0xEFB9:
            func_EFB9(); break;
        case 0xEFBA:
            func_EFBA(); break;
        case 0xFAF0:
            func_FAF0(); break;
        case 0xFB4E:
            func_FB4E(); break;
        case 0xCA4C:
            func_CA4C(); break;
        case 0xCA4D:
            func_CA4D(); break;
        case 0xFECB:
            func_FECB(); break;
        case 0xD687:
            func_D687(); break;
        case 0xD688:
            func_D688(); break;
        case 0xFED7:
            func_FED7(); break;
        case 0xD68C:
            func_D68C(); break;
        case 0xFE49:
            func_FE49(); break;
        case 0xFE79:
            func_FE79(); break;
        case 0xE494:
            func_E494(); break;
        case 0xE495:
            func_E495(); break;
        case 0xE4A8:
            func_E4A8(); break;
        case 0xE4A9:
            func_E4A9(); break;
        case 0xE2FC:
            func_E2FC(); break;
        case 0xE2FD:
            func_E2FD(); break;
        case 0xFE59:
            func_FE59(); break;
        case 0xFE69:
            func_FE69(); break;
        case 0xFE71:
            func_FE71(); break;
        case 0xDA80:
            func_DA80(); break;
        case 0xDAFF:
            func_DAFF(); break;
        case 0xFED2:
            func_FED2(); break;
        case 0xE24C:
            func_E24C(); break;
        case 0xEE4C:
            func_EE4C(); break;
        case 0xEE4D:
            func_EE4D(); break;
        case 0xFC4D:
            func_FC4D(); break;
        case 0xF0BE:
            func_F0BE(); break;
        case 0xDE04:
            func_DE04(); break;
        case 0xF0DE:
            func_F0DE(); break;
        case 0xC059:
            func_C059(); break;
        case 0xC05A:
            func_C05A(); break;
        case 0xE8B9:
            func_E8B9(); break;
        case 0xE8BA:
            func_E8BA(); break;
        case 0xFEE9:
            func_FEE9(); break;
        case 0xEEF0:
            func_EEF0(); break;
        case 0xCEEE:
            func_CEEE(); break;
        case 0xD878:
            func_D878(); break;
        case 0xD879:
            func_D879(); break;
        case 0xF9F0:
            func_F9F0(); break;
        case 0xF9F1:
            func_F9F1(); break;
        case 0xE441:
            func_E441(); break;
        case 0xD741:
            func_D741(); break;
        case 0xD742:
            func_D742(); break;
        case 0xC8D7:
            func_C8D7(); break;
        case 0xFF51:
            func_FF51(); break;
        case 0xC0B2:
            func_C0B2(); break;
        case 0xCFB7:
            func_CFB7(); break;
        case 0xCABE:
            func_CABE(); break;
        case 0xFBDB:
            func_FBDB(); break;
        case 0xC2FA:
            func_C2FA(); break;
        case 0xC2C3:
            func_C2C3(); break;
        case 0xCF2E:
            func_CF2E(); break;
        case 0xCEF4:
            func_CEF4(); break;
        case 0xF26A:
            func_F26A(); break;
        case 0xBFF6:
            func_BFF6_b7(); break;
        case 0xC3E4:
            func_C3E4(); break;
        case 0xE309:
            func_E309(); break;
        case 0xF272:
            func_F272(); break;
        case 0xC3AE:
            func_C3AE(); break;
        case 0xC3E1:
            func_C3E1(); break;
        case 0xCBA2:
            func_CBA2(); break;
        case 0xBFFC:
            func_BFFC_b7(); break;
        case 0xBFD3:
            func_BFD3_b7(); break;
        case 0xCCA5:
            func_CCA5(); break;
        case 0xC009:
            func_C009(); break;
        case 0xC3FD:
            func_C3FD(); break;
        case 0xC3F5:
            func_C3F5(); break;
        case 0xC085:
            func_C085(); break;
        case 0xC0BF:
            func_C0BF(); break;
        case 0xBFF2:
            func_BFF2_b7(); break;
        case 0xBFF4:
            func_BFF4_b7(); break;
        case 0xBFB3:
            func_BFB3_b7(); break;
        case 0x8029:
            switch (g_current_bank) {
                case 0: func_8029_b0(); break;
                case 1: func_8029_b1(); break;
                case 2: func_8029_b2(); break;
                case 3: func_8029_b3(); break;
                case 4: func_8029_b4(); break;
                case 5: func_8029_b5(); break;
                case 6: func_8029_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xFBBB:
            func_FBBB(); break;
        case 0xFBBD:
            func_FBBD(); break;
        case 0xF062:
            func_F062(); break;
        case 0xDD1B:
            func_DD1B(); break;
        case 0xFF0E:
            func_FF0E(); break;
        case 0xDB36:
            func_DB36(); break;
        case 0xE164:
            func_E164(); break;
        case 0xC71F:
            func_C71F(); break;
        case 0xF1E6:
            func_F1E6(); break;
        case 0xFEBD:
            func_FEBD(); break;
        case 0xC0DB:
            func_C0DB(); break;
        case 0xC0DD:
            func_C0DD(); break;
        case 0xC0DF:
            func_C0DF(); break;
        case 0xFB7B:
            func_FB7B(); break;
        case 0xFB6C:
            func_FB6C(); break;
        case 0xC0E1:
            func_C0E1(); break;
        case 0xF3BD:
            func_F3BD(); break;
        case 0xEA7A:
            func_EA7A(); break;
        case 0xECDE:
            func_ECDE(); break;
        case 0xDF44:
            func_DF44(); break;
        case 0xFBA1:
            func_FBA1(); break;
        case 0xE9E4:
            func_E9E4(); break;
        case 0xD5CE:
            func_D5CE(); break;
        case 0xE8D3:
            func_E8D3(); break;
        case 0xFB7F:
            func_FB7F(); break;
        case 0xEE88:
            func_EE88(); break;
        case 0xD511:
            func_D511(); break;
        case 0xC043:
            func_C043(); break;
        case 0xC0B4:
            func_C0B4(); break;
        case 0xF14C:
            func_F14C(); break;
        case 0xC04A:
            func_C04A(); break;
        case 0xD2C0:
            func_D2C0(); break;
        case 0xD2E9:
            func_D2E9(); break;
        case 0xD2AC:
            func_D2AC(); break;
        case 0xF520:
            func_F520(); break;
        case 0xD6EC:
            func_D6EC(); break;
        case 0xD3CB:
            func_D3CB(); break;
        case 0x9D06:
            switch (g_current_bank) {
                case 0: func_9D06_b0(); break;
                case 1: func_9D06_b1(); break;
                case 2: func_9D06_b2(); break;
                case 3: func_9D06_b3(); break;
                case 4: func_9D06_b4(); break;
                case 5: func_9D06_b5(); break;
                case 6: func_9D06_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xC0DC:
            func_C0DC(); break;
        case 0xC0DE:
            func_C0DE(); break;
        case 0xF9C5:
            func_F9C5(); break;
        case 0xFE4B:
            func_FE4B(); break;
        case 0xDB64:
            func_DB64(); break;
        case 0xE842:
            func_E842(); break;
        case 0xEA07:
            func_EA07(); break;
        case 0xF61B:
            func_F61B(); break;
        case 0xC83E:
            func_C83E(); break;
        case 0xFFA2:
            func_FFA2(); break;
        case 0xC010:
            func_C010(); break;
        case 0xC03F:
            func_C03F(); break;
        case 0xDE71:
            func_DE71(); break;
        case 0xF518:
            func_F518(); break;
        case 0xDA43:
            func_DA43(); break;
        case 0xD221:
            func_D221(); break;
        case 0xE7EE:
            func_E7EE(); break;
        case 0xF04E:
            func_F04E(); break;
        case 0xEFDD:
            func_EFDD(); break;
        case 0xF006:
            func_F006(); break;
        case 0xFA57:
            func_FA57(); break;
        case 0xF458:
            func_F458(); break;
        case 0xC6D3:
            func_C6D3(); break;
        case 0xEFAB:
            func_EFAB(); break;
        case 0xE8E2:
            func_E8E2(); break;
        case 0xF8A8:
            func_F8A8(); break;
        case 0xCE10:
            func_CE10(); break;
        case 0xECA3:
            func_ECA3(); break;
        case 0xCACB:
            func_CACB(); break;
        case 0xC416:
            func_C416(); break;
        case 0xF7A3:
            func_F7A3(); break;
        case 0xF34C:
            func_F34C(); break;
        case 0xE66C:
            func_E66C(); break;
        case 0xE627:
            func_E627(); break;
        case 0xF2DC:
            func_F2DC(); break;
        case 0xF305:
            func_F305(); break;
        case 0xC0F3:
            func_C0F3(); break;
        case 0xE5FB:
            func_E5FB(); break;
        case 0xE51C:
            func_E51C(); break;
        case 0xC386:
            func_C386(); break;
        case 0xD01D:
            func_D01D(); break;
        case 0xE983:
            func_E983(); break;
        case 0xEAC6:
            func_EAC6(); break;
        case 0xE530:
            func_E530(); break;
        case 0xDEC6:
            func_DEC6(); break;
        case 0xD77A:
            func_D77A(); break;
        case 0xF5F3:
            func_F5F3(); break;
        case 0xF2D9:
            func_F2D9(); break;
        case 0xD975:
            func_D975(); break;
        case 0xDC76:
            func_DC76(); break;
        case 0xCB9B:
            func_CB9B(); break;
        case 0xF027:
            func_F027(); break;
        case 0xDE83:
            func_DE83(); break;
        case 0xC289:
            func_C289(); break;
        case 0xD5B3:
            func_D5B3(); break;
        case 0xEE86:
            func_EE86(); break;
        case 0xD29E:
            func_D29E(); break;
        case 0xCAC5:
            func_CAC5(); break;
        case 0xC36F:
            func_C36F(); break;
        case 0xC286:
            func_C286(); break;
        case 0xC288:
            func_C288(); break;
        case 0xC339:
            func_C339(); break;
        case 0xC33D:
            func_C33D(); break;
        case 0x9586:
            func_9586_b0(); break;
        case 0x9D96:
            func_9D96_b0(); break;
        case 0x8F21:
            func_8F21_b0(); break;
        case 0x90DE:
            func_90DE_b0(); break;
        case 0x913B:
            func_913B_b0(); break;
        case 0x92F8:
            func_92F8_b0(); break;
        case 0x8E25:
            switch (g_current_bank) {
                case 0: func_8E25_b0(); break;
                case 5: func_8E25_b5(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x939E:
            func_939E_b0(); break;
        case 0x93EE:
            func_93EE_b0(); break;
        case 0x9A2F:
            switch (g_current_bank) {
                case 0: func_9A2F_b0(); break;
                case 4: func_9A2F_b4(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x9C2F:
            func_9C2F_b0(); break;
        case 0x904B:
            func_904B_b0(); break;
        case 0x8D99:
            switch (g_current_bank) {
                case 0: func_8D99_b0(); break;
                case 2: func_8D99_b2(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x985C:
            func_985C_b0(); break;
        case 0x8E19:
            switch (g_current_bank) {
                case 0: func_8E19_b0(); break;
                case 5: func_8E19_b5(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x9008:
            switch (g_current_bank) {
                case 7: func_D008(); break;
                case 0: func_9008_b0(); break;
                case 4: func_9008_b4(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x9EF9:
            func_9EF9_b0(); break;
        case 0x9E90:
            func_9E90_b0(); break;
        case 0x9A56:
            func_9A56_b0(); break;
        case 0x8E07:
            func_8E07_b0(); break;
        case 0x8D9C:
            func_8D9C_b0(); break;
        case 0x9561:
            func_9561_b0(); break;
        case 0x9E9E:
            func_9E9E_b0(); break;
        case 0x8E61:
            func_8E61_b0(); break;
        case 0x8D60:
            func_8D60_b0(); break;
        case 0x8D8E:
            func_8D8E_b0(); break;
        case 0x8D64:
            func_8D64_b0(); break;
        case 0x8D68:
            func_8D68_b0(); break;
        case 0x9FD2:
            func_9FD2_b0(); break;
        case 0x9302:
            func_9302_b0(); break;
        case 0x9F93:
            func_9F93_b0(); break;
        case 0x8DE5:
            switch (g_current_bank) {
                case 0: func_8DE5_b0(); break;
                case 2: func_8DE5_b2(); break;
                case 6: func_8DE5_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xA181:
            func_A181_b1(); break;
        case 0x9C84:
            func_9C84_b1(); break;
        case 0xA20F:
            func_A20F_b1(); break;
        case 0x8E12:
            func_8E12_b1(); break;
        case 0x8A16:
            func_8A16_b1(); break;
        case 0xA912:
            switch (g_current_bank) {
                case 1: func_A912_b1(); break;
                case 5: func_A912_b5(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xA5EB:
            func_A5EB_b1(); break;
        case 0xA100:
            switch (g_current_bank) {
                case 1: func_A100_b1(); break;
                case 2: func_A100_b2(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x85CB:
            func_85CB_b1(); break;
        case 0xA205:
            func_A205_b1(); break;
        case 0xA972:
            func_A972_b1(); break;
        case 0xADA6:
            func_ADA6_b1(); break;
        case 0xA219:
            func_A219_b1(); break;
        case 0x8E01:
            func_8E01_b1(); break;
        case 0x8E05:
            switch (g_current_bank) {
                case 1: func_8E05_b1(); break;
                case 4: func_8E05_b4(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xAD25:
            func_AD25_b1(); break;
        case 0xA203:
            func_A203_b1(); break;
        case 0xAC05:
            func_AC05_b1(); break;
        case 0xA504:
            func_A504_b1(); break;
        case 0x8503:
            switch (g_current_bank) {
                case 1: func_8503_b1(); break;
                case 5: func_8503_b5(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x86CB:
            func_86CB_b1(); break;
        case 0xA02A:
            switch (g_current_bank) {
                case 1: func_A02A_b1(); break;
                case 2: func_A02A_b2(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xB105:
            func_B105_b1(); break;
        case 0xB04B:
            func_B04B_b1(); break;
        case 0xAD0F:
            func_AD0F_b1(); break;
        case 0xA014:
            func_A014_b1(); break;
        case 0xA974:
            func_A974_b1(); break;
        case 0xA191:
            func_A191_b1(); break;
        case 0x9905:
            switch (g_current_bank) {
                case 7: func_D905(); break;
                case 1: func_9905_b1(); break;
                case 4: func_9905_b4(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xAD06:
            func_AD06_b1(); break;
        case 0xAD0A:
            func_AD0A_b1(); break;
        case 0xAE8A:
            func_AE8A_b1(); break;
        case 0xADE6:
            func_ADE6_b1(); break;
        case 0xA93D:
            func_A93D_b1(); break;
        case 0x8A81:
            func_8A81_b1(); break;
        case 0x8D25:
            func_8D25_b1(); break;
        case 0xA909:
            func_A909_b1(); break;
        case 0xA58C:
            func_A58C_b1(); break;
        case 0xA98B:
            func_A98B_b1(); break;
        case 0xAD8C:
            func_AD8C_b1(); break;
        case 0xA578:
            func_A578_b1(); break;
        case 0x8A02:
            func_8A02_b1(); break;
        case 0xA914:
            switch (g_current_bank) {
                case 1: func_A914_b1(); break;
                case 2: func_A914_b2(); break;
                case 5: func_A914_b5(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xB031:
            func_B031_b1(); break;
        case 0x8D0C:
            func_8D0C_b1(); break;
        case 0xACBA:
            switch (g_current_bank) {
                case 1: func_ACBA_b1(); break;
                case 4: func_ACBA_b4(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xAC9A:
            func_AC9A_b1(); break;
        case 0xB461:
            func_B461_b1(); break;
        case 0x8D44:
            func_8D44_b1(); break;
        case 0x8041:
            switch (g_current_bank) {
                case 1: func_8041_b1(); break;
                case 3: func_8041_b3(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x96DF:
            func_96DF_b1(); break;
        case 0xAF09:
            func_AF09_b1(); break;
        case 0xAF58:
            func_AF58_b1(); break;
        case 0x8EA7:
            func_8EA7_b1(); break;
        case 0x8301:
            func_8301_b1(); break;
        case 0x80E1:
            func_80E1_b1(); break;
        case 0xB041:
            func_B041_b1(); break;
        case 0xB0CC:
            func_B0CC_b1(); break;
        case 0xB4FD:
            func_B4FD_b1(); break;
        case 0x9E41:
            func_9E41_b1(); break;
        case 0xAB01:
            func_AB01_b1(); break;
        case 0xB1B2:
            func_B1B2_b1(); break;
        case 0xB0B2:
            func_B0B2_b1(); break;
        case 0xBF5F:
            switch (g_current_bank) {
                case 1: func_BF5F_b1(); break;
                case 2: func_BF5F_b2(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xB2B3:
            func_B2B3_b1(); break;
        case 0xA19A:
            func_A19A_b1(); break;
        case 0xA57B:
            func_A57B_b1(); break;
        case 0xA926:
            func_A926_b1(); break;
        case 0x85D4:
            func_85D4_b1(); break;
        case 0x8DF9:
            func_8DF9_b1(); break;
        case 0xADA1:
            func_ADA1_b1(); break;
        case 0x9504:
            switch (g_current_bank) {
                case 1: func_9504_b1(); break;
                case 5: func_9504_b5(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x8921:
            func_8921_b1(); break;
        case 0xA5FB:
            switch (g_current_bank) {
                case 7: func_E5FB(); break;
                case 1: func_A5FB_b1(); break;
                case 4: func_A5FB_b4(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x8D78:
            func_8D78_b1(); break;
        case 0x8DAE:
            func_8DAE_b1(); break;
        case 0xAD5E:
            func_AD5E_b1(); break;
        case 0xACB6:
            func_ACB6_b1(); break;
        case 0xA520:
            func_A520_b1(); break;
        case 0xA530:
            func_A530_b1(); break;
        case 0xA929:
            func_A929_b1(); break;
        case 0xAEF7:
            func_AEF7_b1(); break;
        case 0x8585:
            switch (g_current_bank) {
                case 1: func_8585_b1(); break;
                case 6: func_8585_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xA804:
            func_A804_b1(); break;
        case 0x95A3:
            func_95A3_b1(); break;
        case 0xA9E0:
            func_A9E0_b1(); break;
        case 0x8536:
            func_8536_b1(); break;
        case 0x854A:
            func_854A_b1(); break;
        case 0xB0EA:
            func_B0EA_b1(); break;
        case 0x9D04:
            switch (g_current_bank) {
                case 1: func_9D04_b1(); break;
                case 5: func_9D04_b5(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x84B5:
            switch (g_current_bank) {
                case 1: func_84B5_b1(); break;
                case 4: func_84B5_b4(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xB621:
            func_B621_b1(); break;
        case 0xAC21:
            func_AC21_b1(); break;
        case 0xADD3:
            func_ADD3_b1(); break;
        case 0x8525:
            func_8525_b1(); break;
        case 0x85DE:
            func_85DE_b1(); break;
        case 0xA500:
            switch (g_current_bank) {
                case 1: func_A500_b1(); break;
                case 5: func_A500_b5(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xA9F8:
            func_A9F8_b1(); break;
        case 0x8D6F:
            func_8D6F_b1(); break;
        case 0x8ECA:
            func_8ECA_b1(); break;
        case 0xA916:
            func_A916_b1(); break;
        case 0x80AA:
            switch (g_current_bank) {
                case 7: func_C0AA(); break;
                case 1: func_80AA_b1(); break;
                case 5: func_80AA_b5(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x8518:
            func_8518_b1(); break;
        case 0xA849:
            func_A849_b1(); break;
        case 0x8461:
            func_8461_b1(); break;
        case 0x80DF:
            func_80DF_b1(); break;
        case 0xA7BA:
            func_A7BA_b1(); break;
        case 0xAC6E:
            func_AC6E_b1(); break;
        case 0x9D6C:
            func_9D6C_b1(); break;
        case 0xA969:
            switch (g_current_bank) {
                case 1: func_A969_b1(); break;
                case 4: func_A969_b4(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xAC4D:
            func_AC4D_b1(); break;
        case 0x9821:
            func_9821_b1(); break;
        case 0xB0F1:
            switch (g_current_bank) {
                case 7: func_F0F1(); break;
                case 1: func_B0F1_b1(); break;
                case 4: func_B0F1_b4(); break;
                case 5: func_B0F1_b5(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xA574:
            func_A574_b1(); break;
        case 0xAC17:
            func_AC17_b1(); break;
        case 0xA9DA:
            func_A9DA_b1(); break;
        case 0x807F:
            func_807F_b1(); break;
        case 0x9B9B:
            func_9B9B_b1(); break;
        case 0xB4B3:
            func_B4B3_b1(); break;
        case 0xACA5:
            func_ACA5_b1(); break;
        case 0x90AD:
            switch (g_current_bank) {
                case 7: func_D0AD(); break;
                case 1: func_90AD_b1(); break;
                case 5: func_90AD_b5(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xB4B1:
            switch (g_current_bank) {
                case 1: func_B4B1_b1(); break;
                case 5: func_B4B1_b5(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xB2B1:
            func_B2B1_b1(); break;
        case 0xA2A1:
            func_A2A1_b1(); break;
        case 0xB49D:
            func_B49D_b1(); break;
        case 0xA09D:
            func_A09D_b1(); break;
        case 0xA8A5:
            func_A8A5_b1(); break;
        case 0xB06D:
            func_B06D_b1(); break;
        case 0xA8F9:
            func_A8F9_b1(); break;
        case 0x840E:
            switch (g_current_bank) {
                case 1: func_840E_b1(); break;
                case 4: func_840E_b4(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xAC09:
            func_AC09_b1(); break;
        case 0xA80E:
            func_A80E_b1(); break;
        case 0xB063:
            func_B063_b1(); break;
        case 0xA878:
            switch (g_current_bank) {
                case 1: func_A878_b1(); break;
                case 4: func_A878_b4(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x9D08:
            func_9D08_b1(); break;
        case 0x8A0A:
            func_8A0A_b1(); break;
        case 0x840C:
            func_840C_b1(); break;
        case 0xA97D:
            func_A97D_b1(); break;
        case 0x802A:
            switch (g_current_bank) {
                case 1: func_802A_b1(); break;
                case 2: func_802A_b2(); break;
                case 6: func_802A_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x9541:
            func_9541_b1(); break;
        case 0xAD9B:
            func_AD9B_b1(); break;
        case 0x8500:
            switch (g_current_bank) {
                case 1: func_8500_b1(); break;
                case 2: func_8500_b2(); break;
                case 6: func_8500_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x8007:
            switch (g_current_bank) {
                case 2: func_8007_b2(); break;
                case 3: func_8007_b3(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x855B:
            switch (g_current_bank) {
                case 2: func_855B_b2(); break;
                case 5: func_855B_b5(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xA503:
            func_A503_b2(); break;
        case 0xA5DE:
            switch (g_current_bank) {
                case 2: func_A5DE_b2(); break;
                case 5: func_A5DE_b5(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x9E0F:
            func_9E0F_b2(); break;
        case 0x90F9:
            func_90F9_b2(); break;
        case 0x98E1:
            func_98E1_b2(); break;
        case 0x8687:
            switch (g_current_bank) {
                case 2: func_8687_b2(); break;
                case 5: func_8687_b5(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x8387:
            func_8387_b2(); break;
        case 0x803B:
            func_803B_b2(); break;
        case 0x9F9F:
            func_9F9F_b2(); break;
        case 0x9A96:
            func_9A96_b2(); break;
        case 0xA9E6:
            func_A9E6_b2(); break;
        case 0x9907:
            func_9907_b2(); break;
        case 0xAD0B:
            func_AD0B_b2(); break;
        case 0x92BD:
            func_92BD_b2(); break;
        case 0x92D2:
            func_92D2_b2(); break;
        case 0x9480:
            func_9480_b2(); break;
        case 0x937D:
            func_937D_b2(); break;
        case 0x9392:
            func_9392_b2(); break;
        case 0x93E9:
            func_93E9_b2(); break;
        case 0x9476:
            func_9476_b2(); break;
        case 0x8E24:
            switch (g_current_bank) {
                case 2: func_8E24_b2(); break;
                case 6: func_8E24_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x94EF:
            func_94EF_b2(); break;
        case 0x9D05:
            func_9D05_b2(); break;
        case 0xAC96:
            func_AC96_b2(); break;
        case 0x9550:
            func_9550_b2(); break;
        case 0x8511:
            func_8511_b2(); break;
        case 0xA9F9:
            switch (g_current_bank) {
                case 2: func_A9F9_b2(); break;
                case 5: func_A9F9_b5(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xAD1E:
            func_AD1E_b2(); break;
        case 0x915F:
            func_915F_b2(); break;
        case 0xA0AB:
            func_A0AB_b2(); break;
        case 0x94AF:
            func_94AF_b2(); break;
        case 0xA904:
            switch (g_current_bank) {
                case 2: func_A904_b2(); break;
                case 4: func_A904_b4(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xAD1A:
            func_AD1A_b2(); break;
        case 0xA998:
            func_A998_b2(); break;
        case 0x84B6:
            func_84B6_b2(); break;
        case 0x8496:
            func_8496_b2(); break;
        case 0xAD85:
            func_AD85_b2(); break;
        case 0x9269:
            func_9269_b2(); break;
        case 0x9501:
            switch (g_current_bank) {
                case 2: func_9501_b2(); break;
                case 5: func_9501_b5(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x9257:
            func_9257_b2(); break;
        case 0x9569:
            func_9569_b2(); break;
        case 0xAA03:
            func_AA03_b2(); break;
        case 0xA00D:
            func_A00D_b2(); break;
        case 0xA902:
            switch (g_current_bank) {
                case 2: func_A902_b2(); break;
                case 5: func_A902_b5(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x9803:
            func_9803_b2(); break;
        case 0x9991:
            func_9991_b2(); break;
        case 0xA4A3:
            func_A4A3_b2(); break;
        case 0xADE1:
            func_ADE1_b2(); break;
        case 0xA906:
            switch (g_current_bank) {
                case 2: func_A906_b2(); break;
                case 5: func_A906_b5(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x9DB3:
            func_9DB3_b2(); break;
        case 0x98B0:
            func_98B0_b2(); break;
        case 0xA004:
            func_A004_b2(); break;
        case 0x99A9:
            switch (g_current_bank) {
                case 2: func_99A9_b2(); break;
                case 4: func_99A9_b4(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x9B69:
            func_9B69_b2(); break;
        case 0x900F:
            func_900F_b2(); break;
        case 0x936E:
            func_936E_b2(); break;
        case 0x9169:
            func_9169_b2(); break;
        case 0x946E:
            func_946E_b2(); break;
        case 0x906D:
            func_906D_b2(); break;
        case 0xA217:
            func_A217_b2(); break;
        case 0x9D5F:
            func_9D5F_b2(); break;
        case 0x9D71:
            func_9D71_b2(); break;
        case 0xA908:
            func_A908_b2(); break;
        case 0xA80B:
            func_A80B_b2(); break;
        case 0x9105:
            func_9105_b2(); break;
        case 0x9109:
            func_9109_b2(); break;
        case 0x9EEC:
            func_9EEC_b2(); break;
        case 0x9006:
            func_9006_b2(); break;
        case 0xA9AB:
            func_A9AB_b2(); break;
        case 0x9101:
            switch (g_current_bank) {
                case 7: func_D101(); break;
                case 2: func_9101_b2(); break;
                case 5: func_9101_b5(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xA407:
            switch (g_current_bank) {
                case 2: func_A407_b2(); break;
                case 5: func_A407_b5(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x9E09:
            func_9E09_b2(); break;
        case 0x9125:
            func_9125_b2(); break;
        case 0xA505:
            switch (g_current_bank) {
                case 2: func_A505_b2(); break;
                case 5: func_A505_b5(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x85A6:
            func_85A6_b2(); break;
        case 0x8D9F:
            func_8D9F_b2(); break;
        case 0xAE04:
            func_AE04_b2(); break;
        case 0x8588:
            func_8588_b2(); break;
        case 0xA94A:
            func_A94A_b2(); break;
        case 0xA514:
            switch (g_current_bank) {
                case 2: func_A514_b2(); break;
                case 5: func_A514_b5(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x9107:
            func_9107_b2(); break;
        case 0x910B:
            func_910B_b2(); break;
        case 0xA5AA:
            func_A5AA_b2(); break;
        case 0xA9C2:
            func_A9C2_b2(); break;
        case 0x91C9:
            switch (g_current_bank) {
                case 2: func_91C9_b2(); break;
                case 5: func_91C9_b5(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xA829:
            func_A829_b2(); break;
        case 0x900D:
            func_900D_b2(); break;
        case 0x8515:
            func_8515_b2(); break;
        case 0xAD17:
            func_AD17_b2(); break;
        case 0xA40C:
            switch (g_current_bank) {
                case 2: func_A40C_b2(); break;
                case 4: func_A40C_b4(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xAC86:
            func_AC86_b2(); break;
        case 0xA807:
            func_A807_b2(); break;
        case 0x9103:
            switch (g_current_bank) {
                case 2: func_9103_b2(); break;
                case 5: func_9103_b5(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xA9EC:
            func_A9EC_b2(); break;
        case 0xAC1E:
            func_AC1E_b2(); break;
        case 0x9904:
            switch (g_current_bank) {
                case 7: func_D904(); break;
                case 2: func_9904_b2(); break;
                case 6: func_9904_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xA4F9:
            func_A4F9_b2(); break;
        case 0x910D:
            func_910D_b2(); break;
        case 0x910F:
            func_910F_b2(); break;
        case 0xA461:
            func_A461_b2(); break;
        case 0x91C1:
            func_91C1_b2(); break;
        case 0xA449:
            func_A449_b2(); break;
        case 0xA93B:
            func_A93B_b2(); break;
        case 0xA4AA:
            func_A4AA_b2(); break;
        case 0xADF8:
            func_ADF8_b2(); break;
        case 0xAC02:
            func_AC02_b2(); break;
        case 0xA01D:
            func_A01D_b2(); break;
        case 0xA9AE:
            func_A9AE_b2(); break;
        case 0x9041:
            func_9041_b2(); break;
        case 0x856B:
            func_856B_b2(); break;
        case 0xA423:
            func_A423_b2(); break;
        case 0xAACF:
            func_AACF_b2(); break;
        case 0x99AB:
            func_99AB_b2(); break;
        case 0xAD01:
            func_AD01_b2(); break;
        case 0xADC9:
            func_ADC9_b2(); break;
        case 0x9011:
            switch (g_current_bank) {
                case 2: func_9011_b2(); break;
                case 4: func_9011_b4(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x8491:
            func_8491_b2(); break;
        case 0xADAE:
            func_ADAE_b2(); break;
        case 0xA013:
            func_A013_b2(); break;
        case 0x90FD:
            func_90FD_b2(); break;
        case 0xAC2F:
            func_AC2F_b2(); break;
        case 0xACF6:
            func_ACF6_b2(); break;
        case 0xAC06:
            func_AC06_b2(); break;
        case 0xA0AE:
            func_A0AE_b2(); break;
        case 0xAF55:
            func_AF55_b2(); break;
        case 0x85B0:
            func_85B0_b2(); break;
        case 0xA221:
            func_A221_b2(); break;
        case 0x9B99:
            func_9B99_b3(); break;
        case 0x9FDC:
            func_9FDC_b3(); break;
        case 0xA581:
            func_A581_b3(); break;
        case 0xA561:
            switch (g_current_bank) {
                case 3: func_A561_b3(); break;
                case 4: func_A561_b4(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x8001:
            func_8001_b3(); break;
        case 0x8031:
            func_8031_b3(); break;
        case 0xA501:
            switch (g_current_bank) {
                case 3: func_A501_b3(); break;
                case 6: func_A501_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x9F81:
            func_9F81_b3(); break;
        case 0x80A0:
            func_80A0_b3(); break;
        case 0x8071:
            switch (g_current_bank) {
                case 3: func_8071_b3(); break;
                case 4: func_8071_b4(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x8F70:
            func_8F70_b3(); break;
        case 0x81EF:
            func_81EF_b3(); break;
        case 0x92B3:
            switch (g_current_bank) {
                case 3: func_92B3_b3(); break;
                case 6: func_92B3_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x87C4:
            func_87C4_b3(); break;
        case 0x85F6:
            func_85F6_b3(); break;
        case 0xA029:
            func_A029_b3(); break;
        case 0x8203:
            func_8203_b3(); break;
        case 0x9F41:
            func_9F41_b3(); break;
        case 0xBF9F:
            func_BF9F_b3(); break;
        case 0x9CDF:
            func_9CDF_b3(); break;
        case 0xA891:
            switch (g_current_bank) {
                case 3: func_A891_b3(); break;
                case 5: func_A891_b5(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x8184:
            func_8184_b3(); break;
        case 0xA58F:
            func_A58F_b3(); break;
        case 0x80B3:
            func_80B3_b3(); break;
        case 0x8C0C:
            func_8C0C_b3(); break;
        case 0xA0D9:
            func_A0D9_b3(); break;
        case 0x88DB:
            func_88DB_b3(); break;
        case 0x8E3F:
            func_8E3F_b3(); break;
        case 0xA83F:
            func_A83F_b3(); break;
        case 0x8F40:
            func_8F40_b3(); break;
        case 0x90E1:
            func_90E1_b3(); break;
        case 0x80C9:
            switch (g_current_bank) {
                case 3: func_80C9_b3(); break;
                case 5: func_80C9_b5(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x8049:
            func_8049_b3(); break;
        case 0x86D0:
            func_86D0_b3(); break;
        case 0x8005:
            switch (g_current_bank) {
                case 3: func_8005_b3(); break;
                case 5: func_8005_b5(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x8011:
            func_8011_b3(); break;
        case 0x90C1:
            func_90C1_b3(); break;
        case 0x8781:
            func_8781_b3(); break;
        case 0x8DC1:
            switch (g_current_bank) {
                case 3: func_8DC1_b3(); break;
                case 6: func_8DC1_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xA9C0:
            func_A9C0_b3(); break;
        case 0xB071:
            func_B071_b4(); break;
        case 0x8802:
            func_8802_b4(); break;
        case 0x9402:
            func_9402_b4(); break;
        case 0xA099:
            func_A099_b4(); break;
        case 0x84F6:
            func_84F6_b4(); break;
        case 0x8821:
            func_8821_b4(); break;
        case 0xA0AA:
            func_A0AA_b4(); break;
        case 0x9085:
            func_9085_b4(); break;
        case 0xA502:
            switch (g_current_bank) {
                case 4: func_A502_b4(); break;
                case 5: func_A502_b5(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x94C9:
            func_94C9_b4(); break;
        case 0x9D19:
            func_9D19_b4(); break;
        case 0x904D:
            func_904D_b4(); break;
        case 0x9D59:
            func_9D59_b4(); break;
        case 0xA901:
            func_A901_b4(); break;
        case 0xA471:
            func_A471_b4(); break;
        case 0xA9C9:
            func_A9C9_b4(); break;
        case 0xB052:
            func_B052_b4(); break;
        case 0x90B2:
            func_90B2_b4(); break;
        case 0x9540:
            func_9540_b4(); break;
        case 0x84A6:
            func_84A6_b4(); break;
        case 0xB012:
            func_B012_b4(); break;
        case 0x951B:
            func_951B_b4(); break;
        case 0xACF7:
            func_ACF7_b4(); break;
        case 0x9321:
            func_9321_b4(); break;
        case 0x8D05:
            switch (g_current_bank) {
                case 4: func_8D05_b4(); break;
                case 5: func_8D05_b5(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x8402:
            func_8402_b4(); break;
        case 0x8579:
            func_8579_b4(); break;
        case 0xA5AB:
            func_A5AB_b4(); break;
        case 0x80BE:
            func_80BE_b4(); break;
        case 0xA50A:
            func_A50A_b4(); break;
        case 0x9D30:
            func_9D30_b4(); break;
        case 0x9D0B:
            func_9D0B_b4(); break;
        case 0x90BA:
            func_90BA_b4(); break;
        case 0xAD84:
            func_AD84_b4(); break;
        case 0x9D41:
            func_9D41_b4(); break;
        case 0x95B3:
            func_95B3_b4(); break;
        case 0x8D2D:
            func_8D2D_b4(); break;
        case 0xA9FB:
            func_A9FB_b4(); break;
        case 0xA0B3:
            func_A0B3_b4(); break;
        case 0x9D07:
            func_9D07_b4(); break;
        case 0x989A:
            func_989A_b4(); break;
        case 0x9826:
            func_9826_b4(); break;
        case 0x9505:
            func_9505_b4(); break;
        case 0x9D21:
            func_9D21_b4(); break;
        case 0xB006:
            func_B006_b4(); break;
        case 0x8683:
            func_8683_b4(); break;
        case 0x8805:
            func_8805_b4(); break;
        case 0xB069:
            func_B069_b4(); break;
        case 0x8637:
            func_8637_b4(); break;
        case 0x8A0C:
            func_8A0C_b4(); break;
        case 0x8AA9:
            func_8AA9_b4(); break;
        case 0xA60E:
            func_A60E_b4(); break;
        case 0x9051:
            func_9051_b4(); break;
        case 0xA907:
            func_A907_b4(); break;
        case 0x9579:
            func_9579_b4(); break;
        case 0xA971:
            func_A971_b4(); break;
        case 0x957E:
            func_957E_b4(); break;
        case 0xA90A:
            func_A90A_b4(); break;
        case 0xA983:
            switch (g_current_bank) {
                case 7: func_E983(); break;
                case 4: func_A983_b4(); break;
                case 5: func_A983_b5(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xA53F:
            switch (g_current_bank) {
                case 4: func_A53F_b4(); break;
                case 5: func_A53F_b5(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x9D69:
            func_9D69_b4(); break;
        case 0xAE85:
            func_AE85_b4(); break;
        case 0xA089:
            func_A089_b4(); break;
        case 0xB411:
            func_B411_b4(); break;
        case 0x8A5A:
            func_8A5A_b4(); break;
        case 0x9D4D:
            func_9D4D_b4(); break;
        case 0xAD3A:
            func_AD3A_b4(); break;
        case 0x95F9:
            func_95F9_b4(); break;
        case 0xA00B:
            func_A00B_b4(); break;
        case 0xA90D:
            func_A90D_b4(); break;
        case 0xA74D:
            func_A74D_b4(); break;
        case 0xA507:
            func_A507_b4(); break;
        case 0xAB86:
            func_AB86_b4(); break;
        case 0xB421:
            switch (g_current_bank) {
                case 7: func_F421(); break;
                case 4: func_B421_b4(); break;
                case 5: func_B421_b5(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x92BE:
            func_92BE_b4(); break;
        case 0x9D5E:
            func_9D5E_b4(); break;
        case 0x8551:
            func_8551_b4(); break;
        case 0xA0AD:
            func_A0AD_b4(); break;
        case 0xAE8E:
            func_AE8E_b4(); break;
        case 0x96AE:
            func_96AE_b4(); break;
        case 0x8DE4:
            func_8DE4_b4(); break;
        case 0xA9B5:
            func_A9B5_b4(); break;
        case 0xA5AD:
            switch (g_current_bank) {
                case 7: func_E5AD(); break;
                case 4: func_A5AD_b4(); break;
                case 5: func_A5AD_b5(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x8495:
            func_8495_b4(); break;
        case 0x9507:
            func_9507_b4(); break;
        case 0xAD91:
            func_AD91_b4(); break;
        case 0xA592:
            func_A592_b4(); break;
        case 0xA0BA:
            func_A0BA_b4(); break;
        case 0x9514:
            func_9514_b4(); break;
        case 0xABBA:
            func_ABBA_b4(); break;
        case 0xA411:
            func_A411_b4(); break;
        case 0xB019:
            func_B019_b4(); break;
        case 0x9921:
            func_9921_b4(); break;
        case 0xAD49:
            func_AD49_b4(); break;
        case 0xA9CD:
            func_A9CD_b4(); break;
        case 0x939A:
            func_939A_b4(); break;
        case 0x9903:
            func_9903_b4(); break;
        case 0x9942:
            func_9942_b4(); break;
        case 0x9D86:
            func_9D86_b4(); break;
        case 0x8DA3:
            func_8DA3_b4(); break;
        case 0x8D02:
            func_8D02_b4(); break;
        case 0xB29E:
            func_B29E_b4(); break;
        case 0xA007:
            func_A007_b4(); break;
        case 0x9A21:
            func_9A21_b4(); break;
        case 0x995E:
            func_995E_b4(); break;
        case 0x819A:
            func_819A_b4(); break;
        case 0xA949:
            switch (g_current_bank) {
                case 4: func_A949_b4(); break;
                case 5: func_A949_b5(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x88AA:
            func_88AA_b4(); break;
        case 0xB404:
            func_B404_b4(); break;
        case 0xAC49:
            func_AC49_b4(); break;
        case 0x844D:
            func_844D_b4(); break;
        case 0xA84B:
            func_A84B_b4(); break;
        case 0x9A31:
            func_9A31_b4(); break;
        case 0xA450:
            func_A450_b4(); break;
        case 0x8EBA:
            func_8EBA_b4(); break;
        case 0xA933:
            func_A933_b4(); break;
        case 0x9DAD:
            func_9DAD_b4(); break;
        case 0x950F:
            func_950F_b4(); break;
        case 0x9DC1:
            func_9DC1_b4(); break;
        case 0x9721:
            func_9721_b4(); break;
        case 0xA0FF:
            func_A0FF_b4(); break;
        case 0xA871:
            func_A871_b4(); break;
        case 0x90A0:
            func_90A0_b4(); break;
        case 0x84C5:
            func_84C5_b4(); break;
        case 0x959F:
            func_959F_b4(); break;
        case 0x927A:
            func_927A_b4(); break;
        case 0x9F19:
            func_9F19_b4(); break;
        case 0x9FEA:
            func_9FEA_b4(); break;
        case 0x868E:
            func_868E_b4(); break;
        case 0x838E:
            func_838E_b4(); break;
        case 0xA917:
            func_A917_b4(); break;
        case 0xB101:
            func_B101_b4(); break;
        case 0xA5A7:
            func_A5A7_b4(); break;
        case 0xA873:
            func_A873_b4(); break;
        case 0x86B6:
            func_86B6_b4(); break;
        case 0x85D6:
            func_85D6_b4(); break;
        case 0x8695:
            func_8695_b4(); break;
        case 0xA2C4:
            func_A2C4_b4(); break;
        case 0x9CA7:
            func_9CA7_b4(); break;
        case 0xB0F3:
            func_B0F3_b4(); break;
        case 0xA0D3:
            func_A0D3_b4(); break;
        case 0xAA08:
            func_AA08_b4(); break;
        case 0x958A:
            func_958A_b4(); break;
        case 0x9D47:
            func_9D47_b4(); break;
        case 0x8986:
            func_8986_b4(); break;
        case 0xAC11:
            func_AC11_b4(); break;
        case 0xA2B6:
            func_A2B6_b4(); break;
        case 0xA8C0:
            func_A8C0_b4(); break;
        case 0x95A9:
            func_95A9_b4(); break;
        case 0xA9EF:
            func_A9EF_b4(); break;
        case 0x8D3A:
            func_8D3A_b4(); break;
        case 0x99FF:
            func_99FF_b4(); break;
        case 0x8D10:
            func_8D10_b4(); break;
        case 0xA9AC:
            switch (g_current_bank) {
                case 4: func_A9AC_b4(); break;
                case 5: func_A9AC_b5(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x8810:
            func_8810_b4(); break;
        case 0xAB65:
            func_AB65_b4(); break;
        case 0xABD0:
            func_ABD0_b4(); break;
        case 0x99AE:
            func_99AE_b4(); break;
        case 0x9A9A:
            func_9A9A_b4(); break;
        case 0x9D1E:
            func_9D1E_b4(); break;
        case 0x8549:
            func_8549_b4(); break;
        case 0x91BA:
            func_91BA_b4(); break;
        case 0x850A:
            func_850A_b4(); break;
        case 0x9DF1:
            func_9DF1_b4(); break;
        case 0x9809:
            func_9809_b4(); break;
        case 0x9DB0:
            func_9DB0_b4(); break;
        case 0xA2A9:
            func_A2A9_b4(); break;
        case 0xA0F9:
            func_A0F9_b4(); break;
        case 0xB023:
            switch (g_current_bank) {
                case 7: func_F023(); break;
                case 4: func_B023_b4(); break;
                case 5: func_B023_b5(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x8812:
            func_8812_b4(); break;
        case 0xA44B:
            func_A44B_b4(); break;
        case 0xB043:
            func_B043_b4(); break;
        case 0x9054:
            func_9054_b4(); break;
        case 0xADBA:
            func_ADBA_b4(); break;
        case 0x8D61:
            func_8D61_b4(); break;
        case 0xA9E7:
            func_A9E7_b5(); break;
        case 0xA9E2:
            func_A9E2_b5(); break;
        case 0xA9B3:
            func_A9B3_b5(); break;
        case 0xA582:
            func_A582_b5(); break;
        case 0xA545:
            func_A545_b5(); break;
        case 0x8D6A:
            func_8D6A_b5(); break;
        case 0xA58B:
            func_A58B_b5(); break;
        case 0xA976:
            func_A976_b5(); break;
        case 0xA5EA:
            func_A5EA_b5(); break;
        case 0xB0DE:
            func_B0DE_b5(); break;
        case 0xA6F1:
            func_A6F1_b5(); break;
        case 0x9E84:
            func_9E84_b5(); break;
        case 0x98A5:
            switch (g_current_bank) {
                case 5: func_98A5_b5(); break;
                case 4: func_98A5_b4(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xB005:
            func_B005_b5(); break;
        case 0xA911:
            func_A911_b5(); break;
        case 0xA50E:
            func_A50E_b5(); break;
        case 0xA4EC:
            func_A4EC_b5(); break;
        case 0xB221:
            func_B221_b5(); break;
        case 0x9021:
            func_9021_b5(); break;
        case 0x83AF:
            func_83AF_b5(); break;
        case 0xAA04:
            func_AA04_b5(); break;
        case 0x980A:
            func_980A_b5(); break;
        case 0xA9FE:
            func_A9FE_b5(); break;
        case 0x81A1:
            func_81A1_b5(); break;
        case 0xA006:
            func_A006_b5(); break;
        case 0xA586:
            func_A586_b5(); break;
        case 0xA58D:
            func_A58D_b5(); break;
        case 0x88FE:
            func_88FE_b5(); break;
        case 0x8560:
            func_8560_b5(); break;
        case 0x8D11:
            func_8D11_b5(); break;
        case 0x866A:
            func_866A_b5(); break;
        case 0xB556:
            func_B556_b5(); break;
        case 0xB576:
            func_B576_b5(); break;
        case 0x9556:
            func_9556_b5(); break;
        case 0x8987:
            func_8987_b5(); break;
        case 0x8F87:
            func_8F87_b5(); break;
        case 0xA387:
            func_A387_b5(); break;
        case 0xA887:
            func_A887_b5(); break;
        case 0x86A9:
            func_86A9_b5(); break;
        case 0xA456:
            func_A456_b5(); break;
        case 0xA5B1:
            func_A5B1_b5(); break;
        case 0xA42A:
            func_A42A_b5(); break;
        case 0x854E:
            func_854E_b5(); break;
        case 0x8599:
            func_8599_b5(); break;
        case 0x8540:
            func_8540_b5(); break;
        case 0xA508:
            func_A508_b5(); break;
        case 0xB786:
            func_B786_b5(); break;
        case 0xB102:
            func_B102_b5(); break;
        case 0x900A:
            func_900A_b5(); break;
        case 0x8CE0:
            func_8CE0_b5(); break;
        case 0xA51B:
            func_A51B_b5(); break;
        case 0x8D07:
            func_8D07_b5(); break;
        case 0x8C56:
            func_8C56_b5(); break;
        case 0x8AEA:
            func_8AEA_b5(); break;
        case 0x9EAE:
            func_9EAE_b5(); break;
        case 0x84E7:
            func_84E7_b5(); break;
        case 0x902F:
            func_902F_b5(); break;
        case 0xA98E:
            func_A98E_b5(); break;
        case 0x852D:
            func_852D_b5(); break;
        case 0x8D42:
            func_8D42_b5(); break;
        case 0xA9FC:
            func_A9FC_b5(); break;
        case 0xA607:
            func_A607_b5(); break;
        case 0xA805:
            func_A805_b5(); break;
        case 0xA528:
            func_A528_b5(); break;
        case 0x80DA:
            func_80DA_b5(); break;
        case 0xA5F9:
            func_A5F9_b5(); break;
        case 0xB572:
            func_B572_b5(); break;
        case 0xB2DA:
            func_B2DA_b5(); break;
        case 0x8DD4:
            func_8DD4_b5(); break;
        case 0xB54A:
            func_B54A_b5(); break;
        case 0xA533:
            func_A533_b5(); break;
        case 0xA499:
            func_A499_b5(); break;
        case 0xB536:
            func_B536_b5(); break;
        case 0x98C6:
            func_98C6_b5(); break;
        case 0xA649:
            func_A649_b5(); break;
        case 0x8873:
            func_8873_b5(); break;
        case 0x89B2:
            func_89B2_b5(); break;
        case 0x8814:
            func_8814_b5(); break;
        case 0xB108:
            func_B108_b5(); break;
        case 0x91F9:
            func_91F9_b5(); break;
        case 0x860F:
            func_860F_b5(); break;
        case 0x9092:
            func_9092_b5(); break;
        case 0x9179:
            func_9179_b5(); break;
        case 0xA405:
            switch (g_current_bank) {
                case 5: func_A405_b5(); break;
                case 2: func_A405_b2(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xA671:
            func_A671_b5(); break;
        case 0x8401:
            func_8401_b5(); break;
        case 0xA5E1:
            func_A5E1_b5(); break;
        case 0x9906:
            func_9906_b5(); break;
        case 0xA8A6:
            func_A8A6_b5(); break;
        case 0xA0C9:
            func_A0C9_b5(); break;
        case 0xA1E7:
            func_A1E7_b5(); break;
        case 0xA835:
            func_A835_b5(); break;
        case 0xA8B6:
            func_A8B6_b5(); break;
        case 0x8097:
            switch (g_current_bank) {
                case 5: func_8097_b5(); break;
                case 6: func_8097_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xB603:
            func_B603_b5(); break;
        case 0xB7D3:
            func_B7D3_b5(); break;
        case 0x83A1:
            func_83A1_b5(); break;
        case 0xB852:
            func_B852_b5(); break;
        case 0xA8B8:
            func_A8B8_b5(); break;
        case 0xB7B6:
            func_B7B6_b5(); break;
        case 0x913A:
            func_913A_b5(); break;
        case 0x9159:
            func_9159_b5(); break;
        case 0x8653:
            func_8653_b5(); break;
        case 0x8768:
            func_8768_b5(); break;
        case 0x8548:
            func_8548_b5(); break;
        case 0x8A75:
            func_8A75_b5(); break;
        case 0x8888:
            func_8888_b5(); break;
        case 0x8287:
            func_8287_b5(); break;
        case 0x84AE:
            func_84AE_b5(); break;
        case 0x8181:
            func_8181_b5(); break;
        case 0x9D9D:
            func_9D9D_b5(); break;
        case 0x8DF6:
            func_8DF6_b5(); break;
        case 0x9695:
            switch (g_current_bank) {
                case 7: func_D695(); break;
                case 5: func_9695_b5(); break;
                case 6: func_9695_b6(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x8701:
            func_8701_b5(); break;
        case 0x8684:
            func_8684_b5(); break;
        case 0xB142:
            func_B142_b5(); break;
        case 0x8682:
            func_8682_b5(); break;
        case 0x8622:
            func_8622_b5(); break;
        case 0xA603:
            func_A603_b5(); break;
        case 0x8127:
            func_8127_b5(); break;
        case 0x8248:
            func_8248_b5(); break;
        case 0x8003:
            func_8003_b5(); break;
        case 0x8103:
            func_8103_b5(); break;
        case 0xB116:
            func_B116_b5(); break;
        case 0xA9A5:
            func_A9A5_b5(); break;
        case 0x8E21:
            func_8E21_b5(); break;
        case 0xB106:
            func_B106_b5(); break;
        case 0xB002:
            func_B002_b5(); break;
        case 0xAA49:
            func_AA49_b5(); break;
        case 0xA973:
            func_A973_b5(); break;
        case 0xB1AB:
            func_B1AB_b5(); break;
        case 0xA4E8:
            func_A4E8_b5(); break;
        case 0xA4DC:
            func_A4DC_b5(); break;
        case 0xA4E4:
            func_A4E4_b5(); break;
        case 0xA5F1:
            func_A5F1_b5(); break;
        case 0x9E00:
            func_9E00_b5(); break;
        case 0x988B:
            func_988B_b5(); break;
        case 0xA989:
            func_A989_b5(); break;
        case 0xA33E:
            func_A33E_b5(); break;
        case 0x98A8:
            func_98A8_b5(); break;
        case 0xA016:
            func_A016_b5(); break;
        case 0x9997:
            func_9997_b5(); break;
        case 0x9001:
            func_9001_b5(); break;
        case 0x9099:
            func_9099_b5(); break;
        case 0x848A:
            func_848A_b5(); break;
        case 0x9096:
            func_9096_b5(); break;
        case 0x9090:
            func_9090_b5(); break;
        case 0x9FA0:
            func_9FA0_b5(); break;
        case 0xA2D5:
            func_A2D5_b5(); break;
        case 0xAB47:
            func_AB47_b5(); break;
        case 0xA2EA:
            func_A2EA_b5(); break;
        case 0x9802:
            func_9802_b5(); break;
        case 0xB0AD:
            func_B0AD_b5(); break;
        case 0x8DEC:
            switch (g_current_bank) {
                case 5: func_8DEC_b5(); break;
                case 4: func_8DEC_b4(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0x89CE:
            func_89CE_b5(); break;
        case 0x8DB1:
            func_8DB1_b5(); break;
        case 0xA5EF:
            func_A5EF_b5(); break;
        case 0xA5C1:
            func_A5C1_b5(); break;
        case 0x914D:
            func_914D_b5(); break;
        case 0x8D29:
            func_8D29_b5(); break;
        case 0xA8B4:
            func_A8B4_b5(); break;
        case 0xA52D:
            func_A52D_b5(); break;
        case 0xA88E:
            func_A88E_b5(); break;
        case 0xB3D1:
            func_B3D1_b5(); break;
        case 0x94AD:
            func_94AD_b5(); break;
        case 0xA55C:
            func_A55C_b5(); break;
        case 0xA86C:
            func_A86C_b5(); break;
        case 0x84F0:
            func_84F0_b5(); break;
        case 0xA4B5:
            func_A4B5_b5(); break;
        case 0xA00C:
            func_A00C_b5(); break;
        case 0xA0F4:
            func_A0F4_b5(); break;
        case 0xB578:
            func_B578_b5(); break;
        case 0x9019:
            func_9019_b5(); break;
        case 0xB104:
            func_B104_b5(); break;
        case 0xB099:
            func_B099_b5(); break;
        case 0x9481:
            func_9481_b5(); break;
        case 0xB698:
            func_B698_b5(); break;
        case 0xA91D:
            func_A91D_b5(); break;
        case 0x9EAA:
            func_9EAA_b5(); break;
        case 0x8C04:
            func_8C04_b5(); break;
        case 0xB71E:
            func_B71E_b5(); break;
        case 0x8E02:
            func_8E02_b5(); break;
        case 0x880B:
            func_880B_b5(); break;
        case 0x8A01:
            func_8A01_b6(); break;
        case 0x9301:
            func_9301_b6(); break;
        case 0x93FD:
            func_93FD_b6(); break;
        case 0x94F9:
            func_94F9_b6(); break;
        case 0x95F5:
            func_95F5_b6(); break;
        case 0x96F1:
            func_96F1_b6(); break;
        case 0x97ED:
            func_97ED_b6(); break;
        case 0x98E9:
            func_98E9_b6(); break;
        case 0x99E5:
            func_99E5_b6(); break;
        case 0x9BDD:
            func_9BDD_b6(); break;
        case 0x9CD9:
            func_9CD9_b6(); break;
        case 0xA981:
            func_A981_b6(); break;
        case 0xA261:
            func_A261_b6(); break;
        case 0x85F1:
            func_85F1_b6(); break;
        case 0xA3BA:
            func_A3BA_b6(); break;
        case 0x83B6:
            func_83B6_b6(); break;
        case 0x886C:
            func_886C_b6(); break;
        case 0x8D73:
            func_8D73_b6(); break;
        case 0x8B75:
            func_8B75_b6(); break;
        case 0x897A:
            func_897A_b6(); break;
        case 0x9304:
            func_9304_b6(); break;
        case 0xA301:
            func_A301_b6(); break;
        case 0x8E03:
            func_8E03_b6(); break;
        case 0x8603:
            func_8603_b6(); break;
        case 0x8A5F:
            func_8A5F_b6(); break;
        case 0x8384:
            func_8384_b6(); break;
        case 0x8D48:
            func_8D48_b6(); break;
        case 0x9010:
            func_9010_b6(); break;
        case 0xA051:
            func_A051_b6(); break;
        case 0x8446:
            func_8446_b6(); break;
        case 0x8509:
            func_8509_b6(); break;
        case 0x894E:
            func_894E_b6(); break;
        case 0x86E3:
            func_86E3_b6(); break;
        case 0xA6A3:
            func_A6A3_b6(); break;
        case 0x921F:
            func_921F_b6(); break;
        case 0xA337:
            func_A337_b6(); break;
        case 0xA619:
            func_A619_b6(); break;
        case 0x8082:
            func_8082_b6(); break;
        case 0x9704:
            func_9704_b6(); break;
        case 0x991A:
            func_991A_b6(); break;
        case 0x8004:
            func_8004_b6(); break;
        case 0x9604:
            func_9604_b6(); break;
        case 0x9217:
            func_9217_b6(); break;
        case 0xA611:
            func_A611_b6(); break;
        case 0x96E7:
            func_96E7_b6(); break;
        case 0xA672:
            func_A672_b6(); break;
        case 0x8634:
            func_8634_b6(); break;
        case 0x87B4:
            func_87B4_b6(); break;
        case 0x8793:
            func_8793_b6(); break;
        case 0x85DC:
            func_85DC_b6(); break;
        case 0xA5DB:
            func_A5DB_b6(); break;
        case 0xA5CB:
            func_A5CB_b6(); break;
        case 0xA629:
            func_A629_b6(); break;
        case 0xA69F:
            func_A69F_b6(); break;
        case 0x9622:
            func_9622_b6(); break;
        case 0x8314:
            func_8314_b6(); break;
        case 0xA310:
            func_A310_b6(); break;
        case 0x9908:
            func_9908_b6(); break;
        case 0x830A:
            func_830A_b6(); break;
        case 0x8223:
            func_8223_b6(); break;
        case 0x82F7:
            func_82F7_b6(); break;
        case 0x8632:
            func_8632_b6(); break;
        case 0x8BA5:
            func_8BA5_b6(); break;
        case 0x952A:
            func_952A_b6(); break;
        case 0x9D25:
            func_9D25_b6(); break;
        case 0x9326:
            func_9326_b6(); break;
        case 0x9895:
            func_9895_b6(); break;
        case 0x9816:
            func_9816_b6(); break;
        case 0x8C97:
            func_8C97_b6(); break;
        case 0x831C:
            func_831C_b6(); break;
        case 0x801B:
            func_801B_b6(); break;
        case 0x8754:
            func_8754_b6(); break;
        case 0xA738:
            func_A738_b6(); break;
        case 0xA33A:
            func_A33A_b6(); break;
        case 0x9624:
            func_9624_b6(); break;
        case 0xA382:
            func_A382_b6(); break;
        case 0x889A:
            func_889A_b6(); break;
        case 0x9C81:
            func_9C81_b6(); break;
        case 0xA99F:
            func_A99F_b6(); break;
        case 0xA2B5:
            func_A2B5_b6(); break;
        case 0x8E69:
            func_8E69_b6(); break;
        case 0xA101:
            func_A101_b6(); break;
        case 0xA184:
            func_A184_b6(); break;
        case 0xA26A:
            func_A26A_b6(); break;
        case 0xA3C9:
            func_A3C9_b6(); break;
        case 0xA25E:
            func_A25E_b6(); break;
        case 0xA39D:
            func_A39D_b6(); break;
        case 0xA3B1:
            func_A3B1_b6(); break;
        case 0xA3BF:
            func_A3BF_b6(); break;
        case 0xA236:
            func_A236_b6(); break;
        case 0xA3F5:
            func_A3F5_b6(); break;
        case 0xA20B:
            func_A20B_b6(); break;
        case 0xA29F:
            func_A29F_b6(); break;
        case 0xA273:
            func_A273_b6(); break;
        case 0x85A1:
            func_85A1_b6(); break;
        case 0x8601:
            func_8601_b6(); break;
        case 0x9009:
            func_9009_b6(); break;
        case 0x9421:
            func_9421_b6(); break;
        case 0x8282:
            func_8282_b6(); break;
        case 0x8A8A:
            func_8A8A_b6(); break;
        case 0x8C25:
            func_8C25_b6(); break;
        case 0x9595:
            func_9595_b6(); break;
        case 0xA1A1:
            func_A1A1_b6(); break;
        case 0xA2A2:
            func_A2A2_b6(); break;
        case 0x9B00:
            func_9B00_b6(); break;
        case 0x9325:
            func_9325_b0(); break;
        case 0x8DDB:
            func_8DDB_b0(); break;
        case 0x8DE3:
            func_8DE3_b0(); break;
        case 0x9C7E:
            func_9C7E_b1(); break;
        case 0xB0FA:
            func_B0FA_b1(); break;
        case 0xAE5A:
            func_AE5A_b1(); break;
        case 0xAE60:
            func_AE60_b1(); break;
        case 0x8D12:
            func_8D12_b1(); break;
        case 0xAD86:
            func_AD86_b1(); break;
        case 0xA572:
            func_A572_b1(); break;
        case 0xAF06:
            func_AF06_b1(); break;
        case 0x9DC5:
            func_9DC5_b1(); break;
        case 0xBF5A:
            switch (g_current_bank) {
                case 1: func_BF5A_b1(); break;
                case 2: func_BF5A_b2(); break;
                default: nes_log_dispatch_miss(addr); break;
            }
            break;
        case 0xA506:
            func_A506_b1(); break;
        case 0xA7DB:
            func_A7DB_b1(); break;
        case 0xB0DF:
            func_B0DF_b1(); break;
        case 0xADC3:
            func_ADC3_b1(); break;
        case 0xAC28:
            func_AC28_b1(); break;
        case 0xA296:
            func_A296_b1(); break;
        case 0x9266:
            func_9266_b2(); break;
        case 0xA009:
            func_A009_b2(); break;
        case 0x983D:
            func_983D_b2(); break;
        case 0x985F:
            func_985F_b2(); break;
        case 0x9881:
            func_9881_b2(); break;
        case 0x98A3:
            func_98A3_b2(); break;
        case 0xA5A6:
            func_A5A6_b2(); break;
        case 0xA827:
            func_A827_b2(); break;
        case 0xABAA:
            func_ABAA_b2(); break;
        case 0xA453:
            func_A453_b2(); break;
        case 0xAA8A:
            func_AA8A_b2(); break;
        case 0xA050:
            func_A050_b2(); break;
        case 0xA54E:
            func_A54E_b3(); break;
        case 0x98FB:
            func_98FB_b3(); break;
        case 0x98CD:
            func_98CD_b3(); break;
        case 0x81F9:
            func_81F9_b3(); break;
        case 0x90BF:
            func_90BF_b3(); break;
        case 0x93E4:
            func_93E4_b4(); break;
        case 0xA4FB:
            func_A4FB_b4(); break;
        case 0x9D16:
            func_9D16_b4(); break;
        case 0x92EC:
            func_92EC_b4(); break;
        case 0xA5A0:
            func_A5A0_b4(); break;
        case 0x867B:
            func_867B_b4(); break;
        case 0x85BB:
            func_85BB_b4(); break;
        case 0xA535:
            func_A535_b4(); break;
        case 0xA95F:
            func_A95F_b4(); break;
        case 0xAB7B:
            func_AB7B_b4(); break;
        case 0x92A7:
            func_92A7_b4(); break;
        case 0x99EC:
            func_99EC_b4(); break;
        case 0x99D4:
            func_99D4_b4(); break;
        case 0x996F:
            func_996F_b4(); break;
        case 0xA5E3:
            func_A5E3_b5(); break;
        case 0xA90C:
            func_A90C_b5(); break;
        case 0x819C:
            func_819C_b5(); break;
        case 0x88F9:
            func_88F9_b5(); break;
        case 0x9531:
            func_9531_b5(); break;
        case 0x9533:
            func_9533_b5(); break;
        case 0xA3FE:
            func_A3FE_b5(); break;
        case 0x848B:
            func_848B_b5(); break;
        case 0xB2D7:
            func_B2D7_b5(); break;
        case 0xA634:
            func_A634_b5(); break;
        case 0x8864:
            func_8864_b5(); break;
        case 0x89A0:
            func_89A0_b5(); break;
        case 0xA93E:
            func_A93E_b5(); break;
        case 0x91BE:
            func_91BE_b5(); break;
        case 0x919D:
            func_919D_b5(); break;
        case 0x9969:
            func_9969_b5(); break;
        case 0x89CC:
            func_89CC_b5(); break;
        case 0xA4A5:
            func_A4A5_b5(); break;
        case 0xB668:
            func_B668_b5(); break;
        case 0xB665:
            func_B665_b5(); break;
        case 0xB6D1:
            func_B6D1_b5(); break;
        case 0xB6EE:
            func_B6EE_b5(); break;
        case 0xC469:
            func_C469(); break;
        case 0xC287:
            func_C287(); break;
        case 0xA381:
            func_A381_b6(); break;
        case 0xA230:
            func_A230_b6(); break;
        case 0x8564:
            func_8564_b6(); break;
        case 0x9D66:
            func_9D66_b1(); break;
        case 0xADB6:
            func_ADB6_b1(); break;
        case 0xADB9:
            func_ADB9_b1(); break;
        case 0xADB3:
            func_ADB3_b1(); break;
        case 0x9880:
            func_9880_b2(); break;
        case 0xBC9D:
            func_BC9D_b4(); break;
        case 0xB65A:
            func_B65A_b5(); break;
        case 0xB6C4:
            func_B6C4_b5(); break;
        case 0xA5C0:
            func_A5C0_b1(); break;
        case 0xA5F0:
            func_A5F0_b1(); break;
        case 0xA627:
            func_A627_b1(); break;
        case 0xA661:
            func_A661_b1(); break;
        case 0xA68D:
            func_A68D_b1(); break;
        case 0xA6A6:
            func_A6A6_b1(); break;
        case 0xA6A9:
            func_A6A9_b1(); break;
        case 0xA6B0:
            func_A6B0_b1(); break;
        case 0xA6E9:
            func_A6E9_b1(); break;
        case 0xA759:
            func_A759_b1(); break;
        case 0xA831:
            func_A831_b1(); break;
        case 0xA883:
            func_A883_b1(); break;
        case 0xAA4E:
            func_AA4E_b1(); break;
        case 0xAAB8:
            func_AAB8_b1(); break;
        case 0xAAE6:
            func_AAE6_b1(); break;
        case 0xAAFC:
            func_AAFC_b1(); break;
        case 0xAAFE:
            func_AAFE_b1(); break;
        case 0xAB84:
            func_AB84_b1(); break;
        case 0xB057:
            func_B057_b1(); break;
        case 0xB0F5:
            func_B0F5_b1(); break;
        case 0xB1F8:
            func_B1F8_b1(); break;
        default:
            nes_log_dispatch_miss(addr);
            break;
    }
}
