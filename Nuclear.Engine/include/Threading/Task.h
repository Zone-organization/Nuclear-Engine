#pragma once
#include <NE_Common.h>
#include <Interfaces/NonCopyable.h>

namespace Nuclear
{
    namespace Threading
    {
       
        class NEAPI Task : public Interfaces::NonCopyable
        {
        public:
            enum class State : Uint8
            {
                Created,
                Running, 
                Failed,
                Canceled,
                Finished
           };

        protected:
            virtual void OnCreate();
            virtual void OnRunning();
            virtual void OnFail();
            virtual void OnCancel();
            virtual void OnFinish();

            State mState = State::Created;
        };

    }
}