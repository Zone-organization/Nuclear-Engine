#pragma once
namespace Nuclear
{
	namespace Interfaces
    {
        template < typename T >
        class Singleton {
        public:
            static T& GetInstance() {
                static T instance;

                return instance;
            }

            Singleton(const Singleton&) = delete;
            Singleton& operator= (const Singleton) = delete;

        protected:
            Singleton() { };
            virtual ~Singleton() { }
        };
	}
}