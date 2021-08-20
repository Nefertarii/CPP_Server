#include "Servprocess.h"

const char* Connectprocess::Str_error(int codenum) {
    return Serverr_map[-codenum % ERREND];
}
const char* Connectprocess::Str_state(int codenum) {
    return Servstate_map[codenum % STATEEND];
}