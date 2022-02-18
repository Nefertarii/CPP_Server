#ifndef NET_CHANNEL_H_
#define NET_CHANNEL_H_

#include "../../../Class/noncopyable.h"
#include <functional>

namespace Wasi {
	namespace Net {
		class EventLoop;
		class Channel : Noncopyable {
		private:
			using EventCallBack = std::function<void()>;
			void Update_();
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
			EventCallBack close_callback;
		public:
			Channel(EventLoop* loop_, int fd_);
			int Fd();
			int Index();
			int Events();
			int Revents();
			void Handle_event();
			void Update();
			void Set_revents(int revents_);
			void Set_index(int index_);
			void Set_read_callback(const EventCallBack& cb);
			void Set_write_callback(const EventCallBack& cb);
			void Set_error_callback(const EventCallBack& cb);
			void Set_close_callback(const EventCallBack& cb);
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

