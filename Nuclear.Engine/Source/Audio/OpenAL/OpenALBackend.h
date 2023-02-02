#pragma once
#include <Audio/AudioBackend.h>

namespace Nuclear
{
	namespace Audio
	{

		class NEAPI OpenALBackend : public AudioBackend
		{
		public:
			bool Initialize() override;

			void Shutdown() override;
		};
	}
}