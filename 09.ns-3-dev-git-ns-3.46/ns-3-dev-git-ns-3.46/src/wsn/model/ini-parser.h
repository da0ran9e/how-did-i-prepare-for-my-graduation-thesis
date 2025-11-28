#ifndef INI_PARSER_H
#define INI_PARSER_H

#include <map>
#include <string>
#include <fstream>
#include <algorithm>
#include <cctype>

class IniParser {
private:
    std::map<std::string, std::string> m_values;

public:
    bool Load(const std::string &path);
    std::string Get(const std::string &key, const std::string &defaultVal = "") const;
    int GetInt(const std::string &key, int def = 0) const;
    double GetDouble(const std::string &key, double def = 0.0) const;


};

#endif