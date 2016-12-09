#include "./imap_session.h"
#include "./curl_exception.h"
#include "./utils.h"
#include <tree_util.hh>
#include <iostream>
#include <regex>

using namespace std;

namespace imap_terminal
{
    CImapSession::CImapSession(
        const std::string& username,
        const std::string& password,
        const std::string& host,
        const std::string& port,
        bool ssl) : 
        m_sHost(host),
        m_sPort(port),
        m_sUsername(username)
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
        
        string url = (ssl ? string("imaps://") : string("imap://")) +
            host +
            string(":") +
            port +
            string("/");
        
        CCurlException::testCurlCode(::curl_easy_setopt(easyHandle(), CURLOPT_URL, (url).c_str()));

        m_ExpectState = EExpectListing;
        perform();
    }

    CImapSession::~CImapSession()
    {
    
    }

    std::string CImapSession::pwd() const
    {
        string pwd;
        tree<CMailboxObject>::iterator iter = m_CurrentPosition;
        while (1)
        {
            pwd += string("/") + (*iter).name(); 
            if (iter == m_MailboxStructure.begin())
            {
                break;
            }

            iter = m_MailboxStructure.parent(iter);
        }

        vector<string> elements = CUtils::tokenize(pwd, "/");
        vector<string>::reverse_iterator r_iter;
        pwd = "";
        for (r_iter = elements.rbegin(); r_iter != elements.rend(); r_iter++)
        {
            pwd += string("/") + *r_iter;
        }

        return pwd;
    }

    std::string CImapSession::ls(const std::string& dir) const
    {
        string sListing;        
        
        tree<CMailboxObject>::iterator iter = m_CurrentPosition;
        if (dir.at(0) == '/')
        {
            //user requested an absolute path
            iter = m_MailboxStructure.begin();
        }

        try
        {
            __moveTreePos(dir, iter);
        }
        catch (const exception& e)
        {
            return e.what();
        }

        tree<CMailboxObject>::sibling_iterator s_iter;
        for (s_iter = m_MailboxStructure.begin(iter); s_iter != m_MailboxStructure.end(iter); s_iter++)
        {
            sListing += (*s_iter).render() + "\n";
        }

        return sListing;
    }

    std::string CImapSession::cd(const std::string& dir)
    {
        if (dir.at(0) == '/')
        {
            //user requested an absolute path
            m_CurrentPosition = m_MailboxStructure.begin();
        }

        try
        {
            __moveTreePos(dir, m_CurrentPosition);
        }
        catch (const exception& e)
        {
            return e.what();
        }

        return string("");
    }

    void CImapSession::handleData(std::string data)
    {
        if (m_ExpectState == EExpectListing)
        {
            //reset local Mailbox structure
            if (m_MailboxStructure.number_of_siblings(m_MailboxStructure.begin()) > 0)
            {
                m_MailboxStructure.erase(m_MailboxStructure.begin());
            }

            CMailboxDirectory rootDir;
            tree<CMailboxObject>::iterator top;
            top = m_MailboxStructure.begin();
            m_CurrentPosition = m_MailboxStructure.insert(top, rootDir);

            regex imapListResponse("\\x2A\\s+LIST\\s+\\([\\\\a-zA-Z\\s]+\\)\\s+\"*([^\"]+)\"*\\s+\"*([^\"]+)\"*");
            smatch sm;
            while (regex_search(data, sm, imapListResponse))
            {
                string path = sm[2];
                string delimiter = sm[1];

                vector<string> pathElements = CUtils::tokenize(path, delimiter);
                vector<string>::iterator i;
                tree<CMailboxObject>::sibling_iterator iterBegin = m_MailboxStructure.begin(m_CurrentPosition);
                tree<CMailboxObject>::sibling_iterator iterEnd = m_MailboxStructure.end(m_CurrentPosition);
                for (i = pathElements.begin(); i != pathElements.end(); i++)
                {
                    tree<CMailboxObject>::sibling_iterator loc;
                    for (loc = iterBegin; loc != iterEnd; loc++)
                    {
                        CMailboxObject& o1 = *loc;
                        CMailboxObject& o2 = CMailboxObject(*i);
                        if (o1 == o2)
                        {
                            break;
                        }
                    }

                    if (loc == iterEnd)
                    {
                        loc = m_MailboxStructure.insert(iterBegin, CMailboxDirectory(*i));
                    }

                    iterBegin = m_MailboxStructure.begin(loc);
                    iterEnd = m_MailboxStructure.end(loc);
                }

                data = sm.suffix();
            }

            //kptree::print_tree_bracketed(m_MailboxStructure);
        }
    }

    CImapSession::CMailboxObject::CMailboxObject()
    {
    
    }

    CImapSession::CMailboxObject::CMailboxObject(const std::string& name) :
        m_sName(name)
    {
    
    }

    CImapSession::CMailboxObject::~CMailboxObject()
    {
    
    }

    std::string CImapSession::CMailboxObject::render() const
    {
        return m_sName;
    }
    
    bool CImapSession::CMailboxObject::operator==(const CMailboxObject& other) const
    {
        return m_sName == other.m_sName;
    }

    const std::string& CImapSession::CMailboxObject::name() const
    {
        return m_sName;
    }

    std::string& CImapSession::CMailboxObject::name()
    {
        return m_sName;
    }

    CImapSession::CMailboxDirectory::CMailboxDirectory(const std::string& name) : 
        CImapSession::CMailboxObject(name)
    {
    
    }

    CImapSession::CMailboxDirectory::CMailboxDirectory()
    {
    
    }

    std::ostream& operator<<(std::ostream& out, const CImapSession::CMailboxObject& obj)
    {
        return out << obj.render();
    }

    const std::string& CImapSession::host() const
    {
        return m_sHost;
    }

    void CImapSession::__moveTreePos(const std::string& where, tree<CMailboxObject>::iterator& pos) const
    {
        string badDir = "No such file or directory";
        
        vector<string> dirs = CUtils::tokenize(where, "/");
        vector<string>::iterator i;
        for (i = dirs.begin(); i != dirs.end(); i++)
        {
            if (*i == ".")
            {

            }
            else if (*i == "..")
            {
                if (pos == m_MailboxStructure.begin())
                {
                    throw runtime_error(badDir);
                }
                pos = m_MailboxStructure.parent(pos);
            }
            else
            {
                tree<CMailboxObject>::iterator s_iter =
                    find(m_MailboxStructure.begin(pos), m_MailboxStructure.end(pos), CMailboxDirectory(*i));
                if (s_iter != m_MailboxStructure.end(pos))
                {
                    pos = s_iter;
                }
                else
                {
                    throw runtime_error(badDir);
                }
            }
        }
    }

    std::string CImapSession::whoami() const
    {
        return m_sUsername;
    }
}