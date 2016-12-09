#ifndef __imap_terminal__
#define __imap_terminal__

#include "./imap_session.h"

namespace imap_terminal
{
    class CImapTerminal : public CImapSession
    {
    public:
        CImapTerminal(
            const std::string& username,
            const std::string& password,
            const std::string& host,
            const std::string& port = "993",
            bool ssl = true);

        virtual ~CImapTerminal();
    private:
        
        std::string __readLine() const;
        std::vector<std::string> __parseLine(const std::string& line) const;
        bool __executeLine(const std::vector<std::string>& line);
        void __run();
    };
}

#endif