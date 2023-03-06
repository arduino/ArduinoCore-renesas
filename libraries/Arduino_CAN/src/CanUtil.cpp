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

std::tuple<bool, uint32_t, uint32_t, uint32_t>
  calc_can_bit_timing(CanBitRate const can_bitrate,
                      uint32_t const can_clock_Hz,
                      uint32_t const tq_min,
                      uint32_t const tq_max,
                      uint32_t const tseg1_min,
                      uint32_t const tseg1_max,
                      uint32_t const tseg2_min,
                      uint32_t const tseg2_max)
{
  /* Note: Concerning the calculation of
   *   - baud_rate_prescaler
   *   - time_segment_1 (TSEG1)
   *   - time_segment_2 (TSEG2)
   * also compare with section 30.4.3, RA4M1 Group User Manual, Rev. 1.00, October 2019.
   */
  for (uint32_t tq = tq_max; tq >= tq_min; tq--)
  {
    /* Determine the CAN baud rate prescaler. */
    double const brp = static_cast<double>(can_clock_Hz) / (tq * static_cast<double>(can_bitrate));
    /* Extract the sub-comma part of the baud rate prescaler. */
    double brp_ipart = 0.0f;
    double const brp_fract = modf(brp, &brp_ipart);
    /* If the fractional part is sufficiently close to zero, we have
     * found a valid prescaler configuration.
     */
    if (brp_fract < 0.01)
    {
      uint32_t const baud_rate_prescaler = static_cast<uint32_t>(brp_ipart);
      /* Assign TSEG1 and TSEG2 to set the sample point at 75%. */
      uint32_t const time_segment_1 = static_cast<int>(static_cast<float>(tq) * 0.75f) - 1;
      uint32_t const time_segment_2 = tq - time_segment_1 - 1;
      /* Check if the found values are within the allowed boundary. */
      if (time_segment_1 < tseg1_min || time_segment_1 > tseg1_max) continue;
      if (time_segment_2 < tseg2_min || time_segment_2 > tseg2_max) continue;
      /* We've found a valid configuration, exit here. */
      return std::make_tuple(true, baud_rate_prescaler, time_segment_1, time_segment_2);
    }
  }

  return std::make_tuple(false, 0, 0, 0);
}

/**************************************************************************************
 * NAMESPACE
 **************************************************************************************/

} /* util */
