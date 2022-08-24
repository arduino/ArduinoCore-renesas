#include "Arduino.h"
#include "rtc.h"

#define RTC_IS_ENABLED      (0)
#define RTC_IS_NOT_ENABLED  (1)


#ifdef __cplusplus
extern "C" {
#endif

static void rtc_init(void) {
  if(!g_rtc0_ctrl.open) {
    R_RTC_Open(&g_rtc0_ctrl, &g_rtc0_cfg);
  }
}

static int rtc_isenabled(void) {
  rtc_info_t rtc_info;
  R_RTC_InfoGet(&g_rtc0_ctrl, &rtc_info);
  if (rtc_info.status == RTC_STATUS_RUNNING) {
    return (int)RTC_IS_ENABLED;
  }
  return (int)RTC_IS_NOT_ENABLED;
}

static time_t rtc_read(void) {
  rtc_time_t time_read;
  rtc_init();
  R_RTC_CalendarTimeGet(&g_rtc0_ctrl, &time_read);
  time_t rv = mktime ( (struct tm *) &time_read);
  return rv;
}

static void rtc_write(time_t t) {
  rtc_init();
  struct tm * timeinfo;
  timeinfo = localtime (&t);
  R_RTC_CalendarTimeSet(&g_rtc0_ctrl, timeinfo);
}


static void (*_rtc_init)(void) = rtc_init;
static int (*_rtc_isenabled)(void) = rtc_isenabled;
static time_t (*_rtc_read)(void) = rtc_read;
static void (*_rtc_write)(time_t t) = rtc_write;


int settimeofday(const struct timeval *tv,  const struct timezone *tz)
{
    if (_rtc_init != NULL) {
        _rtc_init();
    }
    if (_rtc_write != NULL) {
        _rtc_write(tv->tv_sec);
    }
    return 0;
}

int gettimeofday(struct timeval *tv, void *tz)
{
    if (_rtc_isenabled != NULL) {
        if (!(_rtc_isenabled())) {
            set_time(0);
        }
    }

    time_t t = (time_t) - 1;
    if (_rtc_read != NULL) {
        t = _rtc_read();
    }
    
    tv->tv_sec  = t;
    tv->tv_usec = 0;
    return 0;
}

void set_time(time_t t)
{
    const struct timeval tv = { t, 0 };
    settimeofday(&tv, NULL);
}


#ifdef __cplusplus
}
#endif



volatile bool rtc_open = false;

void setRtcTime(rtc_time_t time) {
  time.tm_year -= 1900; // Start date 1900
  if (!rtc_open) {
    R_RTC_Open(&g_rtc0_ctrl, &g_rtc0_cfg);
    rtc_open = true;
  }
  R_RTC_CalendarTimeSet(&g_rtc0_ctrl, &time);
}

rtc_time_t getRtcTime() {
  rtc_time_t time_read;
  if (!rtc_open) {
    R_RTC_Open(&g_rtc0_ctrl, &g_rtc0_cfg);
    rtc_open = true;
  }
  R_RTC_CalendarTimeGet(&g_rtc0_ctrl, &time_read);
  time_read.tm_year += 1900; // Start date 1900
  return time_read;
}

void onRtcInterrupt(void (*func)(rtc_callback_args_t *)) {
  if (!rtc_open) {
    R_RTC_Open(&g_rtc0_ctrl, &g_rtc0_cfg);
    rtc_open = true;
  }
  g_rtc0_ctrl.p_callback = func;
}

void setRtcPeriodicInterrupt(rtc_periodic_irq_select_t period) {
  if (!rtc_open) {
    R_RTC_Open(&g_rtc0_ctrl, &g_rtc0_cfg);
    rtc_open = true;
  }
  R_RTC_PeriodicIrqRateSet(&g_rtc0_ctrl, period);
}

void setRtcAlarm(rtc_time_t time, RtcAlarmSettings time_match) {
  if (!rtc_open) {
    R_RTC_Open(&g_rtc0_ctrl, &g_rtc0_cfg);
    rtc_open = true;
  }
  rtc_alarm_time_t alarm_time;
  alarm_time.time = time;
  memcpy(&alarm_time.sec_match, &time_match, sizeof(RtcAlarmSettings));
  R_RTC_CalendarAlarmSet(&g_rtc0_ctrl, &alarm_time);
}

bool isRtcRunning() {
  rtc_info_t rtc_info;
  R_RTC_InfoGet(&g_rtc0_ctrl, &rtc_info);
  if (rtc_info.status == RTC_STATUS_RUNNING) {
    return true;
  }
  return false;
}

void __attribute__((weak)) rtc_callback(rtc_callback_args_t *p_args) {}