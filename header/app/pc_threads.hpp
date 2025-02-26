#ifndef EMM_PC_THREADS
#define EMM_PC_THREADS

#include <iostream>
#include <string>
#include <memory>
#include <ctime>
#include "lib/lib.hpp"
#include "app/task.hpp"
#include "app/task_queue.hpp"

namespace EMM_PCThreads {

static const char* logLevelDecodeString = "-ll=%d";

class Stoper : public EMMTask::ITask {
	std::shared_ptr<bool> aliveFlag;
public:
	Stoper(std::shared_ptr<bool> aliveFlag);
	void run() override;
};

class Consumer {
	EMMLogger::Logger &logger;
	EMMTaskQueue::ConcurrentTaskQueue &taskQueue;
	std::shared_ptr<bool> isAlive{std::make_shared<bool>(true)};
	int resultCode = -1;

public:
	Consumer(EMMLogger::Logger &logger, EMMTaskQueue::ConcurrentTaskQueue &queue);

	void operator()();

	int getResultCode();

	void makeDead();

	Stoper createStoper();

	std::shared_ptr<bool> consumerAliveFlag();

};


class Producer {
	EMMLogger::Logger &logger;
	EMMTaskQueue::ConcurrentTaskQueue &taskQueue;
	std::shared_ptr<bool> isAlive{std::make_shared<bool>(true)};
	std::shared_ptr<bool> consumerAliveFlag;
public:
	Producer(EMMLogger::Logger &logger, EMMTaskQueue::ConcurrentTaskQueue &taskQueue, std::shared_ptr<bool> consumerAliveFlag);
	
	void operator()();

	void makeDead();

	Stoper createStoper();
};

}	//namespace

#endif
