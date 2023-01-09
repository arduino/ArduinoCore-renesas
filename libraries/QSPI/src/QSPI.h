#ifndef __QSPI_H__
#define __QSPI_H__

#include "Arduino.h"
extern "C" {
#include "r_qspi.h"
}

/* QSPI flash page number to be written */
#define PAGE_FIRST                      (0U)

/* QSPI flash page Size */
#define PAGE_WRITE_SIZE                 (256U)

/* QSPI flash address through page*/
#define QSPI_FLASH_ADDRESS(page_no)     (uint8_t *) (QSPI_DEVICE_START_ADDRESS + (page_no * PAGE_WRITE_SIZE))

/* Status register pay-load */
#define STATUS_REG_PAYLOAD              {0x01,0x40,0x00}

/* data written to status register */
#define SET_SREG_VALUE                  (0x40)

/* sector size of QSPI flash device */
#define SECTOR_SIZE                     (4096U)

/* one byte data transfer */
#define ONE_BYTE                        (0x01)

/* SREG pay-load size */
#define SREG_SIZE                       (0x03)

/* default memory value */
#define DEFAULT_MEM_VAL                 (0xFF)

class QSPIClass {
public:
    QSPIClass(pin_size_t clock, pin_size_t cs, pin_size_t io0, pin_size_t io1, pin_size_t io2, pin_size_t io3) :
        clock(clock), cs(cs), io0(io0), io1(io1), io2(io2), io3(io3) {
            g_qspi_extended_cfg.min_qssl_deselect_cycles = QSPI_QSSL_MIN_HIGH_LEVEL_4_QSPCLK;
            g_qspi_extended_cfg.qspclk_div = QSPI_QSPCLK_DIV_2;

            g_qspi_cfg.spi_protocol = SPI_FLASH_PROTOCOL_EXTENDED_SPI;
            g_qspi_cfg.read_mode = SPI_FLASH_READ_MODE_FAST_READ_QUAD_IO;
            g_qspi_cfg.address_bytes = SPI_FLASH_ADDRESS_BYTES_3;
            g_qspi_cfg.dummy_clocks = SPI_FLASH_DUMMY_CLOCKS_DEFAULT;

            g_qspi_cfg.page_program_address_lines = SPI_FLASH_DATA_LINES_1;
            g_qspi_cfg.page_size_bytes = 256;
            g_qspi_cfg.write_status_bit = 0;
            g_qspi_cfg.page_program_command = 0x02;
            g_qspi_cfg.write_enable_command = 0x06;
            g_qspi_cfg.status_command = 0x05;
            //g_qspi_cfg.read_command = 
            g_qspi_cfg.xip_enter_command = 0x20;
            g_qspi_cfg.xip_exit_command = 0xFF;
            g_qspi_cfg.erase_command_list_length = sizeof(g_qspi_erase_command_list) / sizeof(g_qspi_erase_command_list[0]);
            g_qspi_cfg.p_erase_command_list = &g_qspi_erase_command_list[0];
            g_qspi_cfg.p_extend = &g_qspi_extended_cfg;
        }

    bool begin() {
        R_IOPORT_PinCfg(NULL, g_pin_cfg[clock].pin, (uint32_t) (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_QSPI));
        R_IOPORT_PinCfg(NULL, g_pin_cfg[cs].pin, (uint32_t) (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_QSPI));
        R_IOPORT_PinCfg(NULL, g_pin_cfg[io0].pin, (uint32_t) (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_QSPI));
        R_IOPORT_PinCfg(NULL, g_pin_cfg[io1].pin, (uint32_t) (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_QSPI));
        R_IOPORT_PinCfg(NULL, g_pin_cfg[io2].pin, (uint32_t) (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_QSPI));
        R_IOPORT_PinCfg(NULL, g_pin_cfg[io3].pin, (uint32_t) (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_QSPI));
    
        uint8_t data_sreg[SREG_SIZE] = STATUS_REG_PAYLOAD;
        uint8_t sreg_data = 0;

        if (R_QSPI_Open(&g_qspi_ctrl, &g_qspi_cfg))
            goto deinit;
        /* write enable for further operations */
        if (R_QSPI_DirectWrite(&g_qspi_ctrl, &(g_qspi_cfg.write_enable_command), ONE_BYTE, false))
            goto deinit;
        if (get_flash_status())
            goto deinit;

        if (R_QSPI_DirectWrite(&g_qspi_ctrl, data_sreg, SREG_SIZE, false))
            goto deinit;
        if (get_flash_status())
            goto deinit;

        if (R_QSPI_DirectWrite(&g_qspi_ctrl, &(g_qspi_cfg.status_command), ONE_BYTE, true))
            goto deinit;
        if (R_QSPI_DirectRead(&g_qspi_ctrl, &sreg_data, ONE_BYTE))
            goto deinit;

        if (SET_SREG_VALUE != sreg_data) {
            goto deinit;
        }
        return true;
    deinit:
        deinit();
        return false;
    }

    int write(size_t address, uint8_t* data, size_t len) {
        //len = ((len/PAGE_WRITE_SIZE)+1) * PAGE_WRITE_SIZE;
        size_t written = 0;
        while (written < len) {
            R_QSPI_Write(&g_qspi_ctrl, data, (uint8_t *)(QSPI_DEVICE_START_ADDRESS + address + written), ((len - written) > PAGE_WRITE_SIZE ? PAGE_WRITE_SIZE : len - written));
            get_flash_status();
            written += PAGE_WRITE_SIZE;
        }
    }
    int read(size_t address, uint8_t* data, size_t len) {
        memcpy(data,  (uint8_t *)(QSPI_DEVICE_START_ADDRESS + address), len);
        return len;
    }
    int erase(size_t address, size_t len) {
        if (len < SECTOR_SIZE) {
            return -1;
        }
        R_QSPI_Erase(&g_qspi_ctrl, (uint8_t *)(QSPI_DEVICE_START_ADDRESS + address), len);
        get_flash_status();
    }

private:
    void deinit() {
        R_QSPI_Close(&g_qspi_ctrl);
    }

    fsp_err_t get_flash_status() {
        spi_flash_status_t status = {.write_in_progress = true};
        int32_t time_out          = (INT32_MAX);
        fsp_err_t err             = FSP_SUCCESS;

        do
        {
            /* Get status from QSPI flash device */
            err = R_QSPI_StatusGet(&g_qspi_ctrl, &status);
            if (FSP_SUCCESS != err)
            {
                return err;
            }

            /* Decrement time out to avoid infinite loop in case of consistent failure */
            --time_out;

            if (time_out <= 0)
            {
                return FSP_ERR_TIMEOUT;
            }

        } while (false != status.write_in_progress);

        return err;
    }

private:

    pin_size_t clock;
    pin_size_t cs;
    pin_size_t io0;
    pin_size_t io1;
    pin_size_t io2;
    pin_size_t io3;

    const uint8_t *start_address = (uint8_t *)QSPI_DEVICE_START_ADDRESS;

    qspi_instance_ctrl_t g_qspi_ctrl;
    spi_flash_cfg_t g_qspi_cfg;
    qspi_extended_cfg_t g_qspi_extended_cfg;

    spi_flash_erase_command_t g_qspi_erase_command_list[4] =
    {
        { .command = 0x20, .size = 4096 },
        { .command = 0x52, .size = 32768 },
        { .command = 0xD8, .size = 65536 },
        { .command = 0xC7, .size = SPI_FLASH_ERASE_SIZE_CHIP_ERASE },
    };
};

#endif //__QSPI_H__