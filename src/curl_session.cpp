#include "./curl_session.h"
#include "./curl_exception.h"
#include <exception>
#include <string.h>

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
        CCurlException::testCurlCode(::curl_easy_setopt(m_EasyHandle, CURLOPT_ERRORBUFFER, &(m_ErrorBuffer[0])));
        CCurlException::testCurlCode(::curl_easy_setopt(m_EasyHandle, CURLOPT_WRITEFUNCTION, write_callback));
        CCurlException::testCurlCode(::curl_easy_setopt(
            m_EasyHandle, 
            CURLOPT_WRITEDATA, 
            reinterpret_cast<void*>(this)));
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

    void CCurlSession::handleData(std::string data)
    {
        throw logic_error("CCurlSession::handleData");
    }

    void CCurlSession::perform()
    {
        m_sCurlOutput = "";
        CURLcode status = ::curl_easy_perform(m_EasyHandle);
        if (CURLE_OK != status)
        {
            if (string(&(m_ErrorBuffer[0])).length() > 0)
            {
                raiseCurlRuntimeError();
            }
            else
            {
                throw CCurlException(status);
            }
        }

        this->handleData(m_sCurlOutput);
    }

    void CCurlSession::__globalInit()
    {
        if (!CCurlSession::__global_init)
        {
            ::curl_global_init(CURL_GLOBAL_ALL);
        }
    }

    bool CCurlSession::__global_init = false;

    size_t CCurlSession::write_callback(char *ptr, size_t size, size_t nmemb, void *userdata)
    {
        vector<char> buf;
        buf.resize(size * nmemb + 1, 0);
        ::memcpy(&(buf[0]), ptr, size * nmemb);

        reinterpret_cast<CCurlSession*>(userdata)->m_sCurlOutput += string(&(buf[0]));
        return size * nmemb;
    }

    CCurlSession::CSList::CSList() : 
        m_pCurlSList(NULL)
    {
        
    }

    CCurlSession::CSList::CSList(const CCurlSession::CSList&)
    {
        throw logic_error("Should never get here");
    }

    CCurlSession::CSList::~CSList()
    {
        ::curl_slist_free_all(m_pCurlSList);
    }

    CCurlSession::CSList& CCurlSession::CSList::operator<<(const std::string& s)
    {
        m_pCurlSList = ::curl_slist_append(m_pCurlSList, s.c_str());
        return *this;
    }

    struct curl_slist* CCurlSession::CSList::slist()
    {
        return m_pCurlSList;
    }


}