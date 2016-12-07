#include "./curl_session.h"
#include <exception>

using namespace std;

namespace imap_terminal
{
    CCurlSession::CCUrlRuntimeError::CCUrlRuntimeError(const std::string& message) :
        runtime_error(message)
    {
    
    }
    
    CCurlSession::CCurlSession()
    {
        __globalInit();
        m_EasyHandle = ::curl_easy_init();
        if (NULL == m_EasyHandle)
        {
            throw std::runtime_error("Failed to get cURL easy handle");
        }

        m_ErrorBuffer.resize(CURL_ERROR_SIZE + 1, 0);
        ::curl_easy_setopt(m_EasyHandle, CURLOPT_ERRORBUFFER, &(m_ErrorBuffer[0]));
    }

    CCurlSession::CCurlSession(const CCurlSession& otherSession)
    {
        throw logic_error("Should never get here");
    }
    
    CCurlSession::~CCurlSession()
    {
        if (NULL != m_EasyHandle)
        {
            ::curl_easy_cleanup(m_EasyHandle);
        }
    }

    const CURL* CCurlSession::easyHandle() const
    {
        return m_EasyHandle;
    }

    CURL* CCurlSession::easyHandle()
    {
        return m_EasyHandle;
    }

    void CCurlSession::raiseCurlRuntimeError() const
    {
        throw CCUrlRuntimeError(string(&(m_ErrorBuffer[0])));
    }

    void CCurlSession::__globalInit()
    {
        if (!CCurlSession::__global_init)
        {
            ::curl_global_init(CURL_GLOBAL_ALL);
        }
    }

    bool CCurlSession::__global_init = false;
}