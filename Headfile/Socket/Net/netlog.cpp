#include "Head/netlog.h"

using namespace Wasi::Socket;

bool Logging::Normal_log(std::string log_from, std::string log_detail) {
	if (!logcore.Add_log(Wasi::Log::LOG_NORMAL, log_from, log_detail)) {
		logcore.Save_to_file();
		return false;
	}
	return true;
}

bool Logging::Notifi_log(std::string log_from, std::string log_detail) {
	if (!logcore.Add_log(Wasi::Log::LOG_NOTIFI, log_from, log_detail)) {
		logcore.Save_to_file();
		return false;
	}
	return true;
}

bool Logging::Warning_log(std::string log_from, std::string log_detail) {
	if (!logcore.Add_log(Wasi::Log::LOG_WARNING, log_from, log_detail)) {
		logcore.Save_to_file();
		return false;
	}
	return true;
}

bool Logging::Error_log(std::string log_from, std::string log_detail) {
	if (!logcore.Add_log(Wasi::Log::LOG_ERROR, log_from, log_detail)) {
		logcore.Save_to_file();
		return false;
	}
	return true;
}

bool Logging::Critical_log(std::string log_from, std::string log_detail) {
	if (!logcore.Add_log(Wasi::Log::LOG_CRITICAL, log_from, log_detail)) {
		logcore.Save_to_file();
		return false;
	}
	return true;
}

Logging::~Logging() {
	logcore.Save_to_file();
}