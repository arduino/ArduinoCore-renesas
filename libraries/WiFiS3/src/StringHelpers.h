#ifndef ARDUINO_STRING_HELPERS_H
#define ARDUINO_STRING_HELPERS_H

#include <string>
#include <vector>
#include <algorithm>

void ltrim(std::string &s);

// trim from end (in place)
void rtrim(std::string &s);

// trim from both ends (in place)
void trim(std::string &s);

void split(std::vector<std::string> &res, std::string &str, const std::string &delimiter, bool _trim = true);

bool removeAtBegin(std::string &str, const std::string &what);

#endif