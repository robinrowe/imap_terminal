#include "./curl_exception.h"

using namespace std;

namespace imap_terminal
{
    CCurlException::CCurlException(CURLcode err) : 
        m_nError(err)
    {
    
    }

    CCurlException::~CCurlException() throw()
    {
    
    }

    const char* CCurlException::what() const throw()
    {
        return ::curl_easy_strerror(m_nError);
    }

    void CCurlException::testCurlCode(CURLcode code)
    {
        if (code != CURLE_OK)
        {
            throw CCurlException(code);
        }
    }
}