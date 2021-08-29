#ifndef SERVERRNO_H_
#define SERVERRNO_H_

enum REQUESTYPE
{
    TYPENONE = 0,
    GET,
    POST,
    TYPEND = (1 << 30)
};

enum SERVERR
{
    ERRNONE = 0,
    WRITEFAIL = -1,
    ERREND = -(1 << 30)
};

enum SERVSTATE
{
    STATENONE = 0,
    STATEND = (1 << 30)
};

static const char *Serverr_map[] = {
    [-ERRNONE] = "undefine server error.",
    [-WRITEFAIL] = "send respone fail."
};

static const char *Servstate_map[] = {
    [STATENONE] = "undefine server state."
};

static const char *Request_map[] = {
    [TYPENONE] = "undefine requeset type.",
    [GET] = "GET",
    [POST] = "POST",
};

const char *Str_error(SERVERR err_code) {
    return Serverr_map[-err_code % ERREND];
}

const char *Str_state(SERVSTATE state_code) {
    return Servstate_map[state_code % STATEND];
}

const char *Str_type(REQUESTYPE requset_type) {
    return Servstate_map[requset_type % TYPEND];
}

#endif