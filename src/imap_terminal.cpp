#include "./imap_terminal.h"
#include <iostream>

using namespace std;

namespace imap_terminal
{
    CImapTerminal::CImapTerminal(
        const std::string& username,
        const std::string& password,
        const std::string& host,
        const std::string& port,
        bool ssl) : 
        CImapSession(username, password, host, port, ssl)
    {
        __run();
    }

    CImapTerminal::~CImapTerminal()
    {
    
    }

    std::string CImapTerminal::__readLine() const
    {
        cout << absolutePath() << "$ ";

        std::string line;
        getline(cin, line);

        return line;
    }

    std::vector<std::string> CImapTerminal::__parseLine(const std::string& line) const
    {
        vector<string> tokens;
        string token;
        for (string::size_type i = 0; i < line.length(); i++)
        {
            bool whitesp = __isWhitespace(line.at(i));
            if (whitesp || i == line.length() - 1)
            {
                if (!whitesp)
                {
                    token += line.at(i);
                }

                if (token.length() > 0)
                {
                    tokens.push_back(token);
                    token = "";
                }
            }
            else
            {
                token += line.at(i);
            }
        }

        return tokens;
    }

    bool CImapTerminal::__executeLine(const std::vector<std::string>& line)
    {
        if (line[0] == "exit")
        {
            return false;
        }

        /*vector<string>::const_iterator i;
        for (i = line.begin(); i != line.end(); i++)
        {
            cout << "<" << *i << ">" << endl;
        }*/
        return true;
    }

    void CImapTerminal::__run()
    {
        while (1)
        {
            string line = __readLine();
            vector<string> tokens = __parseLine(line);

            if (tokens.size() == 0)
            {
                continue;
            }

            if (!__executeLine(tokens))
            {
                break;
            }
        }
    }

    bool CImapTerminal::__isWhitespace(std::string::value_type c)
    {
        int n = c;
        if (n == 0x20 || n == 0x09 || n == 0x0A || n == 0x0D)
        {
            return true;
        }
        return false;
    }
}