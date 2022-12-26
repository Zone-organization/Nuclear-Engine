#include <Threading\ThreadPool.h>

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

				std::thread thread([this] { this->ThreadFunc(); });

				mThreads.push_back(std::move(thread));
			}
		}
		void ThreadPool::AddTask(Task* task)
		{
			mTasks.Add(task);
			mTaskConditionVar.notify_one();
		}
	}
}