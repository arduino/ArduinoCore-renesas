#ifndef ARDUINO_FSP_TRANSFERT_H
#define ARDUINO_FSP_TRANSFERT_H

#include "Arduino.h"
#include "r_elc.h"
#include "r_elc_api.h"


extern int attachIrq2Link(uint32_t pin, PinStatus mode);
extern int detachIrq2Link(pin_size_t pinNumber);

/* Wrapper class for FSP ELC 
    at the present only support the link of an external Irq to a peripheral */

const elc_cfg_t cfg = {ELC_EVENT_NONE};

class FspLinkIrq {
    private:
    elc_event_t getIrqExtEventFromChannel(uint8_t ch) {
        if(ch == 0) {
            return ELC_EVENT_ICU_IRQ0;
        }
        else if(ch == 1) {
            return ELC_EVENT_ICU_IRQ1;
        }
        else if(ch == 2) {
            return ELC_EVENT_ICU_IRQ2;
        }
        else if(ch == 3) {
            return ELC_EVENT_ICU_IRQ3;
        }
        else if(ch == 4) {
            return ELC_EVENT_ICU_IRQ4;
        }
        else if(ch == 5) {
            return ELC_EVENT_ICU_IRQ5;
        }
        else if(ch == 6) {
            return ELC_EVENT_ICU_IRQ6;
        }
        else if(ch == 7) {
            return ELC_EVENT_ICU_IRQ7;
        }
        else if(ch == 8) {
            return ELC_EVENT_ICU_IRQ8;
        }
        else if(ch == 9) {
            return ELC_EVENT_ICU_IRQ9;
        }
        else if(ch == 10) {
            return ELC_EVENT_ICU_IRQ10;
        }
        else if(ch == 11) {
            return ELC_EVENT_ICU_IRQ11;
        }
        else if(ch == 12) {
            return ELC_EVENT_ICU_IRQ12;
        }
        else if(ch == 13) {
            return ELC_EVENT_ICU_IRQ13;
        }
        else if(ch == 14) {
            return ELC_EVENT_ICU_IRQ14;
        }
        else {
            return ELC_EVENT_ICU_IRQ15;
        }
    }    
    
    
    public:
    elc_instance_ctrl_t ctrl;
    //elc_cfg_t cfg;
    ~FspLinkIrq() {
        R_ELC_Disable (&ctrl);
        R_ELC_Close (&ctrl);
    }
    FspLinkIrq() {
        
       

    }
    
     

    bool linkExtIrq(uint32_t pin, PinStatus mode, elc_peripheral_t peripheral) {
        
        
        uint8_t ch = attachIrq2Link((pin_size_t)pin, mode);
        
        if(ch >= 0) {
        
            fsp_err_t err = R_ELC_Open ( &ctrl, &cfg );
            if(err != FSP_SUCCESS && err != FSP_ERR_ALREADY_OPEN) {
                return false;
            }

            R_ELC_Disable (&ctrl);

            if(R_ELC_LinkSet ( &ctrl, peripheral, getIrqExtEventFromChannel(ch)) != FSP_SUCCESS) {
                R_ELC_Enable (&ctrl);
                return false;
            }

            if(R_ELC_Enable (&ctrl) != FSP_SUCCESS) {
                return false;
            }
        }
        return true;
    }

    void unlinkExtIrq(uint32_t pin) {
        uint8_t ch = detachIrq2Link((pin_size_t)pin);
        if(ch >= 0) {
            R_ELC_Disable (&ctrl);
            elc_event_t ev = getIrqExtEventFromChannel(ch);
            for(uint8_t i = 0; i < ELC_PERIPHERAL_NUM; i++) {
                if(cfg.link[i] == ev) {
                    R_ELC_LinkBreak (&ctrl, (elc_peripheral_t)ev);
                    break;
                }

            }
            R_ELC_Enable (&ctrl);
        }
    } 
};



#endif

