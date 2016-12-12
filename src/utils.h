#ifndef __misc_utils__
#define __misc_utils__

#include <vector>
#include <string>

namespace imap_terminal
{
    class CUtils
    {
    public:
        static std::vector<std::string> tokenize(std::string src, std::string delimiters);
        static std::vector<std::string> cmdline(std::string src, std::string delimiters = "\x20\x09", char grouping = '\"');
        static std::string trim(const std::string& str);

    private:
    };
}

#endif