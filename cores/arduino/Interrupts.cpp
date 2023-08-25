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

#define MAX_IRQ_CHANNEL (15)

using IrqFuncVoid_f          = void (*)();
using IrqFuncParam_f         = void (*)(void *);

class CIrq {
    public:
    CIrq() { cfg.irq = FSP_INVALID_VECTOR; }
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

pin_size_t digitalPinToInterrupt(pin_size_t pin) { return pin; }

/* -------------------------------------------------------------------------- */
static int pin2IrqChannel(int pin) {
/* -------------------------------------------------------------------------- */  
  /* verify index are good */
  if(pin < 0 || pin >= PINS_COUNT) {
    return -1;
  }
  /* getting configuration from table */
  auto cfg_irq = getPinCfgs(pin, PIN_CFG_REQ_INTERRUPT);
  
  /* verify configuration are good */
  if(cfg_irq[0] == 0) {
    return -1;
  }
 
  return GET_CHANNEL(cfg_irq[0]);
}

/* -------------------------------------------------------------------------- */
static void IrqCallback(external_irq_callback_args_t * p_args) {
/* -------------------------------------------------------------------------- */    
    void * param = (void *)p_args->p_context;
    uint32_t ch = p_args->channel;

    CIrq *irq_context = nullptr;

    if(ch < MAX_IRQ_CHANNEL) {
        irq_context = IrqChannel.get(ch,false);
   
        if(irq_context != nullptr) {
            R_BSP_IrqClearPending(irq_context->cfg.irq);
            
            if(irq_context->fnc_param != nullptr) {
                irq_context->fnc_param(param);
            }
            else if(irq_context->fnc_void != nullptr) {
                irq_context->fnc_void();
            }
        }
    }
    
}

/* -------------------------------------------------------------------------- */
void detachInterrupt(pin_size_t pinNumber) {
/* -------------------------------------------------------------------------- */    

    CIrq *irq_context = nullptr;
    int ch = pin2IrqChannel(pinNumber);
    
    if(ch >= 0 && ch < MAX_IRQ_CHANNEL) {
        irq_context = IrqChannel.get(ch,false);
    }

    if(irq_context != nullptr) {
        irq_context->fnc_param      = nullptr;
        irq_context->fnc_void       = nullptr;
        R_ICU_ExternalIrqDisable(&(irq_context->ctrl));
        R_ICU_ExternalIrqClose(&(irq_context->ctrl));
    } 
}

/* -------------------------------------------------------------------------- */
void attachInterruptParam(pin_size_t pinNumber, voidFuncPtrParam func, PinStatus mode, void* param) {
/* -------------------------------------------------------------------------- */    

    CIrq *irq_context = nullptr;
    int ch = pin2IrqChannel(pinNumber);
    
    if(ch >= 0 && ch < MAX_IRQ_CHANNEL) {
        irq_context = IrqChannel.get(ch,true);
    }

    if(irq_context != nullptr) {
    
        irq_context->cfg.channel            = ch;
        irq_context->cfg.pclk_div           = EXTERNAL_IRQ_PCLK_DIV_BY_64;
        irq_context->cfg.filter_enable      = false;
        irq_context->cfg.p_callback         = IrqCallback;
        irq_context->cfg.p_context          = param;
        irq_context->cfg.p_extend           = nullptr;
        if(param != nullptr) {
            irq_context->fnc_param      = func;
        }
        else {
            irq_context->fnc_void       = (voidFuncPtr)func;
        }
    
        switch (mode) {
            case LOW:
                irq_context->cfg.trigger = EXTERNAL_IRQ_TRIG_LEVEL_LOW;
            break;
            case FALLING:
                irq_context->cfg.trigger = EXTERNAL_IRQ_TRIG_FALLING;
            break;
            case RISING:
            case HIGH:  // TODO: document that HIGH IRQ is unavailable
                irq_context->cfg.trigger = EXTERNAL_IRQ_TRIG_RISING;
            break;
            case CHANGE:
                irq_context->cfg.trigger = EXTERNAL_IRQ_TRIG_BOTH_EDGE;
            break;
        }
    
        if(IRQManager::getInstance().addPeripheral(IRQ_EXTERNAL_PIN,&(irq_context->cfg))) {
            /*
            Configure PIN
            */
            bool has_pullup = R_PFS->PORT[g_pin_cfg[pinNumber].pin >> IOPORT_PRV_PORT_OFFSET].PIN[g_pin_cfg[pinNumber].pin & BSP_IO_PRV_8BIT_MASK].PmnPFS & IOPORT_CFG_PULLUP_ENABLE;
            R_IOPORT_PinCfg(&g_ioport_ctrl, g_pin_cfg[pinNumber].pin, (uint32_t) (IOPORT_CFG_IRQ_ENABLE | IOPORT_CFG_PORT_DIRECTION_INPUT | (has_pullup ? IOPORT_CFG_PULLUP_ENABLE : 0)));
            /* Enable Interrupt */ 
            R_ICU_ExternalIrqOpen(&(irq_context->ctrl), &(irq_context->cfg));
            R_ICU_ExternalIrqEnable(&(irq_context->ctrl));
        }

    }
}

void attachInterrupt(pin_size_t pinNumber, voidFuncPtr func, PinStatus mode) {
    attachInterruptParam(pinNumber, (voidFuncPtrParam)func, mode, NULL);
}

int attachIrq2Link(uint32_t pinNumber, PinStatus mode) {
    CIrq *irq_context = nullptr;
    int ch = pin2IrqChannel(pinNumber);
    
    if(ch >= 0 && ch < MAX_IRQ_CHANNEL) {
        irq_context = IrqChannel.get(ch,true);
    }

    if(irq_context != nullptr) {
        irq_context->cfg.channel            = ch;
        irq_context->cfg.pclk_div           = EXTERNAL_IRQ_PCLK_DIV_BY_64;
        irq_context->cfg.filter_enable      = false;
        irq_context->cfg.p_callback         = IrqCallback;
        irq_context->cfg.p_context          = nullptr;
        irq_context->cfg.p_extend           = nullptr;
        /* ATTACH CALLBACK FUNCTION might be useful ??? */
    
        switch (mode) {
            case LOW:
                irq_context->cfg.trigger = EXTERNAL_IRQ_TRIG_LEVEL_LOW;
            break;
            case FALLING:
                irq_context->cfg.trigger = EXTERNAL_IRQ_TRIG_FALLING;
            break;
            case RISING:
            case HIGH:  // TODO: document that HIGH IRQ is unavailable
                irq_context->cfg.trigger = EXTERNAL_IRQ_TRIG_RISING;
            break;
            case CHANGE:
                irq_context->cfg.trigger = EXTERNAL_IRQ_TRIG_BOTH_EDGE;
            break;
        }
    
        if(IRQManager::getInstance().addPeripheral(IRQ_EXTERNAL_PIN,&(irq_context->cfg))) {
            /*
            Configure PIN
            TODO: decide if a default pull should be provided or if it's up to the user to call pinMode before activating the IRQ
            */
            R_IOPORT_PinCfg(&g_ioport_ctrl, g_pin_cfg[pinNumber].pin, (uint32_t) (IOPORT_CFG_IRQ_ENABLE | IOPORT_CFG_PORT_DIRECTION_INPUT ));
            /* Enable Interrupt */ 
            R_ICU_ExternalIrqOpen(&(irq_context->ctrl), &(irq_context->cfg));
            R_ICU_ExternalIrqEnable(&(irq_context->ctrl));
        }

        return irq_context->cfg.channel;
    }
    return -1;

}


/* -------------------------------------------------------------------------- */
int detachIrq2Link(pin_size_t pinNumber) {
/* -------------------------------------------------------------------------- */    

    CIrq *irq_context = nullptr;
    int ch = pin2IrqChannel(pinNumber);
    
    if(ch >= 0 && ch < MAX_IRQ_CHANNEL) {
        irq_context = IrqChannel.get(ch,false);
    }

    if(irq_context != nullptr) {
        irq_context->fnc_param      = nullptr;
        irq_context->fnc_void       = nullptr;
        R_ICU_ExternalIrqDisable(&(irq_context->ctrl));
        R_ICU_ExternalIrqClose(&(irq_context->ctrl));
        return ch;
    } 
    return -1;
}


#endif