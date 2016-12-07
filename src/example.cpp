//
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#include <curl/curl.h>
//
//
//#undef FALSE
//#define FALSE 0
//#undef TRUE
//#define TRUE 1
//
//#define MAX_GMAIL_USER_LEN 256
//#define MAX_GMAIL_PASS_LEN 256
//
//struct memobj {
//    char *memory;
//    size_t size, allocated;
//};
//
//int ShowGmailFolders(const char *username, const char *password);
//size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata);
//
//int main(int argc, char *argv[])
//{
//    if (argc != 3) {
//        fprintf(stderr, "Usage: ShowGmailFolders <username> <password>\n");
//        return EXIT_FAILURE;
//    }
//
//    if (curl_global_init(CURL_GLOBAL_ALL)) {
//        fprintf(stderr, "Fatal: The initialization of libcurl has failed.\n");
//        return EXIT_FAILURE;
//    }
//
//    if (atexit(curl_global_cleanup)) {
//        fprintf(stderr, "Fatal: atexit failed to register curl_global_cleanup.\n");
//        curl_global_cleanup();
//        return EXIT_FAILURE;
//    }
//
//    if (!ShowGmailFolders(argv[1], argv[2])) {
//        fprintf(stderr, "Fatal: ShowGmailFolders failed.\n");
//        return EXIT_FAILURE;
//    }
//
//    return EXIT_SUCCESS;
//}
//
//size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata)
//{
//    size_t realsize = size * nmemb;
//    struct memobj *mem = (struct memobj *)userdata;
//
//    if (!realsize)
//        return 0;
//
//    if ((mem->size + realsize + 1) > mem->allocated) {
//        size_t bytes = 65536 + mem->size + realsize + 1;
//        char *temp = (char *)realloc(mem->memory, bytes);
//        if (!temp) {
//            fprintf(stderr, "Error: Out of memory, realloc returned NULL.");
//            return 0;
//        }
//        mem->memory = temp;
//        mem->allocated = bytes;
//    }
//
//    memcpy(&(mem->memory[mem->size]), ptr, realsize);
//    mem->size += realsize;
//    mem->memory[mem->size] = 0;
//
//    return realsize;
//}
//
//int ShowGmailFolders(const char *username, const char *password)
//{
//    int retcode = FALSE;
//    CURL *curl = NULL;
//    CURLcode res = CURLE_FAILED_INIT;
//    char errbuf[CURL_ERROR_SIZE] = { 0, };
//    struct memobj body = { 0, };
//    struct memobj headers = { 0, };
//    const char *const sep = "---------------------------------------------------"
//        "----------------------------";
//
//    if (!username || !*username) {
//        fprintf(stderr, "Error: Missing username.\n");
//        goto cleanup;
//    }
//    else if (!memchr(username, 0, MAX_GMAIL_USER_LEN)) {
//        fprintf(stderr, "Error: Username is greater than %d characters.\n",
//            MAX_GMAIL_USER_LEN);
//        goto cleanup;
//    }
//    else if (!password || !*password) {
//        fprintf(stderr, "Error: Missing password.\n");
//        goto cleanup;
//    }
//    else if (!memchr(password, 0, MAX_GMAIL_PASS_LEN)) {
//        fprintf(stderr, "Error: Password is greater than %d characters.\n",
//            MAX_GMAIL_PASS_LEN);
//        goto cleanup;
//    }
//
//    curl = curl_easy_init();
//    if (!curl) {
//        fprintf(stderr, "Error: curl_easy_init failed.\n");
//        goto cleanup;
//    }
//
//    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
//    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
//    curl_easy_setopt(curl, CURLOPT_PROTOCOLS, CURLPROTO_IMAPS);
//    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
//    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&body);
//    curl_easy_setopt(curl, CURLOPT_HEADERDATA, (void *)&headers);
//
//    curl_easy_setopt(curl, CURLOPT_USERNAME, username);
//    curl_easy_setopt(curl, CURLOPT_PASSWORD, password);
//    curl_easy_setopt(curl, CURLOPT_URL, "imaps://imap.gmail.com:993/");
//    curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errbuf);
//
//    res = curl_easy_perform(curl);
//    if (res != CURLE_OK) {
//        size_t len = strlen(errbuf);
//        fprintf(stderr, "\nlibcurl: (%d) ", res);
//        if (len)
//            fprintf(stderr, "%s%s", errbuf, ((errbuf[len - 1] != '\n') ? "\n" : ""));
//        fprintf(stderr, "%s\n\n", curl_easy_strerror(res));
//        goto cleanup;
//    }
//
//    printf("\n\n%s\n", sep);
//    printf("Headers:\n\n%s", headers.memory ? headers.memory : "");
//    printf("\n%s\n", sep);
//
//    printf("\n\n%s\n", sep);
//    printf("Body:\n\n%s", body.memory ? body.memory : "");
//    printf("\n%s\n", sep);
//
//    retcode = TRUE;
//cleanup:
//    free(body.memory);
//    free(headers.memory);
//    curl_easy_cleanup(curl);
//    return retcode;
//}