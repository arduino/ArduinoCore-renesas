#include "RTClock.h"
#include <Arduino.h>

/* -------------------------------------------------------------------------- */
/*                     HELPER CLASS AlarmMatch                                */
/* -------------------------------------------------------------------------- */

/* ------------------------------ DEFINES ----------------------------------- */
#define MATCH_SECOND_POS    (0)
#define MATCH_MINUTE_POS    (1)
#define MATCH_HOUR_POS      (2)
#define MATCH_DAY_POS       (3)
#define MATCH_MONTH_POS     (4)
#define MATCH_YEAR_POS      (5)
#define MATCH_DAYOFWEEK_POS (6)
/* no more than 32 */

#define IS_MATCH_SET(reg,pos)  ((reg & (1<<pos)) >> pos)  
#define SET_MATCH(reg,pos)     reg |= (1<<pos)
#define RESET_MATCH(reg,pos)   reg &= ~(1<<pos)

/* -------------------------------------------------------------------------- */
AlarmMatch::AlarmMatch() : match{0} { }
AlarmMatch::~AlarmMatch() {}    

inline void AlarmMatch::addMatchSecond()    { SET_MATCH(match,MATCH_SECOND_POS); }
inline void AlarmMatch::addMatchMinute()    { SET_MATCH(match,MATCH_MINUTE_POS); }
inline void AlarmMatch::addMatchHour()      { SET_MATCH(match,MATCH_HOUR_POS); }
inline void AlarmMatch::addMatchDay()       { SET_MATCH(match,MATCH_DAY_POS); }
inline void AlarmMatch::addMatchMonth()     { SET_MATCH(match,MATCH_MONTH_POS); }
inline void AlarmMatch::addMatchYear()      { SET_MATCH(match,MATCH_YEAR_POS); }
inline void AlarmMatch::addMatchDayOfWeek() { SET_MATCH(match,MATCH_DAYOFWEEK_POS); }

inline void AlarmMatch::removeMatchSecond()    { RESET_MATCH(match,MATCH_SECOND_POS); }
inline void AlarmMatch::removeMatchMinute()    { RESET_MATCH(match,MATCH_MINUTE_POS); }
inline void AlarmMatch::removeMatchHour()      { RESET_MATCH(match,MATCH_HOUR_POS); }
inline void AlarmMatch::removeMatchDay()       { RESET_MATCH(match,MATCH_DAY_POS); }
inline void AlarmMatch::removeMatchMonth()     { RESET_MATCH(match,MATCH_MONTH_POS); }
inline void AlarmMatch::removeMatchYear()      { RESET_MATCH(match,MATCH_YEAR_POS); }
inline void AlarmMatch::removeMatchDayOfWeek() { RESET_MATCH(match,MATCH_DAYOFWEEK_POS); }

inline bool AlarmMatch::isMatchingSecond()     { (bool)IS_MATCH_SET(match,MATCH_SECOND_POS); }
inline bool AlarmMatch::isMatchingMinute()     { (bool)IS_MATCH_SET(match,MATCH_MINUTE_POS);}
inline bool AlarmMatch::isMatchingHour()       { (bool)IS_MATCH_SET(match,MATCH_HOUR_POS);}
inline bool AlarmMatch::isMatchingDay()        { (bool)IS_MATCH_SET(match,MATCH_DAY_POS);}
inline bool AlarmMatch::isMatchingMonth()      { (bool)IS_MATCH_SET(match,MATCH_MONTH_POS);}
inline bool AlarmMatch::isMatchingYear()       { (bool)IS_MATCH_SET(match,MATCH_YEAR_POS);}
inline bool AlarmMatch::isMatchingDayOfWeek()  { (bool)IS_MATCH_SET(match,MATCH_DAYOFWEEK_POS);}
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/*                        HELPER CLASS RTCTime                                */
/* -------------------------------------------------------------------------- */

