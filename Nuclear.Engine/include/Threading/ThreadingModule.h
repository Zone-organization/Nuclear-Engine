#pragma once
#include <NE_Common.h>
#include <Threading/ThreadPool.h>
#include <Threading/MainThreadTask.h>

namespace Nuclear
{
    namespace Threading
    {
        class NEAPI ThreadingModule {
        public:
            ThreadingModule(ThreadingModule const&) = delete;
            void operator=(ThreadingModule const&) = delete;

            static ThreadingModule& GetInstance();

            bool Initialize();
            void Shutdown();
            void AddTask(Task* task);
            void AddMainThreadTask(MainThreadTask* task);

            ThreadPool& GetThreadPool();
            void ExecuteMainThreadTasks(Uint32 count);
            std::vector<MainThreadTask*>& GetMainTasks();
        protected:
            std::thread::id mMainThreadID;
            ThreadPool mMainPool;
            std::vector<MainThreadTask*> mMainTTasks;
            std::mutex mMainTTasksMutex;
        private:
            ThreadingModule();
        };

    }
}