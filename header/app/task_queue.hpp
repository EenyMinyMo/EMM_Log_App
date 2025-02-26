#ifndef EMM_COMMAND_QUEUE_HEADER
#define EMM_COMMAND_QUEUE_HEADER

#include <queue>
#include <memory>
#include <mutex>
#include <condition_variable>
#include "lib/lib.hpp"
#include "app/task.hpp"

namespace EMMTaskQueue {

/*
	Многопоточная очередь для задач-наследников EMMTask::ITask.
	В конструкторе очереди задается максимальное кол-во элементов очереди.
	Т.к. в очередь может писать несколько потоков, элементы могут поступать быстрее, чем будут обработаны -> нужно разумно ограничивать поступающие задачи.
	У очереди всего 3 метода для взаимодействия:
		- блокирующий метод popAndWait() - блокирует поток, если очередь пуста.
		- блокирующий метод pushAndWait() - блокирует поток, если очередь полна.
		- неблокирующий метод push() - если очередь полна, выбрасывает исключение QueueOverflowException. 
*/
class ConcurrentTaskQueue {
	std::queue<std::unique_ptr<EMMTask::ITask>> queue;
	std::mutex mutex;
	std::condition_variable pop_conditional;	//условная переменная для блокировки в pop
	std::condition_variable push_conditional;	//условная переменная для блокировки в push
	const int maxItems;
public:
	ConcurrentTaskQueue(int maxItems);
	ConcurrentTaskQueue(const ConcurrentTaskQueue&) = delete;
	ConcurrentTaskQueue& operator=(const ConcurrentTaskQueue&) = delete;

	void push(std::unique_ptr<EMMTask::ITask>);
	void pushAndWait(std::unique_ptr<EMMTask::ITask>);
	std::unique_ptr<EMMTask::ITask> popAndWait();
};

class QueueOverflowException : public std::runtime_error {
public:
	explicit QueueOverflowException(const char* mess);
};

}	//namespace

#endif
