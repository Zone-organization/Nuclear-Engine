#include "Engine\Components\DirLightComponent.h"

namespace NuclearEngine
{
	namespace Components
	{
		DirLightComponent::DirLightComponent()
		{
			data.Direction = Math::Vector4(-0.5f, -0.5f, -0.5f, 1.0f);
			data.Color = Math::Vector4(0.5f, 0.5f, 0.5f, 1.0f);
		}
		DirLightComponent::~DirLightComponent()
		{
		}
		Graphics::Color DirLightComponent::GetColor()
		{
			return Graphics::Color(data.Color.r, data.Color.g, data.Color.b, data.Color.w);
		}
		void DirLightComponent::SetColor(Graphics::Color color)
		{
			data.Color = Math::Vector4(color.r, color.g, color.b, 1.0f);
		}
		Math::Vector3 DirLightComponent::GetDirection()
		{
			return Math::Vector3(data.Direction);
		}
		void DirLightComponent::SetDirection(Math::Vector3 dir)
		{
			data.Direction = Math::Vector4(dir, 1.0f);
		}
		Internal::Shader_DirLight_Struct DirLightComponent::GetInternalData()
		{
			return data;
		}
	}
}