#ifndef EMM_COMMAND_HEADER
#define EMM_COMMAND_HEADER

#include <string>
#include <ctime>
#include "lib/lib.hpp"

namespace EMMTask {

class ITask {
public:
	virtual void run() {}
	virtual ~ITask() = default;
};

class SendLogTaskLL : public ITask {
	EMMLogger::Logger &logger;
	EMMLogger::LogLevel logLevel;
	std::time_t time;
	const std::string text;
public: 
	SendLogTaskLL(EMMLogger::Logger &logger, EMMLogger::LogLevel ll, std::time_t time, const std::string text);
	void run() override;
};

class SendLogTask : public ITask {
	EMMLogger::Logger &logger;
	std::time_t time;
	const std::string text;
public: 
	SendLogTask(EMMLogger::Logger &logger, std::time_t time, const std::string text);
	void run() override;
};

class PoisonPillTask : public ITask {
public:
	void run() override;
};

class ChangeLogLevelTask : public ITask {
	EMMLogger::Logger &logger;
	EMMLogger::LogLevel logLevel;
public:
	ChangeLogLevelTask(EMMLogger::Logger &logger, EMMLogger::LogLevel ll);
	void run() override;
};

}	//namespace

#endif
