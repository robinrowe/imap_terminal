#ifndef __curl_exception__
#define __curl_exception__

#include <curl/curl.h>
#include <exception>

namespace imap_terminal
{
    class CCurlException : public std::exception
    {
    public:
        CCurlException(CURLcode err);
        virtual ~CCurlException() throw();
        
        virtual const char* what() const throw();

        static void testCurlCode(CURLcode);

    private:
        CURLcode m_nError;
    };
}

#endif