/*
  This example checks all possible types that can be stored with Preferences library
*/
#include <Preferences.h>
#include <float.h>
Preferences prefs;

void setup() {
  Serial.begin(115200);

  if (!prefs.begin("types")) { // use "types" namespace
    Serial.println("Cannot initialize preferences");
    Serial.println("Make sure your WiFi firmware version is greater than 0.3.0");
    while(1) {};
  }

  if (!testChar()) {
    return;
  }

  if (!testUChar()) {
    return;
  }

  if (!testShort()) {
    return;
  }

  if (!testUShort()) {
    return;
  }

  if (!testInt()) {
    return;
  }

  if (!testUInt()) {
    return;
  }

  if (!testLong64()) {
    return;
  }

  if (!testULong64()) {
    return;
  }

  if (!testFloat()) {
    return;
  }

  if (!testDouble()) {
    return;
  }

  if (!testBytes()) {
    return;
  }

  if (!testString()) {
    return;
  }
}

void loop() {
  Serial.println(prefs.freeEntries());
  prefs.clear();
  Serial.println(prefs.freeEntries());
  prefs.end();

  while(1) {};
}

int testChar() {
  int8_t data;
  int8_t value;

  data = INT8_MAX;
  prefs.putChar("char", data);
  value = prefs.getChar("char", 0);
  if (value != 127) {
    Serial.println("getChar MAX Error");
    return 0;
  } else {
    Serial.print("getChar MAX: ");
    Serial.println(value);
  }

  data = INT8_MIN;
  prefs.putChar("char", data);
  value = prefs.getChar("char", 0);
  if (value != -128) {
    Serial.println("getChar MIN Error");
    return 0;
  } else {
    Serial.print("getChar MIN: ");
    Serial.println(value);
  }

  return 1;
}

int testUChar() {
  uint8_t data;
  uint8_t value;

  data = UINT8_MAX;
  prefs.putUChar("uchar", data);
  value = prefs.getUChar("uchar", 0);
  if (value != 255) {
    Serial.println("getUChar MAX Error");
    return 0;
  } else {
    Serial.print("getUChar MAX: ");
    Serial.println(value);
  }

  prefs.remove("uchar");

  data = 0;
  prefs.putUChar("uchar", data);
  value = prefs.getUChar("uchar", 1);
  if (value != 0) {
    Serial.println("getUChar MIN Error");
    return 0;
  } else {
    Serial.print("getUChar MIN: ");
    Serial.println(value);
  }

  return 1;
}

int testShort() {
  int16_t data;
  int16_t value;

  data = INT16_MAX;
  prefs.putShort("short", data);
  value = prefs.getShort("short", 0);
  if (value != 32767) {
    Serial.println("getShort MAX Error");
    return 0;
  } else {
    Serial.print("getShort MAX: ");
    Serial.println(value);
  }

  data = INT16_MIN;
  prefs.putShort("short", data);
  value = prefs.getShort("short", 0);
  if (value != -32768) {
    Serial.println("getShort MIN Error");
    return 0;
  } else {
    Serial.print("getShort MIN: ");
    Serial.println(value);
  }

  return 1;
}

int testUShort() {
  uint16_t data;
  uint16_t value;

  data = UINT16_MAX;
  prefs.putUShort("ushort", data);
  value = prefs.getUShort("ushort", 0);
  if (value != 65535U) {
    Serial.println("getUShort MAX Error");
    return 0;
  } else {
    Serial.print("getUShort MAX: ");
    Serial.println(value);
  }

  data = 0;
  prefs.putUShort("ushort", data);
  value = prefs.getUShort("ushort", 1);
  if (value != 0) {
    Serial.println("getUShort MIN Error");
    return 0;
  } else {
    Serial.print("getUShort MIN: ");
    Serial.println(value);
  }

  return 1;
}

int testInt() {
  int32_t data;
  int32_t value;

  data = INT32_MAX;
  prefs.putInt("int", data);
  value = prefs.getInt("int", 0);
  if (value != 2147483647) {
    Serial.println("getInt MAX Error");
    return 0;
  } else {
    Serial.print("getInt MAX: ");
    Serial.println(value);
  }

  data = INT32_MIN;
  prefs.putInt("int", data);
  value = prefs.getInt("int", 0);
  if (value != (-2147483647 - 1)) {
    Serial.println("getInt MIN Error");
    return 0;
  } else {
    Serial.print("getInt MIN: ");
    Serial.println(value);
  }

  return 1;
}

