#ifndef EMM_COMMAND_HEADER
#define EMM_COMMAND_HEADER

#include <string>
#include <ctime>
#include "lib/lib.hpp"

namespace EMMTask {

/*
	Интерфейс для задач.
*/
class ITask {
public:
	virtual void run() = 0;
	virtual ~ITask() = default;
};

/*
	Задача для отправки лога с уровнем логирования.
*/
class SendLogTaskLL : public ITask {
	EMMLogger::Logger &logger;
	EMMLogger::LogLevel logLevel;
	std::time_t time;
	const std::string text;
public: 
	SendLogTaskLL(EMMLogger::Logger &logger, EMMLogger::LogLevel ll, std::time_t time, const std::string text);
	void run() override;
};

/*
	Задача для отправки лога без уровня логирования.
*/
class SendLogTask : public ITask {
	EMMLogger::Logger &logger;
	std::time_t time;
	const std::string text;
public: 
	SendLogTask(EMMLogger::Logger &logger, std::time_t time, const std::string text);
	void run() override;
};

/*
	Задача для изменения уровня логирования по умолчанию в логере.
*/
class ChangeLogLevelTask : public ITask {
	EMMLogger::Logger &logger;
	EMMLogger::LogLevel logLevel;
public:
	ChangeLogLevelTask(EMMLogger::Logger &logger, EMMLogger::LogLevel ll);
	void run() override;
};

}	//namespace

#endif
