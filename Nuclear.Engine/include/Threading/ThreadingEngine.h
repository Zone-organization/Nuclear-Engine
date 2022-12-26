#pragma once
#include <NE_Common.h>
#include <Threading/ThreadPool.h>

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

            ThreadPool& GetThreadPool();
        protected:
            std::thread::id mMainThreadID;
            ThreadPool mMainPool;

        private:
            ThreadingEngine();
        };

    }
}