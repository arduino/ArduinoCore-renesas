// Copyright 2015-2021 Espressif Systems (Shanghai) PTE LTD
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at

//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "Preferences.h"


using namespace std;

Preferences::Preferences() {

}

Preferences::~Preferences() {
    end();
}

bool Preferences::begin(const char * name, bool readOnly, const char* partition_label){
    string res = "";
    modem.begin();
    if (name != nullptr && strlen(name) > 0) {
        if (modem.write(string(PROMPT(_PREF_BEGIN)), res, "%s%s,%d,%s\r\n", CMD_WRITE(_PREF_BEGIN), name, readOnly, partition_label != NULL ? partition_label : "")) {
            return (atoi(res.c_str()) != 0) ? true : false;
        }
    }
    return false;
}

void Preferences::end() {
    string res = "";
    modem.write(string(PROMPT(_PREF_END)), res, "%s", CMD(_PREF_END));
}

/*
 * Clear all keys in opened preferences
 * */

bool Preferences::clear() {
    string res = "";
    if (modem.write(string(PROMPT(_PREF_CLEAR)), res, "%s", CMD(_PREF_CLEAR))) {
        return (atoi(res.c_str()) != 0) ? true : false;
    }
    return false;
}

/*
 * Remove a key
 * */

bool Preferences::remove(const char * key) {
    string res = "";
    if (key != nullptr && strlen(key) > 0) {
        if (modem.write(string(PROMPT(_PREF_REMOVE)), res, "%s%s\r\n", CMD_WRITE(_PREF_REMOVE), key)) {
            return (atoi(res.c_str()) != 0) ? true : false;
        }
    }
    return false;
}

/*
 * Put a key value
 * */

size_t Preferences::putChar(const char* key, int8_t value) {
    string res = "";
    if (key != nullptr && strlen(key) > 0) {
        if (modem.write(string(PROMPT(_PREF_PUT)), res, "%s%s,%d,%hd\r\n", CMD_WRITE(_PREF_PUT), key, PT_I8, value)) {
            return atoi(res.c_str());
        }
    }
    return 0;
}

size_t Preferences::putUChar(const char* key, uint8_t value) {
    string res = "";
    if (key != nullptr && strlen(key) > 0) {
        if (modem.write(string(PROMPT(_PREF_PUT)), res, "%s%s,%d,%hu\r\n", CMD_WRITE(_PREF_PUT), key, PT_U8, value)) {
            return atoi(res.c_str());
        }
    }
    return 0;
}

size_t Preferences::putShort(const char* key, int16_t value) {
    string res = "";
    if (key != nullptr && strlen(key) > 0) {
        if (modem.write(string(PROMPT(_PREF_PUT)), res, "%s%s,%d,%hd\r\n", CMD_WRITE(_PREF_PUT), key, PT_I16, value)) {
            return atoi(res.c_str());
        }
    }
    return 0;
}

size_t Preferences::putUShort(const char* key, uint16_t value) {
    string res = "";
    if (key != nullptr && strlen(key) > 0) {
        if (modem.write(string(PROMPT(_PREF_PUT)), res, "%s%s,%d,%hu\r\n", CMD_WRITE(_PREF_PUT), key, PT_U16, value)) {
            return atoi(res.c_str());
        }
    }
    return 0;
}

size_t Preferences::putInt(const char* key, int32_t value) {
    string res = "";
    if (key != nullptr && strlen(key) > 0) {
        if (modem.write(string(PROMPT(_PREF_PUT)), res, "%s%s,%d,%d\r\n", CMD_WRITE(_PREF_PUT), key, PT_I32, value)) {
            return atoi(res.c_str());
        }
    }
    return 0;
}

size_t Preferences::putUInt(const char* key, uint32_t value) {
    string res = "";
    if (key != nullptr && strlen(key) > 0) {
        if (modem.write(string(PROMPT(_PREF_PUT)), res, "%s%s,%d,%u\r\n", CMD_WRITE(_PREF_PUT), key, PT_U32, value)) {
            return atoi(res.c_str());
        }
    }
    return 0;
}

size_t Preferences::putLong(const char* key, int32_t value) {
    return putInt(key, value);
}

