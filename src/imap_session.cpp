#include "./imap_session.h"
#include "./curl_exception.h"
#include "./utils.h"
#include <iostream>
#include <regex>
#include <sstream>
#include <cstdio>

using namespace std;

namespace imap_terminal
{
    CImapSession::CImapSession(
        const std::string& username,
        const std::string& password,
        const std::string& host,
        const std::string& port,
        bool ssl,
        int limit) : 
        m_sHost(host),
        m_sPort(port),
        m_sUsername(username),
        m_bUseSSL(ssl),
        m_nLimit(limit)
    {
        if (ssl)
        {
            CCurlException::testCurlCode(::curl_easy_setopt(easyHandle(), CURLOPT_PROTOCOLS, CURLPROTO_IMAPS));
            CCurlException::testCurlCode(::curl_easy_setopt(easyHandle(), CURLOPT_SSL_VERIFYPEER, 0L));
            CCurlException::testCurlCode(::curl_easy_setopt(easyHandle(), CURLOPT_SSL_VERIFYHOST, 0L));
        }
        else
        {
            CCurlException::testCurlCode(::curl_easy_setopt(easyHandle(), CURLOPT_PROTOCOLS, CURLPROTO_IMAP));
        }

        CCurlException::testCurlCode(::curl_easy_setopt(easyHandle(), CURLOPT_USERNAME, username.c_str()));
        CCurlException::testCurlCode(::curl_easy_setopt(easyHandle(), CURLOPT_PASSWORD, password.c_str()));
        
        __testLogin();
    }

    CImapSession::~CImapSession()
    {
    
    }

    std::string CImapSession::pwd() const
    {
        return __path(m_CurrentPath);
    }

    std::string CImapSession::ls(portable::CommandLine& cmdLine)
    {
        string sListing;
        if (cmdLine.empty())
        {
            //no command line args - list subdirs
            m_CurrentOperation = new CListSubdirOperation(__path(m_CurrentPath));
            __runOperation();
            sListing = static_cast<CListSubdirOperation*>(m_CurrentOperation)->listing();
            delete m_CurrentOperation; m_CurrentOperation = NULL;
        }
        
        if (m_CurrentPath.size() > 0)
        {
            sListing += __runMessageOperation<CListMessageOperation>(cmdLine);
        }
        
        return sListing;
    }

    std::string CImapSession::rm(portable::CommandLine& cmdLine)
    {
        return __runMessageOperation<CRemoveMessageOperation>(cmdLine);
    }

    std::string CImapSession::head(portable::CommandLine& cmdLine)
    {
        return __runMessageOperation<CHeadOperation>(cmdLine);
    }

    std::string CImapSession::mkdir(const std::string& dir)
    {
        if (CUtils::tokenize(dir, "/").size() > 1)
        {
            return string("This command allows only creating a subdirectory inside current directory.");
        }

        m_CurrentOperation = new CMakeDirectoryOperation(dir, __path(m_CurrentPath));
        __runOperation();
        delete m_CurrentOperation;
        m_CurrentOperation = NULL;

        return string("");
    }

    std::string CImapSession::rmdir(const std::string& dir)
    {
        if (CUtils::tokenize(dir, "/").size() > 1)
        {
            return string("This command allows only removing a subdirectory inside current directory.");
        }

        m_CurrentOperation = new CRemoveDirectoryOperation(dir, __path(m_CurrentPath));
        __runOperation();
        delete m_CurrentOperation;
        m_CurrentOperation = NULL;

        return string("");
    }

    std::string CImapSession::cd(const std::string& dir)
    {
        if (!__checkDirectoryExists(dir))
        {
            return string("No such file or directory");
        }

        m_CurrentPath = CUtils::tokenize(__absPath(dir), "/");
        return string("");
    }

