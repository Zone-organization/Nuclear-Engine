#pragma once
#include "Graphics\Skybox.h"
#include <Math\Math.h>
#include <Graphics\API\Context.h>
#include <Graphics\Color.h>
namespace NuclearEngine
{
	namespace Graphics 
	{		
		class NEAPI Background 
		{
		public:
			Background();
			~Background();
			
			void BeginFrame(Uint8 flags = (ClearColorBuffer | ClearDepthBuffer));
			void EndFrame();

			bool mUseSkybox;
			Color mClearColor;
			Graphics::Skybox* mSkybox;
		};
	}
}