size_t Preferences::putULong(const char* key, uint32_t value) {
    return putUInt(key, value);
}

size_t Preferences::putLong64(const char* key, int64_t value) {
    return putBytes(key, (void*)&value, sizeof(int64_t));
}

size_t Preferences::putULong64(const char* key, uint64_t value) {
    return putBytes(key, (void*)&value, sizeof(uint64_t));
}

size_t Preferences::putFloat(const char* key, const float_t value) {
    return putBytes(key, (void*)&value, sizeof(float_t));
}

size_t Preferences::putDouble(const char* key, const double_t value) {
    return putBytes(key, (void*)&value, sizeof(double_t));
}

size_t Preferences::putBool(const char* key, const bool value) {
    return putUChar(key, (uint8_t) (value ? 1 : 0));
}

size_t Preferences::putString(const char* key, const char* value) {
    string res = "";
    if (key != nullptr && strlen(key) > 0 && value != nullptr && strlen(value) > 0) {
        if (modem.write(string(PROMPT(_PREF_PUT)), res, "%s%s,%d,%s\r\n", CMD_WRITE(_PREF_PUT), key, PT_STR, value)) {
            return atoi(res.c_str());
        }
    }
    return 0;
}

size_t Preferences::putString(const char* key, const String value) {
    return putString(key, value.c_str());
}

size_t Preferences::putBytes(const char* key, const void* value, size_t len) {
    string res = "";
    if ( key != nullptr && strlen(key) > 0 && value != nullptr && len > 0) {
        modem.write_nowait(string(PROMPT(_PREF_PUT)), res, "%s%s,%d,%d\r\n", CMD_WRITE(_PREF_PUT), key, PT_BLOB, len);
        if(modem.passthrough((uint8_t *)value, len)) {
            return len;
        }
    }
    return 0;
}

PreferenceType Preferences::getType(const char* key) {
    string res = "";
    if (key != nullptr && strlen(key) > 0) {
        if (modem.write(string(PROMPT(_PREF_PUT)), res, "%s%s\r\n", CMD_WRITE(_PREF_PUT), key)) {
            return static_cast<PreferenceType>(atoi(res.c_str()));
        }
    }
    return PT_INVALID;
}

bool Preferences::isKey(const char* key) {
    return getType(key) != PT_INVALID;
}

/*
 * Get a key value
 * */

int8_t Preferences::getChar(const char* key, const int8_t defaultValue) {
    int16_t value = defaultValue;
    string res = "";
    if (key != nullptr && strlen(key) > 0) {
        if (modem.write(string(PROMPT(_PREF_GET)), res, "%s%s,%d,%hd\r\n", CMD_WRITE(_PREF_GET), key, PT_I8, defaultValue)) {
            sscanf(res.c_str(), "%hd", &value);
        }
    }
    return value;
}

uint8_t Preferences::getUChar(const char* key, const uint8_t defaultValue) {
    uint16_t value = defaultValue;
    string res = "";
    if (key != nullptr && strlen(key) > 0) {
        if (modem.write(string(PROMPT(_PREF_GET)), res, "%s%s,%d,%hu\r\n", CMD_WRITE(_PREF_GET), key, PT_U8, defaultValue)) {
            sscanf(res.c_str(), "%hu", &value);
        }
    }
    return value;
}

int16_t Preferences::getShort(const char* key, const int16_t defaultValue) {
    int16_t value = defaultValue;
    string res = "";
    if (key != nullptr && strlen(key) > 0) {
        if (modem.write(string(PROMPT(_PREF_GET)), res, "%s%s,%d,%hd\r\n", CMD_WRITE(_PREF_GET), key, PT_I16, defaultValue)) {
            sscanf(res.c_str(), "%hd", &value);
        }
    }
    return value;
}

uint16_t Preferences::getUShort(const char* key, const uint16_t defaultValue) {
    uint16_t value = defaultValue;
    string res = "";
    if (key != nullptr && strlen(key) > 0) {
        if (modem.write(string(PROMPT(_PREF_GET)), res, "%s%s,%d,%hu\r\n", CMD_WRITE(_PREF_GET), key, PT_U16, defaultValue)) {
            sscanf(res.c_str(), "%hu", &value);
        }
    }
    return value;
}

