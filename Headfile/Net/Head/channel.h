#ifndef NET_CHANNEL_H_
#define NET_CHANNEL_H_

#include "../../Class/noncopyable.h"
#include <functional>

class EventLoop;

namespace Wasi {
	namespace Net {
		class Channel : Noncopyable {
		private:
			using EventCallBack = std::function<void()>;
			void update();
			static const int none_event;
			static const int read_event;
			static const int write_event;
			EventLoop* loop;
			const int fd;
			int events;
			int revents;
			int index;
			EventCallBack read_callback;
			EventCallBack write_callback;
			EventCallBack error_callback;
		public:
			Channel(EventLoop* loop_, int fd_);
			void HandleEvent();
			void Update();
			void SetReadCallback(const EventCallBack& cb);
			void SetWriteCallback(const EventCallBack& cb);
			void SetErrorCallback(const EventCallBack& cb);
			void SetCloseCallback(const EventCallBack& cb);
		};
	}
}



#endif // !NET_CHANNEL_H_

