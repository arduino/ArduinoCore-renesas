/*
 * Copyright 2010-2014,2018-2019,2022 NXP
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef PHESETYPES_H
#define PHESETYPES_H
#include <stdint.h>
#include <stdlib.h>

typedef uint8_t utf8_t;     /* UTF8 Character String */
typedef uint8_t bool_t;     /* boolean data type */
typedef uint16_t ESESTATUS; /* Return values */

#define ENABLE_T1oI2C_LOGS 0

#if ENABLE_T1oI2C_LOGS
#define T_SMLOG_I(...) SMLOG_I(__VA_ARGS__)
#define T_SMLOG_E(...) SMLOG_E(__VA_ARGS__)
#define T_SMLOG_W(...) SMLOG_W(__VA_ARGS__)
#define T_SMLOG_D(...) SMLOG_D(__VA_ARGS__)
#define T_SMLOG_AU8_D(BUF, LEN) SMLOG_AU8_D(BUF, LEN)
#define T_SMLOG_MAU8_D(MSG, BUF, LEN) SMLOG_MAU8_D(MSG, BUF, LEN)
#else
#define T_SMLOG_I(...)
#define T_SMLOG_E(...)
#define T_SMLOG_W(...)
#define T_SMLOG_D(...)
#define T_SMLOG_AU8_D(BUF, LEN)
#define T_SMLOG_MAU8_D(MSG, BUF, LEN)
#endif //#if ENABLE_T1oI2C_LOGS

#endif /* PHESETYPES_H */
