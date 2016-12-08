#include "./imap_session.h"
#include "./curl_exception.h"
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
        m_sPort(port)
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

    std::string CImapSession::absolutePath() const
    {
        string path = m_sHost + "/";
        vector<string>::const_iterator i;
        for (i = m_RelativePath.begin(); i != m_RelativePath.end(); i++)
        {
            path += *i + "/";
        }
        return path;
    }

    void CImapSession::handleData(std::string data)
    {
        //cout << data << endl;
    }
}