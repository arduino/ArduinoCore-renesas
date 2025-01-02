/***********************************************************************************************************************
 * Copyright [2020-2022] Renesas Electronics Corporation and/or its affiliates.  All Rights Reserved.
 *
 * This software and documentation are supplied by Renesas Electronics America Inc. and may only be used with products
 * of Renesas Electronics Corp. and its affiliates ("Renesas").  No other uses are authorized.  Renesas products are
 * sold pursuant to Renesas terms and conditions of sale.  Purchasers are solely responsible for the selection and use
 * of Renesas products and Renesas assumes no liability.  No license, express or implied, to any intellectual property
 * right is granted by Renesas. This software is protected under all applicable laws, including copyright laws. Renesas
 * reserves the right to change or discontinue this software and/or this documentation. THE SOFTWARE AND DOCUMENTATION
 * IS DELIVERED TO YOU "AS IS," AND RENESAS MAKES NO REPRESENTATIONS OR WARRANTIES, AND TO THE FULLEST EXTENT
 * PERMISSIBLE UNDER APPLICABLE LAW, DISCLAIMS ALL WARRANTIES, WHETHER EXPLICITLY OR IMPLICITLY, INCLUDING WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, AND NONINFRINGEMENT, WITH RESPECT TO THE SOFTWARE OR
 * DOCUMENTATION.  RENESAS SHALL HAVE NO LIABILITY ARISING OUT OF ANY SECURITY VULNERABILITY OR BREACH.  TO THE MAXIMUM
 * EXTENT PERMITTED BY LAW, IN NO EVENT WILL RENESAS BE LIABLE TO YOU IN CONNECTION WITH THE SOFTWARE OR DOCUMENTATION
 * (OR ANY PERSON OR ENTITY CLAIMING RIGHTS DERIVED FROM YOU) FOR ANY LOSS, DAMAGES, OR CLAIMS WHATSOEVER, INCLUDING,
 * WITHOUT LIMITATION, ANY DIRECT, CONSEQUENTIAL, SPECIAL, INDIRECT, PUNITIVE, OR INCIDENTAL DAMAGES; ANY LOST PROFITS,
 * OTHER ECONOMIC DAMAGE, PROPERTY DAMAGE, OR PERSONAL INJURY; AND EVEN IF RENESAS HAS BEEN ADVISED OF THE POSSIBILITY
 * OF SUCH LOSS, DAMAGES, CLAIMS OR COSTS.
 **********************************************************************************************************************/

#ifndef BSP_FEATURE_H
#define BSP_FEATURE_H

/***********************************************************************************************************************
 * Includes   <System Includes> , "Project Includes"
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

/** The main oscillator drive value is based upon the oscillator frequency selected in the configuration */
#if (BSP_CFG_XTAL_HZ > (9999999))
 #define CGC_MAINCLOCK_DRIVE    (0x00U)
#else
 #define CGC_MAINCLOCK_DRIVE    (0x01U)
#endif

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Exported global variables (to be accessed by other files)
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private global variables and functions
 **********************************************************************************************************************/

#define BSP_FEATURE_ACMPHS_MIN_WAIT_TIME_US                  (0) // Feature not available on this MCU
#define BSP_FEATURE_ACMPHS_VREF                              (0) // Feature not available on this MCU

#define BSP_FEATURE_ACMPLP_HAS_COMPSEL_REGISTERS             (1)
#define BSP_FEATURE_ACMPLP_MIN_WAIT_TIME_US                  (100U)

