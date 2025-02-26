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
	std::condition_variable mutex_conditional;
	const int maxItems;
	int currentItems;
public:
	ConcurrentTaskQueue(int maxItems);
	ConcurrentTaskQueue(const ConcurrentTaskQueue&) = delete;
	ConcurrentTaskQueue& operator=(const ConcurrentTaskQueue&) = delete;

	void push(std::unique_ptr<EMMTask::ITask>);
	std::unique_ptr<EMMTask::ITask> pop();
};

}	//namespace

#endif