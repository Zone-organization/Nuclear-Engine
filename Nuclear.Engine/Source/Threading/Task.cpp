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
            //TODO
        }
        void Task::Execute()
        {
            mState = State::Running;

            bool result = OnRunning();


            if (result)
                OnFinish();
            else
                OnFail();

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