#define BSP_FEATURE_ADC_ADDITION_SUPPORTED                   (1U)
#define BSP_FEATURE_ADC_CALIBRATION_REG_AVAILABLE            (0U)
#define BSP_FEATURE_ADC_CLOCK_SOURCE                         (FSP_PRIV_CLOCK_PCLKC)
#define BSP_FEATURE_ADC_GROUP_B_SENSORS_ALLOWED              (0U)
#define BSP_FEATURE_ADC_HAS_ADCER_ADPRC                      (1U)
#define BSP_FEATURE_ADC_HAS_ADCER_ADRFMT                     (1U)
#define BSP_FEATURE_ADC_HAS_ADHVREFCNT                       (1U)
#define BSP_FEATURE_BSP_HAS_CLOCK_SUPPLY_TYPEB               (0U)
#define BSP_FEATURE_ADC_HAS_PGA                              (0) // Feature not available on this MCU
#define BSP_FEATURE_ADC_HAS_SAMPLE_HOLD_REG                  (0U)
#define BSP_FEATURE_ADC_HAS_VREFAMPCNT                       (0U)
#define BSP_FEATURE_ADC_MAX_RESOLUTION_BITS                  (14U)
#define BSP_FEATURE_ADC_SENSORS_EXCLUSIVE                    (1U)
#define BSP_FEATURE_ADC_SENSOR_MIN_SAMPLING_TIME             (5000U)
#define BSP_FEATURE_ADC_TSN_CALIBRATION32_AVAILABLE          (0U)
#define BSP_FEATURE_ADC_TSN_CALIBRATION32_MASK               (0U)
#define BSP_FEATURE_ADC_TSN_CALIBRATION_AVAILABLE            (1U)
#define BSP_FEATURE_ADC_TSN_CONTROL_AVAILABLE                (0U)
#define BSP_FEATURE_ADC_TSN_SLOPE                            (-3650)
#define BSP_FEATURE_ADC_UNIT_0_CHANNELS                      (0x3FF7FFF) // 0 to 14, 16 to 25 in unit 0
#define BSP_FEATURE_ADC_UNIT_1_CHANNELS                      (0)
#define BSP_FEATURE_ADC_VALID_UNIT_MASK                      (1U)
#define BSP_FEATURE_ADC_HAS_ADBUF                            (0U)

#define BSP_FEATURE_ADC_B_TSN_CALIBRATION32_MASK             (0U)
#define BSP_FEATURE_ADC_B_TSN_SLOPE                          (0U)
#define BSP_FEATURE_ADC_B_UNIT_0_CHANNELS                    (0U)
#define BSP_FEATURE_ADC_B_UNIT_1_CHANNELS                    (0U)

#define BSP_FEATURE_AGT_VALID_CHANNEL_MASK                   (0x03)
#define BSP_FEATURE_AGT_MAX_CHANNEL_NUM                      (1)
#define BSP_FEATURE_AGT_HAS_AGTW                             (0U)

#define BSP_FEATURE_BSP_FLASH_CACHE                          (1)
#define BSP_FEATURE_BSP_FLASH_CACHE_DISABLE_OPM              (1U)
#define BSP_FEATURE_BSP_FLASH_PREFETCH_BUFFER                (0)
#define BSP_FEATURE_BSP_HAS_CODE_SYSTEM_CACHE                (0)
#define BSP_FEATURE_BSP_HAS_GPT_CLOCK                        (0)
#define BSP_FEATURE_BSP_HAS_OCTASPI_CLOCK                    (0U)
#define BSP_FEATURE_BSP_HAS_CANFD_CLOCK                      (0) // Feature not available on this MCU
#define BSP_FEATURE_BSP_HAS_SCE5                             (1)
#define BSP_FEATURE_CRYPTO_HAS_SCE5B                         (0)
#define BSP_FEATURE_CRYPTO_HAS_SCE5                          (1)
#define BSP_FEATURE_BSP_HAS_SCE_ON_RA2                       (0) // Feature not available on this MCU
#define BSP_FEATURE_CRYPTO_HAS_CTR_DRBG                      (1)
#define BSP_FEATURE_BSP_HAS_SECURITY_MPU                     (1U)
#define BSP_FEATURE_BSP_HAS_SP_MON                           (1U)
#define BSP_FEATURE_BSP_HAS_USBCKDIVCR                       (0U)
#define BSP_FEATURE_BSP_HAS_USB_CLOCK_DIV                    (0U)
#define BSP_FEATURE_BSP_HAS_USB_CLOCK_REQ                    (0U) // On the RA6M4 there is a request bit that must be set before changing USB clock settings.
#define BSP_FEATURE_BSP_HAS_USB_CLOCK_SEL                    (1U)
#define BSP_FEATURE_BSP_HAS_USB_CLOCK_SEL_ALT                (1U)
#define BSP_FEATURE_BSP_MCU_INFO_POINTER_LOCATION            (0x407FB19C)
#define BSP_FEATURE_BSP_MPU_REGION0_MASK                     (0x00FFFFFFU)
#define BSP_FEATURE_BSP_MSTP_GPT_MSTPD5_MAX_CH               (1U) // Largest channel number associated with lower MSTP bit for GPT on this MCU.
#define BSP_FEATURE_BSP_MSTP_HAS_MSTPCRE                     (0U)
#define BSP_FEATURE_BSP_OFS1_HOCOFRQ_MASK                    (0xFFFF8FFFU)
#define BSP_FEATURE_BSP_OFS1_HOCOFRQ_OFFSET                  (12U)
#define BSP_FEATURE_BSP_OSIS_PADDING                         (1)
#define BSP_FEATURE_BSP_POWER_CHANGE_MSTP_REQUIRED           (0U)
#define BSP_FEATURE_BSP_HAS_DCDC_REGULATOR                   (0U)
#define BSP_FEATURE_BSP_RESET_TRNG                           (1U)
#define BSP_FEATURE_BSP_SYS_CLOCK_FREQ_NO_RAM_WAITS          (0U) // The maximum frequency allowed without having RAM wait state enabled in SRAMWTSC.
#define BSP_FEATURE_BSP_SYS_CLOCK_FREQ_ONE_ROM_WAITS         (0U) // The maximum frequency allowed without having one ROM wait cycle.
#define BSP_FEATURE_BSP_SYS_CLOCK_FREQ_THREE_ROM_WAITS       (0U) // The maximum frequency allowed without having three ROM wait cycles (Set to zero if this is not an option).
#define BSP_FEATURE_BSP_SYS_CLOCK_FREQ_TWO_ROM_WAITS         (0U) // The maximum frequency allowed without having two ROM wait cycles.
#define BSP_FEATURE_BSP_UNIQUE_ID_OFFSET                     (0x14)
#define BSP_FEATURE_BSP_UNIQUE_ID_POINTER                    ((*(uint32_t *) BSP_FEATURE_BSP_MCU_INFO_POINTER_LOCATION) \
                                                              +                                                         \
                                                              BSP_FEATURE_BSP_UNIQUE_ID_OFFSET)
