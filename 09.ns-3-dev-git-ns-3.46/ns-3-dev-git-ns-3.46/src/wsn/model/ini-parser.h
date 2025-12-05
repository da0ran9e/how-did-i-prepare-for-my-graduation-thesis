#ifndef INI_PARSER_H
#define INI_PARSER_H

#include <string>
#include <vector>
#include <istream>
#include <functional>

using SectionCallback = std::function<void(const std::string &section)>;
using KeyValueCallback = std::function<void(const std::string &key, const std::string &value, const std::string &comment, const std::string &baseDir)>;

class IniParser
{
public:
    IniParser();
    ~IniParser();

    void setSectionCallback(SectionCallback cb);
    void setKeyValueCallback(KeyValueCallback cb);

    void read(const std::string &filename);
    void readFile(const std::string &filename);
    void readText(const std::string &text, const std::string &filename = "", const std::string &baseDir = "");
    void readStream(std::istream &in, const std::string &filename, const std::string &baseDir);
    std::string getFilename() const { return m_filename; }
    void setFilename(const std::string &filename) { m_filename = filename; }

private: 
    std::string m_filename;
    void doReadFile(const std::string &filename, std::vector<std::string> &includeStack); 
    void doReadFromStream(std::istream &in, const std::string &filename, std::vector<std::string> &includeStack, const std::string &absoluteFilename, const std::string &baseDir);
    void forEachJoinedLine(std::istream &in, const std::function<void(std::string &, int, int)> &processLine);
    const char *findEndContent(const char *line, const char *filename, int lineNumber); 
    void rtrim(std::string &str); 
    std::string trim(const char *start, const char *end);

private:
    SectionCallback m_onSection;
    KeyValueCallback m_onKeyValue;

};

#endif // INI_PARSER_H
