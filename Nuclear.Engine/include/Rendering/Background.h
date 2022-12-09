#pragma once
#include <Rendering\Skybox.h>
#include <Math\Math.h>
#include <Graphics\Context.h>
#include <Graphics\Color.h>

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