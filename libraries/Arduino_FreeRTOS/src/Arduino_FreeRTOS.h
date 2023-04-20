#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "FreeRTOSConfig.h"

#include "lib/FreeRTOS-Kernel-v10.5.1/FreeRTOS.h"
#include "lib/FreeRTOS-Kernel-v10.5.1/croutine.h"
#include "lib/FreeRTOS-Kernel-v10.5.1/event_groups.h"
#include "lib/FreeRTOS-Kernel-v10.5.1/queue.h"
#include "lib/FreeRTOS-Kernel-v10.5.1/semphr.h"
#include "lib/FreeRTOS-Kernel-v10.5.1/task.h"
#include "lib/FreeRTOS-Kernel-v10.5.1/timers.h"

#ifdef __cplusplus
}
#endif

// NOTE: memory_regions.ld  SOMETHING FISHY HERE, smaller sizes lead to collissions with vector table. Probably error in vector table length.
