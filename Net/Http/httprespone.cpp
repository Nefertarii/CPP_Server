#include "Head/httprespone.h"

using namespace Wasi::Http;

HttpRespone::HttpRespone() :
    respone_head(),
    prepare_respone_head(),
    respone_body(),
    respone_file() {}

HttpRespone::~HttpRespone() {}