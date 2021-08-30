#include "Serverrno.h"

const char *Serverr_map[] = {
    [-ERRNONE] = "undefine server error.",
    [-WRITEFAIL] = "send respone fail."
};

const char *Servstate_map[] = {
    [STATENONE] = "undefine server state."
};

const char *Request_map[] = {
    [TYPENONE] = "undefine requeset type.",
    [GET] = "GET",
    [POST] = "POST",
};

const char *Errnoprint::Str_error(SERVERR err_code) { return Serverr_map[-err_code % ERREND]; }

const char *Errnoprint::Str_state(SERVSTATE state_code) { return Servstate_map[state_code % STATEND]; }

const char *Errnoprint::Str_type(REQUESTYPE requset_type) { return Request_map[requset_type % TYPEND]; }