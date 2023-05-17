CFLAGS += -Wall
CFLAGS += -Wextra -c -w -Os --specs=nano.specs -g0 -fno-use-cxa-atexit -fno-rtti -fno-exceptions -MMD -nostdlib -std=gnu++17 -mcpu=cortex-m33 -mfloat-abi=hard -mfpu=fpv5-sp-d16 -fsigned-char -ffunction-sections -fdata-sections -fmessage-length=0 -fno-builtin -mthumb
CFLAGS += -I ./ -I ../ -I ../mbedtls/include -I ../mbedtls/library -I ../../../libraries/SE05X/src -I ../../../libraries/SE05X/src/lib/apdu -DMBEDTLS_CONFIG_FILE='<config-c33.h>'

SOURCES := ecdsa_o.c ecdsa_se05x.c se05x_mbedtls.c
OBJECTS := $(SOURCES:%.c=%.o)
DEPS := $(SOURCES:%.c=%.d)

libmbedse05x.a: $(OBJECTS)
	$(AR) -src $@ $^

clean:
	rm -f libmbedse05x.a $(OBJECTS) $(DEPS)
