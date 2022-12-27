#include <Threading/Task.h>

namespace Nuclear
{
    namespace Threading
    {
        void Task::Start()
        {
            //TODO
        }
        void Task::Cancel()
        {
            std::atomic_thread_fence(std::memory_order_acq_rel);
            std::atomic_store_explicit(&mCancelRequested, true, std::memory_order_acq_rel);
          //  mCancelRequested.store(true);
            OnCancel();
            //TODO

        }

        void Task::Execute()
        {
            if (IsCanceled())
                return;


            mState = State::Running;
            bool result = OnRunning();


            if (IsCancelRequested())
            {
                mState = State::Canceled;
            }
            else if (result)
                OnFinish();
            else
                OnFail();

        }

        bool Task::IsFailed() const
        {
            return mState == State::Failed;
        }


        bool Task::IsCanceled() const
        {
            return mState == State::Canceled;
        }

        bool Task::IsQueued() const
        {
            return mState == State::Queued;
        }

        bool Task::IsRunning() const
        {
            return mState == State::Running;
        }

        bool Task::IsFinished() const
        {

            return mState == State::Finished;
        }

        bool Task::IsEnded() const
        {
            return (mState == State::Failed || mState == State::Canceled || mState == State::Finished);
        }


        bool Task::IsCancelRequested() const
        {
            return mCancelRequested.load();
        }

        Task::State Task::GetState()
        {
            return mState;
        }

        void Task::OnCreate()
        {
            mState = State::Created;

        }
        void Task::OnFail()
        {
            mState = State::Failed;

        }
        void Task::OnCancel()
        {
            mState = State::Canceled;

        }
        void Task::OnFinish()
        {
            mState = State::Finished;
        }
    }
}