#include "StringHelpers.h"


void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
}

// trim from end (in place)
void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

// trim from both ends (in place)
void trim(std::string &s) {
    rtrim(s);
    ltrim(s);
}


void split(std::vector<std::string> &res, std::string &str, const std::string &delimiter, bool _trim /* = true */) {
    size_t pos = 0;
    std::string token;
    while ((pos = str.find(delimiter)) != std::string::npos) {
        token = str.substr(0, pos);
        if(_trim) {
           trim(token);
        }
        res.push_back(token);
        str.erase(0, pos + delimiter.length());
    }
    if(str != "") {
        if(_trim) {
            trim(str);
        }
        res.push_back(str);
    }
}

bool removeAtBegin(std::string &str, const std::string &what) {
    ltrim(str);
    int pos = str.find(what);
    if(pos == 0) {
        str = str.substr(pos + what.size());
        return true;
    }
    return false;

}