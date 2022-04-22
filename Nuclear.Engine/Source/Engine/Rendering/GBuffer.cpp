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
			for (auto& i : mDesc.mRTDescs)
			{
				Graphics::RenderTarget newrt;
				i.Width = width;
				i.Height = height;
				newrt.Create(i);
				mRenderTargets.push_back(newrt);
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

			//ImGui::Begin("GBUFFER");
			//ImGui::Image(mGBuffer.mPositonBuffer->GetDefaultView(TEXTURE_VIEW_SHADER_RESOURCE), { 256.f,256.f });
			//ImGui::SameLine();
			//ImGui::Image(mGBuffer.mNormalBuffer->GetDefaultView(TEXTURE_VIEW_SHADER_RESOURCE), { 256.f,256.f });
			//ImGui::SameLine();
			//ImGui::Image(mGBuffer.mAlbedoBuffer->GetDefaultView(TEXTURE_VIEW_SHADER_RESOURCE), { 256.f,256.f });

		}
	}
}