#pragma once
#include <NE_Common.h>
#include <Interfaces/NonCopyable.h>
#include <functional>

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
                Queued,
                Running, 
                Failed,
                Canceled,
                Finished
           };

           void Start();

           void Cancel();

           void Execute();
        protected:   

            //Actual task
            virtual bool OnRunning() = 0;

            virtual void OnCreate();
            virtual void OnFail();
            virtual void OnCancel();
            virtual void OnFinish();

            State mState = State::Created;
        };

    }
}