/* generated configuration header file - do not edit */
#ifndef R_CANFD_CFG_H_
#define R_CANFD_CFG_H_
#ifdef __cplusplus
            extern "C" {
            #endif

/* Buffer RAM used: 680 bytes */

#define CANFD_CFG_PARAM_CHECKING_ENABLE   ((1))

#define CANFD_CFG_AFL_CH0_RULE_NUM   (2)
#define CANFD_CFG_AFL_CH1_RULE_NUM   (2)

#define CANFD_CFG_GLOBAL_ERROR_CH    ((1U))

#define CANFD_CFG_FD_STANDARD             ((0))
#define CANFD_CFG_FD_PROTOCOL_EXCEPTION   ((0))

#define CANFD_CFG_GLOBAL_ERR_SOURCES    ( 0x3)
#define CANFD_CFG_TX_PRIORITY           ((R_CANFD_CFDGCFG_TPRI_Msk))
#define CANFD_CFG_DLC_CHECK             ((0))
#define CANFD_CFG_FD_OVERFLOW           ((0))
#define CANFD_CFG_RXMB_NUMBER           (2)
#define CANFD_CFG_RXMB_SIZE             ((0))
#define CANFD_CFG_GLOBAL_ERR_IPL        ((12))
#define CANFD_CFG_RX_FIFO_IPL           ((12))

#define CANFD_CFG_RXFIFO0_INT_THRESHOLD ((3U))
#define CANFD_CFG_RXFIFO0_DEPTH         ((3))
#define CANFD_CFG_RXFIFO0_PAYLOAD       ((0))
#define CANFD_CFG_RXFIFO0_INT_MODE      ((R_CANFD_CFDRFCC_RFIE_Msk | R_CANFD_CFDRFCC_RFIM_Msk))
#define CANFD_CFG_RXFIFO0_ENABLE        ((1))

#define CANFD_CFG_RXFIFO1_INT_THRESHOLD ((3U))
#define CANFD_CFG_RXFIFO1_DEPTH         ((3))
#define CANFD_CFG_RXFIFO1_PAYLOAD       ((0))
#define CANFD_CFG_RXFIFO1_INT_MODE      ((R_CANFD_CFDRFCC_RFIE_Msk | R_CANFD_CFDRFCC_RFIM_Msk))
#define CANFD_CFG_RXFIFO1_ENABLE        ((1))

#define CANFD_CFG_RXFIFO2_INT_THRESHOLD ((3U))
#define CANFD_CFG_RXFIFO2_DEPTH         ((3))
#define CANFD_CFG_RXFIFO2_PAYLOAD       ((0))
#define CANFD_CFG_RXFIFO2_INT_MODE      ((R_CANFD_CFDRFCC_RFIE_Msk | R_CANFD_CFDRFCC_RFIM_Msk))
#define CANFD_CFG_RXFIFO2_ENABLE        ((0))

#define CANFD_CFG_RXFIFO3_INT_THRESHOLD ((3U))
#define CANFD_CFG_RXFIFO3_DEPTH         ((3))
#define CANFD_CFG_RXFIFO3_PAYLOAD       ((0))
#define CANFD_CFG_RXFIFO3_INT_MODE      ((R_CANFD_CFDRFCC_RFIE_Msk | R_CANFD_CFDRFCC_RFIM_Msk))
#define CANFD_CFG_RXFIFO3_ENABLE        ((0))

#define CANFD_CFG_RXFIFO4_INT_THRESHOLD ((3U))
#define CANFD_CFG_RXFIFO4_DEPTH         ((3))
#define CANFD_CFG_RXFIFO4_PAYLOAD       ((0))
#define CANFD_CFG_RXFIFO4_INT_MODE      ((R_CANFD_CFDRFCC_RFIE_Msk | R_CANFD_CFDRFCC_RFIM_Msk))
#define CANFD_CFG_RXFIFO4_ENABLE        ((0))

#define CANFD_CFG_RXFIFO5_INT_THRESHOLD ((3U))
#define CANFD_CFG_RXFIFO5_DEPTH         ((3))
#define CANFD_CFG_RXFIFO5_PAYLOAD       ((0))
#define CANFD_CFG_RXFIFO5_INT_MODE      ((R_CANFD_CFDRFCC_RFIE_Msk | R_CANFD_CFDRFCC_RFIM_Msk))
#define CANFD_CFG_RXFIFO5_ENABLE        ((0))

#define CANFD_CFG_RXFIFO6_INT_THRESHOLD ((3U))
#define CANFD_CFG_RXFIFO6_DEPTH         ((3))
#define CANFD_CFG_RXFIFO6_PAYLOAD       ((0))
#define CANFD_CFG_RXFIFO6_INT_MODE      ((R_CANFD_CFDRFCC_RFIE_Msk | R_CANFD_CFDRFCC_RFIM_Msk))
#define CANFD_CFG_RXFIFO6_ENABLE        ((0))

#define CANFD_CFG_RXFIFO7_INT_THRESHOLD ((3U))
#define CANFD_CFG_RXFIFO7_DEPTH         ((3))
#define CANFD_CFG_RXFIFO7_PAYLOAD       ((0))
#define CANFD_CFG_RXFIFO7_INT_MODE      ((R_CANFD_CFDRFCC_RFIE_Msk | R_CANFD_CFDRFCC_RFIM_Msk))
#define CANFD_CFG_RXFIFO7_ENABLE        ((0))

#ifdef __cplusplus
            }
            #endif
#endif /* R_CANFD_CFG_H_ */
