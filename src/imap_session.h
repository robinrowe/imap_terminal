#ifndef __imap_session__
#define __imap_session__

#include "./curl_session.h"
#include "./CommandLine.h"
#include <vector>
#include <regex>
#include <sstream>
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
        std::string rm(portable::CommandLine&);
        std::string cd(const std::string& dir = ".");
        std::string whoami() const;
        std::string mkdir(const std::string& dir);
        std::string rmdir(const std::string& dir);
        std::string limit(const std::string& arg = "");
                
        const std::string& host() const;

    protected:
        virtual void handleData(std::string data);

    private:

        class COperation
        {
        public:
            virtual ~COperation();

            virtual void completionRoutine(const std::string& data);

            const std::string& path() const;
            const std::string& command() const;

            std::string& path();
            std::string& command();

        protected:
            COperation(
                const std::string& path = "/",
                const std::string& command = "");

            

        private:
            
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

        class CSelectMessageOperation : public COperation
        {
        public:
            CSelectMessageOperation(const std::string& path, int uid,
                const std::string& from = "",
                const std::string& to = "",
                const std::string& subject = "");

            virtual void completionRoutine(const std::string& data);

            bool match() const;

        private:
            bool m_fMatch;
            std::string m_sFrom;
            std::string m_sTo;
            std::string m_sSubject;
            int m_nUid;
        };

        class CMessageOperation : public COperation
        {
        public:
            virtual void completionRoutine(const std::string& data);
            const std::string& listing() const;
            virtual ~CMessageOperation();
            
        protected:
            CMessageOperation(const std::string& path, int uid);            

            std::string m_sListing;
            int m_nUid;
        };

        

        class CListMessageOperation : public CMessageOperation
        {
        public:
            CListMessageOperation(const std::string& path, int uid);

            virtual void completionRoutine(const std::string& data);
        };

        class CRemoveMessageOperation : public CMessageOperation
        {
        public:
            CRemoveMessageOperation(const std::string& path, int uid);

            virtual void completionRoutine(const std::string& data);

        };

        
        class CMakeDirectoryOperation : public COperation
        {
        public:
            CMakeDirectoryOperation(const std::string& newDir, const std::string& currentPath);

        };

        class CRemoveDirectoryOperation : public COperation
        {
        public:
            CRemoveDirectoryOperation(const std::string& rmDir, const std::string& currentPath);

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

        template <class T> std::string __runMessageOperation(portable::CommandLine& cmdLine)
        {
            std::string sOperand;
            std::string sOutput;
            if (cmdLine.operand() == NULL)
            {
                //operation must be run against a subset of messages inside current directory
                //that match -s -t -f criteria
                m_CurrentOperation = new CCheckDirectoryOperation(__path(m_CurrentPath));
                __runOperation();
                int nMaxUid = static_cast<CCheckDirectoryOperation*>(m_CurrentOperation)->maxUid();
                delete m_CurrentOperation; m_CurrentOperation = NULL;

                for (int i = 0; i < m_nLimit && nMaxUid > 0; i++, nMaxUid--)
                {
                    std::string from = (cmdLine.Get("-from") == NULL) ? "" : cmdLine.Get("-from");
                    if (from.length() <= 0)
                    {
                        from = (cmdLine.Get("-f") == NULL) ? "" : cmdLine.Get("-f");
                    }
                    
                    std::string to = (cmdLine.Get("-to") == NULL) ? "" : cmdLine.Get("-to");
                    if (to.length() <= 0)
                    {
                        to = (cmdLine.Get("-t") != NULL) ? cmdLine.Get("-t") : "";
                    }

                    std::string subject = (cmdLine.Get("-subject") == NULL) ? "" : cmdLine.Get("-s");
                    if (subject.length() <= 0)
                    {
                        subject = (cmdLine.Get("-s") != NULL) ? cmdLine.Get("-s") : "";
                    }

                    m_CurrentOperation = new CSelectMessageOperation(__path(m_CurrentPath), nMaxUid, from, to, subject);
                    try
                    {
                        __runOperation();
                        if (static_cast<CSelectMessageOperation*>(m_CurrentOperation)->match())
                        {
                            delete m_CurrentOperation; m_CurrentOperation = NULL;

                            m_CurrentOperation = new T(__path(m_CurrentPath), nMaxUid);
                            __runOperation();
                            sOutput += static_cast<T*>(m_CurrentOperation)->listing() + "\n";
                        }
                        
                        delete m_CurrentOperation; m_CurrentOperation = NULL;
                    }
                    catch (const exception& e)
                    {
                        delete m_CurrentOperation; m_CurrentOperation = NULL;
                        sOutput += std::string("ERROR: ") + std::string(e.what()) + "\n";
                        continue;
                    }
                }
            }
            else
            {
                std::regex rUID("[0-9]+");
                sOperand = cmdLine.operand();
                std::smatch sm;
                if (std::regex_match(sOperand, sm, rUID))
                {
                    //operation must be run against specific message with given UID
                    int nUID;
                    std::istringstream(sOperand) >> std::dec >> nUID;
                    m_CurrentOperation = new T(__path(m_CurrentPath), nUID);
                    try
                    {
                        __runOperation();
                        if (static_cast<T*>(m_CurrentOperation)->listing().length() > 0)
                        {
                            sOutput += static_cast<T*>(m_CurrentOperation)->listing() + "\n";
                        }
                        delete m_CurrentOperation; 
                        m_CurrentOperation = NULL;
                    }
                    catch (const exception& e)
                    {
                        delete m_CurrentOperation; m_CurrentOperation = NULL;
                        sOutput += std::string("ERROR: ") + std::string(e.what()) + "\n";
                    }
                }
                else
                {
                    throw std::runtime_error("Failed to parse command line");
                }
            }

            return sOutput;
        }
    };
}

#endif