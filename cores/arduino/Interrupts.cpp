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

static volatile voidFuncPtrParam intFunc[EXT_INTERRUPTS_HOWMANY];

void detachInterrupt(pin_size_t pinNumber) {
    if (digitalPinToInterruptPin(pinNumber) != NOT_AN_INTERRUPT) {
        R_ICU_ExternalIrqDisable(irqTable[digitalPinToInterruptPin(pinNumber)].icu_ctrl);
        R_ICU_ExternalIrqClose(irqTable[digitalPinToInterruptPin(pinNumber)].icu_ctrl);
        intFunc[digitalPinToInterruptPin(pinNumber)] = nullptr;
    }
}


void attachInterruptParam(pin_size_t pinNumber, voidFuncPtrParam func, PinStatus mode, void* param) {
    intFunc[digitalPinToInterruptPin(pinNumber)] = func;

    uint32_t pullup_enabled = IOPORT_CFG_PULLUP_ENABLE;

    external_irq_cfg_t cfg = *irqTable[digitalPinToInterruptPin(pinNumber)].irq_cfg;
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
        pullup_enabled = 0;
        break;
    case CHANGE:
        cfg.trigger = EXTERNAL_IRQ_TRIG_BOTH_EDGE;
        break;
    }

    pinPeripheral(digitalPinToBspPin(pinNumber), IOPORT_CFG_IRQ_ENABLE | pullup_enabled);

    // Enable the interrupt.
    R_ICU_ExternalIrqOpen(irqTable[digitalPinToInterruptPin(pinNumber)].icu_ctrl, &cfg);
    R_ICU_ExternalIrqEnable(irqTable[digitalPinToInterruptPin(pinNumber)].icu_ctrl);
}

void attachInterrupt(pin_size_t pinNumber, voidFuncPtr func, PinStatus mode) {
    if (digitalPinToInterruptPin(pinNumber) != NOT_AN_INTERRUPT) {
        attachInterruptParam(pinNumber, (voidFuncPtrParam)func, mode, NULL);
    }
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
