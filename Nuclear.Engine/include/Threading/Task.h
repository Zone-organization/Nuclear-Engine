#pragma once
#include <NE_Common.h>
#include <Interfaces/NonCopyable.h>
#include <functional>
#include <atomic>
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

           bool IsFailed() const;
           bool IsCanceled() const;
           bool IsQueued() const;
           bool IsRunning() const;
           bool IsFinished() const;
           bool IsEnded() const;
           bool IsCancelRequested() const;

           State GetState();

        protected:   

            //Actual task
            virtual bool OnRunning() = 0;

            virtual void OnCreate();
            virtual void OnFail();
            virtual void OnCancel();
            virtual void OnFinish();
            virtual void OnEnd();

            State mState = State::Created;
            std::atomic<bool> mCancelRequested;
        };

    }
}