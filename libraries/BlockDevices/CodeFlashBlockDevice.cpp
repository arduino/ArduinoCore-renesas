/*
  CodeFlashBlockDevice.cpp
  Copyright (c) 2023 Arduino SA.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "CodeFlashBlockDevice.h"

// To enable debug set CF_DBG to 1 and make sure STORAGE_DEBUG is defined
// in Storage/storage_common.h
#define CF_DBG     0
#define CF_MEM_DBG 0

CodeFlashBlockDevice::CodeFlashBlockDevice() :
    _cf_address((bd_addr_t)0x00000000),
    _cf_size((bd_size_t)0x00200000),
    _cf_reg0_size((bd_size_t)BSP_FEATURE_FLASH_HP_CF_REGION0_SIZE),
    _cf_reg0_block_size((bd_size_t)BSP_FEATURE_FLASH_HP_CF_REGION0_BLOCK_SIZE),
    _cf_reg1_block_size((bd_size_t)BSP_FEATURE_FLASH_HP_CF_REGION1_BLOCK_SIZE),
    _cf_read_size((bd_size_t)0x1),
    _cf_program_size((bd_size_t)BSP_FEATURE_FLASH_HP_CF_WRITE_SIZE),
    _opened(false)
{
    FLASH_open                = nullptr;
    FLASH_write               = nullptr;
    FLASH_erase               = nullptr;
    FLASH_blank_check         = nullptr;
    FLASH_close               = nullptr;
#ifdef UNUSED_FSP_API_FUNCTIONS
    FLASH_get_info            = nullptr;
    FLASH_get_status          = nullptr;
    FLASH_set_window_access   = nullptr;
    FLASH_clear_window_access = nullptr;
    FLASH_set_code_id         = nullptr;
    FLASH_reset               = nullptr;
    FLASH_update_ck_freq      = nullptr;
    FLASH_select_startup_area = nullptr;
    FLASH_swap_bank           = nullptr;
    FLASH_set_callback        = nullptr;
#endif

#if (ARDUINO_FLASH_TYPE == LP_FLASH)
    FLASH_open                = R_FLASH_LP_Open;
    FLASH_write               = R_FLASH_LP_Write;
    FLASH_erase               = R_FLASH_LP_Erase;
    FLASH_blank_check         = R_FLASH_LP_BlankCheck;
    FLASH_close               = R_FLASH_LP_Close;
#ifdef UNUSED_FSP_API_FUNCTIONS
    FLASH_get_info            = R_FLASH_LP_InfoGet;
    FLASH_get_status          = R_FLASH_LP_StatusGet;
    FLASH_set_window_access   = R_FLASH_LP_AccessWindowSet;
    FLASH_clear_window_access = R_FLASH_LP_AccessWindowClear;
    FLASH_set_code_id         = R_FLASH_LP_IdCodeSet;
    FLASH_reset               = R_FLASH_LP_Reset;
    FLASH_update_ck_freq      = R_FLASH_LP_UpdateFlashClockFreq;
    FLASH_select_startup_area = R_FLASH_LP_StartUpAreaSelect;
    FLASH_swap_bank           = R_FLASH_LP_BankSwap;
    FLASH_set_callback        = R_FLASH_LP_CallbackSet;
#endif
#endif

#if (ARDUINO_FLASH_TYPE == HP_FLASH)
    FLASH_open                = R_FLASH_HP_Open;
    FLASH_write               = R_FLASH_HP_Write;
    FLASH_erase               = R_FLASH_HP_Erase;
    FLASH_blank_check         = R_FLASH_HP_BlankCheck;
    FLASH_close               = R_FLASH_HP_Close;
#ifdef UNUSED_FSP_API_FUNCTIONS
    FLASH_get_info            = R_FLASH_HP_InfoGet;
    FLASH_get_status          = R_FLASH_HP_StatusGet;
    FLASH_set_window_access   = R_FLASH_HP_AccessWindowSet;
    FLASH_clear_window_access = R_FLASH_HP_AccessWindowClear;
    FLASH_set_code_id         = R_FLASH_HP_IdCodeSet;
    FLASH_reset               = R_FLASH_HP_Reset;
    FLASH_update_ck_freq      = R_FLASH_HP_UpdateFlashClockFreq;
    FLASH_select_startup_area = R_FLASH_HP_StartUpAreaSelect;
    FLASH_swap_bank           = R_FLASH_HP_BankSwap;
    FLASH_set_callback        = R_FLASH_HP_CallbackSet;
#endif
#endif

    _cfg.data_flash_bgo         = false;
    _cfg.p_callback             = nullptr;
    _cfg.p_context              = nullptr;
    _cfg.p_extend               = nullptr;
    _cfg.ipl                    = (BSP_IRQ_DISABLED);
    _cfg.irq                    = FSP_INVALID_VECTOR;
    _cfg.err_ipl                = (BSP_IRQ_DISABLED);
    _cfg.err_irq                = FSP_INVALID_VECTOR;
}

CodeFlashBlockDevice& CodeFlashBlockDevice::getInstance()
{
    static CodeFlashBlockDevice instance;
    return instance;
}

CodeFlashBlockDevice::~CodeFlashBlockDevice()
{

}

int CodeFlashBlockDevice::init()
{
    return open();
}

int CodeFlashBlockDevice::deinit()
{
    return close();
}

int CodeFlashBlockDevice::program(const void *buffer, bd_addr_t addr, bd_size_t size)
{
    return write(buffer, addr, size);
}

int CodeFlashBlockDevice::open()
{
    fsp_err_t rv = FSP_ERR_UNSUPPORTED;

    if(_opened) {
        return (int)FSP_SUCCESS;
    }

    if(FLASH_open) {
        rv = FLASH_open(&_ctrl,&_cfg);
        if(rv == FSP_SUCCESS) {
            _opened = true;
        } else {
            debug_if(CF_DBG, "Code flash: open error %d", rv);
        }
    }

    return (int)rv;
}

int CodeFlashBlockDevice::close()
{
    fsp_err_t rv = FSP_ERR_UNSUPPORTED;

    if(!_opened) {
        return (int)FSP_SUCCESS;
    }

    if(FLASH_close) {
        rv = FLASH_close(&_ctrl);
        if(rv == FSP_SUCCESS) {
            _opened = false;
        } else {
            debug_if(CF_DBG, "Code flash: close error %d", rv);
        }
    }

    return (int)rv;
}

int CodeFlashBlockDevice::read(void *buffer, bd_addr_t virtual_address, bd_size_t size)
{
    fsp_err_t rv = FSP_ERR_UNSUPPORTED;

    if(!_opened) {
        debug_if(CF_DBG, "Code flash: read, device not open");
        return (int)FSP_ERR_NOT_OPEN;
    }

    if(!buffer) {
        debug_if(CF_DBG, "Code flash: read, invalid buffer");
        return (int)FSP_ERR_INVALID_POINTER;
    }

    if(!is_valid_read(virtual_address, size)) {
        debug_if(CF_DBG, "Code flash: read, invalid address");
        return (int)FSP_ERR_INVALID_ADDRESS;
    }

    debug_if(CF_DBG, "Code flash: read, address: 0x%x size: %d", _cf_address + virtual_address, size);
    uint8_t *source = (uint8_t *)(_cf_address + virtual_address);
    memcpy(buffer, source, size);

#if CF_MEM_DBG
    debug_if(CF_DBG, "Code flash: read, dump");
    debug_mem((uint8_t*)buffer, size);
#endif

    return (int)rv;
}

int CodeFlashBlockDevice::write(const void *buffer, bd_addr_t virtual_address, bd_size_t size)
{
    fsp_err_t rv = FSP_ERR_UNSUPPORTED;

    if(!_opened) {
        debug_if(CF_DBG, "Code flash: write, device not open");
        return (int)FSP_ERR_NOT_OPEN;
    }

    if(!buffer) {
        debug_if(CF_DBG, "Code flash: write, invalid buffer");
        return (int)FSP_ERR_INVALID_POINTER;
    }

    if((!is_valid_program(virtual_address, size))) {
        debug_if(CF_DBG, "Code flash: write, invalid address");
        return (int)FSP_ERR_INVALID_ADDRESS;
    }

#if CF_MEM_DBG
    debug_if(CF_DBG, "Code flash: write, dump");
    debug_mem((uint8_t*)buffer, size);
#endif

    if(FLASH_write) {
        uint32_t address = _cf_address + virtual_address;
        debug_if(CF_DBG, "Code flash: write, address: 0x%x size: %d", address, size);
        __disable_irq();
        rv = FLASH_write(&_ctrl, (uint32_t)buffer, address, (uint32_t)size);
        __enable_irq();
        if(rv != FSP_SUCCESS) {
            debug_if(CF_DBG, "Code flash: write error %d", rv);
        }
    }

    return (int)rv;
}

uint32_t CodeFlashBlockDevice::get_block_starting_address(bd_addr_t virtual_address)
{
    return virtual_address & ~(get_erase_size(virtual_address) - 1);
}

int CodeFlashBlockDevice::check_blank(bd_addr_t virtual_address)
{
    fsp_err_t rv = FSP_ERR_UNSUPPORTED;
    flash_result_t blank_check_result;

    if(!_opened) {
       return (int)FSP_ERR_NOT_OPEN;
    }

    uint32_t block_base_addr = _cf_address + get_block_starting_address(virtual_address);
    uint32_t block_size = get_erase_size(virtual_address);

    if(FLASH_blank_check) {
       debug_if(CF_DBG, "Code flash: blank, address: 0x%x size: %d", block_base_addr, block_size);
       rv = FLASH_blank_check(&_ctrl, block_base_addr, block_size, &blank_check_result);
    }

    return (rv == FSP_SUCCESS && blank_check_result == FLASH_RESULT_BLANK) ? BLOCK_DEVICE_OK : (int)rv;
}

int CodeFlashBlockDevice::erase_block(bd_addr_t virtual_address)
{
    fsp_err_t rv = FSP_ERR_UNSUPPORTED;

    const uint32_t block_base_addr = _cf_address + get_block_starting_address(virtual_address);

    if(FLASH_erase) {
        debug_if(CF_DBG, "Code flash: erase_block, address: 0x%x num: %d", block_base_addr, 1);
       __disable_irq();
       rv = FLASH_erase(&_ctrl, block_base_addr, 1);
       __enable_irq();
    }

    return (int)rv;
}

int CodeFlashBlockDevice::erase(bd_addr_t virtual_address, bd_size_t size)
{
    fsp_err_t rv = FSP_ERR_UNSUPPORTED;

    if(!_opened) {
        debug_if(CF_DBG, "Code flash: erase, device not open");
        return (int)FSP_ERR_NOT_OPEN;
    }

    if(!is_valid_erase(virtual_address, size)) {
        debug_if(CF_DBG, "Code flash: erase, invalid address");
        return (int)FSP_ERR_INVALID_ADDRESS;
    }

    debug_if(CF_DBG, "Code flash: erase, address: 0x%x size: %d", virtual_address, size);
    uint32_t delete_size = 0;
    do {
        rv = (fsp_err_t)erase_block(virtual_address + delete_size);
        if (rv != FSP_SUCCESS) {
            debug_if(CF_DBG, "Code flash: erase block, error %d", rv);
        }
        delete_size += get_erase_size(virtual_address + delete_size);
        rv = (fsp_err_t)check_blank(virtual_address + delete_size);
        if (rv != FSP_SUCCESS) {
            debug_if(CF_DBG, "Code flash: check blank, error %d", rv);
        }
    } while(rv == FSP_SUCCESS && delete_size < size);

    return (int)rv;
}

bd_size_t CodeFlashBlockDevice::get_program_size() const
{
    return _cf_program_size;;
}

bd_size_t CodeFlashBlockDevice::get_erase_size() const
{
    return _cf_reg0_block_size;
}

bd_size_t CodeFlashBlockDevice::get_erase_size(bd_addr_t addr) const
{
    return addr < _cf_reg0_size ? _cf_reg0_block_size : _cf_reg1_block_size;
}

bd_size_t CodeFlashBlockDevice::get_read_size() const
{
    return _cf_read_size;
}

bd_size_t CodeFlashBlockDevice::size() const
{
    return _cf_size;
}

const char *CodeFlashBlockDevice::get_type() const
{
    return "CODEFLASH";
}

bool CodeFlashBlockDevice::is_valid_program(bd_addr_t addr, bd_size_t size) const
{
    return (addr % get_program_size() == 0 && size % get_program_size() == 0 && addr + size <= this->size());
}

bool CodeFlashBlockDevice::is_valid_erase(bd_addr_t addr, bd_size_t size) const
{
    return (addr % get_erase_size(addr) == 0 && (addr + size) % get_erase_size(addr + size - 1) == 0 && addr + size <= this->size() && addr % get_erase_size(addr) == 0 && size % get_erase_size(addr) == 0);
}
