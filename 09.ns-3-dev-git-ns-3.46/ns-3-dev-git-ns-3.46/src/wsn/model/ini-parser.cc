#include "ini-parser.h"

bool IniParser::Load(const std::string &path)
{
    std::ifstream ifs(path);
    if (!ifs.is_open()) return false;

    std::string line;
    while (std::getline(ifs, line))
    {
        // comment
        if (line.size() == 0 || line[0] == '#')
            continue;

        size_t eq = line.find('=');
        if (eq == std::string::npos)
            continue;

        std::string key = line.substr(0, eq);
        std::string val = line.substr(eq + 1);

        // whitespace
        key.erase(remove_if(key.begin(), key.end(), ::isspace), key.end());
        val.erase(val.begin(), find_if(val.begin(), val.end(), [](int c){return !isspace(c);}));  

        m_values[key] = val;
    }
    return true;
}

std::string IniParser::Get(const std::string &key, const std::string &defaultVal) const
{
    auto it = m_values.find(key);
    return it == m_values.end() ? defaultVal : it->second;
}

int IniParser::GetInt(const std::string &key, int def) const
{
    auto v = Get(key, "");
    if (v == "") return def;
    return std::stoi(v);
}
