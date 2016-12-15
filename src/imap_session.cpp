#include "./imap_session.h"
#include "./curl_exception.h"
#include "./utils.h"
#include <iostream>
#include <regex>
#include <sstream>

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

    CImapSession::COperation::COperation(
        const std::string& path,
        const std::string& command) : 
        m_sPath(path),
        m_sCommand(command)
    {
    
    }

    CImapSession::COperation::~COperation()
    {
    
    }

    void CImapSession::COperation::completionRoutine(const std::string& data)
    {
        
    }

    const std::string& CImapSession::COperation::path() const
    {
        return m_sPath;
    }

    const std::string& CImapSession::COperation::command() const
    {
        return m_sCommand;
    }

    std::string& CImapSession::COperation::path()
    {
        return m_sPath;
    }

    std::string& CImapSession::COperation::command()
    {
        return m_sCommand;
    }

    CImapSession::CCheckLoginOperation::CCheckLoginOperation() : 
        CImapSession::COperation()
    {

    }

    CImapSession::CCheckDirectoryOperation::CCheckDirectoryOperation(const std::string& sPath) : 
        CImapSession::COperation(sPath)
    {
        command() = string("EXAMINE \"") + path().substr(1, path().length() - 1) + string("\"");
        path() = "";
    }

    void CImapSession::CCheckDirectoryOperation::completionRoutine(const std::string& data)
    {
        m_sDirData = data;
        regex r("([0-9]+) EXIST");
        smatch sm;
        while(regex_search(m_sDirData, sm, r))
        {
            istringstream strm(sm.str());
            strm >> m_nMaxUid;
            break;
        }
    }

    const std::string& CImapSession::CCheckDirectoryOperation::directoryData() const
    {
        return m_sDirData;
    }

    const int& CImapSession::CCheckDirectoryOperation::maxUid() const
    {
        return m_nMaxUid;
    }

    CImapSession::CListSubdirOperation::CListSubdirOperation(const std::string& path) : 
        CImapSession::COperation(path)
    {
    
    }

    void CImapSession::CListSubdirOperation::completionRoutine(const std::string& sData)
    {
        m_sListing = "";
        string data = sData;
        vector<string> pathToList = CUtils::tokenize(path(), "/");
        regex imapListResponse("\\x2A\\s+LIST\\s+\\([\\\\a-zA-Z\\s]+\\)\\s+\"*([^\"]+)\"*\\s+\"*([^\"]+)\"*");
        smatch sm;
        while (regex_search(data, sm, imapListResponse))
        {
            string path = sm[2];
            string delimiter = sm[1];

            vector<string> pathElements = CUtils::tokenize(path, delimiter);
            if (pathElements.size() == pathToList.size() + 1)
            {
                m_sListing += string("d    ") + pathElements[pathElements.size() - 1] + "\n";
            }

            data = sm.suffix();
        }
    }

    const std::string& CImapSession::CListSubdirOperation::listing() const
    {
        return m_sListing;
    }

    CImapSession::CSelectMessageOperation::CSelectMessageOperation(const std::string& sPath, int uid,
        const std::string& from,
        const std::string& to,
        const std::string& subject) :
        CImapSession::COperation(sPath),
        m_sFrom(from),
        m_sTo(to),
        m_sSubject(subject),
        m_nUid(uid),
        m_fMatch(true)
    {
        ostringstream os;
        os << path() << ";UID=" << uid << ";SECTION=HEADER.FIELDS (FROM SUBJECT TO)";
        path() = os.str();
    }

    void CImapSession::CSelectMessageOperation::completionRoutine(const std::string& data)
    {
        string sData = data;

        regex r("(.+)\r*\n");
        smatch sm;

        string sFrom, sTo, sSubject;

        while (regex_search(sData, sm, r))
        {
            string sAttr = sm[1];
            smatch sm1;

            if (regex_search(sAttr, sm1, regex("[Ff]{1}[Rr]{1}[Oo]{1}[Mm]{1}:")))
            {
                sFrom = CUtils::trim(sm1.suffix());
            }
            else if (regex_search(sAttr, sm1, regex("[Tt]{1}[Oo]{1}:")))
            {
                sTo = CUtils::trim(sm1.suffix());
            }
            else if (regex_search(sAttr, sm1, regex("[Ss]{1}[Uu]{1}[Bb]{1}[Jj]{1}[Ee]{1}[Cc]{1}[Tt]{1}:")))
            {
                sSubject = CUtils::trim(sm1.suffix());
            }

            sData = sm.suffix();
        }

        m_fMatch = true;
        if (m_sFrom.length() > 0 && sFrom != m_sFrom ||
            m_sTo.length() > 0 && sTo != m_sTo ||
            m_sSubject.length() > 0 && sSubject != m_sSubject)
        {
            m_fMatch = false;
        }
    }

    bool CImapSession::CSelectMessageOperation::match() const
    {
        return m_fMatch;
    }

    CImapSession::CMessageOperation::CMessageOperation(const std::string& sPath, int uid) : 
        m_nUid(uid),
        CImapSession::COperation(sPath)
    {
        
    }

    CImapSession::CMessageOperation::~CMessageOperation()
    {
    
    }

    void CImapSession::CMessageOperation::completionRoutine(const std::string& data)
    {
    
    }

    const std::string& CImapSession::CMessageOperation::listing() const
    {
        return m_sListing;
    }

    CImapSession::CRemoveMessageOperation::CRemoveMessageOperation(const std::string& sPath, int uid) :
        CMessageOperation(sPath, uid)
    {
        ostringstream os;
        os << "STORE " << std::dec << uid << " +Flags \\Deleted";
        command() = os.str();
    }

    void CImapSession::CRemoveMessageOperation::completionRoutine(const std::string& data)
    {
    
    }
    
    CImapSession::CListMessageOperation::CListMessageOperation(const std::string& sPath, int uid) :
        CImapSession::CMessageOperation(sPath, uid)
    {
        ostringstream os;
        os << path() << ";UID=" << uid << ";SECTION=HEADER.FIELDS (FROM SUBJECT)";
        path() = os.str();
    }

    void CImapSession::CListMessageOperation::completionRoutine(const std::string& data)
    {
        string sData = data;
        ostringstream os;

        os << m_nUid << "\t";

        regex r("(.+)\r*\n");
        smatch sm;

        while (regex_search(sData, sm, r))
        {
            os << sm[1] << "\t";
            sData = sm.suffix();
        }

        m_sListing = os.str();
    }

    CImapSession::CHeadOperation::CHeadOperation(const std::string& sPath, int uid) : 
        CImapSession::CMessageOperation(sPath, uid)
    {
        ostringstream os;
        os << path() << ";UID=" << uid << ";SECTION=TEXT";
        path() = os.str();
    }

    void CImapSession::CHeadOperation::completionRoutine(const std::string& data)
    {
        m_sListing = data.substr(0, min(data.length(), 500) );
    }

    CImapSession::CMakeDirectoryOperation::CMakeDirectoryOperation(
        const std::string& newDir,
        const std::string& currentPath) : 
        CImapSession::COperation(currentPath, string("CREATE ") + newDir)
    {
    
    }

    CImapSession::CRemoveDirectoryOperation::CRemoveDirectoryOperation(
        const std::string& rmDir, 
        const std::string& currentPath) : 
        CImapSession::COperation(currentPath, string("DELETE ") + rmDir)
    {
    
    }

    
}
