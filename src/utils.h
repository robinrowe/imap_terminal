#ifndef __misc_utils__
#define __misc_utils__

#include <vector>
#include <string>

namespace imap_terminal
{
    class CUtils
    {
    public:
        static std::vector<std::string> tokenize(std::string src, 
            std::string delimiter, 
            char grouping = '\"');

    private:
    };
}

#endif