int32_t Preferences::getInt(const char* key, const int32_t defaultValue) {
    int32_t value = defaultValue;
    string res = "";
    if (key != nullptr && strlen(key) > 0) {
        if (modem.write(string(PROMPT(_PREF_GET)), res, "%s%s,%d,%d\r\n", CMD_WRITE(_PREF_GET), key, PT_I32, defaultValue)) {
            sscanf(res.c_str(), "%d", &value);
        }
    }
    return value;
}

uint32_t Preferences::getUInt(const char* key, const uint32_t defaultValue) {
    uint32_t value = defaultValue;
    string res = "";
    if (key != nullptr && strlen(key) > 0) {
        if (modem.write(string(PROMPT(_PREF_GET)), res, "%s%s,%d,%u\r\n", CMD_WRITE(_PREF_GET), key, PT_U32, defaultValue)) {
            sscanf(res.c_str(), "%u", &value);
        }
    }
    return value;
}

int32_t Preferences::getLong(const char* key, const int32_t defaultValue) {
    return getInt(key, defaultValue);
}

uint32_t Preferences::getULong(const char* key, const uint32_t defaultValue) {
    return getUInt(key, defaultValue);
}

int64_t Preferences::getLong64(const char* key, const int64_t defaultValue) {
    int64_t value = defaultValue;
    getBytes(key, (void*) &value, sizeof(int64_t));
    return value;
}

uint64_t Preferences::getULong64(const char* key, const uint64_t defaultValue) {
    uint64_t value = defaultValue;
    getBytes(key, (void*) &value, sizeof(uint64_t));
    return value;
}

float_t Preferences::getFloat(const char* key, const float_t defaultValue) {
    float_t value = defaultValue;
    getBytes(key, (void*) &value, sizeof(float_t));
    return value;
}

double_t Preferences::getDouble(const char* key, const double_t defaultValue) {
    double_t value = defaultValue;
    getBytes(key, (void*) &value, sizeof(double_t));
    return value;
}

bool Preferences::getBool(const char* key, const bool defaultValue) {
    return getUChar(key, defaultValue ? 1 : 0) == 1;
}

size_t Preferences::getString(const char* key, char* value, const size_t maxLen) {
    string res = "";
    if (key != nullptr && strlen(key) > 0 && value != nullptr) {
        if (modem.write(string(PROMPT(_PREF_GET)), res, "%s%s,%d\r\n", CMD_WRITE(_PREF_GET), key, PT_STR)) {
            if (res.length() < maxLen) {
                strncpy(value, res.c_str(), res.length());
                return res.length();
            }
        }
    }
    return 0;
}

String Preferences::getString(const char* key, const String defaultValue) {
    string res = "";
    if (key != nullptr && strlen(key) > 0) {
        if (modem.write(string(PROMPT(_PREF_GET)), res, "%s%s,%d,%s\r\n", CMD_WRITE(_PREF_GET), key, PT_STR, defaultValue.c_str())) {
            return String(res.c_str());
        }
    }
    return defaultValue;
}

size_t Preferences::getBytesLength(const char* key) {
    string res = "";
    if (key != nullptr && strlen(key) > 0) {
        if (modem.write(string(PROMPT(_PREF_LEN)), res, "%s%s\r\n", CMD_WRITE(_PREF_LEN), key)) {
            return atoi(res.c_str());
        }
    }
    return 0;
}

size_t Preferences::getBytes(const char* key, void * buf, size_t maxLen) {
    size_t len = getBytesLength(key);
    string res = "";
    if (key != nullptr && strlen(key) > 0 && buf != nullptr && len > 0) {
        modem.avoid_trim_results();
        modem.read_using_size();
        if (modem.write(string(PROMPT(_PREF_GET)), res, "%s%s,%d\r\n", CMD_WRITE(_PREF_GET), key, PT_BLOB)) {
            if (res.size() >= len && len <= maxLen) {
                memcpy(buf, (uint8_t*)&res[0], len);
            }
        }
    }
    return 0;
}

size_t Preferences::freeEntries() {
    string res = "";
    if (modem.write(string(PROMPT(_PREF_STAT)), res, "%s", CMD(_PREF_STAT))) {
        return atoi(res.c_str());
    }
    return 0;
}
