#ifndef ARDUINO_STRING_HELPERS_H
#define ARDUINO_STRING_HELPERS_H

#include <string>
#include <vector>
#include <algorithm>

/**
 * Trims whitespace characters from the beginning (left side) of the input string `s`.
 */
void ltrim(std::string &s);

// trim from end (in place)
void rtrim(std::string &s);

// trim from both ends (in place)
void trim(std::string &s);

/**
 * Takes a string `str` and splits it into substrings based on a specified `delimiter`.
 * The resulting substrings are stored in the `res` vector of strings.
 */
void split(std::vector<std::string> &res, std::string &str, const std::string &delimiter, bool _trim = true);

/** 
 * Checks if the input string `str` starts with the specified string `what`. If `str` starts 
 * with `what`, the function removes `what` from the beginning of `str` and returns `true`.
 */
bool removeAtBegin(std::string &str, const std::string &what);

#endif