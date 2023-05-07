#include "Include/Web/Http/httpcontext.h"
#include "Include/Web/Http/httprequest.h"
#include "Include/Web/Http/httprespone.h"

using namespace wasi::web;

HttpContext::HttpContext() :
    request_(std::make_shared<HttpRequest>()),
    respone_(std::make_shared<HttpRespone>()),
    repeat_num_(0),
    repeat_max_(10),
    repeat_time_(50.0),
    state_(ResponeSendState::kWait) {}

std::weak_ptr<HttpRespone> HttpContext::GetRespone() { return respone_; }

std::weak_ptr<HttpRequest> HttpContext::GetRequest() { return request_; }

size_t HttpContext::GetRepeatNum() { return repeat_num_; }

size_t HttpContext::GetRepeatMax() { return repeat_max_; }

int HttpContext::GetRepeatTime() { return repeat_time_; }

void HttpContext::AddRepeatNum() { repeat_num_ += 1; }

void HttpContext::SetRepeatNum(size_t num) { repeat_num_ = num; }

void HttpContext::SetRepeatMax(size_t num) { repeat_max_ = num; }

void HttpContext::SetRepeatTime(int time) { repeat_time_ = time; }

void HttpContext::ResetRepeatNum() { repeat_num_ = 0; }

ResponeSendState HttpContext::GetState() { return state_; }

void HttpContext::SetRespone(HttpRespone new_respone) {
    respone_ = std::make_shared<HttpRespone>(new_respone);
}

void HttpContext::SetRequest(HttpRequest new_request) {
    request_ = std::make_shared<HttpRequest>(new_request);
}

void HttpContext::SetState(ResponeSendState state) { state_ = state; }

void HttpContext::Clear() {
    request_->Clear();
    respone_->Clear();
    repeat_num_ = 0;
    state_      = ResponeSendState::kWait;
}

HttpContext::~HttpContext() {
    
}