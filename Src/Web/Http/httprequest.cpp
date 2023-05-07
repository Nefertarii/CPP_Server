#include "Include/Web/Http/httprequest.h"

using namespace wasi;
using namespace wasi::web;

HttpRequest::HttpRequest() :
    request_data_(),
    file_stat_(),
    origin_request_("\0"),
    accept_types_() {}

HttpRequest::HttpRequest(std::string request) :
    request_data_(),
    file_stat_(),
    origin_request_(request),
    accept_types_() {}

HttpRequest::HttpRequest(const HttpRequest& other) {
    this->request_data_   = other.request_data_;
    this->file_stat_      = other.file_stat_;
    this->origin_request_ = other.origin_request_;
    this->accept_types_   = other.accept_types_;
}

HttpRequest& HttpRequest::operator=(const HttpRequest& other) {
    this->request_data_   = other.request_data_;
    this->file_stat_      = other.file_stat_;
    this->origin_request_ = other.origin_request_;
    this->accept_types_   = other.accept_types_;
    return *this;
}

HttpRequestHead HttpRequest::GetRequestData() { return request_data_; }

std::string HttpRequest::GetRequest() { return origin_request_; }

head::FileStat HttpRequest::GetFileStat() { return file_stat_; }

std::map<FileType, bool> HttpRequest::GetAcceptTypes() { return accept_types_; }

void HttpRequest::SetRequestData(const HttpRequestHead& request_head) {
    request_data_ = request_head;
}

void HttpRequest::SetHttpRequest(const std::string& request_str) {
    origin_request_ = request_str;
}

void HttpRequest::SetFileStat(const head::FileStat& file_stat) {
    file_stat_ = file_stat;
}

void HttpRequest::SetAcceptTypes(const std::map<FileType, bool>& accept_types) {
    accept_types_ = accept_types;
}

void HttpRequest::SetExistFields(const HttpRequest& other) {
    request_data_.SetExistFields(other.request_data_);
    file_stat_.SetExistFields(other.file_stat_);
    if (!other.origin_request_.empty()) {
        this->origin_request_ = other.origin_request_;
    }
    if (!other.accept_types_.empty()) {
        this->accept_types_ = other.accept_types_;
    }
}

void HttpRequest::Clear() {
    origin_request_.clear();
    accept_types_.clear();
    file_stat_.Clear();
    request_data_ = HttpRequestHead();
}

HttpRequest::~HttpRequest() {}