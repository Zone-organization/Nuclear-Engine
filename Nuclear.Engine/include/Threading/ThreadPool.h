#pragma once
#include <NE_Common.h>
#include <Threading\Task.h>
#include <Threading\ConcurrentTaskQueue.h>
#include <mutex>
#include <condition_variable>

namespace Nuclear
{
    namespace Threading
    {
        class NEAPI ThreadPool
        {
        public:
            ThreadPool();

            void Initialize(Uint32 threadscount);
            void AddTask(Task* task);
            void Shutdown();

        protected:
            std::vector<std::jthread> mThreads;
            ConcurrentTaskQueue<Task> mTasks;
            std::mutex mTasksMutex;
            std::condition_variable mTaskConditionVar;
            std::atomic<bool> mShouldClose;

           void ThreadFunc();
        private:
        };

    }
}