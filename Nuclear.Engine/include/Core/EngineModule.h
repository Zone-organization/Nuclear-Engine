#pragma once
#include <NE_Common.h>

namespace Nuclear
{
	namespace Core
	{
		template < typename T >
		class NEAPI EngineModule
		{
		public:
			FORCE_INLINE static T& Get()
			{
				static T instance;
				return instance;
			}

			virtual void Shutdown() = 0;

			EngineModule(const EngineModule&) = delete;
			EngineModule& operator= (const EngineModule) = delete;

		protected:
			EngineModule() { };
			virtual ~EngineModule() { }
		};
	}
}