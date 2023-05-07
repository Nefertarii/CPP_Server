#include "Include/Web/Http/httprespone.h"

using namespace wasi;
using namespace wasi::web;

HttpRespone::HttpRespone() :
    respone_head_("\0"),
    respone_line_("\0"),
    respone_body_("\0"),
    respone_file_(),
    reaspone_data_() {}

HttpRespone::HttpRespone(const HttpRespone& other) {
    this->respone_head_  = other.respone_head_;
    this->respone_line_  = other.respone_line_;
    this->respone_body_  = other.respone_body_;
    this->respone_file_  = other.respone_file_;
    this->reaspone_data_ = other.reaspone_data_;
}

HttpRespone& HttpRespone::operator=(const HttpRespone& other) {
    this->respone_head_  = other.respone_head_;
    this->respone_line_  = other.respone_line_;
    this->respone_body_  = other.respone_body_;
    this->respone_file_  = other.respone_file_;
    this->reaspone_data_ = other.reaspone_data_;
    return *this;
}

std::string HttpRespone::GetResponeHead() { return respone_head_; }

std::string HttpRespone::GetResponeLine() { return respone_line_; }

std::string HttpRespone::GetResponeBody() { return respone_body_; }

head::FileStat HttpRespone::GetResponeFile() { return respone_file_; }

HttpResponeHead HttpRespone::GetResponeDate() { return reaspone_data_; }

void HttpRespone::SetResponeHead(const std::string& respone_head) {
    respone_head_ = respone_head;
}

void HttpRespone::SetResponeLine(const std::string& respone_line) {
    respone_line_ = respone_line;
}

void HttpRespone::SetResponeBody(const std::string& respone_body) {
    respone_body_ = respone_body;
}

void HttpRespone::SetResponeFile(const head::FileStat& respone_file) {
    respone_file_ = respone_file;
}

void HttpRespone::SetResponeDate(const HttpResponeHead& date) {
    reaspone_data_ = date;
}

void HttpRespone::Clear() {
    respone_head_.clear();
    respone_line_.clear();
    respone_body_.clear();
    reaspone_data_ = HttpResponeHead();
}

HttpRespone::~HttpRespone() {}