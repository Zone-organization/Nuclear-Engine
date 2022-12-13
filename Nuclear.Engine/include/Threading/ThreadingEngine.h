#pragma once
#include <NE_Common.h>
#include <vector>
#include <queue>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <stdexcept>
#include <type_traits>

namespace Nuclear
{
    namespace Threading
    {
        class NEAPI ThreadingEngine {
        public:
            ThreadingEngine(ThreadingEngine const&) = delete;
            void operator=(ThreadingEngine const&) = delete;

            static ThreadingEngine& GetInstance();

            void Initialize();

        protected:
            std::thread::id mMainThreadID;


        private:
            ThreadingEngine();
        };

    }
}