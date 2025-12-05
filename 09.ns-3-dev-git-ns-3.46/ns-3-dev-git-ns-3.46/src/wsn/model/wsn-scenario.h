#ifndef WSN_SCENARIO_H
#define WSN_SCENARIO_H

#pragma once
#include "ini-parser.h"
//#include "wsn-trace.h"
#include <string>
#include <unordered_map>

class WsnScenario {
public:
    void configure(std::string iniFile);
private:
    // callbacks
    void onSection(const std::string &section);
    void onKeyValue(const std::string &key,
                    const std::string &value,
                    const std::string &comment,
                    const std::string &baseDir);

private:
    std::string m_currentSection;
    std::unordered_map<std::string, std::unordered_map<std::string, std::string>> m_config; // key-value 
    
    bool m_traceEnabled = false;
    std::string m_traceFile = "wsn-trace.txt";

    // Parsed parameters
    uint32_t m_numNodes = 0;
    double m_fieldX = 0;
    double m_fieldY = 0;
};

#endif // WSN_SCENARIO_H