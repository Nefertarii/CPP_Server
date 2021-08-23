#ifndef SERVERRNO_H_
#define SERVERRNO_H_

enum REQUESTYPE
{
    TYPENONE = 0,
    GET,
    POST
};

enum SERVERR
{
    ERRNONE = 0,
    WRITEFAIL = -1,
    ERREND = -(1<<30)
};

enum SERVSTATE
{
    STATENONE = 0,
    STATEEND = (1 << 30)
};

static const char *Serverr_map[] = {
    [-ERRNONE] = "undefine server error.",
    [-WRITEFAIL] = "send respone fail."
};

static const char *Servstate_map[] = {
    [STATENONE] = "undefine server state."
};

const char *Str_error(int codenum) {
    return Serverr_map[-codenum % ERREND];
}

const char *Str_state(int codenum) {
    return Servstate_map[codenum % STATEEND];
}

#endif