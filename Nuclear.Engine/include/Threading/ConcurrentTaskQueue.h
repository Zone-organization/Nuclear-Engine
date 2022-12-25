#pragma once
#include <Threading/ConcurrentQueue.h>
#include <Threading/Task.h>

namespace Nuclear
{
    namespace Threading
    {
        template<typename T = Task>
        class ConcurrentTaskQueue : public ConcurrentQueue<T*>
        {
        public:

            inline void Add(T* item)
            {
                ConcurrentQueue<T*>::enqueue(item);
            }

        };
    }
}