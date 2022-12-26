#pragma once
#include <NE_Common.h>
#include <Threading/Task.h>

namespace Nuclear
{
    namespace Threading
    {

        class NEAPI DelegateTask : public Task
        {
        public:
            DelegateTask(std::function<bool()>& del)
                : mDelegate(std::move(del))
            {

            }
            DelegateTask(std::function<void()>& del)
                : mDelegateNoReturn(std::move(del))
            {

            }        

        protected:
            std::function<bool()> mDelegate;
            std::function<void()> mDelegateNoReturn;

            bool OnRunning() override
            {
                if (mDelegate)
                {
                    return mDelegate();
                }

                if (mDelegateNoReturn)
                {
                    mDelegateNoReturn();
                    return true;
                }

                return true;
            }
        };

    }
}