#define BSP_FEATURE_BSP_VBATT_HAS_VBTCR1_BPWSWSTP            (1U)

#define BSP_FEATURE_CAN_CHECK_PCLKB_RATIO                    (1U)
#define BSP_FEATURE_CAN_CLOCK                                (FSP_PRIV_CLOCK_PCLKA)
#define BSP_FEATURE_CAN_MCLOCK_ONLY                          (0U)
#define BSP_FEATURE_CAN_NUM_CHANNELS                         (1U)

#define BSP_FEATURE_CANFD_NUM_CHANNELS                       (0U) // Feature not available on this MCU
#define BSP_FEATURE_CANFD_LITE                               (0U)
#define BSP_FEATURE_CANFD_FD_SUPPORT                         (0U)

#define BSP_FEATURE_CGC_HAS_BCLK                             (0U) // This MCU does not have a BCLK
#define BSP_FEATURE_CGC_HAS_FCLK                             (1U)
#define BSP_FEATURE_CGC_HAS_FLDWAITR                         (0U)
#define BSP_FEATURE_CGC_HAS_FLWT                             (0U)
#define BSP_FEATURE_CGC_HAS_FLL                              (0U)
#define BSP_FEATURE_CGC_HAS_HOCOWTCR                         (1U)
#define BSP_FEATURE_CGC_HAS_MEMWAIT                          (1U)
#define BSP_FEATURE_CGC_HAS_PCLKA                            (1U)
#define BSP_FEATURE_CGC_HAS_PCLKB                            (1U)
#define BSP_FEATURE_CGC_HAS_PCLKC                            (1U)
#define BSP_FEATURE_CGC_HAS_PCLKD                            (1U)
#define BSP_FEATURE_CGC_HAS_PLL                              (1U)
#define BSP_FEATURE_CGC_HAS_PLL2                             (0U) // On the RA6M4 there is another PLL that can be used as a clock source for USB and OCTASPI.
#define BSP_FEATURE_CGC_HAS_SOSC                             (1U)
#define BSP_FEATURE_CGC_HAS_SRAMPRCR2                        (0U) // On the RA6M4 there is another register to enable write access for SRAMWTSC.
#define BSP_FEATURE_CGC_HAS_SRAMWTSC                         (0U)
#define BSP_FEATURE_CGC_HOCOSF_BEFORE_OPCCR                  (1U)
#define BSP_FEATURE_CGC_HOCOWTCR_64MHZ_ONLY                  (1U)
#define BSP_FEATURE_CGC_ICLK_DIV_RESET                       (BSP_CLOCKS_SYS_CLOCK_DIV_16)
#define BSP_FEATURE_CGC_LOCO_STABILIZATION_MAX_US            (100U)
#define BSP_FEATURE_CGC_LOW_SPEED_MAX_FREQ_HZ                (1000000U) // This MCU does have Low Speed Mode, up to 1MHz
#define BSP_FEATURE_CGC_LOW_VOLTAGE_MAX_FREQ_HZ              (4000000U) // This MCU does have Low Voltage Mode, up to 4MHz
#define BSP_FEATURE_CGC_MIDDLE_SPEED_MAX_FREQ_HZ             (8000000U) // This MCU does have Middle Speed Mode, up to 8MHz
#define BSP_FEATURE_CGC_MOCO_STABILIZATION_MAX_US            (1U)
#define BSP_FEATURE_CGC_MODRV_MASK                           (0x08U)
#define BSP_FEATURE_CGC_MODRV_SHIFT                          (0x3U)
#define BSP_FEATURE_CGC_PLLCCR_TYPE                          (2U)
#define BSP_FEATURE_CGC_PLLCCR_WAIT_US                       (1U) // 1 us wait between setting PLLCCR and clearing PLLSTP
#define BSP_FEATURE_CGC_PLLCCR_MAX_HZ                        (0U) // This MCU does not use PLLCCR to set PLL frequency
#define BSP_FEATURE_CGC_SCKDIVCR_BCLK_MATCHES_PCLKB          (1U) // RA4M1 requires that bits 16-18 of SCKDIVCR be the same as the bits for PCKB
#define BSP_FEATURE_CGC_SODRV_MASK                           (0x03U)
#define BSP_FEATURE_CGC_SODRV_SHIFT                          (0x0U)
#define BSP_FEATURE_CGC_STARTUP_OPCCR_MODE                   (2)

