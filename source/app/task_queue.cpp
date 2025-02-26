#include <queue>
#include <memory>
#include <mutex>
#include <condition_variable>
#include "lib/lib.hpp"
#include "app/task.hpp"
#include "app/task_queue.hpp"

namespace EMMTaskQueue {

template <typename T>
class queue_emtpy_predicate {
	std::queue<T> &queue;
public:
	queue_emtpy_predicate(std::queue<T> &queue) : queue(queue) {}
	bool operator()() {
		return !queue.empty();
	}
};

ConcurrentTaskQueue::ConcurrentTaskQueue(int maxItems) : maxItems(maxItems > 1 ? maxItems : 1), currentItems(0) {}

void ConcurrentTaskQueue::push(std::unique_ptr<EMMTask::ITask> task) {
	{
		std::lock_guard<std::mutex> lockm(mutex);
		if (currentItems == maxItems) {
			throw QueueOverflowException("Concurrent task queue overflowed");
		}
		queue.push(std::move(task));
		currentItems++;
	}
	mutex_conditional.notify_one();
}

std::unique_ptr<EMMTask::ITask> ConcurrentTaskQueue::pop() {
	std::unique_lock<std::mutex> lockm(mutex);
	mutex_conditional.wait(lockm, queue_emtpy_predicate(queue));

	std::unique_ptr<EMMTask::ITask> task = std::move(queue.front());
	queue.pop();
	currentItems--;

	return task;
}

QueueOverflowException::QueueOverflowException(const char* mess) : std::runtime_error(mess) {}

}	//namespace
