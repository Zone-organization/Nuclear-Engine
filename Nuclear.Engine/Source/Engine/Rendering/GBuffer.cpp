#include "Engine/Rendering/GBuffer.h"
#include <Engine/Graphics/ImGui.h>

namespace Nuclear
{
	namespace Rendering
	{
		void GBuffer::Initialize(const GBufferDesc& desc)
		{
			mDesc = desc;
		}
		void GBuffer::Bake(Uint32 width, Uint32 height)
		{
			mRenderTargets.clear();
			for (auto& i : mDesc.mRTDescs)
			{
				Graphics::RenderTarget newrt;
				i.Width = width;
				i.Height = height;
				newrt.Create(i);
				mRenderTargets.push_back(newrt);
			}
		}
		void GBuffer::Resize(Uint32 width, Uint32 height)
		{
			for (auto& i : mRenderTargets)
			{
				i.Resize(width, height);
			}
		}
		void GBuffer::DebugIMGUI()
		{   	
			ImGui::Begin("GBUFFER");

			for (int i = 0; i < mRenderTargets.size(); i++)
			{			
				ImGui::Image(mRenderTargets.at(i).GetShaderRTV(), { 256.f,256.f });
				if (i % 2 == 0)
				{
					ImGui::SameLine();
				}
			}
			ImGui::End();
		}
	}
}