#ifndef PROTOCOL_H_
#define PROTOCOL_H_

namespace HTTP
{
    struct Sender
    {
        int sockfd;
        char* message_head;
        char* message_body;
        int total_length;
        int body_length;
        int sent_length;
        void Reset()
        {
            message_head = nullptr;
            message_body = nullptr;
            total_length = 0;
            body_length = 0;
            sent_length = 0;
        }
    };
}

#endif