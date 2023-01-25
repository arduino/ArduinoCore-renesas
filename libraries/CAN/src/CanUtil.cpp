/*
 * Copyright (c) 2022 by Alexander Entinger <a.entinger@arduino.cc>
 * CAN library for Arduino.
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of either the GNU General Public License version 2
 * or the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */

/**************************************************************************************
 * INCLUDE
 **************************************************************************************/

#include "CanUtil.h"

#include <math.h> /* modf */

/**************************************************************************************
 * NAMESPACE
 **************************************************************************************/

namespace util
{

/**************************************************************************************
 * FUNCTION DEFINITION
 **************************************************************************************/

std::tuple<bool, uint32_t, uint32_t, uint32_t, uint32_t> calc_can_bit_timing(CanBitRate const can_bitrate, uint32_t const can_clock_Hz, uint32_t const tq_max)
{
  /* Calculate the CAN bitrate based on the value of this functions parameter.
   *
   * Note: Concerning the calculation of
   *   - _canfd_bit_timing_cfg.baud_rate_prescaler
   *   - time_segment_1 (TSEG1)
   *   - time_segment_2 (TSEG2)
   *   - synchronization_jump_width (SJW)
   * also compare with Table 32.14, RA6M5 Group User Manual, Rev. 1.10.
   */
  for (uint32_t tq = tq_max; tq > 0; tq--)
  {
    /* If it has come this far we've failed to find a valid prescaler. */
    if (tq == 0)
      return std::make_tuple(false, 0, 0, 0, 0);

    /* Determine the CAN baud rate prescaler. */
    double const brp = static_cast<double>(can_clock_Hz) / (tq * static_cast<double>(can_bitrate));
    /* Extract the sub-comma part of the baud rate prescaler. */
    double const brp_fract = modf(brp, nullptr);
    /* If the fractional part is sufficiently close to zero, we have
     * found a valid prescaler configuration.
     */
    if (brp_fract < 0.01)
    {
      uint32_t const baud_rate_prescaler = static_cast<uint32_t>(brp);
      /* Assign TSEG1 and TSEG2 to set the sample point at 75%. */
      uint32_t const synchronization_jump_width = 1; /* Is always 1. */
      uint32_t const time_segment_1 = static_cast<uint32_t>(static_cast<float>(tq) * 0.75) - 1;
      uint32_t const time_segment_2 = tq - time_segment_1 - synchronization_jump_width;
      /* We've found a valid configuration, exit here. */
      return std::make_tuple(true, baud_rate_prescaler, time_segment_1, time_segment_2, synchronization_jump_width);
    }
  }

  return std::make_tuple(false, 0, 0, 0, 0);
}

/**************************************************************************************
 * NAMESPACE
 **************************************************************************************/

} /* util */