#define MIN_DAY_OF_MONTH (1)
#define MAX_DAY_OF_MONTH (31)
#define MIN_HOURS        (0)
#define MAX_HOURS        (23)
#define MIN_MINUTES      (0)
#define MAX_MINUTES      (59)
#define MIN_SECONDS      (0)
#define MAX_SECONDS      (59)

#define TM_YEAR_OFFSET   (1900)

#define NO_SAVE_LIGHT_ACTIVE (0)
#define SAVE_LIGHT_ACTIVE (1)


int isleap(int year)
{
    return year % 4 == 0 && (year % 100 != 0 || year % 400 == 0);
}

int yday(int year, int month)
{
    static const short int upto[12] =
        {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};
    int yd;

    yd = upto[month - 1];
    if (month > 2 && isleap(year))
        yd++;
    return yd;
}

int Month2tm(Month m) {
    switch(m) {
        case Month::JAN: return 0; 
        case Month::FEB: return 1; 
        case Month::MAR: return 2; 
        case Month::APR: return 3; 
        case Month::MAY: return 4; 
        case Month::JUN: return 5; 
        case Month::JUL: return 6; 
        case Month::AUG: return 7; 
        case Month::SEP: return 8; 
        case Month::OCT: return 9; 
        case Month::NOV: return 10;
        case Month::DEC: return 11; 
        default:         return 0;    
    }
}

int SaveLight2tm(SaveLight sl) {
    int rv = (sl == SaveLight::SAVING_TIME_INACTIVE) ? NO_SAVE_LIGHT_ACTIVE : SAVE_LIGHT_ACTIVE;
}

int DayOfWeek2tm(DayOfWeek dow) {
    switch(dow) {
        case DayOfWeek::SUNDAY:    return 0;
        case DayOfWeek::MONDAY:    return 1;
        case DayOfWeek::TUESDAY:   return 2;
        case DayOfWeek::WEDNESDAY: return 3;
        case DayOfWeek::THURSDAY:  return 4;
        case DayOfWeek::FRIDAY:    return 5;
        case DayOfWeek::SATURDAY:  return 6;
        default:                   return 0;   
    }
    return 0;
}

Month tm2Month(struct tm &t) {
    switch(t.tm_mon) {
        case 0: return Month::JAN;
        case 1: return Month::FEB;
        case 2: return Month::MAR;
        case 3: return Month::APR;
        case 4: return Month::MAY;
        case 5: return Month::JUN;
        case 6: return Month::JUL;
        case 7: return Month::AUG;
        case 8: return Month::SEP;
        case 9: return Month::OCT;
        case 10: return Month::NOV;
        case 11: return Month::DEC;
        default: return Month::JAN;
    }
    return Month::JAN;
}

DayOfWeek tm2DayOfWeek(struct tm &t) {
    switch(t.tm_wday) {
        case 0: return DayOfWeek::SUNDAY;
        case 1: return DayOfWeek::MONDAY;
        case 2: return DayOfWeek::TUESDAY;
        case 3: return DayOfWeek::WEDNESDAY;
        case 4: return DayOfWeek::THURSDAY;
        case 5: return DayOfWeek::FRIDAY;
        case 6: return DayOfWeek::SATURDAY;
        default: return DayOfWeek::SUNDAY;
    }
    return DayOfWeek::SUNDAY;
}

SaveLight tm2SaveLight(struct tm &t) {
    return (t.tm_isdst == 0) ?  SaveLight::SAVING_TIME_INACTIVE : SaveLight::SAVING_TIME_ACTIVE;
}

RTCTime::RTCTime(struct tm &t)  {
    setTM(t);
}

RTCTime::RTCTime() : day{1}, 
                     month{Month::JAN}, 
                     year{2000}, 
                     hours{0}, 
                     minutes{0}, 
                     seconds{0}, 
                     day_of_week{DayOfWeek::SATURDAY}, 
                     save_light{SaveLight::SAVING_TIME_INACTIVE} { }

