#pragma once
#include <NE_Common.h>
#include <Threading/ThreadPool.h>
#include <Threading/MainThreadTask.h>

namespace Nuclear
{
    namespace Threading
    {
        class NEAPI ThreadingEngine {
        public:
            ThreadingEngine(ThreadingEngine const&) = delete;
            void operator=(ThreadingEngine const&) = delete;

            static ThreadingEngine& GetInstance();

            bool Initialize();
            void AddTask(Task* task);
            void AddMainThreadTask(MainThreadTask* task);

            ThreadPool& GetThreadPool();
            std::vector<MainThreadTask*>& GetMainTasks();
        protected:
            std::thread::id mMainThreadID;
            ThreadPool mMainPool;
            std::vector<MainThreadTask*> mMainTTasks;
        private:
            ThreadingEngine();
        };

    }
}