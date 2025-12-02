#include "ini-parser.h"
#include <fstream>
#include <sstream>
#include <stdexcept>

IniParser::IniParser()
{
    // TODO: initialize anything if needed
}

void IniParser::read(const std::string &filename)
{
    // TODO:
    // - call readFile(filename)
}

void IniParser::readFile(const std::string &filename)
{
    // TODO:
    // - create include stack
    // - call doReadFile(filename, includeStack)
}

void IniParser::readText(const std::string &text,
                         const std::string &filename,
                         const std::string &baseDir)
{
    // TODO:
    // - convert text to std::stringstream
    // - call readStream()
}

void IniParser::readStream(std::istream &in,
                           const std::string &filename,
                           const std::string &baseDir)
{
    // TODO:
    // - prepare include stack
    // - call doReadFromStream() with stream
}

void IniParser::doReadFile(const std::string &filename,
                           std::vector<std::string> &includeStack)
{
    // TODO:
    // - detect circular includes
    // - open file stream
    // - compute base directory from filename
    // - call doReadFromStream()
}

void IniParser::doReadFromStream(std::istream &in,
                                 const std::string &filename,
                                 std::vector<std::string> &includeStack,
                                 const std::string &absoluteFilename,
                                 const std::string &baseDir)
{
    // TODO:
    // - parse stream line by line using forEachJoinedLine
    // - detect:
    //      '#' comment
    //      "include <file>"
    //      "[section]"
    //      "key = value"
    // - call callbacks or store results
}

void IniParser::forEachJoinedLine(
    std::istream &in,
    const std::function<void(std::string &, int, int)> &processLine)
{
    // TODO:
    // - implement multi-line joining algorithm similar to OMNeT++:
    //     * join with backslash continuation
    //     * join with indent continuation
    // - for each joined line, call processLine(buffer, lineNumber, numPhysicalLines)
}

const char *IniParser::findEndContent(const char *line,
                                      const char *filename,
                                      int lineNumber)
{
    // TODO:
    // - scan through string
    // - skip quoted literals
    // - find '#' comment start
    // - return pointer to the end of meaningful content
    return nullptr;
}

void IniParser::rtrim(std::string &str)
{
    // TODO:
    // - remove trailing spaces/tabs
}

std::string IniParser::trim(const char *start, const char *end)
{
    // TODO:
    // - return substring with left+right spaces removed
    return "";
}

// bool IniParser::Load(const std::string &path)
// {
//     std::ifstream ifs(path);
//     if (!ifs.is_open()) return false;

//     std::string line;
//     while (std::getline(ifs, line))
//     {
//         // comment
//         if (line.size() == 0 || line[0] == '#')
//             continue;

//         size_t eq = line.find('=');
//         if (eq == std::string::npos)
//             continue;

//         std::string key = line.substr(0, eq);
//         std::string val = line.substr(eq + 1);

//         // whitespace
//         key.erase(remove_if(key.begin(), key.end(), ::isspace), key.end());
//         val.erase(val.begin(), find_if(val.begin(), val.end(), [](int c){return !isspace(c);}));  

//         m_values[key] = val;
//     }
//     return true;
// }

// std::string IniParser::Get(const std::string &key, const std::string &defaultVal) const
// {
//     auto it = m_values.find(key);
//     return it == m_values.end() ? defaultVal : it->second;
// }

// int IniParser::GetInt(const std::string &key, int def) const
// {
//     auto v = Get(key, "");
//     if (v == "") return def;
//     return std::stoi(v);
// }
