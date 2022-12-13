#include "Threading/ThreadingEngine.h"

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
        void ThreadingEngine::Initialize()
        {
            mMainThreadID = std::this_thread::get_id();
        }
    }
}