
# mbedTLS library for Arduino Renesas Core

## Build

### Export environmental variables
```bash
export CC=/opt/gcc-arm-none-eabi-9-2019-q4-major/bin/arm-none-eabi-gcc 
export AR=/opt/gcc-arm-none-eabi-9-2019-q4-major/bin/arm-none-eabi-ar
```

### Generate libraries
```bash
CFLAGS="-I$PWD/../ -DMBEDTLS_CONFIG_FILE='<config-c33.h>' -Wall -Wextra -Os -g0 --specs=nano.specs -nostdlib -std=gnu11 -mcpu=cortex-m33 -mfloat-abi=hard -mfpu=fpv5-sp-d16 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fmessage-length=0 -fno-builtin -mthumb" make lib
```

### Move libraries inside SSLClient 
```bash
cp library/*.a ../../../libraries/SSLClient/src/cortex-m33/
```

# mbedTLS SE050 library for Arduino Renesas Core

## Build

### Export environmental variables
```bash
export CC=/opt/gcc-arm-none-eabi-9-2019-q4-major/bin/arm-none-eabi-g++
export AR=/opt/gcc-arm-none-eabi-9-2019-q4-major/bin/arm-none-eabi-ar
```

### Generate libraries
```bash
make
```

### Move libraries inside SSLClient
```bash
cp libmbedse05x.a ../../../libraries/SSLClient/src/cortex-m33/
```
