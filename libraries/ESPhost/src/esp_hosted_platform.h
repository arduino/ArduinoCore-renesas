/* ########################################################################## */
/* - File: esp_host_platform.h
   - Copyright (c): 2023 Arduino srl.
   - Author: Daniele Aimo (d.aimo@arduino.cc)

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
     Foundation, Inc.,51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA */
/* ########################################################################## */


#ifndef ESP_HOST_PLATFORM_H
#define ESP_HOST_PLATFORM_H

#include <cstdlib>
#include <string.h>

#define command_log(...) 
#define min(X, Y)                    (((X) < (Y)) ? (X) : (Y))

#define mem_free(x)                            \
{                                              \
    if (x) {                                   \
        hosted_free(x);                        \
        x = NULL;                              \
    }                                          \
}
/*
 * hosted_malloc function allocates size bytes.
 * Input parameter
 *      size    :   Number of Bytes
 * Returns
 *      pointer to allocated memory
 */
void* hosted_malloc(size_t size);

/*
 * hosted_calloc function allocates memory for an array
 * of nmemb elements of size bytes each.
 * Input parameter
 *      size    :   Number of Bytes
 *      nmemb   :   Number of blocks of size bytes
 * Returns
 *     pointer to allocated memory
 */
void* hosted_calloc(size_t blk_no, size_t size);

/*
 * hosted_free function frees the memory space pointed to by ptr.
 * Input parameter
 *      ptr     :   Address of pointer to allocated memory
 */

void hosted_free(void* ptr);

void *hosted_realloc(void *mem, size_t newsize);



#endif