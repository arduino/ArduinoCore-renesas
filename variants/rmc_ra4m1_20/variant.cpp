#include "Arduino.h"
#include "pinmux.inc"

const uint16_t P400_b[] = {
PIN_PWM|CHANNEL_6|PWM_CHANNEL_A|GPT_ODD_CFG,
PIN_SCL|CHANNEL_0,
PIN_INTERRUPT|CHANNEL_0,
SCI_CHANNEL|PIN_SCK|CHANNEL_0|SCI_EVEN_CFG|LAST_ITEM_GUARD
};
#define P400 P400_b
/*
const uint16_t P408_b[] = {
PIN_PWM|CHANNEL_5|PWM_CHANNEL_B|GPT_ODD_CFG,
PIN_INTERRUPT|CHANNEL_7,
SCI_CHANNEL|PIN_CTS_RTS_SS|CHANNEL_1|SCI_EVEN_CFG,
SCI_CHANNEL|PIN_RX_MISO_SCL|CHANNEL_9|SCI_ODD_CFG|LAST_ITEM_GUARD
};
#define P408 P408_b
*/
const uint16_t P014_b[] = {
PIN_DAC|CHANNEL_0,
PIN_ANALOG|CHANNEL_9|LAST_ITEM_GUARD
};
#define P014 P014_b

extern "C" const PinMuxCfg_t g_pin_cfg[] = { 
  { BSP_IO_PORT_03_PIN_01,    P301   }, /* (0)  D0  */
  { BSP_IO_PORT_03_PIN_02,    P302   }, /* (1)  D1  */
  { BSP_IO_PORT_01_PIN_05,    P105   }, /* (2)  D2  */
  { BSP_IO_PORT_01_PIN_04,    P104   }, /* (3)  D3  */
  { BSP_IO_PORT_01_PIN_03,    P103   }, /* (4)  D4  */
  { BSP_IO_PORT_01_PIN_02,    P102   }, /* (5)  D5  */
  { BSP_IO_PORT_01_PIN_06,    P106   }, /* (6)  D6  */
  { BSP_IO_PORT_01_PIN_07,    P107   }, /* (7)  D7  */

  { BSP_IO_PORT_03_PIN_04,    P304   }, /* (8)  D8  */
  { BSP_IO_PORT_03_PIN_03,    P303   }, /* (9)  D9  */
  { BSP_IO_PORT_01_PIN_12,    P112   }, /* (10) D10 */
  { BSP_IO_PORT_01_PIN_09,    P109   }, /* (11) D11 */
  { BSP_IO_PORT_01_PIN_10,    P110   }, /* (12) D12 */
  { BSP_IO_PORT_01_PIN_11,    P111   }, /* (13) D13 LED */

  { BSP_IO_PORT_00_PIN_14,    P014   }, /* (14) A0  */
  { BSP_IO_PORT_00_PIN_00,    P000   }, /* (15) A1  */
  { BSP_IO_PORT_00_PIN_01,    P001   }, /* (16) A2  */
  { BSP_IO_PORT_00_PIN_02,    P002   }, /* (17) A3  */
  { BSP_IO_PORT_01_PIN_01,    P101   }, /* (18) A4  */
  { BSP_IO_PORT_01_PIN_00,    P100   }, /* (19) A5  */
	
  { BSP_IO_PORT_01_PIN_11,    P111   }, /* (20) dummy */
  { BSP_IO_PORT_01_PIN_11,    P111   }, /* (21) dummy */
  { BSP_IO_PORT_01_PIN_11,    P111   }, /* (22) dummy */

  { BSP_IO_PORT_02_PIN_05,    P205   }, /* (23) LED */
  { BSP_IO_PORT_01_PIN_11,    P111   }, /* (24) dummy */

  { BSP_IO_PORT_03_PIN_07,    P307   }, /* (25) DIPSW1 */
  { BSP_IO_PORT_03_PIN_06,    P306   }, /* (26) DIPSW2 */

  { BSP_IO_PORT_04_PIN_02,    P402   }, /* (27) */
  { BSP_IO_PORT_04_PIN_05,    P405   }, /* (28) */
  { BSP_IO_PORT_04_PIN_06,    P406   }, /* (29) */
  { BSP_IO_PORT_04_PIN_03,    P403   }, /* (30) */
  { BSP_IO_PORT_04_PIN_04,    P404   }, /* (31) */
  { BSP_IO_PORT_04_PIN_01,    P401   }, /* (32) */
  { BSP_IO_PORT_04_PIN_00,    P400   }, /* (33) */
  { BSP_IO_PORT_00_PIN_03,    P003   }, /* (34) */

  { BSP_IO_PORT_00_PIN_12,    P012   }, /* (35) */
  { BSP_IO_PORT_00_PIN_11,    P011   }, /* (36) */
  { BSP_IO_PORT_00_PIN_10,    P010   }, /* (37) */
  { BSP_IO_PORT_00_PIN_08,    P008   }, /* (38) */
  { BSP_IO_PORT_00_PIN_07,    P007   }, /* (39) */
  { BSP_IO_PORT_00_PIN_06,    P006   }, /* (40) */
  { BSP_IO_PORT_00_PIN_05,    P005   }, /* (41) */
  { BSP_IO_PORT_00_PIN_04,    P004   }, /* (42) */

  { BSP_IO_PORT_02_PIN_15,    P215   }, /* (43) */
  { BSP_IO_PORT_02_PIN_14,    P214   }, /* (44) */
  { BSP_IO_PORT_07_PIN_08,    P708   }, /* (45) */
  { BSP_IO_PORT_04_PIN_15,    P415   }, /* (46) */
  { BSP_IO_PORT_04_PIN_14,    P414   }, /* (47) */
  { BSP_IO_PORT_04_PIN_13,    P413   }, /* (48) */
  { BSP_IO_PORT_04_PIN_12,    P412   }, /* (49) */
  { BSP_IO_PORT_04_PIN_11,    P411   }, /* (50) */
  { BSP_IO_PORT_04_PIN_10,    P410   }, /* (51) */
  { BSP_IO_PORT_04_PIN_09,    P409   }, /* (52) */
  { BSP_IO_PORT_04_PIN_08,    P408   }, /* (53) */
  { BSP_IO_PORT_04_PIN_07,    P407   }, /* (54) */
  { BSP_IO_PORT_02_PIN_06,    P206   }, /* (55) */
  { BSP_IO_PORT_02_PIN_00,    P200   }, /* (56) */
  { BSP_IO_PORT_08_PIN_08,    P808   }, /* (57) */
  { BSP_IO_PORT_08_PIN_09,    P809   }, /* (58) */
  { BSP_IO_PORT_03_PIN_00,    P300   }, /* (59) */
  { BSP_IO_PORT_01_PIN_08,    P108   }, /* (60) */

  { BSP_IO_PORT_00_PIN_13,    P013   }, /* (61) */
  { BSP_IO_PORT_00_PIN_15,    P015   }, /* (62) */
  { BSP_IO_PORT_05_PIN_05,    P505   }, /* (63) */
  { BSP_IO_PORT_05_PIN_04,    P504   }, /* (64) */
  { BSP_IO_PORT_05_PIN_03,    P503   }, /* (65) */
  { BSP_IO_PORT_05_PIN_02,    P502   }, /* (66) */
  { BSP_IO_PORT_05_PIN_01,    P501   }, /* (67) */
  { BSP_IO_PORT_05_PIN_00,    P500   }, /* (68) */
  { BSP_IO_PORT_06_PIN_00,    P600   }, /* (69) */
  { BSP_IO_PORT_06_PIN_01,    P601   }, /* (70) */
  { BSP_IO_PORT_06_PIN_02,    P602   }, /* (71) */
  { BSP_IO_PORT_06_PIN_03,    P603   }, /* (72) */
  { BSP_IO_PORT_06_PIN_10,    P610   }, /* (73) */
  { BSP_IO_PORT_06_PIN_09,    P609   }, /* (74) */
  { BSP_IO_PORT_06_PIN_08,    P608   }, /* (75) */
  { BSP_IO_PORT_01_PIN_15,    P115   }, /* (76) */
  { BSP_IO_PORT_01_PIN_14,    P114   }, /* (77) */
  { BSP_IO_PORT_01_PIN_13,    P113   }, /* (78) */

  { BSP_IO_PORT_02_PIN_03,    P203   }, /* (79) MOSI */
  { BSP_IO_PORT_02_PIN_02,    P202   }, /* (80) MISO */
  { BSP_IO_PORT_02_PIN_04,    P204   }, /* (81) CLK  */
};