RTCTime::RTCTime(int _day, 
                 Month _m, 
                 int _year, 
                 int _hours, 
                 int _minutes, 
                 int _seconds, 
                 DayOfWeek _dof, 
                 SaveLight _sl) : RTCTime() {
    setDayOfMonth(_day);
    setMonthOfYear(_m);
    setYear(_year);
    setHour(_hours);
    setMinute(_minutes);
    setSecond(_seconds);
    setDayOfWeek(_dof);
    setSaveLight(_sl);
}
RTCTime::~RTCTime() { }
    
/* setters */
void RTCTime::setTM(struct tm &t) {
    setDayOfMonth(t.tm_mday);
    setMonthOfYear(tm2Month(t));
    setYear(t.tm_year);
    setHour(t.tm_hous);
    setMinute(t.tm_min);
    setSecond(t.tm_sec);
    setDayOfWeek(tm2DayOfWeek(t));
    setSaveLight(tm2SaveLight(t));

}

 

rtc_periodic_irq_select_t Periodic2RtcPeriodic(Periodic p) {
    switch(p) {
        case Periodic::ONCE_EVERY_2_SEC: return RTC_PERIODIC_IRQ_SELECT_2_SECOND;
        case Periodic::ONCE_EVERY_1_SEC: return RTC_PERIODIC_IRQ_SELECT_1_SECOND;
        case Periodic::N2_TIMES_EVERY_SEC: return RTC_PERIODIC_IRQ_SELECT_1_DIV_BY_2_SECOND;
        case Periodic::N4_TIMES_EVERY_SEC: return RTC_PERIODIC_IRQ_SELECT_1_DIV_BY_4_SECOND;
        case Periodic::N8_TIMES_EVERY_SEC: return RTC_PERIODIC_IRQ_SELECT_1_DIV_BY_8_SECOND;
        case Periodic::N16_TIMES_EVERY_SEC: return RTC_PERIODIC_IRQ_SELECT_1_DIV_BY_16_SECOND;
        case Periodic::N32_TIMES_EVERY_SEC: return RTC_PERIODIC_IRQ_SELECT_1_DIV_BY_32_SECOND;
        case Periodic::N64_TIMES_EVERY_SEC: return RTC_PERIODIC_IRQ_SELECT_1_DIV_BY_64_SECOND;
        case Periodic::N128_TIMES_EVERY_SEC: return RTC_PERIODIC_IRQ_SELECT_1_DIV_BY_128_SECOND;
        case Periodic::N256_TIMES_EVERY_SEC: return RTC_PERIODIC_IRQ_SELECT_1_DIV_BY_256_SECOND;
        default: return RTC_PERIODIC_IRQ_SELECT_1_SECOND; 
    }
    return RTC_PERIODIC_IRQ_SELECT_1_SECOND;
}




bool RTCTime::setDayOfMonth(int _d) {
    bool rv = false;
    if(_d >= MIN_DAY_OF_MONTH && _d <= MAX_DAY_OF_MONTH) {
        day = _d;
        rv = true;
        stime.tm_mday = day;
        stime.tm_yday = day + yday(year, Month2tm(month));
    }
    return rv;
}

bool RTCTime::setMonthOfYear(Month _m) {
    month = _m;
    stime.tm_mon = Month2tm(month);
    stime.tm_yday = day + yday(year, Month2tm(month));
    return true;
}

bool RTCTime::setYear(int _y) {
    year = _y;
    stime.tm_year = year - TM_YEAR_OFFSET;
    stime.tm_yday = day + yday(year, Month2tm(month));
    return true;
}
bool RTCTime::setHour(int _h) {
    bool rv = false;
    if(_h >= MIN_HOURS && _h <= MAX_HOURS) {
        hours = _h;
        rv = true;
        stime.tm_hour = hours;
    }
    return rv;
}

