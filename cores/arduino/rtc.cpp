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

using rtc_simple_cbk_t = void (*)();

rtc_simple_cbk_t alarm_func = nullptr;
rtc_simple_cbk_t periodic_func = nullptr;

void setRtcPeriodicClbk(rtc_simple_cbk_t f) {
  periodic_func = f;
}
void setRtcAlarmClbk(rtc_simple_cbk_t f) {
  alarm_func = f;
}

void __attribute__((weak)) rtc_callback(rtc_callback_args_t *p_args) {
  if(p_args->event == RTC_EVENT_ALARM_IRQ) {
    if(alarm_func != nullptr) {
      alarm_func();
    }
  }

  if(p_args->event == RTC_EVENT_PERIODIC_IRQ) {
    if(periodic_func != nullptr) {
      periodic_func();
    }
  }
}


bool openRtc() {
  if(!g_rtc0_ctrl.open) {
    if(FSP_SUCCESS == R_RTC_Open(&g_rtc0_ctrl, &g_rtc0_cfg)) {
      return true;
    }
  }
  else {
    return true;
  }
  return false;
}

bool setRtcTime(rtc_time_t time) {
  if(FSP_SUCCESS == R_RTC_CalendarTimeSet(&g_rtc0_ctrl, &time) ) {
    return true;
  }
}

bool isRtcRunning() {
  rtc_info_t rtc_info;
  R_RTC_InfoGet(&g_rtc0_ctrl, &rtc_info);
  if (rtc_info.status == RTC_STATUS_RUNNING) {
    return true;
  }
  return false;
}

bool getRtcTime(struct tm &t) {
  rtc_time_t time_read;
  if(FSP_SUCCESS == R_RTC_CalendarTimeGet(&g_rtc0_ctrl, &time_read)) {
    memcpy(&t,&time_read,sizeof(struct tm));
    return true;
  }
  return false;
}

void onRtcInterrupt() {
  g_rtc0_ctrl.p_callback = rtc_callback;
}

bool setRtcPeriodicInterrupt(rtc_periodic_irq_select_t period) {
  
  if(FSP_SUCCESS == R_RTC_PeriodicIrqRateSet(&g_rtc0_ctrl, period)) {
    return true;
  }
  return false;
}

bool setRtcAlarm(rtc_alarm_time_t alarm_time) {
  if(FSP_SUCCESS == R_RTC_CalendarAlarmSet(&g_rtc0_ctrl, &alarm_time) ) {
    return true;
  }
  return false;
}


