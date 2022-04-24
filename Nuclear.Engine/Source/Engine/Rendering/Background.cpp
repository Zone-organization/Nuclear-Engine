#include <Engine\Rendering\Background.h>

namespace Nuclear
{
	namespace Rendering
	{
		Background::Background()
		{
			//mUseSkybox = false;
			mClearColor = Graphics::Color(0.0f, 0.0f, 0.0f, 1.0f);
			mSkybox = nullptr;
		}
		Background::~Background()
		{
		}

		void Background::SetSkybox(Skybox* skybox)
		{
			mSkybox = skybox;
		}

		Skybox* Background::GetSkybox()
		{
			return mSkybox;
		}

		//void Background::EndFrame()
		//{
		//	if (mUseSkybox == true && mSkybox != nullptr)
		//	{
		//		mSkybox->Render();
		//	}
		//}
	
	}
}