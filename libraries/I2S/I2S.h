#include "r_i2s_api.h"
#include "r_ssi.h"
#include "pwm.h"

class I2SClass {
public:
    I2SClass() {}
    bool begin();
    bool start() {}
    bool stop() {}
    bool available();
    size_t read(int16_t* buf);
    size_t write(int32_t* buf, size_t len) {}
};

extern I2SClass I2S;