extern "C" {
    unsigned int PINCOUNT_fn() {
        return (sizeof(g_pin_cfg) / sizeof(g_pin_cfg[0]));
    }
}

int32_t getPinIndex(bsp_io_port_pin_t p) {
  int max_index = PINS_COUNT;
  int rv = -1;
  for(int i = 0; i < max_index; i++) {
    if(g_pin_cfg[i].pin == p) {
      rv = i;
      break;
    }
  }
  return rv;
}

#include "FspTimer.h"

void initVariant() {
  // bootloader configures LED_BUILTIN as PWM output, deconfigure it to avoid spurious signals
//  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);  // LOWで点灯
  pinMode(LED_BUILTIN, INPUT_PULLUP);

  R_MSTP->MSTPCRA = 0x00000000;     // 絶対に変更しないでください
  R_MSTP->MSTPCRB = 0x00000000;     // 変更すると書き込みできなくなることがあります
  R_MSTP->MSTPCRC = 0x00000000;
  R_MSTP->MSTPCRD = 0x00000000;

  R_SYSTEM->PRCR = 0xA50d;          // プロテクトレジスタ 書き込み許可
  R_SYSTEM->SOSCCR_b.SOSTP = 1;     // サブクロック端子(P214,P215)を入力端子に設定

  R_PMISC->PWPR_b.B0WI = 0;         // PmnPFSレジスタへの書き込みを許可
  R_PMISC->PWPR_b.PFSWE = 1;        // PmnPFSレジスタ書き込み許可

  R_PFS->PORT[1].PIN[ 8].PmnPFS = 0x00000000;  // デフォルトが汎用ポートではないので、汎用ポートにする
  R_PFS->PORT[1].PIN[ 9].PmnPFS = 0x00000000;
  R_PFS->PORT[1].PIN[10].PmnPFS = 0x00000000;
  R_PFS->PORT[3].PIN[ 0].PmnPFS = 0x00000000;

}