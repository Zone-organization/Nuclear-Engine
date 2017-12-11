#pragma once
#include "NE_Common.h"

namespace NuclearEngine {
	namespace Core {
		namespace Systems {
			class NEAPI AudioSystem {
			public:
				AudioSystem(AudioAPIs TheAPI);
				~AudioSystem();

				bool Initialize();

			private:
				bool InitOAL();
				bool InitDXA();
			};
		}
	}
}