#define BSP_FEATURE_CRYPTO_HAS_SCE7_MISSING_PROCS            (0)
#define BSP_FEATURE_CRYPTO_HAS_AES                           (1)
#define BSP_FEATURE_CRYPTO_HAS_SCE7                          (0)
#define BSP_FEATURE_CRYPTO_HAS_AES_WRAPPED                   (1)
#define BSP_FEATURE_CRYPTO_HAS_ECC                           (0)
#define BSP_FEATURE_CRYPTO_HAS_ECC_WRAPPED                   (0)
#define BSP_FEATURE_CRYPTO_HAS_HASH                          (0)
#define BSP_FEATURE_CRYPTO_HAS_RSA                           (0)
#define BSP_FEATURE_CRYPTO_HAS_RSA_WRAPPED                   (0)

#define BSP_FEATURE_CTSU_CTSUCHAC_REGISTER_COUNT             (5U)
#define BSP_FEATURE_CTSU_CTSUCHTRC_REGISTER_COUNT            (5U)
#define BSP_FEATURE_CTSU_HAS_TXVSEL                          (0)
#define BSP_FEATURE_CTSU_VERSION                             (1)

#define BSP_FEATURE_DAC8_HAS_CHARGEPUMP                      (0U)
#define BSP_FEATURE_DAC8_HAS_DA_AD_SYNCHRONIZE               (0U)
#define BSP_FEATURE_DAC8_HAS_REALTIME_MODE                   (0U)
#define BSP_FEATURE_DAC8_MAX_CHANNELS                        (2U)

#define BSP_FEATURE_DAC_HAS_CHARGEPUMP                       (0U)
#define BSP_FEATURE_DAC_HAS_DAVREFCR                         (1U)
#define BSP_FEATURE_DAC_HAS_DA_AD_SYNCHRONIZE                (1U)
#define BSP_FEATURE_DAC_HAS_OUTPUT_AMPLIFIER                 (0U)
#define BSP_FEATURE_DAC_HAS_INTERNAL_OUTPUT                  (0U)
#define BSP_FEATURE_DAC_MAX_CHANNELS                         (1U)

