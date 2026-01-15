# SFU Library Examples - Over-The-Air (OTA) Update Workflows

This directory contains examples demonstrating Over-The-Air (OTA) firmware update workflows for Arduino Portenta C33 using the SFU (Second stage bootloader Firmware Update) library.

**Structure:**
- **2 OTA Update Approaches**: `OTAUpdate` and `UpdateFromFile` - showing different methods to deliver and apply firmware updates
- **1 Test Sketch**: `OTAUsage` - a simple demo application used as the target firmware for both OTA approaches

---

## OTA Update Approaches

### Approach 1: OTAUpdate (Network-Based OTA)
Downloads firmware from a remote server over WiFi. 

### Approach 2: UpdateFromFile (Local/Embedded OTA)
Embeds the firmware update directly in the sketch: useful for testing, offline updates, or pre-loaded production devices.

---

## Overview: General OTA Update Process

Both OTA approaches follow a similar workflow:
1. **Create** a new firmware binary (sketch)
2. **Compress & Package** the binary into OTA format
3. **Store** the OTA file (on server or embedded in sketch)
4. **Download** the OTA file to the board's QSPI flash
5. **Apply** the update via bootloader

---

## Test Sketch: OTAUsage

**File:** [OTAUsage/OTAUsage.ino](OTAUsage/OTAUsage.ino)

### Purpose
This is a simple test application designed to be packaged as the target firmware that will be delivered and installed via OTA updates. Used to verify that the OTA update process succeeded.

### What It Does
- Controls the RGB LED on Portenta C33
- Cycles through three colors: Blue → Green → Red (1-second intervals each)
- Acts as a visible indicator that OTA completed successfully

### ⚠️ Important: Always Include SFU.h

Notice that OTAUsage.ino includes `#include "SFU.h"` at the top. **This is critical for any firmware that will be deployed via OTA.**

**Why?** The SFU library embeds the second-stage bootloader binary. By including it, the linker script (see [variants/PORTENTA_C33/fsp.ld](../../variants/PORTENTA_C33/fsp.ld)) properly reserves memory for the bootloader, ensuring that your application is placed in flash *after* the bootloader—not overwriting it.

**Best Practice:** Always include `#include "SFU.h"` in sketches intended for OTA deployment.

### How to Use It
1. Open OTAUsage.ino and verify it compiles and runs on Portenta C33
2. Export compiled binary: `Sketch → Export Compiled Binary`
3. Package the binary as an OTA file (see tools and steps below)
4. Use the packaged OTA file with either **OTAUpdate** or **UpdateFromFile** approach

---

## OTA Approach 1: OTAUpdate (Network-Based)

**File:** [OTAUpdate/OTAUpdate.ino](OTAUpdate/OTAUpdate.ino)

### Purpose
This example demonstrates **downloading an OTA update from a remote server** over WiFi and applying it to the board.

### What It Does
1. Connects to WiFi network (credentials from `arduino_secrets.h`)
2. Mounts the QSPI flash filesystem
3. **Downloads** an OTA file from a remote URL
4. Saves it to QSPI at `/ota/UPDATE.BIN.OTA`
5. **Applies** the update (bootloader takes over on next reset)

### Key Features
- **Network-based OTA**: Fetches firmware from `http://downloads.arduino.cc/ota/OTAUsage.ino.PORTENTA_C33.ota`
- **Requires WiFi**: Uses `WiFiC3` library to connect to network
- **Error handling**: Validates filesystem mount/unmount operations
- **Real-world deployment**: Suitable for production firmware distribution


### Complete OTA Workflow
```
Step 1: Prepare firmware image
  - Create & test a sketch (e.g., OTAUsage.ino)
  - Export compiled binary: Sketch → Export Compiled Binary
  
Step 2: Package for OTA
  - Download tools: https://github.com/arduino-libraries/ArduinoIoTCloud/tree/master/extras/tools
  - Compress: ./lzss.py --encode YourSketch.bin YourSketch.lzss
  - Package: ./bin2ota.py PORTENTA_C33 YourSketch.lzss YourSketch.ota
  
Step 3: Upload to server
  - Host the .ota file at a publicly accessible URL
  - Example: http://your-server.com/ota/YourSketch.ota
  
Step 4: Deploy OTA update
  - Upload OTAUpdate.ino to Portenta C33
  - Update the OTA_FILE_LOCATION constant with your URL
  - Board connects to WiFi, downloads, and applies update
```

---

## OTA Approach 2: UpdateFromFile (Local/Embedded)

**File:** [UpdateFromFile/UpdateFromFile.ino](UpdateFromFile/UpdateFromFile.ino)

### Purpose
This example demonstrates **storing an OTA update locally** on the device (embedded in firmware) and applying it. Useful for testing, offline updates, or pre-loading updates on production devices.

### What It Does
1. Reformats the QSPI filesystem
2. **Copies** a pre-compiled OTA file (from `update.h` header) into QSPI flash
3. Saves it to `/ota/UPDATE.BIN.OTA`
4. **Applies** the update immediately
5. Resets the board to trigger bootloader firmware installation

### Preparation Steps
```
Step 1: Create and export firmware binary
  - Create your target sketch (e.g., OTAUsage.ino)
  - Verify it works on the board
  - Export: Sketch → Export Compiled Binary
  
Step 2: Compress and package
  - ./lzss.py --encode OTAUsage.ino.bin OTAUsage.ino.lzss
  - ./bin2ota.py PORTENTA_C33 OTAUsage.ino.lzss OTAUsage.ino.ota
  
Step 3: Convert to header file
  - xxd -i OTAUsage.ino.ota > update.h
  - Creates array: OTAUsage_ino_PORTENTA_C33_ota[]
  
Step 4: Include and deploy
  - Copy update.h to UpdateFromFile sketch folder
  - Upload UpdateFromFile.ino
  - Update executes immediately
```

---

## OTA Packaging (Common to Both Approaches)

Both OTA approaches use the same OTA file format (`.ota`) and packaging process. Only the delivery method differs.

### Packaging Steps (Detailed)

1. **Export binary from Arduino IDE**
   ```bash
   # Generated as: YourSketch.ino.PORTENTA_C33.bin
   ```

2. **Compress with LZSS**
   ```bash
   ./lzss.py --encode YourSketch.ino.PORTENTA_C33.bin YourSketch.ino.lzss
   ```

3. **Package for OTA**
   ```bash
   ./bin2ota.py PORTENTA_C33 YourSketch.ino.lzss YourSketch.ino.ota
   ```

4. **For embedded use (UpdateFromFile): Convert to header**
   ```bash
   xxd -i YourSketch.ino.ota > update.h
   ```

### Download Tools
Repository: https://github.com/arduino-libraries/ArduinoIoTCloud/tree/master/extras/tools
- `lzss.py` - LZSS compression encoder
- `bin2ota.py` - OTA file format generator

---



