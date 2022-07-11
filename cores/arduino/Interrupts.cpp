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

#include "bsp_api.h"
#include "hal_data.h"

typedef struct {
    icu_instance_ctrl_t* icu_ctrl;
    const external_irq_cfg_t* irq_cfg;
} irqTable_t;

static irqTable_t irqTable[] = {
    {&g_external_irq0_ctrl, &g_external_irq0_cfg}, // ext_int0
    {&g_external_irq1_ctrl, &g_external_irq1_cfg}, // ext_int1
#if EXT_INTERRUPTS_HOWMANY > 2
    {&g_external_irq2_ctrl, &g_external_irq2_cfg}, // ext_int2
    {&g_external_irq3_ctrl, &g_external_irq3_cfg}, // ext_int3
    {&g_external_irq4_ctrl, &g_external_irq4_cfg}, // ext_int4
    {&g_external_irq5_ctrl, &g_external_irq5_cfg}, // ext_int5
    {&g_external_irq6_ctrl, &g_external_irq6_cfg}, // ext_int6
    {&g_external_irq7_ctrl, &g_external_irq7_cfg}, // ext_int7
    {&g_external_irq8_ctrl, &g_external_irq8_cfg}, // ext_int8
    {&g_external_irq9_ctrl, &g_external_irq9_cfg}, // ext_int9
#endif
    {NULL, NULL}, // not implemented
};

static volatile voidFuncPtrParam intFunc[EXT_INTERRUPTS_HOWMANY];

void detachInterrupt(pin_size_t interruptNum) {
  pin_size_t irq_idx = interruptNum - IRQ0;
  if(irq_idx < EXT_INTERRUPTS_HOWMANY) {
    R_ICU_ExternalIrqDisable(irqTable[irq_idx].icu_ctrl);
    R_ICU_ExternalIrqClose(irqTable[irq_idx].icu_ctrl);
    intFunc[irq_idx] = nullptr;
  }
}


void attachInterruptParam(pin_size_t interruptNum, voidFuncPtrParam func, PinStatus mode, void* param) {
  if (interruptNum >= PINS_COUNT) {
    return;
  }

  pin_size_t irq_idx = interruptNum - IRQ0;
  if(irq_idx < EXT_INTERRUPTS_HOWMANY) {
    intFunc[irq_idx] = func;

    external_irq_cfg_t cfg = *irqTable[irq_idx].irq_cfg;
    switch (mode)
    {
    case LOW:
        cfg.trigger = EXTERNAL_IRQ_TRIG_LEVEL_LOW;
        break;
    case FALLING:
        cfg.trigger = EXTERNAL_IRQ_TRIG_FALLING;
        break;
    case RISING:
        cfg.trigger = EXTERNAL_IRQ_TRIG_RISING;
        break;
    case CHANGE:
        cfg.trigger = EXTERNAL_IRQ_TRIG_BOTH_EDGE;
        break;
    }

    // Enable the interrupt.
    R_ICU_ExternalIrqOpen(irqTable[irq_idx].icu_ctrl, &cfg);
    R_ICU_ExternalIrqEnable(irqTable[irq_idx].icu_ctrl);
  }
}

void attachInterrupt(pin_size_t interruptNum, voidFuncPtr func, PinStatus mode) {
  attachInterruptParam(interruptNum, (voidFuncPtrParam)func, mode, NULL);
}

void isr_irq0(external_irq_callback_args_t *p_args){
    if (intFunc[0] != NULL)
    {
        intFunc[0](p_args);
    }
}

void isr_irq1(external_irq_callback_args_t *p_args){
    if (intFunc[1] != NULL)
    {
        intFunc[1](p_args);
    }
}

void isr_irq2(external_irq_callback_args_t *p_args){
    if (intFunc[2] != NULL)
    {
        intFunc[2](p_args);
    }
}

void isr_irq3(external_irq_callback_args_t *p_args){
    if (intFunc[3] != NULL)
    {
        intFunc[3](p_args);
    }
}

void isr_irq4(external_irq_callback_args_t *p_args){
    if (intFunc[4] != NULL)
    {
        intFunc[4](p_args);
    }
}

void isr_irq5(external_irq_callback_args_t *p_args){
    if (intFunc[5] != NULL)
    {
        intFunc[5](p_args);
    }
}

void isr_irq6(external_irq_callback_args_t *p_args){
    if (intFunc[6] != NULL)
    {
        intFunc[6](p_args);
    }
}

void isr_irq7(external_irq_callback_args_t *p_args){
    if (intFunc[7] != NULL)
    {
        intFunc[7](p_args);
    }
}

void isr_irq8(external_irq_callback_args_t *p_args){
    if (intFunc[8] != NULL)
    {
        intFunc[8](p_args);
    }
}

void isr_irq9(external_irq_callback_args_t *p_args){
    if (intFunc[9] != NULL)
    {
        intFunc[9](p_args);
    }
}
