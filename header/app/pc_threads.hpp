#ifndef EMM_PC_THREADS
#define EMM_PC_THREADS

#include <iostream>
#include <string>
#include <memory>
#include <ctime>
#include "lib/lib.hpp"
#include "app/task.hpp"
#include "app/task_queue.hpp"

namespace EMMProdCons {

//строка для парсинга уровня логирования в логе
static const char* logLevelDecodeString = "-ll=%d";

/*
	Класс задачи, переводящие флаг жизни производителя-потребителя в false.
*/
class Stoper : public EMMTask::ITask {
	std::shared_ptr<bool> aliveFlag;
public:
	Stoper(std::shared_ptr<bool> aliveFlag);
	void run() override;
};

/*
	Потребитель логов.
	Получает таски из ConcurrentTaskQueue и выполняет их.
	В случае возникновения исключения или перевода флага жизни в false прекращает работу.
	Убивать потребителя нужно через задачу Stoper, которую он возвращает.

	У этого класса есть особенности - копии получают один и тот же флаг жизни.
	Единовременно должна исполняться одна копия.
	Такое поведение нужно, потому что thread принимает объект по значению, иначе получить флаг жизни у копии не выйдет.
*/
class Consumer {
	EMMTaskQueue::ConcurrentTaskQueue &taskQueue;
	std::shared_ptr<bool> isAlive{std::make_shared<bool>(false)};

public:
	Consumer(EMMTaskQueue::ConcurrentTaskQueue &queue);

	void operator()();

	void makeDead();

	Stoper createStoper();

	std::shared_ptr<bool> consumerAliveFlag();

	~Consumer();
};


/*
	Производитель логов.
	Считывает данные с консоли и отправляет задачи с тасками в потребителя через очередь.
	Должен отслеживать состояние потребителя, чтобы иметь возможность закончить работу в случае преждевременного конца работы потребителя.
*/
class Producer {
	EMMLogger::Logger &logger;
	EMMTaskQueue::ConcurrentTaskQueue &taskQueue;
	std::shared_ptr<bool> isAlive{std::make_shared<bool>(false)};
	std::shared_ptr<bool> consumerAliveFlag;	//состояние соответствующего потребителя
public:
	Producer(EMMLogger::Logger &logger, EMMTaskQueue::ConcurrentTaskQueue &taskQueue, std::shared_ptr<bool> consumerAliveFlag);
	
	void operator()();

	void makeDead();

	Stoper createStoper();	//мб не стоило для производителя делать стопер

	~Producer();
};

}	//namespace

#endif
