#pragma once
#include <Base\NE_Common.h>

namespace NuclearEngine
{
	namespace Scripting
	{
		class NEAPI Script
		{
		public:
			Script();
			~Script();

			bool Initialize();

			void Shutdown();

		};
	}
}