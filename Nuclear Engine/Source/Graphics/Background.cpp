#include <Graphics\Background.h>

namespace NuclearEngine
{
	namespace Graphics
	{
		Background::Background()
		{
			mUseSkybox = false;
			mClearColor = Color(0.0f, 0.0f, 0.0f, 1.0f);
			mSkybox = nullptr;
		}
		Background::~Background()
		{
		}

		void Background::BeginFrame(Uint8 flags)
		{
			Graphics::API::Context::Clear(mClearColor, flags);
		}

		void Background::EndFrame()
		{
			if (mUseSkybox == true && mSkybox != nullptr)
			{
				mSkybox->Render();
			}
		}
	
	}
}