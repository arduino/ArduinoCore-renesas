/*
 * Copyright (c) 2001-2003 Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 * EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 *
 * Author: Adam Dunkels <adam@sics.se>
 *
 */

#ifndef __ARCH_CC_H__
#define __ARCH_CC_H__

#include <stdio.h>
#include <stdlib.h>

#include <lwipopts.h>
/** if you want to use the struct timeval provided
 * by your system, set this to 0 and include <sys/time.h> in cc.h */
#define LWIP_TIMEVAL_PRIVATE 0
//#include <sys/time.h>
#include <time.h>

/* 
 Assuming Arduino boards are little endian
 In case not, add conditional macro to change endianness for lwIP
*/
#ifndef BYTE_ORDER
#define BYTE_ORDER LITTLE_ENDIAN
#endif


#define S16_F "d"
#define U16_F "d"
#define S32_F "d"
#define U32_F "x"

#define X16_F "x"
#define X32_F "x"

/** Define random number generator function of your system */
#define LWIP_RAND rand


/** Define this to 1 in arch/cc.h of your port if your compiler does not provide
 * the inttypes.h header. You need to define the format strings listed in
 * lwip/arch.h yourself in this case (X8_F, U16_F...).
 */
#ifndef LWIP_NO_INTTYPES_H
#define LWIP_NO_INTTYPES_H 0
#endif


/** Platform specific diagnostic output.<br>
 * Note the default implementation pulls in printf, which may
 * in turn pull in a lot of standard library code. In resource-constrained
 * systems, this should be defined to something less resource-consuming.
 */
#if LWIP_DEBUG 
extern void sys_printf(const char *format, ...);
#define LWIP_PLATFORM_ASSERT(x) do	{ \
									sys_printf("Assertion \"%s\" failed at line %d in %s\n", \
                                     x, __LINE__, __FILE__); \
									while(1);\
									}while(0)
                                

#define LWIP_PLATFORM_DIAG(x) do{ \
								sys_printf x;\
								sys_printf("\n");\
								\
								}while(0)
#else
#define LWIP_PLATFORM_ASSERT(x)
#define LWIP_PLATFORM_DIAG(x)                
#endif

#define LWIP_PROVIDE_ERRNO

#endif /* __ARCH_CC_H__ */ 

