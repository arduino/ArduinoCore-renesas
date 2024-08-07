#ifndef ARDUINO_REAL_TIME_CLOCK
#define ARDUINO_REAL_TIME_CLOCK

#include <ctime>
#include "r_rtc_api.h"
#include <api/String.h>

struct timeval {
	time_t		tv_sec;
	useconds_t	tv_usec;
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


using stime_t = struct tm;
using rtc_cbk_t = void (*)();

enum class Month : uint8_t {
    JANUARY = 0,
    FEBRUARY,
    MARCH,
    APRIL,
    MAY,
    JUNE,
    JULY,
    AUGUST,
    SEPTEMBER,
    OCTOBER,
    NOVEMBER,
    DECEMBER
};

int Month2int(Month m);


enum class DayOfWeek : uint8_t {
    MONDAY = 1,
    TUESDAY = 2,
    WEDNESDAY = 3,
    THURSDAY = 4,
    FRIDAY = 5,
    SATURDAY = 6,
    SUNDAY = 0
};

int DayOfWeek2int(DayOfWeek dow, bool sunday_first);

enum class SaveLight : uint8_t {
    SAVING_TIME_INACTIVE = 0,
    SAVING_TIME_ACTIVE

};

bool SaveLigth2bool(SaveLight sl);

class RTCTime {
    private:
    stime_t stime;

    int day;
    Month month;
    int year;
    int hours;
    int minutes;
    int seconds;
    DayOfWeek day_of_week;
    SaveLight save_light;

    public:
    RTCTime();
    RTCTime(time_t t);
    RTCTime(struct tm &t);
    RTCTime(int _day, Month _m, int _year, int _hours, int _minutes, int _seconds, DayOfWeek _dof, SaveLight _sl);
    ~RTCTime();

    /* setters */
    bool setDayOfMonth(int day); /* day from 1 to 31 */
    bool setMonthOfYear(Month m); /* month from 1 (January) to 12 (December) */
    bool setYear(int year); /* the year 1989 or 2022 */
    bool setHour(int hour); /* from 0 (midnight) to 23 */
    bool setMinute(int minute); /* from 0 to 59 */
    bool setSecond(int second); /* from 0 to 59 */
    bool setDayOfWeek(DayOfWeek d);
    bool setSaveLight(SaveLight sl);
    bool setUnixTime(time_t time);
    void setTM(struct tm &t);

    /* Getters */
    int getDayOfMonth() const;
    Month getMonth() const;
    int getYear() const;
    int getHour() const;
    int getMinutes() const;
    int getSeconds() const;
    DayOfWeek getDayOfWeek() const;
    time_t getUnixTime();
    struct tm getTmTime();

    /**
     * @brief Returns the ISO 8601 string representation of the date and time.
     *
     * @return String The date and time in the format YYYY-MM-DDTHH:MM:SS.
     */
    arduino::String toString() const;

    /**
     * @brief Returns the ISO 8601 string representation of the date and time
     * by calling the toString() method.
     * @return String The date and time in the format YYYY-MM-DDTHH:MM:SS.
     */
    operator arduino::String() const;
};

enum class Period {
    ONCE_EVERY_2_SEC,
    ONCE_EVERY_1_SEC,
    N2_TIMES_EVERY_SEC,
    N4_TIMES_EVERY_SEC,
    N8_TIMES_EVERY_SEC,
    N16_TIMES_EVERY_SEC,
    N32_TIMES_EVERY_SEC,
    N64_TIMES_EVERY_SEC,
    N128_TIMES_EVERY_SEC,
    N256_TIMES_EVERY_SEC
};

class AlarmMatch {
    private:
    unsigned int match;

    public:
    AlarmMatch();
    ~AlarmMatch();
    void addMatchSecond();
    void addMatchMinute();
    void addMatchHour();
    void addMatchDay();
    void addMatchMonth();
    void addMatchYear();
    void addMatchDayOfWeek();
    void removeMatchSecond();
    void removeMatchMinute();
    void removeMatchHour();
    void removeMatchDay();
    void removeMatchMonth();
    void removeMatchYear();
    void removeMatchDayOfWeek();
    bool isMatchingSecond();
    bool isMatchingMinute();
    bool isMatchingHour();
    bool isMatchingDay();
    bool isMatchingMonth();
    bool isMatchingYear();
    bool isMatchingDayOfWeek();

};

class RTClock {
    private:
    bool is_initialized;

    public:
    RTClock();
    ~RTClock();

    bool begin();

    bool getTime(RTCTime &t);

    bool setPeriodicCallback(rtc_cbk_t fnc, Period p);
    bool setAlarmCallback(rtc_cbk_t fnc, RTCTime &t, AlarmMatch &m);
    bool setAlarm(RTCTime &t, AlarmMatch &m);

    bool isRunning();
    bool setTime(RTCTime &t);
    bool setTimeIfNotRunning(RTCTime &t);

};

extern RTClock RTC;

#endif
