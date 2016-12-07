#ifndef __curl_session__
#define __curl_session__

#include <string>
#include <exception>
#include <vector>
#include <curl/curl.h>

namespace imap_terminal
{
    class CCurlSession
    {
    public:
        CCurlSession();
        virtual ~CCurlSession();

        class CCUrlRuntimeError : public std::runtime_error
        {
        public:
            CCUrlRuntimeError(const std::string& message);
        };

    protected:
        const CURL* easyHandle() const;
        CURL* easyHandle();
        void raiseCurlRuntimeError() const;

    private:
        static void __globalInit();
        static bool __global_init;

        CCurlSession(const CCurlSession&);
        
        CURL* m_EasyHandle;
        std::vector<char> m_ErrorBuffer;

    };
}

#endif