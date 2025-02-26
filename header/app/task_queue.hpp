#ifndef EMM_COMMAND_QUEUE_HEADER
#define EMM_COMMAND_QUEUE_HEADER

#include <queue>
#include <memory>
#include <mutex>
#include <condition_variable>
#include "lib/lib.hpp"
#include "app/task.hpp"

namespace EMMTaskQueue {

class ConcurrentTaskQueue {
	std::queue<std::unique_ptr<EMMTask::ITask>> queue;
	std::mutex mutex;
	std::condition_variable pop_conditional;
	std::condition_variable push_conditional;
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
