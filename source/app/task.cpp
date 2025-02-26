#include <ctime>
#include "lib/lib.hpp"
#include "app/task.hpp"

namespace EMMTask {

SendLogTaskLL::SendLogTaskLL(EMMLogger::Logger &logger, EMMLogger::LogLevel ll, std::time_t time, const std::string text) : logger(logger), logLevel(ll), time(time), text(text) {}

void SendLogTaskLL::run() {
	logger.log(logLevel, time, text);
}

SendLogTask::SendLogTask(EMMLogger::Logger &logger, std::time_t time, const std::string text) : logger(logger), time(time), text(text) {}

void SendLogTask::run() {
	logger.log(time, text);
}

void PoisonPillTask::run() {
	//TODO закрыть поток вывода.
}

ChangeLogLevelTask::ChangeLogLevelTask(EMMLogger::Logger &logger, EMMLogger::LogLevel ll) : logger(logger), logLevel(ll) {}

void ChangeLogLevelTask::run() {
	logger.setLogLevel(logLevel);
}

}	//namespace