#define BSP_FEATURE_DOC_VERSION                              (1U)

#define BSP_FEATURE_DMAC_MAX_CHANNEL                         (4U)
#define BSP_FEATURE_DMAC_HAS_REPEAT_BLOCK_MODE               (0U)

#define BSP_FEATURE_DWT_CYCCNT                               (1U)          // RA4M1 has Data Watchpoint Cycle Count Register

#define BSP_FEATURE_ELC_PERIPHERAL_MASK                      (0x0007D3FFU) // Positions of event link set registers (ELSRs) available on this MCU
#define BSP_FEATURE_ELC_VERSION                              (1U)

#define BSP_FEATURE_ETHER_FIFO_DEPTH                         (0)           // Feature not available on this MCU
#define BSP_FEATURE_ETHER_MAX_CHANNELS                       (0)           // Feature not available on this MCU

#define BSP_FEATURE_FLASH_DATA_FLASH_START                   (0x40100000U)
#define BSP_FEATURE_FLASH_HP_CF_REGION0_BLOCK_SIZE           (0)           // Feature not available on this MCU
#define BSP_FEATURE_FLASH_HP_CF_REGION0_SIZE                 (0)           // Feature not available on this MCU
#define BSP_FEATURE_FLASH_HP_CF_REGION1_BLOCK_SIZE           (0)           // Feature not available on this MCU
#define BSP_FEATURE_FLASH_HP_CF_WRITE_SIZE                   (0)           // Feature not available on this MCU
#define BSP_FEATURE_FLASH_HP_DF_BLOCK_SIZE                   (0)           // Feature not available on this MCU
#define BSP_FEATURE_FLASH_HP_DF_WRITE_SIZE                   (0)           // Feature not available on this MCU
#define BSP_FEATURE_FLASH_HP_HAS_FMEPROT                     (0)           // Feature not available on this MCU
#define BSP_FEATURE_FLASH_HP_SUPPORTS_DUAL_BANK              (0)
#define BSP_FEATURE_FLASH_HP_CF_DUAL_BANK_START              (0)           // Feature not available on this MCU
#define BSP_FEATURE_FLASH_HP_VERSION                         (0)           // Feature not available on this MCU
#define BSP_FEATURE_FLASH_LP_AWS_FAW_MASK                    (0xFFFU)
#define BSP_FEATURE_FLASH_LP_AWS_FAW_SHIFT                   (10)
#define BSP_FEATURE_FLASH_LP_CF_BLOCK_SIZE                   (0x800U)
#define BSP_FEATURE_FLASH_LP_CF_WRITE_SIZE                   (8)
#define BSP_FEATURE_FLASH_LP_DF_BLOCK_SIZE                   (0x400U)
#define BSP_FEATURE_FLASH_LP_DF_WRITE_SIZE                   (1)
#define BSP_FEATURE_FLASH_LP_FLASH_CLOCK_SRC                 ((fsp_priv_clock_t) FSP_PRIV_CLOCK_FCLK) // RA4M1 FlashIF uses FCLK
#define BSP_FEATURE_FLASH_LP_VERSION                         (3)
#define BSP_FEATURE_FLASH_SUPPORTS_ACCESS_WINDOW             (1)
#define BSP_FEATURE_FLASH_SUPPORTS_ID_CODE                   (1)

#define BSP_FEATURE_GPTEH_CHANNEL_MASK                       (0)

#define BSP_FEATURE_GPTE_CHANNEL_MASK                        (0)

#define BSP_FEATURE_GPT_32BIT_CHANNEL_MASK                   (0x3)
#define BSP_FEATURE_GPT_VALID_CHANNEL_MASK                   (0xFF)
#define BSP_FEATURE_GPT_EVENT_COUNT_CHANNEL_MASK             (BSP_FEATURE_GPT_VALID_CHANNEL_MASK)
#define BSP_FEATURE_GPT_HAS_GTCLKCR                          (0U)
#define BSP_FEATURE_GPT_ODC_VALID_CHANNEL_MASK               (0U) // Feature not available on this MCU
#define BSP_FEATURE_GPT_ODC_FRANGE_FREQ_MIN                  (0U) // Feature not available on this MCU
#define BSP_FEATURE_GPT_ODC_FREQ_MAX                         (0U) // Feature not available on this MCU
#define BSP_FEATURE_GPT_ODC_FREQ_MIN                         (0U) // Feature not available on this MCU
#define BSP_FEATURE_GPT_CLOCK_DIVIDER_STEP_SIZE              (4U)
#define BSP_FEATURE_GPT_CLOCK_DIVIDER_VALUE_7_9_VALID        (0U)

