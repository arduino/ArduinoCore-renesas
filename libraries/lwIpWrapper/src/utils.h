#pragma once
#include <Arduino.h>
#include "lwip/include/lwip/ip_addr.h"

inline ip_addr_t fromArduinoIP(const IPAddress& ip) {
#if LWIP_IPV4
    ip_addr_t res;
    if(ip.type() == arduino::IPv4) {
        if(ip == INADDR_NONE) {
            ip_addr_copy(res, *IP4_ADDR_ANY);
        } else {
            IP_ADDR4(&res, ip[0], ip[1], ip[2], ip[3]);
        }
    }
#endif // LWIP_IPV4
#if LWIP_IPV4 && LWIP_IPV6
    else
#endif // LWIP_IPV4 && LWIP_IPV6
#if LWIP_IPV6 // TODO change the setting and try ipv6: This is currently set to 0
    if(ip.type() == arduino::IPv6) {
        if(ip == INADDR_NONE) {
            // ip_addr_copy(res, *IP6_ADDR_ANY);
            // FIXME implement this
        } else {
            // FIXME implement this, it could be useful to have a function in the IPAddress class to help this out
        }
    }
#endif // LWIP_IPV6
    return res;
}

inline IPAddress toArduinoIP(const ip_addr_t* ip) {
    if(ip == nullptr) {
        return INADDR_NONE;
    }

#if LWIP_IPV4
    if(IP_IS_V4(ip)) {
        if(ip_addr_isany_val(*ip)) {
            return INADDR_NONE;
        } else {
            return IPAddress(arduino::IPv4, (uint8_t*)&ip_2_ip4(ip)->addr);
        }
    }
#endif // LWIP_IPV4

#if LWIP_IPV6 // TODO change the setting and try ipv6: This is currently set to 0
    if(IP_IS_V6(ip)) {
        if(ip_addr_isany_val(*ip)) {
            return IN6ADDR_ANY;
        } else {
            return IPAddress(arduino::IPv6, (uint8_t*)ip_2_ip6(ip)->addr);
        }
    }
#endif

#if LWIP_IPV4 && LWIP_IPV6
    if(IP_IS_ANY_TYPE_VAL(ip)) {
        // FIXME understand what this means
    }
#endif

    return INADDR_NONE;
}


namespace arduino {
    // TODO leverage on RAII
    inline volatile uint32_t lock_counter;
    inline void lock() {
        __disable_irq();
        lock_counter++; // This action breaks everything
    }

    inline void unlock() {
        if(lock_counter > 0) {
            lock_counter--;
        }

        if(lock_counter == 0) {
            __enable_irq(); // this could be called multiple times if the calls are not setup properly
        }
    }
}