#ifndef TOKENS_HPP
#define TOKENS_HPP

    // C system header files
extern "C"
{

}
    // C++ system header files

    // the different answer codes
const unsigned int STATUS_CODE_CONTINUE = 101;
const unsigned int STATUS_CODE_SWITCHING_PROTOCOLS = 101;
const unsigned int STATUS_CODE_OK = 200;
const unsigned int STATUS_CODE_CREATED = 201;
const unsigned int STATUS_CODE_ACCEPTED = 202;
const unsigned int STATUS_CODE_NON_AUTHORITATIVE_INFORMATION = 203;
const unsigned int STATUS_CODE_NO_CONTENT = 204;
const unsigned int STATUS_CODE_RESET_CONTENT = 205;
const unsigned int STATUS_CODE_PARTIAL_CONTENT = 206;
const unsigned int STATUS_CODE_MULTIPLE_CHOICES = 300;
const unsigned int STATUS_CODE_MOVED_PERMANENTLY = 301;
const unsigned int STATUS_CODE_MOVED_TEMPORARILY = 302;
const unsigned int STATUS_CODE_NOT_MODIFIED = 304;
const unsigned int STATUS_CODE_USE_PROXY = 305;
const unsigned int STATUS_CODE_BAD_REQUEST = 400;
const unsigned int STATUS_CODE_UNAUTHORIZED = 401;
const unsigned int STATUS_CODE_FORBIDDEN = 403;
const unsigned int STATUS_CODE_NOT_FOUND = 404;
const unsigned int STATUS_CODE_METHOD_NOT_ALLOWED = 405;
const unsigned int STATUS_CODE_NOT_ACCEPTABLE = 406;
const unsigned int STATUS_CODE_PROXY_AUTHENTICATION_REQUIRED = 407;
const unsigned int STATUS_CODE_REQUEST_TIME_OUT = 408;
const unsigned int STATUS_CODE_CONFLICT = 409;
const unsigned int STATUS_CODE_GONE = 410;
const unsigned int STATUS_CODE_LENGTH_REQUIRED = 411;
const unsigned int STATUS_CODE_PRECONDITION_FAILED = 412;
const unsigned int STATUS_CODE_REQUEST_ENTITY_TOO_LARGE = 413;
const unsigned int STATUS_CODE_REQUEST_URI_TOO_LARGE = 414;
const unsigned int STATUS_CODE_UNSUPPORTED_MEDIA_TYPE = 415;
const unsigned int STATUS_CODE_INTERNAL_SERVER_ERROR = 500;
const unsigned int STATUS_CODE_NOT_IMPLEMENTED = 501;
const unsigned int STATUS_CODE_BAD_GATEWAY = 502;
const unsigned int STATUS_CODE_SERVICE_UNAVAILABLE = 503;
const unsigned int STATUS_CODE_GATEWAY_TIME_OUT = 504;
const unsigned int STATUS_CODE_HTTP_VERSION_NOT_SUPPORTED = 505;


    // HTTP headers

extern const char *HDR_CONTENT_LENGTH;
extern const char *HDR_IF_MODIFIED_SINCE;
extern const char *HDR_LAST_MODIFIED;
extern const char *HDR_CONTENT_TYPE;
extern const char *HDR_DATE;
extern const char *HDR_EXPIRES;
extern const char *HDR_SERVER;
extern const char *HDR_WWW_AUTHENTICATE;
extern const char *HDR_AUTHORIZATION;
extern const char *HDR_SET_COOKIE;
extern const char *HDR_COOKIE;
extern const char *HDR_LOCATION;

    // HTTP header values
extern const char *VAL_CONTENT_TYPE_FORM;


    // HTML CSS colors by fonction
extern const char *COLOR_BACK;
extern const char *COLOR_TEXT;
extern const char *COLOR_PADBACK;
extern const char *COLOR_PADFRONT;
extern const char *COLOR_PADBORD;
#endif
