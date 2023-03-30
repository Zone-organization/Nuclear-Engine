#include <Threading\ThreadPool.h>
#include <Utilities/Logger.h>

namespace Nuclear
{
	namespace Threading
	{
		ThreadPool::ThreadPool()
		{

		}
		void ThreadPool::ThreadFunc()
		{
			Task* task;

			while (!mShouldClose.load())
			{
				if (mTasks.try_dequeue(task))
				{
					task->Execute();
				}
				else
				{
					std::unique_lock lk(mTasksMutex);
					mTaskConditionVar.wait(lk);
				}
			}
		}

		void ThreadPool::Initialize(Uint32 threadscount)
		{

			for (Uint32 i = 0; i < threadscount; i++)
			{

				std::jthread thread([this](std::stop_token stoken) { if (stoken.stop_requested()) {
					std::stringstream ss;
					ss << std::this_thread::get_id();
					NUCLEAR_INFO("[ThreadPool] Stopping Thread: {0}", ss.str());
					return;
				} this->ThreadFunc(); });

				mThreads.push_back(std::move(thread));
			}
		}
		void ThreadPool::AddTask(Task* task)
		{
			mTasks.Add(task);
			mTaskConditionVar.notify_one();
		}
		void ThreadPool::Shutdown()
		{  
			mShouldClose.store(true);
			mTaskConditionVar.notify_all();

			for (auto& thread : mThreads)
			{
				thread.request_stop();
			}
			mThreads.clear();
		}
	}
}