#define BSP_FEATURE_ICU_HAS_WUPEN1                           (0U)
#define BSP_FEATURE_ICU_IRQ_CHANNELS_MASK                    (0xDFFFU)
#define BSP_FEATURE_ICU_WUPEN_MASK                           (0xFB9FDFFFU)
#define BSP_FEATURE_ICU_HAS_INTERRUPT_GROUPS                 (0U)

#define BSP_FEATURE_IIC_FAST_MODE_PLUS                       (0U)
#define BSP_FEATURE_IIC_VALID_CHANNEL_MASK                   (0x07)
#define BSP_FEATURE_IIC_VERSION                              (1U)
#define BSP_FEATURE_BSP_HAS_IIC_CLOCK                        (0U)
#define BSP_FEATURE_IIC_BUS_FREE_TIME_MULTIPLIER             (0U)

#define BSP_FEATURE_I3C_NUM_CHANNELS                         (0U) // Feature not available on this MCU
#define BSP_FEATURE_I3C_MAX_DEV_COUNT                        (0U) // Feature not available on this MCU
#define BSP_FEATURE_I3C_NTDTBP0_DEPTH                        (0U) // Feature not available on this MCU

#define BSP_FEATURE_IOPORT_ELC_PORTS                         (0x001EU)
#define BSP_FEATURE_IOPORT_HAS_ETHERNET                      (0U)

#define BSP_FEATURE_KINT_HAS_MSTP                            (0U)

#define BSP_FEATURE_LPM_CHANGE_MSTP_ARRAY                    (0) // Feature not available on this MCU
#define BSP_FEATURE_LPM_CHANGE_MSTP_REQUIRED                 (0U)
#define BSP_FEATURE_LPM_DPSIEGR_MASK                         (0) // Feature not available on this MCU
#define BSP_FEATURE_LPM_DPSIER_MASK                          (0) // Feature not available on this MCU
#define BSP_FEATURE_LPM_HAS_DEEP_STANDBY                     (0U)
#define BSP_FEATURE_LPM_HAS_SBYCR_OPE                        (0U)
#define BSP_FEATURE_LPM_HAS_SNZEDCR1                         (0U)
#define BSP_FEATURE_LPM_HAS_SNZREQCR1                        (0U)
#define BSP_FEATURE_LPM_HAS_STCONR                           (0U)
#define BSP_FEATURE_LPM_SBYCR_WRITE1_B14                     (1U)
#define BSP_FEATURE_LPM_SNZEDCR_MASK                         (0x0000009FU)
#define BSP_FEATURE_LPM_SNZREQCR_MASK                        (0x7382DFFFU)

#define BSP_FEATURE_LVD_HAS_DIGITAL_FILTER                   (0U)
#define BSP_FEATURE_LVD_HAS_LVDLVLR                          (1U)
#define BSP_FEATURE_LVD_MONITOR_1_HI_THRESHOLD               (LVD_THRESHOLD_MONITOR_1_LEVEL_4_29V) // 4.29V
#define BSP_FEATURE_LVD_MONITOR_1_LOW_THRESHOLD              (LVD_THRESHOLD_MONITOR_1_LEVEL_1_65V) // 1.65V
#define BSP_FEATURE_LVD_MONITOR_2_HI_THRESHOLD               (LVD_THRESHOLD_MONITOR_2_LEVEL_4_29V) // 4.29V
#define BSP_FEATURE_LVD_MONITOR_2_LOW_THRESHOLD              (LVD_THRESHOLD_MONITOR_2_LEVEL_3_84V) // 3.84V
#define BSP_FEATURE_LVD_STABILIZATION_TIME_US                (300U)                                // Time in microseconds required for LVD to stabilize

#define BSP_FEATURE_IOPORT_VERSION                           (1U)

