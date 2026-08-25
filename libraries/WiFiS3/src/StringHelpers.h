#ifndef ARDUINO_STRING_HELPERS_H
#define ARDUINO_STRING_HELPERS_H

#include <string>
#include <vector>
#include <algorithm>

/**
 * @brief Trims whitespace characters from the beginning of a string. 
 * 
 * @param `s` is the string from which leading whitespace characters will be removed. 
 * The function modifies this string directly.
 */
void ltrim(std::string &s);

/**
 * @brief Trims trailing whitespace characters from a string.
 *
 * @param `s` the string from which trailing whitespace characters will be removed. 
 * The function modifies this string directly.
 */
void rtrim(std::string &s);


/**
 * @brief Trims whitespace characters from both ends of a string.
 *
 * This function removes any leading and trailing whitespace characters (spaces, 
 * tabs, etc.) from the given string. It modifies the string in place, erasing 
 * whitespace from both the beginning and the end of the string.
 *
 * @param `s` is the string from which both leading and trailing whitespace characters 
 * will be removed. The function modifies this string directly.
 */
void trim(std::string &s);

/**
 * @brief Takes a string and splits it into substrings.
 * 
 * This function splits a string into multiple substrings, storing each substring 
 * as an element in the `res` vector. The string is split at each occurrence of 
 * the specified delimiter. Optionally, the function can trim whitespace from 
 * each token before storing it in the result vector.
 * 
 * @param `res` is a reference to a vector of strings where the resulting tokens 
 * will be stored.
 * @param `str` is the string to be split. This string will be modified as it is 
 * split.
 * @param `delimiter` is the delimiter string that separates the tokens in `str`.
 * @param `_trim` is a boolean flag indicating whether to trim whitespace from each 
 * token. If true, leading and trailing whitespace will be removed from each token. Defaults to true.
 */
void split(std::vector<std::string> &res, std::string &str, const std::string &delimiter, bool _trim = true);

/** 
 * @brief Removes a specified substring from the beginning of a string.
 *
 * This function attempts to remove the first occurrence of the specified substring 
 * (`what`) from the beginning of the string (`str`). Before performing the removal, 
 * it trims leading whitespace from the string. If the substring is found at the 
 * beginning of the string, it is removed, and the function returns `true`. Otherwise the 
 * function returns `false`.
 *
 * @param `str` is a reference to the string from which the substring will be removed. 
 * The string is modified if the substring is removed.
 * @param `what` is the substring to be removed from the beginning of `str`.
 * 
 * @return `true` if the substring was found and removed from the beginning of 
 * the string, `false` otherwise.
 */
bool removeAtBegin(std::string &str, const std::string &what);

#endif