#ifndef POLL_CHANNEL_H_
#define POLL_CHANNEL_H_

#include "../../../Class/noncopyable.h"
#include <functional>

namespace Wasi {
	namespace Time {
		class TimeStamp;
	}
	namespace Poll {
		class EventLoop;
		class Channel : Noncopyable {
		private:
			using EventCallBack = std::function<void()>;
			using ReadEventCallBack = std::function<void(Time::TimeStamp)>;
			void Update();
			static const int none_event;
			static const int read_event;
			static const int write_event;
			EventLoop* loop;
			const int fd;
			bool in_loop;
			int events;
			int revents;
			int index;
			ReadEventCallBack read_callback;
			EventCallBack write_callback;
			EventCallBack error_callback;
			EventCallBack close_callback;
		public:
			Channel(EventLoop* loop_, int fd_);
			int Fd();
			int Index();
			int Events();
			int Revents();
			//void Remove():
			void Handle_event(Time::TimeStamp receive_time);
			void Set_revents(int revents_);
			void Set_index(int index_);
			void Set_read_callback(ReadEventCallBack cb);
			void Set_write_callback(EventCallBack cb);
			void Set_error_callback(EventCallBack cb);
			void Set_close_callback(EventCallBack cb);
			void Enable_reading();
			void Disable_reading();
			void Enable_writing();
			void Disable_writing();
			bool Is_none_event();
			EventLoop* Owner_loop();
			~Channel();
		};
	}
}



#endif // !NET_CHANNEL_H_