#define BSP_FEATURE_OPAMP_BASE_ADDRESS                       (1U)
#define BSP_FEATURE_OPAMP_HAS_MIDDLE_SPEED                   (0U)
#define BSP_FEATURE_OPAMP_HAS_SWITCHES                       (0U)
#define BSP_FEATURE_OPAMP_HAS_THIRD_CHANNEL                  (1U)
#define BSP_FEATURE_OPAMP_MIN_WAIT_TIME_HS_US                (13U)
#define BSP_FEATURE_OPAMP_MIN_WAIT_TIME_LP_US                (650U)    // This information comes from the Electrical Characteristics chapter of the hardware manual.
#define BSP_FEATURE_OPAMP_MIN_WAIT_TIME_MS_US                (0xFFFFU) // Middle speed mode not supported
#define BSP_FEATURE_OPAMP_TRIM_CAPABLE                       (0U)
#define BSP_FEATURE_OPAMP_VARIANT_CHANNEL_MASK               (0xFU)

#define BSP_FEATURE_OSPI_DEVICE_0_START_ADDRESS              (0x0U)
#define BSP_FEATURE_OSPI_DEVICE_1_START_ADDRESS              (0x0U)

#define BSP_FEATURE_POEG_CHANNEL_MASK                        (0x3U)

#define BSP_FEATURE_QSPI_DEVICE_START_ADDRESS                (0x0U)

#define BSP_FEATURE_RTC_HAS_ROPSEL                           (0U)

#define BSP_FEATURE_SCI_VERSION                              (1U)
#define BSP_FEATURE_SCI_ADDRESS_MATCH_CHANNELS               (BSP_FEATURE_SCI_CHANNELS)
#define BSP_FEATURE_SCI_CHANNELS                             (0x207U)
#define BSP_FEATURE_SCI_CLOCK                                (FSP_PRIV_CLOCK_PCLKA)
#define BSP_FEATURE_SCI_UART_FIFO_CHANNELS                   (0x3U)
#define BSP_FEATURE_SCI_UART_FIFO_DEPTH                      (16U)
#define BSP_FEATURE_SCI_UART_CSTPEN_CHANNELS                 (0x0U)

#define BSP_FEATURE_SDHI_HAS_CARD_DETECTION                  (0) // Feature not available on this MCU
#define BSP_FEATURE_SDHI_SUPPORTS_8_BIT_MMC                  (0) // Feature not available on this MCU
#define BSP_FEATURE_SDHI_VALID_CHANNEL_MASK                  (0) // Feature not available on this MCU
#define BSP_FEATURE_SDHI_CLOCK                               (FSP_PRIV_CLOCK_PCLKA)
#define BSP_FEATURE_SDHI_MIN_CLOCK_DIVISION_SHIFT            (0) // Feature not available on this MCU

#define BSP_FEATURE_SDRAM_START_ADDRESS                      (0x0U)

#define BSP_FEATURE_SLCDC_HAS_8_TIME_SLICE                   (1U)
#define BSP_FEATURE_SLCDC_HAS_INTERNAL_VOLT_GEN              (1U)
#define BSP_FEATURE_SLCDC_MAX_NUM_SEG                        (38U)

#define BSP_FEATURE_SPI_CLK                                  (FSP_PRIV_CLOCK_PCLKA)
#define BSP_FEATURE_SPI_HAS_SPCR3                            (0U)
#define BSP_FEATURE_SPI_HAS_SSL_LEVEL_KEEP                   (0U)
#define BSP_FEATURE_SPI_SSL_LEVEL_KEEP_VALID_CHANNEL_MASK    (0x0U)
#define BSP_FEATURE_SPI_MAX_CHANNEL                          (2U)

#define BSP_FEATURE_SRAM_SRAMWTSC_WAIT_CYCLE_ENABLE          (0) // Feature not available on this MCU

#define BSP_FEATURE_SSI_FIFO_NUM_STAGES                      (8U)
#define BSP_FEATURE_SSI_VALID_CHANNEL_MASK                   (1U)

#define BSP_FEATURE_TFU_SPPORTED                             (0U) // Trigonometric Function Unit (TFU) not available on this MCU

#define BSP_FEATURE_TRNG_HAS_MODULE_STOP                     (0U)

#define BSP_FEATURE_TZ_HAS_TRUSTZONE                         (0U)

#define BSP_FEATURE_BSP_NUM_PMSAR                            (0U)

#endif
