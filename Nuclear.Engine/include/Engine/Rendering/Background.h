#pragma once
#include <Engine\Rendering\Skybox.h>
#include <Core\Math\Math.h>
#include <Engine\Graphics\Context.h>
#include <Engine\Graphics\Color.h>

namespace Nuclear
{
	namespace Rendering
	{		
		class NEAPI Background 
		{
		public:
			Background();
			~Background();
			
			Graphics::Color mClearColor;

			void SetSkybox(Skybox* skybox);
			Skybox* GetSkybox();

		private:
			Skybox* mSkybox;
		};
	}
}