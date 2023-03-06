/* ########################################################################## */
/* - File: esp_host_queues.cpp
   - Copyright (c): 2022 Arduino srl.
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

#include "esp_hosted_platform.h"

/* -------- Memory ---------- */
void* hosted_malloc(size_t size) {
   return malloc(size);
}

void* hosted_calloc(size_t blk_no, size_t size) {
   void* ptr = malloc(blk_no*size);
   if (!ptr) {
      return NULL;
   }

   memset(ptr, 0, blk_no*size);
   return ptr;
}

void hosted_free(void* ptr) {
   if(ptr) {
      free(ptr);
      ptr=NULL;
   }
}

void *hosted_realloc(void *mem, size_t newsize) {
   void *p = NULL;

   if (newsize == 0) {
      mem_free(mem);
      return NULL;
   }

   p = hosted_malloc(newsize);
   if (p) {
      /* zero the memory */
      if (mem != NULL) {
         memcpy(p, mem, newsize);
         mem_free(mem);
      }
   }
   return p;
}
