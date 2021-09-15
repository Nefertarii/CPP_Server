#include "Httperrno.h"

const char* Serverr_map[] = {
    [-ERRNONE] = "undefine server error.",
    [-WRITEFAIL] = "send respone fail."
};

const char* Servstate_map[] = {
    [STATENONE] = "undefine server state."
};

const char* Request_map[] = {
    [TYPENONE] = "undefine requeset type.",
    [GET] = "GET",
    [POST] = "POST",
};

const char* Strerr::Str_error(SERVERR err_code) { return Serverr_map[-err_code % ERREND]; }

const char* Strerr::Str_state(SERVSTATE state_code) { return Servstate_map[state_code % STATEND]; }

const char* Strerr::Str_type(REQUESTYPE requset_type) { return Request_map[requset_type % TYPEND]; }

void Clientinfo::Reset() {
    port.clear();
    ip.clear();
    respone_head.clear();
    respone_body.clear();
    clientfd = 0;
    rewrite_count = 0;
    fileinfo.filefd = 0;
    fileinfo.filelength = 0;
    fileinfo.offset = 0;
    err_code = ERRNONE;
    state_code = STATENONE;
    requset_type = TYPENONE;
}