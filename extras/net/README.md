
# lwIP library for Arduino Renesas Core

## Build

### Export environmental variables
```bash
export CC=/opt/gcc-arm-none-eabi-9-2019-q4-major/bin/arm-none-eabi-gcc 
export AR=/opt/gcc-arm-none-eabi-9-2019-q4-major/bin/arm-none-eabi-ar
```

### Generate libraries
```bash
make clean
make
```

### Move libraries inside SSLClient 
```bash
cp liblwIP.a ../../libraries/lwIpWrapper/src/cortex-m33/
```
