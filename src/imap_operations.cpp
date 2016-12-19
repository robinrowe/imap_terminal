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
        while (regex_search(m_sDirData, sm, r))
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


    CImapSession::CMoveMessageOperation::CMoveMessageOperation(
        const std::string& sPath,
        int uid,
        CImapSession* pSession,
        const std::string& destination) :
        CImapSession::CMessageOperation(sPath, uid),
        m_pSession(pSession),
        m_sDestinationPath(destination)
    {
        ostringstream os;
        os << path() << ";UID=" << uid;
        path() = os.str();

        m_sDestinationPath = m_pSession->__absPath(m_sDestinationPath);
        CCurlException::testCurlCode(::curl_easy_setopt(m_pSession->easyHandle(), CURLOPT_FORBID_REUSE, 1L));
    }

    CImapSession::CMoveMessageOperation::~CMoveMessageOperation()
    {
        CCurlException::testCurlCode(::curl_easy_setopt(m_pSession->easyHandle(), CURLOPT_FORBID_REUSE, 0L));
    }

    void CImapSession::CMoveMessageOperation::completionRoutine(const std::string& data)
    {
        m_pSession->m_CurrentOperation = new CCheckDirectoryOperation(m_sDestinationPath);
        m_pSession->__runOperation();
        int nNewUid = static_cast<CCheckDirectoryOperation*>(m_pSession->m_CurrentOperation)->maxUid() + 1;
        delete m_pSession->m_CurrentOperation;

        ostringstream os;
        os << std::dec << nNewUid;
        /*m_sDestinationPath = (m_sDestinationPath.at(m_sDestinationPath.length() - 1) == '/') ?
            m_sDestinationPath + os.str() :
            m_sDestinationPath + string("/") + os.str();*/
        m_sDestinationPath += string(";UID=") + os.str();
        
        try
        {
            m_pSession->m_CurrentOperation = new CCreateMessageOperation(
                m_sDestinationPath, data, m_pSession);
            m_pSession->__runOperation();
            delete m_pSession->m_CurrentOperation;
        }
        catch (...)
        {
            delete m_pSession->m_CurrentOperation;
            m_pSession->m_CurrentOperation = NULL;

            throw;
        }

        m_pSession->m_CurrentOperation = new CRemoveMessageOperation(
            CUtils::tokenize(path(), ";")[0], m_nUid);
        m_pSession->__runOperation();
        delete m_pSession->m_CurrentOperation;

        m_pSession->m_CurrentOperation = this;
    }

    CImapSession::CCreateMessageOperation::CCreateMessageOperation(
        const std::string& sPath, const std::string& message, CImapSession* pSession) :
        CImapSession::CMessageOperation(sPath, -1),
        m_sMessage(message),
        m_pSession(pSession)
    {
        __prepareTransfer();
    }

    CImapSession::CCreateMessageOperation::~CCreateMessageOperation()
    {
        __unprepareTransfer();
    }

    void CImapSession::CCreateMessageOperation::__prepareTransfer()
    {
        CCurlException::testCurlCode(::curl_easy_setopt(m_pSession->easyHandle(), CURLOPT_READFUNCTION, read_callback));
        CCurlException::testCurlCode(::curl_easy_setopt(m_pSession->easyHandle(), CURLOPT_READDATA, this));
        CCurlException::testCurlCode(::curl_easy_setopt(m_pSession->easyHandle(), CURLOPT_UPLOAD, 1L));
        CCurlException::testCurlCode(::curl_easy_setopt(m_pSession->easyHandle(), CURLOPT_INFILESIZE, m_sMessage.length()));
    }

    void CImapSession::CCreateMessageOperation::__unprepareTransfer()
    {
        CCurlException::testCurlCode(::curl_easy_setopt(m_pSession->easyHandle(), CURLOPT_READFUNCTION, NULL));
        CCurlException::testCurlCode(::curl_easy_setopt(m_pSession->easyHandle(), CURLOPT_READDATA, NULL));
        CCurlException::testCurlCode(::curl_easy_setopt(m_pSession->easyHandle(), CURLOPT_UPLOAD, 0L));
        CCurlException::testCurlCode(::curl_easy_setopt(m_pSession->easyHandle(), CURLOPT_INFILESIZE, 0L));
    }

    size_t CImapSession::CCreateMessageOperation::read_callback(char *buffer, size_t size, size_t nitems, void *data)
    {
        CCreateMessageOperation* pOp = reinterpret_cast<CCreateMessageOperation*>(data);

        if (pOp->m_sMessage.length() <= 0)
        {
            return 0;
        }

        ::memcpy(buffer, &(pOp->m_sMessage[0]), (size * nitems <  pOp->m_sMessage.length()) ? size * nitems : pOp->m_sMessage.length());
        if (size * nitems < pOp->m_sMessage.length())
        {
            pOp->m_sMessage = pOp->m_sMessage.substr(size * nitems, pOp->m_sMessage.length() - size * nitems);
            return size * nitems;
        }
        else
        {
            return pOp->m_sMessage.length();
        }
    }

    void CImapSession::CCreateMessageOperation::completionRoutine(const std::string& data)
    {
        __unprepareTransfer();
    }


    void CImapSession::CHeadOperation::completionRoutine(const std::string& data)
    {
        m_sListing = data.substr(0, (data.length() <  500) ? data.length() : 500);
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