bool RTCTime::setMinute(int _m) {
    bool rv = false;
    if(_m >= MIN_MINUTES && _m <= MAX_MINUTES) {
        minutes = _m;
        rv = true;
        stime.tm_min = minutes;
    }
    return rv;
}

bool RTCTime::setSecond(int _s) {
    bool rv = false;
    if(_s >= MIN_SECONDS && _s <= MAX_SECONDS) {
        seconds = _s;
        rv = true;
        stime.tm_sec = seconds;
    }
    return rv;
}
bool RTCTime::setDayOfWeek(DayOfWeek d) {
    day_of_week = d;
    stime.tm_wday = DayOfWeek2tm(day_of_week);
    return true;
}

bool RTCTime::setSaveLight(SaveLight sl) {
    save_light = sl;
    stime.tm_isdst = SaveLight2tm(save_light);
    return true;
}

/* getters */
int RTCTime::getDayOfMonth()      { return day; }
Month RTCTime::getMont()          { return month; }
int RTCTime::getYear()            { return year; }
int RTCTime::getHour()            { return hours; }
int RTCTime::getMinutes()         { return minutes; }
int RTCTime::getSeconds()         { return seconds; }
DayOfWeek RTCTime::getDayOfWeek() { return day_of_week; }

time_t RTCTime::getUnixTime()  { return mktime ( (struct tm *)&stime );}
struct tm RTCTime::getTmTime() { return (struct tm)stime; }

/* -------------------------------------------------------------------------- */
/*                             RTClass                                        */
/* -------------------------------------------------------------------------- */

RTClock::RTClock() : is_initalized{false} { }
RTClock::~RTClock() { }

bool RTClock::begin() {
    if(openRtc()) {
        is_initalized = true;
    }
    else {
        is_initalized = false;
    }
}

bool RTClock::getTime(RTCTime &t) {
    struct tm present;
    if(is_initialized) {
        if( getRtcTime(present) ) {
            t.setTM(present);
            return true;
        }
    }
    return false
}

bool RTClock::setPeriodicCallback(rtc_cbk_t fnc, Period p) {
    onRtcInterrupt();
    setRtcPeriodicClbk(fnc);
    return setRtcPeriodicInterrupt(Periodic2RtcPeriodic(p)); 
}

bool RTClock::setAlarmCallback(rtc_cbk_t fnc, RTCTime &t, AlarmMatch &m) {
    onRtcInterrupt();
    setRtcAlarmClbk(fnc);
    rtc_alarm_time_t at;
    at.min_match = false;               
    at.sec_match = false;                
    at.hour_match = false;               
    at.mday_match = false;              
    at.mon_match = false;                
    at.year_match = false;               
    at.dayofweek_match = false; 

    struct tm alrm = RTCTime.getTmTime();
    
    memcpy(&at.time, &alrm, sizeof(struct tm));
    if(isMatchingSecond()) {
        at.sec_match = true;
    }
    else if(isMatchingMinute()) {
        at.min_match = true;
    }
    else if(isMatchingHour() ) {
        at.hour_match = true;
    }
    else if(isMatchingDay() ) {
        at.mday_match = true;
    }
    else if(isMatchingMonth()) {
        at.mon_match = true;
    }
    else if(isMatchingYear()) {
        at.year_match = true;
    }
    else if(isMatchingDayOfWeek()) {
        at.dayofweek_match = true;
    }
    
    return setRtcAlarm(at);
}

bool RTClock::isRunning() {
    return isRtcRunning();
}

bool RTClock::setTime(RTCTime &t) {
    if(is_initialized) {
        if(setRtcTime((rtc_time_t)t.getTmTime())) {
            return true;
        }
    }
    return false
}

bool RTClock::setTimeIfNotRunning(RTCTime &t) {
    if(!isRunning()) {
        return setTime(t);
    }
    return false;
}

#if RTC_HOWMANY > 0
RTClock RTC{};
#endif


