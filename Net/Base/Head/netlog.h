#ifndef BASE_LOG_H_
#define BASE_LOG_H_

#include "../../../Log/Head/core.h"

namespace Wasi {
	namespace Base {
		class Logging {
		private:
			Wasi::Log::Core logcore;
		public:
			Logging();
			bool Normal_log(std::string log_from, std::string log_detail);
			bool Notifi_log(std::string log_from, std::string log_detail);
			bool Warning_log(std::string log_from, std::string log_detail);
			bool Error_log(std::string log_from, std::string log_detail);
			bool Critical_log(std::string log_from, std::string log_detail);
			~Logging();
		};
	}
}

#endif // !NET_LOG_H_