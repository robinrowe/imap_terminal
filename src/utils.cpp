#include "./utils.h"
#include <regex>
#include <sstream>
#include <exception>
#include <ios>
#include  <iomanip>

using namespace std;

namespace imap_terminal
{
    vector<string> CUtils::tokenize(string src, string delimiters)
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

    std::vector<std::string> CUtils::cmdline(std::string src, const std::string& delimiters, char grouping)
    {
        ostringstream hexGrouping;
        hexGrouping << "\\x" << setfill('0') << setw(2) << std::hex << (int)grouping;
        string sHexGrp = hexGrouping.str();
        src = regex_replace(src, regex(string("\\\\") + string(1, grouping)), hexGrouping.str());

        ostringstream groupedSequence;
        bool inGrouping = false;
        for (string::size_type i = 0; i < src.length(); ++i)
        {
            if (src.at(i) == grouping)
            {
                inGrouping = !inGrouping;

            }
            else
            {
                if (inGrouping)
                {
                    bool inDelimiters = false;
                    for (string::size_type j = 0; j < delimiters.length(); ++j)
                    {
                        if (src.at(i) == delimiters.at(j))
                        {
                            inDelimiters = true;
                            break;
                        }
                    }

                    if (inDelimiters)
                    {
                        vector<char> buf;
                        buf.resize(5, 0);
                        ::sprintf(&(buf[0]), "\\x%02X", src.at(i));

                        string s = string(&(buf[0]));

                        groupedSequence << s;
                    }
                    else
                    {
                        groupedSequence << src.at(i);
                    }
                }
                else
                {
                    groupedSequence << src.at(i);
                }
            }
        }

        if (inGrouping)
        {
            throw runtime_error("Failed to parse the command line");
        }

        vector<string> tokens = tokenize(groupedSequence.str(), delimiters);
        vector<string> _tokens;
        vector<string>::iterator iter;
        for (iter = tokens.begin(); iter != tokens.end(); iter++)
        {
            string token = *iter;
            string _token;
            regex r("\\\\x([0-9a-fA-F]{2})");
            smatch sm;

            while (regex_search(token, sm, r))
            {
                _token += sm.prefix();

                istringstream is;
                string s = sm[1];
                int nChar;
                istringstream(s) >> std::hex >> nChar;

                _token.push_back(nChar);
                
                token = sm.suffix();
            }

            _token += token;
            _tokens.push_back(_token);

            
        }

        return _tokens;
    }

    string CUtils::trim(const string& str)
    {
        size_t first = str.find_first_not_of(' ');
        if (string::npos == first)
        {
            return str;
        }
        size_t last = str.find_last_not_of(' ');
        return str.substr(first, (last - first + 1));
    }

}