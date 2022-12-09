#include <Threading\ThreadPool.h>

namespace Nuclear
{
	namespace Threading
	{
        inline ThreadPool::ThreadPool()
        {
        }
        // the constructor just launches some amount of workers
        inline ThreadPool::ThreadPool(size_t threads)
        {
            Initalize(threads);
        }

        // the destructor joins all threads
        inline ThreadPool::~ThreadPool()
        {
            {
                std::unique_lock<std::mutex> lock(queue_mutex);
                stop = true;
            }
            condition.notify_all();
            for (std::thread& worker : workers)
                worker.join();
        }

        void ThreadPool::Initalize(size_t threads)
        {
            stop = false;
            for (size_t i = 0; i < threads; ++i)
                workers.emplace_back(
                    [this]
                    {
                        for (;;)
                        {
                            std::function<void()> task;
                            {
                                std::unique_lock<std::mutex> lock(this->queue_mutex);
                                this->condition.wait(lock,
                                    [this] { return this->stop || !this->tasks.empty(); });
                                if (this->stop && this->tasks.empty())
                                    return;
                                task = std::move(this->tasks.front());
                                this->tasks.pop();
                            }

                            task();
                        }
                    }
                    );
        }
	}
}