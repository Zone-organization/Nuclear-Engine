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

            NUCLEAR_INFO("[ThreadingEngine] ThreadingEngine has been initialized succesfully!");
            return true;
        }
        void ThreadingEngine::Shutdown()
        {
            mMainPool.Shutdown();
        }
        void ThreadingEngine::AddTask(Task* task)
        {
            mMainPool.AddTask(task);
        }
        void ThreadingEngine::AddMainThreadTask(MainThreadTask* task)
        {
            mMainTTasksMutex.lock();
            mMainTTasks.push_back(task);
            mMainTTasksMutex.unlock();
        }
        ThreadPool& ThreadingEngine::GetThreadPool()
        {
            return mMainPool;
        }
        void ThreadingEngine::ExecuteMainThreadTasks(Uint32 count)
        {
            for (Uint32 i = 0; i < mMainTTasks.size() && i < count; i++)
            {
                mMainTTasks.at(i)->Execute();
                mMainTTasks.erase(mMainTTasks.begin() + i);
            }
         
        }
        std::vector<MainThreadTask*>& ThreadingEngine::GetMainTasks()
        {
            return mMainTTasks;
        }
    }
}