#ifndef HTTPROCESS_H_
#define HTTPROCESS_H_

#include "Httperrno.h"

class Http_Process {
private:
    size_t readmax;
    std::string document_root;
    Log* this_log;
    bool have_upper;
    HttpHeadConfig respone_head_config;

public:
    Http_Process() {}
    void SetLog(Log* log_p, size_t buffer_size);
    void SetHttpRespone(HttpHeadConfig config) { respone_head_config = config; }
    void SetReadmax(size_t readmax_) { readmax = readmax_; }
    void SetDocumentRoot(std::string document_root_) { document_root = document_root_; }
    std::string StrHttpState(int codenum);
    std::string FileType(std::string filename);
    void CreateResponeHead(Clientinfo* client, std::string filetype = "");
    void CreateBadHead(Clientinfo* client);
    REQUESTYPE RequestType(std::string* readbuf);
    //Use GET type parse readbuf and set fileinfo
    int GETParse(std::string readbuf, std::string* filename, Filestate* file);
    //Use POST type parse readbuf and set respone data
    int POSTParse(std::string request, std::string* post_type, std::string* post_data);
    //string to int for use switch
    REQUESTYPE POSTChoose(std::string post_type);
    ~Http_Process();
};

#endif