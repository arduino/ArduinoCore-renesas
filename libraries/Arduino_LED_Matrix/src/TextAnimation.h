#pragma once

#include "Arduino.h"

#if not __has_include("ArduinoGraphics.h")
#error "TextAnimation work only when ArduinoGraphics is installed and used. Include ArduinoGraphics first."
#endif

#define TEXT_ANIMATION_DECLARE(NAME, MAX_CHARS) \
	extern uint32_t NAME ## _buf[MAX_CHARS][4]; \
	extern uint32_t NAME ## _buf_used; 

#define TEXT_ANIMATION_DEFINE(NAME, MAX_CHARS) \
	uint32_t NAME ## _buf[MAX_CHARS][4]; \
	uint32_t NAME ## _buf_used = 0; 