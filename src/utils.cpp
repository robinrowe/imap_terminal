#include "./utils.h"
#include <regex>

using namespace std;

namespace imap_terminal
{
    vector<string> CUtils::tokenize(string src, string delimiters, char grouping)
    {
        vector<string> tokens;
        regex r(string("[^") + delimiters + string("]+"));
        smatch sm;
        while (regex_search(src, sm, r))
        {
            tokens.push_back(sm.str());
            src = sm.suffix();
        }

        return tokens;
    }

}