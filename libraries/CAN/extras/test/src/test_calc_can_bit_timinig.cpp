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
   INCLUDE
 **************************************************************************************/

#include <catch2/catch.hpp>

#include "CanUtil.h"

/**************************************************************************************
   TEST CODE
 **************************************************************************************/

SCENARIO("calc_can_bit_timing", "[calc_can_bit_timing]")
{
  GIVEN("Santiago")
  {
  static uint32_t const F_CAN_CLK_Hz = 20*1000*1000UL;
  static uint32_t const CAN_TIME_QUANTA_per_Bit = 32; /* TQ */

  WHEN("CanBitRate::BR_125k")
  {
    auto [is_valid_baudrate, baud_rate_prescaler, time_segment_1, time_segment_2, synchronization_jump_width] =
      util::calc_can_bit_timing(CanBitRate::BR_125k, F_CAN_CLK_Hz, CAN_TIME_QUANTA_per_Bit);
    THEN("")
    {
      REQUIRE(is_valid_baudrate          == true);
      REQUIRE(baud_rate_prescaler        == 5);
      REQUIRE(time_segment_1             == 23);
      REQUIRE(time_segment_2             == 8);
      REQUIRE(synchronization_jump_width == 1);
    };
  }
  WHEN("CanBitRate::BR_250k")
  {
    auto [is_valid_baudrate, baud_rate_prescaler, time_segment_1, time_segment_2, synchronization_jump_width] =
      util::calc_can_bit_timing(CanBitRate::BR_250k, F_CAN_CLK_Hz, CAN_TIME_QUANTA_per_Bit);
    THEN("")
    {
      REQUIRE(is_valid_baudrate          == true);
      REQUIRE(baud_rate_prescaler        == 4);
      REQUIRE(time_segment_1             == 14);
      REQUIRE(time_segment_2             == 5);
      REQUIRE(synchronization_jump_width == 1);
    };
  }
  WHEN("CanBitRate::BR_500k")
  {
    auto [is_valid_baudrate, baud_rate_prescaler, time_segment_1, time_segment_2, synchronization_jump_width] =
      util::calc_can_bit_timing(CanBitRate::BR_500k, F_CAN_CLK_Hz, CAN_TIME_QUANTA_per_Bit);
    THEN("")
    {
      REQUIRE(is_valid_baudrate          == true);
      REQUIRE(baud_rate_prescaler        == 2);
      REQUIRE(time_segment_1             == 14);
      REQUIRE(time_segment_2             == 5);
      REQUIRE(synchronization_jump_width == 1);
    };
  }
  WHEN("CanBitRate::BR_1000k")
  {
    auto [is_valid_baudrate, baud_rate_prescaler, time_segment_1, time_segment_2, synchronization_jump_width] =
      util::calc_can_bit_timing(CanBitRate::BR_1000k, F_CAN_CLK_Hz, CAN_TIME_QUANTA_per_Bit);
    THEN("")
    {
      REQUIRE(is_valid_baudrate          == true);
      REQUIRE(baud_rate_prescaler        == 1);
      REQUIRE(time_segment_1             == 14);
      REQUIRE(time_segment_2             == 5);
      REQUIRE(synchronization_jump_width == 1);
    };
    }
  }

  GIVEN("Portenta H33")
  {
    static uint32_t const F_CAN_CLK_Hz = 24*1000*1000UL;
    static uint32_t const CAN_TIME_QUANTA_per_Bit = 32; /* TQ */

    WHEN("CanBitRate::BR_125k")
    {
      auto [is_valid_baudrate, baud_rate_prescaler, time_segment_1, time_segment_2, synchronization_jump_width] =
        util::calc_can_bit_timing(CanBitRate::BR_125k, F_CAN_CLK_Hz, CAN_TIME_QUANTA_per_Bit);
      THEN("")
      {
        REQUIRE(is_valid_baudrate          == true);
        REQUIRE(baud_rate_prescaler        == 6);
        REQUIRE(time_segment_1             == 23);
        REQUIRE(time_segment_2             == 8);
        REQUIRE(synchronization_jump_width == 1);
      };
    }
    WHEN("CanBitRate::BR_250k")
    {
      auto [is_valid_baudrate, baud_rate_prescaler, time_segment_1, time_segment_2, synchronization_jump_width] =
        util::calc_can_bit_timing(CanBitRate::BR_250k, F_CAN_CLK_Hz, CAN_TIME_QUANTA_per_Bit);
      THEN("")
      {
        REQUIRE(is_valid_baudrate          == true);
        REQUIRE(baud_rate_prescaler        == 3);
        REQUIRE(time_segment_1             == 23);
        REQUIRE(time_segment_2             == 8);
        REQUIRE(synchronization_jump_width == 1);
      };
    }
    WHEN("CanBitRate::BR_500k")
    {
      auto [is_valid_baudrate, baud_rate_prescaler, time_segment_1, time_segment_2, synchronization_jump_width] =
        util::calc_can_bit_timing(CanBitRate::BR_500k, F_CAN_CLK_Hz, CAN_TIME_QUANTA_per_Bit);
      THEN("")
      {
        REQUIRE(is_valid_baudrate          == true);
        REQUIRE(baud_rate_prescaler        == 2);
        REQUIRE(time_segment_1             == 17);
        REQUIRE(time_segment_2             == 6);
        REQUIRE(synchronization_jump_width == 1);
      };
    }
    WHEN("CanBitRate::BR_1000k")
    {
      auto [is_valid_baudrate, baud_rate_prescaler, time_segment_1, time_segment_2, synchronization_jump_width] =
        util::calc_can_bit_timing(CanBitRate::BR_1000k, F_CAN_CLK_Hz, CAN_TIME_QUANTA_per_Bit);
      THEN("")
      {
        REQUIRE(is_valid_baudrate          == true);
        REQUIRE(baud_rate_prescaler        == 1);
        REQUIRE(time_segment_1             == 17);
        REQUIRE(time_segment_2             == 6);
        REQUIRE(synchronization_jump_width == 1);
        };
      }
  }
}
