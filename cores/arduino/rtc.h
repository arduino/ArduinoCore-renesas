#ifndef ARDUINO_RTC_H
#define ARDUINO_RTC_H

#include <ctime>

struct timeval {
	time_t		tv_sec;		
	suseconds_t	tv_usec;	
};

#ifdef __cplusplus
extern "C" {
#endif

void set_time(time_t t);
void attach_rtc(time_t (*read_rtc)(void), void (*write_rtc)(time_t), void (*init_rtc)(void), int (*isenabled_rtc)(void));
int gettimeofday(struct timeval *tv, void *tz);
int settimeofday(const struct timeval *tv, const struct timezone *tz);

#ifdef __cplusplus
}
#endif

#endif