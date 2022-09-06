# Arduino Renesas fsp Boards

[![Sync Labels status](https://github.com/arduino/ArduinoCore-renesas/actions/workflows/sync-labels.yml/badge.svg)](https://github.com/arduino/ArduinoCore-renesas/actions/workflows/sync-labels.yml)

## Recompile libfsp.a
* Navigate to `extras/e2studioProjects/$projectName`
* Open the project in e2studio and build it
* Launch `fsp_to_arduino.sh` script; this will
  * rebuild the library without .c files in ra_gen
  * rename the library and copy in `variant/$boardName/libs`
  * copy the include files (both generated and from fsp) in `variant/$boardName/libs`
  * copy the .c files in ra_gen to `variants/$boardName/tmp_gen_c_files/` -> **ONLY TEMPORARILY, these files will eventually go away**
