#pragma once
#include <Engine\Graphics\Skybox.h>
#include <Base\Math\Math.h>
#include <Engine\Graphics\Context.h>
#include <Engine\Graphics\Color.h>

namespace NuclearEngine
{
	namespace Graphics 
	{		
		class NEAPI Background 
		{
		public:
			Background();
			~Background();
			
			//void BeginFrame(Uint8 flags = (ClearColorBuffer | ClearDepthBuffer));
			void EndFrame();

			bool mUseSkybox;
			Color mClearColor;
			Graphics::Skybox* mSkybox;
		};
	}
}