#ifndef __imap_session__
#define __imap_session__

#include "./curl_session.h"
#include <tree.hh>
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

        std::string pwd() const;
        std::string ls(const std::string& dir = ".") const;
        std::string cd(const std::string& dir = ".");
        std::string whoami() const;

        class CMailboxObject
        {
        public:
            CMailboxObject();
            CMailboxObject(const std::string& name);
            virtual ~CMailboxObject();

            virtual std::string render() const;
            virtual bool operator==(const CMailboxObject& other) const;

            const std::string& name() const;
            std::string& name();
        private:
            std::string m_sName;
        };

        const std::string& host() const;

        class CMailboxDirectory : public CMailboxObject
        {
        public:
            CMailboxDirectory(const std::string& name);
            CMailboxDirectory();
        };

        class CEmailMessage : public CMailboxObject
        {};

    protected:
        virtual void handleData(std::string data);

    private:
        enum EExpectState
        {
            EExpectListing          = 0,
            EExpectExamination      = 1
        };

        std::string m_sHost;
        std::string m_sPort;
        std::string m_sUsername;
        EExpectState m_ExpectState;
        tree<CMailboxObject> m_MailboxStructure;
        tree<CMailboxObject>::iterator m_CurrentPosition;

        void __moveTreePos(const std::string& where, tree<CMailboxObject>::iterator& pos) const;
    };


    std::ostream& operator<<(std::ostream& out, const CImapSession::CMailboxObject& obj);
}

#endif