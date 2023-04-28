PORT_DIR = arch

CFLAGS += -Wall
CFLAGS += -Wextra -Os -g0 --specs=nano.specs -nostdlib -std=gnu11 -mcpu=cortex-m33 -mfloat-abi=hard -mfpu=fpv5-sp-d16 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fmessage-length=0 -fno-builtin -mthumb
CFLAGS += -I ./ -I lwip/src/include -I src/include/ipv4 -I $(PORT_DIR)

COREFILES := $(wildcard lwip/src/core/*.c)
CORE4FILES := $(wildcard lwip/src/core/ipv4/*.c)
CORE6FILES := $(wildcard lwip/src/core/ipv6/*.c)
NETIFFILES := $(wildcard lwip/src/netif/*.c)
PPPFILES := $(wildcard lwip/src/netif/ppp/*.c)
APIFILES := $(wildcard lwip/src/api/*.c)
TFTPFILES := $(wildcard lwip/src/apps/tftp/*.c)
SNTPFILES := $(wildcard lwip/src/apps/sntp/*.c)
SNMPFILES := $(wildcard lwip/src/apps/snmp/*.c)
SMTPFILES := $(wildcard lwip/src/apps/smtp/*.c)
NETBIOSNSFILES := $(wildcard lwip/src/apps/netbiosns/*.c)
MQTTFILES := $(wildcard lwip/src/apps/mqtt/*.c)
MDNSFILES := $(wildcard lwip/src/apps/mdns/*.c)
LWIPERFFILES := $(wildcard lwip/src/apps/lwiperf/*.c)
HTTPFILES := $(wildcard lwip/src/apps/http/*.c)
ALTCP_TLSFILES := $(wildcard lwip/src/apps/altcp_tls/*.c)

SOURCES := $(COREFILES) $(CORE4FILES) $(CORE6FILES) $(NETIFFILES) $(PPPFILES) \
	$(APIFILES) $(TFTPFILES) $(SNTPFILES) $(SNMPFILES) $(SMTPFILES) $(NETBIOSNSFILES) \
	$(MQTTFILES) $(MDNSFILES) $(LWIPERFFILES) $(HTTPFILES) $(ALTCP_TLSFILES)
OBJECTS := $(SOURCES:%.c=%.o) $(PORT_DIR)/sys_arch.o

liblwIP.a: $(OBJECTS)
	$(AR) -src $@ $^

clean:
	rm -f liblwIP.a $(OBJECTS)
