#include <Engine\Systems\LightingSubSystem.h>
#include <Engine\Graphics\Context.h>

namespace NuclearEngine
{
	namespace Systems
	{
		bool LightingSubSystem::RequiresBaking()
		{
			if (Baked_DirLights_Size == DirLights.size() && Baked_PointLights_Size == PointLights.size() && Baked_SpotLights_Size == SpotLights.size())
				return true;

			return false;
		}
		void LightingSubSystem::BakeBuffer()
		{
			if (HasbeenBakedBefore)
			{
				if (!RequiresBaking())
				{
					Log.Warning("[LightingSubSystem] No need for baking the sub system!\n");
				}
			}

			HasbeenBakedBefore = true;

			NE_Light_CB_Size = sizeof(Math::Vector4);
			NUM_OF_LIGHT_VECS = 1;
			Baked_DirLights_Size = DirLights.size();
			Baked_PointLights_Size = PointLights.size();
			Baked_SpotLights_Size = SpotLights.size();

			if (DirLights.size() > 0)
			{
				NE_Light_CB_Size = NE_Light_CB_Size + (DirLights.size() * sizeof(Components::Internal::Shader_DirLight_Struct));
				NUM_OF_LIGHT_VECS = NUM_OF_LIGHT_VECS + (DirLights.size() * 2);
			}
			if (PointLights.size() > 0)
			{
				NE_Light_CB_Size = NE_Light_CB_Size + (PointLights.size() * sizeof(Components::Internal::Shader_PointLight_Struct));
				NUM_OF_LIGHT_VECS = NUM_OF_LIGHT_VECS + (PointLights.size() * 3);
			}
			if (SpotLights.size() > 0)
			{
				NE_Light_CB_Size = NE_Light_CB_Size + (SpotLights.size() * sizeof(Components::Internal::Shader_SpotLight_Struct));
				NUM_OF_LIGHT_VECS = NUM_OF_LIGHT_VECS + (SpotLights.size() * 5);
			}

			if (mPSLightCB.RawPtr() != nullptr)
			{
				mPSLightCB.Release();
			}
			BufferDesc CBDesc;
			CBDesc.Name = "LightCB";
			CBDesc.Size = static_cast<Uint32>(NE_Light_CB_Size);
			CBDesc.Usage = USAGE_DYNAMIC;
			CBDesc.BindFlags = BIND_UNIFORM_BUFFER;
			CBDesc.CPUAccessFlags = CPU_ACCESS_WRITE;
			BufferData DATA;
			Graphics::Context::GetDevice()->CreateBuffer(CBDesc, &DATA, mPSLightCB.RawDblPtr());
		}
		void LightingSubSystem::UpdateBuffer(const Math::Vector4& CameraPos)
		{
			std::vector<Math::Vector4> LightsBuffer;
			LightsBuffer.reserve(NUM_OF_LIGHT_VECS);

			LightsBuffer.push_back(CameraPos);

			for (size_t i = 0; i < DirLights.size(); i++)
			{
				LightsBuffer.push_back(DirLights[i]->GetInternalData().Direction);
				LightsBuffer.push_back(DirLights[i]->GetInternalData().Color);
			}
			for (size_t i = 0; i < PointLights.size(); i++)
			{
				LightsBuffer.push_back(PointLights[i]->GetInternalData().Position);
				LightsBuffer.push_back(PointLights[i]->GetInternalData().Intensity_Attenuation);
				LightsBuffer.push_back(PointLights[i]->GetInternalData().Color);

			}
			for (size_t i = 0; i < SpotLights.size(); i++)
			{
				LightsBuffer.push_back(SpotLights[i]->GetInternalData().Position);
				LightsBuffer.push_back(SpotLights[i]->GetInternalData().Direction);
				LightsBuffer.push_back(SpotLights[i]->GetInternalData().Intensity_Attenuation);
				LightsBuffer.push_back(SpotLights[i]->GetInternalData().InnerCutOf_OuterCutoff);
				LightsBuffer.push_back(SpotLights[i]->GetInternalData().Color);
			}

			PVoid data;
			Graphics::Context::GetContext()->MapBuffer(mPSLightCB, MAP_WRITE, MAP_FLAG_DISCARD, (PVoid&)data);
			data = memcpy(data, LightsBuffer.data(), NE_Light_CB_Size);
			Graphics::Context::GetContext()->UnmapBuffer(mPSLightCB, MAP_WRITE);
		}
	}
}