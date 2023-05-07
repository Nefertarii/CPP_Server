#ifndef WEB_HTTP_HTTPCONTEXT_H_
#define WEB_HTTP_HTTPCONTEXT_H_

#include "Include/Web/Http/httphead.h"
#include <memory>

namespace wasi {
namespace web {

class HttpRespone;
class HttpRequest;

class HttpContext {
private:
    std::shared_ptr<HttpRequest> request_;
    std::shared_ptr<HttpRespone> respone_;
    size_t repeat_num_;
    size_t repeat_max_;
    int repeat_time_; //ms
    ResponeSendState state_;

public:
    HttpContext();
    std::weak_ptr<HttpRespone> GetRespone();
    std::weak_ptr<HttpRequest> GetRequest();
    size_t GetRepeatNum();
    size_t GetRepeatMax();
    int GetRepeatTime();
    ResponeSendState GetState();
    // true, create new. false, copy exist value
    void AddRepeatNum();
    void SetRepeatNum(size_t num);
    void SetRepeatMax(size_t num);
    void SetRepeatTime(int time);
    void ResetRepeatNum();
    void SetRespone(HttpRespone new_respone);
    void SetRequest(HttpRequest new_request);
    void SetState(ResponeSendState state);
    void Clear();
    ~HttpContext();
};

}} // namespace wasi::web

#endif