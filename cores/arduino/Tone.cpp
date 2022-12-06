#include "Arduino.h"
#include "FspTimer.h"

void tone_timer_callback(timer_callback_args_t *args);

class Tone {

    uint32_t           limit = UINT32_MAX;
    uint8_t            status = LOW;
    static FspTimer    tone_timer;
    static int         channel;

public:
    uint32_t           frequency;
    uint32_t           duration;
    pin_size_t         pin;

    Tone(pin_size_t pin, unsigned int frequency, unsigned long duration) : frequency(frequency), duration(duration), pin(pin)  {
        pinMode(pin, OUTPUT);
        if (frequency) {
            timer_config(500000 / frequency);
        }
    }

    ~Tone() {
        stop();
    }

    void start(void) {
        if ((frequency != 0) && (channel != -1)) {
            tone_timer.start();
        }
        if (duration != 0) {
            limit = millis() + duration;
        }
    }

    void toggle() {
    	digitalWrite(pin, status);
        status = !!!status;
        if (millis() > limit) {
            stop();
        }
    }

    void stop(void) {
        if ((frequency != 0) && (channel != -1)) {
            tone_timer.stop();
        }
        digitalWrite(pin, LOW);
    }

    static void timer_config(uint32_t period_us) {
        // Configure and enable the tone timer.
        uint8_t type = 0;
        if (tone_timer.is_opened()) {
            tone_timer.set_frequency(1000000.0f/period_us);
        } else {
            channel = FspTimer::get_available_timer(type);
            if (channel != -1) {
                tone_timer.begin(TIMER_MODE_PERIODIC, type, channel,
                        1000000.0f/period_us, 50.0f, tone_timer_callback, nullptr);
                tone_timer.setup_overflow_irq();
                tone_timer.open();
                tone_timer.stop();
            }
        }
    }
};

FspTimer Tone::tone_timer;
int Tone::channel = -1;
static Tone* active_tone = NULL;

void tone_timer_callback(timer_callback_args_t __attribute__((unused)) *args) {
    active_tone->toggle();
}

void tone(pin_size_t pin, unsigned int frequency, unsigned long duration) {
	if (active_tone) {
		if (active_tone->pin == pin && active_tone->frequency == frequency && active_tone->duration == 0) {
			// infinite duration notes do not need to be restarted
			return;
		}
		delete active_tone;
	}
	Tone* t = new Tone(pin, frequency, duration);
	active_tone = t;
	t->start();
};

void noTone(pin_size_t __attribute__((unused)) pin) {
	if (active_tone) {
		active_tone->stop();
		delete active_tone;
		active_tone = NULL;
	}
};
