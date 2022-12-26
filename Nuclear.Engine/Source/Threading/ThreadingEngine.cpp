#include "Threading/ThreadingEngine.h"
#include <Utilities/Logger.h>
namespace Nuclear
{
    namespace Threading
    {
        ThreadingEngine::ThreadingEngine()
        {

        }
        ThreadingEngine& ThreadingEngine::GetInstance()
        {
            static ThreadingEngine instance;

            return instance;
        }
        bool ThreadingEngine::Initialize()
        {
            mMainThreadID = std::this_thread::get_id();

            //TODO: Clamp
            auto allowedthreads = std::thread::hardware_concurrency() - 1;
            mMainPool.Initialize(allowedthreads);

            NUCLEAR_INFO("[ThreadingEngine] ThreadingEngine has been initalized succesfully!");
            return true;
        }
        ThreadPool& ThreadingEngine::GetThreadPool()
        {
            return mMainPool;
        }
    }
}