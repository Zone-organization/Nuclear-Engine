#pragma once
#include <NE_Common.h>

namespace Nuclear
{
	namespace Audio
	{

		class NEAPI AudioBackend
		{
		public:
			virtual bool Initialize() = 0;

			virtual void Shutdown() = 0;
		};
	}
}