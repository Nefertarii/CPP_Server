#include "Head/core.h"

//using namespace Wasi;
using namespace Wasi::Log;

Core::Core() {
	log_size = 0;
	log_capacity = 200;
	Set_concurrency(false);
	sink.Set_in_filter(LOG_NONE);
	sink.Set_out_filter(LOG_NONE);
	record.Set_filename("Default_log.txt");
}

Core::Core(bool flag, uint capacity, LogLevel in_filter,
	LogLevel out_filter, std::string filename) {
	log_size = 0;
	log_capacity = capacity;
	Set_concurrency(flag);
	sink.Set_in_filter(in_filter);
	sink.Set_out_filter(out_filter);
	record.Set_filename(filename);
}

bool Core::Add_log(LogLevel log_level, std::string log_from, std::string log_detail) {
	if (log_size + 1 >= log_capacity) { return false; }
	if (sink.Log_add(log_level, clock.Nowtime_sec(), log_from, log_detail) == true) {
		log_size += 1;
		return true;
	}
	return false;
}

void Core::Set_log_in_filter(LogLevel level) { sink.Set_in_filter(level); }

void Core::Set_log_out_filter(LogLevel level) { sink.Set_out_filter(level); }

void Core::Set_save_file(std::string filename) { record.Set_filename(filename); }

void Core::Set_concurrency(bool flag) {
	formatter.Set_flag(flag);
	sink.Set_flag(flag);
	record.Set_flag(flag);
}

void Core::Set_capacity(uint size) { log_capacity = size; }

bool Core::Save_to_file() {
	//consume
	std::string write_befroe = clock.Sec_to_string(clock.Nowtime_sec());
	write_befroe += "Logline:" + std::to_string(log_size) + "\n";
	record.Save_to_file(write_befroe);
	sink.Log_consume(&logs_tmp);
	for (uint i = 0; i < log_size; i++) {
		logs.push(formatter.Trans_log(logs_tmp.front()));
		logs_tmp.pop();
	}
	log_size = 0;
	return record.Save_to_file(&logs);
}