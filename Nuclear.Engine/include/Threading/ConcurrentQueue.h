#pragma once
#include <NE_Common.h>

#define MOODYCAMEL_EXCEPTIONS_ENABLED 0
#include <ThirdParty/concurrentqueue.h>

namespace Nuclear
{
    namespace Threading
    {
        template<typename T>
        class ConcurrentQueue : public moodycamel::ConcurrentQueue<T>
        {
        public:

            inline Int32 Size() const
            {
                return static_cast<Int32>(moodycamel::ConcurrentQueue<T>::size_approx());
            }

            inline void Add(T& item)
            {
                enqueue(item);
            }
        };
    }
}