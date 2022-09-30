/*
  Copyright (c) 2012 Arduino.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "Arduino.h"
#include "IRQManager.h"
#include "bsp_api.h"
#include "hal_data.h"

#if EXT_INTERRUPTS_HOWMANY > 0

extern const PinMuxCfg_t g_pin_cfg[];
extern const size_t g_pin_cfg_size;

#define MAX_IRQ_CHANNEL (15)

using IrqFuncVoid_f          = void (*)();
using IrqFuncParam_f         = void (*)(void *);

class CIrq {
    public:
    CIrq() {cfg.irq = FSP_INVALID_VECTOR; }
    volatile IrqFuncVoid_f fnc_void = nullptr;
    volatile IrqFuncParam_f fnc_param = nullptr;
    icu_instance_ctrl_t ctrl;
    external_irq_cfg_t cfg;
};

class IrqPool {
  public:
  IrqPool() {}
  ~IrqPool() {
    for(int i = 0; i < MAX_IRQ_CHANNEL; i++) {
      if(irqs[i] != nullptr){
        delete irqs[i];
        irqs[i] = nullptr;
      }
    }
  }
  CIrq *get(int index, bool make) { 
    if(index < MAX_IRQ_CHANNEL) {
        if(irqs[index] == nullptr && make) {
            irqs[index] = new CIrq{};
        }
        return irqs[index]; 
    }
    else {
      return nullptr;
    }
  }
  
  private:
  CIrq* irqs[MAX_IRQ_CHANNEL] = {nullptr};

};

static IrqPool IrqChannel{};

int  digitalPinToInterrupt(int pin) { return pin; }

/* -------------------------------------------------------------------------- */
static int pin2IrqChannel(int pin) {
/* -------------------------------------------------------------------------- */  
  /* verify index are good */
  if(pin < 0 || pin >= (g_pin_cfg_size / sizeof(g_pin_cfg[0]))) {
    return -1;
  }
  /* getting configuration from table */
  const uint16_t *cfg = g_pin_cfg[pin].list;
  uint16_t cfg_irq = getPinCfg(cfg, PIN_CFG_REQ_INTERRUPT,false);
  
  /* verify configuration are good */
  if(cfg_irq == 0) {
    return -1;
  }
 
  return GET_CHANNEL(cfg_irq);
}

/* -------------------------------------------------------------------------- */
static void IrqCallback(external_irq_callback_args_t * p_args) {
/* -------------------------------------------------------------------------- */    
    void * param = (void *)p_args->p_context;
    uint32_t ch = p_args->channel;

    CIrq *irq_contest = nullptr;

    if(ch >= 0 && ch < MAX_IRQ_CHANNEL) {
        irq_contest = IrqChannel.get(ch,false);
   
        if(irq_contest != nullptr) {
            R_BSP_IrqClearPending(irq_contest->cfg.irq);
            
            if(irq_contest->fnc_param != nullptr) {
                irq_contest->fnc_param(param);
            }
            else if(irq_contest->fnc_void != nullptr) {
                irq_contest->fnc_void();
            }
        }
    }
    
}

/* -------------------------------------------------------------------------- */
void detachInterrupt(pin_size_t pinNumber) {
/* -------------------------------------------------------------------------- */    

    CIrq *irq_contest = nullptr;
    int ch = pin2IrqChannel(pinNumber);
    
    if(ch >= 0 && ch < MAX_IRQ_CHANNEL) {
        irq_contest = IrqChannel.get(ch,false);
    }

    if(irq_contest != nullptr) {
        irq_contest->fnc_param      = nullptr;
        irq_contest->fnc_void       = nullptr;
        R_ICU_ExternalIrqDisable(&(irq_contest->ctrl));
        R_ICU_ExternalIrqClose(&(irq_contest->ctrl));
    } 
}

/* -------------------------------------------------------------------------- */
void attachInterruptParam(pin_size_t pinNumber, voidFuncPtrParam func, PinStatus mode, void* param) {
/* -------------------------------------------------------------------------- */    

    CIrq *irq_contest = nullptr;
    int ch = pin2IrqChannel(pinNumber);
    
    if(ch >= 0 && ch < MAX_IRQ_CHANNEL) {
        irq_contest = IrqChannel.get(ch,true);
    }

    if(irq_contest != nullptr) {
    
        irq_contest->cfg.channel            = ch;
        irq_contest->cfg.pclk_div           = EXTERNAL_IRQ_PCLK_DIV_BY_64;
        irq_contest->cfg.filter_enable      = false;
        irq_contest->cfg.p_callback         = IrqCallback;
        irq_contest->cfg.p_context          = param;
        irq_contest->cfg.p_extend           = nullptr;
        if(param != nullptr) {
            irq_contest->fnc_param      = func;
        }
        else {
            irq_contest->fnc_void       = (voidFuncPtr)func;
        }
    
        uint32_t pullup_enabled = IOPORT_CFG_PULLUP_ENABLE; 

        switch (mode) {
            case LOW:
                irq_contest->cfg.trigger = EXTERNAL_IRQ_TRIG_LEVEL_LOW;
            break;
            case FALLING:
                irq_contest->cfg.trigger = EXTERNAL_IRQ_TRIG_FALLING;
            break;
            case RISING:
                irq_contest->cfg.trigger = EXTERNAL_IRQ_TRIG_RISING;
                pullup_enabled = 0;
            break;
            case CHANGE:
                irq_contest->cfg.trigger = EXTERNAL_IRQ_TRIG_BOTH_EDGE;
            break;
        }
    
        if(IRQManager::getInstance().addPeripheral(IRQ_EXTERNAL_PIN,&(irq_contest->cfg))) {
            /* Configure PIN */
            R_IOPORT_PinCfg(&g_ioport_ctrl, g_pin_cfg[pinNumber].pin, (uint32_t) (IOPORT_CFG_IRQ_ENABLE | IOPORT_CFG_PORT_DIRECTION_INPUT ));
            /* Enable Interrupt */ 
            R_ICU_ExternalIrqOpen(&(irq_contest->ctrl), &(irq_contest->cfg));
            R_ICU_ExternalIrqEnable(&(irq_contest->ctrl));
        }

    }
}

void attachInterrupt(pin_size_t pinNumber, voidFuncPtr func, PinStatus mode) {
    attachInterruptParam(pinNumber, (voidFuncPtrParam)func, mode, NULL);
}

#endif