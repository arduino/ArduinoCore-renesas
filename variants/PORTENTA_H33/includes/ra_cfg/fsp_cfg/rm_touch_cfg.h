/* generated configuration header file - do not edit */
#ifndef RM_TOUCH_CFG_H_
#define RM_TOUCH_CFG_H_
#ifdef __cplusplus
                    extern "C" {
                    #endif

#define TOUCH_CFG_PARAM_CHECKING_ENABLE   (BSP_CFG_PARAM_CHECKING_ENABLE)
#define TOUCH_CFG_UART_MONITOR_SUPPORT    (0)
#define TOUCH_CFG_UART_TUNING_SUPPORT     (0)
#ifndef QE_TOUCH_CONFIGURATION
#define TOUCH_CFG_NUM_BUTTONS             (1)
#define TOUCH_CFG_NUM_SLIDERS             (0)
#define TOUCH_CFG_NUM_WHEELS              (0)
#if (BSP_FEATURE_CTSU_VERSION == 2)
                     #define TOUCH_CFG_PAD_ENABLE             (0)
                    #endif
#endif

#ifdef __cplusplus
                    }
                    #endif
#endif /* RM_TOUCH_CFG_H_ */
