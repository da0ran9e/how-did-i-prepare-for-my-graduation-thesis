#ifndef INI_PARSER_H
#define INI_PARSER_H

#include <string>
#include <vector>
#include <istream>
#include <functional>

// -------------------------------------------------------------
// IniParser: skeleton class inspired by OMNeT++ InifileReader
// -------------------------------------------------------------
class IniParser
{
public:
    IniParser();

    // Read from filename
    void read(const std::string &filename);

    // Read a file directly
    void readFile(const std::string &filename);

    // Read from text buffer
    void readText(const std::string &text,
                  const std::string &filename = "",
                  const std::string &baseDir = "");

    // Read from stream
    void readStream(std::istream &in,
                    const std::string &filename,
                    const std::string &baseDir);

private:
    // Core recursive reader (handles include stack)
    void doReadFile(const std::string &filename,
                    std::vector<std::string> &includeStack);

    // Core parsing logic for lines coming from a stream
    void doReadFromStream(std::istream &in,
                          const std::string &filename,
                          std::vector<std::string> &includeStack,
                          const std::string &absoluteFilename,
                          const std::string &baseDir);

    // Utility: process lines with continuation
    void forEachJoinedLine(
        std::istream &in,
        const std::function<void(std::string &, int, int)> &processLine);

    // Utility: find where content ends (before comment '#')
    const char *findEndContent(const char *line,
                               const char *filename,
                               int lineNumber);

    // Utility: trim spaces from right
    void rtrim(std::string &str);

    // Utility: trim spaces from left and right
    std::string trim(const char *start, const char *end);

private:
    // Optional: You may store callback functions here if needed later
    // For example:
    // std::function<void(const std::string &section)> onSection;
    // std::function<void(const std::string &key, const std::string &value)> onKeyValue;

};

#endif // INI_PARSER_H