    std::string CImapSession::mv(portable::CommandLine& cmdLine)
    {
        string sOutput;
        if (cmdLine.size() < 2)
        {
            return "Failed to parse command line";
        }

        if (cmdLine.operand(0) != NULL && cmdLine.operand(1) == NULL)
        {
            //operand(0) is destination 
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

                        m_CurrentOperation = new CMoveMessageOperation(__path(m_CurrentPath), nMaxUid, this, cmdLine.operand());
                        __runOperation();
                        
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
        else if (cmdLine.operand(0) != NULL && cmdLine.operand(1) != NULL)
        {
            bool f = true;
            int nUID;
            std::istringstream(cmdLine.operand(0)) >> std::dec >> nUID;
            if (nUID < 0)
            {
                f = false;
                std::istringstream(cmdLine.operand(1)) >> std::dec >> nUID;

                if (nUID <= 0)
                {
                    return "Failed to parse command line";
                }
            }

            m_CurrentOperation = new CMoveMessageOperation(__path(m_CurrentPath), nUID, this, 
                f ? cmdLine.operand(1) : cmdLine.operand(0));
            __runOperation();
            delete m_CurrentOperation; m_CurrentOperation = NULL;
        }
        else
        {
            return "Failed to parse command line";
        }
        return sOutput;
    }

    bool CImapSession::__checkDirectoryExists(const std::string& dir)
    {
        string absPathToCheck = __absPath(dir);
        
        if (absPathToCheck == "/")
        {
            return true;
        }
        else
        {
            try
            {
                m_CurrentOperation = new CCheckDirectoryOperation(absPathToCheck);
                __runOperation();
                bool f = (static_cast<CCheckDirectoryOperation*>(m_CurrentOperation)->directoryData().length() <= 0);
                delete m_CurrentOperation;
                m_CurrentOperation = NULL;

                if (f)
                {
                    return false;
                }
                
            }
            catch (const exception&)
            {
                delete m_CurrentOperation;
                m_CurrentOperation = NULL;
                return false;
            }
        }
        return true;
    }

    void CImapSession::handleData(std::string data)
    {
        m_CurrentOperation->completionRoutine(data);
    }

    

    const std::string& CImapSession::host() const
    {
        return m_sHost;
    }

    
    std::string CImapSession::whoami() const
    {
        return m_sUsername;
    }

    std::string CImapSession::limit(const std::string& arg)
    {
        if (arg.length() > 0)
        {
            istringstream(arg) >> std::dec >> m_nLimit;
            return string("");
        }
        else
        {
            ostringstream os;
            os << std::dec << m_nLimit;
            return os.str();
        }
    }

    std::string CImapSession::__path(const std::vector < std::string >& vec) const
    {
        string pwd = "/";
        vector<string>::const_iterator i;
        for (i = vec.begin(); i != vec.end(); i++)
        {
            pwd += string((i == vec.begin()) ? "" : "/") + *i;
        }

        return pwd;
    }

    void CImapSession::__testLogin()
    {
        m_CurrentOperation = new CCheckLoginOperation();
        try
        {
            __runOperation();
        }
        catch (...)
        {
            delete m_CurrentOperation;
            m_CurrentOperation = NULL;

            throw;
        }
        delete m_CurrentOperation;
        m_CurrentOperation = NULL;
    }

    std::string CImapSession::__absPath(const std::string& dir) const
    {
        string absPathToCheck;
        if (dir.at(0) == '/')
        {
            absPathToCheck = dir;
        }
        else
        {
            vector<string> absPathToCheckVec = m_CurrentPath;
            vector<string> relPath = CUtils::tokenize(dir, "/");
            vector<string>::iterator i;
            for (i = relPath.begin(); i != relPath.end(); i++)
            {
                if (*i == ".")
                {

                }
                else if (*i == "..")
                {
                    absPathToCheckVec.pop_back();
                }
                else
                {
                    absPathToCheckVec.push_back(*i);
                }
            }

            absPathToCheck = __path(absPathToCheckVec);
        }

        return absPathToCheck;
    }

    void CImapSession::__runOperation()
    {
        string url = (m_bUseSSL ? string("imaps://") : string("imap://")) +
            m_sHost +
            string(":") +
            m_sPort;

        char* pUrl = ::curl_easy_escape(easyHandle(), m_CurrentOperation->path().c_str(), m_CurrentOperation->path().length());
        string escapedPath = (pUrl == NULL) ? string("") : string(pUrl);
        ::curl_free(pUrl);

        escapedPath = regex_replace(escapedPath, regex("\\%2F"), "/");
        escapedPath = regex_replace(escapedPath, regex("\\%3B"), ";");
        escapedPath = regex_replace(escapedPath, regex("\\%3D"), "=");
        escapedPath = regex_replace(escapedPath, regex("\\%25"), "%");
        escapedPath = regex_replace(escapedPath, regex("\\%28"), "(");
        escapedPath = regex_replace(escapedPath, regex("\\%29"), ")");

        url += escapedPath;
        
        CCurlException::testCurlCode(::curl_easy_setopt(const_cast<CURL*>(easyHandle()), CURLOPT_URL, (url).c_str()));

        if (m_CurrentOperation->command().length() > 0)
        {
            CCurlException::testCurlCode(::curl_easy_setopt(easyHandle(), 
                CURLOPT_CUSTOMREQUEST, m_CurrentOperation->command().c_str()));
        }

        try
        {
            perform();
        }
        catch (...)
        {
            CCurlException::testCurlCode(::curl_easy_setopt(easyHandle(), CURLOPT_CUSTOMREQUEST, NULL));
            throw;
        }

        CCurlException::testCurlCode(::curl_easy_setopt(easyHandle(), CURLOPT_CUSTOMREQUEST, NULL));
    }

    

    
}
