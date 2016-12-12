#ifndef __imap_session__
#define __imap_session__

#include "./curl_session.h"
#include "./CommandLine.h"
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
            bool ssl = true,
            int limit = 20);
        virtual ~CImapSession();

        std::string pwd() const;
        std::string ls(portable::CommandLine& );
        std::string cd(const std::string& dir = ".");
        std::string whoami() const;
                
        const std::string& host() const;

    protected:
        virtual void handleData(std::string data);

    private:

        class COperation
        {
        public:
            enum EOperationType
            {
                ECheckDirectoryExists       = 0,
                ECheckLogin                 = 1,
                EDirectoryListing           = 2,
                EMessageListing             = 3
            };

            virtual ~COperation();

            virtual void completionRoutine(const std::string& data);

            const EOperationType& type() const;
            const std::string& path() const;
            const std::string& command() const;

            EOperationType& type();
            std::string& path();
            std::string& command();

        protected:
            COperation(EOperationType type,
                const std::string& path = "/",
                const std::string& command = "");

            

        private:
            EOperationType m_Type;
            std::string m_sPath;
            std::string m_sCommand;

        };

        class CCheckLoginOperation : public COperation
        {
        public:
            CCheckLoginOperation();
            
        };

        class CCheckDirectoryOperation : public COperation
        {
        public:
            CCheckDirectoryOperation(const std::string& path);
            virtual void completionRoutine(const std::string& data);

            const std::string& directoryData() const;
            const int& maxUid() const;
        private:
            std::string m_sDirData;
            int m_nMaxUid;
        };

        class CListSubdirOperation : public COperation
        {
        public:
            CListSubdirOperation(const std::string& path);
            virtual void completionRoutine(const std::string& data);

            const std::string& listing() const;
        private:
            std::string m_sListing;
        };

        class CListMessageOperation : public COperation
        {
        public:
            CListMessageOperation(const std::string& path, int uid, 
                const std::string& from = "",
                const std::string& to = "",
                const std::string& subject = "");

            virtual void completionRoutine(const std::string& data);

            const std::string& listing() const;
        private:
            std::string m_sListing;
            int m_nUid;
            std::string m_sFrom;
            std::string m_sTo;
            std::string m_sSubject;
        };

        std::string m_sHost;
        std::string m_sPort;
        std::string m_sUsername;
        bool m_bUseSSL;
        int m_nLimit;
        COperation* m_CurrentOperation;
        std::vector<std::string> m_CurrentPath;

        std::string __path(const std::vector < std::string >& vec ) const;
        std::string __absPath(const std::string& dir) const;
        bool __checkDirectoryExists(const std::string& dir);
        void __testLogin();
        void __runOperation();
    };
}

#endif