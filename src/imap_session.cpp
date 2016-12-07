#include "./imap_session.h"
#include "./curl_exception.h"

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
        CURLcode curlResult = CURLE_OK;
        
        CCurlException::testCurlCode(::curl_easy_setopt(easyHandle(), CURLOPT_SSL_VERIFYPEER, 0L));
        CCurlException::testCurlCode(::curl_easy_setopt(easyHandle(), CURLOPT_SSL_VERIFYHOST, 0L));

        if (ssl)
        {
            CCurlException::testCurlCode(::curl_easy_setopt(easyHandle(), CURLOPT_PROTOCOLS, CURLPROTO_IMAPS));
        }
        else
        {
            CCurlException::testCurlCode(::curl_easy_setopt(easyHandle(), CURLOPT_PROTOCOLS, CURLPROTO_IMAP));
        }

        CCurlException::testCurlCode(::curl_easy_setopt(easyHandle(), CURLOPT_USERNAME, username));
        CCurlException::testCurlCode(::curl_easy_setopt(easyHandle(), CURLOPT_PASSWORD, password));
        CCurlException::testCurlCode(
            ::curl_easy_setopt(
                easyHandle(),
                CURLOPT_URL,
                ((ssl ? string("imaps://") : string("imap://")) +
                    host +
                    string(":") +
                    port +
                    string("/")
                    ).c_str()));
    }

    CImapSession::~CImapSession()
    {
    
    }
}