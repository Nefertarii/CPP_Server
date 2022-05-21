#include "httprespone.h"

using namespace Wasi::Http;

HttpRespone::HttpRespone() :
    respone_head(),
    prepare_respone_head(),
    respone_body(),
    respone_file() {}

void HttpRespone::Init() {
    respone_head.charset            = HttpCharset::UNKNOWN;
    respone_head.http_version       = HttpVersion::UNKNOWN;
    respone_head.code_num           = HttpCode::CODE404;
    respone_head.server             = "Test Server";
    respone_head.connection         = ConnectionType::UNKNOWN;
    respone_head.keep_alive_timeout = 0;
    respone_head.keep_alive_max     = 0;
    respone_head.content_type       = FileType::PLAIN;
    respone_head.content_length     = 0;
    respone_head.last_modified      = 0;
    respone_head.date               = 0;
}

HttpRespone::~HttpRespone() {}