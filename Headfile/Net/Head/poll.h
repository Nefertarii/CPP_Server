#ifndef NET_POLL_H_
#define NET_POLL_H_

#include "../../Class/noncopyable.h"
#include "../../Timer/Head/timer.h"
#include <vector>
#include <map>

class Channel;
class Eventloop;
struct pollfd;

namespace Wasi {
    namespace Net {
        class Poller : Noncopyable {
        private:
            void FillActiveChannel(int events, std::vector<Channel*> channels) const;
            //using std::vector<struct pollfd> PollFdList;
            //using std::map<int, Channel*> ChannelMap;
            Eventloop* ownerloop;
            std::vector<struct pollfd> pollfds;
            std::map<int, Channel*> channles;
        public:
            Poller(Eventloop* loop);
            ~Poller();
            Wasi::Time::TimeStamp Poll(int timeout_ms, std::vector<Channel*> channels);
            void UpdateChannel(Channel* channel);
        };
    }
}




#endif