int testUInt() {
  uint32_t data;
  uint32_t value;

  data = UINT32_MAX;
  prefs.putUInt("uint", data);
  value = prefs.getUInt("uint", 0);
  if (value != 4294967295U) {
    Serial.println("getUInt MAX Error");
    return 0;
  } else {
    Serial.print("getUInt MAX: ");
    Serial.println(value);
  }

  data = 0;
  prefs.putUInt("uint", data);
  value = prefs.getUInt("uint", 1);
  if (value != 0) {
    Serial.println("getUInt MIN Error");
    return 0;
  } else {
    Serial.print("getUInt MIN: ");
    Serial.println(value);
  }

  return 1;
}

int testLong64() {
  int64_t data;
  int64_t value;

  data = INT64_MAX;
  prefs.putLong64("long", data);
  value = prefs.getLong64("long", 0);
  if (value != 9223372036854775807LL) {
    Serial.println("getLong MAX Error");
    return 0;
  } else {
    Serial.print("getLong MAX: ");
    Serial.println(value);
  }

  data = INT64_MIN;
  prefs.putLong64("long", data);
  value = prefs.getLong64("long", 0);
  if (value != (-9223372036854775807LL - 1)) {
    Serial.println("getLong MIN Error");
    return 0;
  } else {
    Serial.print("getLong MIN: ");
    Serial.println(value);
  }

  return 1;
}

int testULong64() {
  uint64_t data;
  uint64_t value;

  data = UINT64_MAX;
  prefs.putULong64("ulong", data);
  value = prefs.getULong64("ulong", 0);
  if (value != 18446744073709551615ULL) {
    Serial.println("getULong64 MAX Error");
    return 0;
  } else {
    Serial.print("getULong64 MAX: ");
    Serial.println(value);
  }

  data = 0;
  prefs.putULong64("ulong", data);
  value = prefs.getULong64("ulong", 1);
  if (value != 0) {
    Serial.println("getULong64 MIN Error");
    return 0;
  } else {
    Serial.print("getULong64 MIN: ");
    Serial.println(value);
  }

  return 1;
}

int testFloat() {
  float data;
  float value;

  data = FLT_MAX;
  prefs.putFloat("float", data);
  value = prefs.getFloat("float", 0);
  if (value != FLT_MAX) {
    Serial.println("getFloat MAX Error");
    return 0;
  } else {
    Serial.print("getFloat MAX: ");
    Serial.println(value);
  }

  data = FLT_MIN;
  prefs.putFloat("float", data);
  value = prefs.getFloat("float", 0);
  if (value != FLT_MIN) {
    Serial.println("getFloat MIN Error");
    return 0;
  } else {
    Serial.print("getFloat MIN: ");
    Serial.println(value);
  }

  return 1;
}

int testDouble() {
  double data;
  double value;

  data = DBL_MAX;
  prefs.putDouble("double", data);
  value = prefs.getDouble("double", 0);
  if (value != DBL_MAX) {
    Serial.println("getDouble MAX Error");
    return 0;
  } else {
    Serial.print("getDouble MAX: ");
    Serial.println(value);
  }

  data = DBL_MIN;
  prefs.putDouble("double", data);
  value = prefs.getDouble("double", 0);
  if (value != DBL_MIN) {
    Serial.println("getDouble MIN Error");
    return 0;
  } else {
    Serial.print("getDouble MIN: ");
    Serial.println(value);
  }

  return 1;
}

int testBytes() {
  uint8_t data[] = {9, 30, 235, 255, 20, 15, 0, 1, 9, 30, 235, 255, 20, 15, 0, 1, 9, 30, 235, 255, 20, 15, 0, 1, 9, 30, 235, 255, 20, 15, 0, 1};
  size_t len;

  prefs.putBytes("bytes", data, sizeof(data));
  len = prefs.getBytesLength("bytes");
  uint8_t value[len];
  prefs.getBytes("bytes", value, len);
  if (memcmp(data, value, len) != 0) {
    Serial.println("getBytes Error");
    return 0;
  } else {
    Serial.println("getBytes OK");
  }

  return 1;
}

int testString() {
  const char* data = "Lorem ipsum dolor sit amet consectetur adipiscing elit.";
  String value = "default";

  prefs.putString("string", data);
  value = prefs.getString("string", value);
  if (strcmp(data, value.c_str()) != 0) {
    Serial.println("getString Error");
    Serial.println("Make sure your WiFi firmware version is at least 0.5.0");
    return 0;
  } else {
    Serial.println("getString OK");
  }

  return 1;
}
