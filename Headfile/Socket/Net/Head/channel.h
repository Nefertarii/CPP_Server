#ifndef NET_CHANNEL_H_
#define NET_CHANNEL_H_

#include "../../../Class/noncopyable.h"
#include <functional>

namespace Wasi {
	namespace Socket {
		class Channel : Noncopyable {
		private:
			using EventCallBack = std::function<void()>;
		public:
		};
	}
}



#endif // !SOCKET_CHANNEL_H_

