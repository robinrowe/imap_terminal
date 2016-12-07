#ifndef __imap_session__
#define __imap_session__

#include "./curl_session.h"
#include <stack>
#include <string>

namespace imap_terminal
{
    class CImapSession : public CCurlSession
    {
    public:
        CImapSession(
            const std::string& username,
            const std::string& password,
            const std::string& host,
            const std::string& port = "993",
            bool ssl = true);

        virtual ~CImapSession();

    private:
        std::string m_sHost;
        std::string m_sPort;
        std::stack<std::string> m_RelativePath;
    };
}

#endif