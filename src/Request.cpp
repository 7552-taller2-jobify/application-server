// "Copyright 2016 <Jobify>"

#include <string>
#include "Request.h"

size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream) {
    std::string data((const char*) ptr, (size_t) size * nmemb);
    *((std::stringstream*) stream) << data << std::endl;
    return size * nmemb;
}

Request::Request() {
    slist1 = NULL;
}

void Request::SetContentJson() {
  slist1 = curl_slist_append(slist1, "Authorization:key=AIzaSyAkvyuxBXTzeLiz9wkE2WrAsfyk0ylkQOk");
  slist1 = curl_slist_append(slist1, "Accept: application/json");
  slist1 = curl_slist_append(slist1, "Content-Type: application/json");
  // headers = curl_slist_append(headers, "charsets: utf-8");
}

void Request::SetAuthorization(std::string authorization) {
  std::string value = "Authorization:" + authorization;
  slist1 = curl_slist_append(slist1, value.c_str() );
}

Response*  Request::Execute(const std::string url) {
    curl_global_init(CURL_GLOBAL_DEFAULT);

    curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    /* example.com is redirected, so we tell libcurl to follow redirection */
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);  // Prevent "longjmp causes uninitialized stack frame" bug
    curl_easy_setopt(curl, CURLOPT_ACCEPT_ENCODING, "deflate");


// https
#ifdef SKIP_PEER_VERIFICATION
    /*
     * If you want to connect to a site who isn't using a certificate that is
     * signed by one of the certs in the CA bundle you have, you can skip the
     * verification of the server's certificate. This makes the connection
     * A LOT LESS SECURE.
     *
     * If you have a CA cert for the server stored someplace else than in the
     * default bundle, then the CURLOPT_CAPATH option might come handy for
     * you.
     */
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
#endif

#ifdef SKIP_HOSTNAME_VERIFICATION
    /*
     * If the site you're connecting to uses a different host name that what
     * they have mentioned in their server certificate's commonName (or
     * subjectAltName) fields, libcurl will refuse to connect. You can skip
     * this check, but this will make the connection less secure.
     */
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
#endif

    // https
    std::stringstream out;
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &out);
    /* Perform the request, res will get the return code */
    CURLcode res = curl_easy_perform(curl);

    int http_code = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);

    /* Check for errors */
    if (res != CURLE_OK) {
        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
    }
    curl_global_cleanup();

    Response* response = new Response();
    response->setContent(out.str());
    response->setStatus(http_code);

    return response;
}

struct WriteThis {
    const char *readptr;
    int sizeleft;
};

static size_t read_callback(void *ptr, size_t size, size_t nmemb, void *userp) {
    struct WriteThis *pooh = (struct WriteThis *)userp;

    if (size*nmemb < 1) {
        return 0;
    }
    if (pooh->sizeleft) {
        *reinterpret_cast<char *>(ptr) = pooh->readptr[0]; /* copy one single byte */
        pooh->readptr++;                 /* advance pointer */
        pooh->sizeleft--;                /* less data left */
        return 1;                        /* we return 1 byte at a time! */
    }

    return 0;                          /* no more data left to deliver */
}

Response* Request::ExecutePost(std::string url, std::string body) {
    std::stringstream out;
    CURLcode ret;
    slist1 = curl_slist_append(slist1, "Content-Type: application/json");
    curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body.c_str());
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "curl/7.38.0");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, slist1);
    curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 50L);
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
    curl_easy_setopt(curl, CURLOPT_TCP_KEEPALIVE, 1L);

  //  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
  //  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &out);
    ret = curl_easy_perform(curl);

    int http_code = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);

  //  curl_slist_free_all(slist1);
  //  slist1 = NULL;

    Response* response = new Response();
    response->setContent(out.str());
    response->setStatus(http_code);

    return response;
  }

Request::~Request() {
    curl = NULL;
    curl_easy_cleanup(curl);
}
